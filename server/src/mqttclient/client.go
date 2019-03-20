package mqttclient

import (
	"fmt"
	"log"
	"net/url"
	"regexp"
	"strings"
	"time"

	"github.com/vchatchai/Farm/server/src/db"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

type Message struct {
	Time     time.Time
	Topic    string
	FarmName string
	Type     string
	DeviceId string
	Value    string
}

func Connect(clientId string, uri *url.URL) mqtt.Client {
	opts := createClientOptions(clientId, uri)
	client := mqtt.NewClient(opts)
	token := client.Connect()
	for !token.WaitTimeout(3 * time.Second) {
	}
	if err := token.Error(); err != nil {
		log.Fatal(err)
	}
	return client
}

func createClientOptions(clientId string, uri *url.URL) *mqtt.ClientOptions {
	opts := mqtt.NewClientOptions()
	opts.AddBroker(fmt.Sprintf("tcp://%s", uri.Host))
	opts.SetUsername(uri.User.Username())
	password, _ := uri.User.Password()
	opts.SetPassword(password)
	opts.SetClientID(clientId)
	return opts
}

const INSERT = "INSERT INTO MESSAGE(Time,Topic,FarmName,Type,DeviceId,Value) VALUES(?,?,?,?,?,?) "

func Listen(uri *url.URL, topic string) {
	client := Connect("sub", uri)
	client.Subscribe(topic, 0, func(client mqtt.Client, msg mqtt.Message) {
		topic := msg.Topic()
		message := Message{
			time.Now(),
			topic,
			getFarmName(topic),
			getType(topic),
			getDeviceID(topic),
			string(msg.Payload()),
		}
		fmt.Printf("* [%s] %s %v\n", msg.Topic(), string(msg.Payload()), message)

		tx, err := db.DB.Begin()
		if err != nil {
			log.Fatal(err)
		}

		stmt, err := tx.Prepare(INSERT)
		if err != nil {
			tx.Rollback()
		}
		defer stmt.Close()
		_, err = stmt.Exec(message.Time, message.Topic, message.FarmName, message.Type, message.DeviceId, message.Value)

		if err != nil {
			tx.Rollback()
		} else {
			tx.Commit()
		}

	})
}

var farmNameRe = regexp.MustCompile(`^/\w+`)
var deviceIDRe = regexp.MustCompile(`^/\w+`)
var types = []string{"temperature", "valve", "pump", "humidity"}

func getDeviceID(topic string) string {
	result := deviceIDRe.FindString(topic)
	return result
}

func getType(topic string) string {
	for _, t := range types {
		if strings.Contains(topic, t) {
			return t
		}
	}
	return ""
}

func getFarmName(topic string) string {
	result := farmNameRe.FindString(topic)
	return result
}

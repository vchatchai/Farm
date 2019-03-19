package mqttclient

import (
	"fmt"
	"log"
	"net/url"
	"time"

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

func Listen(uri *url.URL, topic string) {
	client := Connect("sub", uri)
	client.Subscribe(topic, 0, func(client mqtt.Client, msg mqtt.Message) {
		topic := msg.Topic()
		message := Message{
			time.Now(),
			topic,
			getFarmName(topic),
			getType(topic),
			getDeviceId(topic),
			string(msg.Payload()),
		}

		fmt.Printf("* [%s] %s %v\n", msg.Topic(), string(msg.Payload()), message)
		/*


			tx, err := db.DB.Begin()
			if err != nil {
				log.Fatal(err)
			}
			_, err = tx.Exec("")
			if err != nil {
				tx.Rollback()
			} else {
				tx.Commit()
			}
		*/
	})
}

func getDeviceId(topic string) string {

	return "DeviceId"
}

func getType(topic string) string {
	return "type"
}

func getFarmName(topic string) string {
	return "name"
}

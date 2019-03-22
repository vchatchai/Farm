package main

import (
	"sync"

	"github.com/vchatchai/Farm/server/src/mqttclient"
	//import the Paho Go MQTT library
	"log"
	"net/url"
	"os"
)

const TOPIC = "#"

var wg sync.WaitGroup

func init() {

}

func main() {

	uri, err := url.Parse(os.Getenv("CLOUDMQTT_URL"))
	if err != nil {
		log.Fatal(err)
	}
	uri, _ = url.Parse("tcp://192.168.1.10:1883")
	// topic := uri.Path[1:len(uri.Path)]
	// if topic == "" {
	// 	topic = "test"
	// }

	go mqttclient.Listen(uri, TOPIC)

	wg.Add(1)
	wg.Wait()

	// client := mqttclient.connect("pub", uri)
	// timer := time.NewTicker(1 * time.Second)
	// for t := range timer.C {
	// 	client.Publish(topic, 0, false, t.String())
	// }
}

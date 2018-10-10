package main

import (
	"github.com/brutella/hc"
	"github.com/brutella/hc/accessory"
	"github.com/brutella/hc/service"
	"github.com/tarm/serial"
	"github.com/urfave/cli"
	"log"
	"os"
	"time"
)

type MessageType byte

const (
	// checking the availability of the desk
	TypeAliveRequest  MessageType = 0x01
	TypeAliveResponse MessageType = 0x02

	// setting the height of the desk
	TypeSetHeightRequest MessageType = 0x03

	// querying the height of the desk
	TypeGetHeightRequest  MessageType = 0x04
	TypeGetHeightResponse MessageType = 0x05

	// stopping the desk
	TypeStopRequest MessageType = 0x06

	// TODO: to be implemented
	TypeGetStatusRequest  MessageType = 0x07
	TypeGetStatusResponse MessageType = 0x08

	// moving the desk
	TypeMoveUpRequest   MessageType = 0x0A
	TypeMoveDownRequest MessageType = 0x0B

	// the desk notifying about a height change
	TypeUpdateHeightEvent MessageType = 0x0C
)

type Message struct {
	Type  MessageType
	Value byte
}

func receiver(c chan<- Message, p *serial.Port) {
	message := make([]byte, 3)
	buf := make([]byte, 1)

	for {
		// shift bytes to left
		message[0] = message[1]
		message[1] = message[2]

		// read new byte
		_, err := p.Read(buf)
		if err != nil {
			log.Fatal(err)
		}

		// append new byte
		message[2] = buf[0]

		// checksum
		if message[0]+message[1] != message[2] {
			continue
		}

		c <- Message{Type: MessageType(message[0]), Value: message[1]}
	}
}

func sender(c <-chan Message, p *serial.Port) {
	message := make([]byte, 3)

	for {
		// get a message
		m := <-c

		log.Println("Message to send", m)

		// fill the message buffer
		message[0] = byte(m.Type)
		message[1] = m.Value

		// calculate the checksum
		message[2] = message[0] + message[1]

		log.Println("Buffer to send", message)

		// write the buffer
		_, err := p.Write(message)
		if err != nil {
			log.Fatal(err)
		}
	}
}

func heightToPercentage(height int) int {
	return (height - 68) / 50
}

func heightPercentageToCentimeters(percentage int) int {
	factor := float64(percentage) / 100.0
	return int(68.0 + 50.0*factor)
}

func setInitialDeskPosition(outgoing chan<- Message, incoming <-chan Message, service *service.Window) {
	time.Sleep(2000 * time.Millisecond)

	outgoing <- Message{Type: TypeGetHeightRequest}
	hi := <-incoming

	log.Println("height is", hi.Value)

	percentage := heightToPercentage(int(hi.Value))

	service.TargetPosition.SetValue(percentage)
	service.CurrentPosition.SetValue(percentage)
}

func startServer(dataPath string) {
	info := accessory.Info{
		Name:         "Office Desk",
		Manufacturer: "David Knezic",
		SerialNumber: "3214-3232-32",
		Model:        "A",
	}

	// sadly, window is the closest thing to a desk in HomeKit
	acc := accessory.New(info, accessory.TypeWindow)
	service := service.NewWindow()

	c := &serial.Config{
		Name: "/dev/ttyAMA0",
		Baud: 9600,
	}

	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	var incoming chan Message = make(chan Message)
	var outgoing chan Message = make(chan Message)

	go receiver(incoming, s)
	go sender(outgoing, s)

	service.TargetPosition.OnValueRemoteUpdate(func(position int) {
		log.Println("Setting desk to", position, "percent height")

		height := heightPercentageToCentimeters(position)

		log.Println("This corresponds to", height, "cm height")

		outgoing <- Message{Type: MessageType(TypeSetHeightRequest), Value: byte(height)}

		service.CurrentPosition.SetValue(position)
	})

	acc.AddService(service.Service)

	config := hc.Config{
		Pin:         "32191123",
		StoragePath: dataPath,
	}

	t, err := hc.NewIPTransport(config, acc)

	if err != nil {
		log.Fatal(err)
	}

	hc.OnTermination(func() {
		t.Stop()
	})

	go setInitialDeskPosition(outgoing, incoming, service)

	t.Start()
}

func main() {
	app := cli.NewApp()

	app.Name = "desk"
	app.Usage = "HomeKit bridge for height-adjustable desks"
	app.Version = "1.0.0"

	app.Flags = []cli.Flag{
		cli.StringFlag{
			Name:   "data",
			Value:  "./desk",
			Usage:  "Save HomeKit data to `PATH`",
			EnvVar: "DESK_DATA_PATH",
		},
	}

	app.Action = func(c *cli.Context) error {
		startServer(c.String("data"))
		return nil
	}

	app.Run(os.Args)
}

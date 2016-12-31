package main

import (
	"github.com/brutella/hc"
	"github.com/brutella/hc/accessory"
	"github.com/brutella/hc/service"
	"github.com/tarm/serial"
	"log"
)

type MessageType byte

const (
	TypeAliveRequest      MessageType = 0x01
	TypeAliveResponse     MessageType = 0x02
	TypeSetHeightRequest  MessageType = 0x03
	TypeGetHeightRequest  MessageType = 0x04
	TypeGetHeightResponse MessageType = 0x05
	TypeStopRequest       MessageType = 0x06
	TypeGetStatusRequest  MessageType = 0x07
	TypeGetStatusResponse MessageType = 0x08
	TypeMoveUpRequest     MessageType = 0x0A
	TypeMoveDownRequest   MessageType = 0x0B
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
		if message[0] + message[1] != message[2] {
			continue
		}

		c <- Message{Type: MessageType(message[0]), Value: message[1]}
	}
}

func sender(c <-chan Message, p *serial.Port) {
	message := make([]byte, 3)

	for {
		// get a message
		m := <- c

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

func main() {
	info := accessory.Info{
		Name: "Desk",
		Manufacturer: "David Knezic",
	}

	acc := accessory.New(info, accessory.TypeWindow)

	service := service.NewWindow()
	service.TargetPosition.SetValue(100)

	c := &serial.Config{Name: "/dev/ttyAMA0", Baud: 9600}
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

		factor := float64(position) / 100.0
		height := byte(80) + byte(100.0 * factor)

		log.Println("This corresponds to", height, "cm height")

		outgoing <- Message{Type: MessageType(TypeSetHeightRequest), Value: height}

		service.CurrentPosition.SetValue(position)
	})

	acc.AddService(service.Service)

	config := hc.Config{
		Pin: "32191123",
	}

	t, err := hc.NewIPTransport(config, acc)

	if err != nil {
		log.Fatal(err)
	}

	hc.OnTermination(func() {
		t.Stop()
	})

	t.Start()
}

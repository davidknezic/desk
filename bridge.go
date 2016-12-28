package main

import (
	"github.com/brutella/hc"
	"github.com/brutella/hc/accessory"
	"github.com/brutella/hc/service"
	"log"
	"fmt"
)

func main() {
	info := accessory.Info{
		Name: "Desk",
		Manufacturer: "David Knezic",
	}

	acc := New(info, TypeWindow)

	service := service.NewWindow()
	service.TargetPosition.SetValue(100)

	service.On.OnValueRemoteUpdate(func(position int) {
		log.Println("Setting desk to %d", position)
	})

	acc.AddService(service.Service)

	config := hc.Config{
		Pin: "32191123"
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

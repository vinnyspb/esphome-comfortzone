#pragma once

#include "rs485_interface.h"
#include "esphome/components/uart/uart.h"
#include "driver/gpio.h"

class EspHomeRS485Interface: public RS485Interface {
public:
    EspHomeRS485Interface(esphome::uart::UARTDevice* uart_device, int de_pin):
        _uart_device(uart_device),
        _de_pin(de_pin) {}

    void begin() {
	    gpio_set_direction((gpio_num_t)_de_pin, GPIO_MODE_OUTPUT);
        enable_receiver_mode();
    }

    int available() {
        return _uart_device->available();
    }

    int read_byte() {
        return _uart_device->read();
    }

    int write_bytes(const void* data, int size) {
        _uart_device->write_array((const uint8_t*)data, size);
        return size;
    }

    void flush() {
        return _uart_device->flush();
    }

    void enable_receiver_mode() {
	    gpio_set_level((gpio_num_t)_de_pin, 0);
    }

    void enable_sender_mode() {
	    gpio_set_level((gpio_num_t)_de_pin, 1);
    }

private:
    esphome::uart::UARTDevice* _uart_device;
    int _de_pin;
};

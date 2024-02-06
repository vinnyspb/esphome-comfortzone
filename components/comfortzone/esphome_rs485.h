#pragma once

#include "rs485_interface.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/gpio.h"

namespace esphome::comfortzone {

class EspHomeRS485Interface: public RS485Interface {
public:
    EspHomeRS485Interface(uart::UARTDevice* uart_device, GPIOPin* re_de_pin):
        uart_device_(uart_device),
        re_de_pin_(re_de_pin) {}

    void begin() {
        re_de_pin_->pin_mode(gpio::FLAG_OUTPUT);
        enable_receiver_mode();
    }

    int available() {
        return uart_device_->available();
    }

    int read_byte() {
        return uart_device_->read();
    }

    int write_bytes(const void* data, int size) {
        uart_device_->write_array((const uint8_t*)data, size);
        return size;
    }

    void flush() {
        return uart_device_->flush();
    }

    void enable_receiver_mode() {
	    re_de_pin_->digital_write(false);
    }

    void enable_sender_mode() {
	    re_de_pin_->digital_write(true);
    }

private:
    uart::UARTDevice* uart_device_;
    GPIOPin* re_de_pin_;
};

}

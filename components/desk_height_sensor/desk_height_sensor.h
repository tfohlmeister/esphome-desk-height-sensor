#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart_device.h"

namespace esphome
{
    namespace desk_height_sensor
    {

        class DeskHeightSensor : public Component, public uart::UARTDevice, public sensor::Sensor
        {
        public:
            DeskHeightSensor() = default;

            void set_uart_parent(uart::UARTComponent *parent)
            {
                this->set_uart_device(parent);
            }

            void setup() override;
            void loop() override;
            float get_setup_priority() const override;

        protected:
            float value{NAN};
            float last_published{-1};
            unsigned long history[5]{};
            int msg_len{0};
            unsigned long msg_type{};
            bool valid{false};

            int hex_to_int(uint8_t s);
            bool is_decimal(uint8_t b);
        };

    } // namespace desk_height_sensor
} // namespace esphome
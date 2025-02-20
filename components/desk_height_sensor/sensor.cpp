#include "esphome.h"
#include "desk_height_sensor.h"
#include <bitset>

namespace esphome
{
    namespace desk_height_sensor
    {

        float DeskHeightSensor::get_setup_priority() const
        {
            return setup_priority::DATA;
        }

        int DeskHeightSensor::hex_to_int(uint8_t s)
        {
            std::bitset<8> b(s);

            if (b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && !b[6])
            {
                return 0;
            }
            // ... rest of your hex_to_int logic ...
            return 0;
        }

        bool DeskHeightSensor::is_decimal(uint8_t b)
        {
            return (b & 0x80) == 0x80;
        }

        void DeskHeightSensor::setup()
        {
            // nothing to do here
        }

        void DeskHeightSensor::loop()
        {
            while (available() > 0)
            {
                uint8_t incomingByte = read();

                // First byte, start of a packet
                if (incomingByte == 0x9b)
                {
                    msg_len = 0;
                    valid = false;
                }

                // ... rest of your loop logic ...

                // End byte
                if (incomingByte == 0x9d)
                {
                    if (value && value != last_published)
                    { // Note: changed lastPublished to last_published to match header
                        publish_state(value);
                        last_published = value;
                    }
                }
            }
        }

    } // namespace desk_height_sensor
} // namespace esphome
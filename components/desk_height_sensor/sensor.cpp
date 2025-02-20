#include "esphome.h"
#include "desk_height_sensor.h"

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
            if (not b[0] && b[1] && b[2] && !b[3] && !b[4] && !b[5] && !b[6])
            {
                return 1;
            }
            if (b[0] && b[1] && !b[2] && b[3] && b[4] && !b[5] && b[6])
            {
                return 2;
            }
            if (b[0] && b[1] && b[2] && b[3] && !b[4] && !b[5] && b[6])
            {
                return 3;
            }
            if (not b[0] && b[1] && b[2] && !b[3] && !b[4] && b[5] && b[6])
            {
                return 4;
            }
            if (b[0] && !b[1] && b[2] && b[3] && !b[4] && b[5] && b[6])
            {
                return 5;
            }
            if (b[0] && !b[1] && b[2] && b[3] && b[4] && b[5] && b[6])
            {
                return 6;
            }
            if (b[0] && b[1] && b[2] && !b[3] && !b[4] && !b[5] && !b[6])
            {
                return 7;
            }
            if (b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && b[6])
            {
                return 8;
            }
            if (b[0] && b[1] && b[2] && b[3] && !b[4] && b[5] && b[6])
            {
                return 9;
            }
            if (!b[0] && !b[1] && !b[2] && !b[3] && !b[4] && !b[5] && b[6])
            {
                return 10;
            }
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

                // Second byte defines the message length
                if (history[0] == 0x9b)
                {
                    msg_len = (int)incomingByte;
                }

                // Third byte is message type
                if (history[1] == 0x9b)
                {
                    msg_type = incomingByte;
                }

                // Fourth byte is first height digit, if msg type 0x12 & msg len 7
                if (history[2] == 0x9b)
                {
                    if (msg_type == 0x12 && msg_len == 7)
                    {
                        if (incomingByte == 0)
                        {
                            // Empty height
                        }
                        else
                        {
                            valid = true;
                        }
                    }
                }

                // Fifth byte is second height digit
                if (history[3] == 0x9b)
                {
                    if (valid == true)
                    {
                        // Process second height digit
                    }
                }

                // Sixth byte is third height digit
                if (history[4] == 0x9b)
                {
                    if (valid == true)
                    {
                        int height1 = hex_to_int(history[1]) * 100;
                        int height2 = hex_to_int(history[0]) * 10;
                        int height3 = hex_to_int(incomingByte);
                        if (height2 == 100)
                        { // check if 'number' is a hyphen
                            // Handle hyphen case
                        }
                        else
                        {
                            float finalHeight = height1 + height2 + height3;
                            if (is_decimal(history[0]))
                            {
                                finalHeight = finalHeight / 10;
                            }
                            value = finalHeight;
                        }
                    }
                }

                // Save byte buffer to history array
                history[4] = history[3];
                history[3] = history[2];
                history[2] = history[1];
                history[1] = history[0];
                history[0] = incomingByte;

                // End byte
                if (incomingByte == 0x9d)
                {
                    if (value && value != last_published)
                    {
                        publish_state(value);
                        last_published = value;
                    }
                }
            }
        }

    } // namespace desk_height_sensor
} // namespace esphome
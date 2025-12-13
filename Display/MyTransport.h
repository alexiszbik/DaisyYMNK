#ifndef MY_TRANSPORT_H
#define MY_TRANSPORT_H /**< & */

#include "per/i2c.h"
#include "per/spi.h"
#include "per/gpio.h"
#include "sys/system.h"

namespace daisy
{
/**
 * I2C Transport for SSD1306 / SSD1309 OLED display devices
 */
class MyTransport
{
  public:
    struct Config
    {
        Config()
        {
            // Intialize using defaults
            Defaults();
        }
        I2CHandle::Config i2c_config;
        uint8_t           i2c_address;
        void              Defaults()
        {
            i2c_config.periph         = I2CHandle::Config::Peripheral::I2C_1;
            i2c_config.speed          = I2CHandle::Config::Speed::I2C_1MHZ;
            i2c_config.mode           = I2CHandle::Config::Mode::I2C_MASTER;
            i2c_config.pin_config.scl = {DSY_GPIOB, 8};
            i2c_config.pin_config.sda = {DSY_GPIOB, 9};
            i2c_address               = 0x3C;
        }
    };
    void Init(const Config& config)
    {
        i2c_address_ = config.i2c_address;
        i2c_.Init(config.i2c_config);
    };
    void SendCommand(uint8_t cmd)
    {
        uint8_t buf[2] = {0X00, cmd};
        i2c_.TransmitBlocking(i2c_address_, buf, 2, 1000);
    };

    void SendData(uint8_t* buff, size_t size)
    {
        for(size_t i = 0; i < size; i++)
        {
            uint8_t buf[2] = {0X40, buff[i]};
            i2c_.TransmitBlocking(i2c_address_, buf, 2, 1000);
        }
    };

    void SendDataPerPixel(uint8_t* buff, size_t pixel)
    {
        uint8_t buf[2] = {0X40, buff[pixel]};
        i2c_.TransmitBlocking(i2c_address_, buf, 2, 1000);
    };

  private:
    daisy::I2CHandle i2c_;
    uint8_t          i2c_address_;
};
}

#endif //MY_TRANSPORT_H
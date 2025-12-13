#ifndef UPDATED_DRIVER_H
#define UPDATED_DRIVER_H

#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"

/**
 * A driver implementation for the SSD1306/SSD1309
 */
template <size_t width, size_t height, typename Transport>
class UpdatedDriver
{
  public:
    struct Config
    {
        typename Transport::Config transport_config;
    };

    void Init(Config config)
    {
        transport_.Init(config.transport_config);

        // Init routine...

        // Display Off
        transport_.SendCommand(0xaE);
        // Dimension dependent commands...
        switch(height)
        {
            case 16:
                // Display Clock Divide Ratio
                transport_.SendCommand(0xD5);
                transport_.SendCommand(0x60);
                // Multiplex Ratio
                transport_.SendCommand(0xA8);
                transport_.SendCommand(0x0F);
                // COM Pins
                transport_.SendCommand(0xDA);
                transport_.SendCommand(0x02);
                break;
            case 32:
                // Display Clock Divide Ratio
                transport_.SendCommand(0xD5);
                transport_.SendCommand(0x80);
                // Multiplex Ratio
                transport_.SendCommand(0xA8);
                transport_.SendCommand(0x1F);
                // COM Pins
                transport_.SendCommand(0xDA);
                if(width == 64)
                {
                    transport_.SendCommand(0x12);
                }
                else
                {
                    transport_.SendCommand(0x02);
                }

                break;
            case 48:
                // Display Clock Divide Ratio
                transport_.SendCommand(0xD5);
                transport_.SendCommand(0x80);
                // Multiplex Ratio
                transport_.SendCommand(0xA8);
                transport_.SendCommand(0x2F);
                // COM Pins
                transport_.SendCommand(0xDA);
                transport_.SendCommand(0x12);
                break;
            default: // 128
                // Display Clock Divide Ratio
                transport_.SendCommand(0xD5);
                transport_.SendCommand(0x80);
                // Multiplex Ratio
                transport_.SendCommand(0xA8);
                transport_.SendCommand(0x3F);
                // COM Pins
                transport_.SendCommand(0xDA);
                transport_.SendCommand(0x12);
                break;
        }

        // Display Offset
        transport_.SendCommand(0xD3);
        transport_.SendCommand(0x00);
        // Start Line Address
        transport_.SendCommand(0x40);
        // Normal Display
        transport_.SendCommand(0xA6);
        // All On Resume
        transport_.SendCommand(0xA4);
        // Charge Pump
        transport_.SendCommand(0x8D);
        transport_.SendCommand(0x14);
        // Set Segment Remap
        transport_.SendCommand(0xA1);
        // COM Output Scan Direction
        transport_.SendCommand(0xC8);
        // Contrast Control
        transport_.SendCommand(0x81);
        transport_.SendCommand(0x8F);
        // Pre Charge
        transport_.SendCommand(0xD9);
        transport_.SendCommand(0x25);
        // VCOM Detect
        transport_.SendCommand(0xDB);
        transport_.SendCommand(0x34);


        // Display On
        transport_.SendCommand(0xAF); //--turn on oled panel
    };

    size_t Width() const { return width; };
    size_t Height() const { return height; };

    void DrawPixel(uint_fast8_t x, uint_fast8_t y, bool on)
    {
        if(x >= width || y >= height)
            return;
        if(on)
            buffer_[x + (y / 8) * width] |= (1 << (y % 8));
        else
            buffer_[x + (y / 8) * width] &= ~(1 << (y % 8));
    }

    void Fill(bool on)
    {
        for(size_t i = 0; i < sizeof(buffer_); i++)
        {
            buffer_[i] = on ? 0xff : 0x00;
        }
    };

    void Update(int i) {
        int pix = i % width;
        int j = int(i/width);

        if (pix == 0) {
            
            uint8_t high_column_addr;
            switch(height)
            {
                case 32: high_column_addr = 0x12; break;

                default: high_column_addr = 0x10; break;
            }
            transport_.SendCommand(0xB0 + j);
            transport_.SendCommand(0x00);
            transport_.SendCommand(high_column_addr);
        }
        

        transport_.SendDataPerPixel(&buffer_[width * j], pix);
        //transport_.SendDataPerPixel(&buffer_[width * i], i);
    }

    /**
     * Update the whole display 
    */
    void Update()
    {
        uint8_t i;
        uint8_t high_column_addr;
        switch(height)
        {
            case 32: high_column_addr = 0x12; break;

            default: high_column_addr = 0x10; break;
        }
        for(i = 0; i < (height / 8); i++)
        {
            
            transport_.SendCommand(0xB0 + i);
            transport_.SendCommand(0x00);
            transport_.SendCommand(high_column_addr);
            transport_.SendData(&buffer_[width * i], width);
            
        }
    };

  private:
    Transport transport_;
    uint8_t   buffer_[width * height / 8];
};

#endif //UPDATED_DRIVER_H
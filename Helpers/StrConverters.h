#pragma once

// buffer MINIMUM: 8 chars -> "-123.45\0"
inline void floatToCString2(float v, char* buf)
{
    // handle sign
    if (v < 0.0f)
    {
        *buf++ = '-';
        v = -v;
    }

    // round to 2 decimals
    int value = (int)(v * 100.0f + 0.5f);

    int intPart  = value / 100;
    int fracPart = value % 100;

    // convert integer part (max 3-4 digits typically)
    char tmp[6];
    int i = 0;
    do {
        tmp[i++] = '0' + (intPart % 10);
        intPart /= 10;
    } while (intPart);

    // reverse
    while (i--)
        *buf++ = tmp[i];

    *buf++ = '.';
    *buf++ = '0' + (fracPart / 10);
    *buf++ = '0' + (fracPart % 10);
    *buf = '\0';
}

// buffer minimum 4 chars -> "-99\0"
inline void intToCString2(int v, char* buf)
{
    char* p = buf;

    // handle sign
    if(v < 0)
    {
        *p++ = '-';
        v = -v;
    }

    // limit to 2 digits
    if(v > 99)
        v = 99;

    // convert tens
    if(v >= 10)
    {
        *p++ = '0' + (v / 10);
    }

    // convert units
    *p++ = '0' + (v % 10);

    // string termination
    *p = '\0';
}

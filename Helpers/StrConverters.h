#pragma once

// Float to string with 2 deciaml after comma when is in between -1 & 1
inline void floatToCStringFixed4(float v, char* buf)
{
    if (v > 9.99f)  v = 9.99f;
    if (v < -9.99f) v = -9.99f;

    int value = (int)(v * 100.0f + (v >= 0.0f ? 0.5f : -0.5f));

    int intPart  = abs(value / 100);
    int fracPart = abs(value % 100);

    if (v < 0.0f)
        buf[0] = '-';
    else
        buf[0] = '0' + intPart;

    buf[1] = '.';
    buf[2] = '0' + (fracPart / 10);
    buf[3] = '0' + (fracPart % 10);
}

inline void floatToCStringPct3(float v, char* buf)
{
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;

    int pct = (int)(v * 100.0f + 0.5f); // 0..100

    if (pct > 100) pct = 100;

    buf[0] = '0' + (pct / 100);          // centaines
    buf[1] = '0' + ((pct / 10) % 10);    // dizaines
    buf[2] = '0' + (pct % 10);           // unités
}

// Float to string with 2 deciaml after comma
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

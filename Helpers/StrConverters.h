#pragma once


// buffer MINIMUM: 8 chars -> "-123.45\0"
inline void floatToCString2(float v, char* buf)
{
    // gestion du signe
    if (v < 0.0f)
    {
        *buf++ = '-';
        v = -v;
    }

    // arrondi à 2 décimales
    int value = (int)(v * 100.0f + 0.5f);

    int intPart  = value / 100;
    int fracPart = value % 100;

    // conversion partie entière (max 3–4 digits typiquement)
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

    // gestion du signe
    if(v < 0)
    {
        *p++ = '-';
        v = -v;
    }

    // limiter à 2 chiffres
    if(v > 99)
        v = 99;

    // conversion des dizaines
    if(v >= 10)
    {
        *p++ = '0' + (v / 10);
    }

    // conversion des unités
    *p++ = '0' + (v % 10);

    // terminaison de la chaîne
    *p = '\0';
}

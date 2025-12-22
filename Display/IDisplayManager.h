#pragma once

#include "stdint.h"

class IDisplayManager {
public:
    virtual ~IDisplayManager() = default;
public:
    virtual void Write(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr, bool now=false) = 0;
    virtual void WriteLine(uint8_t lineIdx, const char* str) = 0;
    virtual void WriteNow(const char* l0=nullptr, const char* l1=nullptr, const char* l2=nullptr, const char* l3=nullptr) = 0;
};

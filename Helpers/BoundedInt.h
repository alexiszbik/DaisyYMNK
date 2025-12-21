#pragma once

template<int Min, int Max>
class BoundedInt
{
public:
    constexpr BoundedInt(int v = Min) : value(v) {}

    void increment()
    {
        value = (value >= Max) ? Min : value + 1;
    }

    void decrement()
    {
        value = (value <= Min) ? Max : value - 1;
    }

    int get() const { return value; }

private:
    int value;
};

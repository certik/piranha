#ifndef MYINT_H
#define MYINT_H

#include <iostream>

class myint
{
private:
    int i_;
public:
    myint() : i_{0} {}
    myint(int i) : i_{i} {}
    std::ostream& to_ostream(std::ostream &out) const
    {
        out << i_;
        return out;
    }
    bool eq(const myint &b) const
    {
        return i_ == b.i_;
    }
    myint add(const myint &b) const
    {
        return myint(i_+b.i_);
    }
    void iadd(const myint &b)
    {
        i_ += b.i_;
    }
    myint sub(const myint &b) const
    {
        return myint(i_-b.i_);
    }
    void isub(const myint &b)
    {
        i_ -= b.i_;
    }
    myint neg() const
    {
        return myint(-i_);
    }
    myint mul(const myint &b) const
    {
        return myint(i_*b.i_);
    }
    void imul(const myint &b)
    {
        i_ *= b.i_;
    }
};

std::ostream& operator<<(std::ostream& out, const myint &i)
{
    return i.to_ostream(out);
}

bool operator==(const myint &a, const myint &b)
{
    return a.eq(b);
}

bool operator!=(const myint &a, const myint &b)
{
    return !a.eq(b);
}

myint operator+(const myint &a, const myint &b)
{
    return a.add(b);
}

void operator+=(myint &a, const myint &b)
{
    a.iadd(b);
}

myint operator-(const myint &a, const myint &b)
{
    return a.sub(b);
}

void operator-=(myint &a, const myint &b)
{
    a.isub(b);
}

myint operator-(const myint &a)
{
    return a.neg();
}

myint operator*(const myint &a, const myint &b)
{
    return a.mul(b);
}

void operator*=(myint &a, const myint &b)
{
    a.imul(b);
}

#endif

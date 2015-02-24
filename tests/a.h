#ifndef MYINT_H
#define MYINT_H

#include <iostream>
#include <cstddef>

#include <gmp.h>
#define PTR(x)   ((x)->_mp_d)

class myint
{
private:
    mpz_t z_;
public:
    myint() {
        mpz_init(z_);
    }
    myint(int i) {
        mpz_init(z_);
        mpz_set_si(z_, i);
    }
    // Copy constructor and assignment
    myint(const myint& i) {
        mpz_init(z_);
        mpz_set(z_, i.z_);
    }
    myint& operator=(const myint& i) {
        mpz_set(z_, i.z_);
        return *this;
    }
    // Move constructor and assignment
    myint(myint&& i) noexcept {
        PTR(z_) = NULL;
        mpz_swap(z_, i.z_);
    }
    myint& operator=(myint&& i) noexcept {
        mpz_swap(z_, i.z_);
        return *this;
    }
    ~myint() {
        mpz_clear(z_);
    }
    std::ostream& to_ostream(std::ostream &out) const
    {
        out << mpz_get_si(z_);
        return out;
    }
    bool eq(const myint &b) const
    {
        return mpz_cmp(z_, b.z_) == 0;
    }
    myint add(const myint &b) const
    {
        myint r;
        mpz_add(r.z_, z_, b.z_);
        return r;
    }
    void iadd(const myint &b)
    {
        mpz_add(z_, z_, b.z_);
    }
    myint sub(const myint &b) const
    {
        myint r;
        mpz_sub(r.z_, z_, b.z_);
        return r;
    }
    void isub(const myint &b)
    {
        mpz_sub(z_, z_, b.z_);
    }
    myint neg() const
    {
        myint r;
        mpz_neg(r.z_, z_);
        return r;
    }
    myint mul(const myint &b) const
    {
        myint r;
        mpz_mul(r.z_, z_, b.z_);
        return r;
    }
    void imul(const myint &b)
    {
        mpz_mul(z_, z_, b.z_);
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

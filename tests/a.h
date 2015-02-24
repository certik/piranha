#ifndef MYINT_H
#define MYINT_H

#include <iostream>

#include <gmp.h>
#define PTR(x)   ((x)->_mp_d)

using bint = std::int64_t;
using ubint = std::uint64_t;
#define CSYMPY_INT_BITS 64
static_assert(CSYMPY_INT_BITS == sizeof(bint)*8,
        "INT_BITS value is inconsistent");
#define CSYMPY_PTR2INT(x) (((ubint) (x) >> 2) | (1LL << (CSYMPY_INT_BITS - 2)))
#define CSYMPY_INT2PTR(x) ((__mpz_struct *) ((x) << 2))
#define CSYMPY_INT_IS_MPZ(x) (((x) >> (CSYMPY_INT_BITS - 2)) == 1)
#define CSYMPY_INT_MAX ((1LL << (CSYMPY_INT_BITS - 2)) - 1)
#define CSYMPY_INT_MIN (-(1LL << (CSYMPY_INT_BITS - 2)))


class myint
{
private:
    bint b_;
public:
    myint() : b_{0} {}
    myint(int i) : b_{i} {}
    // Copy constructor and assignment
    myint(const myint& i) {
    }
    myint& operator=(const myint& i) {
        if (!CSYMPY_INT_IS_MPZ(i.b_)) {
            if (CSYMPY_INT_IS_MPZ(b_))
                mpz_clear(CSYMPY_INT2PTR(b_));
            b_ = i.b_;
        } else {
            if (!CSYMPY_INT_IS_MPZ(b_)) {
                promote();
            }
            mpz_set(CSYMPY_INT2PTR(b_), CSYMPY_INT2PTR(i.b_));
        }
        return *this;
    }
    // Move constructor and assignment
    myint(myint&& i) noexcept {
        b_ = 0;
        std::swap(b_, i.b_);
    }
    myint& operator=(myint&& i) noexcept {
        std::swap(b_, i.b_);
        return *this;
    }
    ~myint() {
        if (CSYMPY_INT_IS_MPZ(b_)) {
            mpz_clear(CSYMPY_INT2PTR(b_));
            // delete CSYMPY_INT2PTR(b_);
        }
    }
    void promote()
    {
        __mpz_struct *p = new __mpz_struct;
        mpz_init(p);
        mpz_set_si(p, b_);
        b_ = CSYMPY_PTR2INT(p);
    }
    std::ostream& to_ostream(std::ostream &out) const
    {
        if (CSYMPY_INT_IS_MPZ(b_)) {
            out << mpz_get_si(CSYMPY_INT2PTR(b_));
        } else {
            out << b_;
        }
        return out;
    }
    bool eq(const myint &b) const
    {
        if (!CSYMPY_INT_IS_MPZ(b_) && !CSYMPY_INT_IS_MPZ(b.b_)) {
            return b_ == b.b_;
        } else if (CSYMPY_INT_IS_MPZ(b_) && CSYMPY_INT_IS_MPZ(b.b_)) {
            return mpz_cmp(CSYMPY_INT2PTR(b_), CSYMPY_INT2PTR(b.b_)) == 0;
        } else {
            return false;
        }
    }
    myint add(const myint &b) const
    {
        if (!CSYMPY_INT_IS_MPZ(b_) && !CSYMPY_INT_IS_MPZ(b.b_)) {
            return myint(b_ + b.b_);
        } else if (CSYMPY_INT_IS_MPZ(b_) && CSYMPY_INT_IS_MPZ(b.b_)) {
            myint r;
            r.promote();
            mpz_add(CSYMPY_INT2PTR(r.b_), CSYMPY_INT2PTR(b_),
                    CSYMPY_INT2PTR(b.b_));
            return r;
        } else {
            // FIXME
        }
    }
    void iadd(const myint &b)
    {
        if (!CSYMPY_INT_IS_MPZ(b_) && !CSYMPY_INT_IS_MPZ(b.b_)) {
            b_ += b.b_;
        } else if (CSYMPY_INT_IS_MPZ(b_) && CSYMPY_INT_IS_MPZ(b.b_)) {
            mpz_add(CSYMPY_INT2PTR(b_), CSYMPY_INT2PTR(b_),
                    CSYMPY_INT2PTR(b.b_));
        } else {
            // FIXME
        }
    }
    myint sub(const myint &b) const
    {
        /*
        myint r;
        mpz_sub(r.z_, z_, b.z_);
        return r;
        */
    }
    void isub(const myint &b)
    {
//        mpz_sub(z_, z_, b.z_);
    }
    myint neg() const
    {
/*
        myint r;
        mpz_neg(r.z_, z_);
        return r;
*/
    }
    myint mul(const myint &b) const
    {
/*
        myint r;
        mpz_mul(r.z_, z_, b.z_);
        return r;
*/
    }
    void imul(const myint &b)
    {
//        mpz_mul(z_, z_, b.z_);
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

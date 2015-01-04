#ifndef __int_64_H__
#define __int_64_H__

//#include "type.h"
#include "global.h"

#define TOP_ONE     0x80000000
#define TOP_ZERO    0

typedef class _int_64
{
    private:
    int value[2];

    public:
    _int_64(){ value[0] = value[1] = 0; }
    _int_64(int *);
   // _int_64(_int_64 &param);
    _int_64(const _int_64 &param);
    _int_64(int param);
    ~_int_64();


    const int* GetValue() const ;
    _int_64 operator=(const _int_64& param);
    _int_64 operator=(int param);
    _int_64 operator<<(int param);
    _int_64 operator>>(int param);

    _int_64 operator&(const _int_64& param);
    int operator&(int param);
    _int_64 operator+(const _int_64& param);
    _int_64 operator^(const _int_64& param);

    friend ostream & operator<< (ostream &out,_int_64 &param);
    friend istream & operator>> (istream &in ,_int_64 &param);
};

#endif

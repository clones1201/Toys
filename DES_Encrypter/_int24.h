#ifndef __INT24_H__
#define __INT24_H__

//#include "type.h"
#include "global.h"

#define mask24 0x00ffffff
#define mask23 0x007fffff

class _int24
{
    private:
    int value;
    _int24 shiftLeftHelp();

    public:
    _int24(){ value = 0; }
   // _int24(_int24 &param);
    _int24(const _int24 &param);
    _int24(int param);
    ~_int24(){};

    int GetValue() const;
    _int24 operator=(const _int24& param);
    _int24 operator=(int param);
    _int24 operator<<(int param);
    _int24 operator>>(int param);

    _int24 operator&(const _int24& param);
    int operator&(int param);
    _int24 operator+(const _int24& param);
    _int24 operator+(int param);
    _int24 operator^(const _int24 &param);

    _int24 shiftLeft(int param);
    friend ostream & operator<< (ostream &out,const _int24 &param);
    friend istream & operator>> (istream &in ,_int24 &param);
};

#endif

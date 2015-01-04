#ifndef __INT28_H__
#define __INT28_H__

//#include "type.h"
#include "global.h"

#define mask28 0x0fffffff
#define mask27 0x07ffffff

class _int28
{
    private:
    int value;

    _int28 shiftLeftHelp();
    public:
    _int28(){ value = 0; }
   // _int28(_int28 &param);
    _int28(const _int28 &param);
    _int28(int param);
    ~_int28(){};

    int GetValue() const;
    _int28 operator=(const _int28& param);
    _int28 operator=(int param);
    _int28 operator<<(int param);
    _int28 operator>>(int param);

    _int28 operator&(const _int28& param);
    int operator&(int param);
    _int28 operator+(const _int28& param);
    _int28 operator+(int param);
    _int28 operator^(const _int28& param);

    _int28 shiftLeft(int param);
    friend ostream & operator<< (ostream &out,const _int28 &param);
    friend istream & operator>> (istream &in ,_int28 &param);
};

#endif


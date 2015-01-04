#ifndef __INT48_H__
#define __INT48_H__

//#include "type.h"
#include "global.h"
#include "_int24.h"

class _int48
{
    private:
    _int24 value[2];

    public:
    _int48(){
        value[0] = 0; value[1] = 0; }
    _int48(int param);
    _int48(int* param);
    _int48(_int24 *);
  //  _int48(_int48 &param);
    _int48(const _int48 &param);
    ~_int48();

    _int24* GetValue();
    const _int24* GetValue() const;
    _int48 operator=(const _int48& param);
    _int48 operator=(int param);
    _int48 operator<<(int param);
    _int48 operator>>(int param);

    _int48 operator&(const _int48& param);
    _int48 operator+(const _int48& param);
    int operator&(int param);
    _int48 operator^(const _int48& param);

    friend ostream & operator<< (ostream &out,const _int48 &param);
    friend istream & operator>> (istream &in ,_int48 &param);
};

#endif

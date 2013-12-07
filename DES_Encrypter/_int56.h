#ifndef __INT56_H__
#define __INT56_H__

//#include "type.h"
#include "global.h"
#include "_int28.h"

class _int56
{
    private:
    _int28 value[2];

    public:
    _int56(){
       value[0] = 0; value[1] = 0; }
    _int56(_int28 *);
   // _int56(_int56 &param);
    _int56(const _int56 &param);
    _int56(int param);
    ~_int56();

    _int28* GetValue();
    const _int28* GetValue() const;
    _int56 operator=(const _int56& param);
    _int56 operator=(int param);
    _int56 operator<<(int param);
    _int56 operator>>(int param);

    _int56 operator&(const _int56& param);
    _int56 operator+(const _int56& param);
    int operator&( int param);
    _int56 operator^(const _int56& param);

    _int56 ShiftLeft(int param);
    friend ostream & operator<< (ostream &out,const _int56 &param);
    friend istream & operator>> (istream &in ,_int56 &param);
};

#endif

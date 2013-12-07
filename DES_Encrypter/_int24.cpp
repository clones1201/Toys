
/******* integer - 28 bits *********/

#include "_int24.h"

_int24::_int24(int param)
{
    value = param;
}

_int24 _int24::operator=(const _int24& param)
{
    value = param.GetValue();
    return *this;
}


_int24 _int24::operator=(int param)
{
    value = param;
    return *this;
}

//_int24::_int24(_int24 &param)
//{
//    value = param.GetValue();
//}

_int24::_int24(const _int24 &param)
{
    value = param.GetValue();
}

int _int24::GetValue() const
{
    return value;
}

_int24 _int24::operator<<(int param)
{
    _int24 temp(value);
    temp.value <<=param;
    return temp;
}

_int24 _int24::operator>>(int param)
{

    int mask = 0x00ffffff;
    _int24 temp(value);
    temp.value = temp.value & mask;
    temp.value = temp.value>>param;
    return temp;
}

_int24 _int24::shiftLeftHelp()
{
    int top = 1<<23;
    int temp = ( value & top ) >> 23;
    value = (value<<1) + temp;
    return *this;
}

_int24 _int24::shiftLeft(int param)
{
    int i;
    for( i = 0 ; i < param ; i++)
        shiftLeftHelp();
    return *this;
}

_int24 _int24::operator&(const _int24& param)
{
    _int24 temp(value);
    temp.value = temp.value & param.value;
    return temp;
}

int _int24::operator&(int param)
{
 //   _int24 temp(value);
 //   temp.value = temp.value & param;
    return  (value & param);
}

_int24 _int24::operator+(const _int24 &param)
{
    _int24 temp(value);
    temp.value = temp.value + param.value;
    return temp;
}

_int24 _int24::operator+(int param)
{
    _int24 temp(value);
    temp.value = temp.value + param;
    return temp;
}

_int24 _int24::operator^(const _int24 &param)
{
    _int24 temp(value);
    temp.value = (temp.value ^ param.value) & mask24;
    return temp;
}

ostream & operator<< (ostream &out,const _int24 &param)
{
    out.setf(ios_base::hex,ios_base::basefield);
    int mask = 0x00ffffff;
    int temp = param.GetValue() & mask;
    out<<temp;
    return out;
}

istream & operator>> (istream &in ,_int24 &param)
{
    in.setf(ios_base::hex,ios_base::basefield);
    int temp;
    in>>temp;
    param = (_int24&)temp;
    return in;
}

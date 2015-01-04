
/******* integer - 28 bits *********/

#include "_int28.h"

_int28::_int28(int param)
{
    value = param;
}

_int28 _int28::operator=(const _int28& param)
{
    value = param.GetValue();
    return *this;
}


_int28 _int28::operator=(int param)
{
    value = param;
    return *this;
}
/*
_int28::_int28(_int28 &param)
{
    value = param.GetValue();
}*/

_int28::_int28(const _int28 &param)
{
    value = param.GetValue();
}

int _int28::GetValue() const
{
    return value;
}

_int28 _int28::operator<<(int param)
{
    _int28 temp(value);
    temp.value <<=param;
    return temp;
}

_int28 _int28::operator>>(int param)
{

    int mask = 0x0fffffff;
    _int28 temp(value);
    temp.value = temp.value & mask;
    temp.value = temp.value>>param;
    return temp;
}

_int28 _int28::shiftLeftHelp()
{
    int top = 1<<27;
    int temp = ( value & top ) >> 27;
    value = ((value<<1) + temp ) & mask28;
    return *this;
}

_int28 _int28::shiftLeft(int param)
{
    int i;
    for( i = 0 ; i < param ; i++)
        shiftLeftHelp();
    return *this;
}

_int28 _int28::operator&(const _int28& param)
{
    _int28 temp(value);
    temp.value = temp.value & param.value;
    return temp;
}

int _int28::operator&(int param)
{
  //  _int28 temp(value);
  //  temp.value = temp.value & param;
    return value & param;
}

_int28 _int28::operator+(const _int28 &param)
{
    _int28 temp(value);
    temp.value = temp.value + param.value;
    return temp;
}

_int28 _int28::operator+(int param)
{
    _int28 temp(value);
    temp.value = temp.value + param;
    return temp;
}

_int28 _int28::operator^(const _int28& param)
{
    _int28 temp(value);
    temp.value = (temp.value ^ param.value) & mask28;
    return temp;
}

ostream & operator<< (ostream &out,const _int28 &param)
{
    out.setf(ios_base::hex,ios_base::basefield);
    int mask = 0x0fffffff;
    int temp = param.GetValue() & mask;
    out<<temp;
    return out;
}

istream & operator>> (istream &in ,_int28 &param)
{
    in.setf(ios_base::hex,ios_base::basefield);
    int temp;
    in>>temp;
    param = (_int28&)temp;
    return in;
}

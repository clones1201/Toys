/*************  integer - 64 bits ************/

#include "_int64.h"

//_int_64::_int_64(_int_64 &param)
//{
//    const int *temp = param.GetValue();
//    value[0] = temp[0];
//    value[1] = temp[1];
//}

_int_64::_int_64(const _int_64 &param)
{
    int *temp = new int[2];
    const int* p = param.GetValue();
    temp[0] = p[0];
    temp[1] = p[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
}

_int_64::_int_64(int* temp)
{
    value[0] = temp[0];
    value[1] = temp[1];
}

_int_64::_int_64(int param)
{
    value[2] = 0;
    value[1] = param;
}

_int_64::~_int_64()
{
}

const int* _int_64::GetValue() const
{
    return value;
}

_int_64 _int_64::operator=(const _int_64& param)
{
    const int* temp = param.GetValue();
    value[0] = temp[0];
    value[1] = temp[1];
    return *this;
}

_int_64 _int_64::operator=(int param)
{
    value[0] =0;
    value[1] = param;
    return *this;
}

_int_64 _int_64::operator<<(int param)
{
    int i,topbit;

    _int_64 temp(value);

    for( i =0 ; i < param ; i++ )
    {
        topbit = (( 0x80000000 & temp.value[1]) >> 31);
        temp.value[0]<<=1;
        temp.value[0]+=topbit;
        temp.value[1]<<=1;
    }
    return temp;
}

_int_64 _int_64::operator>>(int param)
{
    int i,lastbit;

    _int_64 temp(value);
    for( i =0 ; i < param ; i++ )
    {
        lastbit = (( 0x00000001 & temp.value[0]) << 31);
        temp.value[1]>>=1;
        temp.value[1] = temp.value[1] & 0x7fffffff ;
        temp.value[1]+=lastbit;
        temp.value[0]>>=1;
    }
    return temp;
}

_int_64 _int_64::operator&(const _int_64& param)
{
    _int_64 temp(value);
    temp.value[0] &= param.value[0];
    temp.value[1] &= param.value[1];
    return temp;
}

int _int_64::operator&(int param)
{
    return (value[1] & param);
}

_int_64 _int_64::operator+(const _int_64& param)
{
    _int_64 temp(value);
    temp.value[0] += param.value[0];
   unsigned int x,y,r;
    x = temp.value[1] & TOP_ONE;
    y = param.value[1] & TOP_ONE;
    r = (temp.value[1] + param.value[1]) & TOP_ONE;

    if( (( (x == TOP_ONE && y == TOP_ZERO) || (x == TOP_ZERO && y == TOP_ONE) ) && r == TOP_ZERO)  ||  (x == TOP_ONE && y == TOP_ONE) )  //最高位不同，结果最高位为0，或者两者最高位均为1，有进位
        temp.value[0] += 1;
    temp.value[1] = temp.value[1] + param.value[1];
    return temp;
}

_int_64 _int_64::operator^(const _int_64& param)
{
    _int_64 temp(value);
    temp.value[0] =temp.value[0] ^ param.value[0];
    temp.value[1] =temp.value[1] ^ param.value[1];
    return temp;
}

ostream & operator<< (ostream &out,_int_64 &param)
{
    out.setf(ios_base::hex,ios_base::basefield);



    const int* temp = param.GetValue();
    if(temp[0]==0)
    {
         out.width(8);
        out.fill('0');
        out<<right<<temp[1];
    }
    else
    {
        out.width(8);
        out.fill('0');
        out<<right<<temp[0];
        out.width(8);
        out.fill('0');
        out<<right<<temp[1];
    }
    return out;
}
istream & operator>> (istream &in ,_int_64 &param)
{
    in.setf(ios_base::hex,ios_base::basefield);
    int temp[2];
    in>>temp[0]>>temp[1];
    param = (_int_64&)(temp);
    return in;
}


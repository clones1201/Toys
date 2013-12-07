
/*************  integer - 48 bits ************/

#include "_int48.h"

//_int48::_int48(_int48 &param)
//{
//    _int24 *temp = new _int24[2];
//    temp[0] = param.GetValue()[0];
//    temp[1] = param.GetValue()[1];
//    value[0] = temp[0];
//    value[1] = temp[1];
//    delete [] temp;
//}

_int48::_int48(const _int48 &param)
{
    _int24 *temp = new _int24[2];
    const _int24 *p = param.GetValue();
    temp[0] = p[0];
    temp[1] = p[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
}

_int48::_int48(_int24* param)
{
    _int24 *temp = new _int24[2];
    temp[0] = param[0];
    temp[1] = param[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
}

_int48::_int48(int* param)
{
    _int24 *temp = new _int24[2];
    temp[0] = param[0];
    temp[1] = param[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
}

_int48::_int48(int param)
{
    value[2] = 0;
    value[1] = param;
}

_int48::~_int48()
{

}

_int24* _int48::GetValue()
{
    return value;
}

const _int24* _int48::GetValue() const
{
    return value;
}

_int48 _int48::operator=(const _int48& param)
{
    _int24 *temp = new _int24[2];
    temp[0] = param.GetValue()[0];
    temp[1] = param.GetValue()[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
    return *this;
}

_int48 _int48::operator=(int param)
{
    value[0] = 0;
    value[1] = param;
    return *this;
}

_int48 _int48::operator<<(int param)
{
    int i,topbit;

    _int48 temp(value);

    for( i =0 ; i < param ; i++ )
    {
        topbit = (( 0x00800000 & temp.value[1].GetValue()) >> 23);
        temp.value[0] = (temp.value[0] <<1 ) & mask24 ;
        temp.value[0] = temp.value[0] + topbit;
        temp.value[1] = (temp.value[1] << 1) & mask24  ;
    }
    return temp;
}

_int48 _int48::operator>>(int param)
{
    int i,lastbit;

    _int48 temp(value);
    for( i =0 ; i < param ; i++ )
    {
        lastbit = (( temp.value[0].GetValue() & 0x00000001 ) << 23);
        temp.value[1] = ( temp.value[1] >> 1 ) & mask23 ;
        temp.value[1] = temp.value[1]+lastbit;
        temp.value[0] = ( temp.value[0] >> 1 ) & mask24  ;
    }
    return temp;
}

_int48 _int48::operator&(const _int48& param)
{
    _int48 temp(value);
    temp.value[0] =temp.value[0] & param.value[0];
    temp.value[1] =temp.value[1] & param.value[1];
    return temp;
}

int _int48::operator&(int param)
{
    return (value[1] & param);
}

_int48 _int48::operator+(const _int48& param)
{
    _int48 temp(value);
    temp.value[0] =(temp.value[0] + param.value[0]) & mask24 ;

    if( ( (temp.value[1].GetValue() + param.value[1].GetValue() ) & 0x01000000 )== 0x01000000 ) //有进位
        temp.value[0] =  temp.value[0] + 1;

    temp.value[1] = (temp.value[1] + param.value[1]) & mask24 ;
    return temp;
}

_int48 _int48::operator^(const _int48& param)
{
    _int48 temp(value);
    temp.value[0] =temp.value[0] ^ param.value[0];
    temp.value[1] =temp.value[1] ^ param.value[1];
    return temp;
}

ostream & operator<< (ostream &out,const _int48 &param)
{
    out.setf(ios_base::hex,ios_base::basefield);
    const _int24* temp = param.GetValue();
    if(temp[0].GetValue()==0)
    {
        out.width(6);
        out.fill('0');
        out<<right<<temp[1];
    }
    else
    {
        out.width(6);
        out.fill('0');
        out<<right<<temp[0];
        out.width(6);
        out.fill('0');
        out<<right<<temp[1];
    }
    return out;
}

istream & operator>> (istream &in ,_int48 &param)
{
    in.setf(ios_base::hex,ios_base::basefield);
    int temp[2];
    in>>temp[0]>>temp[1];
    param = (_int48&)(temp);
    return in;
}


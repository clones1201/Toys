
/*************  integer - 56 bits ************/

#include "_int56.h"
//
//_int56::_int56(_int56 &param)
//{
//    _int28 *temp = new _int28[2];
//    temp[0] = param.GetValue()[0];
//    temp[1] = param.GetValue()[1];
//    value[0] = temp[0];
//    value[1] = temp[1];
//    delete [] temp;
//}

_int56::_int56(const _int56 &param)
{
    _int28 *temp = new _int28[2];
    const _int28 *p = param.GetValue();
    temp[0] = p[0];
    temp[1] = p[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
}

_int56::_int56(_int28* param)
{
    _int28 *temp = new _int28[2];
    temp[0] = param[0];
    temp[1] = param[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
}

_int56::_int56(int param)
{
    value[2] = 0;
    value[1] = param;
}

_int56::~_int56()
{

}

_int28* _int56::GetValue()
{
    return value;
}

const _int28* _int56::GetValue() const
{
    return value;
}

_int56 _int56::operator=(const _int56& param)
{
    _int28 *temp = new _int28[2];
    temp[0] = param.GetValue()[0];
    temp[1] = param.GetValue()[1];
    value[0] = temp[0];
    value[1] = temp[1];
    delete [] temp;
    return *this;
}

_int56 _int56::operator=(int param)
{
    value[0]=0;
    value[1] = param;
    return *this;
}

_int56 _int56::operator<<(int param)
{
    int i,topbit;

    _int56 temp(value);

    for( i =0 ; i < param ; i++ )
    {
        topbit = (( 0x08000000 & temp.value[1].GetValue()) >> 27);
        temp.value[0] = (temp.value[0] << 1) & mask28;
        temp.value[0] = temp.value[0] + topbit;
        temp.value[1] = (temp.value[1] << 1) & mask28;
    }
    return temp;
}

_int56 _int56::operator>>(int param)
{
    int i,lastbit;

    _int56 temp(value);
    for( i =0 ; i < param ; i++ )
    {
        lastbit = (( temp.value[0].GetValue() & 0x00000001 ) << 27);
        temp.value[1] = (temp.value[1] >> 1 ) & mask27 ;
        temp.value[1] = temp.value[1]+lastbit;
        temp.value[0] = (temp.value[0] >> 1 ) & mask28;
    }
    return temp;
}

_int56 _int56::operator&(const _int56& param)
{
    _int56 temp(value);
    temp.value[0] =temp.value[0] & param.value[0];
    temp.value[1] =temp.value[1] & param.value[1];
    return temp;
}

_int56 _int56::operator+(const _int56& param)
{
    _int56 temp(value);
    temp.value[0] =(temp.value[0] + param.value[0]) & mask28;

    if( ( (temp.value[1].GetValue() + param.value[1].GetValue() ) & 0x10000000 ) == 0x10000000 ) //ÓÐ½øÎ»
        temp.value[0] =  temp.value[0] + 1 ;

    temp.value[1] = (temp.value[1] + param.value[1]) & mask28;
    return temp;
}

int _int56::operator&(int param)
{
    return (value[1] & param);
}

_int56 _int56::operator^(const _int56& param)
{
    _int56 temp(value);
    temp.value[0] =temp.value[0] ^ param.value[0] ;
    temp.value[1] =temp.value[1] ^ param.value[1] ;
    return temp;
}

_int56 _int56::ShiftLeft(int param)
{
    _int56 temp(value);
    temp.value[0].shiftLeft(param);
    temp.value[1].shiftLeft(param);
    value[0] = temp.value[0];
    value[1] = temp.value[1];
    return *this;
}

ostream & operator<< (ostream &out,const _int56 &param)
{
    out.setf(ios_base::hex,ios_base::basefield);
     out.width(7);
    out.fill('0');

    const _int28* temp = param.GetValue();
    if(temp[0].GetValue()==0)
    {
        out.width(7);
        out.fill('0');
        out<<right<<temp[1];
    }
    else
     {
         out.width(7);
        out.fill('0');
        out<<right<<temp[0];
        out.width(7);
        out.fill('0');
        out<<right<<temp[1];
     }
    return out;
}

istream & operator>> (istream &in ,_int56 &param)
{
    in.setf(ios_base::hex,ios_base::basefield);
    int temp[2];
    in>>temp[0]>>temp[1];
    param = (_int56&)(temp);
    return in;
}

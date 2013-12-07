#include "DES.h"
#include "global.h"

template <class TYPE1,class TYPE2>
void  DES::displace(TYPE1 &param1,TYPE2 &param2,int length1,int length2,int * Table)
{
    TYPE1 temp = param1;
    int i;
    TYPE1 bit1;  //从TYPE1取出的bit
    TYPE2 bit2;  //将要放到TYPE2的bit

    param2 = 0;

    for( i = 0 ; i < length2 ; i ++ )
    {
        bit1 = TYPE1(1) << ( length1 - Table[i] );
        bit1 = ( bit1 & temp ) >> ( length1 - Table[i] ) ;
        bit2 = bit1 & 1;
        param2 = param2 + (bit2 << (length2 - i - 1 ));
    }
}

int DES::feistel(int RHalfblock,_int48& subkey)
{
    //_int48 _subkey = subkey;
    _int48 _RHalfblock48 = 0;

#ifdef DEBUG
    cout<<"\tRi:\t\t";
    cout.width(8);
    cout.fill('0');
    cout<<right<<RHalfblock<<endl;
#endif

    displace(RHalfblock,_RHalfblock48,32,48,ExtendTable);

#ifdef DEBUG
    cout<<"\tAfter E:\t";
    cout<<_RHalfblock48<<endl;
#endif

#ifdef DEBUG
    cout<<"\tsubkey:\t\t"<<subkey<<endl;
#endif
    _RHalfblock48 = _RHalfblock48 ^ subkey;

#ifdef DEBUG
    cout<<"\tAfter xor:\t";
    cout<<_RHalfblock48<<endl;
#endif

    int RTemp = SBoxMethod(_RHalfblock48);

#ifdef DEBUG
    cout<<"\tAfter Sbox:\t";
    cout.width(8);
    cout.fill('0');
    cout<<right<<RTemp<<endl;
#endif

    int output =0;
    displace(RTemp,output,32,32,PTable);

#ifdef DEBUG
    cout<<"\tAfter P:\t";
    cout.width(8);
    cout.fill('0');
    cout<<right<<output<<endl;
#endif

    return output;
}

int DES::SBoxMethod(_int48 &param)
{
    int result = 0;
    int s;
    int x,y;
    int i;
    for ( i = 0 ; i < 8 ; i++ )
    {
        s = ( param.GetValue()[ (i*6)/24 ]  &   (0x3f << (6*(3-i%4))) ) >> (6*(3-i%4));
        x = (((s & 0x20) >> 4 ) & 0x2) + (s & 0x1);
        y = ((s & 0x1e)  >> 1 )& 0xf;
        result = result + ( SBOX[i][y+x*16] << (4 * (7- i) ));
    }
    return result;
}

void DES::GenerateSubkey(_int_64 key)
{
    DES::cipher = key;
    _int56 temp;
    int i;

    displace(key,temp,64,56,displaceTable1);

    for( i = 0 ; i < 16 ; i++ )
    {
        subkey[i] = 0;
        temp = temp.ShiftLeft( shiftLeft[i] );
        displace(temp,subkey[i],56,48,displaceTable2);

#ifdef DEBUG
        cout<<"subkey"<<i+1<<":\t"<<subkey[i]<<endl;
#endif
    }
}

void DES::Encryption(const BYTE* data,BYTE *result)
{
    Transform(data,result,ENC);
}

void DES::Decryption(const BYTE*data,BYTE *result)
{
    Transform(data,result,DEC);
}

void DES::Transform(const BYTE *data,BYTE* result,int mode)
{
    int Half[2]= {0,0};
    int temp32 = 0;

#ifdef DEBUG
    switch(mode)
    {
    case ENC:
        cout<<"\n\nProcess of Encryption:\n";
        break;
    case DEC:
        cout<<"\n\nProcess of Decryption:\n";
        break;
    }

#endif

    int i;
    for( i = 0 ; i < 4 ; i++ )
    {
        Half[0] = Half[0] +  (( int(data[i]) << (8 * (3 - i)))  &  0xff << (8 * (3 - i)));
        Half[1] = Half[1] +  (( int(data[i+4]) << (8 * (3 - i)))  &  0xff << (8 * (3 - i)));
    }

    _int_64 Message(Half);


    _int_64 temp;
    displace(Message,temp,64,64,IPTable);  //初始变换
    Half[0] = *temp.GetValue();
    Half[1] = *(temp.GetValue()+1);

#ifdef DEBUG
    cout<<"Plaintext in Hex:\t\t"<<Message<<endl;
    cout << "After IP:\t\t\t"<<temp<<endl;
#endif

    for ( i = 0 ; i < 16 ; i++ )            //轮转
    {

#ifdef DEBUG
        cout<<"\tLi:\t\t";

        cout.width(8);
        cout.fill('0');
        cout<<right<<Half[0]<<endl;
#endif
        switch(mode)
        {
        case ENC:
            temp32 = Half[1];
            Half[1] = Half[0] ^ feistel(Half[1],subkey[i]);
            Half[0] = temp32;
            break;

        case DEC:
            temp32 = Half[1];
            Half[1] = Half[0] ^ feistel(Half[1],subkey[15 - i]);
            Half[0] = temp32;
            break;
        }

#ifdef DEBUG
        cout<<"Round "<<i<<":\t\t\t";
        cout.width(8);
        cout.fill('0');
        cout<<right<<Half[0];
        cout.width(8);
        cout.fill('0');
        cout<<right<<Half[1];
        cout<<endl;
#endif
    }

    temp32 = Half[1];
    Half[1] = Half[0] ;
    Half[0] = temp32;   //32位互换

#ifdef DEBUG
    cout << "Before IP^-1:\t\t\t";
    cout.width(8);
    cout.fill('0');
    cout<<right<<Half[0];
    cout.width(8);
    cout.fill('0');
    cout<<right<<Half[1];
    cout<<endl;
#endif


    Message = _int_64(Half);
    displace(Message,temp,64,64,IIPTable); //逆初始变换


#ifdef DEBUG
    _int_64 temp2,temp3;
    displace(temp,temp2,64,64,IPTable);
    displace(temp2,temp3,64,64,IIPTable);
    cout << "After IP^-1:\t\t\t"<<temp<<endl;
    cout << "IP(M):\t\t\t"<<temp2<<endl;
    cout << "IP^-1(IP(M)):\t\t"<<temp3<<endl;
#endif

    Half[0] = *temp.GetValue();
    Half[1] = *(temp.GetValue()+1);

    for( i = 0 ; i < 4 ; i++ )
    {
        result[i]   = ( Half[0] & ( 0x000000ff << ((3 - i)*8) ) )>>(3 - i)*8;
        result[i+4] = ( Half[1] & ( 0x000000ff << ((3 - i)*8) ) )>>(3 - i)*8;
    }
}

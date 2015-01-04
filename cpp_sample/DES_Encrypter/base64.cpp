#include "base64.h"

string Base64Encoder::Encode(vector<BYTE> &data)  //input 3 Bytes and output 4 characters
{
    int i;
    BYTE inbuf[3];
    vector<BYTE>::iterator iter;
    string result;
    char outBuf[4];

    iter = data.begin();
    int append_num = data.size() %3 ;

    for( i = 0 ; i <  data.size()/3   ; i++ )
    {
        inbuf[0] = *iter++;
        inbuf[1] = *iter++;
        inbuf[2] = *iter++;

        outBuf[0] = base64Table[ (( inbuf[0] & 0xfc ) >> 2 ) & 0x3f ] ;
        outBuf[1] = base64Table[ ((( inbuf[0] & 0x03) << 4 ) & 0x30 ) + (((inbuf[1] & 0xf0) >> 4 ) & 0x0f)];
        outBuf[2] = base64Table[ ((( inbuf[1] & 0x0f ) << 2 ) & 0x3c ) + (((inbuf[2] & 0xc0) >> 6 ) & 0x03)];
        outBuf[3] = base64Table[ inbuf[2] & 0x3f ];

        result.push_back(outBuf[0]);
        result.push_back(outBuf[1]);
        result.push_back(outBuf[2]);
        result.push_back(outBuf[3]);
    }

    if(append_num != 0 )
    {
        inbuf[0] = *iter++;
        inbuf[1] = (append_num ==1 )?  0 : *iter;
        inbuf[2] =  0;

        outBuf[0] = base64Table[ (( inbuf[0] & 0xfc ) >> 2 ) & 0x3f ] ;
        outBuf[1] = base64Table[ ((( inbuf[0] & 0x03) << 4 ) & 0x30 ) + (((inbuf[1] & 0xf0) >> 4 ) & 0x0f)];
        outBuf[2] = base64Table[ ((( inbuf[1] & 0x0f ) << 2 ) & 0x3c ) + (((inbuf[2] & 0xc0) >> 6 ) & 0x03)];
        outBuf[3] = base64Table[ inbuf[2] & 0x3f ];

        result.push_back(outBuf[0]);
        result.push_back(outBuf[1]);
        result.push_back(outBuf[2]);
        result.push_back(outBuf[3]);
    }

    for(i = 0 ; i < 3 - append_num ; i++ )
        result.push_back(fill_append);

    return result;
}

/*
vector<BYTE> Decode(string DecData)
{
    int length = DecData.size();
    int append_length = length % 4;
    int i;
    vector<BYTE> outBuf;
    char inbuf[4];
    for ( i = 0 ; i < length ; i++)
    {

    }

}
*/


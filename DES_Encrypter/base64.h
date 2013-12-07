#ifndef __BASE64_H__
#define __BASE64_H__

#include "global.h"
#include "type.h"

static char base64Table[64] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9',
    '+','/'
};

static char fill_append = '=';

class Base64Encoder
{
public:
    string Encode(vector<BYTE> &EncData); //input 3 Bytes and output 4 characters
    vector<BYTE> Decode(string DecData);
};

#endif

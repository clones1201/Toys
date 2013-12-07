#include "type.h"
#include "global.h"
#include "DES.h"
#include "base64.h"
#include "bmp.h"
using namespace std;

int main()
{

    int i,j;
    BYTE data[8] = {'a','b','c','d','e','f','g','h'};
    BYTE* result = new BYTE[8];
    BYTE* temp = new BYTE[8];

    int inputTemp[2] = { 0x61626364, 0x65666768};
    _int_64 cipher = inputTemp;
    DES DES_ENC;

    cout<<"Cipher:\t\t0x"<<cipher<<endl;

    DES_ENC.GenerateSubkey(cipher);

    cout<<"Plaintext before encryption:\t";
    for( i = 0 ; i < 8 ; i++)
    {
        cout<<data[i];
    }
    cout<<endl;

    cout<<"Plaintext in Base64:\t\t\t";
    vector<BYTE> b64input;
    for( i = 0 ; i < 8 ; i++)
        b64input.push_back(data[i]);

    Base64Encoder base64Enc;
    string b64result = base64Enc.Encode(b64input);
    cout<<b64result;
    cout<<endl;

    DES_ENC.Encryption(data,result);

    cout<<"Ciphertext:\t\t\t";
    b64input.clear();
    for( i = 0 ; i < 8 ; i++)
        b64input.push_back(result[i]);

    b64result = base64Enc.Encode(b64input);
    cout<<b64result;
    cout<<endl;

    DES_ENC.Decryption(result,temp);

    cout<<"plaintext after decryption:\t";
    for( i = 0 ; i < 8 ; i++)
    {
        result[i] = *(temp + i);
        cout<<result[i];
    }
    cout<<endl;

    cout<<"图片加密实验"<<endl;
    BYTE* image;
    BYTE* imageEncryted;
    BYTE* imageDecryted;
    int xsize = 512;
    int ysize = 512;
    image = (BYTE *)malloc((size_t)xsize * ysize * 3);
    imageEncryted = (BYTE *)malloc((size_t)xsize * ysize * 3);
    imageDecryted = (BYTE *)malloc((size_t)xsize * ysize * 3);


    char *inputFile = "512px-Lenna";
    char *outputFile = "Lenna-Encrypted";
    bmp_read(image, xsize,ysize, inputFile);

    cout<<"加密文件："<<inputFile<<".bmp"<<endl;

    for( i = 0 ; i < (3 * xsize * ysize)/8 ; i++ )
    {
        for( j = 0 ; j < 8 ; j++  )
        {
            data[j] = *(image + (i * 8) + j);
        }
        DES_ENC.Encryption(data,result);
        BYTEcpy(imageEncryted + (i * 8),result,8);
        cout.setf(ios_base::dec,ios_base::basefield);
        cout<<100 * i/((3 * xsize * ysize)/8)<<"%\r";
    }
    bmp_write(imageEncryted,xsize,ysize,outputFile);
    cout<<"输出文件:"<<outputFile<<".bmp"<<endl;

    inputFile = "Lenna-Encrypted";
    outputFile = "Lenna-Decrypted";
    bmp_read(image, xsize,ysize, "Lenna-Encrypted");

    cout<<"解密文件："<<inputFile<<".bmp"<<endl;
    for( i = 0 ; i < (3 * xsize * ysize)/8 ; i++ )
    {
        for( j = 0 ; j < 8 ; j++  )
        {
            data[j] = *(image + (i * 8) + j);
        }
        DES_ENC.Decryption(data,result);
        BYTEcpy(imageDecryted + (i * 8),result,8);
        cout.setf(ios_base::dec,ios_base::basefield);
        cout<<100 * i/((3 * xsize * ysize)/8)<<"%\r";
    }
    bmp_write(imageDecryted,xsize,ysize,outputFile);
    cout<<"输出文件:"<<outputFile<<".bmp"<<endl;;
    return 0;
}

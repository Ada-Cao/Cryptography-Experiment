#include "AES.h"

// 构造函数default
AES::AES()
{
}

// 加密
void AES::encrypt(byte* block, byte* roundKey, int Nr)
{
    // 轮密钥加
    AddRoundKey(block, roundKey, 0);

    // 第1-9轮：顺序进行字节代换、行移位、列混合、轮密钥加
    for (int round = 1; round < Nr; ++round)
    {
        ByteSub(block);		// 字节代换
        ShiftRow(block);	// 行移位
        MixColumn(block);	// 列混合
        AddRoundKey(block, roundKey, round); // 轮密钥加
    }
    // 第10轮：字节代换、行移位、轮密钥加
    ByteSub(block);
    ShiftRow(block);
    AddRoundKey(block, roundKey, Nr);
}

// 解密
void AES::decrypt(byte* block, byte* roundKey, int Nr)
{
    // 先进行一轮：轮密钥加，逆行移位，逆字节代换（无列混合）
    AddRoundKey(block, roundKey, Nr);
    ShiftRow_1(block);
    ByteSub_1(block);

    // 进行九轮完整的轮函数
    for (int round = (Nr - 1); round >= 1; --round)
    {
        AddRoundKey(block, roundKey, round);
        MixColumn_1(block);
        ShiftRow_1(block);
        ByteSub_1(block);
    }
    AddRoundKey(block, roundKey, 0);
}

// 密钥拓展
void AES::KeyExpansion(byte* inkey, byte* outkey, int Nk, int Nr)
{
    byte temp[4], t;
    int x, i;

    // 第0组：[0-3]直接拷贝
    for (i = 0; i < (4 * Nk); i++)
    {
        outkey[i] = inkey[i];
    }
    // 第1-10组：[4-43]
    i = Nk;
    while (i < (4 * (Nr + 1)))
    {
        // 当i不是4的倍数的时候
        for (x = 0; x < 4; ++x)
        {
            temp[x] = outkey[(4 * (i - 1)) + x];
        }
	
        //i是4的倍数的时候
        if (i % Nk == 0)
        {
            // 字循环
            t = temp[0]; temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
            // 字节代换
            for (x = 0; x < 4; x++)
            {
                temp[x] = S[temp[x]];
            }
            // 异或轮常量
            temp[0] ^= Rcon[(i / Nk) - 1];
        }
        for (x = 0; x < 4; ++x)
        {
            outkey[(4 * i) + x] = outkey[(4 * (i - Nk)) + x] ^ temp[x];
        }

        ++i;
    }

}

// 字节代换
void AES::ByteSub(byte* col)
{
    for (int i = 0; i < 16; ++i)
    {
        col[i] = S[col[i]];
    }
}

// 逆字节代换
void AES::ByteSub_1(byte* col)
{
    for (int i = 0; i < 16; ++i)
    {
        col[i] = S_1[col[i]];
    }
}

// 行移位
void AES::ShiftRow(byte* col)
{
    byte temp;

    // 第一行左移1位
    temp = col[1];  //   保存第一行第一列，最后将其赋给最后一行第一列
    for (int i = 1; i < 12; i += 4)
    {
        col[i] = col[i + 4];
    }
    col[13] = temp;

    // 第二行左移2位
    temp = col[2];
    col[2] = col[10];
    col[10] = temp;
    temp = col[6];
    col[6] = col[14];
    col[14] = temp;

    // 第三行左移3位
    temp = col[15];
    for (int i = 15; i > 3; i-=4)
    {
        col[i] = col[i - 4];
    }
    col[3] = temp;

    // 第四行不移位
}

// 逆行移位
void AES::ShiftRow_1(byte* col)
{
    byte temp;
    
    // 第一行
    temp = col[13];
    for (int i = 13; i > 3; i -= 4)
    {
        col[i] = col[i - 4];
    }
    col[1] = temp;

    // 第二行，直接交换
    temp = col[2]; col[2] = col[10]; col[10] = temp;
    temp = col[6]; col[6] = col[14]; col[14] = temp;

    // 第三行
    temp = col[3];
    for (int i = 3; i < 12; i += 4)
    {
        col[i] = col[i + 4];
    }
    col[15] = temp;
}

// 列混合
void AES::MixColumn(byte* col)
{
    byte temp[4];
    
    // col+4每次往后移动一列,共进行4次
    for (int i = 0; i < 4; ++i, col += 4)
    {
        // 进行异或，矩阵运算
        temp[0] = x2time(col[0]) ^ x3time(col[1]) ^ col[2] ^ col[3];	// 02 03 01 01
        temp[1] = col[0] ^ x2time(col[1]) ^ x3time(col[2]) ^ col[3];	// 01 02 03 01
        temp[2] = col[0] ^ col[1] ^ x2time(col[2]) ^ x3time(col[3]);	// 01 01 02 03
        temp[3] = x3time(col[0]) ^ col[1] ^ col[2] ^ x2time(col[3]);	// 03 01 01 02

        // 赋值
        for (int i = 0; i < 4; ++i)
        {
            col[i] = temp[i];
        }
    }
}

// 逆列混合
void AES::MixColumn_1(byte* col)
{
    byte temp[4];

    for (int x = 0; x < 4; ++x, col += 4)
    {
        // 乘以逆矩阵
        temp[0] = xEtime(col[0]) ^ xBtime(col[1]) ^ xDtime(col[2]) ^ x9time(col[3]);
        temp[1] = x9time(col[0]) ^ xEtime(col[1]) ^ xBtime(col[2]) ^ xDtime(col[3]);
        temp[2] = xDtime(col[0]) ^ x9time(col[1]) ^ xEtime(col[2]) ^ xBtime(col[3]);
        temp[3] = xBtime(col[0]) ^ xDtime(col[1]) ^ x9time(col[2]) ^ xEtime(col[3]);

        // 赋值
        for (int i = 0; i < 4; ++i)
        {
            col[i] = temp[i];
        }
    }
}

// 密钥加
void AES::AddRoundKey(byte* col, byte* roundKey, int round)
{
    for (int x = 0; x < 16; ++x)
    {	
        // 将状态矩阵每个字节与对应轮密钥字节进行异或，实现轮密钥加
        col[x] ^= roundKey[(round << 4) + x];
    }
}

// 实现有限域GF(2^8)上的乘法
byte AES::x2time(byte x)
{
    // 如果最高位是1，则需进行取模运算
    if (x & 0x80)
    {
        return (((x << 1) ^ 0x1B) & 0xFF);
    }

    // 如果最高位不是1，则直接移位
    return x << 1;
}

byte AES::x3time(byte x)
{
    return (x2time(x) ^ x);
}

byte AES::x4time(byte x)
{
    return (x2time(x2time(x)));
}

byte AES::x8time(byte x)
{
    return (x2time(x2time(x2time(x))));
}

byte AES::x9time(byte x)
{
    return (x8time(x) ^ x);
}

byte AES::xBtime(byte x)
{
    return (x9time(x) ^ x2time(x));
}

byte AES::xDtime(byte x)
{
    return (x8time(x) ^ x4time(x) ^ x);
}

byte AES::xEtime(byte x)
{
    return (x8time(x) ^ x4time(x) ^ x2time(x));
}
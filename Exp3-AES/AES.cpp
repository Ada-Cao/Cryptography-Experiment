#include "AES.h"

// ���캯��default
AES::AES()
{
}

// ����
void AES::encrypt(byte* block, byte* roundKey, int Nr)
{
    // ����Կ��
    AddRoundKey(block, roundKey, 0);

    // ��1-9�֣�˳������ֽڴ���������λ���л�ϡ�����Կ��
    for (int round = 1; round < Nr; ++round)
    {
        ByteSub(block);		// �ֽڴ���
        ShiftRow(block);	// ����λ
        MixColumn(block);	// �л��
        AddRoundKey(block, roundKey, round); // ����Կ��
    }
    // ��10�֣��ֽڴ���������λ������Կ��
    ByteSub(block);
    ShiftRow(block);
    AddRoundKey(block, roundKey, Nr);
}

// ����
void AES::decrypt(byte* block, byte* roundKey, int Nr)
{
    // �Ƚ���һ�֣�����Կ�ӣ�������λ�����ֽڴ��������л�ϣ�
    AddRoundKey(block, roundKey, Nr);
    ShiftRow_1(block);
    ByteSub_1(block);

    // ���о����������ֺ���
    for (int round = (Nr - 1); round >= 1; --round)
    {
        AddRoundKey(block, roundKey, round);
        MixColumn_1(block);
        ShiftRow_1(block);
        ByteSub_1(block);
    }
    AddRoundKey(block, roundKey, 0);
}

// ��Կ��չ
void AES::KeyExpansion(byte* inkey, byte* outkey, int Nk, int Nr)
{
    byte temp[4], t;
    int x, i;

    // ��0�飺[0-3]ֱ�ӿ���
    for (i = 0; i < (4 * Nk); i++)
    {
        outkey[i] = inkey[i];
    }
    // ��1-10�飺[4-43]
    i = Nk;
    while (i < (4 * (Nr + 1)))
    {
        // ��i����4�ı�����ʱ��
        for (x = 0; x < 4; ++x)
        {
            temp[x] = outkey[(4 * (i - 1)) + x];
        }
	
        //i��4�ı�����ʱ��
        if (i % Nk == 0)
        {
            // ��ѭ��
            t = temp[0]; temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
            // �ֽڴ���
            for (x = 0; x < 4; x++)
            {
                temp[x] = S[temp[x]];
            }
            // ����ֳ���
            temp[0] ^= Rcon[(i / Nk) - 1];
        }
        for (x = 0; x < 4; ++x)
        {
            outkey[(4 * i) + x] = outkey[(4 * (i - Nk)) + x] ^ temp[x];
        }

        ++i;
    }

}

// �ֽڴ���
void AES::ByteSub(byte* col)
{
    for (int i = 0; i < 16; ++i)
    {
        col[i] = S[col[i]];
    }
}

// ���ֽڴ���
void AES::ByteSub_1(byte* col)
{
    for (int i = 0; i < 16; ++i)
    {
        col[i] = S_1[col[i]];
    }
}

// ����λ
void AES::ShiftRow(byte* col)
{
    byte temp;

    // ��һ������1λ
    temp = col[1];  //   �����һ�е�һ�У�����丳�����һ�е�һ��
    for (int i = 1; i < 12; i += 4)
    {
        col[i] = col[i + 4];
    }
    col[13] = temp;

    // �ڶ�������2λ
    temp = col[2];
    col[2] = col[10];
    col[10] = temp;
    temp = col[6];
    col[6] = col[14];
    col[14] = temp;

    // ����������3λ
    temp = col[15];
    for (int i = 15; i > 3; i-=4)
    {
        col[i] = col[i - 4];
    }
    col[3] = temp;

    // �����в���λ
}

// ������λ
void AES::ShiftRow_1(byte* col)
{
    byte temp;
    
    // ��һ��
    temp = col[13];
    for (int i = 13; i > 3; i -= 4)
    {
        col[i] = col[i - 4];
    }
    col[1] = temp;

    // �ڶ��У�ֱ�ӽ���
    temp = col[2]; col[2] = col[10]; col[10] = temp;
    temp = col[6]; col[6] = col[14]; col[14] = temp;

    // ������
    temp = col[3];
    for (int i = 3; i < 12; i += 4)
    {
        col[i] = col[i + 4];
    }
    col[15] = temp;
}

// �л��
void AES::MixColumn(byte* col)
{
    byte temp[4];
    
    // col+4ÿ�������ƶ�һ��,������4��
    for (int i = 0; i < 4; ++i, col += 4)
    {
        // ������򣬾�������
        temp[0] = x2time(col[0]) ^ x3time(col[1]) ^ col[2] ^ col[3];	// 02 03 01 01
        temp[1] = col[0] ^ x2time(col[1]) ^ x3time(col[2]) ^ col[3];	// 01 02 03 01
        temp[2] = col[0] ^ col[1] ^ x2time(col[2]) ^ x3time(col[3]);	// 01 01 02 03
        temp[3] = x3time(col[0]) ^ col[1] ^ col[2] ^ x2time(col[3]);	// 03 01 01 02

        // ��ֵ
        for (int i = 0; i < 4; ++i)
        {
            col[i] = temp[i];
        }
    }
}

// ���л��
void AES::MixColumn_1(byte* col)
{
    byte temp[4];

    for (int x = 0; x < 4; ++x, col += 4)
    {
        // ���������
        temp[0] = xEtime(col[0]) ^ xBtime(col[1]) ^ xDtime(col[2]) ^ x9time(col[3]);
        temp[1] = x9time(col[0]) ^ xEtime(col[1]) ^ xBtime(col[2]) ^ xDtime(col[3]);
        temp[2] = xDtime(col[0]) ^ x9time(col[1]) ^ xEtime(col[2]) ^ xBtime(col[3]);
        temp[3] = xBtime(col[0]) ^ xDtime(col[1]) ^ x9time(col[2]) ^ xEtime(col[3]);

        // ��ֵ
        for (int i = 0; i < 4; ++i)
        {
            col[i] = temp[i];
        }
    }
}

// ��Կ��
void AES::AddRoundKey(byte* col, byte* roundKey, int round)
{
    for (int x = 0; x < 16; ++x)
    {	
        // ��״̬����ÿ���ֽ����Ӧ����Կ�ֽڽ������ʵ������Կ��
        col[x] ^= roundKey[(round << 4) + x];
    }
}

// ʵ��������GF(2^8)�ϵĳ˷�
byte AES::x2time(byte x)
{
    // ������λ��1���������ȡģ����
    if (x & 0x80)
    {
        return (((x << 1) ^ 0x1B) & 0xFF);
    }

    // ������λ����1����ֱ����λ
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
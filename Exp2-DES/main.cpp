#include<iostream>
#include<iomanip>
#include<string>
#include <numeric>
using namespace std;
#include"DES.h"

// ��ʮ������ת��Ϊ������
array<int, 64> hexToBinary(const string& hexStr)
{
    array<int, 64> result = { 0 };
    if (hexStr.length() != 16)
    {
        throw invalid_argument("�����ʮ�������ַ�����Ϊ16λ��");
    }

    for (int i = 0; i < 16; ++i)
    {
        int decValue = 0;
        if (hexStr[i] >= '0' && hexStr[i] <= '9')
        {
            decValue = hexStr[i] - '0';
        }
        else if (hexStr[i] >= 'A' && hexStr[i] <= 'F')
        {
            decValue = hexStr[i] - 'A' + 10;
        }
        else if (hexStr[i] >= 'a' && hexStr[i] <= 'f')
        {
            decValue = hexStr[i] - 'a' + 10;
        }
        else
        {
            throw invalid_argument("�ַ�����0-9��A-F֮�䣡");
        }

        for (int j = 0; j < 4; ++j)
        {
            result[i * 4 + (3 - j)] = decValue % 2;
            decValue /= 2;
        }
    }

    return result;
}

// ��������ת��Ϊʮ������
string binToHexadecimal(const array<int, 64>& binArr)
{
    // ��string���ͱ���Ԥ�ȷ���16���ռ�
    string result(16, '0');
    for (int i = 0; i < 16; ++i)
    {
        int decValue = 0;
        decValue = binArr[i * 4] * 8 + binArr[i * 4 + 1] * 4 + binArr[i * 4 + 2] * 2 + binArr[i * 4 + 3];
        
        if(decValue >= 0 && decValue <= 9)
        {
            result[i] = '0' + decValue;
        }
        else if (decValue >= 10 && decValue <= 15)
        {
            // Ĭ�����Ϊ��д��ĸ
            result[i] = 'A' + decValue - 10;
        }
        else if (decValue < 0 || decValue>15)
        {
            throw invalid_argument("����ת��ʱ���ִ���");
        }
    }
    return result;
}

// �������ʮ�������ַ����ֿ飬ÿ��16λ������16λ�������
vector<string> splitIntoGroup(const string& input)
{
    vector<string> blocks;
    int str_size = input.size();
    int block_size = 16;
    int round = str_size / block_size;

    // �����ķ���
    for (int i = 0; i < round; ++i)
    {
        string result = input.substr(i * 16, 16);
        blocks.push_back(result);
    }
   
    // ����16λ������䣬ȱ����λ�����Ϊ���ٵ��ַ�
    if (str_size % block_size > 0)
    {
        int index = round * block_size;
        int remain = str_size - index;
        string result(16, '0'); // �ȳ�ʼ��һ������Ϊ16λ���ַ���
       
        // ����ʣ���ַ�
        for (int i = 0; i < remain; ++i)
        {
            result[i] = input[index + i];
        }
        // ȱ�ٲ��ֽ�����䣬ȱ��һλ�����0�������ȱ�ٶ���λ�������ٸ�����λ���ַ�
        int padNum = 16 - remain;
        char padChar;
        if (padNum == 1)
        {
            padChar = '0';
        }
        else if (padNum > 1 && padNum <= 9)
        {
            padChar = '0' + padNum;
        }
        else if (padNum >= 10 && padNum < 16)
        {
            padChar = 'A' + padNum - 10;
        }

        for (int i = remain; i < 16; ++i)
        {
            result[i] = padChar;
        }

        blocks.push_back(result);
    }

    return blocks;
}

// ������ܲ��ϲ����ܽ��
string groupEncrypt(const vector<string>& blocks, DES des)
{
    // ��ȡ���鳤��
    int block_size = blocks.size();
    vector<string> encryptResult;

    for (int i = 0; i < block_size; ++i)
    {
        array<int, 64> temp = des.encrypt(hexToBinary(blocks[i]));
        string result = binToHexadecimal(temp);
        encryptResult.push_back(result);
    }
    string finalResult= accumulate(encryptResult.begin(), encryptResult.end(), string());

    return finalResult;
}

// ������ܲ��ϲ����ܽ����������ĩβ�����ַ�
string groupDecrypt(const vector<string>& blocks, DES des)
{
    // ��ȡ��������
    int block_size = blocks.size();
    vector<string> decryptResult;

    for (int i = 0; i < block_size; ++i)
    {
        array<int, 64> temp = des.decrypt(hexToBinary(blocks[i]));
        string result = binToHexadecimal(temp);
        decryptResult.push_back(result);
    }
    string finalResult = accumulate(decryptResult.begin(), decryptResult.end(), string());

    // ������������ַ�
    int length = finalResult.size();
    char pad = finalResult[length - 1];
    int calculate_sum = 1;
    int pad_sum = 0;

    if (pad == '0')
    {
        finalResult.erase(length - 1);
    }
    else if (pad >= '1' && pad <= '9')
    {
        pad_sum = pad - '0';
    }
    else if (pad >= 'A' && pad <= 'F')
    {
        pad_sum = pad - 'A' + 10;
    }


    for (int i = length - 2; i >= 0; --i)
    {
        if (finalResult[i] != pad)
        {
            break; // �������ģ�������ɾ��
        }
        else if (finalResult[i] == pad)
        {
            calculate_sum++;
        }
    }

    if (calculate_sum == pad_sum)
    {
        finalResult.erase(length - pad_sum);
    }
    
    return finalResult;
}

int main()
{
    int select = 1;
    cout << "��ӭ����DES�ӽ���ϵͳ��\n";
    cout << "Tips����Կ����16λʮ����������������������λ��16��������\n";
    cout << setw(6) << " " << "�������һλ����Ϊ0��\n";
    cout << "********************�˵�*******************\n";
    cout << "1������\n";
    cout << "2������\n";
    cout << "3���˳�\n";


    while (select) 
    { 
        cout << "��ѡ����Ҫ���еĻ��ţ�";
        cin >> select;
        
        if (select == 3)
        {
            break;
        }
        else if (select == 1)   // DES����
        {
            // ��ȡ���ĺ���Կ
            string plaintext;
            cout << "@��������ܵ����ģ�";
            cin >> plaintext;
            
            string key;
            cout << "@��������Կ��";
            cin >> key;
            array<int, 64 > Key = hexToBinary(key);
            vector<string> Plaintext = splitIntoGroup(plaintext);
            
            // ���м�������
            DES des;
            des.generateSubKeys(Key);
            string encryptResult = groupEncrypt(Plaintext, des);
            cout << "@���ܽ��Ϊ��" << encryptResult << endl << endl;
        }
        else if (select == 2)   // DES����
        {
            // ��ȡ���ĺ���Կ
            string ciphertext;
            cout << "@��������ܵ����ģ�";
            cin >> ciphertext;

            string key;
            cout << "@��������Կ��";
            cin >> key;
            array<int, 64 > Key = hexToBinary(key);
            vector<string> Ciphertext = splitIntoGroup(ciphertext);

            // ���н�������
            DES des;
            des.generateSubKeys(Key);
            string decryptResult = groupDecrypt(Ciphertext, des);
            cout << "@���ܽ��Ϊ��" << decryptResult << endl << endl;
        }
        else
        {
            cout << "������������ţ�\n";
        }
        
    }

    system("cls");
    cout << "��ӭ����ʹ�ã��ڴ��´��ټ���\n";


	system("pause");
	return 0;
}
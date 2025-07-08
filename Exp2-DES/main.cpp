#include<iostream>
#include<iomanip>
#include<string>
#include <numeric>
using namespace std;
#include"DES.h"

// 将十六进制转换为二进制
array<int, 64> hexToBinary(const string& hexStr)
{
    array<int, 64> result = { 0 };
    if (hexStr.length() != 16)
    {
        throw invalid_argument("输入的十六进制字符串需为16位！");
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
            throw invalid_argument("字符需在0-9，A-F之间！");
        }

        for (int j = 0; j < 4; ++j)
        {
            result[i * 4 + (3 - j)] = decValue % 2;
            decValue /= 2;
        }
    }

    return result;
}

// 将二进制转换为十六进制
string binToHexadecimal(const array<int, 64>& binArr)
{
    // 给string类型变量预先分配16个空间
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
            // 默认输出为大写字母
            result[i] = 'A' + decValue - 10;
        }
        else if (decValue < 0 || decValue>15)
        {
            throw invalid_argument("进制转换时出现错误！");
        }
    }
    return result;
}

// 将输入的十六进制字符串分块，每块16位，不足16位进行填充
vector<string> splitIntoGroup(const string& input)
{
    vector<string> blocks;
    int str_size = input.size();
    int block_size = 16;
    int round = str_size / block_size;

    // 完整的分组
    for (int i = 0; i < round; ++i)
    {
        string result = input.substr(i * 16, 16);
        blocks.push_back(result);
    }
   
    // 不足16位进行填充，缺多少位则填充为多少的字符
    if (str_size % block_size > 0)
    {
        int index = round * block_size;
        int remain = str_size - index;
        string result(16, '0'); // 先初始化一个长度为16位的字符串
       
        // 复制剩余字符
        for (int i = 0; i < remain; ++i)
        {
            result[i] = input[index + i];
        }
        // 缺少部分进行填充，缺少一位则填充0，其余的缺少多少位则填充多少个多少位的字符
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

// 分组加密并合并加密结果
string groupEncrypt(const vector<string>& blocks, DES des)
{
    // 获取分组长度
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

// 分组解密并合并解密结果，并处理末尾填充的字符
string groupDecrypt(const vector<string>& blocks, DES des)
{
    // 获取分组组数
    int block_size = blocks.size();
    vector<string> decryptResult;

    for (int i = 0; i < block_size; ++i)
    {
        array<int, 64> temp = des.decrypt(hexToBinary(blocks[i]));
        string result = binToHexadecimal(temp);
        decryptResult.push_back(result);
    }
    string finalResult = accumulate(decryptResult.begin(), decryptResult.end(), string());

    // 处理最后填充的字符
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
            break; // 正常明文，不进行删除
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
    cout << "欢迎来到DES加解密系统！\n";
    cout << "Tips：密钥输入16位十六进制数，明文密文随意位数16进制数！\n";
    cout << setw(6) << " " << "明文最后一位不能为0！\n";
    cout << "********************菜单*******************\n";
    cout << "1、加密\n";
    cout << "2、解密\n";
    cout << "3、退出\n";


    while (select) 
    { 
        cout << "请选择你要进行的活动序号：";
        cin >> select;
        
        if (select == 3)
        {
            break;
        }
        else if (select == 1)   // DES加密
        {
            // 获取明文和密钥
            string plaintext;
            cout << "@请输入加密的明文：";
            cin >> plaintext;
            
            string key;
            cout << "@请输入密钥：";
            cin >> key;
            array<int, 64 > Key = hexToBinary(key);
            vector<string> Plaintext = splitIntoGroup(plaintext);
            
            // 进行加密运算
            DES des;
            des.generateSubKeys(Key);
            string encryptResult = groupEncrypt(Plaintext, des);
            cout << "@加密结果为：" << encryptResult << endl << endl;
        }
        else if (select == 2)   // DES解密
        {
            // 获取密文和密钥
            string ciphertext;
            cout << "@请输入解密的密文：";
            cin >> ciphertext;

            string key;
            cout << "@请输入密钥：";
            cin >> key;
            array<int, 64 > Key = hexToBinary(key);
            vector<string> Ciphertext = splitIntoGroup(ciphertext);

            // 进行解密运算
            DES des;
            des.generateSubKeys(Key);
            string decryptResult = groupDecrypt(Ciphertext, des);
            cout << "@解密结果为：" << decryptResult << endl << endl;
        }
        else
        {
            cout << "请重新输入序号！\n";
        }
        
    }

    system("cls");
    cout << "欢迎您的使用，期待下次再见！\n";


	system("pause");
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include"AES.h"
#include<vector>
#include<sstream>
#include <stdexcept>
using namespace std;


int main()
{
    AES aes;
    int select = 1;
    string input;
    byte key[17], expansionkey[15 * 16];
    memset(key, 0, 17);

    cout << "欢迎来到AES加解密系统！（明文128bit，密文128bit）\n";
    cout << "Tips：密钥输入16位十六进制数，明文随意位数，密文32位！\n";
    cout << "********************菜单*******************\n";
    cout << "1、加密\n";
    cout << "2、解密\n";
    cout << "3、退出\n";

    while (true)
    {
        cout << "请选择你要进行的活动序号：";
        cin >> select;
        cin.ignore(); // 清除换行符

        switch (select)
        {
        case 1:
        {
            cout << "@请输入加密的明文：";
            getline(cin, input);
            cout << "@请输入密钥：";
            cin.getline(reinterpret_cast<char*>(key), 17);

            // 计算填充后的长度
            int original_len = input.size();
            int block_num = (original_len + 15) / 16;
            int padded_len = block_num * 16;
            vector<unsigned char> padded_input(padded_len, 0);
            memcpy(padded_input.data(), input.data(), original_len);

            // 密钥扩展
            aes.KeyExpansion(key, expansionkey, 4, 10);

            // 加密每个块
            vector<byte> ciphertext(padded_len);
            for (int i = 0; i < block_num; ++i) {
                unsigned char block[16];
                memcpy(block, padded_input.data() + i * 16, 16);
                aes.encrypt(block, expansionkey, 10);
                memcpy(ciphertext.data() + i * 16, block, 16);
            }

            cout << "@加密结果为：";
            for (auto& c : ciphertext) {
                printf("%02x", c);
            }
            cout << endl << endl;
        } break;

        case 2:
        {
            cout << "@请输入解密的密文：";
            getline(cin, input);
            if (input.size() % 32 != 0) 
            {
                cout << "非法密文长度！" << endl;
                break;
            }

            // 转换十六进制字符串为字节
            vector<unsigned char> ciphertext(input.size() / 2);
            for (int i = 0; i < ciphertext.size(); ++i) {
                sscanf(input.c_str() + 2 * i, "%2hhx", &ciphertext[i]);
            }

            cout << "@请输入密钥：";
            cin.getline(reinterpret_cast<char*>(key), 17);

            // 密钥扩展
            aes.KeyExpansion(key, expansionkey, 4, 10);

            // 解密每个块
            vector<unsigned char> decrypted(ciphertext.size());
            int block_num = ciphertext.size() / 16;
            for (int i = 0; i < block_num; ++i) {
                unsigned char block[16];
                memcpy(block, ciphertext.data() + i * 16, 16);
                aes.decrypt(block, expansionkey, 10);
                memcpy(decrypted.data() + i * 16, block, 16);
            }

            cout << "明文：";
            for (auto& c : decrypted) {
                cout << c;
            }
            cout << endl << endl;
        } break;

        case 3:
            system("cls");
            cout << "欢迎您的使用，期待下次再见！\n";
            system("pause");
            return 0;

        default:
            cout << "非法选择！请重新选择。\n";
            break;
        }
    }

    return 0;
}



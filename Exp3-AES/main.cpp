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

    cout << "��ӭ����AES�ӽ���ϵͳ��������128bit������128bit��\n";
    cout << "Tips����Կ����16λʮ������������������λ��������32λ��\n";
    cout << "********************�˵�*******************\n";
    cout << "1������\n";
    cout << "2������\n";
    cout << "3���˳�\n";

    while (true)
    {
        cout << "��ѡ����Ҫ���еĻ��ţ�";
        cin >> select;
        cin.ignore(); // ������з�

        switch (select)
        {
        case 1:
        {
            cout << "@��������ܵ����ģ�";
            getline(cin, input);
            cout << "@��������Կ��";
            cin.getline(reinterpret_cast<char*>(key), 17);

            // ��������ĳ���
            int original_len = input.size();
            int block_num = (original_len + 15) / 16;
            int padded_len = block_num * 16;
            vector<unsigned char> padded_input(padded_len, 0);
            memcpy(padded_input.data(), input.data(), original_len);

            // ��Կ��չ
            aes.KeyExpansion(key, expansionkey, 4, 10);

            // ����ÿ����
            vector<byte> ciphertext(padded_len);
            for (int i = 0; i < block_num; ++i) {
                unsigned char block[16];
                memcpy(block, padded_input.data() + i * 16, 16);
                aes.encrypt(block, expansionkey, 10);
                memcpy(ciphertext.data() + i * 16, block, 16);
            }

            cout << "@���ܽ��Ϊ��";
            for (auto& c : ciphertext) {
                printf("%02x", c);
            }
            cout << endl << endl;
        } break;

        case 2:
        {
            cout << "@��������ܵ����ģ�";
            getline(cin, input);
            if (input.size() % 32 != 0) 
            {
                cout << "�Ƿ����ĳ��ȣ�" << endl;
                break;
            }

            // ת��ʮ�������ַ���Ϊ�ֽ�
            vector<unsigned char> ciphertext(input.size() / 2);
            for (int i = 0; i < ciphertext.size(); ++i) {
                sscanf(input.c_str() + 2 * i, "%2hhx", &ciphertext[i]);
            }

            cout << "@��������Կ��";
            cin.getline(reinterpret_cast<char*>(key), 17);

            // ��Կ��չ
            aes.KeyExpansion(key, expansionkey, 4, 10);

            // ����ÿ����
            vector<unsigned char> decrypted(ciphertext.size());
            int block_num = ciphertext.size() / 16;
            for (int i = 0; i < block_num; ++i) {
                unsigned char block[16];
                memcpy(block, ciphertext.data() + i * 16, 16);
                aes.decrypt(block, expansionkey, 10);
                memcpy(decrypted.data() + i * 16, block, 16);
            }

            cout << "���ģ�";
            for (auto& c : decrypted) {
                cout << c;
            }
            cout << endl << endl;
        } break;

        case 3:
            system("cls");
            cout << "��ӭ����ʹ�ã��ڴ��´��ټ���\n";
            system("pause");
            return 0;

        default:
            cout << "�Ƿ�ѡ��������ѡ��\n";
            break;
        }
    }

    return 0;
}



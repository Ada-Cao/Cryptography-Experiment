#define _CRT_SECURE_NO_WARNINGS
#include"miracl.h"
#include"mirdef.h"
#include<iostream> 
#include<sstream>
#include<iomanip>
#include<vector>
using namespace std;

int main()
{
    // ��ʼ��1000λ��ʮ���ƴ���ϵͳ
    miracl* mip = mirsys(1000, 10);
    mip->IOBASE = 10;
    
    big m = mirvar(0);      // ����
    big c = mirvar(0);      // ����
    big p = mirvar(0);      // ������p
    big q = mirvar(0);      // ������q
    big n = mirvar(0);      // n=p*q
    big phi_n = mirvar(0);  // n��ŷ��������phi_n=(p-1)*(q-1)
    big d = mirvar(0);      // ˽Կd
    big e = mirvar(0);      // ��Կe
    int select = 1;

    cout << "��ӭ����RSA�ӽ���ϵͳ��\n";
    cout << "tips������10������ʽ���룡\n";
    cout << "**************�˵�*************\n";
    cout << "1��������Կ\n";
    cout << "2������\n";
    cout << "3������\n";
    cout << "4���˳�\n";


    while (select)
    {
        cout << "@��ѡ����Ҫ���еĻ��ţ�";
        cin >> select;

        if (select == 4)
        {
            break;
        }
        else if (select == 1)   // ������Կ
        {
            // ��������������p��q�������
            char input_p[100], input_q[100];
            cout << "@�������������ܴ�����������20λ����\n";
            while(true)
            {
                // �û�����
                cout << " p = ";
                cin >> input_p;
                cinstr(p, input_p);
                cout << " q = ";
                cin >> input_q;
                cinstr(q, input_q);

                // �ж��û�������Ƿ�Ϊ����
                if (isprime(p) && isprime(q))
                {
                    break;
                }
                else
                {
                    cout << " �������p��q��ȫΪ����������������\n";
                }

            }


            // ���ɹ�Կ��˽Կ
            big sub_1 = mirvar(0);
            big p_1 = mirvar(0);
            big q_1 = mirvar(0);
            convert(1, sub_1);
            subtract(p, sub_1, p_1); 
            subtract(q, sub_1, q_1);
            multiply(p, q, n);          // ����õ���Կn
            multiply(p_1, q_1, phi_n);  // ����õ�n��ŷ������
            cout << " n = ";
            cotnum(n, stdout);
            cout << " phi_n = ";
            cotnum(phi_n, stdout);

            cout << "@��ѡ��һ������e��Ϊ��Կ��1 < e < ��(n) �� gcd(��(n),e) = 1)��\n";
            char input_e[100];
            while(true)
            {
                cout << " e = ";
                cin >> input_e;
                cinstr(e, input_e);

                // �ж�e�Ƿ��������������������������
                big gcd = mirvar(0);
                egcd(e, phi_n,gcd);

                if (mr_compare(e, sub_1) > 0 && mr_compare(e, phi_n) < 0 && mr_compare(gcd, sub_1) == 0)
                {
                    break;
                }
                else
                {
                    cout << " �������e����������������������\n ";
                }
            }

            // ����õ�ģphi_n��e����Ԫd
            big d = mirvar(0);
            xgcd(e, phi_n, d, d, d);    // ��չŷ������㷨

            cout << "~��Կ(e,n)��\n";
            cout << " e = ";
            cotnum(e, stdout);
            cout << " n = ";
            cotnum(n, stdout);
            cout << "~��Կd��\n";
            cout << " d = ";
            cotnum(d, stdout);
        }
        else if (select == 2)   // ����
        {
            cout << "@�����빫Կn��";
            char input_n[100];
            cin >> input_n;
            cinstr(n, input_n);
            int block_size = strlen(input_n) - 1;

            cout << "@�����빫Կe��";
            char input_e[100];
            cin >> input_e;
            cinstr(e, input_e);
            cin.ignore();   // ������з�

            cout << "@���������ģ��������룬ÿ��λ��������"<< block_size<<"���Կո�ָ�س�����) ��\n";
            char input_m[1000];
            cin.getline(input_m, 1000);

            // ����
            vector<big> blocks;
            char* token = strtok(input_m, " ");
            while (token != NULL)
            {
                big temp = mirvar(0);
                cinstr(temp, token);
                blocks.push_back(temp);
                token = strtok(NULL, " ");
            }

            // �������
            vector<big> results;
            for (int i = 0; i < blocks.size(); ++i)
            {
                big temp = mirvar(0);
                powmod(blocks[i], e, n, temp);  // ��������
                results.push_back(temp);        // ��ӵ����������
            }

            // �������
            cout << "@���ܺ���Ϊ��";
            for (int i = 0; i < results.size(); ++i)
            {
                char temp[1000];
                cotstr(results[i], temp);
                cout << temp << " ";
            }
            cout << endl;
        }
        else if (select == 3)   // ����
        {
            cout << "@�����빫Կn��";
            char input_n[100];
            cin >> input_n;
            cinstr(n, input_n);
            int block_size = strlen(input_n) - 1;

            cout << "@��������Կd��";
            char input_d[100];
            cin >> input_d;
            cinstr(d, input_d);
            cin.ignore();   // ������з�

            cout << "@���������ģ��������룬ÿ��λ��������" << block_size << "���Կո�ָ�س�����) ��\n";
            char input_c[1000];
            cin.getline(input_c, 1000);
            // ����
            vector<big> blocks;
            char* token = strtok(input_c, " ");
            while (token != NULL)
            {
                big temp = mirvar(0);
                cinstr(temp, token);
                blocks.push_back(temp);
                token = strtok(NULL, " ");
            }

            // �������
            vector<big> results;
            for (int i = 0; i < blocks.size(); ++i)
            {
                big temp = mirvar(0);
                powmod(blocks[i], d, n, temp);  // ��������
                results.push_back(temp);        // ��ӵ����������
            }

            // �������
            cout << "@���ܺ���Ϊ��";
            for (int i = 0; i < results.size(); ++i)
            {
                char temp[100];
                cotstr(results[i], temp);
                cout << temp << " ";
            }
            cout << endl;
        }
        else
        {
            cout << "����������������������\n";
        }
    }

    mirexit();  // �ͷŴ����ռ�
    system("cls");
    cout << "��ӭ����ʹ�ã��ڴ��´��ټ���\n";
    system("pause");
	return 0;
}
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
    // 初始化1000位的十进制大数系统
    miracl* mip = mirsys(1000, 10);
    mip->IOBASE = 10;
    
    big m = mirvar(0);      // 明文
    big c = mirvar(0);      // 密文
    big p = mirvar(0);      // 大素数p
    big q = mirvar(0);      // 大素数q
    big n = mirvar(0);      // n=p*q
    big phi_n = mirvar(0);  // n的欧拉函数，phi_n=(p-1)*(q-1)
    big d = mirvar(0);      // 私钥d
    big e = mirvar(0);      // 公钥e
    int select = 1;

    cout << "欢迎来到RSA加解密系统！\n";
    cout << "tips：均以10进制形式输入！\n";
    cout << "**************菜单*************\n";
    cout << "1、生成密钥\n";
    cout << "2、加密\n";
    cout << "3、解密\n";
    cout << "4、退出\n";


    while (select)
    {
        cout << "@请选择你要进行的活动序号：";
        cin >> select;

        if (select == 4)
        {
            break;
        }
        else if (select == 1)   // 生成密钥
        {
            // 输入两个大素数p、q，并检查
            char input_p[100], input_q[100];
            cout << "@请输入两个保密大素数（至少20位）：\n";
            while(true)
            {
                // 用户输入
                cout << " p = ";
                cin >> input_p;
                cinstr(p, input_p);
                cout << " q = ";
                cin >> input_q;
                cinstr(q, input_q);

                // 判断用户输入的是否为素数
                if (isprime(p) && isprime(q))
                {
                    break;
                }
                else
                {
                    cout << " ！输入的p、q不全为素数，请重新输入\n";
                }

            }


            // 生成公钥和私钥
            big sub_1 = mirvar(0);
            big p_1 = mirvar(0);
            big q_1 = mirvar(0);
            convert(1, sub_1);
            subtract(p, sub_1, p_1); 
            subtract(q, sub_1, q_1);
            multiply(p, q, n);          // 计算得到公钥n
            multiply(p_1, q_1, phi_n);  // 计算得到n的欧拉函数
            cout << " n = ";
            cotnum(n, stdout);
            cout << " phi_n = ";
            cotnum(phi_n, stdout);

            cout << "@请选择一个整数e作为公钥（1 < e < φ(n) 且 gcd(φ(n),e) = 1)：\n";
            char input_e[100];
            while(true)
            {
                cout << " e = ";
                cin >> input_e;
                cinstr(e, input_e);

                // 判断e是否符合条件，不符合则重新输入
                big gcd = mirvar(0);
                egcd(e, phi_n,gcd);

                if (mr_compare(e, sub_1) > 0 && mr_compare(e, phi_n) < 0 && mr_compare(gcd, sub_1) == 0)
                {
                    break;
                }
                else
                {
                    cout << " ！输入的e不符合条件，请重新输入\n ";
                }
            }

            // 计算得到模phi_n下e的逆元d
            big d = mirvar(0);
            xgcd(e, phi_n, d, d, d);    // 扩展欧几里得算法

            cout << "~公钥(e,n)：\n";
            cout << " e = ";
            cotnum(e, stdout);
            cout << " n = ";
            cotnum(n, stdout);
            cout << "~密钥d：\n";
            cout << " d = ";
            cotnum(d, stdout);
        }
        else if (select == 2)   // 加密
        {
            cout << "@请输入公钥n：";
            char input_n[100];
            cin >> input_n;
            cinstr(n, input_n);
            int block_size = strlen(input_n) - 1;

            cout << "@请输入公钥e：";
            char input_e[100];
            cin >> input_e;
            cinstr(e, input_e);
            cin.ignore();   // 清除换行符

            cout << "@请输入明文（分组输入，每组位数不多于"<< block_size<<"，以空格分割，回车结束) ：\n";
            char input_m[1000];
            cin.getline(input_m, 1000);

            // 分组
            vector<big> blocks;
            char* token = strtok(input_m, " ");
            while (token != NULL)
            {
                big temp = mirvar(0);
                cinstr(temp, token);
                blocks.push_back(temp);
                token = strtok(NULL, " ");
            }

            // 分组加密
            vector<big> results;
            for (int i = 0; i < blocks.size(); ++i)
            {
                big temp = mirvar(0);
                powmod(blocks[i], e, n, temp);  // 计算密文
                results.push_back(temp);        // 添加到结果向量中
            }

            // 输出密文
            cout << "@加密后结果为：";
            for (int i = 0; i < results.size(); ++i)
            {
                char temp[1000];
                cotstr(results[i], temp);
                cout << temp << " ";
            }
            cout << endl;
        }
        else if (select == 3)   // 解密
        {
            cout << "@请输入公钥n：";
            char input_n[100];
            cin >> input_n;
            cinstr(n, input_n);
            int block_size = strlen(input_n) - 1;

            cout << "@请输入密钥d：";
            char input_d[100];
            cin >> input_d;
            cinstr(d, input_d);
            cin.ignore();   // 清除换行符

            cout << "@请输入密文（分组输入，每组位数不多于" << block_size << "，以空格分割，回车结束) ：\n";
            char input_c[1000];
            cin.getline(input_c, 1000);
            // 分组
            vector<big> blocks;
            char* token = strtok(input_c, " ");
            while (token != NULL)
            {
                big temp = mirvar(0);
                cinstr(temp, token);
                blocks.push_back(temp);
                token = strtok(NULL, " ");
            }

            // 分组解密
            vector<big> results;
            for (int i = 0; i < blocks.size(); ++i)
            {
                big temp = mirvar(0);
                powmod(blocks[i], d, n, temp);  // 计算明文
                results.push_back(temp);        // 添加到结果向量中
            }

            // 输出明文
            cout << "@解密后结果为：";
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
            cout << "！序号输入错误，请重新输入\n";
        }
    }

    mirexit();  // 释放大数空间
    system("cls");
    cout << "欢迎您的使用，期待下次再见！\n";
    system("pause");
	return 0;
}
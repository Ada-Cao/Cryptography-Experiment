#define _CRT_SECURE_NO_WARNINGS
#include"miracl.h"
#include"mirdef.h"
#include<iostream> 
#include<string>
#include<ctime>
#include<chrono>
#include<ctime>
#include <thread> 
using namespace std;
using namespace std::chrono;

#define Round 50

// 全局变量
miracl* mip = mirsys(2000, 10);
big p, q, g;	// 公共参数
big x, y;		// 秘密钥和公开钥
big r, s;		// 签名结果

// 签名算法的实现和验证
void generatePublicParam();	// 生成公共参数
void generateKey();			// 生成秘密钥和公开钥
void signMessage(const string& m);			// 对信息签名
void verifySignature(const string& m, int choice = 0);		// 对签名验证
big Hash(const string& m);	// 对信息哈希


int main()
{
	cout << "欢迎来到DSA签名系统！\n";
	cout << "tips：参数都以十进制形式呈现！\n";
	cout << "**************菜单*************\n";
	cout << "1、签名并验证\n";
	cout << "2、测试算法平均运行时间\n";
	cout << "3、退出\n";
	int select = 0;


	while (true)
	{
		cout << "@请选择你要进行的活动序号：";
		cin >> select;


		if (select == 3)
		{
			break;
		}
		else if (select == 1)
		{
			cout << "@请输入签名的信息：";
			cin.ignore();	// 清除上一个输入的换行符
			string message;
			getline(cin, message);


			cout << "Ⅰ. 公共参数p、q、g生成中…………\n";
			// 随机种子，确保每次生成的公开参数、秘密钥和公开钥是不一样的
			irand(time(NULL));
			auto start1 = high_resolution_clock::now();
			generatePublicParam();
			auto end1 = high_resolution_clock::now();
			cout << "# 公共参数（p. q, g）：\n";
			cout << "~p："; cotnum(p, stdout);
			cout << "~q："; cotnum(q, stdout);
			cout << "~g："; cotnum(g, stdout);
			cout << endl;


			cout << "Ⅱ. 秘密钥X、公开钥Y生成中…………\n";
			auto start2 = high_resolution_clock::now();
			generateKey();
			auto end2 = high_resolution_clock::now();
			cout << "~x："; cotnum(x, stdout);
			cout << "~y："; cotnum(y, stdout);
			cout << endl;


			cout << "Ⅲ. 信息签名中…………\n";
			auto start3 = high_resolution_clock::now();
			signMessage(message);
			auto end3 = high_resolution_clock::now();
			cout << "# 签名信息（r. s）：\n";
			cout << "~r："; cotnum(r, stdout);
			cout << "~s："; cotnum(s, stdout);
			cout << endl;


			cout << "Ⅳ. 签名验证中…………\n";
			auto start4 = high_resolution_clock::now();
			verifySignature(message);
			auto end4 = high_resolution_clock::now();
			cout << endl;

			cout << "~公共参数生成耗时: " << duration<double, milli>(end1 - start1).count() << " ms\n";
			cout << "~秘密钥公开钥生成耗时: " << duration<double, milli>(end2 - start2).count() << " ms\n";
			cout << "~签名耗时: " << duration<double, milli>(end3 - start3).count() << " ms\n";
			cout << "~验证耗时: " << duration<double, milli>(end4 - start4).count() << " ms\n\n";

		}
		else if (select == 2)
		{ 
			double time1 = 0.0;
			double time2 = 0.0;
			double time3 = 0.0;
			double time4 = 0.0;

			cout << "@请输入签名的信息：";
			cin.ignore();	// 清除上一个输入的换行符
			string message;
			getline(cin, message);

			cout << "测试中……………………\n";
			irand(time(NULL));

			for (int i = 0; i < Round; ++i)
			{
				auto start1 = high_resolution_clock::now();
				generatePublicParam();
				auto end1 = high_resolution_clock::now();

				auto start2 = high_resolution_clock::now();
				generateKey();
				auto end2 = high_resolution_clock::now();

				auto start3 = high_resolution_clock::now();
				signMessage(message);
				auto end3 = high_resolution_clock::now();

				auto start4 = high_resolution_clock::now();
				verifySignature(message, 1);
				auto end4 = high_resolution_clock::now();

				time1 += duration<double, milli>(end1 - start1).count();
				time2 += duration<double, milli>(end2 - start2).count();
				time3 += duration<double, milli>(end3 - start3).count();
				time4 += duration<double, milli>(end4 - start4).count();

				std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 暂停1毫秒
			}

			cout << "~公共参数生成耗时: " << time1/ Round << " ms\n";
			cout << "~秘密钥公开钥生成耗时: " << time2 / Round << " ms\n";
			cout << "~签名耗时: " << time3 / Round << " ms\n";
			cout << "~验证耗时: " << time4 / Round << " ms\n\n";
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

void generatePublicParam()
{
	p = mirvar(0);
	q = mirvar(0);
	g = mirvar(0);
	big temp = mirvar(0);
	big h = mirvar(0);
	big p_1 = mirvar(0);
	big one = mirvar(0);
	convert(1, one);
	
	// 随机生成160位的素数q
	forever
	{
		bigbits(160,q);
		if (isprime(q))
		{
			break;
		}
	}

	// 根据选取的q生成大素数p
	// p的位数需要是64的倍数，且在512-1024位之间（包含）
	// 在此我们选取512位的大素数p，p=kq+1
	forever
	{
		bigbits(352,temp);
		multiply(temp, q, p);
		incr(p, 1, p);
		if (isprime(p))
		{
			break;
		}
	}

	// 生成g
	forever
	{
		bigbits(512,h);
		// 计算指数
		incr(p, 1, p_1);
		divide(p_1, q, temp);
		powmod(h, temp, p, g);

		// 保证g比1大
		if (mr_compare(g, one) == 1)
		{
			break;
		}
	}

}

void generateKey()
{
	x = mirvar(0);
	y = mirvar(0);

	// 生成用户的秘密钥x，满足0<x<q的随机数
	bigrand(q, x);
	// 计算用户的公开钥y，y = g^x mod p
	powmod(g, x, p, y);
}

void signMessage(const string& m)
{
	s = mirvar(0);
	r = mirvar(0);

	big k = mirvar(0);
	big k_1 = mirvar(0);
	big H_m = mirvar(0);
	big temp_k = mirvar(0);
	big temp_s1 = mirvar(0);
	big temp_s2 = mirvar(0);
	big zero = mirvar(0);


	H_m = Hash(m);

	forever
	{
		bigrand(q,k);	// 0 < k < q
		powmod(g, k, p, temp_k);	// temp = g^k mod p
		divide(temp_k, q, q);	// r = (g^k mod p) mod	q
		if (mr_compare(temp_k , zero) == 0)
		{
			continue;
		}
		copy(temp_k, r);

		
		// 计算得到模q下k的逆元k_1
		xgcd(k, q, k_1, k_1, k_1);    // 扩展欧几里得算法
	
		// s = k^-1 * (H(m) + x*r) mod q
		multiply(x, r, temp_s1);
		add(H_m, temp_s1, temp_s1);
		divide(temp_s1, q, q);
		multiply(k_1, temp_s1, temp_s2);
		divide(temp_s2, q, q);

		if (mr_compare(temp_s2,zero) == 1)
		{
			copy(temp_s2, s);
			break;
		}
	}

}

void verifySignature(const string& m, int choice)
{
	big s_ = mirvar(0);
	big r_ = mirvar(0);
	copy(s, s_);
	copy(r, r_);

	// 计算w = s^-1 mod q
	big w = mirvar(0);
	xgcd(s_, q, w, w, w);

	// 计算消息的哈希值
	big H_m = mirvar(0);
	H_m = Hash(m);

	// 计算u1 = (H(m) * w) mod q
	big u1 = mirvar(0);
	multiply(H_m, w, u1);
	divide(u1, q, q);
	
	// 计算u2 = (r * w) mod q
	big u2 = mirvar(0);
	multiply(r, w, u2);
	divide(u2, q, q);

	// 计算v = (g^u1 * y^u2 mod p) mod q
	big v = mirvar(0);
	big temp1 = mirvar(0);
	powmod(g, u1, p, temp1);
	big temp2 = mirvar(0);
	powmod(y, u2, p, temp2);
	multiply(temp1, temp2, v);
	divide(v, p, p);
	divide(v, q, q);

	// 验证签名是否正确
	if (choice == 0)
	{
		cout << "~"; cotnum(v, stdout);

		if (mr_compare(v, r_) == 0)
		{
			cout << "签名认证成功！\n";
		}
		else
		{
			cout << "签名认证失败！\n";
		}
	}


}

big Hash(const string& m)
{
	big result = mirvar(0);
	
	// 定义SHA-1上下文结构体
	sha sh;
	// 用于保存最终哈希的结果，20字节=160bit
	char hash_output[20];
	// 初始化
	shs_init(&sh);
	// 将字符串m中的每个字符传给sha
	for (size_t i = 0; i < m.length(); ++i)
	{
		shs_process(&sh, m[i]);
	}
	// 计算最终的哈希值
	shs_hash(&sh,hash_output);

	// 转换为大数类型
	bytes_to_big(20, hash_output, result);

	return result;
}


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

// ȫ�ֱ���
miracl* mip = mirsys(2000, 10);
big p, q, g;	// ��������
big x, y;		// ����Կ�͹���Կ
big r, s;		// ǩ�����

// ǩ���㷨��ʵ�ֺ���֤
void generatePublicParam();	// ���ɹ�������
void generateKey();			// ��������Կ�͹���Կ
void signMessage(const string& m);			// ����Ϣǩ��
void verifySignature(const string& m, int choice = 0);		// ��ǩ����֤
big Hash(const string& m);	// ����Ϣ��ϣ


int main()
{
	cout << "��ӭ����DSAǩ��ϵͳ��\n";
	cout << "tips����������ʮ������ʽ���֣�\n";
	cout << "**************�˵�*************\n";
	cout << "1��ǩ������֤\n";
	cout << "2�������㷨ƽ������ʱ��\n";
	cout << "3���˳�\n";
	int select = 0;


	while (true)
	{
		cout << "@��ѡ����Ҫ���еĻ��ţ�";
		cin >> select;


		if (select == 3)
		{
			break;
		}
		else if (select == 1)
		{
			cout << "@������ǩ������Ϣ��";
			cin.ignore();	// �����һ������Ļ��з�
			string message;
			getline(cin, message);


			cout << "��. ��������p��q��g�����С�������\n";
			// ������ӣ�ȷ��ÿ�����ɵĹ�������������Կ�͹���Կ�ǲ�һ����
			irand(time(NULL));
			auto start1 = high_resolution_clock::now();
			generatePublicParam();
			auto end1 = high_resolution_clock::now();
			cout << "# ����������p. q, g����\n";
			cout << "~p��"; cotnum(p, stdout);
			cout << "~q��"; cotnum(q, stdout);
			cout << "~g��"; cotnum(g, stdout);
			cout << endl;


			cout << "��. ����ԿX������ԿY�����С�������\n";
			auto start2 = high_resolution_clock::now();
			generateKey();
			auto end2 = high_resolution_clock::now();
			cout << "~x��"; cotnum(x, stdout);
			cout << "~y��"; cotnum(y, stdout);
			cout << endl;


			cout << "��. ��Ϣǩ���С�������\n";
			auto start3 = high_resolution_clock::now();
			signMessage(message);
			auto end3 = high_resolution_clock::now();
			cout << "# ǩ����Ϣ��r. s����\n";
			cout << "~r��"; cotnum(r, stdout);
			cout << "~s��"; cotnum(s, stdout);
			cout << endl;


			cout << "��. ǩ����֤�С�������\n";
			auto start4 = high_resolution_clock::now();
			verifySignature(message);
			auto end4 = high_resolution_clock::now();
			cout << endl;

			cout << "~�����������ɺ�ʱ: " << duration<double, milli>(end1 - start1).count() << " ms\n";
			cout << "~����Կ����Կ���ɺ�ʱ: " << duration<double, milli>(end2 - start2).count() << " ms\n";
			cout << "~ǩ����ʱ: " << duration<double, milli>(end3 - start3).count() << " ms\n";
			cout << "~��֤��ʱ: " << duration<double, milli>(end4 - start4).count() << " ms\n\n";

		}
		else if (select == 2)
		{ 
			double time1 = 0.0;
			double time2 = 0.0;
			double time3 = 0.0;
			double time4 = 0.0;

			cout << "@������ǩ������Ϣ��";
			cin.ignore();	// �����һ������Ļ��з�
			string message;
			getline(cin, message);

			cout << "�����С���������������\n";
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

				std::this_thread::sleep_for(std::chrono::milliseconds(1)); // ��ͣ1����
			}

			cout << "~�����������ɺ�ʱ: " << time1/ Round << " ms\n";
			cout << "~����Կ����Կ���ɺ�ʱ: " << time2 / Round << " ms\n";
			cout << "~ǩ����ʱ: " << time3 / Round << " ms\n";
			cout << "~��֤��ʱ: " << time4 / Round << " ms\n\n";
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
	
	// �������160λ������q
	forever
	{
		bigbits(160,q);
		if (isprime(q))
		{
			break;
		}
	}

	// ����ѡȡ��q���ɴ�����p
	// p��λ����Ҫ��64�ı���������512-1024λ֮�䣨������
	// �ڴ�����ѡȡ512λ�Ĵ�����p��p=kq+1
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

	// ����g
	forever
	{
		bigbits(512,h);
		// ����ָ��
		incr(p, 1, p_1);
		divide(p_1, q, temp);
		powmod(h, temp, p, g);

		// ��֤g��1��
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

	// �����û�������Կx������0<x<q�������
	bigrand(q, x);
	// �����û��Ĺ���Կy��y = g^x mod p
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

		
		// ����õ�ģq��k����Ԫk_1
		xgcd(k, q, k_1, k_1, k_1);    // ��չŷ������㷨
	
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

	// ����w = s^-1 mod q
	big w = mirvar(0);
	xgcd(s_, q, w, w, w);

	// ������Ϣ�Ĺ�ϣֵ
	big H_m = mirvar(0);
	H_m = Hash(m);

	// ����u1 = (H(m) * w) mod q
	big u1 = mirvar(0);
	multiply(H_m, w, u1);
	divide(u1, q, q);
	
	// ����u2 = (r * w) mod q
	big u2 = mirvar(0);
	multiply(r, w, u2);
	divide(u2, q, q);

	// ����v = (g^u1 * y^u2 mod p) mod q
	big v = mirvar(0);
	big temp1 = mirvar(0);
	powmod(g, u1, p, temp1);
	big temp2 = mirvar(0);
	powmod(y, u2, p, temp2);
	multiply(temp1, temp2, v);
	divide(v, p, p);
	divide(v, q, q);

	// ��֤ǩ���Ƿ���ȷ
	if (choice == 0)
	{
		cout << "~"; cotnum(v, stdout);

		if (mr_compare(v, r_) == 0)
		{
			cout << "ǩ����֤�ɹ���\n";
		}
		else
		{
			cout << "ǩ����֤ʧ�ܣ�\n";
		}
	}


}

big Hash(const string& m)
{
	big result = mirvar(0);
	
	// ����SHA-1�����Ľṹ��
	sha sh;
	// ���ڱ������չ�ϣ�Ľ����20�ֽ�=160bit
	char hash_output[20];
	// ��ʼ��
	shs_init(&sh);
	// ���ַ���m�е�ÿ���ַ�����sha
	for (size_t i = 0; i < m.length(); ++i)
	{
		shs_process(&sh, m[i]);
	}
	// �������յĹ�ϣֵ
	shs_hash(&sh,hash_output);

	// ת��Ϊ��������
	bytes_to_big(20, hash_output, result);

	return result;
}


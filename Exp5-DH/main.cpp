#include "DH.h"
#include<iostream>
using namespace std;

int main()
{
	cout << "��ӭ����Diffie-Hellman��Կ����ϵͳ��\n";
	cout << "tips������10������ʽ���������\n";
	cout << "*****************�˵�****************\n";
	cout << "1����������Ԫa\n";
	cout << "2��ѡȡ��Կ��˽Կ\n";
	cout << "3�����㹲����Կ\n";
	cout << "4���˳�\n";

	int select = 1;
	DH dh;
	while (select) 
	{
		cout << "@��ѡ����Ҫ���еĻ��ţ�";
		cin >> select;

		if (select == 4)
		{
			break;
		}
		else if (select == 1)	// ѡȡ������p����������Ԫa
		{
			LL p = 2;
			cout << "@������������GF(p)�д�����p��ֵ��";
			while (true)
			{
				cin >> p;
				if (p == 1 || p == 2)
				{
					cout << "���������p��Ϊ���������������룺";
				}
				else if (dh.isPrime(p))
				{
					break;
				}
				else
				{
					cout << "���������p��Ϊ���������������룺";
				}
			}

			// ����ŷ������������Ԫ
			LL phi_n = p - 1;
			vector<LL> factors;
			factors = dh.getFactors(phi_n);
			LL a = dh.generatePrimitiveRoot(factors, p);
			cout << "~����Ԫ��(p, a) = (" << p << ", " << a << ")\n";
		}
		else if (select == 2)
		{
			LL XA = 1;
			LL XB = 1;
			LL a = 2;
			LL p = 2;
			cout << "@�����빫��Ԫ��(p, a)��";
			cin >> p >> a;
			cout << "@�û�A��ѡ��˽ԿXa��";
			cin >> XA;
			cout << "@�û�B��ѡ��˽ԿXb��";
			cin >> XB;
			LL YA = dh.quickpow(a, XA, p);
			LL YB = dh.quickpow(a, XB, p);
			cout << "~�û�A�Ĺ�Կ��˽Կ(Ya, Xa)��(" << YA << " ," << XA << ")\n";
			cout << "~�û�B�Ĺ�Կ��˽Կ(Yb, Xb)��(" << YB << " ," << XB << ")\n";
		}
		else if (select == 3)
		{
			LL p = 2;
			LL X = 1;
			LL Y = 1;
			cout << "@�����빫��Ԫ��p��";
			cin >> p;
			cout << "@�������Լ���˽Կ�ͶԷ��Ĺ�Կ(X, Y)��";
			cin >> X >> Y;
			LL K = dh.quickpow(Y, X, p);
			cout << "~������ԿK = " << K << endl;
		}
		else
		{
			cout << "@������������ţ�\n";
		}
	}

	system("cls");
	cout << "��ӭ����ʹ�ã��ڴ��´��ټ���\n";

	system("pause");
	return 0;
}


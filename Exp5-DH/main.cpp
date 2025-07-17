#include "DH.h"
#include<iostream>
using namespace std;

int main()
{
	cout << "欢迎来到Diffie-Hellman密钥交换系统！\n";
	cout << "tips：均以10进制形式输入输出！\n";
	cout << "*****************菜单****************\n";
	cout << "1、计算生成元a\n";
	cout << "2、选取公钥和私钥\n";
	cout << "3、计算共享密钥\n";
	cout << "4、退出\n";

	int select = 1;
	DH dh;
	while (select) 
	{
		cout << "@请选择你要进行的活动序号：";
		cin >> select;

		if (select == 4)
		{
			break;
		}
		else if (select == 1)	// 选取大素数p并计算生成元a
		{
			LL p = 2;
			cout << "@请输入有限域GF(p)中大素数p的值：";
			while (true)
			{
				cin >> p;
				if (p == 1 || p == 2)
				{
					cout << "！！输入的p不为素数，请重新输入：";
				}
				else if (dh.isPrime(p))
				{
					break;
				}
				else
				{
					cout << "！！输入的p不为素数，请重新输入：";
				}
			}

			// 计算欧拉函数和生成元
			LL phi_n = p - 1;
			vector<LL> factors;
			factors = dh.getFactors(phi_n);
			LL a = dh.generatePrimitiveRoot(factors, p);
			cout << "~公开元素(p, a) = (" << p << ", " << a << ")\n";
		}
		else if (select == 2)
		{
			LL XA = 1;
			LL XB = 1;
			LL a = 2;
			LL p = 2;
			cout << "@请输入公开元素(p, a)：";
			cin >> p >> a;
			cout << "@用户A请选择私钥Xa：";
			cin >> XA;
			cout << "@用户B请选择私钥Xb：";
			cin >> XB;
			LL YA = dh.quickpow(a, XA, p);
			LL YB = dh.quickpow(a, XB, p);
			cout << "~用户A的公钥和私钥(Ya, Xa)：(" << YA << " ," << XA << ")\n";
			cout << "~用户B的公钥和私钥(Yb, Xb)：(" << YB << " ," << XB << ")\n";
		}
		else if (select == 3)
		{
			LL p = 2;
			LL X = 1;
			LL Y = 1;
			cout << "@请输入公开元素p：";
			cin >> p;
			cout << "@请输入自己的私钥和对方的公钥(X, Y)：";
			cin >> X >> Y;
			LL K = dh.quickpow(Y, X, p);
			cout << "~共享密钥K = " << K << endl;
		}
		else
		{
			cout << "@请重新输入序号！\n";
		}
	}

	system("cls");
	cout << "欢迎您的使用，期待下次再见！\n";

	system("pause");
	return 0;
}


#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;

typedef long long int LL;

class DH
{
public:
	bool isPrime(const LL&);	// 判断是否为素数
	vector<LL> getFactors(const LL&);	// 获取欧拉函数的所有因子
	LL quickmul(const LL&, const LL&, const LL&);	// 快速乘
	LL quickpow(LL, LL, const LL&);	// 快速幂
	int generatePrimitiveRoot(const vector<LL>&, const LL&);	// 计算本原根
	

};

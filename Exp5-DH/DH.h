#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;

typedef long long int LL;

class DH
{
public:
	bool isPrime(const LL&);	// �ж��Ƿ�Ϊ����
	vector<LL> getFactors(const LL&);	// ��ȡŷ����������������
	LL quickmul(const LL&, const LL&, const LL&);	// ���ٳ�
	LL quickpow(LL, LL, const LL&);	// ������
	int generatePrimitiveRoot(const vector<LL>&, const LL&);	// ���㱾ԭ��
	

};

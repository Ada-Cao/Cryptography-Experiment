#include "DH.h"

bool DH::isPrime(const LL& x)
{
    // �Գ������Ч��
    if (x <= 1)
    {
        return false;
    }
    if (x == 2 || x == 3)
    {
        return true;
    }
    if (x % 2 == 0 || x % 3 == 0)
    {
        return false;
    }
    
    // ���6k+1/-1������
    for (LL i = 5; i*i <= x; i+=6)
    {
        if (x % i == 0 || x % (i+2) == 0)
        {
            return false;
        }
    }
    return true;
}


vector<LL> DH::getFactors(const LL& x)
{
    vector<LL> factors;
    // �ɶ�������ӣ�����ƽ������
    for (LL i = 1; i*i <= x; ++i)
    {
        if (x % i == 0)
        {
            factors.push_back(i);

            // �����ظ����ƽ����
            if (i != x / i) 
            {
                factors.push_back(x / i);
            }
        }
    }

    // �����ӽ�������
    sort(factors.begin(), factors.end());
    return factors;
}

LL DH::quickmul(const LL& a, const LL& b, const LL& mod)
{
    // ������������
    LL d = (long double)a / mod * b;
    // ����õ�a*bȡģ�������������Ϊ����
    LL res = (unsigned long long)a * b - (unsigned long long)d * mod;

    return (res + mod) % mod;
}


LL DH::quickpow(LL a, LL n, const LL& mod)
{
    LL res = 1;
    while (n)
    {
        // �ж�nΪ��������ż��
        if (n & 1)
        {
            // a*res �� mod ȡģ
            res = quickmul(res, a, mod);
        }

        // a*a �� mod ȡģ
        a = quickmul(a, a, mod);
        // ����ƶ�һλ��ֱ��n=0
        n >>= 1;
    }
    return res;
}

int DH::generatePrimitiveRoot(const vector<LL>& factors, const LL& mod)
{
    int a = 0;
    for (int i = 2; i <= 50; ++i)
    {
        bool flag = false;

        // ��������i^j�η���jΪŷ���������������ӣ�������û��=1�ģ���Ϊ��ԭ��
        for (size_t j = 0; j < factors.size() -1; ++j)
        {
            if (quickpow(i, factors[j], mod) == 1)
            {
                flag = false;
                break;
            }

            flag = true;
        }

        if (flag)
        {
            a = i;
            break;
        }
    }

    return a;
}



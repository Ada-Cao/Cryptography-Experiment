#include "DH.h"

bool DH::isPrime(const LL& x)
{
    // 试除法提高效率
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
    
    // 检查6k+1/-1的因子
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
    // 成对添加因子（除了平方根）
    for (LL i = 1; i*i <= x; ++i)
    {
        if (x % i == 0)
        {
            factors.push_back(i);

            // 避免重复添加平方根
            if (i != x / i) 
            {
                factors.push_back(x / i);
            }
        }
    }

    // 对因子进行排序
    sort(factors.begin(), factors.end());
    return factors;
}

LL DH::quickmul(const LL& a, const LL& b, const LL& mod)
{
    // 计算整数部分
    LL d = (long double)a / mod * b;
    // 计算得到a*b取模后的余数，可能为负数
    LL res = (unsigned long long)a * b - (unsigned long long)d * mod;

    return (res + mod) % mod;
}


LL DH::quickpow(LL a, LL n, const LL& mod)
{
    LL res = 1;
    while (n)
    {
        // 判断n为奇数还是偶数
        if (n & 1)
        {
            // a*res 对 mod 取模
            res = quickmul(res, a, mod);
        }

        // a*a 对 mod 取模
        a = quickmul(a, a, mod);
        // 向后移动一位，直到n=0
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

        // 计算所有i^j次方，j为欧拉函数的所有因子，若其中没有=1的，则为本原根
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



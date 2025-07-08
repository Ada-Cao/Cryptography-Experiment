#include "DES.h"

// 构造函数default
DES::DES()
{

}


// 加密
array<int, 64> DES::encrypt(const array<int, 64>& plaintext)
{
	// 1、初始置换IP
	array<int, 64> permutedText = permutate_IP(plaintext);

	// 2、16轮变换，并交换左右次序
	for (int i = 1; i <= 16; ++i)
	{
		permutedText = feistelFunction(permutedText, i);
	}
	array<int, 64> temp = changeOrder(permutedText);

	//3、逆初始置换IP_1
	array<int, 64> result = permutate_IP_1(temp);

	return result;
}


// 解密
array<int, 64> DES::decrypt(const array<int, 64>& ciphertext)
{
	// 1、初始置换IP
	array<int, 64> permutedText = permutate_IP(ciphertext);

	// 2、16轮变换，并交换左右次序（子密钥使用顺序和加密相反）
	for (int i = 1; i <= 16; ++i)
	{
		permutedText = feistelFunction(permutedText, 17 - i);
	}
	array<int, 64> temp = changeOrder(permutedText);

	//3、逆初始置换IP_1
	array<int, 64> result = permutate_IP_1(temp);

	return result;
}



// 明文初始置换IP
array<int, 64> DES::permutate_IP(const array<int, 64>& input)
{
	// 将置换结果初始化
	array<int, 64> result = { 0 };
	for (int i = 0; i < 64; ++i)
	{
		result[i] = input[IP[i] - 1];
	}
	return result;
}


// 明文逆初始置换IP_1;
array<int, 64> DES::permutate_IP_1 (const array<int, 64>& input)
{
	// 将逆置换结果初始化
	array<int, 64> result = { 0 };
	for (int i = 0; i < 64; ++i)
	{
		result[i] = input[IP_1[i] - 1];
	}
	return result;
}


// 轮函数，共需调用16次
// 注明：round从1-16
array<int, 64> DES::feistelFunction(const array<int, 64>& input ,int round)
{
	// 防止数组越界
	if (round < 1 || round > 16)
	{
		throw out_of_range("轮数越界！");
	}

	// 将64bit输入分为左右各32bit
	array<int, 32> left = getLeft_32(input);
	array<int, 32> right = getRight_32(input);

	//右边32bit进行E表扩展为48bit，并和本轮子密钥异或
	array<int, 48> temp1 = XOR_48(expand_E(right), subKeys[round - 1]);
	
	//经过S盒代换选择后输出32bit，这32bit再进行一次P置换，最后和原先左边的32bit异或得到结果
	array<int, 32> temp2 = XOR_32(left, permute_P(substitute_S(temp1)));

	// 右边32bit直接作为下一轮的左边32bit
	array<int, 64> result = combineLeftRight_32(right,temp2);
	
	return result;
}


// 生成子密钥并派发
void DES::generateSubKeys(const array<int, 64>& original_key)
{
	// 先进行置换选择1，并分为左右两半各28bit
	array<int, 56> permutedKey = permutate_PC_1(original_key);
	array<int, 28> left = getLeft_28(permutedKey);
	array<int, 28> right = getRight_28(permutedKey);

	// 共进行16轮子密钥生成
	for (int i = 1; i <= 16; ++i)
	{
		// 进行左右循环移位
		left = leftShift(left, i);
		right = leftShift(right, i);

		// 进行置换选择2并将结果保存在子密钥容器里
		subKeys.push_back(permutate_PC_2(combineLeftRight_28(left, right)));
	}
}


// 获取左边32bit
array<int, 32> DES::getLeft_32(const array<int, 64>& input)
{
	array<int, 32> left = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		left[i] = input[i];
	}
	return left;
}


// 获取右边32bit
array<int, 32> DES::getRight_32(const array<int, 64>& input)
{
	array<int, 32> right = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		right[i] = input[i + 32];
	}
	return right;
}


// 交换左右32bit的顺序
array<int, 64> DES::changeOrder(const array<int, 64>& input)
{
	array<int, 64> result = { 0 };
	// 每一次赋值第i个和第i+32个
	for (int i = 0; i < 32; ++i)
	{
		result[i] = input[i + 32];
		result[i + 32] = input[i];
	}
	return result;
}


//  将左右各32bit拼接为64bit
array<int, 64> DES::combineLeftRight_32(const array<int, 32>& left, const array<int, 32>& right)
{
	array<int, 64> result = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		result[i] = left[i];
		result[i + 32] = right[i];
	}
	return  result;
}


// 扩展轮输入的右半部分R(32bit->48bit)
array<int, 48> DES::expand_E(const array<int, 32>& input)
{
	array<int, 48> result = { 0 };
	for (int i = 0; i < 48; ++i)
	{
		result[i] = input[E[i] - 1];
	}
	return result;
}


// S盒代换选择(48bit->32bit)
array<int, 32> DES::substitute_S(const array<int, 48>& input)
{
	array<int, 32> result = { 0 };
	for (int i = 0; i < 8; ++i)
	{
		// 共分为8组，取第一个和第六个bit组成的二进制数为行号，第二个到第五个bit组成的二进制数为列号
		int row = input[i * 6] * 2 + input[i * 6 + 5] * 1;
		int col = input[i * 6 + 1] * 8 + input[i * 6 + 2] * 4 + input[i * 6 + 3] * 2 + input[i * 6 + 4] * 1;
		int num = S[i][row][col];	// 十进制

		// 将S表中的十进制结果转换成二进制，并按顺序储存在数组中
		for (int j = 0; j < 4; ++j)
		{
			result[i * 4 + 3 - j] = num % 2;
			num /= 2;
		}
	}
	return result;
}


// 置换运算P
array<int, 32> DES::permute_P(const array<int, 32>& input)
{
	array<int, 32> result = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		result[i] = input[P[i] - 1];
	}
	return result;
}


// 48bit异或
array<int, 48> DES::XOR_48(const array<int, 48>& input1, const array<int, 48>& input2)
{
	array<int, 48> result = { 0 };
	for (int i = 0; i < 48; ++i)
	{
		result[i] = input1[i] ^ input2[i];
	}
	return result;
}


// 32bit异或
array<int, 32> DES::XOR_32(const array<int, 32>& input1, const array<int, 32>& input2)
{
	array<int, 32> result = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		result[i] = input1[i] ^ input2[i];
	}
	return result;
}


// 置换选择1(PC_1)
array<int, 56> DES::permutate_PC_1(const array<int, 64>& input)
{
	array<int, 56> result = { 0 };
	for (int i = 0; i < 56; ++i)
	{
		result[i] = input[PC_1[i] - 1];
	}
	return result;
}


// 置换选择2(PC_2)
array<int, 48> DES::permutate_PC_2(const array<int, 56>& input)
{
	array<int, 48> result = { 0 };
	for (int i = 0; i < 48; ++i)
	{
		result[i] = input[PC_2[i] - 1];
	}
	return result;
}


// 获取左边28bit
array<int, 28> DES::getLeft_28(const array<int, 56>& input)
{
	array<int, 28> left = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		left[i] = input[i];
	}
	return left;
}


// 获取右边28bit
array<int, 28> DES::getRight_28(const array<int, 56>& input)
{
	array<int, 28> right = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		right[i] = input[i + 28];
	}
	return right;
}


//  将左右各28bit拼接为56bit
array<int, 56> DES::combineLeftRight_28(const array<int, 28>& left, const array<int, 28>& right)
{
	array<int, 56> result = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		result[i] = left[i];
		result[i + 28] = right[i];
	}
	return  result;
}


// 左循环移位
// 注明：round从1-16
array<int, 28> DES::leftShift(const array<int, 28>& input, int round)
{
	// 防止数组越界
	if (round < 1 || round > 16)
	{
		throw out_of_range("轮数越界！");
	}
	int shift = LS[round - 1];

	// 根据表对输入进行左循环移位
	array<int, 28> result = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		result[i] = input[(i + shift) % 28];
	}
	return result;
}
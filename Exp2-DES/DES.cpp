#include "DES.h"

// ���캯��default
DES::DES()
{

}


// ����
array<int, 64> DES::encrypt(const array<int, 64>& plaintext)
{
	// 1����ʼ�û�IP
	array<int, 64> permutedText = permutate_IP(plaintext);

	// 2��16�ֱ任�����������Ҵ���
	for (int i = 1; i <= 16; ++i)
	{
		permutedText = feistelFunction(permutedText, i);
	}
	array<int, 64> temp = changeOrder(permutedText);

	//3�����ʼ�û�IP_1
	array<int, 64> result = permutate_IP_1(temp);

	return result;
}


// ����
array<int, 64> DES::decrypt(const array<int, 64>& ciphertext)
{
	// 1����ʼ�û�IP
	array<int, 64> permutedText = permutate_IP(ciphertext);

	// 2��16�ֱ任�����������Ҵ�������Կʹ��˳��ͼ����෴��
	for (int i = 1; i <= 16; ++i)
	{
		permutedText = feistelFunction(permutedText, 17 - i);
	}
	array<int, 64> temp = changeOrder(permutedText);

	//3�����ʼ�û�IP_1
	array<int, 64> result = permutate_IP_1(temp);

	return result;
}



// ���ĳ�ʼ�û�IP
array<int, 64> DES::permutate_IP(const array<int, 64>& input)
{
	// ���û������ʼ��
	array<int, 64> result = { 0 };
	for (int i = 0; i < 64; ++i)
	{
		result[i] = input[IP[i] - 1];
	}
	return result;
}


// �������ʼ�û�IP_1;
array<int, 64> DES::permutate_IP_1 (const array<int, 64>& input)
{
	// �����û������ʼ��
	array<int, 64> result = { 0 };
	for (int i = 0; i < 64; ++i)
	{
		result[i] = input[IP_1[i] - 1];
	}
	return result;
}


// �ֺ������������16��
// ע����round��1-16
array<int, 64> DES::feistelFunction(const array<int, 64>& input ,int round)
{
	// ��ֹ����Խ��
	if (round < 1 || round > 16)
	{
		throw out_of_range("����Խ�磡");
	}

	// ��64bit�����Ϊ���Ҹ�32bit
	array<int, 32> left = getLeft_32(input);
	array<int, 32> right = getRight_32(input);

	//�ұ�32bit����E����չΪ48bit�����ͱ�������Կ���
	array<int, 48> temp1 = XOR_48(expand_E(right), subKeys[round - 1]);
	
	//����S�д���ѡ������32bit����32bit�ٽ���һ��P�û�������ԭ����ߵ�32bit���õ����
	array<int, 32> temp2 = XOR_32(left, permute_P(substitute_S(temp1)));

	// �ұ�32bitֱ����Ϊ��һ�ֵ����32bit
	array<int, 64> result = combineLeftRight_32(right,temp2);
	
	return result;
}


// ��������Կ���ɷ�
void DES::generateSubKeys(const array<int, 64>& original_key)
{
	// �Ƚ����û�ѡ��1������Ϊ���������28bit
	array<int, 56> permutedKey = permutate_PC_1(original_key);
	array<int, 28> left = getLeft_28(permutedKey);
	array<int, 28> right = getRight_28(permutedKey);

	// ������16������Կ����
	for (int i = 1; i <= 16; ++i)
	{
		// ��������ѭ����λ
		left = leftShift(left, i);
		right = leftShift(right, i);

		// �����û�ѡ��2�����������������Կ������
		subKeys.push_back(permutate_PC_2(combineLeftRight_28(left, right)));
	}
}


// ��ȡ���32bit
array<int, 32> DES::getLeft_32(const array<int, 64>& input)
{
	array<int, 32> left = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		left[i] = input[i];
	}
	return left;
}


// ��ȡ�ұ�32bit
array<int, 32> DES::getRight_32(const array<int, 64>& input)
{
	array<int, 32> right = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		right[i] = input[i + 32];
	}
	return right;
}


// ��������32bit��˳��
array<int, 64> DES::changeOrder(const array<int, 64>& input)
{
	array<int, 64> result = { 0 };
	// ÿһ�θ�ֵ��i���͵�i+32��
	for (int i = 0; i < 32; ++i)
	{
		result[i] = input[i + 32];
		result[i + 32] = input[i];
	}
	return result;
}


//  �����Ҹ�32bitƴ��Ϊ64bit
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


// ��չ��������Ұ벿��R(32bit->48bit)
array<int, 48> DES::expand_E(const array<int, 32>& input)
{
	array<int, 48> result = { 0 };
	for (int i = 0; i < 48; ++i)
	{
		result[i] = input[E[i] - 1];
	}
	return result;
}


// S�д���ѡ��(48bit->32bit)
array<int, 32> DES::substitute_S(const array<int, 48>& input)
{
	array<int, 32> result = { 0 };
	for (int i = 0; i < 8; ++i)
	{
		// ����Ϊ8�飬ȡ��һ���͵�����bit��ɵĶ�������Ϊ�кţ��ڶ����������bit��ɵĶ�������Ϊ�к�
		int row = input[i * 6] * 2 + input[i * 6 + 5] * 1;
		int col = input[i * 6 + 1] * 8 + input[i * 6 + 2] * 4 + input[i * 6 + 3] * 2 + input[i * 6 + 4] * 1;
		int num = S[i][row][col];	// ʮ����

		// ��S���е�ʮ���ƽ��ת���ɶ����ƣ�����˳�򴢴���������
		for (int j = 0; j < 4; ++j)
		{
			result[i * 4 + 3 - j] = num % 2;
			num /= 2;
		}
	}
	return result;
}


// �û�����P
array<int, 32> DES::permute_P(const array<int, 32>& input)
{
	array<int, 32> result = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		result[i] = input[P[i] - 1];
	}
	return result;
}


// 48bit���
array<int, 48> DES::XOR_48(const array<int, 48>& input1, const array<int, 48>& input2)
{
	array<int, 48> result = { 0 };
	for (int i = 0; i < 48; ++i)
	{
		result[i] = input1[i] ^ input2[i];
	}
	return result;
}


// 32bit���
array<int, 32> DES::XOR_32(const array<int, 32>& input1, const array<int, 32>& input2)
{
	array<int, 32> result = { 0 };
	for (int i = 0; i < 32; ++i)
	{
		result[i] = input1[i] ^ input2[i];
	}
	return result;
}


// �û�ѡ��1(PC_1)
array<int, 56> DES::permutate_PC_1(const array<int, 64>& input)
{
	array<int, 56> result = { 0 };
	for (int i = 0; i < 56; ++i)
	{
		result[i] = input[PC_1[i] - 1];
	}
	return result;
}


// �û�ѡ��2(PC_2)
array<int, 48> DES::permutate_PC_2(const array<int, 56>& input)
{
	array<int, 48> result = { 0 };
	for (int i = 0; i < 48; ++i)
	{
		result[i] = input[PC_2[i] - 1];
	}
	return result;
}


// ��ȡ���28bit
array<int, 28> DES::getLeft_28(const array<int, 56>& input)
{
	array<int, 28> left = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		left[i] = input[i];
	}
	return left;
}


// ��ȡ�ұ�28bit
array<int, 28> DES::getRight_28(const array<int, 56>& input)
{
	array<int, 28> right = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		right[i] = input[i + 28];
	}
	return right;
}


//  �����Ҹ�28bitƴ��Ϊ56bit
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


// ��ѭ����λ
// ע����round��1-16
array<int, 28> DES::leftShift(const array<int, 28>& input, int round)
{
	// ��ֹ����Խ��
	if (round < 1 || round > 16)
	{
		throw out_of_range("����Խ�磡");
	}
	int shift = LS[round - 1];

	// ���ݱ�����������ѭ����λ
	array<int, 28> result = { 0 };
	for (int i = 0; i < 28; ++i)
	{
		result[i] = input[(i + shift) % 28];
	}
	return result;
}
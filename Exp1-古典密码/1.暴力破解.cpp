#include<iostream>
#include<string>
using namespace std;


// 解密单个字符
char decryptChar(const char ch, int shift)
{
	// 如果不是字母，则原样输出
	char result = ch;
	// 如果是字母，则做移位变换
	if (ch >= 'a' && ch <= 'z')
		result = (ch - 'a' - shift + 26) % 26 + 'a';
	if (ch >= 'A' && ch <= 'Z')
		result = (ch - 'A' - shift + 26) % 26 + 'A';
	return result;
}

// 解密字符串
string decryptString(const string &str, int length, int shift)
{
	string result(length, '0');
	for (int i = 0; i < length; ++i)
	{
		result[i] = decryptChar(str[i], shift);
	}
	return result;
}

// 尝试不同偏移量破译(暴力破解)
void bruteForceDecrypt(const string& str, int length)
{
	for (int i = 0; i < 26; ++i)
	{
		cout << "Shift: " << i << ": " << decryptString(str, length, i) << "..." << endl;
	}
}


int main()
{
	string ciphertext = "Ynulpkcnwldu eo pda lnwypeya wjz opqzu kb paydjemqao bkn oayqna ykiiqjeywpekj ej pda lnaoajya kb wzranownewh xadwrekn. Ikna cajanwhhu, ynulpkcnwldu eo wxkqp ykjopnqypejc wjz wjwhuvejc lnkpkykho pdwp lnarajp pdenz lwnpeao kn pda lqxhey bnki nawzejc lnerwpa iaoowcao.";
	cout << "Enter the ciphertext (Shift Cipher)：" << endl;
	cout << ciphertext << endl << endl;
	bruteForceDecrypt(ciphertext, 20);
	cout << endl;

	cout << "Enter the shift that successfully decrypted: ";
	int shift;
	cin >> shift;
	cout << "Result: " << decryptString(ciphertext, ciphertext.length(), shift) << endl;

	system("pause");
	return 0;
}

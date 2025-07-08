#include<iostream>
#include<string>
using namespace std;


// ���ܵ����ַ�
char decryptChar(const char ch, int shift)
{
	// ���������ĸ����ԭ�����
	char result = ch;
	// �������ĸ��������λ�任
	if (ch >= 'a' && ch <= 'z')
		result = (ch - 'a' - shift + 26) % 26 + 'a';
	if (ch >= 'A' && ch <= 'Z')
		result = (ch - 'A' - shift + 26) % 26 + 'A';
	return result;
}

// �����ַ���
string decryptString(const string &str, int length, int shift)
{
	string result(length, '0');
	for (int i = 0; i < length; ++i)
	{
		result[i] = decryptChar(str[i], shift);
	}
	return result;
}

// ���Բ�ͬƫ��������(�����ƽ�)
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
	cout << "Enter the ciphertext (Shift Cipher)��" << endl;
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

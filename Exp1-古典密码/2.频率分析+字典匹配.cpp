#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;


// ��ȡ�ֵ��еĵ���
vector<string> readDictionary(const string& filename) 
{
    vector<string> dictionary;
    ifstream file(filename);
    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);   // Сд
        dictionary.push_back(word);
    }
    return dictionary;
}

// ͳ����������ĸƵ��
vector<char> getFrequency(const string& ciphertext) 
{
    // ��Ƶ����ʼ��Ϊ0
    vector<int> freq;
    freq.resize(26, 0);
    
    for (char ch : ciphertext) 
    {
        // ����Ƿ�Ϊ�ַ�����������Ƶ��
        if (isalpha(ch)) 
        {
            freq[tolower(ch) - 'a']++;
        }
    }


    // ѡ������
    vector<char> letter(26);
    for (int i = 0; i < 26; ++i)
    {
        letter[i] = 'a' + i;
    }

    for (int i = 0; i < 25; ++i)
    {
        int max = freq[i];
        int index = i;
        for (int j = i + 1; j < 26; ++j)
        {
            if (freq[j] > max)
            {
                max = freq[j];
                index = j;
            }
        }
        freq[index] = -1;
        letter[i] = index + 'a';
    }

    return letter;
}

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
string decryptString(const string& str, int length, int shift)
{
    string result(length, '0');
    for (int i = 0; i < length; ++i)
    {
        result[i] = decryptChar(str[i], shift);
    }
    return result;
}

// ������ܺ�ƥ���ֵ�ĵ�������
int countMatchedWords(const string& plaintext, const vector<string>& dictionary)
{
    stringstream ss(plaintext);
    string word;
    int count = 0;

    while (ss >> word)
    {
        // ȥ����� + ת��Сд
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        if (find(dictionary.begin(), dictionary.end(), word) != dictionary.end())
        {
            count++;
        }
    }
    cout << endl;

    return count;
}


int main() 
{
    string ciphertext = "Ynulpkcnwldu eo pda lnwypeya wjz opqzu kb paydjemqao bkn oayqna ykiiqjeywpekj ej pda lnaoajya kb wzranownewh xadwrekn. Ikna cajanwhhu, ynulpkcnwldu eo wxkqp ykjopnqypejc wjz wjwhuvejc lnkpkykho pdwp lnarajp pdenz lwnpeao kn pda lqxhey bnki nawzejc lnerwpa iaoowcao.";
    cout << "���ģ� " << ciphertext << endl << endl;
    vector<string> dictionary = readDictionary("words.txt");
    vector<char> freqList = getFrequency(ciphertext);
    int times = min(5, (int)freqList.size());

    string correctDecryption;    
    int correctShift = 0;
    int maxMatched = 0;

    for (int i = 0; i < times; i++)
    {
        int shift = (freqList[i] - 'e' + 26) % 26; //  ������ֵ�e������

        string decryption = decryptString(ciphertext, ciphertext.length(), shift);
        cout << i + 1 << ". ����ƫ������" << shift << endl;
        cout << "���ܽ����" << decryption ;

        int matched = countMatchedWords(decryption, dictionary);
        cout << "ƥ�䵥������" << matched << endl << endl;

        if (matched > maxMatched)
        {
            maxMatched = matched;
            correctDecryption = decryption;
            correctShift = shift;
        }
    }

    cout << "��ѽ��ܽ����\n";
    cout << "ƫ������" << correctShift << endl;
    cout << "���ģ�" << correctDecryption << endl;

    system("pause");
    return 0;
}

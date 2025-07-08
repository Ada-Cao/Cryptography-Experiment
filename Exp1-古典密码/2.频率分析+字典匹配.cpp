#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;


// 读取字典中的单词
vector<string> readDictionary(const string& filename) 
{
    vector<string> dictionary;
    ifstream file(filename);
    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);   // 小写
        dictionary.push_back(word);
    }
    return dictionary;
}

// 统计密文中字母频率
vector<char> getFrequency(const string& ciphertext) 
{
    // 将频数初始化为0
    vector<int> freq;
    freq.resize(26, 0);
    
    for (char ch : ciphertext) 
    {
        // 检查是否为字符，是则增加频数
        if (isalpha(ch)) 
        {
            freq[tolower(ch) - 'a']++;
        }
    }


    // 选择排序
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
string decryptString(const string& str, int length, int shift)
{
    string result(length, '0');
    for (int i = 0; i < length; ++i)
    {
        result[i] = decryptChar(str[i], shift);
    }
    return result;
}

// 计算解密后匹配字典的单词数量
int countMatchedWords(const string& plaintext, const vector<string>& dictionary)
{
    stringstream ss(plaintext);
    string word;
    int count = 0;

    while (ss >> word)
    {
        // 去除标点 + 转成小写
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
    cout << "密文： " << ciphertext << endl << endl;
    vector<string> dictionary = readDictionary("words.txt");
    vector<char> freqList = getFrequency(ciphertext);
    int times = min(5, (int)freqList.size());

    string correctDecryption;    
    int correctShift = 0;
    int maxMatched = 0;

    for (int i = 0; i < times; i++)
    {
        int shift = (freqList[i] - 'e' + 26) % 26; //  以最常出现的e做尝试

        string decryption = decryptString(ciphertext, ciphertext.length(), shift);
        cout << i + 1 << ". 尝试偏移量：" << shift << endl;
        cout << "解密结果：" << decryption ;

        int matched = countMatchedWords(decryption, dictionary);
        cout << "匹配单词数：" << matched << endl << endl;

        if (matched > maxMatched)
        {
            maxMatched = matched;
            correctDecryption = decryption;
            correctShift = shift;
        }
    }

    cout << "最佳解密结果：\n";
    cout << "偏移量：" << correctShift << endl;
    cout << "明文：" << correctDecryption << endl;

    system("pause");
    return 0;
}

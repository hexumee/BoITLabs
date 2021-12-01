#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define WORD_LENGTH 11    // длина фрагмента сообщения
#define CODE_LENGTH 15    // длина фрагмента после кодирования
#define NULL_CODE 48      // код символа цифры 0

using namespace std;

/*  
* ФУНКЦИЯ: addControlBits(vector<string>&)
*    ЦЕЛЬ: добавление контрольных битов на места, где индекс равен степени двойки (2^0, 2^1, 2^2, 2^3...), но на единицу меньше
*/
void addControlBits(vector<string> &v) {
    for (unsigned int j = 0; j < v.size(); j++)
        for (unsigned int i = 1; i < v[j].length(); i *= 2)
            v[j].insert(i-1, 1, '0');
}

/*
* ФУНКЦИЯ: removeControlBits(vector<string>&)
*    ЦЕЛЬ: удаление контрольных битов из закодированного сообщения
*/
void removeControlBits(vector<string> &v) {
   for (unsigned int j = 0; j < v.size(); j++) {
        unsigned int steps = 0;
        for (unsigned int i = 1; i < v[j].length(); i *= 2) {
            v[j].erase(i-1-steps, 1);
            steps++;
        }
    }
}

/* 
* ФУНКЦИЯ: buildMessage(vector<string>&)
*    ЦЕЛЬ: сборка фрагментов в сообщение
*/
string buildMessage(vector<string> &v) {
    string result;

    for (unsigned int i = 0; i < v.size(); i++)
        result += v[i];

    return result;
}


/* 
* ФУНКЦИЯ: splitToFragments(string&, vector<string>&, unsigned int)
*    ЦЕЛЬ: разделение сообщения на фрагменты
* КОММЕНТ: если полученный фрагмент оказался меньше, чем нужно, то добавляются нули в конец до его заполнения
*/
void splitToFragments(string &msg, vector<string> &v, unsigned int size) {
    for (unsigned int i = 0; i < msg.length(); i += size) {
        string word = msg.substr(i, size);

        if (word.length() < size)
            word += string(size-word.length(), '0');
        
        v.push_back(word);
    }
}

/* 
* ФУНКЦИЯ: getSyndromeMatrix(unsigned int)
*    ЦЕЛЬ: получение матрицы синдрома
*/
vector<vector<unsigned int>> getSyndromeMatrix(unsigned int msg_size) {
    vector<vector<unsigned int>> syndromeMatrix;
    unsigned int matrixWidth = msg_size;
    unsigned int matrixHeight = (unsigned int)log2(msg_size)+1;
    unsigned int bit_count = 1;

    for (unsigned int i = 0; i < matrixHeight; i++) {
        string section = string(matrixWidth, '0');

        for (unsigned int j = bit_count-1; j < section.length(); j += 2*bit_count) {
            for (unsigned int k = j; k < j+bit_count; k++) {
                if (k > section.length()-1)
                    break;
                section[k] = '1';
            }
        }

        bit_count *= 2;

        vector<unsigned int> section_bits;
        for (unsigned int k = 0; k < section.length(); k++) {
            section_bits.push_back(section[k]-NULL_CODE);
        }
        syndromeMatrix.push_back(section_bits);
    }

    return syndromeMatrix;
}

/* 
* ФУНКЦИЯ: computeControlBits(vector<string>&, vector<vector<unsigned int>>&)
*    ЦЕЛЬ: вычисление контрольных битов
* КОММЕНТ: контрольные биты считаются так: остаток от деления на 2 суммы произведений битов в сообщении и строки контрольного бита
*/
void computeControlBits(vector<string> &f, vector<vector<unsigned int>> &sm) {
    for (unsigned int k = 0; k < f.size(); k++) {
        vector<unsigned int> control_bits;

        for (unsigned int i = 0; i < sm.size(); i++) {
            unsigned int bit = 0;
            for (unsigned int j = 0; j < sm[i].size(); j++) {
                bit += (f[k][j]-NULL_CODE)*sm[i][j];
            }
            control_bits.push_back(bit%2);
        }

        for (unsigned int i = 1, j = 0; i < f[k].length() && j < control_bits.size(); i *= 2, j++)
            f[k][i-1] = control_bits[j]+NULL_CODE;
    }
}

/* 
* ФУНКЦИЯ: string2Number(string&)
*    ЦЕЛЬ: преобразование строки в беззнаковое число
*/
unsigned long long string2Number(string &s) {
    int num;
    istringstream(s) >> num;
    return num > 0 ? num : -num;
}

/* 
* ФУНКЦИЯ: binary2Decimal(unsigned long long)
*    ЦЕЛЬ: преобразование двоичного числа в беззнаковое десятичное
*/
unsigned int binary2Decimal(unsigned long long num) {
    int rem = 0, dec = 0, base = 1;

    while (num > 0) {
         rem = num % 10;
         dec += rem * base;
         base *= 2;
         num /= 10;
     }

    return dec;
}

/* 
* ФУНКЦИЯ: checkForErrors(vector<string>&)
*    ЦЕЛЬ: проверка сообщения на ошибки
* КОММЕНТ: индекс ошибочного бита считается путем получения остатка от деления на 2 суммы произведений битов в сообщении и строки каждого контрольного
*          бита и преобразования полученной двоичной записи к десятичному виду
*/
vector<unsigned int> checkForErrors(vector<string> &f) {
    vector<vector<unsigned int>> sm = getSyndromeMatrix(CODE_LENGTH);
    vector<unsigned int> err_loc;

    for (unsigned int k = 0; k < f.size(); k++) {
        string binary_index;

        for (unsigned int i = 0; i < sm.size(); i++) {
            unsigned int bit = 0;
            for (unsigned int j = 0; j < sm[i].size(); j++) {
                bit += (f[k][j]-NULL_CODE)*sm[i][j];
            }
            binary_index.insert(binary_index.begin(), (bit%2)+NULL_CODE);
        }

        err_loc.push_back(binary2Decimal(string2Number(binary_index)));
    }

    return err_loc;
}

/* 
* ФУНКЦИЯ: encode(string&)
*    ЦЕЛЬ: кодирование двоичных данных по методу Хэмминга
*/
string encode(string &msg) {
    vector<string> frags;
    splitToFragments(msg, frags, WORD_LENGTH);
    addControlBits(frags);
    
    cout << "\nМатрица синдромов для данной последовательности:" << endl;
    vector<vector<unsigned int>> v = getSyndromeMatrix(CODE_LENGTH);
    for (unsigned int i = 0; i < v.size(); i++) {
        for (unsigned int j = 0; j < v[i].size(); j++) {
            cout << v[i][j];
        }
        cout << endl;
    }

    computeControlBits(frags, v);

    return buildMessage(frags);
}

/* 
* ФУНКЦИЯ: decode(string&)
*    ЦЕЛЬ: декодирование сообщения
*/
string decode(string &msg_e) {
    vector<string> frags;
    splitToFragments(msg_e, frags, CODE_LENGTH);

    cout << "\nМатрица синдромов для данной последовательности:" << endl;
    vector<vector<unsigned int>> v = getSyndromeMatrix(CODE_LENGTH);
    for (unsigned int i = 0; i < v.size(); i++) {
        for (unsigned int j = 0; j < v[i].size(); j++) {
            cout << v[i][j];
        }
        cout << endl;
    }

    vector<unsigned int> check_result = checkForErrors(frags);
    for (unsigned int i = 0; i < check_result.size(); i++) {
        if (check_result[i] > 0) {
            cout << "\nИсправлена ошибка на позиции " << check_result[i]+CODE_LENGTH*i << endl;
            frags[i][check_result[i]-1] = 1^(frags[i][check_result[i]-1]-NULL_CODE)+NULL_CODE;
        }
    }

    removeControlBits(frags);

    return buildMessage(frags);
}

int main() {
    setlocale(LC_ALL, "rus");
    string message;
    string message_e;
    cout << "[Энкодер] Введите сообщение (в двоичном виде): ";
    cin >> message;
    cout << "\nРезультат: " << encode(message) << endl;
    cout << "\n\n[Декодер] Введите сообщение (в двоичном виде): ";
    cin >> message_e;
    cout << "\nРезультат: " << decode(message_e) << endl;
}

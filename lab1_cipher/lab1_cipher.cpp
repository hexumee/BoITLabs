#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

#define MATRIX_ROWS 8                                                                  // высота таблицы
#define MATRIX_COLS 8                                                                  // ширина таблицы
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ,.?!"    // алфавит 64-символьный (матрица 8x8)

using namespace std;

/*
* ФУНКЦИЯ: contains(char, vector<char>*)
*    ЦЕЛЬ: выяснить, находится ли symbol в v
*/
bool contains(char symbol, vector<char> *v) {
    return find(v->begin(), v->end(), symbol) != v->end();
}

/*
* ФУНКЦИЯ: remove_duplicates(vector<char>*, const string&)
*    ЦЕЛЬ: удалить повторяющиеся символы из elements и поместить данные в v
*/
void remove_duplicates(vector<char> *v, const string &elements) {
    for (char symbol : elements) {
        if (!contains(symbol, v)) {
            v->push_back(symbol);
        }
    }
}

/*
* ФУНКЦИЯ: encode(char[][], const string&)
*    ЦЕЛЬ: применить шифр Трисемуса для message с использованием table
* КОММЕНТ: используя циклы, проходимся по всему сообщению и опускаем каждую его букву на строку ниже (тем самым составляя шифр)
*/
string encode(char table[MATRIX_ROWS][MATRIX_COLS], const string &message) {
    string result;
    int msg_idx = 0;

    while (msg_idx != message.length()) {
        for (int i = 0; i < MATRIX_ROWS; i++) {
            for (int j = 0; j < MATRIX_COLS; j++) {
                if (table[i][j] == message[msg_idx]) {
                    result += table[(i+1)%MATRIX_ROWS][j];
                    msg_idx++;
                }
            }
        }
    }

    return result;
}

/*
* ФУНКЦИЯ: decode(char[][], const string&)
*    ЦЕЛЬ: расшифровать message с использованием table (шифр Трисемуса)
* КОММЕНТ: используя циклы, проходимся по всему сообщению и поднимаем каждую его букву на строку выше (тем самым расшифровывая его)
*/
string decode(char table[MATRIX_ROWS][MATRIX_COLS], const string &message) {
    string result;
    int msg_idx = 0;

    while (msg_idx != message.length()) {
        for (int i = 0; i < MATRIX_ROWS; i++) {
            for (int j = 0; j < MATRIX_COLS; j++) {
                if (table[i][j] == message[msg_idx]) {
                    result += table[(i+MATRIX_ROWS-1)%MATRIX_ROWS][j];
                    msg_idx++;
                }
            }
        }
    }

    return result;
}

/*
* ФУНКЦИЯ: capitalize(const string&)
*    ЦЕЛЬ: повышение регистра символов в строке sentence
*/
string capitalize(const string &sentence) {
    string result;

    for (int i = 0; i < sentence.length(); i++) {
        if (islower((unsigned char)sentence[i])) {
            result += toupper((unsigned char)sentence[i]);
        } else {
            result += sentence[i];
        }
    }

    return result;
}

int main() {
    // Эти функции необходимы
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // для правильной обработки кириллицы
    char table[MATRIX_ROWS][MATRIX_COLS];
    vector<char> v;
    string keyword;
    string message;

    cout << "Введите ключ: ";
    getline(cin, keyword);
    remove_duplicates(&v, (capitalize(keyword) + ALPHABET));

    // заполнение таблицы значениями
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            table[i][j] = v[j+i*MATRIX_COLS];
        }
    }

    cout << "Итоговая таблица: " << endl;
    // вывод полученной таблицы
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            cout << table[i][j];
        }
        cout << '\n';
    }

    cout << "Введите шифруемое сообщение: ";
    getline(cin, message);
    
    string encoded = encode(table, capitalize(message));
    cout << "Зашифрованное сообщение: " << encoded << endl;
    cout << "Расшифрованное сообщение: " << decode(table, encoded) << endl;
    system("pause");
}


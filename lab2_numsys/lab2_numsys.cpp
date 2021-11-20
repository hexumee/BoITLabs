#include <iostream>
#include <string>
#include <vector>

#define NULL_CODE 48         // код нуль-символа ('0')
#define MANTISSA_SIZE 52     // объем памяти для мантиссы (для double 52 бита)
#define SHIFT 1023           // смещение экспоненты (2^10-1)
#define SIGNEXP_SIZE 12      // объем памяти для знака и экспоненты (для double 1+11 бит)

using namespace std;

/*
* ФУНКЦИЯ: cast2int(char)
*    ЦЕЛЬ: преобразование символа в беззнаковом представлении в беззнаковое число путем вычитания нуля-символа
*/
unsigned int cast2int(unsigned char sym) {
    return (unsigned int)(sym-NULL_CODE);
}

/*
* ФУНКЦИЯ: cast2char(unsigned int)
*    ЦЕЛЬ: преобразование беззнакового числа в символ в беззнаковом представлении путем добавления нуля-символа
*/
unsigned char cast2char(unsigned int num) {
    return (unsigned char)(num+NULL_CODE);
}

/*
* ФУНКЦИЯ: build(vector<int>&, int, int)
*    ЦЕЛЬ: объединение всех битов в одну строку, включая плавающую точку
*/
string build(vector<unsigned int> &v, int mlpe_dot_idx, int mlpr_dot_idx) {
    string result;
    int dot_shift = 0;

    for (unsigned int bit : v)
        result += cast2char(bit);

    if (mlpe_dot_idx > 0)
        dot_shift += mlpe_dot_idx;
    if (mlpr_dot_idx > 0)
        dot_shift += mlpr_dot_idx;
    if (dot_shift != 0)
        result.insert(result.length()-dot_shift, 1, '.');

    return result;
}

/*
* ФУНКЦИЯ: conv2bin(unsigned int)
*    ЦЕЛЬ: преобразование положительного десятичного числа в двоичную систему счисления
* КОММЕНТ: используется только в части B
*/
string conv2bin(unsigned int num) {
    string result;

    while (num > 0) {
        result.insert(0, 1, cast2char(num%2));
        num /= 2;
    }

    return result;
}

/*
* ФУНКЦИЯ: indexOf(string&, char)
*    ЦЕЛЬ: поиск индекса первого char в строке
* КОММЕНТ: если char не найден, то возвращаем -1
*/
int indexOf(string &where, char what) {
    int idx = -1;
    
    for (int i = where.length()-1; i >= 0; i--) {
        if (where[i] == what) {
            idx = i;
            return where.length()-1-idx;
        }
    }

    return idx;
}

// Часть A - перемножение двух двоичных чисел
string exec_part_a(string &num1, string &num2) {
    string multiplyee;
    string multiplier;
    vector<string> v;
    vector<unsigned int> a;
    int dot_idx_mlpe;
    int dot_idx_mlpr;

    // удаляем точку из числа, запоминая ее положение
    dot_idx_mlpe = indexOf(num1, '.');
    dot_idx_mlpr = indexOf(num2, '.');
    if (dot_idx_mlpe != -1)
        num1.erase(num1.length()-1-dot_idx_mlpe, 1);
    if (dot_idx_mlpr != -1)
        num2.erase(num2.length()-1-dot_idx_mlpr, 1);

    // от смены мест множителей произведение не меняется (просто удобнее, когда умножаемое больше множителя)
    if (num2.length() > num1.length()) {
        multiplyee = num2;
        multiplier = num1;
    } else {
        multiplyee = num1;
        multiplier = num2;
    }

    // добавляем в v произведения умножаемого и каждого бита множителя
    for (unsigned char bit : multiplier) {
        if (cast2int(bit))
            v.insert(v.begin(), multiplyee);
        else
            v.insert(v.begin(), string(multiplyee.length(), '0'));
    }

    // задаем начало для сумм произведений
    for (unsigned int bit : v[v.size()-1]) {
        a.push_back(bit);
    }

    // складываем суммы произведений со сдвигом
    for (int i = v.size()-2; i >= 0; i--) {
        char to_drop = '0';
        for (unsigned long j = 0; j < v[i].length(); j++) {
            if (j == multiplyee.length()-1) {
                to_drop = v[i][j];
                break;
            }
            a[v.size()-1-i+j] += cast2int(v[i][j]);
        }
        a.push_back(to_drop);
    }

    // приводим сумму произведений к должному виду ("201010011" -> "1001010011")
    for (int i = a.size()-1; i >= 0; i--) {
        unsigned int cur_bit = a[i]-NULL_CODE;
        if (i == 0) {
            if (cur_bit > 1) {
                a.erase(a.begin());
                a.insert(a.begin(), cur_bit%2);
                a.insert(a.begin(), cur_bit/2);
                break;
            } else {
                a[i] = cur_bit%2;
            }
        } else {
            a[i] = cur_bit%2;
            a[i-1] += cur_bit/2;
        }
    }

    return build(a, dot_idx_mlpe, dot_idx_mlpr);
}

// Часть B - перевод числа с плавающей запятой в его внутреннее представление в компьютере (тип double)
string exec_part_b(double &number) {
    string result;
    int pow_of_two = 0;

    // знак числа
    if (number < 0)
        result += '1';
    else
        result += '0';

    // нормализуем мантиссу (1,... * 2^i)
    if (abs(number) >= 1) {
        while (abs(number) > 2) {
            number /= 2;
            pow_of_two++;
        }
    } else if (0 < abs(number) && abs(number) < 1) {
        while (abs(number) < 1) {
            number *= 2;
            pow_of_two--;
        }
    } else {
        // для нуля точность нулевая, поэтому смещение экспоненты должно обратиться в ноль
        pow_of_two = -SHIFT;
    }

    // приводим смещенный порядок к двоичному виду и, если необходимо, добавляем в экспоненту незначащие нули
    result += conv2bin(pow_of_two+SHIFT);
    while (result.length() != SIGNEXP_SIZE) {
        result.insert(0, 1, '0');
    }

    // приводим мантиссу к двоичному виду (работаем только с дробной частью)
    double t = abs(number)-((unsigned int)abs(number));
    while (result.length() < SIGNEXP_SIZE+MANTISSA_SIZE) {
        t *= 2;
        if (t >= 1) {
            result += conv2bin((unsigned int)t);
            t -= (unsigned int)t;
        } else {
            result += '0';
        }
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "rus");
    string pos1, pos2;
    double pts;

    cout << "Введите 2 двоичных числа: ";
    cin >> pos1 >> pos2;
    cout << "Введите число: ";
    cin >> pts;

    cout << exec_part_a(pos1, pos2) << endl;
    cout << exec_part_b(pts) << endl;

    system("pause");
}

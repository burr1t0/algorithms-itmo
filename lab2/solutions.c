#include "lab2.h" // обращаем внимание, файл изменился!!!
#include <math.h> // а вы как думали, подлянки везде
#include <stdio.h>
#include <stdlib.h>

// Вставьте свою реализацию вместо этого файла!
// Задача 1 - Чётное или нечётное
int is_even(int n)
{
    // TODO: реализовать
    if (n % 2 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Задача 2 - Максимум из трёх чисел
int max_of_three(int a, int b, int c)
{
    // TODO: реализовать
    if (a >= b && a >= c)
    {
        return a;
    }
    else if (b >= c && b >= a)
    {
        return b;
    }
    else
    {
        return c;
    }
}

// Задача 3 - Знак числа
int sign_of_number(int n)
{
    // TODO: реализовать
    if (n > 0)
    {
        return 1;
    }
    else if (n < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

// Задача 4 - Абсолютное значение
int absolute_value(int n)
{
    // TODO: реализовать
    if (n < 0)
    {
        return -n;
    }
    else
    {
        return n;
    }
}

// Задача 5 - Треугольник существует
int triangle_exists(int a, int b, int c)
{
    // TODO: реализовать
    if (a + b > c && a + c > b && b + c > a)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Задача 6 - Класс оценок
const char *get_grade(int score)
{
    // TODO: реализовать

    if (score >= 90 && score <= 100)
    {
        return "отлично";
    }
    else if (score > 74 && score <= 90)
    {
        return "хорошо";
    }
    else if (score >= 60 && score <= 74)
    {
        return "удовлетворительно";
    }
    else if (score < 60 && score >= 0)
    {
        return "неудовлетворительно";
    }
    else
    {
        return "некорректно";
    }
}

// Задача 7 - Евклидово расстояние
double euclidean_distance(double x1, double y1, double x2, double y2)
{
    // TODO: реализовать
    return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

// Задача 8 - Манхэттенское расстояние
int manhattan_distance(int x1, int y1, int x2, int y2)
{
    // TODO: реализовать
    return abs(x1 - x2) + abs(y1 - y2);
}

// Вспомогательная функция для проверки простоты (можно писать сразу в задаче 9, но это плохой тон)
int is_prime(int n)
{
    // TODO: реализовать
    if (n <= 1)
    {
        return 0;   // O(1)
    }
    if (n == 2)
    {
        return 1;   // O(1)
    }
    if (n % 2 == 0)
    {
        return 0;   // O(1)
    }
    for (int i = 3; i <= sqrt(n); i += 2)   // O(n^0.5) итераций
    {
        if (n % i == 0)
        {
            return 0;   // O(1)
        }
    }
    return 1;   // O(1)
}
/*
O(n^0.5) - верхняя оценка (худший случай - простое число)
Θ(n^0.5) - точная оценка (в среднем придется проверить до n^0.5)
Ω(1) - нижняя оценка (лучший случай - четное число, кроме 2)

уже оптимизированно, для единичной проверки оптимально
*/

// Задача 9 - Среднее арифметическое простых чисел в интервале
double average_primes_in_interval(int a, int b)
{
    // TODO: реализовать
    int cnt = 0;    // O(1)
    int res = 0;    // O(1)

    for (int i = a; i <= b; i++)    // O(b-a) = O(n) итераций
    {
        if (is_prime(i))    // O(i^0.5) = O(n^0.5)
        {
            res += i;   // O(1)
            cnt++;      // O(1)
        }
    }
    if (cnt == 0)
    {
        return 0;   // O(1)
    }
    return (double)res / cnt;   // O(1)
}
/*
O(n*n^0.5) - верхняя оценка (в худшем случае все числа простые)
Θ(n*n^0.5) - точная оценка (для больших интервалов)
Ω(n) - нижняя оценка (если простых чисел нет, но все равно проверяем is_prime)

Можно ускорить, используя решето Эратосфена для ген-ии всех простых чисел до b за O(n log log n)
*/

// Задача 10 - Кратные числа в интервале
int count_multiples(int start, int end, int k)
{
    // TODO: реализовать
    int cnt = 0;    // O(1)
    if (k != 0)     // O(1)
    {
        for (int i = start; i <= end; i++)  // O(n) итераций
        {
            if (i % k == 0 && i != 0)   // O(1)
            {
                cnt += 1;   // O(1)
            }
        }
    }
    return cnt;     // O(1)
}
/*
O(n) - верхняя оценка
Θ(n) - точная оценка
Ω(1) - нижняя оценка (k = 0 или start > end)
можно сделать за O(1), если искать первое кратное и последнее, а потом по ф-ле арифм. прогр-ии
*/

// Задача 11 - Високосный год
int is_leap_year(int year)
{
    // TODO: реализовать
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
    {
        return 1;
    }
    return 0;
}

// Задача 12 - Сезон по номеру месяца
const char *season_by_month(int month)
{
    // TODO: реализовать
    switch (month)
    {
    case 1:
    case 2:
    case 12:
        return "зима";
    case 3:
    case 4:
    case 5:
        return "весна";
    case 6:
    case 7:
    case 8:
        return "лето";
    case 9:
    case 10:
    case 11:
        return "осень";
    default:
        // если некорректный ввод
        return "некорректно";
    }
}

// Задача 13 - День недели
const char *day_of_week(int day_num)
{
    // TODO: реализовать
    switch (day_num)
    {
    case 1:
        return "понедельник";
    case 2:
        return "вторник";
    case 3:
        return "среда";
    case 4:
        return "четверг";
    case 5:
        return "пятница";
    case 6:
        return "суббота";
    case 7:
        return "воскресенье";
    default:
        return "некорректно";
    }
}

// Задача 14 - Подсчёт цифр в числе
int count_digits(int n)
{
    // TODO: реализовать
    int cnt = 0;    // O(1)
    if (n < 0)      // O(1)
    {
        n = -n;     // O(1)
    }
    do
    {
        cnt += 1;   // О(log_10(n)) раз
        n /= 10;    //О(1)
    } while (n != 0);   // О(log_10(n)) итераций
    return cnt;     // О(1)
}
/*
O(log n) - верхняя оценка
Θ(log n) - точная оценка
Ω(1) - нижняя оценка (n = 0)
*/

// Задача 15 - Реверс числа
int reverse_number(int n)
{
    // TODO: реализовать
    int res = 0;    // O(1)
    do
    {
        res = res * 10 + (n % 10);  // O(log n) раз
        n /= 10;    // О(1)
    } while (n != 0);   // O(log n) тьераций
    return res;     // О(1)
}
/*
O(log n) - верхняя оценка
Θ(log n) - точная оценка
Ω(1) - нижняя оценка (n = 0)
*/

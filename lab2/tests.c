#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "lab2.h"

/* Вспомогательная конструкция, можно определить свой макрос */
void log_result(const char* task, const char* message) {
    printf("[%s] %s\n", task, message);
}

int main() {
    // ====== Задача 1: Чётное или нечётное ======
    log_result("Задача 1", "Начинаем тестирование");
    // TODO: реализовать
    assert(is_even(4) == 1);
    assert(is_even(3) == 0);
    assert(is_even(0) == 1);
    assert(is_even(1) == 0);
    assert(is_even(2) == 1);
    assert(is_even(-1) == 0);
    assert(is_even(2147483646) == 1);
    assert(is_even(-2147483648) == 1);
    log_result("Задача 1", "Все тесты пройдены");

    // ====== Задача 2: Максимум из трёх чисел ======
    log_result("Задача 2", "Начинаем тестирование");
    // TODO: реализовать
    assert(max_of_three(1, 2, 3) == 3);
    assert(max_of_three(12, 11, 10) == 12);
    assert(max_of_three(3, 3, 3) == 3);
    assert(max_of_three(3, 4, 3) == 4);
    assert(max_of_three(-1, -2, -3) == -1);
    assert(max_of_three(-1, 2, -2147483647) == 2);
    assert(max_of_three(2147483647, 0, -2147483647) == 2147483647);
    log_result("Задача 2", "Все тесты пройдены");

    // ====== Задача 3: Знак числа ======
    log_result("Задача 3", "Начинаем тестирование");
    // TODO: реализовать
    assert(sign_of_number(1) == 1);
    assert(sign_of_number(-1) == -1);
    assert(sign_of_number(0) == 0);
    assert(sign_of_number(2147483647) == 1);
    assert(sign_of_number(-2147483647) == -1);
    log_result("Задача 3", "Все тесты пройдены");

    // ====== Задача 4: Абсолютное значение ======
    log_result("Задача 4", "Начинаем тестирование");
    // TODO: реализовать
    assert(absolute_value(-3) == 3);
    assert(absolute_value(3) == 3);
    assert(absolute_value(0) == 0);
    assert(absolute_value(-2147483647) == 2147483647);
    assert(absolute_value(2147483647) == 2147483647);
    log_result("Задача 4", "Все тесты пройдены");

    // ====== Задача 5: Треугольник существует ======
    log_result("Задача 5", "Начинаем тестирование");
    // TODO: реализовать
    assert(triangle_exists(3, 4, 5) == 1);
    assert(triangle_exists(3, 2, 5) == 0);
    assert(triangle_exists(30, 4, 50) == 0);
    assert(triangle_exists(10, 17, 8) == 1);
    assert(triangle_exists(5, 3, 2) == 0);
    log_result("Задача 5", "Все тесты пройдены");

    // ====== Задача 6: Класс оценок ======
    log_result("Задача 6", "Начинаем тестирование");
    // TODO: реализовать
    assert(strcmp(get_grade(25), "неудовлетворительно") == 0);
    assert(strcmp(get_grade(60), "неудовлетворительно") == 0);
    assert(strcmp(get_grade(61), "удовлетворительно") == 0);
    assert(strcmp(get_grade(75), "удовлетворительно") == 0);
    assert(strcmp(get_grade(77), "хорошо") == 0);
    assert(strcmp(get_grade(90), "хорошо") == 0);
    assert(strcmp(get_grade(98), "отлично") == 0);
    assert(strcmp(get_grade(-1), "некорректно") == 0);
    assert(strcmp(get_grade(101), "некорректно") == 0);
    log_result("Задача 6", "Все тесты пройдены");

    // ====== Задача 7: Евклидово расстояние ======
    log_result("Задача 7", "Начинаем тестирование");
    // TODO: реализовать
    assert(euclidean_distance(0, 0, 3, 4) == 5);
    assert(euclidean_distance(-3, -5, 2, 7) == 13);
    assert(euclidean_distance(-3, -5, -7, -2) == 5);
    assert(euclidean_distance(0, 0, 0, 0) == 0);
    assert(euclidean_distance(30, 40, 50, 55) == 25);
    assert(euclidean_distance(4.5, 2.5, 1.5, 2.5) == 3);
    // fabs для проверки, что абсолютное значение не превшвет погрешность
    assert(fabs(euclidean_distance(0, 0, 6, -4.5) - 7.5) < 1e-6);
    log_result("Задача 7", "Все тесты пройдены");

    // ====== Задача 8: Манхэттенское расстояние ======
    log_result("Задача 8", "Начинаем тестирование");
    // TODO: реализовать
    assert(manhattan_distance(9, 5, 2, 4) == 8);
    assert(manhattan_distance(-9, -5, 2, -4) == 12);
    assert(manhattan_distance(0, 0, 0, 0) == 0);
    assert(manhattan_distance(-15, -33, -75, -3) == 90);
    log_result("Задача 8", "Все тесты пройдены");

    // ====== Задача 9: Среднее простых чисел ======
    log_result("Задача 9", "Начинаем тестирование");
    // TODO: реализовать
    assert(fabs(average_primes_in_interval(1, 10) - 4.25) < 1e-6);
    assert(fabs(average_primes_in_interval(10, 20) - 15) < 1e-6);
    assert(average_primes_in_interval(-1, 1) == 0);
    assert(average_primes_in_interval(-100, -80) == 0);
    assert(average_primes_in_interval(7, 7) == 7);
    assert(average_primes_in_interval(8, 10) == 0);
    assert(average_primes_in_interval(17, 19) == 18);
    log_result("Задача 9", "Все тесты пройдены");

    // ====== Задача 10: Кратные числа ======
    log_result("Задача 10", "Начинаем тестирование");
    // TODO: реализовать
    assert(count_multiples(0, 10, 2) == 5);
    assert(count_multiples(-12, 12, 3) == 8);
    assert(count_multiples(-10, 0, 2) == 5);
    assert(count_multiples(0, 0, 2) == 0);
    assert(count_multiples(0, 0, 0) == 0);
    assert(count_multiples(0, 100, 5) == 20);
    log_result("Задача 10", "Все тесты пройдены");

    // ====== Задача 11: Високосный год ======
    log_result("Задача 11", "Начинаем тестирование");
    // TODO: реализовать
    assert(is_leap_year(2000) == 1);
    assert(is_leap_year(1900) == 0);
    assert(is_leap_year(2025) == 0);
    assert(is_leap_year(4) == 1);
    assert(is_leap_year(100) == 0);
    assert(is_leap_year(400) == 1);
    assert(is_leap_year(0) == 0);
    assert(is_leap_year(-100) == 0);
    assert(is_leap_year(-4) == 1);
    log_result("Задача 11", "Все тесты пройдены");

    // ====== Задача 12: Сезон по месяцу ======
    log_result("Задача 12", "Начинаем тестирование");
    // TODO: реализовать
    assert(strcmp(season_by_month(0), "некорректно") == 0);
    assert(strcmp(season_by_month(-10), "некорректно") == 0);
    assert(strcmp(season_by_month(2), "зима") == 0);
    assert(strcmp(season_by_month(3), "весна") == 0);
    assert(strcmp(season_by_month(7), "лето") == 0);
    assert(strcmp(season_by_month(11), "осень") == 0);
    assert(strcmp(season_by_month(100), "некорректно") == 0);
    // граничные
    assert(strcmp(season_by_month(12), "зима") == 0);
    assert(strcmp(season_by_month(1), "зима") == 0);
    log_result("Задача 12", "Все тесты пройдены");

    // ====== Задача 13: День недели ======
    log_result("Задача 13", "Начинаем тестирование");
    // TODO: реализовать
    assert(strcmp(day_of_week(0), "некорректно") == 0);
    assert(strcmp(day_of_week(11), "некорректно") == 0);
    assert(strcmp(day_of_week(1), "понедельник") == 0);
    assert(strcmp(day_of_week(3), "среда") == 0);
    assert(strcmp(day_of_week(5), "пятница") == 0);
    assert(strcmp(day_of_week(7), "воскресенье") == 0);
    log_result("Задача 13", "Все тесты пройдены");

    // ====== Задача 14: Подсчёт цифр ======
    log_result("Задача 14", "Начинаем тестирование");
    // TODO: реализовать
    assert(count_digits(2020) == 4);
    assert(count_digits(7) == 1);
    assert(count_digits(-2020) == 4);
    assert(count_digits(0) == 1);
    assert(count_digits(1234567890) == 10);
    log_result("Задача 14", "Все тесты пройдены");

    // ====== Задача 15: Реверс числа ======
    log_result("Задача 15", "Начинаем тестирование");
    // TODO: реализовать
    assert(reverse_number(123) == 321);
    assert(reverse_number(-123) == -321);
    assert(reverse_number(1000) == 1);
    assert(reverse_number(1) == 1);
    assert(reverse_number(0) == 0);
    assert(reverse_number(1800) == 81);
    log_result("Задача 15", "Все тесты пройдены");

    printf("Все тесты пройдены успешно!\n");
    return 0;
}


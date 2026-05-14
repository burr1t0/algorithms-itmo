#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "generic.h"

// Определяем увеличивать размер или уменьшать
static int resize(Vector *vector)
{
    if (!vector) {
        return -1;
    }
    //определяю что нужно сделать
    bool need_increase = (vector->size >= vector->capacity);
    bool need_decrease = (vector->size < vector->capacity / 4 && vector->capacity > MIN_SIZE);

    if (!need_increase && !need_decrease) {
        return 0;
    }
    size_t new_capacity;
    if (need_increase) {
        new_capacity = vector->capacity * 2;
    } else {
        new_capacity = vector->capacity / 2;
        if (new_capacity < MIN_SIZE) {
            new_capacity = MIN_SIZE;
        }
    }

    // новое выделение памяти
    void *new_data = realloc(vector->data, new_capacity * vector->elem_size);
    // realloc - изменяет размер существующего массива, а не создает новый
    if (!new_data && new_capacity > 0) {
        return -1;
    }
    vector->data = new_data;
    vector->capacity = new_capacity;
    return 0;
}

Vector *createVector(size_t elem_size)
{
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    if (!vector) {
        printf("Не получилось выделить память");
        return NULL;
    }

    // выделение памяти под данные
    vector->data = malloc(MIN_SIZE * elem_size);
    if (!vector->data) {
        printf("Не получилось выделить память");
        return NULL;
    }

    // объявление поля
    vector->elem_size = elem_size;
    vector->size = 0;
    vector->capacity = MIN_SIZE;

    return vector;
}

int appendVectorItem(Vector *vector, void *el)
{
    // существует и есть эл-т который надо добавить
    if (!vector || !el) {
        return -1; // O(1)
    }

    // расширение при необходимости
    if (resize(vector) != 0) {
        return -1; // O(n) при копирование
    }

    // вычесление адреса для вставки
    void *destination = (char *)vector->data + (vector->size * vector->elem_size); // O(1)
    // копирование данных
    memcpy(destination, el, vector->elem_size); // O(1) фикс-й размер
    // увеличение количества эл-ов
    vector->size++; // O(1)

    return 0;
}
/*
O(n) - верхняя оценка (при реаллокации)
Θ(1) - точная оценка (амортизированная стоимость)
Ω(1) - нижняя оценка (нет реаллокации)

Амортизированная сложность O(1) при увел-ии размера в 2 раза
*/

void *getVectorItem(Vector *vector, size_t index)
{
    if (!vector || index >= vector->size) {
        return NULL; // O(1)
    }

    // возвращает указатель на элемент по индексу без его удаления
    return (char *)vector->data + (index * vector->elem_size); // O(1)
}
/*
O(1) - верхняя оценка
Θ(1) - точная оценка
Ω(1) - нижняя оценка
*/

int setVectorItem(Vector *vector, size_t index, void *value)
{
    if (!vector || !value || index >= vector->size) {
        return -1; // O(1)
    }

    // вычесление адреса эл-та для замены
    void *destination = (char *)vector->data + (index * vector->elem_size); // O(1)
    memcpy(destination, value, vector->elem_size);                          // O(1)
    return 0;
}
/*
O(1) - верхняя оценка
Θ(1) - точная оценка
Ω(1) - нижняя оценка
*/

void *popVectorItem(Vector *vector, size_t index)
{
    if (!vector || index >= vector->size) {
        return NULL; // O(1)
    }

    // создание копии для возврата
    void *data_copy = malloc(vector->elem_size); // O(1)
    if (!data_copy) {
        printf("Не получилось выделить память");
        return NULL; // O(1)
    }

    // копия данных удаляемого эл-та
    void *sourse = (char *)vector->data + (index * vector->elem_size); // O(1)
    memcpy(data_copy, sourse, vector->elem_size);                      // O(1)

    // сдвиг эл-ов после удаления влево
    for (size_t i = index; i < vector->size - 1; i++) { // O(n) в худшем случае
        void *destination = (char *)vector->data + (i * vector->elem_size);
        void *source = (char *)vector->data + ((i + 1) * vector->elem_size);
        memcpy(destination, source, vector->elem_size); // O(1)
    }
    // уменьшение кол-ва эл-в
    vector->size--; // O(1)

    // проверка и выполнение уменьшения, если надо
    resize(vector); // O(n) в худшем случае

    return data_copy;
}
/*
O(n) - верхняя оценка (удаление из начала + реаллокация)
Θ(n) - точная оценка (в среднем для удаления не из конца)
Ω(1) - нижняя оценка (удаление из конца, нет реаллокации)
*/

long int findVectorItem(Vector *vector, void *value, EqualsFunc cmp)
{
    if (!vector || !value || !cmp) {
        return -1; // O(1)
    }

    for (size_t i = 0; i < vector->size; i++) { // O(n) итераций
        // получаю указатель на эл-т с инд-м i
        void *current = getVectorItem(vector, i); // O(1) внутри цикла
        // сравниваю эл-ты
        if (cmp(current, value)) {
            return (long int)i; // O(1) внутри цикла
        }
    }
    return -1;
}
/*
O(n) - верхняя оценка (элемент в конце или отсутствует)
Θ(n) - точная оценка (линейный поиск)
Ω(1) - нижняя оценка (элемент первый)
*/

int vectorFree(Vector *vector)
{
    if (!vector) {
        return -1;
    }

    free(vector->data);
    free(vector);
    return 0;
}
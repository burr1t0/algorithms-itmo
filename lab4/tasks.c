#include "tasks.h"
#include "hash_table/generic.h"
#include "../lab3/vector/generic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
хеш ф-ция для указ-й на строки
аргументы: key - указ-ль на указ-ль на строку
возвращает хеш код для строки
*/
int HashStringPtr(const void *key)
{
    const char **str_ptr = (const char **)key;

    if (!str_ptr)
        return 0;

    const char *str = *str_ptr;
    if (!str)
        return 0;

    const unsigned long INITIAL_SEED = 5381UL;
    const int SHIFT_BITS = 5;

    unsigned long hash = INITIAL_SEED;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << SHIFT_BITS) + hash) + c;
    }

    const int MAX_INT_VALUE = 2147483647;
    return (int)(hash % MAX_INT_VALUE);
}

/*
сравн-е 2-ч строк ч/з указатели
а - указ-ль на 1-ую строку
b - указ-ль на 2-ую строку
возвращает 1, если равны иначе 0
*/
int CmpStringPtr(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    // обработка указ-лей
    if (str1 == NULL && str2 == NULL) {
        return 1;
    } else if (str1 == NULL || str2 == NULL) {
        return 0;
    }
    return strcmp(str1, str2) == 0;
}

Vector *removeDuplicates(Vector *strings)
{
    if (!strings) {
        return NULL; // O(1)
    }

    // созд-е таблицы для отслеж-я уникальных знач-й
    HashTable *unique_table = createHashTable(sizeof(char *), sizeof(int)); // O(1)

    if (!unique_table) {
        // не удалось создать таблицу
        return NULL; // O(1)
    }

    // созд-е вектора для рез-ов
    Vector *result = createVector(sizeof(char *)); // O(1)
    if (!result) {
        freeHashTable(unique_table);
        return NULL; // O(1)
    }

    // обход всех строк
    for (size_t i = 0; i < strings->size; i++) { // O(n) итераций
        char *str = *(char **)getVectorItem(strings, i); // O(1)

        // поиск строки в таблице
        int found = 0;
        void *value = getItemHashTable(unique_table, &str, HashStringPtr, CmpStringPtr); // O(1) в среднем
        if (value) { // O(1)
            found = 1; // O(1)
        }

        // если строка не найдена, то она уникальная
        if (!found) { // O(1)
            int dummy = 1; // O(1)
            setItemHashTable(unique_table, &str, &dummy, HashStringPtr, CmpStringPtr); // O(1) в среднем

            // добавлв-е в резулбт-ий вектор
            appendVectorItem(result, &str); // O(1) аморт-но
        }
    }

    freeHashTable(unique_table); // O(m) где m - размер таблицы

    return result;
}
/*
O(n²) - верхняя оценка (все коллизии, деградация до списка)
Θ(n) - точная оценка (хорошая хеш-функция, мало коллизий)
Ω(n) - нижняя оценка (в любом случае пройдем все элементы)
*/

Vector *encodeStrings(Vector *strings)
{
    if (!strings) {
        return NULL; // O(1)
    }

    // созд-е табл-цы для сопост-я строк с кодом
    HashTable *encoding_table = createHashTable(sizeof(char *), sizeof(int)); // O(1)

    if (!encoding_table) {
        // не удалось созд-ть таблицу
        return NULL;
    }

    // созд-е вектора для рез-та
    Vector *result = createVector(sizeof(int)); // O(1)
    if (!result) {
        freeHashTable(encoding_table);
        return NULL;
    }

    int next_code = 0; // O(1)
    // след-й доступный код

    // обход всех строк
    for (size_t i = 0; i < strings->size; i++) { // O(n) итераций
        char *str = *(char **)getVectorItem(strings, i); // O(1)

        // поиск в таблице
        int found = 0;
        int existing_code = 0;
        void *value = getItemHashTable(encoding_table, &str, HashStringPtr, CmpStringPtr); // O(1) в среднем
        if (value) { // O(1)
            found = 1; // O(1)
            existing_code = *(int *)value; // O(1)
        }

        if (found) { // O(1)
            // строка была
            appendVectorItem(result, &existing_code); // O(1)
        } else { // O(1)
            // новая строка
            setItemHashTable(encoding_table, &str, &next_code, HashStringPtr, CmpStringPtr); // O(1)
            appendVectorItem(result, &next_code);                                            // O(1)
            next_code++;                                                                     // O(1)
        }
    }

    freeHashTable(encoding_table); // O(m)

    return result;
}
/*
O(n²) - верхняя оценка (деградация хеш-таблицы)
Θ(n) - точная оценка
Ω(n) - нижняя оценка
*/

HashTable *swapKeysValues(HashTable *table, HashFunc hash, CmpFunc cmp) // O(1)
{
    if (!table || !hash || !cmp) {
        return NULL; // O(1)
    }

    // проверка, что размер ключа и знач-я совместимы
    if (table->key_size != sizeof(int) || table->val_size != sizeof(int)) {
        // таблица содержит ключ и знач-е не инт
        return NULL; // O(1)
    }

    // созд-е нов-й таблицы
    HashTable *swapped_table = createHashTable(table->val_size, table->key_size); // O(1)

    if (!swapped_table) {
        // не удалось создать
        return NULL;
    }

    // обход все слотов исходной
    for (size_t i = 0; i < table->capacity; i++) { // O(m) где m - capacity
        uint8_t *slot = (uint8_t *)getVectorItem(table->values, i); // O(1)
        if (!slot) {
            continue; // O(1)
        }

        uint8_t state = *slot; // O(1)

        if (state == SLOT_OCCUPIED) {
            // получ-е ключ и знач-е текущего слота
            void *key_ptr = slot + 1; // O(1)
            // ключ после сост
            void *val_ptr = (char *)key_ptr + table->key_size; // O(1)

            //смена мест
            setItemHashTable(swapped_table, val_ptr, key_ptr, hash, cmp); // O(1)
        }
    }

    return swapped_table;
}
/*
O(m) - верхняя оценка (проходим по всем слотам таблицы)
Θ(m) - точная оценка (всегда проходим всю capacity)
Ω(m) - нижняя оценка (в любом случае проходим все слоты)

m - capacity таблицы, может быть больше n (кол-ва эл-ов)
*/

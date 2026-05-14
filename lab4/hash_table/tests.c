#include "generic.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

// ф-ции для сравнения
static int int_equals(const void* a, const void* b) {
    return *(int *)a == *(int *)b;
}
static int string_equals(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b) == 0;
}

// созд-е и опреации
static void test_create_and_basic_ops() {
    HashTable *table = createHashTable(sizeof(int), sizeof(int));
    assert(table != 0);
    assert(table->size == 0);
    assert(table->capacity == TABLE_MIN_SIZE);

    // вставка
    int key = 42;
    int value = 100;
    setItemHashTable(table, &key, &value, HashInt, int_equals);
    assert(table->size == 1);

    // получ-е
    int *retrieved = (int *)getItemHashTable(table, &key, HashInt, int_equals);
    assert(retrieved != NULL);
    assert(*retrieved == 100);

    // обновление
    value = 200;
    setItemHashTable(table, &key, &value, HashInt, int_equals);
    assert(table->size == 1);

    retrieved = (int *)getItemHashTable(table, &key, HashInt, int_equals);
    assert(retrieved != NULL);
    assert(*retrieved == 200);

    freeHashTable(table);
}

// коллизии
static void test_collisions() {
    HashTable *table = createHashTable(sizeof(int), sizeof(int));

    // эл-ты которые могут вызвать коллизии
    int keys[] = {1, 18, 35, 52, 69};
    for (int i = 0; i < 5; i++) {
        int value = i * 10;
        setItemHashTable(table, &keys[i], &value, HashInt, int_equals);
    }
    assert(table->size == 5);

    // проверка эл-ов
    for (int i = 0; i < 5; i++) {
        int *val = (int *)getItemHashTable(table, &keys[i], HashInt, int_equals);
        assert(val != NULL);
        assert(*val == i * 10);
    }

    freeHashTable(table);
}

// удал-е и повторная вставка
static void test_delete_and_reinsert() {
    HashTable *table = createHashTable(sizeof(int), sizeof(int));

    int key = 10;
    int value = 100;
    setItemHashTable(table, &key, &value, HashInt, int_equals);
    assert(table->size == 1);

    // удал-е
    int *popped = (int *)popItemHashTable(table, &key, HashInt, int_equals);
    assert(popped != NULL);
    assert(*popped == 100);
    free(popped);
    assert(table->size == 0);

    // проверка удал-я эл-та
    void *retrived = getItemHashTable(table, &key, HashInt, int_equals);
    assert(retrived == NULL);

    // повторная вставка
    value = 200;
    setItemHashTable(table, &key, &value, HashInt, int_equals);
    assert(table->size == 1);

    retrived = getItemHashTable(table, &key, HashInt, int_equals);
    assert(retrived != NULL);
    assert(*(int *)retrived == 200);

    freeHashTable(table);
}

// rehash
static void test_rehash() {
    HashTable *table = createHashTable(sizeof(int), sizeof(int));
    size_t initial_capacity = table->capacity;

    // вставка эл-та до сравн-я
    int num_to_rehash = (int)(initial_capacity * LOAD_FACTOR_THRESHOLD) + 2;

    for (int i = 0; i < num_to_rehash; i++) {
        int value = i * 5;
        setItemHashTable(table, &i, &value, HashInt, int_equals);
    }

    // проверка rehash
    assert(table->capacity > initial_capacity);
    assert(table->size == (size_t)num_to_rehash);

    // проверка доступности эл-ов
    for (int i = 0; i < num_to_rehash; i++) {
        int *val = (int *)getItemHashTable(table, &i, HashInt, int_equals);
        assert(val != NULL);
        assert(*val == i * 5);
    }

    freeHashTable(table);
}

// строковые ключи
static void test_string_keys() {
    HashTable *table = createHashTable(sizeof(char *), sizeof(int));
    char *key1 = "apple";
    char *key2 = "banana";
    int value1 = 10;
    int value2 = 20;

    setItemHashTable(table, &key1, &value1, HashString, string_equals);
    setItemHashTable(table, &key2, &value2, HashString, string_equals);

    assert(table->size == 2);

    int *retrived = (int *)getItemHashTable(table, &key1, HashString, string_equals);
    assert(retrived != NULL && *retrived == 10);

    retrived = (int *)getItemHashTable(table, &key2, HashString, string_equals);
    assert(retrived != NULL && *retrived == 20);

    freeHashTable(table);
}

// угловые случаи
static void test_edge_cases() {
    // пустая таблица
    HashTable *table = createHashTable(sizeof(int), sizeof(int));

    // поиск в пустой таблице
    int key = 5;
    void *result = getItemHashTable(table, &key, HashInt, int_equals);
    assert(result == NULL);

    // удал-е из пустой табл.
    result = popItemHashTable(table, &key, HashInt, int_equals);
    assert(result == NULL);

    // отриц-е ключи
    int neg_key = -10;
    int value = 100;
    setItemHashTable(table, &neg_key, &value, HashInt, int_equals);
    assert(table->size == 1);

    result = getItemHashTable(table, &neg_key, HashInt, int_equals);
    assert(result != NULL && *(int*)result == 100);

    // нулевой ключ
    int zero_key = 0;
    setItemHashTable(table, &zero_key, &value, HashInt, int_equals);
    assert(table->size == 2);

    freeHashTable(table);
}

// некорректные данные
static void test_invalid_input() {
    // неверные пар-ры созд-я
    HashTable *table = createHashTable(0, sizeof(int));
    assert(table == NULL);

    table = createHashTable(sizeof(int), 0);
    assert(table == NULL);

    // корректное созд-е
    table = createHashTable(sizeof(int), sizeof(int));
    assert(table != NULL);

    int key = 5;
    int value = 50;

    // null пар-ры для setItemHashTable
    setItemHashTable(NULL, &key, &value, HashInt, int_equals);
    setItemHashTable(table, NULL, &value, HashInt, int_equals);
    setItemHashTable(table, &key, NULL, HashInt, int_equals);
    setItemHashTable(table, &key, &value, NULL, int_equals);
    setItemHashTable(table, &key, &value, HashInt, NULL);

    // должна ост-ся пустой
    assert(table->size == 0);

    freeHashTable(table);
}

// подсчет коллизий
static void test_collision_count() {
    HashTable *table = createHashTable(sizeof(int), sizeof(int));

    // вставка нескольких арг-ов
    for (int i = 0; i < 10; i++) {
        int value = i * 2;
        setItemHashTable(table, &i, &value, HashInt, int_equals);
    }

    freeHashTable(table);
}



int main()
{
    test_create_and_basic_ops();
    test_collisions();
    test_delete_and_reinsert();
    test_rehash();
    test_string_keys();
    test_edge_cases();
    test_invalid_input();
    test_collision_count();

    return 0;
}

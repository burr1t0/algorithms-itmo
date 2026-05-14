#include "tasks.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../lab3/vector/generic.h"
#include "hash_table/generic.h"

static Vector *create_string_vector(const char *strings[], size_t n) {
    Vector *v = createVector(sizeof(char *));
    for (size_t i = 0; i < n; i++)
        appendVectorItem(v, &strings[i]);
    return v;
}

static int hash_int(const void *k) { 
    return *(int *)k; 
}
static int cmp_int(const void *a, const void *b) { 
    return *(int *)a == *(int *)b; 
}

// тест removeDuplicates
static void test_removeDuplicate() {

    // базовый случай
    const char* dupes[] = {"a", "b", "a", "c", "b", "d"};
    Vector* input = create_string_vector(dupes, 6);
    Vector *result = removeDuplicates(input);
    assert(result && result->size == 4);

    // проверка содерж-я
    int counts[4] = {0};
    for (size_t i = 0; i < result->size; i++) {
        char* s = *(char**)getVectorItem(result, i);
        if (strcmp(s, "a") == 0) {
            counts[0]++;
        } else if (strcmp(s, "b") == 0) {
            counts[1]++;
        } else if (strcmp(s, "c") == 0) {
            counts[2]++;
        } else if (strcmp(s, "d") == 0) {
            counts[3]++;
        }
    }
    assert(counts[0] == 1 && counts[1] == 1 && counts[2] == 1 && counts[3] == 1);

    // пустой вектор
    Vector *empty = createVector(sizeof(char *));
    Vector *res_empty = removeDuplicates(empty);
    assert(res_empty && res_empty->size == 0);

    // NULL вход
    assert(removeDuplicates(NULL) == NULL);

    vectorFree(input);
    vectorFree(result);
    vectorFree(empty);
    vectorFree(res_empty);
}

// тест для encodeStrings
static void test_encodeStrings() {

    // базовый случай с дубликатами
    const char *strs[] = {"apple", "banana", "apple", "orange"};
    Vector *input = create_string_vector(strs, 4);
    Vector *codes = encodeStrings(input);
    assert(codes && codes->size == 4);

    // проверка кодиров-я
    assert(*(int *)getVectorItem(codes, 0) == 0);
    assert(*(int *)getVectorItem(codes, 1) == 1);
    assert(*(int *)getVectorItem(codes, 2) == 0);
    assert(*(int *)getVectorItem(codes, 3) == 2);

    // если все строки одинаковые
    const char *same[] = {"x", "x", "x"};
    Vector *same_input = create_string_vector(same, 3);
    Vector *same_codes = encodeStrings(same_input);
    assert(same_codes && same_codes->size == 3);
    for (size_t i = 0; i < 3; i++) {
        assert(*(int *)getVectorItem(same_codes, i) == 0);
    }

    // NULL вход
    assert(encodeStrings(NULL) == NULL);

    vectorFree(input);
    vectorFree(codes);
    vectorFree(same_input);
    vectorFree(same_codes);
}

// тест для swapKeysValues
static void test_swapKeysValues() {
    // Базовый случай
    HashTable *table = createHashTable(sizeof(int), sizeof(int));
    int pairs[][2] = {{1, 10}, {2, 20}, {3, 30}};
    for (int i = 0; i < 3; i++) {
        setItemHashTable(table, &pairs[i][0], &pairs[i][1], hash_int, cmp_int);
    }

    HashTable *swapped = swapKeysValues(table, hash_int, cmp_int);
    assert(swapped && swapped->size == 3);

    // проверка обмена old_value и old_key
    for (int i = 0; i < 3; i++) {
        void *val = getItemHashTable(swapped, &pairs[i][1], hash_int, cmp_int);
        assert(val && *(int *)val == pairs[i][0]);
    }

    // пустая таблица
    HashTable *empty = createHashTable(sizeof(int), sizeof(int));
    HashTable *swapped_empty = swapKeysValues(empty, hash_int, cmp_int);
    assert(swapped_empty && swapped_empty->size == 0);

    // неправ-ые типы
    HashTable *wrong = createHashTable(sizeof(char), sizeof(double));
    assert(swapKeysValues(wrong, hash_int, cmp_int) == NULL);

    // NULL параметры
    assert(swapKeysValues(NULL, hash_int, cmp_int) == NULL);
    assert(swapKeysValues(table, NULL, cmp_int) == NULL);
    assert(swapKeysValues(table, hash_int, NULL) == NULL);

    freeHashTable(table);
    freeHashTable(swapped);
    freeHashTable(empty);
    freeHashTable(swapped_empty);
    freeHashTable(wrong);
}

// угловые случаи
static void test_edge_cases() {

    // removeDuplicates с одним элементом
    const char *single[] = {"alone"};
    Vector *single_vec = create_string_vector(single, 1);
    Vector *single_res = removeDuplicates(single_vec);
    assert(single_res && single_res->size == 1);
    assert(strcmp(*(char **)getVectorItem(single_res, 0), "alone") == 0);

    // encodeStrings с пустым вектором
    Vector *empty = createVector(sizeof(char *));
    Vector *encoded_empty = encodeStrings(empty);
    assert(encoded_empty && encoded_empty->size == 0);

    // swapKeysValues с коллизиями
    HashTable *coll_table = createHashTable(sizeof(int), sizeof(int));
    int k1 = 1;
    int k2 = 18;
    int v1 = 100;
    int v2 = 200;
    setItemHashTable(coll_table, &k1, &v1, hash_int, cmp_int);
    setItemHashTable(coll_table, &k2, &v2, hash_int, cmp_int);
    HashTable *swapped_coll = swapKeysValues(coll_table, hash_int, cmp_int);
    assert(swapped_coll && swapped_coll->size == 2);

    vectorFree(single_vec);
    vectorFree(single_res);
    vectorFree(empty);
    vectorFree(encoded_empty);
    freeHashTable(coll_table);
    freeHashTable(swapped_coll);
}

int main()
{
    test_removeDuplicate();
    test_encodeStrings();
    test_swapKeysValues();
    test_edge_cases();
    printf("finish");
    return 0;
}

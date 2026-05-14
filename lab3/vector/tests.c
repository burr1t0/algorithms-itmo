#include "generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

void test_empty_vector() {
    //пустой ввод
    Vector *vec = createVector(sizeof(int));
    // проверка основы для пустого вектора
    assert(vec != NULL);
    assert(vec->size == 0);
    assert(vec->capacity >= MIN_SIZE);

    int cnt = 51;
    long int index = findVectorItem(vec, &cnt, intEquals);
    assert(index == -1);

    void *result = popVectorItem(vec, 0);
    assert(result == NULL);

    int new_val = 100;
    int set_result = setVectorItem(vec, 0, &new_val);
    assert(set_result == -1);

    vectorFree(vec);
}

void test_vec_single_element() {
    // мин-е значения
    Vector *vec = createVector(sizeof(int));
    int singl_cnt = 1;
    appendVectorItem(vec, &singl_cnt);
    assert(vec->size == 1);

    int *retrived = (int *)getVectorItem(vec, 0);
    assert(vec->size == 1);

    long int index = findVectorItem(vec, &singl_cnt, intEquals);
    assert(index == 0);

    int *popped = (int *)popVectorItem(vec, 0);
    assert(*popped == 1);
    assert(vec->size == 0);
    free(popped);
    vectorFree(vec);
}

void test_vec_min_values() {
    // мин-е значения данных
    char min_char = CHAR_MIN;
    Vector *char_vec = createVector(sizeof(char));
    appendVectorItem(char_vec, &min_char);
    long int index = findVectorItem(char_vec, &min_char, charEquals);
    assert(index == 0);

    vectorFree(char_vec);
}

void test_vec_typical_scenarios() {    
    // типичные сценарии
    Vector *int_vec = createVector(sizeof(int));
    int cnt1 = 10;
    for (int i = 0; i < 5; i++) {
        appendVectorItem(int_vec, &cnt1);
        cnt1 += 10;
    }
    assert(int_vec->size == 5);

    assert(*(int *)getVectorItem(int_vec, 0) == 10);
    assert(*(int *)getVectorItem(int_vec, 2) == 30);
    assert(*(int *)getVectorItem(int_vec, 4) == 50);

    int target = 30;
    assert(findVectorItem(int_vec, &target, intEquals) == 2);
    target = 20;
    assert(findVectorItem(int_vec, &target, intEquals) == 1);
    target = 40;
    assert(findVectorItem(int_vec, &target, intEquals) == 3);

    int new_value = 99;
    assert(setVectorItem(int_vec, 2, &new_value) == 0);
    assert(*(int*)getVectorItem(int_vec, 2) == 99);

    int *val1 = (int *)popVectorItem(int_vec, 2);
    assert(*val1 == 99);
    free(val1);
    assert(int_vec->size == 4);

    int *val2 = (int *)popVectorItem(int_vec, 0);
    assert(*val2 == 10);
    free(val2);
    assert(int_vec->size == 3);

    vectorFree(int_vec);
}

void test_large_vector() {
    // граничные значения
    Vector *large_vec = createVector(sizeof(int));
    int large_count = 100;
    for (int i = 0; i < large_count; i++) {
        appendVectorItem(large_vec, &i);
    }
    assert(large_vec->size == large_count);

    int first = 0;
    int middle = 49;
    int last = 99;
    assert(findVectorItem(large_vec, &first, intEquals) == 0);
    assert(findVectorItem(large_vec, &middle, intEquals) == 49);
    assert(findVectorItem(large_vec, &last, intEquals) == 99);

    int max_int = INT_MAX;
    Vector *max_vec = createVector(sizeof(int));
    appendVectorItem(max_vec, &max_int);
    assert(findVectorItem(max_vec, &max_int, intEquals) == 0);

    vectorFree(max_vec);
    vectorFree(large_vec);
}

void test_vector_all(void)
{
    test_empty_vector();
    test_vec_single_element();
    test_vec_min_values();
    test_vec_typical_scenarios();
    test_large_vector();
    printf("All vector test passed\n");
}

int main()
{
    test_vector_all();
    return 0;
}
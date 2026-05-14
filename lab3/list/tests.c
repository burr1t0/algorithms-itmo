#include "generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

void test_empty_list() {
    // пустое значение
    GenericList *list = createList(sizeof(int));
    assert(listLength(list) == 0);

    int cnt = 51;
    unsigned int index = findItem(list, &cnt, intEquals);
    assert(index == (unsigned int)-1);

    void *result = popItem(list, 0);
    assert(result == NULL);
    freeList(list);
    assert(listLength(NULL) == 0);
}

void test_single_element() {
    // минимальные значения
    GenericList *list = createList(sizeof(int));
    int single_cnt = 1;
    appendItem(list, &single_cnt);
    assert(listLength(list) == 1);

    unsigned int index = findItem(list, &single_cnt, intEquals);
    assert(index == 0);

    int *popped = (int *)popItem(list, 0);
    assert(*popped == 1);
    assert(listLength(list) == 0);
    freeList(list);
}

void test_min_values() {
    // мин-е значение данных
    char min_char = CHAR_MIN;
    GenericList *char_list = createList(sizeof(char));
    appendItem(char_list, &min_char);

    unsigned int index = findItem(char_list, &min_char, charEquals);
    assert(index == 0);

    freeList(char_list);
}

void test_typical_scenarios() {
    // типичные сценарии
    GenericList *int_list = createList(sizeof(int));
    int cnt2 = 10;
    for (int i = 0; i < 5; i++) {
        appendItem(int_list, &cnt2);
        cnt2 += 10;
    }
    assert(listLength(int_list) == 5);
    int target = 30;
    assert(findItem(int_list, &target, intEquals) == 2);
    target = 10;
    assert(findItem(int_list, &target, intEquals) == 0);
    target = 50;
    assert(findItem(int_list, &target, intEquals) == 4);

    // 30
    int *val1 = (int *)popItem(int_list, 2);
    assert(*val1 == 30);
    assert(listLength(int_list) == 4);
    // 10
    int *val2 = (int *)popItem(int_list, 0); 
    assert(*val2 == 10);
    assert(listLength(int_list) == 3);

    freeList(int_list);
}

void test_large_list() {
    // граничные значения
    GenericList *large_list = createList(sizeof(int));
    int large_count = 1000;
    for (int i = 0; i < large_count; i++) {
        appendItem(large_list, &i);
    }
    assert(listLength(large_list) == large_count);

    int first = 0;
    int middle = 499;
    int last = 999;
    assert(findItem(large_list, &first, intEquals) == 0);
    assert(findItem(large_list, &middle, intEquals) == 499);
    assert(findItem(large_list, &last, intEquals) == 999);
    freeList(large_list);
}

void test_max_values() {
    // максимальные значения данных
    int max_int = INT_MAX;
    GenericList *max_list = createList(sizeof(int));
    appendItem(max_list, &max_int);
    assert(findItem(max_list, &max_int, intEquals) == 0);

    freeList(max_list);
}

int main()
{
    test_empty_list();
    test_single_element();
    test_min_values();
    test_typical_scenarios();
    test_large_list();
    test_max_values();
    printf("All list test passed\n");
    return 0;
}
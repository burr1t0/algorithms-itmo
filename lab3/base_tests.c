#include "base_tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// тест первой задачи
void test_find_max_student() {
    // создание списка студентов
    GenericList *student_list = createList(sizeof(Student));

    Student s1 = {"Иванов", 4.2f};
    Student s2 = {"Петров", 4.8f};
    Student s3 = {"Сидоров", 4.5f};
    Student s4 = {"Кузнецов", 3.9f};

    appendItem(student_list, &s1);
    appendItem(student_list, &s2);
    appendItem(student_list, &s3);
    appendItem(student_list, &s4);

    // поиск студента с максимальным баллом
    Student *max_student = findMaxStudent(student_list);

    assert(max_student != NULL);
    assert(strcmp(max_student->name, "Петров") == 0);
    assert(max_student->avg == 4.8f);
    freeList(student_list);
}

// поиск максимума в векторе с int
void test_find_max_vector_int()
{
    Vector *int_vec = createVector(sizeof(int));
    int numbers[] = {3, 9, 27, 81, 243, 729, 2187, 6561};
    for (int i = 0; i < 8; i++) {
    appendVectorItem(int_vec, &numbers[i]);
    }

    int *max_val = (int *)findMaxVector(int_vec, intGreater);

    assert(max_val != NULL);
    assert(*max_val == 6561);

    vectorFree(int_vec);
}

// удаление дубликатов в списке
void test_remove_duplicates_list()
{
    GenericList *list = createList(sizeof(int));
    int numbers[] = {1, 2, 2, 3, 4, 4, 4, 5, 1};

    for (int i = 0; i < 9; i++) {
        appendItem(list, &numbers[i]);
    }

    assert(listLength(list) == 9);

    // удаление дубликатов
    int result = removeDuplicatesList(list, intEquals);
    assert(result == 0);

    // проверка рез-та
    assert(listLength(list) == 5);

    // проверка порядока и уникальности
    int expected[] = {1, 2, 3, 4, 5};
    Node *current = list->head;

    for (int i = 0; i < 5; i++) {
        assert(*(int *)current->data == expected[i]);
        current = current->next;
    }

    freeList(list);
}

// удаление дубликатов в векторе
void test_remove_duplicates_vector()
{
    Vector *vec = createVector(sizeof(int));
    int numbers[] = {5, 3, 5, 1, 3, 7, 5, 1};

    for (int i = 0; i < 8; i++) {
        appendVectorItem(vec, &numbers[i]);
    }

    assert(vec->size == 8);

    // удаление дубликатов
    int result = removeDuplicatesVector(vec, intEquals);
    assert(result == 0);

    // проверка рез-ов
    assert(vec->size == 4);

    // проверка порядока и уникальности
    int expected[] = {5, 3, 1, 7};
    for (int i = 0; i < 4; i++) {
        assert(*(int *)getVectorItem(vec, i) == expected[i]);
    }
    
    vectorFree(vec);
}

int main()
{
    test_find_max_student();
    printf("test_find_max_student passed");
    test_find_max_vector_int();
    printf("test_find_max_vec passed");
    test_remove_duplicates_list();
    printf("test_remove_duplicates_list passed");
    test_remove_duplicates_vector();
    printf("test_remove_duplicates_vector passed");
    return 0;
}
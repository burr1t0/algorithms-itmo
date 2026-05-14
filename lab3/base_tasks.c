#include "base_tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Student *findMaxStudent(GenericList *list)
{
    // существ-е
    if (!list || !list->head)
    {
        return NULL; // O(1)
    }

    // указатель на начало
    Node *current = list->head;                      // O(1)
    Student *max_student = (Student *)current->data; // O(1)

    // сравниваем средний балл каждого студента, с лучшим
    while (current != NULL)
    {                                                        // O(n) итераций
        Student *current_student = (Student *)current->data; // O(1)
        if (current_student->avg > max_student->avg)
        {                                  // O(1)
            max_student = current_student; // O(1)
        }
        // указатель на след. эл-т
        current = current->next; // O(1)
    }
    return max_student; // O(1)
}
/*
O(n) - верхняя оценка
Θ(n) - точная оценка
Ω(1) - нижняя оценка (список из одного элемента)
*/

void *findMaxVector(Vector *vector, EqualsFunc cmp)
{
    // существ-е
    if (!vector || vector->size == 0 || !cmp)
    {
        return NULL; // O(1)
    }
    void *max = getVectorItem(vector, 0); // O(1)

    // перебор всех эл-в
    for (size_t i = 1; i < vector->size; i++)
    {                                             // O(n) итераций
        void *current = getVectorItem(vector, i); // O(1)
        // cmp вернет 1, если current > max
        if (cmp(current, max))
        {                  // O(1)
            max = current; // O(1)
        }
    }

    // создание копии иначе не получилось
    void *result_copy = malloc(vector->elem_size); // O(1)
    if (!result_copy)
    {
        return NULL;
    }
    memcpy(result_copy, max, vector->elem_size); // O(1)

    return result_copy;
}
/*
O(n) - верхняя оценка
Θ(n) - точная оценка
Ω(1) - нижняя оценка (вектор из одного элемента)
*/

int removeDuplicatesList(GenericList *list, EqualsFunc cmp)
{
    if (!list || !cmp)
    {
        return -1; // O(1)
    }

    // если пустой список
    if (!list->head)
    {
        return 0; // O(1)
    }
    Node *current = list->head; // O(1)

    // пока находимся в саиске пробегаем все эл-ты
    while (current != NULL)
    {                                 // O(n) внешний цикл
        Node *prev = current;         // O(1)
        Node *runner = current->next; // O(1)

        // для каждого эл-та пробегаем все следующие
        while (runner != NULL)
        { // O(n) внутренний цикл
            if (cmp(current->data, runner->data))
            { // O(1)
                // удаление дублтката
                prev->next = runner->next; // O(1)
                free(runner->data);        // O(1)
                free(runner);              // O(1)
                runner = prev->next;       // O(1)
            }
            else
            {
                prev = runner;         // O(1)
                runner = runner->next; // O(1)
            }
        }
        current = current->next; // O(1)
    }
    return 0;
}
/*
O(n²) - верхняя оценка (двойной вложенный цикл)
Θ(n²) - точная оценка
Ω(n) - нижняя оценка (все элементы уникальны, но все равно проверяем)

можно исп-ть хеш-таблицу для O(n) решения
*/

int removeDuplicatesVector(Vector *vector, EqualsFunc cmp)
{
    // сущетвование
    if (!vector || !cmp)
    {
        return -1; // O(1)
    }

    // если пустой или из 1-го эл-та
    if (vector->size <= 1)
    {
        return 0; // O(1)
    }

    size_t write_index = 1; // O(1)

    for (size_t i = 1; i < vector->size; i++)
    {                                             // O(n) внешний цикл
        void *current = getVectorItem(vector, i); // O(1)
        int is_duplicate = 0;                     // O(1)

        // был ли уже такой эл-т
        for (size_t j = 0; j < write_index; j++)
        {                                              // O(n) внутренний цикл
            void *existing = getVectorItem(vector, j); // O(1)
            if (cmp(current, existing))
            {                     // O(1)
                is_duplicate = 1; // O(1)
                break;            // O(1)
            }
        }

        // если не дубликат
        if (!is_duplicate)
        { // O(1)
            if (write_index != i)
            {                                                                                 // O(1)
                void *destination = (char *)vector->data + (write_index * vector->elem_size); // O(1)
                memcpy(destination, current, vector->elem_size);                              // O(1)
            }
            write_index++; // O(1)
        }
    }

    vector->size = write_index; // O(1)
    return 0;
}
/*
O(n²) - верхняя оценка
Θ(n²) - точная оценка
Ω(n) - нижняя оценка (все элементы уникальны)

сорт-ка за O(n log n) + удаление дубликатов за O(n) = O(n log n), или хеш-таблица за O(n)
*/

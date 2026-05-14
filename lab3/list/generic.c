#include "generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GenericList *createList(size_t elem_size)
{
    GenericList *list = (GenericList *)malloc(sizeof(GenericList));
    // Выделение памяти под GenericList
    if (!list) {
        printf("Не получилось выделить память");
        return NULL;
    }

    list->head = NULL;
    // список пуст, т.к. отсутствует первый эл-т
    list->elem_size = elem_size;
    // сохранение размера переменной
    return list;
}

void appendItem(GenericList *list, void *data)
{
    if (!list || !data) {
        return; // O(1)
    }

    // создание нового узла
    Node *new_node = (Node *)malloc(sizeof(Node)); // O(1)
    // создание переменной, в которой храниться адрес памяти
    if (!new_node) { 
        printf("Не получилось выделить память");
        return;
    }

    // выделение памяти для данных и копирование их
    new_node->data = malloc(list->elem_size); // O(1)
    if (!new_node->data) {
        printf("Не получилось выделить память");
        free(new_node);
        // освобождение памяти
        return;
    }

    // копирование данных из data в new_node->data
    memcpy(new_node->data, data, list->elem_size); // O(1)
    // новый узел будет последним
    new_node->next = NULL; // O(1)

    // если пустой список, то новый узел становиться головой
    if (list->head == NULL) {
        list->head = new_node; // O(1)
        return;
    }

    // если список не пуст, то иду до последнего эл-та
    Node *current = list->head;
    while (current->next != NULL) { // O(n) в худшем случае
        // пока есть следующий переходим к нему
        current = current->next; // O(1) внутри цикла
    }
    
    // последний эл-т
    current->next = new_node; // O(1)
}
/*
O(n) - верхняя оценка (добавление в конец без хвостового указателя)
Θ(n) - точная оценка (всегда нужно пройти весь список)
Ω(1) - нижняя оценка (пустой список)
добавить поле tail в структуру списка для О(1)
*/

int findItem(GenericList *list, void *value, EqualsFunc cmp)
{
    // существует и не путой
    if (!list || !list->head) {
        return (unsigned int)-1; // O(1)
    }

    // начало с головы
    Node *current = list->head;
    unsigned int index = 0;

    // проход по всем узлам
    while (current != NULL) { // O(n) итераций
        // сравнение
        if (cmp(current->data, value)) {
            return index; // O(1) внутри цикла
        }

        // переход к след. узлу
        current = current->next; // O(1)
        index++;                 // O(1)
    }

    // если не нашли
    return (unsigned int)-1;
}
/*
O(n) - верхняя оценка (элемент в конце или отсутствует)
Θ(n) - точная оценка
Ω(1) - нижняя оценка (элемент первый)
*/

void *popItem(GenericList *list, size_t index)
{
    // существует и не пустой
    if (!list || !list->head) {
        return NULL; // O(1)
    }

    Node *current = list->head;
    // предыдущий эл-т
    Node *prev = NULL;

    // если удаляем первый эл-т
    if (index == 0) { // O(1)
        list->head = current->next; // O(1)
        // теперь головой становиться 2 эл-т

        // копия данных для возврата
        void *data_copy = malloc(list->elem_size);
        if (!data_copy) {
            printf("Не получилось выделить память");
            return NULL; // O(1)
        }
        // копирование
        memcpy(data_copy, current->data, list->elem_size); // O(1)

        // освобождение данных
        free(current->data);
        // освобождение узла
        free(current);

        return data_copy;
    }

    // поиск эл-та по индексу (для всех кроме 1-го)
    for (size_t i = 0; i < index && current != NULL; i++) { // O(n) в худшем случае
        prev = current;                                     // O(1) внутри цикла
        current = current->next;                            // O(1)
    }

    // проверка, что нашли нужный эл-т
    if (current == NULL) {
        return NULL; // O(1)
    }

    // пропускаем удаленный эл-т
    prev->next = current->next; // O(1)

    //создание копии для возврата
    void *data_copy = malloc(list->elem_size); // O(1)
    if (!data_copy) {
        printf("Не получилось выделить память");
        return NULL; // O(1)
    } 
    // копирование
    memcpy(data_copy, current->data, list->elem_size); // O(1)

    free(current->data);
    // освобождение данных 
    free(current);
    // освобождение узла

    return data_copy;
}
/*
O(n) - верхняя оценка (удаление из конца)
Θ(n) - точная оценка (в среднем n/2 операций)
Ω(1) - нижняя оценка (удаление из начала)
*/

void freeList(GenericList *list)
{
    // существование
    if (!list) {
        return;
    }

    // начало с головы
    Node *current = list->head;

    // идем по всем эл-м и освобождвем
    while (current != NULL) {
        Node *next = current->next;
        
        // освобождение данных текцщего узла
        free(current->data);
        // освобождение узла
        free(current);

        // след-й узел
        current = next;
    }

    // освобождение структуры списка
    free(list);
}

unsigned int listLength(GenericList *list)
{
    // существует ли
    if (!list) {
        return 0;
    }

    // начинаем с начала списка
    Node *current = list->head;
    unsigned int cnt = 0;

    // проходим по всем эл-ам
    while (current != NULL) {
        cnt++;
        current = current->next;
    }

    // возврат кол-ва эл-ов
    return cnt;
}

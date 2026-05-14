#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table/generic.h"

// ф-ция сравнения для указателей на строки
int compare_string_ptr(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2) == 0;
}

// ф-ция для обработки csv файла
void process_csv_file(const char* filename) {
    // открытие файла
    FILE* file = fopen(filename, "r");
    if (!file) {
        // не удалось откыть файл
        return;
    }

    // созд-е хеш таблицы: ключ строка, знач-е ссчетчик
    HashTable* table = createHashTable(sizeof(char*), sizeof(int));

    if (!table) {
        // не удалось открыть таблицу
        fclose(file);
        return;
    }

    char buffer[256];
    int line_count = 0;

    // чтение файла построчно
    while (fgets(buffer, sizeof(buffer), file)) {
        line_count++;

        // избавление от симовола новой строки
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // пропуск пустых строк
        if (strlen(buffer) == 0) {
            continue;
        }

        // созд-е копии строки для ключа
        char *key = malloc(strlen(buffer) + 1);
        if (!key) {
            // ошибка памяти
            continue;
        }
        strcpy(key, buffer);

        // поиск ключа в таблице
        int *existing_value = (int *)getItemHashTable(table, &key, HashString, compare_string_ptr);

        if (existing_value) {
            // есть ключ, увелич-е счетчика
            (*existing_value)++;
            free(key);
            // освобожд-е копии
        } else {
            // добавл-е нового ключа
            int new_value = 1;
            setItemHashTable(table, &key, &new_value, HashString, compare_string_ptr);
            // сохр-е ключа в таблице
        }
    }

    fclose(file);

    // получ-е рез-ов
    size_t capacity = table->capacity;
    unsigned long collisions = getCollisionCount(table, HashString);

    // рез-ты
    printf("file: %s\n", filename);
    printf("capacity: %zu\n", capacity);
    printf("collisions: %lu\n", collisions);

    freeHashTable(table);
}

int main() {
    process_csv_file("data/good_cases.csv");
    process_csv_file("data/bad_cases.csv");

    return 0;
}
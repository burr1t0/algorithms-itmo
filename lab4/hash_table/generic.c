#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "../../lab3/vector/generic.h"
#include "generic.h"

// золотое сечение
#define GOLDEN_RATIO_PRIME 0x9E3779B1U // 2^32 * (sqrt(5)-1)/2

// вспомог-я ф-ция для вычисления второй хеш ф-ции, она должна возвращать нечетное сичло != 0
static int compute_second_hash(int first_hash) {
    // умнож-е на золотое сечение для лучшего распреде-я
    unsigned int hash = (unsigned int)first_hash * GOLDEN_RATIO_PRIME;

    // гарантированно нечетное и != 0
    hash = hash | 1;
    if (hash == 0) {
        hash = 1;
    }

    return (int)hash;
}

// вспомог-я ф-ция для двойного хеш-ия. возвращает индек для попытки attempt
static size_t double_hash_index(int hash1, int hash2, size_t attempt, size_t capacity) {
    // привед-е к беззнаковым типпам, для простых вычислений
    size_t h1, h2;
    if (hash1 < 0) {
        h1 = (size_t)(-hash1) % capacity;
    } else {
        h1 = (size_t)hash1 % capacity;
    }
    if (hash2 < 0) {
        h2 = (size_t)(-hash2) % capacity;
    } else {
        h2 = (size_t)hash2 % capacity;
    }

    // гарантирует, что h2 не четный и != 0
    h2 = (h2 | 1) % capacity;
    if (h2 == 0) {
        h2 = 1;
    }

    return (h1 + attempt * h2) % capacity;
}

int HashInt(const void *key)
{
    int value = *(const int *)key;
    if (value < 0) {
        return (-value);
    } else {
        return value;
    }
}

int HashString(const void *key)
{
    const char *str = (const char *)key;
    const unsigned long INITIAL_SEED = 5381UL; // простое число для начала
    const int SHIFT_BITS = 5; // для оптимиз-ии 2**SHIFT_BITS = MULTIPLIER-1

    unsigned long hash = INITIAL_SEED;
    int c;

    while ((c = *str++)) {
        // hash = hash * MULTIPLIER + c
        // hash * 33 = hash * 32 + hash = (hash << 5) + hash
        hash = ((hash << SHIFT_BITS) + hash) + c;
    }

    // привед-е к гиапазону int
    const int MAX_INT_VALUE = INT_MAX;

    return (int)(hash % MAX_INT_VALUE);
}

HashTable *createHashTable(size_t key_size, size_t val_size)
{
    if (key_size == 0 || val_size == 0) {
        // неверный размер ключа или знач-я
        return NULL;
    }

    // выдел-е памяти под структуру таблице
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) {
        // не удалось выделить память
        return NULL;
    }

    // размер слота (сост(1), ключ, знач-е)
    size_t slot_size = 1 + key_size + val_size;

    // созд-е вектора для хранения слотов
    table->values = createVector(slot_size);
    if (!table->values) {
        free(table);
        // не удалось создать вектоор знач-й
        return NULL;
    }

    // инициализ-я 
    table->key_size = key_size;
    table->val_size = val_size;
    table->size = 0;
    table->capacity = TABLE_MIN_SIZE;

    // созд-е пустого слота для инициализ-ии
    uint8_t* empty_slot = (uint8_t*)malloc(slot_size);
    if (!empty_slot) {
        vectorFree(table->values);
        free(table);
        // не удалось выделить память под слоты
        return NULL;
    }

    // инициализ-я всех слотов, как пустых
    memset(empty_slot, 0, slot_size); // заполнение памяти опред-м знач-м
    *empty_slot = SLOT_EMPTY; // перв-й байт - сост-е слота

    for (size_t i = 0; i < TABLE_MIN_SIZE; i++) {
        if (appendVectorItem(table->values, empty_slot) != 0) {
            free(empty_slot);
            vectorFree(table->values);
            free(table);
            // не удалось инициализ-ть слоты таблицы
            return NULL;
        }
    }

    free(empty_slot);
    return table;
}

// ф-ция для получения указателя на слот
static uint8_t* get_slot_pointer(HashTable* table, size_t index) {
    if (!table || !table->values || index >= table->capacity) {
        return NULL;
    }
    return (uint8_t *)getVectorItem(table->values, index);
}

// ф-ция для получ-я сост-я слота
static uint8_t get_slot_state(HashTable* table, size_t index) {
    uint8_t *slot = get_slot_pointer(table, index);
    if (slot != NULL) {
        return *slot;
    } else {
        return SLOT_EMPTY;
    }   
}

// ф-ция для получ-я указателя на ключ в слоте
static void* get_slot_key(HashTable* table, size_t index) {
    uint8_t* slot = get_slot_pointer(table, index);
    if (!slot) {
        return NULL;
    }
    // ключ идет после байта сост-я
    return (void *)(slot + 1);
}

// ф-ция для получ-я указ-ля на знач-е в слоте
static void* get_slot_value(HashTable* table, size_t index) {
    uint8_t *slot = get_slot_pointer(table, index);
    if (!slot) {
        return NULL;
    }
    // знач-е идет после сост-я и ключа
    return (void *)(slot + 1 + table->key_size);
}

/*
поиск слота по ключу с двойным хешированием
возвращает индекс найденного слота или table->capacity, если не найден
also_find_deleted - если true, ищет удал-е слоты для вставки
*/
static size_t find_slot_index(HashTable *table, void *key, HashFunc hash, CmpFunc cmp, bool also_find_deleted, bool *found_existing) {
    if (!table || !key || !hash || !cmp) {
        if (found_existing) {
            *found_existing = false;
        }
        if (table == NULL) {
            return 0;
        }
        return table->capacity;
    }

    // вычисление хеш знач-я для двойного хеширования
    int hash1 = hash(key);
    int hash2 = compute_second_hash(hash1);

    size_t first_deleted_index = table->capacity;

    // поиск с двойным хешированием
    for (size_t attempt = 0; attempt < table->capacity; attempt++) {
        size_t index = double_hash_index(hash1, hash2, attempt, table->capacity);
        uint8_t state = get_slot_state(table, index);

        if (state == SLOT_EMPTY) {
            // если нашли пустой слот
            if (found_existing) {
                *found_existing = false;
            }
            // возврат либо первый удал-й слот, либо пустой
            if (first_deleted_index < table->capacity && also_find_deleted) {
                return first_deleted_index;
            }
            return index;
        }

        if (state == SLOT_DELETED)
        {
            // запоминаем первый удал-й слот
            if (first_deleted_index == table->capacity)
            {
                first_deleted_index = index;
            }
            // продолж-е поиска, если also_find_deleted = false
            if (!also_find_deleted)
            {
                continue;
            }
        }

        if (state == SLOT_OCCUPIED || (also_find_deleted && state == SLOT_DELETED))
        {
            void *slot_key = get_slot_key(table, index);
            if (slot_key && cmp(slot_key, key))
            {
                // найден сущ-ий ключ
                if (found_existing)
                {
                    *found_existing = (state == SLOT_OCCUPIED);
                }
                return index;
            }
        }
    }

    // Не нашли подходящий слот
    if (found_existing) {
        *found_existing = false;
    }

    // Возвращаем первый удаленный слот, если нужно
    if (also_find_deleted && first_deleted_index < table->capacity) {
        return first_deleted_index;
    }

    return table->capacity;
}

void setItemHashTable(HashTable * table, void *key, void *data, HashFunc hash, CmpFunc cmp)
{
    if (!table || !key || !data || !hash || !cmp) {
        // неверные пар-ры для вставки
        return;
    }

    // проверка необход-ти rehash
    if ((double)table->size / table->capacity > LOAD_FACTOR_THRESHOLD) {
        rehashHashTable(table, hash, cmp);
    }

    // поиск слота для вставки
    bool key_exists = false;
    size_t slot_index = find_slot_index(table, key, hash, cmp, true, &key_exists);

    if (slot_index >= table->capacity) {
        //  не удалось найти слот
        return;
    }

    // получ-е указателя на слот
    uint8_t* slot = get_slot_pointer(table, slot_index);
    if (!slot) {
        // неверный индекс
        return;
    }
    
    if (key_exists) {
        // уже был ключ, обновляем
        void *slot_value = get_slot_value(table, slot_index);
        if (slot_value) {
            memcpy(slot_value, data, table->val_size);
        }

    } else {
        // новый ключ
        *slot = SLOT_OCCUPIED; 

        // копир-е ключа
        void *slot_key = get_slot_key(table, slot_index);
        memcpy(slot_key, key, table->val_size);

        // копир-е знач-я
        void *slot_value = get_slot_value(table, slot_index);
        memcpy(slot_value, data, table->key_size);

        // уыелич-е счетчика эл-ов
        table->size++;
    }
}

void rehashHashTable(HashTable *table, HashFunc hash, CmpFunc cmp)
{
    if (!table || !hash || !cmp) {
        // неверные пар-ры для rehash
        return;
    }

    // сохр-е старых данных
    Vector *old_values = table->values;
    size_t old_capacity = table->capacity;

    // созд-е временного масива для ключей и знач-й
    void **old_keys = malloc(old_capacity * sizeof(void *)); 
    // void** - указатель на указатель на люб-й тип данных
    void **old_values_data = malloc(old_capacity * sizeof(void *));
    size_t valid_items = 0;

    if (!old_keys || !old_values_data) {
        if (old_keys) {
            free(old_keys);
        }
        if (old_values_data) {
            free(old_values_data);
        }
        return;
    }

    // сборка всех занятых эл-ов
    for (size_t i = 0; i < old_capacity; i++) {
        uint8_t *old_slot = (uint8_t *)getVectorItem(old_values, i);
        if (old_slot && *old_slot == SLOT_OCCUPIED) {
            // ключ
            old_keys[valid_items] = malloc(table->key_size);
            void *key_ptr = old_slot + 1;
            memcpy(old_keys[valid_items], key_ptr, table->key_size);

            // знач-е
            old_values_data[valid_items] = malloc(table->val_size);
            void *value_ptr = old_slot + 1 + table->key_size;
            memcpy(old_values_data[valid_items], value_ptr, table->val_size);

            valid_items++;
        }
    }

    // увелич-е емкости
    size_t new_caapcity = old_capacity * 2;
    // поиск ближ-го простого числа для лучшего распред-я
    while (new_caapcity % 2 == 0 || new_caapcity % 3 == 0) {
        new_caapcity++;
    }

    // созд-е нового вектора с увелич-й емкостью
    size_t slot_size = 1 + table->key_size + table->val_size;
    table->values = createVector(slot_size);
    if (!table->values) {
        // не удвлось создать новый вектор
        table->values = old_values; // восст-ие старого вектора
        
        // освобожд-е временных данных
        for (size_t i = 0; i < valid_items; i++){
            free(old_keys[i]);
            free(old_values_data[i]);
        }
        free(old_keys);
        free(old_values_data);

        return;
    }

    // инициализ-я нового вектра с пустыми слотами
    uint8_t *empty_slot = (uint8_t *)malloc(slot_size);
    if (!empty_slot) {
        vectorFree(table->values);
        table->values = old_values;
        // не удалось выделить память

        // освобожд-е врем-х двнных
        for (size_t i = 0; i < valid_items; i++)
        {
            free(old_keys[i]);
            free(old_values_data[i]);
        }
        free(old_keys);
        free(old_values_data);

        return;
    }

    memset(empty_slot, 0, slot_size);
    *empty_slot = SLOT_EMPTY;

    for (size_t i = 0; i < new_caapcity; i++) {
        if (appendVectorItem(table->values, empty_slot) != 0) {
            free(empty_slot);
            vectorFree(table->values);
            table->values = old_values;
            // не удалось инициализ-ть нов. вектор

            // освобожд-е врем-х двнных
            for (size_t i = 0; i < valid_items; i++)
            {
                free(old_keys[i]);
                free(old_values_data[i]);
            }
            free(old_keys);
            free(old_values_data);

            return;
        }
    }

    free(empty_slot);

    // обновление емкости и сброс счетчика
    table->capacity = new_caapcity;
    table->size = 0;

    // перенос эл-ов из старой таблицы
    for (size_t i = 0; i < valid_items; i++) {
        setItemHashTable(table, old_keys[i], old_values_data[i], hash, cmp);

        // освобожд-е врем-х копий
        free(old_keys[i]);
        free(old_values_data[i]);
    }
    free(old_keys);
    free(old_values_data);
    vectorFree(old_values);
}

void *getItemHashTable(HashTable *table, void *key, HashFunc hash, CmpFunc cmp)
{
    if (!table || !key || !hash || !cmp) {
        return NULL;
    }

    // поиск сущ-го ключа
    bool key_exists = false;
    size_t slot_index = find_slot_index(table, key, hash, cmp, false, &key_exists);

    if (key_exists && slot_index < table->capacity) {
        return get_slot_value(table, slot_index);
    }

    //ключ не найден
    return NULL;
}

void *popItemHashTable(HashTable *table, void *key, HashFunc hash, CmpFunc cmp)
{
    if (!table || !key || !hash || !cmp) {
        return NULL;
    }

    // поиск сущ-го ключа
    bool key_exists = false;
    size_t slot_index = find_slot_index(table, key, hash, cmp, false, &key_exists);

    if (!key_exists || slot_index >= table->capacity) {
        // ключ не найден
        return NULL;
    }

    // получ-е знач-я перед удал-м
    void* slot_value = get_slot_value(table, slot_index);
    if (!slot_value) {
        return NULL;
    }

    // созд-е копии знач-я для возврата
    void *value_copy = malloc(table->val_size);
    if (!value_copy) {
        // не удалось выделить память для копии
        return NULL;
    }
    memcpy(value_copy, slot_value, table->val_size);

    // пометка слота, как удал-го
    uint8_t *slot = get_slot_pointer(table, slot_index);
    if (slot) {
        *slot = SLOT_DELETED;
    }

    // уменьш-е счетчика эл-ов
    table->size--;

    return value_copy;
}

unsigned long int getCollisionCount(HashTable *table, HashFunc hash)
{
    if (!table || !hash) {
        return 0;
    }

    unsigned long int collisions = 0;

    // обход всех слотов
    for (size_t i = 0; i < table->capacity; i++) {
        uint8_t state = get_slot_state(table, i);

        if (state == SLOT_OCCUPIED) {
            void* key = get_slot_key(table, i);
            if (key) {
                // вычисд-е первого инд-са для ключа
                int hash_value = hash(key);
                size_t primary_index;
                int positive_hash;

                // получ не отриц знач хеша
                if (hash_value < 0) {
                    positive_hash = -hash_value;
                } else {
                    positive_hash = hash_value;
                }

                // вычисл-е инд-са в пределах таблицы
                primary_index = (size_t)positive_hash % table->capacity;

                // если не в своей первичн поз-ии, то коллизия
                if (i != primary_index) {
                    collisions++;
                }
            }
        }
    }
    
    return collisions;
}

void freeHashTable(HashTable *table)
{
    if (!table) {
        return;
    }

    // освобожд-е вектора со слотами
    if (table->values) {
        vectorFree(table->values);
    }

    // освобожд-е структуры таблицы
    free(table);
}
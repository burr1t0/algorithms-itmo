#include "clustering.h"
#include <stdlib.h>
#include <time.h>

// вычисл-е центра кластера
double calculateCentroid(Vector* cluster) {
    if (cluster->size == 0) {
        return 0.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < cluster->size; i++) {
        double *value = (double *)getVectorItem(cluster, i);
        sum += *value;
    }
    return sum / cluster->size;
}

// вычисл-е суммы квадратов расст-й от эл-ов до центра
double calculateClusterVariance(Vector* cluster) {
    if (cluster->size == 0) {
        return 0.0;
    }

    double centroid = calculateCentroid(cluster);
    double variance = 0.0;
    
    for (size_t i = 0; i < cluster->size; i++) {
        double* value = (double*)getVectorItem(cluster, i);
        double diff = *value - centroid;
        variance += diff * diff;
    }
    return variance;
}

// вычисл-е общей суммы дисперсии для всех кластеров
double calculateTotalVariance(ClusterList* clusters) {
    double total = 0.0;
    for (int i = 0; i < clusters->size; i++) {
        total += calculateClusterVariance(clusters->clusters[i]);
    }
    return total;
}

// созд-е списка кластеров
ClusterList* createClusterList(int n_clusters) {
    ClusterList* cl = (ClusterList*)malloc(sizeof(ClusterList));
    cl->size = n_clusters;
    cl->score = DBL_MAX;
    cl->clusters = (Vector**)malloc(n_clusters * sizeof(Vector*));
    
    for (int i = 0; i < n_clusters; i++) {
        cl->clusters[i] = createVector(sizeof(double));
    }
    return cl;
}

// освобожд-е списка кластеров
void freeClusterList(ClusterList* cl) {
    if (cl) {
        for (int i = 0; i < cl->size; i++) {
            vectorFree(cl->clusters[i]);
        }
        free(cl->clusters);
        free(cl);
    }
}

// копиров-е списка кластеров
ClusterList* copyClusterList(ClusterList* src) {
    ClusterList* dst = createClusterList(src->size);
    dst->score = src->score;
    
    for (int i = 0; i < src->size; i++) {
        // очмсика нового вектора
        while (dst->clusters[i]->size > 0) {
            free(popVectorItem(dst->clusters[i], 0));
        }
        
        // копир-е эл-ов из src в dst
        for (size_t j = 0; j < src->clusters[i]->size; j++) {
            double* value = (double*)getVectorItem(src->clusters[i], j);
            appendVectorItem(dst->clusters[i], value);
        }
    }
    return dst;
}

// печать кластера
void printCluster(Vector* cluster) {
    printf("[");
    for (size_t i = 0; i < cluster->size; i++) {
        double* value = (double*)getVectorItem(cluster, i);
        printf("%.2f", *value);
        if (i < cluster->size - 1) printf(", ");
    }
    printf("]");
}

// печать списка кластера
void printClusterList(ClusterList* clusters) {
    printf("кластеризация (score = %.4f):\n", clusters->score);
    for (int i = 0; i < clusters->size; i++) {
        printf("  кластер %d: ", i + 1);
        printCluster(clusters->clusters[i]);
        double centroid = calculateCentroid(clusters->clusters[i]);
        printf(" (центр = %.2f, дисперсия = %.2f)\n", 
               centroid, calculateClusterVariance(clusters->clusters[i]));
    }
}

// генерация всех разбиений масива на k кластеров
void generateAllPartitions(Vector* arr, int index, Vector** current_clusters,
                          int n_clusters, ClusterList** best_clusters,
                          double* best_score) {
    // базовый случай: обработали все элементы
    if (index == (int)arr->size) {
        // проверка, что нет пустых кластеров
        int empty_cluster = 0;
        for (int i = 0; i < n_clusters; i++) {
            if (current_clusters[i]->size == 0) {
                empty_cluster = 1;
                break;
            }
        }
        if (empty_cluster) {
            return;
        }
        
        // вычисл-е качества кластеризации
        double total_variance = 0.0;
        for (int i = 0; i < n_clusters; i++) {
            total_variance += calculateClusterVariance(current_clusters[i]);
        }
        
        // если нашли лучшее решение
        if (total_variance < *best_score) {
            *best_score = total_variance;
            
            // копирование текущего кластера в best_clusters
            for (int i = 0; i < n_clusters; i++) {
                // очищение старого кластера
                while ((*best_clusters)->clusters[i]->size > 0) {
                    free(popVectorItem((*best_clusters)->clusters[i], 0));
                }
                
                // копируем элементы из текущего кластера
                for (size_t j = 0; j < current_clusters[i]->size; j++) {
                    double* value = (double*)getVectorItem(current_clusters[i], j);
                    appendVectorItem((*best_clusters)->clusters[i], value);
                }
            }
            (*best_clusters)->score = total_variance;
        }
        return;
    }
    
    // получаем текущий элемент
    double* current_value = (double*)getVectorItem(arr, index);
    
    // рекурсивный случай: пробуем добавить текущий элемент в каждый кластер
    for (int i = 0; i < n_clusters; i++) {
        appendVectorItem(current_clusters[i], current_value);
        generateAllPartitions(arr, index + 1, current_clusters, n_clusters,
                            best_clusters, best_score);
        // откат - удаляем последний элемент
        free(popVectorItem(current_clusters[i], current_clusters[i]->size - 1));
    }
}

// осн-я ф-ция кластеризации
ClusterList* cluster(Vector* arr, unsigned char n_clusters) {
    // проверка входных данных
    if (n_clusters == 0 || n_clusters > (unsigned char)arr->size) {
        printf("некорректное количество кластеров\n");
        return NULL;
    }
    
    // созд-е рабочих кластеров для перебора
    Vector** current_clusters = (Vector**)malloc(n_clusters * sizeof(Vector*));
    for (int i = 0; i < n_clusters; i++) {
        current_clusters[i] = createVector(sizeof(double));
    }
    
    // созд-е лучшего рез-та
    ClusterList* best_clusters = createClusterList(n_clusters);
    double best_score = DBL_MAX;
    
    // генерация всех разбиений 
    printf("начало перебора для n=%zu, k=%d\n", arr->size, n_clusters);
    generateAllPartitions(arr, 0, current_clusters, n_clusters, 
                         &best_clusters, &best_score);
    
    // освобожд-е рабочих кластеров
    for (int i = 0; i < n_clusters; i++) {
        vectorFree(current_clusters[i]);
    }
    free(current_clusters);
    
    return best_clusters;
}

// получ-е текущего времени
double get_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}

// созд-е вектора из массива double
Vector* createDoubleVector(double* data, int n) {
    Vector* vec = createVector(sizeof(double));
    for (int i = 0; i < n; i++) {
        appendVectorItem(vec, &data[i]);
    }
    return vec;
}

// запуск эксперементов
void run_experiments() {
    // тестовые массивы
    double data5[] = {1, 2, 3, 4, 5};                  // 5 элементов
    double data6[] = {1, 2, 3, 10, 11, 12};            // 6 элементов
    double data7[] = {1, 2, 3, 4, 5, 6, 7};            // 7 элементов
    double data10[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // 10 элементов

    printf("алгоритм: полный перебор\n");
    printf("сложность: очень большая, растет как k^n\n\n");

    // 1) 5 элементов, 2 кластера
    printf("5 эл-ов, 2 кластера\n");
    Vector *arr1 = createDoubleVector(data5, 5);
    printf("Данные: ");
    printCluster(arr1);
    printf("\n");
    double t1 = get_time();
    ClusterList *res1 = cluster(arr1, 2);
    t1 = get_time() - t1;
    if (res1) {
        printClusterList(res1);
        printf("Время: %.3f сек\n", t1);
        freeClusterList(res1);
    }
    vectorFree(arr1);
    printf("\n");

    // 2) 7 элементов, 2 кластера
    printf("7 эл-ов, 2 кластера\n");
    Vector *arr2 = createDoubleVector(data7, 7);
    printf("Данные: ");
    printCluster(arr2);
    printf("\n");
    double t2 = get_time();
    ClusterList *res2 = cluster(arr2, 2);
    t2 = get_time() - t2;
    if (res2) {
        printClusterList(res2);
        printf("Время: %.3f сек\n", t2);
        freeClusterList(res2);
    }
    vectorFree(arr2);
    printf("\n");

    // 3) 10 элементов, 2 кластера
    printf("10 эл-ов, 2 кластера\n");
    Vector *arr3 = createDoubleVector(data10, 10);
    printf("Данные: ");
    printCluster(arr3);
    printf("\n");
    double t3 = get_time();
    ClusterList *res3 = cluster(arr3, 2);
    t3 = get_time() - t3;
    if (res3) {
        printClusterList(res3);
        printf("Время: %.3f сек\n", t3);
        freeClusterList(res3);
    }
    vectorFree(arr3);
    printf("\n");

    // 4) 5 элементов, 3 кластера
    printf("5 эл-ов, 3 кластера\n");
    Vector *arr4 = createDoubleVector(data5, 5);
    printf("Данные: ");
    printCluster(arr4);
    printf("\n");
    double t4 = get_time();
    ClusterList *res4 = cluster(arr4, 3);
    t4 = get_time() - t4;
    if (res4) {
        printClusterList(res4);
        printf("Время: %.3f сек\n", t4);
        freeClusterList(res4);
    }
    vectorFree(arr4);
    printf("\n");

    // 5) 7 элементов, 3 кластера
    printf("7 эл-ов, 3 кластера\n");
    Vector *arr5 = createDoubleVector(data7, 7);
    printf("Данные: ");
    printCluster(arr5);
    printf("\n");
    double t5 = get_time();
    ClusterList *res5 = cluster(arr5, 3);
    t5 = get_time() - t5;
    if (res5) {
        printClusterList(res5);
        printf("Время: %.3f сек\n", t5);
        freeClusterList(res5);
    }
    vectorFree(arr5);
    printf("\n");

    // 6) 5 элементов, 4 кластера
    printf("5 эл-ов, 4 кластера\n");
    Vector *arr6 = createDoubleVector(data5, 5);
    printf("Данные: ");
    printCluster(arr6);
    printf("\n");
    double t6 = get_time();
    ClusterList *res6 = cluster(arr6, 4);
    t6 = get_time() - t6;
    if (res6) {
        printClusterList(res6);
        printf("Время: %.3f сек\n", t6);
        freeClusterList(res6);
    }
    vectorFree(arr6);
    printf("\n");

    // пример из задания
    printf("\nПример из задания:\n");
    printf("Массив: [1, 2, 3, 10, 11, 12]\n");
    printf("Нужно разбить на 2 кластера\n\n");

    Vector *example = createDoubleVector(data6, 6);
    ClusterList *example_res = cluster(example, 2);

    if (example_res) {
        printf("Лучшее разбиение:\n");
        for (int i = 0; i < example_res->size; i++) {
            printf("Кластер %d: ", i + 1);
            printCluster(example_res->clusters[i]);
            double c = calculateCentroid(example_res->clusters[i]);
            printf(" центр=%.1f\n", c);
        }

        // Простой расчет
        printf("\nСчитаем качество:\n");
        double total = 0;
        for (int i = 0; i < example_res->size; i++) {
            double c = calculateCentroid(example_res->clusters[i]);
            Vector *clust = example_res->clusters[i];
            double var = 0;
            for (size_t j = 0; j < clust->size; j++) {
                double *val = (double *)getVectorItem(clust, j);
                double d = *val - c;
                var += d * d;
            }
            printf("Кластер %d: дисперсия = %.1f\n", i + 1, var);
            total += var;
        }
        printf("Общая дисперсия = %.1f\n", total);

        freeClusterList(example_res);
    }
    vectorFree(example);
}

int main(int argc, char* argv[]) {
    // если запустили без параметров
    if (argc == 1) {
        run_experiments();
        return 0;
    }
    
    // если запустили с параметрами
    if (argc == 3) {
        int n, k;
        
        // попытка чтения числа
        n = atoi(argv[1]);
        k = atoi(argv[2]);
        
        // проверяем, что числа правильные
        if (n < 1 || k < 1 || k > n) {
            return 1;
        }
        
        printf("созд-е массива из %d чисел\n", n);
        
        // созд-е вектора
        Vector* data = createVector(sizeof(double));
        
        // инициализ-я генератора случайных чисел
        srand(time(NULL));
        
        // заполн-е случайными числами
        for (int i = 0; i < n; i++) {
            double x = (rand() % 100) + 1;
            appendVectorItem(data, &x);
        }
        
        printf("массив: ");
        printCluster(data);
        printf("\n\n");
        
        printf("поиск лучшего рабзиения на %d кластеров\n", k);
        
        // замер времени
        double start = get_time();
        ClusterList* best = cluster(data, k);
        double end = get_time();
        
        if (best == NULL) {
            vectorFree(data);
            return 1;
        }
        
        // результат
        printf("\nразбиение:\n");
        printClusterList(best);
        
        printf("\nвремя %.3f секунд\n", end - start);

        freeClusterList(best);
        vectorFree(data);
        
        return 0;
    }
     
    return 1;
}
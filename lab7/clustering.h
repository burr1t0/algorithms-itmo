#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "../lab3/vector/generic.h"

typedef struct {
    Vector** clusters;
    int size;
    double score;
} ClusterList;

// вычисление центроида
double calculateCentroid(Vector* cluster);
// вычисл-е внутрикластерной дисперсии
double docalculateClusterVariance(Vector* cluster);
// вычисл-е общей дисперсии для всех кластеров
double calculateTotalVariance(ClusterList* cluster);
// рекурсивно генерирует разбиения массива на кластеры 
void generateAllPartitions(Vector* arr, int index, Vector** current_clusters, int n_clusters, ClusterList** best_clusters, double* best_score);
// осн-я ф-ция кластеризации методом полного перебора
ClusterList *cluster(Vector *arr, unsigned char n_clusters);

// возвращ-т текущее время работы в сек
double get_time();
// запуск экспериментов с кластеризацией
void run_experiments();
// полное освобождение памяти занятой ClusterList
void freeClusterList(ClusterList *cl);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// пузырьковая сорт-ка
void bubbleSort(int* arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


// ф-ция для разделения массива
int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1); // инд-с меньшего эл-та

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // замена arr[i] и arr[high]
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}


// быстрая сортировка quickSort
void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // рекурсивно сортируем эл-ты до и после разделения
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Обертка для quickSort
void quickSortWrapper(int* arr, size_t n) {
    quickSort(arr, 0, n - 1);
}

// countingSort
void countingSort(int* arr, size_t n, int max_value) {
    // созд-е массива для каждого эл-та
    int *count = (int *)calloc(max_value + 1, sizeof(int));

    // подсчет кол-ва каждого эл-та
    for (size_t i = 0; i < n; i++) {
        count[arr[i]]++;
    }

    // восстановка отсорт-го массива
    size_t index = 0;
    for (int i = 0; i <= max_value; i++) {
        while (count[i] > 0) {
            arr[index++] = i;
            count[i]--;
        }
    }

    free(count);
}

void countingSortWrapper(int *arr, size_t n) {
    countingSort(arr, n, 10000);
}

int *createRandomArray(size_t size, int max_value) {
    int *array = (int *)malloc(size * sizeof(int));
    if (!array) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        if (max_value > 0) {
            array[i] = rand() % (max_value + 1);
        }
        else {
            array[i] = rand(); 
        }
    }

    return array;
}


// ф-ция измерения
double measureSortTime(void (*sortFunc)(int *, size_t), size_t size, int iterations, char *filename, int max_val)
{
    double total = 0;
    double *times = malloc(iterations * sizeof(double));

    for (int i = 0; i < iterations; i++)
    {
        int *arr = malloc(size * sizeof(int));
        for (size_t j = 0; j < size; j++)
        {
            if (max_val > 0)
                arr[j] = rand() % (max_val + 1);
            else
                arr[j] = rand();
        }

        clock_t start = clock();
        sortFunc(arr, size);
        clock_t end = clock();

        times[i] = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        total += times[i];
        free(arr);
    }

    FILE *f = fopen(filename, "w");
    if (f)
    {
        for (int i = 0; i < iterations; i++)
            fprintf(f, "%.3f\n", times[i]);
        fclose(f);
    }

    free(times);
    return total / iterations;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: %s size iterations\n", argv[0]);
        return 1;
    }

    size_t size = atoi(argv[1]);
    int iter = atoi(argv[2]);
    srand(time(NULL));

    char names[3][20] = {"bubble", "quick", "counting"};
    int max_vals[3] = {0, 0, 10000};
    double results[3];

    for (int i = 0; i < 3; i++)
    {
        char filename[50];
        sprintf(filename, "%s_sort_%d.txt", names[i], (int)size);

        void (*func)(int *, size_t);
        if (i == 0)
            func = bubbleSort;
        else if (i == 1)
            func = quickSortWrapper;
        else
            func = countingSortWrapper;

        results[i] = measureSortTime(func, size, iter, filename, max_vals[i]);
    }

    printf("\nResults for N=%zu:\n", size);
    for (int i = 0; i < 3; i++)
    {
        printf("%-10s: %.2f ms\n", names[i], results[i]);
    }

    return 0;
}
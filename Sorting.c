#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int* input_array = NULL;
int* sorted_array = NULL;
int SIZE = 0;

typedef struct {
    int start;
    int end;
} Range;

void insertion_sort(int* arr, int start, int end) {
    for (int i = start + 1; i <= end; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= start && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void* sort_thread(void* arg) {
    Range* r = (Range*) arg;
    insertion_sort(input_array, r->start, r->end);
    free(r);
    pthread_exit(NULL);
}

void* merge_thread(void* arg) {
    int mid = SIZE / 2;
    int i = 0, j = mid, k = 0;

    while (i < mid && j < SIZE) {
        if (input_array[i] < input_array[j]) {
            sorted_array[k++] = input_array[i++];
        } else {
            sorted_array[k++] = input_array[j++];
        }
    }

    while (i < mid) {
        sorted_array[k++] = input_array[i++];
    }
    while (j < SIZE) {
        sorted_array[k++] = input_array[j++];
    }

    pthread_exit(NULL);
}

int main() {
    printf("Enter the number of List lenght: ");
    scanf("%d", &SIZE);

    input_array = (int*) malloc(SIZE * sizeof(int));
    sorted_array = (int*) malloc(SIZE * sizeof(int));

    printf("enter the list number with space:\n");
    for (int i = 0; i < SIZE; i++) {
        scanf("%d", &input_array[i]);
    }

    pthread_t tid1, tid2, tid3;

    Range* r1 = malloc(sizeof(Range));
    r1->start = 0;
    r1->end = (SIZE / 2) - 1;

    Range* r2 = malloc(sizeof(Range));
    r2->start = SIZE / 2;
    r2->end = SIZE - 1;

    pthread_create(&tid1, NULL, sort_thread, (void*) r1);
    pthread_create(&tid2, NULL, sort_thread, (void*) r2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_create(&tid3, NULL, merge_thread, NULL);
    pthread_join(tid3, NULL);

    printf("sorted array:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    free(input_array);
    free(sorted_array);

    return 0;
}


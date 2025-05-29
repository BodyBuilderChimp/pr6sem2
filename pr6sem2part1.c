#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 4

typedef struct {
    int* data;
    int target;
    int start;
    int end;
    int* found;
    int* result_index;
    pthread_mutex_t* mutex;
    pthread_barrier_t* barrier;
} thread_args_t;

void* search_first(void* arg) {
    thread_args_t* args = (thread_args_t*)arg;

    for (int i = args->start; i < args->end; ++i) {
        pthread_mutex_lock(args->mutex);
        if (*args->found) {
            pthread_mutex_unlock(args->mutex);
            break;
        }
        if (args->data[i] == args->target) {
            *args->found = 1;
            *args->result_index = i;
            pthread_mutex_unlock(args->mutex);
            break;
        }
        pthread_mutex_unlock(args->mutex);
    }

    pthread_barrier_wait(args->barrier);
    return NULL;
}

int main() {
    int data[] = {1, 3, 5, 7, 3, 9, 3, 5, 3, 1, 0, 3, 8};
    int size = sizeof(data) / sizeof(data[0]);
    int target = 3;

    pthread_t threads[THREADS];
    thread_args_t args[THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, THREADS);

    int found = 0;
    int result_index = -1;
    int chunk = size / THREADS;

    for (int i = 0; i < THREADS; ++i) {
        args[i].data = data;
        args[i].target = target;
        args[i].start = i * chunk;
        args[i].end = (i == THREADS - 1) ? size : args[i].start + chunk;
        args[i].found = &found;
        args[i].result_index = &result_index;
        args[i].mutex = &mutex;
        args[i].barrier = &barrier;
        pthread_create(&threads[i], NULL, search_first, &args[i]);
    }

    for (int i = 0; i < THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    if (result_index != -1) {
        printf("Первое вхождение найдено на индексе: %d\n", result_index);
    } else {
        printf("Элемент не найден\n");
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);
    return 0;
}

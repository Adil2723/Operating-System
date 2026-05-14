#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_FLIGHTS 100
#define RUNWAYS 2
#define TOTAL_FLIGHTS 20

#define TAKEOFF 1
#define LANDING 2
#define LOW_FUEL 3
#define EMERGENCY 4

typedef struct {
    int id;
    int type;
    int priority;
    time_t arrivalTime;
} Flight;

Flight flightQueue[MAX_FLIGHTS];

int flightCount = 0;
int handledFlights = 0;
int done = 0;

pthread_mutex_t queueLock;
pthread_mutex_t runwayLock[RUNWAYS];
pthread_cond_t queueCond;

void sortQueue();
void insertFlight(Flight f);
Flight getFlight();
void updatePriority();

void* generator(void* arg);
void* runway(void* arg);

void sortQueue() {
    for (int i = 0; i < flightCount - 1; i++) {
        for (int j = i + 1; j < flightCount; j++) {

            if (flightQueue[i].priority < flightQueue[j].priority) {

                Flight temp = flightQueue[i];
                flightQueue[i] = flightQueue[j];
                flightQueue[j] = temp;
            }
        }
    }
}

void insertFlight(Flight f) {

    flightQueue[flightCount++] = f;

    sortQueue();
}

Flight getFlight() {

    Flight f = flightQueue[0];

    for (int i = 1; i < flightCount; i++) {
        flightQueue[i - 1] = flightQueue[i];
    }

    flightCount--;

    return f;
}

void updatePriority() {

    time_t now = time(NULL);

    for (int i = 0; i < flightCount; i++) {

        if (difftime(now, flightQueue[i].arrivalTime) > 5) {
            flightQueue[i].priority++;
        }
    }

    sortQueue();
}

void* generator(void* arg) {

    for (int i = 1; i <= TOTAL_FLIGHTS; i++) {

        Flight f;

        f.id = i;
        f.type = rand() % 4 + 1;
        f.priority = f.type;
        f.arrivalTime = time(NULL);

        pthread_mutex_lock(&queueLock);

        insertFlight(f);

        printf(
            "[GENERATED] Flight %d | Type: %d | Priority: %d\n",
            f.id,
            f.type,
            f.priority
        );

        pthread_cond_signal(&queueCond);

        pthread_mutex_unlock(&queueLock);

        usleep(200000);
    }

    pthread_mutex_lock(&queueLock);

    done = 1;

    pthread_cond_broadcast(&queueCond);

    pthread_mutex_unlock(&queueLock);

    return NULL;
}

void* runway(void* arg) {

    int id = *(int*)arg;

    while (1) {

        pthread_mutex_lock(&queueLock);

        while (flightCount == 0 && !done) {
            pthread_cond_wait(&queueCond, &queueLock);
        }

        if (flightCount == 0 && done) {
            pthread_mutex_unlock(&queueLock);
            break;
        }

        updatePriority();

        Flight f = getFlight();

        pthread_mutex_unlock(&queueLock);

        pthread_mutex_lock(&runwayLock[id]);

        printf(
            "[RUNWAY %d] Handling Flight %d | Type: %d\n",
            id,
            f.id,
            f.type
        );

        sleep(1);

        pthread_mutex_unlock(&runwayLock[id]);

        pthread_mutex_lock(&queueLock);

        handledFlights++;

        pthread_mutex_unlock(&queueLock);
    }

    return NULL;
}

int main() {

    srand(time(NULL));

    pthread_mutex_init(&queueLock, NULL);
    pthread_cond_init(&queueCond, NULL);

    for (int i = 0; i < RUNWAYS; i++) {
        pthread_mutex_init(&runwayLock[i], NULL);
    }

    pthread_t gen;

    pthread_create(&gen, NULL, generator, NULL);

    pthread_t r[RUNWAYS];
    int ids[RUNWAYS];

    for (int i = 0; i < RUNWAYS; i++) {

        ids[i] = i;

        pthread_create(&r[i], NULL, runway, &ids[i]);
    }

    pthread_join(gen, NULL);

    for (int i = 0; i < RUNWAYS; i++) {
        pthread_join(r[i], NULL);
    }

    printf("All flights handled: %d\n", handledFlights);

    return 0;
}

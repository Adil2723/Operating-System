#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define N 1000
#define TILE 100
#define MAX_THREADS 8
#define SATELLITES 5

#define T_HOT 35
#define T_COLD -10

double satelliteData[SATELLITES][N][N];
double globalMatrix[N][N];
double normalized[N][N];
double risk[N][N];

double global_min = 1e9;
double global_max = -1e9;
double global_sum = 0;

int hotspot_count = 0;
int coldspot_count = 0;
int anomaly_count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int nextTile = 0;

void initializeData() {
    srand(time(NULL));

    for (int k = 0; k < SATELLITES; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {

                satelliteData[k][i][j] =
                    (rand() % 71) - 20;

                if (rand() % 20 == 0) {
                    satelliteData[k][i][j] = NAN;
                }
            }
        }
    }
}

void* processSatellite(void* arg) {

    int id = *(int*)arg;

    for (int i = 1; i < N - 1; i++) {

        for (int j = 1; j < N - 1; j++) {

            if (isnan(satelliteData[id][i][j])) {

                double up    = satelliteData[id][i - 1][j];
                double down  = satelliteData[id][i + 1][j];
                double left  = satelliteData[id][i][j - 1];
                double right = satelliteData[id][i][j + 1];

                satelliteData[id][i][j] =
                    (up + down + left + right) / 4.0;
            }
        }
    }

    pthread_exit(NULL);
}

void mergeMatrices() {

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            double sum = 0;
            int count = 0;

            for (int k = 0; k < SATELLITES; k++) {

                if (!isnan(satelliteData[k][i][j])) {

                    sum += satelliteData[k][i][j];
                    count++;
                }
            }

            if (count > 0)
                globalMatrix[i][j] = sum / count;
            else
                globalMatrix[i][j] = 0;
        }
    }
}

void* processTile(void* arg) {

    while (1) {

        pthread_mutex_lock(&lock);
        int tile = nextTile++;
        pthread_mutex_unlock(&lock);

        int totalTiles = (N / TILE) * (N / TILE);

        if (tile >= totalTiles)
            break;

        int row = (tile / (N / TILE)) * TILE;
        int col = (tile % (N / TILE)) * TILE;

        double local_min = 1e9;
        double local_max = -1e9;

        double sum = 0;
        double sq_sum = 0;

        int count = 0;

        for (int i = row; i < row + TILE; i++) {

            for (int j = col; j < col + TILE; j++) {

                double val = globalMatrix[i][j];

                if (val < local_min)
                    local_min = val;

                if (val > local_max)
                    local_max = val;

                sum += val;
                sq_sum += val * val;

                count++;
            }
        }

        double mean = sum / count;

        double variance =
            (sq_sum / count) - (mean * mean);

        double std_dev = sqrt(variance);

        int local_anomalies = 0;

        for (int i = row; i < row + TILE; i++) {

            for (int j = col; j < col + TILE; j++) {

                if (fabs(globalMatrix[i][j] - mean)
                    > 2 * std_dev) {

                    local_anomalies++;
                }
            }
        }

        pthread_mutex_lock(&lock);

        if (local_min < global_min)
            global_min = local_min;

        if (local_max > global_max)
            global_max = local_max;

        global_sum += sum;

        anomaly_count += local_anomalies;

        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

void* hotspotTask(void* arg) {

    int local_hotspots = 0;

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            if (globalMatrix[i][j] > T_HOT) {
                local_hotspots++;
            }
        }
    }

    pthread_mutex_lock(&lock);
    hotspot_count += local_hotspots;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void* coldspotTask(void* arg) {

    int local_coldspots = 0;

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            if (globalMatrix[i][j] < T_COLD) {
                local_coldspots++;
            }
        }
    }

    pthread_mutex_lock(&lock);
    coldspot_count += local_coldspots;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void* normalizeTask(void* arg) {

    double range = global_max - global_min;

    if (range == 0)
        range = 1;

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            normalized[i][j] =
                (globalMatrix[i][j] - global_min)
                / range;
        }
    }

    pthread_exit(NULL);
}

void computeRisk() {

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            double proximity_hot = 1;
            double proximity_cold = 1;

            risk[i][j] =
                normalized[i][j]
                * proximity_hot
                / (proximity_cold + 1);
        }
    }
}

int main() {

    initializeData();

    pthread_t satelliteThreads[SATELLITES];
    int ids[SATELLITES];

    for (int i = 0; i < SATELLITES; i++) {

        ids[i] = i;

        pthread_create(
            &satelliteThreads[i],
            NULL,
            processSatellite,
            &ids[i]
        );
    }

    for (int i = 0; i < SATELLITES; i++) {

        pthread_join(
            satelliteThreads[i],
            NULL
        );
    }

    mergeMatrices();

    pthread_t tileThreads[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {

        pthread_create(
            &tileThreads[i],
            NULL,
            processTile,
            NULL
        );
    }

    for (int i = 0; i < MAX_THREADS; i++) {

        pthread_join(
            tileThreads[i],
            NULL
        );
    }

    pthread_t hotThread;
    pthread_t coldThread;
    pthread_t normalizeThread;

    pthread_create(
        &hotThread,
        NULL,
        hotspotTask,
        NULL
    );

    pthread_create(
        &coldThread,
        NULL,
        coldspotTask,
        NULL
    );

    pthread_create(
        &normalizeThread,
        NULL,
        normalizeTask,
        NULL
    );
  
    pthread_join(hotThread, NULL);
    pthread_join(coldThread, NULL);
    pthread_join(normalizeThread, NULL);
    computeRisk();
    double average = global_sum / (N * N);

    printf("\n===== Climate Analysis Report =====\n");
    printf("Global Maximum Temperature : %.2f\n",
           global_max);
    printf("Global Minimum Temperature : %.2f\n",
           global_min);
    printf("Global Average Temperature : %.2f\n",
           average);
    printf("Hotspots Detected          : %d\n",
           hotspot_count);
    printf("Coldspots Detected         : %d\n",
           coldspot_count);
    printf("Anomalies Detected         : %d\n",
           anomaly_count);
    printf("Sample Risk Value [0][0]  : %.4f\n",
           risk[0][0]);

    pthread_mutex_destroy(&lock);

    return 0;
}

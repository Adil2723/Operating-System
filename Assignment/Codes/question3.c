#include <pthread.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <time.h> 
#include <stdlib.h> 
#define MAX 200 
#define TOTAL_PATIENTS 30 
typedef struct { 
int id; 
int type; 
time_t arrival_time; 
} Patient; 
Patient criticalQ[MAX], seriousQ[MAX], normalQ[MAX]; 
int c_front = 0, c_rear = 0; 
int s_front = 0, s_rear = 0; 
int n_front = 0, n_rear = 0; 
int treated_count = 0; 
pthread_mutex_t mutexQ = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond_doctor = PTHREAD_COND_INITIALIZER; 
typedef struct { 
int id; 
int isSenior; 
int normalCount; 
} Doctor; 
void enqueue(Patient arr[], int *rear, Patient p) { 
arr[(*rear)++] = p; 
} 
Patient dequeue(Patient arr[], int *front) { 
return arr[(*front)++]; 
} 
int isEmpty(int front, int rear) { 
return front == rear; 
} 
void* patientThread(void* arg) { 
Patient p = *(Patient*)arg; 
pthread_mutex_lock(&mutexQ); 
p.arrival_time = time(NULL); 
if (p.type == 2) 
enqueue(criticalQ, &c_rear, p); 
else if (p.type == 1) 
enqueue(seriousQ, &s_rear, p); 
else 
enqueue(normalQ, &n_rear, p); 
printf("Patient %d arrived Type: %d\n", p.id, p.type); 
pthread_cond_signal(&cond_doctor); 
pthread_mutex_unlock(&mutexQ); 
pthread_exit(NULL); 
} 
void* doctorThread(void* arg) { 
Doctor* doc = (Doctor*)arg; 
while (1) { 
pthread_mutex_lock(&mutexQ); 
if (treated_count >= TOTAL_PATIENTS) { 
pthread_mutex_unlock(&mutexQ); 
break; 
} 
while (isEmpty(c_front, c_rear) && isEmpty(s_front, s_rear) && isEmpty(n_front, n_rear)) 
{ 
pthread_cond_wait(&cond_doctor, &mutexQ); 
} 
Patient p; 
int found = 0; 
if (!isEmpty(c_front, c_rear) && doc->isSenior) { 
p = dequeue(criticalQ, &c_front); 
found = 1; 
} 
else if (!isEmpty(s_front, s_rear)) { 
p = dequeue(seriousQ, &s_front); 
found = 1; 
} 
else if (!isEmpty(n_front, n_rear)) { 
p = dequeue(normalQ, &n_front); 
found = 1; 
} 
if (!found) { 
pthread_mutex_unlock(&mutexQ); 
continue; 
} 
if (p.type == 0) 
doc->normalCount++; 
else 
doc->normalCount = 0; 
if (doc->normalCount == 3 && !isEmpty(s_front, s_rear)) { 
p = dequeue(seriousQ, &s_front); 
doc->normalCount = 0; 
} 
treated_count++; 
pthread_mutex_unlock(&mutexQ); 
printf("Doctor %d treating Patient %d Type: %d\n", doc->id, p.id, p.type); 
sleep(1); 
} 
pthread_exit(NULL); 
} 
int main() { 
srand(time(NULL)); 
pthread_t docThreads[2]; 
Doctor docs[2] = { 
{1, 1, 0},  // Senior 
{2, 0, 0}   // Junior 
}; 
for (int i = 0; i < 2; i++) 
pthread_create(&docThreads[i], NULL, doctorThread, &docs[i]); 
pthread_t patients[TOTAL_PATIENTS]; 
Patient pts[TOTAL_PATIENTS]; 
for (int i = 0; i < TOTAL_PATIENTS; i++) { 
pts[i].id = i; 
pts[i].type = rand() % 3; 
pts[i].arrival_time = time(NULL); 
pthread_create(&patients[i], NULL, patientThread, &pts[i]); 
usleep(100000); 
} 
for (int i = 0; i < TOTAL_PATIENTS; i++) 
pthread_join(patients[i], NULL); 
pthread_cond_broadcast(&cond_doctor); 
for (int i = 0; i < 2; i++) 
pthread_join(docThreads[i], NULL); 
printf("All patients processed\n"); 
return 0; 
}

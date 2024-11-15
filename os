#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include <unistd.h>

pthread_mutex_t readWriteMutex;

pthread_mutex_t mutex;

int readCount = 0;

int sharedData = 0;

int iterations; // Number of iterations for readers and writers

void* reader(void* arg) {

 int id = *(int*)arg;

 for (int i = 0; i < iterations; i++) { // Run for a ϐixed number of iterations

 pthread_mutex_lock(&mutex); 

 readCount++; 

 if (readCount == 1) {

 pthread_mutex_lock(&readWriteMutex); 

 } 

 pthread_mutex_unlock(&mutex); 

 // Reading section

 printf("Reader %d: Reading data %d\n", id, sharedData);

 sleep(1); // Simulate reading time

 pthread_mutex_lock(&mutex); 

 readCount--; 

 if (readCount == 0) {

 pthread_mutex_unlock(&readWriteMutex); 

 } 

 pthread_mutex_unlock(&mutex); 

 sleep(1); // Simulate some delay before next read

 } 

 return NULL;

} 

void* writer(void* arg) {

 int id = *(int*)arg;

 for (int i = 0; i < iterations; i++) { // Run for a ϐixed number of iterations

 pthread_mutex_lock(&readWriteMutex); 

 // Writing section

 sharedData++; 

 printf("Writer %d: Writing data %d\n", id, sharedData);

 sleep(1); // Simulate writing time

 pthread_mutex_unlock(&readWriteMutex); 

 sleep(3); // Simulate some delay before next write
} 

 return NULL;

} 

int main() {

 int numReaders, numWriters;

 printf("Enter the number of readers: ");

 scanf("%d", &numReaders);

 printf("Enter the number of writers: ");

 scanf("%d", &numWriters);

 printf("Enter the number of iterations for each reader/writer: ");

 scanf("%d", &iterations);

 pthread_t* readers = malloc(numReaders * sizeof(pthread_t));

 pthread_t* writers = malloc(numWriters * sizeof(pthread_t));

 int* readerIDs = malloc(numReaders * sizeof(int));

 int* writerIDs = malloc(numWriters * sizeof(int));

 pthread_mutex_init(&readWriteMutex, NULL);

 pthread_mutex_init(&mutex, NULL);

 // Create reader threads

 for (int i = 0; i < numReaders; i++) {

 readerIDs[i] = i + 1;

 pthread_create(&readers[i], NULL, reader, &readerIDs[i]);

 } 

 // Create writer threads

 for (int i = 0; i < numWriters; i++) {

 writerIDs[i] = i + 1;

 pthread_create(&writers[i], NULL, writer, &writerIDs[i]);

 } 

 // Join threads

 for (int i = 0; i < numReaders; i++) {

 pthread_join(readers[i], NULL);

 } 

 for (int i = 0; i < numWriters; i++) {

 pthread_join(writers[i], NULL); 

 } 

 // Cleanup

 free(readers); 

 free(writers); 

 free(readerIDs); 

 free(writerIDs); 

 pthread_mutex_destroy(&readWriteMutex); 

 pthread_mutex_destroy(&mutex); 

 return 0;

}

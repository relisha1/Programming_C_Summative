
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_URLS 10

typedef struct {
    char url[256];
    int thread_id;
} ThreadData;

void* scrape(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    char command[512];

    sprintf(command, "curl -s %s -o output_%d.txt", data->url, data->thread_id);

    printf(" Thread %d: Fetching %s\n", data->thread_id, data->url);

    int res = system(command);

    if(res == 0)
        printf(" Thread %d: Saved to output_%d.txt\n", data->thread_id, data->thread_id);
    else
        printf(" Thread %d: Failed to fetch URL %s\n", data->thread_id, data->url);

    return NULL;
}

int main() {
    pthread_t threads[MAX_URLS];
    ThreadData thread_data[MAX_URLS];
    int num_urls;

    printf("Enter number of URLs (max %d): ", MAX_URLS);
    scanf("%d", &num_urls);

    if(num_urls <= 0 || num_urls > MAX_URLS) {
        printf(" Invalid number of URLs\n");
        return 1;
    }

    for(int i = 0; i < num_urls; i++) {
        printf("Enter URL %d: ", i + 1);
        scanf("%s", thread_data[i].url);
        thread_data[i].thread_id = i;
    }

    for(int i = 0; i < num_urls; i++) {
        if(pthread_create(&threads[i], NULL, scrape, &thread_data[i]) != 0) {
            printf(" Error creating thread %d\n", i);
            return 1;
        }
    }

    for(int i = 0; i < num_urls; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n All downloads completed!\n");

    return 0;
}
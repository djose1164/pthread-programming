#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_MEM 1024 * 2

char mem[MAX_MEM] = {0};
pthread_mutex_t lock;

/**
 * @brief Handle user's input. Dispaly on terminal what he types.
 * 
 * @param arg Pointer to buffer
 */
void *handle_input(void *arg)
{
    pthread_mutex_trylock(&lock);
    fgets(mem, sizeof mem, stdin);
    mem[strcspn(mem, "\n")] = 0;
    if (!strcmp(mem, "exit"))
        exit(0);
    else
        mem[strcspn(mem, "\0")] = '\n';

    printf("You said: ");
    printf("%s\n", mem);
    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * @brief Write to a file call io.txt.
 * 
 * @param arg Where to read from.
 */
void *wrtietofile(void *arg)
{
    puts("In writing to file");

    FILE *file = fopen("IO/io.txt", "a");
    assert(file);

    pthread_mutex_trylock(&lock);
    fprintf(file, "%s", mem);
    pthread_mutex_unlock(&lock);

    fclose(file);
    return NULL;
}

int main(int argc, char const *argv[])
{
    puts("Starting...");
    pthread_t tofile, handleinput;

    puts("Initializing mutex and creating threads...");
    pthread_mutex_init(&lock, NULL);
    while (1)
    {
        pthread_create(&handleinput, NULL, handle_input, (void *)mem);
        pthread_join(handleinput, NULL);
        pthread_create(&tofile, NULL, wrtietofile, (void *)mem);
        pthread_join(tofile, NULL);
    }
    pthread_mutex_destroy(&lock);

    puts("Look at `io.txt` on IO/ to see our conversation :)\n"
         "Bye!");
    return 0;
}

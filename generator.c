#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "generator.h"

char *generate_key(int length, char from, char to, const char *salt) {
    srand(time(NULL));

    int range = to - from + 1;
    char *key = (char *)malloc((length + 1) * sizeof(char));

    for (int i = 0; i < length; ++i) {
        key[i] = from + rand() % range;
    }
    key[length] = '\0';

    int salt_length = strlen(salt);
    char *final_key = (char *)malloc((length + salt_length + 1) * sizeof(char));

    for (int i = 0; i < length; ++i) {
        final_key[i] = key[i] ^ salt[i % salt_length];
    }

    final_key[length] = '\0';

    free(key);

    return final_key;
}

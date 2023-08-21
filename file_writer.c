#include <stdio.h>
#include <string.h>
#include "file_writer.h"

int write_to_file(const char *path, const char *content) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(file, "%s\n", content);

    fclose(file);
    return 0;
}

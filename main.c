#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "file_writer.h"

int main(int argc, char *argv[]) {
    if (argc != 9) {
        printf("Usage: generate -l <length> -f <from> -t <to> -o <output_path>\n");
        return 1;
    }

    int length;
    char from, to;
    const char *output_path;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-l") == 0) {
            length = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-f") == 0) {
            from = argv[i + 1][0];
        } else if (strcmp(argv[i], "-t") == 0) {
            to = argv[i + 1][0];
        } else if (strcmp(argv[i], "-o") == 0) {
            output_path = argv[i + 1];
        }
    }

    const char *salt = "12345678!@#$)(_^&*()";
    char *generated_key = generate_key(length, from, to, salt);

    if (write_to_file(output_path, generated_key) != 0) {
        fprintf(stderr, "Error writing to file\n");
        return 1;
    }

    printf("Key generated and written to file: %s\n", output_path);

    free(generated_key);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include "generator.h"
#include "file_writer.h"

void print_hex(const unsigned char *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

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

    // Generate AES key
    unsigned char aes_key[32]; // 256-bit key
    if (!RAND_bytes(aes_key, sizeof(aes_key))) {
        fprintf(stderr, "Error generating random key\n");
        return 1;
    }

    // Use AES key to generate final key
    const char *salt = "12345678!@#$)(_^&*()";
    char *generated_key = generate_key(length, from, to, salt);

    // Perform AES encryption
    AES_KEY aes_enc_key;
    AES_set_encrypt_key(aes_key, 256, &aes_enc_key);

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);

    unsigned char *encrypted_key = (unsigned char *)malloc(length + AES_BLOCK_SIZE);
    AES_cbc_encrypt((unsigned char *)generated_key, encrypted_key, length, &aes_enc_key, iv, AES_ENCRYPT);

    // Print encrypted key in hexadecimal format
    printf("Encrypted Key (Hex): ");
    print_hex(encrypted_key, length + AES_BLOCK_SIZE);

    // Write encrypted key to file
    if (write_to_file(output_path, (char *)encrypted_key) != 0) {
        fprintf(stderr, "Error writing to file\n");
        return 1;
    }

    printf("Encrypted key written to file: %s\n", output_path);

    // Clean up
    free(generated_key);
    free(encrypted_key);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "generator.h"
#include "file_writer.h"

void print_hex(const unsigned char *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void random_permute(unsigned char *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        size_t j = rand() % length;
        unsigned char temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }
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
    unsigned char aes_key[32];
    if (!RAND_bytes(aes_key, sizeof(aes_key))) {
        fprintf(stderr, "Error generating random key\n");
        return 1;
    }

    // Use AES key to generate final key
    const char *salt = "12345678!@#$)(_^&*()";
    char *generated_key = generate_key(length, from, to, salt);

    // Calculate the required padded length
    size_t padded_length = (length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE * AES_BLOCK_SIZE;

    // Allocate memory for the padded input data
    unsigned char *padded_input = (unsigned char *)malloc(padded_length);
    if (!padded_input) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    // Copy the input data to the padded buffer
    memcpy(padded_input, generated_key, length);

    // Perform AES encryption
    AES_KEY aes_enc_key;
    AES_set_encrypt_key(aes_key, 256, &aes_enc_key);

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);

    unsigned char *encrypted_key = (unsigned char *)malloc(padded_length);
    AES_cbc_encrypt(padded_input, encrypted_key, padded_length, &aes_enc_key, iv, AES_ENCRYPT);

    // Perform a random permutation on the encrypted key
    random_permute(encrypted_key, padded_length);

    // Convert encrypted key to Base64 encoding
    size_t base64_length = ((padded_length + 2) / 3) * 4;
    unsigned char *base64_encrypted_key = (unsigned char *)malloc(base64_length + 1);
    if (!base64_encrypted_key) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    EVP_ENCODE_CTX *ctx = EVP_ENCODE_CTX_new();
    int base64_len = EVP_EncodeBlock(base64_encrypted_key, encrypted_key, padded_length);
    base64_encrypted_key[base64_len] = '\0';

    // Remove trailing equal signs from the Base64 string
    while (base64_encrypted_key[base64_len - 1] == '=') {
        base64_encrypted_key[--base64_len] = '\0';
    }

    // Write Base64 encoded encrypted key to file
    if (write_to_file(output_path, (char *)base64_encrypted_key) != 0) {
        fprintf(stderr, "Error writing to file\n");
        return 1;
    }

    printf("Encrypted key written to file: %s\n", output_path);

    // Clean up
    EVP_ENCODE_CTX_free(ctx);
    free(generated_key);
    free(padded_input);
    free(encrypted_key);
    free(base64_encrypted_key);

    return 0;
}

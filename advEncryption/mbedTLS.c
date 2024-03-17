#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbedtls/aes.h>

#define KEY_SIZE 32
int aes_encrypt_file(const char *input_filename, const char *output_filename, const unsigned char *key) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");

    if (!input_file || !output_file) {
        perror("Error opening file");
        return -1;
    }

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    unsigned char iv[16] = { 0 };
    mbedtls_aes_setkey_enc(&aes, key, KEY_SIZE * 8);
    unsigned char input_buffer[16];
    unsigned char output_buffer[16];

    while (1) {
        size_t bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file);
        if (bytes_read == 0) {
            break; 
        }
        if (bytes_read < sizeof(input_buffer)) {
            memset(input_buffer + bytes_read, sizeof(input_buffer) - bytes_read, sizeof(input_buffer) - bytes_read);
        }

        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, sizeof(input_buffer), iv, input_buffer, output_buffer);

        fwrite(output_buffer, 1, sizeof(output_buffer), output_file);
    }

    mbedtls_aes_free(&aes);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
int aes_decrypt_file(const char *input_filename, const char *output_filename, const unsigned char *key) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");

    if (!input_file || !output_file) {
        perror("Error opening file");
        return -1;
    }
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    unsigned char iv[16] = { 0 };

    mbedtls_aes_setkey_dec(&aes, key, KEY_SIZE * 8);

    unsigned char input_buffer[16];
    unsigned char output_buffer[16];

    while (1) {
        size_t bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file);
        if (bytes_read == 0) {
            break; // End of file
        }
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, sizeof(input_buffer), iv, input_buffer, output_buffer);
        size_t padding = output_buffer[sizeof(output_buffer) - 1];
        size_t output_size = sizeof(output_buffer) - padding;

        fwrite(output_buffer, 1, output_size, output_file);
    }
    mbedtls_aes_free(&aes);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
int main() {
    //set_key
    unsigned char key[KEY_SIZE] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };
    //set inputFile, encryptedFile
    const char *input_file = "input.txt";
    const char *encrypted_file = "encrypted.txt";
    const char *decrypted_file = "decrypted.txt";

    if (aes_encrypt_file(input_file, encrypted_file, key) == 0) {
        printf("Encryption successful.\n");
    } else {
        printf("Encryption failed.\n");
    }
    if (aes_decrypt_file(encrypted_file, decrypted_file, key) == 0) {
        printf("Decryption successful.\n");
    } else {
        printf("Decryption failed.\n");
    }

    return 0;
}

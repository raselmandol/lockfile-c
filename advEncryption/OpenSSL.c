#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define KEY_SIZE 32
int aes_encrypt_file(const char *input_filename, const char *output_filename, const unsigned char *key) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");

    if (!input_file || !output_file) {
        perror("Error opening file");
        return -1;
    }
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();

    unsigned char iv[EVP_MAX_IV_LENGTH] = { 0 }; 

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char input_buffer[1024];
    unsigned char output_buffer[1024 + EVP_MAX_BLOCK_LENGTH];

    int bytes_read, bytes_written;
    while ((bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file)) > 0) {
        EVP_EncryptUpdate(ctx, output_buffer, &bytes_written, input_buffer, bytes_read);
        fwrite(output_buffer, 1, bytes_written, output_file);
    }

    EVP_EncryptFinal_ex(ctx, output_buffer, &bytes_written);
    fwrite(output_buffer, 1, bytes_written, output_file);

    EVP_CIPHER_CTX_free(ctx);
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

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    unsigned char iv[EVP_MAX_IV_LENGTH] = { 0 };
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char input_buffer[1024 + EVP_MAX_BLOCK_LENGTH];
    unsigned char output_buffer[1024];

    int bytes_read, bytes_written;
    while ((bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file)) > 0) {
        EVP_DecryptUpdate(ctx, output_buffer, &bytes_written, input_buffer, bytes_read);
        fwrite(output_buffer, 1, bytes_written, output_file);
    }

    EVP_DecryptFinal_ex(ctx, output_buffer, &bytes_written);
    fwrite(output_buffer, 1, bytes_written, output_file);
    EVP_CIPHER_CTX_free(ctx);
    fclose(input_file);
    fclose(output_file);
    return 0;
}

int main() {
    unsigned char key[KEY_SIZE] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };

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

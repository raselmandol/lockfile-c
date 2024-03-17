#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>

//define_key_size
#define KEY_SIZE 32
#define NONCE_SIZE crypto_aead_aes256gcm_NPUBBYTES
int aes_encrypt_file(const char *input_filename, const char *output_filename, const unsigned char *key) {
    FILE *input_file = fopen(input_filename, "rb");
    FILE *output_file = fopen(output_filename, "wb");

    if (!input_file || !output_file) {
        perror("Error opening file");
        return -1;
    }
    unsigned char nonce[NONCE_SIZE];
    randombytes_buf(nonce, NONCE_SIZE);

    unsigned char header[NONCE_SIZE];
    memcpy(header, nonce, NONCE_SIZE);
    fwrite(header, 1, NONCE_SIZE, output_file);
    unsigned char input_buffer[1024];
    unsigned char output_buffer[1024 + crypto_aead_aes256gcm_ABYTES];

    while (1) {
        size_t bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file);
        if (bytes_read == 0) {
            break;
        }
        crypto_aead_aes256gcm_encrypt(output_buffer, NULL, input_buffer, bytes_read, NULL, 0, NULL, nonce, key);

        fwrite(output_buffer, 1, bytes_read + crypto_aead_aes256gcm_ABYTES, output_file);
    }

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
    unsigned char header[NONCE_SIZE];
    fread(header, 1, NONCE_SIZE, input_file);
    unsigned char input_buffer[1024 + crypto_aead_aes256gcm_ABYTES];
    unsigned char output_buffer[1024];
    while (1) {
        size_t bytes_read = fread(input_buffer, 1, sizeof(input_buffer), input_file);
        if (bytes_read == 0) {
            break;
        }

        size_t output_len;
        if (crypto_aead_aes256gcm_decrypt(output_buffer, &output_len, NULL, input_buffer, bytes_read, NULL, 0, header, key) != 0) {
            printf("Decryption failed.\n");
            fclose(input_file);
            fclose(output_file);
            return -1;
        }
        fwrite(output_buffer, 1, output_len, output_file);
    }
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
    //set inputFile_location, encrypted file
    const char *input_file = "input.txt";
    const char *encrypted_file = "encrypted.txt";
    const char *decrypted_file = "decrypted.txt";

    if (sodium_init() == -1) {
        printf("Error initializing libsodium.\n");
        return -1;
    }

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

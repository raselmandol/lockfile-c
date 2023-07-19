#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define MAX_FILENAME_LENGTH 100

int main() {
    const char* inputFileName = "input.txt";
    const char* encryptedFileName = "encrypted.txt";
    FILE* publicKeyFile = fopen("public_key.pem", "rb");
    if (publicKeyFile == NULL) {
        printf("Error opening public key file.\n");
        return 1;
    }
    RSA* rsa = PEM_read_RSA_PUBKEY(publicKeyFile, NULL, NULL, NULL);
    fclose(publicKeyFile);
    if (rsa == NULL) {
        printf("Error reading public key.\n");
        return 1;
    }
    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        printf("Error opening file: %s\n", inputFileName);
        RSA_free(rsa);
        return 1;
    }
    FILE* outputFile = fopen(encryptedFileName, "wb");
    if (outputFile == NULL) {
        printf("Error creating file: %s\n", encryptedFileName);
        fclose(inputFile);
        RSA_free(rsa);
        return 1;
    }

    int rsaLen = RSA_size(rsa);
    unsigned char* inputBuffer = (unsigned char*)malloc(rsaLen);
    unsigned char* outputBuffer = (unsigned char*)malloc(rsaLen);

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), rsaLen - 11, inputFile)) > 0) {
        int encryptedLen = RSA_public_encrypt(bytesRead, inputBuffer, outputBuffer, rsa, RSA_PKCS1_PADDING);
        if (encryptedLen == -1) {
            printf("Error encrypting data.\n");
            fclose(inputFile);
            fclose(outputFile);
            RSA_free(rsa);
            free(inputBuffer);
            free(outputBuffer);
            return 1;
        }

        fwrite(outputBuffer, sizeof(unsigned char), encryptedLen, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
    RSA_free(rsa);
    free(inputBuffer);
    free(outputBuffer);

    printf("File encrypted successfully: %s\n", encryptedFileName);

    return 0;
}

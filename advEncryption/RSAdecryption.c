//RSA Decryption 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define MAX_FILENAME_LENGTH 100

int main() {
    const char* encryptedFileName = "encrypted.txt";
    const char* decryptedFileName = "decrypted.txt";
    FILE* privateKeyFile = fopen("private_key.pem", "rb");
    if (privateKeyFile == NULL) {
        printf("Error opening private key file.\n");
        return 1;
    }

    RSA* rsa = PEM_read_RSAPrivateKey(privateKeyFile, NULL, NULL, NULL);
    fclose(privateKeyFile);
    if (rsa == NULL) {
        printf("Error reading private key.\n");
        return 1;
    }
    FILE* encryptedFile = fopen(encryptedFileName, "rb");
    if (encryptedFile == NULL) {
        printf("Error opening file: %s\n", encryptedFileName);
        RSA_free(rsa);
        return 1;
    }
    FILE* decryptedFile = fopen(decryptedFileName, "wb");
    if (decryptedFile == NULL) {
        printf("Error creating file: %s\n", decryptedFileName);
        fclose(encryptedFile);
        RSA_free(rsa);
        return 1;
    }

    int rsaLen = RSA_size(rsa);
    unsigned char* inputBuffer = (unsigned char*)malloc(rsaLen);
    unsigned char* outputBuffer = (unsigned char*)malloc(rsaLen);

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), rsaLen, encryptedFile)) > 0) {
        int decryptedLen = RSA_private_decrypt(bytesRead, inputBuffer, outputBuffer, rsa, RSA_PKCS1_PADDING);
        if (decryptedLen == -1) {
            printf("Error decrypting data.\n");
            fclose(encryptedFile);
            fclose(decryptedFile);
            RSA_free(rsa);
            free(inputBuffer);
            free(outputBuffer);
            return 1;
        }
        fwrite(outputBuffer, sizeof(unsigned char), decryptedLen, decryptedFile);
    }

    fclose(encryptedFile);
    fclose(decryptedFile);
    RSA_free(rsa);
    free(inputBuffer);
    free(outputBuffer);
    printf("File decrypted successfully: %s\n", decryptedFileName);
    return 0;
}

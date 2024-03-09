#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define BUF_SIZE 1024

void handleErrors()
{
    printf("Error occurred in OpenSSL. Exiting.\n");
    exit(1);
}

void encryptFile(const char* inputFileName, const char* outputFileName, const unsigned char* key, const unsigned char* iv)
{
    FILE *inputFile, *outputFile;
    unsigned char inBuffer[BUF_SIZE], outBuffer[BUF_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, outLength;

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        handleErrors();
    }
    //set_input&output_file

    inputFile = fopen(inputFileName, "rb");
    outputFile = fopen(outputFileName, "wb");
    if (!inputFile || !outputFile) {
        printf("Error opening files.\n");
        exit(1);
    }
    while ((bytesRead = fread(inBuffer, 1, sizeof(inBuffer), inputFile)) > 0) {
        if (EVP_EncryptUpdate(ctx, outBuffer, &outLength, inBuffer, bytesRead) != 1) {
            handleErrors();
        }
        fwrite(outBuffer, 1, outLength, outputFile);
    }
    if (EVP_EncryptFinal_ex(ctx, outBuffer, &outLength) != 1) {
        handleErrors();
    }
    fwrite(outBuffer, 1, outLength, outputFile);
    EVP_CIPHER_CTX_free(ctx);
    fclose(inputFile);
    fclose(outputFile);
    printf("Encryption complete. Output file: %s\n", outputFileName);
}

int main()
{
    const char* inputFileName = "input.txt";
    const char* outputFileName = "output.enc";
    unsigned char key[] = "1234567890abcdef1234567890abcdef";
    unsigned char iv[] = "1234567890abcdef"; // 16 bytes for AES
    OpenSSL_add_all_algorithms();
    encryptFile(inputFileName, outputFileName, key, iv);
    EVP_cleanup();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//blowfish.h -->add
#include <openssl/blowfish.h>

#define MAX_FILENAME_LENGTH 100

void decryptFile(const char* inputFileName, const char* outputFileName, const unsigned char* key, int keyLength) {
    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        printf("Error opening file: %s\n", inputFileName);
        return;
    }
    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL) {
        printf("Error creating file: %s\n", outputFileName);
        fclose(inputFile);
        return;
    }
    BF_KEY bfKey;
    BF_set_key(&bfKey, keyLength, key);

    unsigned char inputBuffer[8]; //Blowfish uses a block size of 64 bits (8 bytes)
    unsigned char outputBuffer[8];

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), 8, inputFile)) > 0) {
        BF_ecb_encrypt(inputBuffer, outputBuffer, &bfKey, BF_DECRYPT);
        int padding = outputBuffer[7];
        if (padding < 8) {
            bytesRead -= padding;
        }
        fwrite(outputBuffer, sizeof(unsigned char), bytesRead, outputFile);
    }
    //close files, pointers
    fclose(inputFile);
    fclose(outputFile);

    printf("File decrypted successfully: %s\n", outputFileName);
}

int main() {
    const char* encryptedFileName = "encrypted.bin";
    const char* decryptedFileName = "decrypted.txt";

    //Blowfish uses a variable-length key (up to 448 bits)
    const unsigned char key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int keyLength = 8;
    decryptFile(encryptedFileName, decryptedFileName, key, keyLength);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tf/twofish.h>

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

    TwofishInstance ctx;
    TwofishInitialise(&ctx, key, keyLength);

    unsigned char inputBuffer[16]; // Twofish uses a block size of 128 bits (16 bytes)
    unsigned char outputBuffer[16];

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), 16, inputFile)) > 0) {
        TwofishDecrypt(&ctx, inputBuffer, outputBuffer);
        int padding = outputBuffer[15];
        if (padding < 16) {
            bytesRead -= padding;
        }
        fwrite(outputBuffer, sizeof(unsigned char), bytesRead, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);

    printf("File decrypted successfully: %s\n", outputFileName);
}

int main() {
    const char* encryptedFileName = "encrypted.bin";
    const char* decryptedFileName = "decrypted.txt";
    const unsigned char key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                                  0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    int keyLength = 128 / 8;
    decryptFile(encryptedFileName, decryptedFileName, key, keyLength);
    return 0;
}

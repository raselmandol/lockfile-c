#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tf/twofish.h>

#define MAX_FILENAME_LENGTH 100
void encryptFile(const char* inputFileName, const char* outputFileName, const unsigned char* key, int keyLength) {
    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        printf("Error opening file: %s\n", inputFileName);
        return;
    }

    // Open the output file for writing
    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL) {
        printf("Error creating file: %s\n", outputFileName);
        fclose(inputFile);
        return;
    }
    TwofishInstance ctx;
    TwofishInitialise(&ctx, key, keyLength);

    unsigned char inputBuffer[16];
    unsigned char outputBuffer[16];

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), 16, inputFile)) > 0) {
        int padding = 16 - bytesRead;
        while (padding > 0) {
            inputBuffer[bytesRead] = (unsigned char)padding;
            bytesRead++;
            padding--;
        }
        TwofishEncrypt(&ctx, inputBuffer, outputBuffer);
        fwrite(outputBuffer, sizeof(unsigned char), 16, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);

    printf("File encrypted successfully: %s\n", outputFileName);
}

int main() {
    const char* inputFileName = "input.txt";
    const char* encryptedFileName = "encrypted.bin";

    //Twofish uses a fixed-length key (128, 192, or 256 bits)
    const unsigned char key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                                  0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    int keyLength = 128 / 8;
    encryptFile(inputFileName, encryptedFileName, key, keyLength);

    return 0;
}

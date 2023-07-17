#include <stdio.h>
#include <stdlib.h>

void encryptFile(const char* inputFileName, const char* outputFileName, unsigned char key) {
    //Opening the input file for reading
    FILE* inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        printf("Error opening file: %s\n", inputFileName);
        return;
    }

    //Opening the output file for writing
    FILE* outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL) {
        printf("Error creating file: %s\n", outputFileName);
        fclose(inputFile);
        return;
    }

    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        //Performing XOR encryption
        unsigned char encryptedByte = ch ^ key;
        fputc(encryptedByte, outputFile);
    }

    //Closing the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File encrypted successfully: %s\n", outputFileName);
}

void decryptFile(const char* inputFileName, const char* outputFileName, unsigned char key) {
    //Calling the encryption function with the same key
    encryptFile(inputFileName, outputFileName, key);

    printf("File decrypted successfully: %s\n", outputFileName);
}

int main() {
    const char* inputFileName = "input.txt";
    const char* encryptedFileName = "encrypted.txt";
    const char* decryptedFileName = "decrypted.txt";
    unsigned char key = 0xAB;  // Example key value

    //Encrypting the file
    encryptFile(inputFileName, encryptedFileName, key);

    //Decrypting the file
    decryptFile(encryptedFileName, decryptedFileName, key);

    return 0;
}

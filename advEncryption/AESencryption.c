//AES Encryption 
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>

//AES key Size
#define AES_KEY_SIZE 256

void encryptFile(const char* inputFileName, const char* outputFileName, const unsigned char* key) {
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

    //ing the AES encryption context
    AES_KEY aesKey;
    AES_set_encrypt_key(key, AES_KEY_SIZE, &aesKey);

    unsigned char inputBuffer[AES_BLOCK_SIZE];
    unsigned char outputBuffer[AES_BLOCK_SIZE];

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), AES_BLOCK_SIZE, inputFile)) > 0) {
        //Padding the input buffer if needed
        int padding = AES_BLOCK_SIZE - bytesRead;
        while (padding > 0) {
            inputBuffer[bytesRead] = (unsigned char)padding;
            bytesRead++;
            padding--;
        }

        //Encrypting the input buffer
        AES_encrypt(inputBuffer, outputBuffer, &aesKey);

        //Writing the encrypted data to the output file
        fwrite(outputBuffer, sizeof(unsigned char), AES_BLOCK_SIZE, outputFile);
    }

    //Closing the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File encrypted successfully: %s\n", outputFileName);
}

void decryptFile(const char* inputFileName, const char* outputFileName, const unsigned char* key) {
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

    //Initializing the AES decryption context
    AES_KEY aesKey;
    AES_set_decrypt_key(key, AES_KEY_SIZE, &aesKey);

    unsigned char inputBuffer[AES_BLOCK_SIZE];
    unsigned char outputBuffer[AES_BLOCK_SIZE];

    int bytesRead;
    while ((bytesRead = fread(inputBuffer, sizeof(unsigned char), AES_BLOCK_SIZE, inputFile)) > 0) {
        //Decrypting the input buffer
        AES_decrypt(inputBuffer, outputBuffer, &aesKey);

        //Removing padding from the last block if needed
        int padding = outputBuffer[AES_BLOCK_SIZE - 1];
        if (padding < AES_BLOCK_SIZE) {
            bytesRead -= padding;
        }

        //Writing the decrypted data to the output file
        fwrite(outputBuffer, sizeof(unsigned char), bytesRead, outputFile);
    }

    //Closing the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File decrypted successfully: %s\n", outputFileName);
}

int main() {
    const char* inputFileName = "input.txt";
    const char* encryptedFileName = "encrypted.bin";
    const char* decryptedFileName = "decrypted.txt";

    unsigned char key[AES_KEY_SIZE / 8] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
    };

    //Encrypting the file
    encryptFile(inputFileName, encryptedFileName, key);

    //Decrypting the file
    decryptFile(encryptedFileName, decryptedFileName, key);

    return 0;
}

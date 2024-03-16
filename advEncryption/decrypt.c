#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 100

void encryptExtension(char *extension, int key) {
    int i = 0;
    while (extension[i] != '\0') {
        extension[i] = (extension[i] + key) % 128;
        i++;
    }
}

void decryptExtension(char *extension, int key) {
    int i = 0;
    while (extension[i] != '\0') {
        extension[i] = (extension[i] - key + 128) % 128;
        i++;
    }
}

void encryptFile(char *filename, int key) {
    //Open the input file for reading
    FILE *inputFile = fopen(filename, "rb");
    if (inputFile == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    //Create the output file with encrypted extension
    char encryptedFilename[MAX_FILENAME_LENGTH];
    strcpy(encryptedFilename, filename);
    encryptExtension(strchr(encryptedFilename, '.') + 1, key);

    FILE *outputFile = fopen(encryptedFilename, "wb");
    if (outputFile == NULL) {
        printf("Error creating encrypted file: %s\n", encryptedFilename);
        fclose(inputFile);
        return;
    }

    //Read and encrypt each byte from the input file
    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        ch = (ch + key) % 256;
        fputc(ch, outputFile);
    }

    //Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File encrypted successfully: %s\n", encryptedFilename);
}

void decryptFile(char *filename, int key) {
    //Open the input file for reading
    FILE *inputFile = fopen(filename, "rb");
    if (inputFile == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    //Create the output file with decrypted extension
    char decryptedFilename[MAX_FILENAME_LENGTH];
    strcpy(decryptedFilename, filename);
    decryptExtension(strchr(decryptedFilename, '.') + 1, key);

    FILE *outputFile = fopen(decryptedFilename, "wb");
    if (outputFile == NULL) {
        printf("Error creating decrypted file: %s\n", decryptedFilename);
        fclose(inputFile);
        return;
    }

    //Read and decrypt each byte from the input file
    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        ch = (ch - key + 256) % 256;
        fputc(ch, outputFile);
    }

    //Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File decrypted successfully: %s\n", decryptedFilename);
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    int key;
    int choice;

    //Read the filename and encryption key from the user
    printf("Enter the filename: ");
    scanf("%s", filename);

    //key_input
    //save_key
    //key_format_int
    printf("Enter the encryption key: ");
    scanf("%d", &key);

    //Prompt user for encryption or decryption
    printf("Choose an option:\n");
    printf("1. Encrypt\n");
    printf("2. Decrypt\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    //Perform encryption or decryption based on user choice
    if (choice == 1) {
        encryptFile(filename, key);
    } else if (choice == 2) {
        decryptFile(filename, key);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}

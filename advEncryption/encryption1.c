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
        //new_key? 
        //see_the_logic_here
        ch = (ch + key) % 256;
        fputc(ch, outputFile);
    }

    //Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("File encrypted successfully: %s\n", encryptedFilename);
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    int key;

    //Read the filename and encryption key from the user
    printf("Enter the filename: ");
    scanf("%s", filename);

    printf("Enter the encryption key: ");
    scanf("%d", &key);

    //Encrypt the file
    encryptFile(filename, key);

    return 0;
}

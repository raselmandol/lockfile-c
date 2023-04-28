#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_FILENAME_LEN 256
#define ENCRYPTED_EXTENSION ".rasel"
#define DECRYPTED_EXTENSION ".decrypted"

void encrypt_file(char *filename, char *password);
void decrypt_file(char *filename, char *password);

void encrypt_directory(char *directory_path, char *password);
void decrypt_directory(char *directory_path, char *password);

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <encrypt|decrypt> <directory_path> <password>\n", argv[0]);
        exit(1);
    }

    char *action = argv[1];
    char *directory_path = argv[2];
    char *password = argv[3];

    if (strcmp(action, "encrypt") == 0) {
        encrypt_directory(directory_path, password);
        printf("Encryption complete.\n");
    } else if (strcmp(action, "decrypt") == 0) {
        decrypt_directory(directory_path, password);
        printf("Decryption complete.\n");
    } else {
        printf("Error: invalid action %s. Must be \"encrypt\" or \"decrypt\".\n", action);
        exit(1);
    }

    return 0;
}
void encrypt_file(char *filename, char *password) {
    FILE *fp, *fp_out;
    int c, i;
    char out_filename[MAX_FILENAME_LEN];

    //Constructing output filename by appending encrypted extension
    strncpy(out_filename, filename, MAX_FILENAME_LEN - strlen(ENCRYPTED_EXTENSION) - 1);
    strcat(out_filename, ENCRYPTED_EXTENSION);

    //Opening input and output files
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: could not open input file %s.\n", filename);
        exit(1);
    }

    fp_out = fopen(out_filename, "wb");
    if (fp_out == NULL) {
        printf("Error: could not open output file %s.\n", out_filename);
        fclose(fp);
        exit(1);
    }

    //Encrypting file contents by XOR-ing with password
    i = 0;
    while ((c = fgetc(fp)) != EOF) {
        fputc(c ^ password[i], fp_out);
        i = (i + 1) % strlen(password);
    }

    //Closing files
    fclose(fp);
    fclose(fp_out);
}

void decrypt_file(char *filename, char *password) {
    FILE *fp, *fp_out;
    int c, i;
    char out_filename[MAX_FILENAME_LEN];

    //Constructing output filename by appending decrypted extension
    strncpy(out_filename, filename, MAX_FILENAME_LEN - strlen(DECRYPTED_EXTENSION) - 1);
    strcat(out_filename, DECRYPTED_EXTENSION);

    //Opening input and output files
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: could not open input file %s.\n", filename);
        exit(1);
    }

    fp_out = fopen(out_filename, "wb");
    if (fp_out == NULL) {
        printf("Error: could not open output file %s.\n", out_filename);
        fclose(fp);
        exit(1);
    }

    //Decrypting file contents by XOR-ing with password
    i = 0;
    while ((c = fgetc(fp)) != EOF) {
        fputc(c ^ password[i], fp_out);
        i = (i + 1) % strlen(password);
    }

    //Closing files
    fclose(fp);
    fclose(fp_out);
}
void encrypt_directory(char *directory_path, char *password) {
    DIR *dir;
    struct dirent *ent;
    struct stat st;

    //Opening directory and iterate over files
    dir = opendir(directory_path);
    if (dir == NULL) {
        printf("Error: could not open directory %s.\n", directory_path);
        exit(1);
    }

    while ((ent = readdir(dir)) != NULL) {
        char filename[MAX_FILENAME_LEN];
        snprintf(filename, MAX_FILENAME_LEN, "%s/%s", directory_path, ent->d_name);

        //Checking if file is a regular file (not a directory or other special file)
        if (stat(filename, &st) == 0 && S_ISREG(st.st_mode)) {
            encrypt_file(filename, password);
        }
    }

    closedir(dir);
}

void decrypt_directory(char *directory_path, char *password) {
    DIR *dir;
    struct dirent *ent;
    struct stat st;

    //Opening directory and iterate over files
    dir = opendir(directory_path);
    if (dir == NULL) {
        printf("Error: could not open directory %s.\n", directory_path);
        exit(1);
    }

    while ((ent = readdir(dir)) != NULL) {
        char filename[MAX_FILENAME_LEN];
        snprintf(filename, MAX_FILENAME_LEN, "%s/%s", directory_path, ent->d_name);

        //Checking if file is a regular file (not a directory or other special file)
        if (stat(filename, &st) == 0 && S_ISREG(st.st_mode)) {
            decrypt_file(filename, password);
        }
    }

    closedir(dir);
}

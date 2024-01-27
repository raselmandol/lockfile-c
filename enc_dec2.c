#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_FILENAME_LEN 256
#define ENCRYPTED_EXTENSION ".rasel"
#define DECRYPTED_EXTENSION ".decrypted"

void process_file(char *filename, char *password, void (*process_function)(FILE *, FILE *, char *));

void encrypt_file(FILE *input, FILE *output, char *password) {
    int c, i = 0;
    while ((c = fgetc(input)) != EOF) {
        fputc(c ^ password[i], output);
        i = (i + 1) % strlen(password);
    }
}

void decrypt_file(FILE *input, FILE *output, char *password) {

    encrypt_file(input, output, password);  
}

void process_file(char *filename, char *password, void (*process_function)(FILE *, FILE *, char *)) {
    FILE *fp, *fp_out;
    char out_filename[MAX_FILENAME_LEN];

    //constructing output filename based on the process function
    char *extension = (process_function == encrypt_file) ? ENCRYPTED_EXTENSION : DECRYPTED_EXTENSION;
    strncpy(out_filename, filename, MAX_FILENAME_LEN - strlen(extension) - 1);
    strcat(out_filename, extension);

    //opening input and output files
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

    //processing file
    process_function(fp, fp_out, password);

    //closing files
    fclose(fp);
    fclose(fp_out);
}

void process_directory(char *directory_path, char *password, void (*process_function)(FILE *, FILE *, char *)) {
    DIR *dir;
    struct dirent *ent;
    struct stat st;

        //opening directory and iterate over files
    dir = opendir(directory_path);
    if (dir == NULL) {
        printf("Error: could not open directory %s.\n", directory_path);
        exit(1);
    }

    while ((ent = readdir(dir)) != NULL) {
        char filename[MAX_FILENAME_LEN];
        snprintf(filename, MAX_FILENAME_LEN, "%s/%s", directory_path, ent->d_name);

        
        if (stat(filename, &st) == 0 && S_ISREG(st.st_mode)) {
            process_file(filename, password, process_function);
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <encrypt|decrypt> <directory_path> <password>\n", argv[0]);
        exit(1);
    }

    char *action = argv[1];
    char *directory_path = argv[2];
    char *password = argv[3];

    void (*process_function)(FILE *, FILE *, char *);

    if (strcmp(action, "encrypt") == 0) {
        process_function = encrypt_file;
        printf("Encryption complete.\n");
    } else if (strcmp(action, "decrypt") == 0) {
        process_function = decrypt_file;
        printf("Decryption complete.\n");
    } else {
        printf("Error: invalid action %s. Must be \"encrypt\" or \"decrypt\".\n", action);
        exit(1);
    }

    process_directory(directory_path, password, process_function);

    return 0;
}

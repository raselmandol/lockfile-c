#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void move_file(char *source, char *destination) {
    int result = rename(source, destination);
    if (result == -1) {
        perror("Error: could not move file");
    }
}

void search_directory(char *directory_path, char *destination_directory) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory_path);
    if (dir == NULL) {
        perror("Error: could not open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                //recursively searching subdirectories
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", directory_path, entry->d_name);
                search_directory(path, destination_directory);
            }
        } else {
            //checking if file has (.rasel) extension
            char *extension = strrchr(entry->d_name, '.');
            if (extension != NULL && strcmp(extension, ".rasel") == 0) {
                //moving file to destination directory
                char source_path[1024];
                char destination_path[1024];
                snprintf(source_path, sizeof(source_path), "%s/%s", directory_path, entry->d_name);
                snprintf(destination_path, sizeof(destination_path), "%s/%s", destination_directory, entry->d_name);
                move_file(source_path, destination_path);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) {
    //checking if there are enough arguments
    if (argc < 3) {
        printf("Usage: %s <directory_path> <destination_directory>\n", argv[0]);
        return 1;
    }

    char *directory_path = argv[1];
    char *destination_directory = argv[2];

    //creating destination directory if it does not exist
    if (access(destination_directory, F_OK) == -1) {
        mkdir(destination_directory, 0700);
    }

    search_directory(directory_path, destination_directory);

    return 0;
}


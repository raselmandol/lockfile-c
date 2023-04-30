//deleting (.rasel)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void delete_files_with_extension(char *directory_path, char *extension) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory_path);
    if (dir == NULL) {
        perror("Could not open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                //deleting files in subdirectories
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", directory_path, entry->d_name);
                delete_files_with_extension(path, extension);
            }
        } else {
            //checking if file has specified extension
            char *file_extension = strrchr(entry->d_name, '.');
            if (file_extension != NULL && strcmp(file_extension, extension) == 0) {
                char file_path[1024];
                snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);
                if (remove(file_path) == 0) {
                    printf("Deleted file %s\n", file_path);
                } else {
                    perror("Error: could not delete file");
                }
            }
        }
    }

    closedir(dir);
}
//how to use?
/*
int main() {
    char *directory_path = "/path/to/delete/from";
    char *extension = ".rasel";
    delete_files_with_extension(directory_path, extension);
    return 0;
}

*/

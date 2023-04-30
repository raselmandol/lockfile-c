#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int has_encrypted_files(char *directory_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory_path);
    if (dir == NULL) {
        perror("Could not open directory.");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                //searching subdirectories
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", directory_path, entry->d_name);
                if (has_encrypted_files(path)) {
                    closedir(dir);
                    return 1;
                }
            }
        } else {
            //Checking if file has .rasel extension
          //change it to your own
          //you can also use it to find ransomware encrypted files
            char *extension = strrchr(entry->d_name, '.');
            if (extension != NULL && strcmp(extension, ".rasel") == 0) {
                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);
    return 0;
}


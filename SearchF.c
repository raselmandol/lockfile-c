#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int isSuspiciousFile(const char *filename) {
    if (strstr(filename, "malware") != NULL || strstr(filename, ".exe") != NULL) {
        return 1;
    }
    return 0;
}
void searchForMaliciousFiles(const char *dirname) {
    struct dirent *entry;
    DIR *dir = opendir(dirname);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        if (entry->d_type == DT_DIR) {
            searchForMaliciousFiles(path);
        } else if (entry->d_type == DT_REG) {
            if (isSuspiciousFile(entry->d_name)) {
                printf("Suspicious file found: %s\n", path);
            }
        }
    }

    closedir(dir);
}

int main() {
    const char *startDirectory = "/path/to/scan";
    searchForMaliciousFiles(startDirectory);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#define MD5_DIGEST_LENGTH 16

int calculateMD5(const char* fileName, unsigned char* md5Digest) {
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", fileName);
        return 0;
    }
    MD5_CTX md5;
    MD5_Init(&md5);
    unsigned char buffer[1024];
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        MD5_Update(&md5, buffer, bytesRead);
    }
    MD5_Final(md5Digest, &md5);
    fclose(file);
    return 1;
}
int main() {
    //set_input file
    const char* fileName = "file.txt";
    unsigned char md5Digest[MD5_DIGEST_LENGTH];
    if (calculateMD5(fileName, md5Digest)) {
        printf("MD5 Hash: ");
        for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
            printf("%02x", md5Digest[i]);
        }
        printf("\n");
    }
    return 0;
}

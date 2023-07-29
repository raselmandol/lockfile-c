#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define SHA256_DIGEST_LENGTH 32

int calculateSHA256(const char* fileName, unsigned char* sha256Digest) {
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", fileName);
        return 0;
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[1024];
    int bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }

    SHA256_Final(sha256Digest, &sha256);

    fclose(file);
    return 1;
}

int main() {
    const char* fileName = "file.txt";
    unsigned char sha256Digest[SHA256_DIGEST_LENGTH];

    if (calculateSHA256(fileName, sha256Digest)) {
        printf("SHA-256 Hash: ");
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%02x", sha256Digest[i]);
        }
        printf("\n");
    }
    return 0;
}

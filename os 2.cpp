#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 4096

int main() {
    int sourceFile, destFile;
    ssize_t bytesRead, bytesWritten;
    char buffer[BUF_SIZE];

    // Open the source file for reading
    sourceFile = open("source.txt", O_RDONLY);
    if (sourceFile == -1) {
        perror("Error opening source file");
        return 1;
    }

    // Open the destination file for writing

    if (destFile == -1) {
        perror("Error opening destination file");
        close(sourceFile);
        return 1;
    }

    // Copy content from source to destination
    while ((bytesRead = read(sourceFile, buffer, BUF_SIZE)) > 0) {
        bytesWritten = write(destFile, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            perror("Error writing to destination file");
            close(sourceFile);
            close(destFile);
            return 1;
        }
    }

    if (bytesRead == -1) {
        perror("Error reading from source file");
        close(sourceFile);
        close(destFile);
        return 1;
    }

    // Close the file descriptors
    close(sourceFile);
    close(destFile);

    printf("File copied successfully.\n");

    return 0;
}


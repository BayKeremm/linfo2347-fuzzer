#include "info.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char * argv[]){
    FILE * file = fopen( "test.tar", "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Initialize the header
    struct posix_header header;
    memset(&header, 0, sizeof(struct posix_header)); // Initialize header with zeros
    strncpy(header.name, "Kerem", 100);  // Set file name
    strncpy(header.mode, TSUID, 8);      // Set file mode

    // Write the header to the file
    fwrite(&header, sizeof(struct posix_header), 1, file);

    fclose(file);  // Close the file
    return 0;
}

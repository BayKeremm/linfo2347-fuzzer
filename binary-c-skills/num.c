#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *str = "aa11bb22";
    int len = strlen(str);
    FILE * f = fopen("file.txt","r+b");
    
    // Ensure the length is even
    if (len % 2 != 0) {
        printf("Invalid input length\n");
        return 1;
    }
    
    // Iterate through the string two characters at a time
    int j = 0;
    for (int i = 0; i < len; i += 2) {
        // Extract two characters
        char hex[3] = {str[i], str[i+1], '\0'};
        
        // Convert hex string to integer
        int num = strtol(hex, NULL, 16);
        fseek(f,j,SEEK_SET);
        fputc(num,f);
        
        // Print the hexadecimal number
        printf("%02X, %d\n", num, num);
        j++;
    }
    
    fclose(f);
    return 0;
}

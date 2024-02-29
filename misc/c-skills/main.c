#include <stdio.h>
#include <stdarg.h>

void printStrings(int num_strings, ...) {
    va_list args;       // Declaring a variable to hold the arguments
    int i;
    
    va_start(args, num_strings);    // Initializing the va_list with the last named argument
    
    // Loop through each string argument
    for (i = 0; i < num_strings; ++i) {
        char *str = va_arg(args, char*);   // Access each string argument
        printf("%s\n", str);
    }
    
    va_end(args);   // Clean up the va_list
}

int main() {
    // Call the function with a variable number of strings
    printStrings(3, "Hello", "World", "How are you?");
    printStrings(2, "Hello", "World");
    
    return 0;
}

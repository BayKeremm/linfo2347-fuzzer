#include <stdio.h>
void main(){
    FILE * f = fopen("./files/file.txt","r+b");
    fseek(f,0,SEEK_SET);
    fputc(0x11,f);
    fclose(f);
}
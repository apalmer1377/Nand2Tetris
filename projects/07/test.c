#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char * t = (char*) malloc(100*sizeof(char));
    int i;
    for (i=0;i<10;i++)
        t[i] = 'a';
    t[10] = '\0';
    printf("%s\t%i\n",t,strlen(t));
    for (i=0;i<10;i++)
        t[10+i] = 'b';
    t[20] = '\0';
    printf("%s\t%i\n",t,strlen(t));
    strcat(t,"cccccccccc");
    t[strlen(t)] = '\0';
    printf("%s\t%i\n",t,strlen(t));

    free(t);

}

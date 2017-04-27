#include "jack_compiler.h"

int main(int argc, char **argv) {
    char * fname = argv[1];
    printf("%s\n",fname);
    parseFile(fname);
    return 0;
}

void parseFile(char *file) {
    FILE * f = fopen(file,"r");
    if (f == NULL)
        exit(1);

    char * token = malloc(MAX_LEN);
    char c;
    int i = 0;
    struct classDec ** newClass = (struct classDec **) malloc(sizeof(struct classDec *));
    char * xml = (char *) malloc(INT_MAX);
    while ((c = parseToken(f , token)) != EOF) {

        if (strcmp(token,"class") == 0) {
            parseClass(f,token,newClass+i);
            newClass[i]->next = NULL;
            if (i > 0)
                newClass[i-1]->next = newClass[i];
            i++;
        }

    }

    struct classDec * temp = newClass[0];
    while (temp != NULL) {
        outputClass(xml,temp);
        printf("%s\n",xml);
        temp = temp->next;
    }
    printf("%s\n",xml);
    return;
}


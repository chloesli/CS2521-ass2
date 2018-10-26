#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

char *strdup(char *s) {
    char *d = malloc (strlen (s) + 1);
    if (d == NULL) {
       return NULL;
    }
    strcpy (d,s);
    return d;
}

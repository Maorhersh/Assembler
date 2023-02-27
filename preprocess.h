#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE 256
#define MAX_LINE 82

typedef struct node *ptr;
typedef struct node {
	char name[MAX_LINE];
	char* data ;
	ptr next;
}macro;

void addtomacrolist( ptr* , char[] );

void freemacrolist( ptr* );

/*void printlist( ptr );*/

ptr findmacro( ptr , char[] );

FILE* openas( FILE* , char * );

FILE* openam( FILE* , char * );

void preprocess( FILE** , FILE** , char* );

void checkmacro( char [] );


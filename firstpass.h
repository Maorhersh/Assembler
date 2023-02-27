#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"

#define MAX_LINE 82
#define MAX_LABEL 32
#define MAX_NUM 13

typedef struct node2 *ptr2;
typedef struct node2 {
	char name[MAX_LABEL];
	char ext;
	char ent;
	int line;
	char data;
	int* locations;
	int counter;
	ptr2 next;
}label;

void firstpass( FILE * , int*, int* , char** , char** , ptr2* );

void buildarray( char**  , char** );

int islabel( char [] );

void addtolabellist( ptr2* , char[] );

void freelabellist( ptr2* );

int findlabel( ptr2 , char[] );

int instordata( char[] );

int checkinst( char[] /*, int **/);

int methodparam( char[] , int* , int* );

void putinstruction( char ** , int  , int , int , int* , int , int );

void printlist( ptr2  );



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"
#include "firstpass.h"

void secondpass( FILE * , int* , int* , char**  , char**  , ptr2* , char* );

void entrylabel( ptr2*  , char[]  );

int methodparam2 ( char [] , int*  , char [] , char [] , int* );

void insert( int , char [] , char** , int , int , ptr2* , int );

void insertconst( char [] , char**  , int );

void insertaddress( char [] , char**  , int  , ptr2* , int );

void insertreg( char [] , char** , int , int);

ptr2 getlabel( ptr2*  , char [] );

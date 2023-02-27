#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE 82


void getword( char [] , char [] , int*  );

void getnum( char [] , char [] , int*  );

void getcomma( char [] , int*  );

void getstr( char [] , int* , int* , char** , int* );

void insertdata( char  , char *** , int**);

void insertnum( int , char** , int* , int*);

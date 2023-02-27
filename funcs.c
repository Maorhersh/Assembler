#include "funcs.h"

void getword( char line[] , char word[] , int* curr ){

	int i = 0;
	/* what ahooens if its over 80 ? */
	while( line[*curr] == ' ' || line[*curr] == '\t' || line[*curr] == ','){/*skip white chars */
	
		(*curr)++;/*uptate line index*/
	
	}
	
	while( *curr < MAX_LINE && line[*curr] != ' ' && line[*curr] != '\t' && line[*curr] != '\n' && line[*curr] != ',' ){
	
		word[i] = line[*curr];/*as long we are in the current word we insert to the array the current word characters*/
		i++;
		(*curr)++;
	
	}
	(*curr)++; 
	word[i] = '\0';/* end of the array*/


}

void getnum( char line[] , char num[] , int* curr ){

	
	int i = 0;
	
	while( line[*curr] == ' ' || line[*curr] == '\t'){ /*skip white chars*/
	
		(*curr)++;
	
	}
	
	if( isdigit( line[*curr])  || line[*curr] == '+' || line[*curr] == '-' ){ /*if its a number */
	
		while( *curr < MAX_LINE  && line[*curr] != ' ' && line[*curr] != '\t' && line[*curr] != '\n' && line[*curr] != ','){
	
			if( line[*curr] != '+' ){ /*as long as the number doesnt end we insert the number digit after digit if the number has + symbol at the beggining we dont add it*/
				num[i] = line[*curr];
				i++;
			}
			(*curr)++;
	
		}
	}
	num[i] = '\0';



}


void getcomma( char line[] , int* curr ){

	while( line[*curr] == ' ' || line[*curr] == '\t'){/* skipping white chars before the coma*/
	
		(*curr)++;
	
	}
	
	if( line[*curr] == ','){ /*getting the coma*/
		(*curr)++;
	}	
	
	while( line[*curr] == ' ' || line[*curr] == '\t'){ /*skiping white chars after the coma*/
	
		(*curr)++;
	
	}

}


void getstr( char line[] , int* curr , int* counter , char** p , int *dc){

	char* q;
	char ch;

	
	while( line[*curr] == ' ' || line[*curr] == '\t'){ /*skipping white chars*/
	
		(*curr)++;
	
	}
	
	if( line[*curr] == '"'){ /*is we are at the begging of a string*/
	
		(*curr)++;
		while ( line[*curr] != '"'){ /*stop the loop when we reach the end of the string*/
			ch = line[*curr]; /*getting current char from the string*/
		
			(*dc)++; /*increament of data counter*/
			q = ( char * ) realloc ( *p , ( *dc  ) * 14   ); /*reallocate new memory in the data array*/
			*p = q;
			insertdata( ch ,  &p , &counter); /*insert char to the data array after the memory allocation*/
/*צריך לתת שגיאה כאשר נגמר הזיכרון */
	
			(*curr)++;
		}
		
		(*dc)++; /* increament for : '\0' */
	q = ( char * ) realloc ( *p , ( *dc  ) * 14   ); /* memory allocation to :  '\0' */
	*p = q;
	ch = 0 ; 
	insertdata( ch ,  &p , &counter); /* inserting char to the data array*/

	
	}
	
}

void insertdata( char ch , char ***p , int **counter){

	char mask = 1;
	int i;
	char bin[14]; /*array that contains the current ascii value of the char*/
	for( i = 0 ; i < 6 ; i++){ /*six letf bits will always be zeos because we represent char with 8 bits */
		bin[i] = 0 ;
	}
	for(i = 7 ; i>=0 ;i-- ){ /* insert digits by masking */
		mask = mask << i;
		mask = mask & ch ;
		mask = mask >> i;
		bin [ 13 - i] = mask ;
		mask =1;
	}

	for( i = 0; i<14 ;  i++ ){
	
		*( **p + **counter) = bin[i]; /* insert bits to the dinamic array that has been created before*/
		(**counter)++; /* increament the counter according to the amoung of chars that had been inserted*/
	}
	
	
}

void insertnum(int num , char **p , int* counter , int* dc){

	unsigned int mask = 1;
	int i;
	
	char* q = NULL;
	q = ( char * ) realloc ( *p , ( *dc  ) * 14   ); /* reallocation for a number in the data array*/
	*p = q;
	
	
	for(i = 13 ; i >= 0 ;i-- ){ /* insert bits to the data array by masking*/
		mask = mask << i;
		mask = mask & num ;
		mask = mask >> i;
		*( *p + *counter) = mask;
		(*counter)++;
		mask = 1;
		
	}
	

}

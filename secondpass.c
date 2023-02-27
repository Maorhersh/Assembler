#include "secondpass.h"


void secondpass( FILE * fd1 , int* ic , int* dc , char** inst , char** data , ptr2* hptr , char* name ){

	
	int numline = 0 , iccopy =0 , curr , flag   , type , oper1 , oper2  , param1 , param2 , reg , i;
	char line[MAX_LINE];
	char label[MAX_LABEL];
	char word[MAX_LINE];
	char word2[MAX_LINE];
	char param1word[MAX_LINE];
	char param2word[MAX_LINE];
	fpos_t pos;


	rewind( fd1 );
	fgetpos( fd1 , &pos );
	
	while( !feof( fd1 ) ){
	
		curr = 0;
		param1 = 0;
		param2 = 0;
		oper1 = 0 ;
		oper2 = 0;
		reg = 0 ;	
	
		fsetpos( fd1 , &pos );
		fgets( line , MAX_LINE , fd1 );
		numline++;
		getword( line , label , &curr );
		
		if( !strlen( label ) || label[0] == ';'){
		
			fgetpos( fd1 , &pos );
			fgetc( fd1 );
			continue;
			
		}
		
		flag = islabel( label );
		
		if( flag == 1 ){
			getword( line , word , &curr );
			
			type = instordata( word );
		}
		
		if ( !flag ){
		
			strcpy( word , label );
			
		}	
		
		type = instordata( word );
		
		if( type ){
		

			iccopy++;
			oper1 = methodparam2( line , &curr , word , param1word , &param1 );
			
			if( oper1 == -1){
				
				fgetpos( fd1 , &pos );
				fgetc( fd1 );
				continue;
			
			}
			
			if( oper1 != 2 && oper1 != -1 ){
				
				iccopy++;
				strcpy(word2 , word);			
				oper2 = methodparam2( line , &curr , word , param1word , &param1 );
				if( oper2 == -1){
				
					reg = 1;
				}
				
				insert( oper1 , word2 , inst , reg , iccopy , hptr , *ic );
				
				if( oper2 != -1 && ( oper1 != 3 || oper2 != 3)){
				
					reg = 1;
					iccopy++;
					insert( oper2 , word , inst , reg , iccopy , hptr , *ic);

				}
				
				else if( (oper1 == 3 && oper2 == 3) ){
				
					reg = 1;
					insert( oper2 , word , inst , reg , iccopy , hptr , *ic );
				}	
					
			}
			
			else if( oper1 == 2 ){
			
				iccopy++;
				insert( oper1 , word ,  inst , reg , iccopy , hptr , *ic);
				iccopy++;
				insert( param1 , param1word , inst , reg , iccopy , hptr , *ic);	
				getword( line , param2word , &curr );
				param2word[ strlen(param2word) -1 ] = '\0';
				
				if( param1word[0]  == 'r' && param1word[1] >= 48 && param1word[1] <= 55 && param1word[2] == '\0' && param2word[0]  == 'r' && param2word[1] >= 48 && param2word[1] <= 55 && param2word[2] == '\0' ){
				
				param2 = 3;
				reg = 1;
				insert( param2 , param2word , inst , reg , iccopy , hptr , *ic);		
				
				
				}
				
				else{
				
					iccopy++;
					if( param2word[0]   == '#'){
						param2 = 0;
						insert( param2 , param2word , inst , reg , iccopy , hptr , *ic);	
					}	
					else if( param2word[0]  == 'r' && param2word[1] >= 48 && param2word[1] <= 55 && param2word[2] == '\0'){
				
						param2 = 3;
						reg = 1;
						insert( param2 , param2word , inst , reg , iccopy , hptr , *ic);	
					}
					
					else{
						param2 = 1;	
						insert( param2 , param2word , inst , reg , iccopy , hptr , *ic);	
					}
					
				
				}
			
				
			}
			
			
			
			
		}
		
		
	
		
		else if( !strcmp( word , ".entry") ){
			
				getword( line , word , &curr );
				entrylabel( hptr , word );
				
		}
		
		fgetpos( fd1 , &pos );
		fgetc( fd1 );
		
	}
	for(i = 0 ; i < (( *ic ) * 14 ) ; i++){
		fprintf( stderr , " %d " ,  *( *inst + i )  );
	}		
	for(i = 0 ; i < (( *dc ) * 14 ) ; i++){
		fprintf( stderr , " %d " ,  *( *data + i )  );
	}		
	fprintf( stderr , "\n\n ic == %d \n\n dc == %d\n\n" , *ic , *dc );
	
}	


void entrylabel( ptr2* hptr , char name1[] ){

	/*ptr2 s = *hptr ;*/
	ptr2 t = *hptr ;
	
	
	while( t ){ 
		if( !strcmp ( t -> name , name1 ) ){
		
			t-> ent = 1;
			/**hptr  = s;*/
			return ; 
		}
		t = t -> next;	
		
	}
	/**hptr  = s;*/
	return ;

}

int methodparam2( char line[] , int* curr , char word[] , char param1word[] , int* param1 ){

	int i , j , tmp;
	
	getword( line , word , curr );
	
	if( word[0] == '#')
		return 0;
		
	if( word [0] == 'r'  && word[1] >= 48 && word[1] <= 55  && word[2] == '\0')
		return 3;
			
	if( isalpha ( word[0] ) ){
	
		for( i = 0 ; word[i] != '\0' ; i++ ){
		
			if( word [i] == '('){
				if( word [i +1 ] == '#' ){
					*param1 = 0 ;
				}	
				else if( word [i +1 ] == 'r'  && word [i +2 ] >= 48 && word[i+2] <= 55 &&  word[i+3] == '\0'){
				
					*param1 = 3;
				}
				else
					*param1 = 1;		
				
				for( i = 0 ; word[i] != '\0' ; i++ ){
				
					if( word [i] == '('){
						tmp = i ;
						for( j=0 ; word[i+1] != '\0'  ; j++ , i++)
							param1word[j] = word[i+1] ;
					}
					
				}
				param1word[j] = '\0';
				word[tmp] = '\0';
				return 2;
			}
			
		} 
		return 1;
	
	
	}
	
	return -1;


}

void insert( int method , char data[] , char** inst , int reg , int ic , ptr2* hptr , int ic2){

	if( method == 0){
	
		insertconst( data , inst , ic );
	}
	
	else if( method == 1 || method == 2 ){
	
		insertaddress(  data , inst , ic , hptr , ic2 );
	
	}
	
	else if( method == 3 ){
	
		insertreg( data , inst , ic , reg );
	
	
	}


}

void insertconst( char data[] , char** inst , int ic ){

	int i , j = 0 , number , counter = 0 ;
	char num[MAX_LINE];
	unsigned int mask = 1;
	
	for( i = 0 ; data[i] != '\0' ; i++ ){
		
		if( isdigit ( data[i] ) || data[i] == '-' || data[i] == '+'){
			num[j] = data[i];
			j++;
		}
	
	}
	num[j] = '\0';
	number = atoi( num );
	
	for(i = 11 ; i >= 0 ;i-- ){
		mask = mask << i;
		mask = mask & number ;
		mask = mask >> i;
		*( *inst + (ic-1) * 14 + counter) = mask;
		counter++;
		mask = 1;
		
	}
	

}

void insertaddress( char data[] , char** inst , int ic , ptr2* hptr  , int ic2){
	
	int address , i , counter = 0 ;
	int* p;
	unsigned int mask = 1;
	ptr2 t;
	t = getlabel( hptr , data );
	
	
	if( !t ){
		fprintf( stderr , "error");
		exit(0);
	}
	
	address = (t -> line) + 99 ;
		if( t -> data ){
			address += ic2  ;
		
		}
		
	if ( ( t -> ext ) == 1){
	
		(t -> counter)++;
		p =  ( int * ) realloc ( t -> locations , t -> counter * sizeof(int)  );
		if(!p){
			fprintf(stderr , "error");
			exit(0);
		
		}
		t -> locations = p;
		*((t -> locations) + (t->counter)) = ic ;
		*( *inst + (ic-1) * 14 + 13) = 1;
	}
	
	else{
	
		
		for(i = 11 ; i >= 0 ;i-- ){
			mask = mask << i;
			mask = mask & address ;
			mask = mask >> i;
			*( *inst + (ic-1) * 14 + counter) = mask;
			counter++;
			mask = 1;
		
		}
		*( *inst + (ic-1) * 14 + 12) = 1;
		
	}
}

ptr2 getlabel( ptr2* hptr , char name1[] ){

	ptr2 t = *hptr ;
	/*ptr2 s = *hptr ;*/

	while( t && strcmp ( (t -> name) , name1 ) ){
	
		t = ( t -> next );
		
	}
	/**hptr = s;*/
	return t;

}

void insertreg( char data[] , char** inst , int ic , int reg ){

	int num , i , counter = 0 ;
	unsigned int mask = 1;
	
	num = data[1] - 48 ;
	
	if( reg == 0 ){
	
		for(i = 5 ; i >= 0 ;i-- ){
		mask = mask << i;
		mask = mask & num ;
		mask = mask >> i;
		*( *inst + (ic-1) * 14 + counter) = mask;
		counter++;
		mask = 1;
		
		}
	
	
	
	}
	
	else{
	
		counter = 6;
		
		for(i = 5 ; i >= 0 ;i-- ){
		mask = mask << i;
		mask = mask & num ;
		mask = mask >> i;
		*( *inst + (ic-1) * 14 + counter) = mask;
		counter++;
		mask = 1;
		
		}
	
	
	}
	
	

}
		
		

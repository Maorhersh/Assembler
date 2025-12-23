#include "firstpass.h"
#include "funcs.h"


void firstpass( FILE * fd1 , int* ic , int* dc , char** inst , char** data , ptr2* hptr ){

	int numline = 0 , curr , flag , names , counter = 0 , type , i , num2 ,  oper1 , oper2 , instnum , add , param1 , param2;
	char line[MAX_LINE];
	char label[MAX_LABEL];
	char word[MAX_LINE];
	char num[MAX_NUM];
	fpos_t pos;
	/*ptr2 hptr = NULL;*/
	char* q = NULL;
	
	buildarray( inst , data );  /* הקצאת מקום ל2 המערכים*/
	
	rewind( fd1 ); /*.am חזרה לתחילת הקובץ  */
	fgetpos( fd1 , &pos );
	fgetc( fd1 );

	while( !feof( fd1 ) ){
	
		curr = 0;
		add = 0;
		param1 = 0;
		param2 = 0;
		oper1 = 0 ;
		oper2 = 0;	
		
		fsetpos( fd1 , &pos );
		fgets( line , MAX_LINE , fd1 );  /*line קליטת שורה למערך  */
		numline++; /*סופר את מספר השורה בקובץ כדי לדווח על תקלה בשורה */
		getword( line , label , &curr ); /*label קולט מילה לפי המיקום הנוכחי בשורה לתוך המערך */
		
		if( !strlen( label ) || label[0] == ';'){ /* אנחנו בשורה ריקה או בשורת הערה*/
		
			fgetpos( fd1 , &pos ); /*דלג לשורה הבאה*/
			fgetc( fd1 );
			continue;
			
		}
			
		flag = islabel( label ); /* .בודקת אם המילה הראשונה בשורה היא תווית , אם כן מחזירה 1 , אם תווית שגויה מחזירה 2 , אם לא תווית מחזירה 0 , הפונקציה בתחתית הקובץ*/
		
		if( flag == 2 ){/* התווית שגויה */
		
			fprintf( stderr , "illegal label at line %d \n\n" , numline );
			fgetpos( fd1 , &pos );/*דלג לשורה הבאה*/
			fgetc( fd1 );
			continue;
			
		}
		
		
		
		
		if( flag == 1 ){ /*יש תווית */
		
			names = findlabel( *hptr , label ); /* חיפוש אם כבר יש תווית בשם זה*/
			
			if( !names ){ /*סימן שיש כבר תווית בשם זה null אם המשתנה אינו */
			
				fprintf( stderr , "2 labels with the same name at line %d \n\n" , numline );
				fgetpos( fd1 , &pos );/*דלג לשורה הבאה*/
				fgetc( fd1 );
				continue;
			
			}
			
			addtolabellist( hptr , label );/* הוסף את התווית החדשה לרשימת התוויות. הפונקציה בתחתית הקובץ*/
			
			getword( line , word , &curr ); /*קליטת המילה הבאה מהשורה*/
			
			type = instordata( word ); /* בדיקה אם זוהי שורת פעולה או מידע*/
			
			if( type ){ /* התווית היא תווית של שורת פקודה*/
			
				(*hptr) -> line = *ic + 1 ; /* עדכון כתובת התווית לפי כמות שורות הפקודה שהיו עד כה*/
				(*hptr) -> data = 0 ; /* סימן שהתווית אינה תווית של שורת מידע*/
			}
			
			else{
			
				(*hptr) -> line = *dc + 1 ; /* עדכון כתובת התווית לפי כמות שורות המידע שהיו עד כה*/
				(*hptr) -> data = 1 ; /* סימן שהתווית היא תווית של שורת מידע*/
			}
			
		
		}
		
		if ( !flag ){ /*לא היה לנו תווית*/
		
			strcpy( word , label ); /*word העתקת המילה הראשונה למערך */
			
		}	
		
		type = instordata( word ); /* בדיקה אם זוהי שורת פעולה או מידע*/
		
		if( type ){ /* זוהי שורת פקודה */
		
			fprintf( stderr , "\n\n"  );
			instnum = checkinst( word /*, &inst */);
			(*ic)++;
			oper1 = methodparam( line , &curr , &param1 );
			fprintf( stdout , "\nmethod param1	%d\n\n" , oper1 ); 
			
			/*if( oper1 != -1 )
				add++;*/
				
			q = ( char * ) realloc ( *inst , ( *ic  ) * 14   );
			if( !q ){
				printf( "allocated failed\n\n" );
				exit(0);
			}
			*inst = q ;
			
			if( oper1 != 2 && oper1 != -1 ){
			
				oper2 = methodparam( line , &curr , &param1 );
				
				fprintf( stdout , "method param2	%d\n\n" , oper2 ); 
				
				if( oper2 != -1 && ( oper1 != 3 || oper2 != 3)){
					add += 2;

				}
				
				else if( (oper1 == 3 && oper2 == 3) ||  oper2 == -1 )
					add++;
					
					
			}
			
			else if( oper1 == 2){
			
				oper2 = oper1;
				oper1 = 0;
				
				
				if( line[curr - 3] == 'r' && line[curr - 2] >= 48 && line[curr - 2] <= 55 && line[curr]  == 'r' && line[curr +1] >= 48 && line[curr +1] <= 55 && line[curr+2] == ')' ){
				
				param2 = 3;
				add += 2;
				
				}
				
				else{
					if( line[curr]  == '#')
						param2 = 0;
					else if( line[curr]  == 'r' && line[curr +1] >= 48 && line[curr +1] <= 55 && line[curr+2] == ')'){
				
						param2 = 3;
					}
					else
						param2 = 1;	
				
			
					add += 3 ;
				}
			
				
			}
			
			
			if( oper2 == -1){
			
				oper2 = oper1;
				oper1 = 0;
				
			}
				
			fprintf(stdout , "%d\t%d\t%d\t%d\t%d\t\n\n" , param1 , param2 , instnum , oper1 , oper2 );/*הדפסה לשם נוחות */
			putinstruction( inst , instnum , oper1 , oper2 , ic , param1 , param2 );
			*ic += add ;
			
			q = ( char * ) realloc ( *inst , ( *ic  ) * 14   );
			if( !q ){
				printf( "allocated failed\n\n" );
				exit(0);
			}
			*inst = q ;
			
			for( i = 0 ; i < add*14 ; i++){
			
			*( *inst + ( *ic - add ) *14 + i )= 0;
			
			}
			
		}
		
		
		else{ /* זוהי שורת מידע */
		
			if( !strcmp( word , ".data") ){ /* התחלת הקצאת משתנים ספרתיים*/
			
				while( line[curr] != '\n'){ /* כל עוד לא סיימנו את השורה */
					getnum( line , num , &curr ); /* .func.c עדכון המיקום הנוכחי בשורה.הפונקציה נמצאת בקובץ  + num קליטת מספר מהשורה למערך */
			
					num2 = atoi (num);/* intהפיכת המספר במערך ל*/
									
					(*dc)++; /*  */
					insertnum( num2 , data , &counter , dc ); /* func.c הכנסת המספר למערך המידע, הפונקציה נמצאת בקובץ */
					getcomma( line , &curr ); /*  ,func.c קליטת פסיק שמפריד בין מספר למספר אם קיים, הפונקציה נמצאת בקובץ */
				}	
				
			}
			
			else if(!strcmp( word , ".string")){ /* התחלת הקצאת מחרוזת*/
			
				getstr( line , &curr , &counter , data ,dc); /*func.c הכנסת התווים למערך המידע+תו 0 בסוף. הפונקציה נמצאת בקובץ */
		
			} 
			
			else if( !strcmp( word , ".extern") ){ /* הצהרה על תווית חיצונית */
			
				getword( line , word , &curr ); /*( קליטת המילה הבאה(שם התווית*/
				names = findlabel(  *hptr , word );/* בדיקה אם קיימת תוית בקובץ זה עם השם הזה*/
				if( !names ){
			
					fprintf( stderr , "extern label defined in the file at line %d \n\n" , numline );
					fgetpos( fd1 , &pos );
					fgetc( fd1 );
					continue;
			
				}
				
				addtolabellist( hptr , word ); /*הכנסת התווית לרשימת התוויות. הפונקציה בתחתית הקובץ*/
				(*hptr) -> ext = 1; /* סימון שהתוויות היא חיצונית*/
				(*hptr) -> counter = 0; /* איפוס מונה מספר הפעמים שהתוויות החיצונית מופיעה בקובץ*/
					
			
			
			}
		
		}
		
		
		fgetpos( fd1 , &pos );
		fgetc( fd1 );
		
	}
	/* הדפסת המערכים לשם נוחות */
	for(i = 0 ; i < (( *ic ) * 14 ) ; i++){
		fprintf( stderr , " %d " ,  *( *inst + i )  );
	}		
	for(i = 0 ; i < (( *dc ) * 14 ) ; i++){
		fprintf( stderr , " %d " ,  *( *data + i )  );
	}		
	fprintf( stderr , "\n\n ic == %d \n\n dc == %d\n\n" , *ic , *dc );
	/*freelabellist( &hptr );*/

}

void buildarray( char** inst , char** data){ /* בנייה התחלתית של המערכים*/
 
	*data = ( char * ) malloc ( sizeof ( char ) );
	if( !(*data) ){
		printf( "allocated failed\n\n" );
		exit(0);
	}
	*inst = ( char * ) malloc ( sizeof ( char ) );
	if( !(*inst) ){
		printf( "allocated failed\n\n" );
		exit(0);
	} 
 
 
}


int islabel( char label[] ){

	int i ;
	int length = strlen( label );
	
	if( length > 30){
	
	return 3;
	}
	
	if( label[ length - 1 ] == ':' ) { /* :המילה נגמרת ב*/
	
		if( isalpha( label[0] ) ){ /* התו הראשון הוא אות*/
	
			for( i = 1 ; i < length-1 ; i++){
		
				if( !isalnum( label[i] ) )/* אם אחד התוים אינו אות או מספר*/
		  			return 2;/* תווית שגויה*/
		
			}
			label[ length -1 ] = '\0';
			return 1;	/*תווית תקנית*/
		
		}
		
		return 2;/* תווית שגויה - התו הראשון אינו אות*/
		
	}
	
	return 0; /* לא תווית*/
	
}

void addtolabellist( ptr2* hptr, char nameoflabel[] ){


	ptr2 t , p ;
	t = ( ptr2 ) malloc ( sizeof ( label ) ); /* הקצאת מקום לחוליה ברשימה*/
	
	if( !t ){
		printf( "allocated failed\n\n" );
		exit(0);
	}
	
	strcpy( t -> name , nameoflabel ); /* העתקת שם התוויות לשדה המתאים*/
	t -> ext = 0; /* התווית לא חיצונית*/
	t -> ent = 0; /*entry התווית לא */
	p = *hptr; /* עדכון החוליה הבאה*/
	*hptr = t ;
	t->next = p ; 

}

void freelabellist( ptr2* hptr ){/* שחרור רשימת התוויות*/

	ptr2 p;
	
	while( *hptr ){
	
		p = *hptr;
		*hptr =  (*hptr) -> next ;
		/*free( p -> locations );*/
		free( p );
		
	}

}



int findlabel( ptr2 hptr , char name1[] ){ /* מחזירה 0 אם יש כבר תווית בשם זה, 1 אחרת*/

	while( hptr ){ 
		if( !strcmp (( (hptr) -> name) , name1 ) )
			return 0; 
		hptr = ( (hptr) -> next);
	}
	return 1;

}

int instordata( char word[] ){

	if( word[0] == '.' ) /* אם התו הראשון הוא נקודה אז זהו משפט הנחיה*/
		return 0;
	return 1; /*משפט פקודה*/

}

int checkinst( char word[] ){

	int  i  ;
	char* instructions[] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
	
	for( i = 0 ; i < 16 ; i++ ){
		if( !strcmp ( word , instructions[i]) )
		
			return i; /* מחזיר את מספר הפקודה*/
	} 
	
	return -1; /* אין פקודה כזו */
	

}


int methodparam( char line[] , int* curr , int * param1 ){

	int i;
	char word[MAX_LINE];
	getword( line , word , curr );
	
	if( word[0] == '#')/*if the first char in the word contains #*/
		return 0;
		
	if( word [0] == 'r'  && word[1] >= 48 && word[1] <= 55  && word[2] == '\0')/*if the first char in the word contains is r and 2nd char is in the range of 0-7*/
		return 3;
			
	if( isalpha ( word[0] ) ){/*if the first char is from the alphabet*/
	
		for( i = 0 ; word[i] != '\0' ; i++ ){
		
			if( word [i] == '('){
				if( word [i +1 ] == '#' ){
					*param1 = 0 ;
				}	
				else if( word [i +1 ] == 'r'  && word [i +2 ] >= 48 && word[i+2] <= 55 &&  word[i+3] == '\0'){/*if its not in the range of 0-7 and the char after ( is r */
				
					*param1 = 3;
				}
				else
					*param1 = 1;		
			
			return 2;
			}
			
		} 
		return 1;
	
	
	}
	
	return -1;


}


void putinstruction( char ** in , int inst , int oper1 , int oper2 ,int* ic , int param1 , int param2   ){


	if( !param1 || param1 == 1 || param1 == -1)
		*( *in + ( *ic - 1 )*14  ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14  ) = 1;
	if( !param1 || param1 == 2 || param1 == -1)	
		*( *in + ( *ic - 1 )*14 +1  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +1 ) = 1;
	if( !param2 || param2 == 1 || param2 == -1)
		*( *in + ( *ic - 1 )*14 +2 ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14 +2 ) = 1;
	if( !param2 || param2 == 2 || param2 == -1)	
		*( *in + ( *ic - 1 )*14 +3  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +3 ) = 1;	
	if(inst >=8)
		*( *in + ( *ic - 1 )*14 +4 ) = 1;		
	else		
		*( *in + ( *ic - 1 )*14 +4 ) = 0 ;
	if( (inst >= 4 && inst < 8) || inst >= 12)
		*( *in + ( *ic - 1 )*14 +5 ) = 1 ;	
	else
		*( *in + ( *ic - 1 )*14 +5 ) = 0 ;
	if(inst ==2 || inst ==3 || inst ==6 || inst ==7 || inst ==10 || inst ==11 || inst ==14 || inst ==15 )	
		*( *in + ( *ic - 1 )*14 +6 ) = 1 ;
	else
		*( *in + ( *ic - 1 )*14 +6 ) = 0 ;
	if( inst % 2 )
		*( *in + ( *ic - 1 )*14 +7 ) = 1 ;
	else
		*( *in + ( *ic - 1 )*14 +7 ) = 0 ;
	if( !oper1 || oper1 == 1 || oper1 == -1)
		*( *in + ( *ic - 1 )*14 +8 ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14 +8 ) = 1;
	if( !oper1 || oper1 == 2 || oper1 == -1)	
		*( *in + ( *ic - 1 )*14 +9  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +9 ) = 1;	
	if( !oper2 || oper2 == 1 || oper2 == -1)
		*( *in + ( *ic - 1 )*14 +10 ) = 0 ;
	else
		*( *in + ( *ic - 1 )*14 +10 ) = 1;
	if( !oper2 || oper2 == 2 || oper2 == -1)	
		*( *in + ( *ic - 1 )*14 +11  ) = 0; 
	else
		*( *in + ( *ic - 1 )*14 +11 ) = 1;
	*( *in + ( *ic - 1 )*14 +12 ) = 0 ;
	*( *in + ( *ic - 1 )*14 +13 ) = 0 ;
	
	
	
								
}

void printlist( ptr2 hptr ){ /* הדפסה לשם נוחות*/

	while( hptr ){
	
		printf("%s\n" , hptr -> name);
		printf("%d\n" , hptr -> ext);
		printf("%d\n" , hptr -> ent);
		printf("%d\n" , hptr -> line);
		printf("%d\n" , hptr -> data);
		printf("%d\n" , hptr -> counter);
		puts("\n\n");
		hptr = hptr -> next;
	
	}
}

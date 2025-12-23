#include "preprocess.h"

void preprocess( FILE ** fd , FILE ** fd1 , char* name){

	
	int  flag = 0 , counter = 0 , curr  ;
	fpos_t pos;
	char line[MAX_LINE];
	char word[MAX_LINE];
	ptr hptr = NULL;
	ptr pt;
	char* q;
	
	*fd = openas( *fd , name ); /* . מצביע לקובץ זה. הפונקציה נמצאת בסוף הקובץfd  , (אם קיים) .as פתיחת הקובץ  */
	
	/* צריך לבדוק אם אם אין שגיאות בקובץ AS אם יש שגיאות אז לא ליצור את קובץ AM*/
	*fd1 = openam( *fd1 , name  ); /* .מצביע לקובץ זה. הפונקציה נמצאת בסוף הקובץ fd1  , .am יצירת הקובץ  */
	
	fgetpos( *fd , &pos ); /* שמירת מיקום + קליטת תו לבדיקה אם הגענו לסוף הקובץ */
	fgetc( *fd );
		
	while( !feof( *fd ) ){
	
		fsetpos( *fd , &pos ); /* חזרה למיקום */
		
		curr = 0; /* האינדקס שאנו נמצאים בשורה שנקלטה*/
		
		fgets( line , MAX_LINE , *fd ); /* line לתוך מערך .asקליטת שורה מ */
		
		getword( line , word , &curr ); /*func.c הפונקציה נמצאת בקובץ  .currעדכון המיקום הנוכחי ב +  word  למערך line קליטת מילה ממערך */
		
		
		if ( !flag ){ /* לא נמצאים בתוך מאקרו */
			
		
			if( !strcmp( word , "mcr")){ /* תחילת מאקרו*/
		
				flag = 1; /*סימון שאנחנו באמצע מאקרו*/
			
			
				getword( line , word , &curr ); /* word  למערך line קליטת מילה (שם המאקרו) ממערך */
				checkmacro( word ); /* בדיקה אם השם של המאקרו הוא לא שם של פקודה*/
				pt = findmacro( hptr , word ); /*   . בדיקה שאין כבר מאקרו עם שם זהה, הפונקציה נמצאת בתחתית הקובץ*/
				if( pt ){ /*סימן שכבר יש כזה מאקרו null יצביע לחוליה ולא ל pt אם */
				
					fprintf( stderr ,"2 macros with the same name");
					exit(0);
				}
				
				else{
					
					addtomacrolist( &hptr , word ); /* . הכנסת שם המאקרו לרשימה , הפונקציה נמצאת בתחתית הקובץ*/
				
				}	
		
			}
		
			else{ /* אנחנו לא בתחילת מאקרו אלא או בשורה רגילה או בקריאה למאקרו */
			
				pt = findmacro( hptr , word ); /* בודקים אם המילה הראשונה בשורה היא שם של מאקרו, אם כן נמצא מילה זו ברשימה של המקרואים*/
			
				if( pt ){ /* קיבלנו מצביע לרשימה , כלומר המילה היא קריאה למאקרו*/
				
					fputs( pt-> data , *fd1); /*.am כתיבת המאקרו בקובץ */
			
				}
			
				else{ /*אנחנו בשורה רגילה*/
				
					fputs( line , *fd1); /*.am כתיבת השורה בקובץ */
				
				}	
		
			}
		
			
		
		}
		
		else { /* אנחנו באמצע מאקרו*/
			
			
			if( !strcmp( word , "endmcr")){ /*הגענו לסוף המאקרו */
			
				flag = 0 ; /* סימון שאנחנו לא במתוך מאקרו*/
				counter = 0; /* איפוס כמות התווים במאקרו */
				
			}
			
			else{
			
				counter += strlen( line ); /* ספירת כמה תווים מכיל המאקרו*/
				q = ( char * ) realloc ( hptr -> data , counter  );/*הקצאת מקום לכמות תהויים החדשה*/
				if(!q){ /*הקצאה נכשלה*/
					fprintf(stderr , "error");
					exit(0);
		
				}
				hptr -> data = q;
				strcat( hptr -> data , line );/* הכנסת השורה למערך הדינמי*/
		
			
			}
			
			
		}
		
		fgetpos( *fd , &pos ); /* שמירת מיקום + קליטת תו לבדיקה אם הגענו לסוף הקובץ */
		fgetc( *fd );
		
	}
		
	freemacrolist( &hptr );	/* שחרור הרשימה, הפונקציה בתחתית הקובץ*/
		
}

FILE* openas( FILE * fd  , char* name ){

	char filename[MAX_FILE]; /* מערך בגודל שם קובץ מקסימלי*/
	strcpy( filename , name ); /* העתקת שם הקובץ ללא הסיומת  למערך */

	fd = fopen( strcat( filename , ".as" ), "r+" ); /* הדבקת הסיומת המתאימה לשם הקובץ ופתיחתו לקריאה*/
	
	if( !fd ){ /* the file didn't open successfuly */
	
		fprintf( stderr , "\nCannot open file \n\n" );
		fprintf( stderr , "----------------------------------------------------------------\n\n" );
		exit(0);
		}
	return fd; /* החזרת המצביע לקובץ*/	

}

FILE* openam( FILE * fd1  , char* name ){

	char filename[MAX_FILE]; /* מערך בגודל שם קובץ מקסימלי*/
	strcpy( filename , name );  /* העתקת שם הקובץ ללא הסיומת למערך */
	
	fd1 = fopen( strcat( filename , ".am" ) , "w+" ); /* הדבקת הסיומת המתאימה לשם הקובץ ופתיחתו לכתיבה*/
	
	if( !fd1 ){ /* the file didn't open successfuly */
	
		fprintf( stderr , "\nCannot create file \n\n" );
		fprintf( stderr , "----------------------------------------------------------------\n\n" );
		exit(0);
		}
		
	return fd1; /* החזרת המצביע לקובץ*/	

}


void addtomacrolist( ptr* hptr, char nameofmac[] ){


	ptr t , p ;
	t = ( ptr ) malloc ( sizeof ( macro ) ); /* הקצאת מקום לחוליה ברשימה המקושרת*/
	
	if( !t ){
	
		printf("cannot build\n\n");
		exit(0);
		
	}
	
	strcpy( t -> name , nameofmac ); /* הכנסת שם המאקרו לשדה המתאים בחוליה*/
	p = *hptr; /* עדכון החוליה הבאה*/
	*hptr = t ;
	t->next = p ; 

}

void freemacrolist( ptr* hptr ){

	ptr p;
	
	while( *hptr ){
	
		p = *hptr;
		*hptr = ( (*hptr) -> next);
		free(p->data); /* שחרור המערך הדינמי שמכיל את תוכן המאקרו*/
		free(p); /*שחרור החוליה*/
		
	}

}

/*void printlist( ptr hptr ){

	while( hptr ){
	
		printf("%s\n" , hptr -> name);
		hptr = hptr -> next;
	
	}


}*/


ptr findmacro(ptr hptr , char name1[] ){

	while( hptr && strcmp ( ((hptr) -> name) , name1 ) ){ /*נמשיך לעבור על הרשימה name1 כל עוד לא סיימנו את הרשימה וגם לא מצאנו מאקרו עם שם כמו שנמצא במערך */
	
		hptr = ((hptr) -> next);
		
	}
	return hptr;/*אם אין כזה מאקרו null  החזרת מצביע למאקרו שנמצא, או */

}

void checkmacro( char macro[] ){/*בדיקה שאין מאקרו עם שם של פקודה*/

	int  i;
	char* instructions[] = { "mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

	for( i = 0 ; i < 16 ; i++ ){
		if( !strcmp ( macro , instructions[i]) )
			fprintf( stdout , "error" );	
	
	} 


}



#include "preprocess.h"
#include "secondpass.h"

int main( int argc , char* argv[] ){

	FILE *fd = NULL , *fd1 = NULL /*, *fd2 = NULL , *fd3 = NULL , *fd4 = NULL*/ ; /* fd- .asמצביע לקובץ שמסתיים ב . fd1 - .am מצביע לקובץ שנוצר עם סיומת  */
	int i , ic = 0, dc = 0 ; /* ic - insrtuctions כמות השורות הנצרכות לקידוד ה ,  dc - data כמות השורות הנצרכות לקידוד ה*/
	char *inst  , *data ;/* inst - מצביע לתחילת המערך הדינמי בו מאוחסנות הפקודות .  data - מצביע לתחילת המערך הדינמי בו מאוחסנים הנתונים*/
	ptr2 hptr = NULL; /* (labels) מצביע לראש הרשימה המקושרת שתכיל את התוויות   */

	/*for( i = 1 ; i < argc ; i++ ){*/

		preprocess( &fd , &fd1 , "b"  /*argv[i]*/ ); /*שלב פרישת המקרואים*/

		firstpass( fd1  , &ic , &dc , &inst , &data , &hptr );/*מעבר ראשון*/
		
		secondpass( fd1 , &ic , &dc , &inst , &data , &hptr , "b"  /*argv[i]*/ );/*מעבר שני */
		/*printlist(hptr);*/
		
		freelabellist( &hptr ); /*freeing allocated memory of the label linked list*/
		free( inst ); /* freeing allocated memory of the instructions dinamic array*/
		free( data ); /*freeing allocated memory of the data dinamic array*/
		fclose( fd ); /*  closing .as file */
		fclose( fd1 ); /*  closing .am file */
		/*fclose( fd2 );
		fclose( fd3 );
		fclose( fd4 );*/

/*	}*/
	
	return 0;
	
}

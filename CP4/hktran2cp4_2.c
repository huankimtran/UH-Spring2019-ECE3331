/*
@Author	:	Huan Kim Tran
@Date	:	03/13/2019
@Problem:	CP4_2
@Purpose:	Filtering out non-numalphabet characters
*/
#include <stdio.h>

typedef char* string;                               /*char* as string for convience*/

const string INPUT	=	"cp4in_2.txt";              /*Input filename*/

int main(void){
	FILE *inpf;                                     /*inpf for reading strings from the input file*/
	char c;                                         /*c is to hold the current character*/
	inpf=fopen(INPUT,"r");                          /*Open file for reading*/                           
	while(fscanf(inpf,"%c",&c)!=EOF)                /*Reading in the character of each string one by one*/
	    if((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||(c=='\n'))     /*Filter out alphabet and numeric characters*/
	        printf("%c",c);                                                     /*Print out valid characters*/
	fclose(inpf);                                   /*Close the file*/
	return 1;
}

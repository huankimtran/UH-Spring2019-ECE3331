/*
@Author	:	Huan Kim Tran
@Date	:	03/13/2019
@Problem:	CP4_1
@Purpose:	Merging strings
*/
#include <stdio.h>

#define MAX_LENGTH 21                               /*Maximum length of a string*/

typedef char* string;                               /*char* as string for convience*/

const string INPUT	=	"cp4in_1.txt";              /*Input filename*/

string mergeStrings(string,string,int,int);
int getString(string,FILE*);

int main(void){
	FILE *inpf;                                     /*inpf for reading strings from the input file*/
	char s1[MAX_LENGTH*2],s2[MAX_LENGTH];           /*s1 to hold the first string, s2 to hold the second string*/
	int l1,l2;                                      /*l1,l2 are to hold the lengths of s1 and s2*/
	inpf=fopen(INPUT,"r");                          /*Open file for reading*/                           
	while(1){
	    l1=getString(s1,inpf);                      /*Get the first string and save it to s1*/
		l2=getString(s2,inpf);                      /*Get the second string and save it to s2*/
		if(l1==EOF||l2==EOF)                        /*End of file yet?*/
		    break;                                  /*Getting out of the infinite loop*/
		mergeStrings(s1,s2,l1,l2>0?l2:-l2);         /*If not, merge the two strings according to the required fashion*/
		printf("%s\n",s1);                          /*Print out the result to the screen*/
	}
	fclose(inpf);                                   /*Close the file*/
	return 1;
}

/*
@Purpose:	Interweave two inputted strings, save result to string 1 and return string 1's address
@Para s1: 	First string
@Para s2:	Second string
@Para l1:	First string's length
@Para l2:	Second string's length
@Return	:	Address of the first string
*/
string mergeStrings(string s1,string s2,int l1, int l2){
	int i,p1,p2;                                                                    /*i-> iterating variable, p1 current position of the first string, p2 current position of the second string*/
	char rs[MAX_LENGTH*2];                                                          /*rs is to hold the result string*/
	p1=p2=0;                                                                        /*begin at the first position*/
	for(i=0;i<l1+l2;i++)                                                            /*loop through each position of the result string*/
		*(rs+i)=(i%2?(p2<l2?*(s2+p2++):*(s1+p1++)):(p1<l1?*(s1+p1++):*(s2+p2++)));  /*the current position of rs is from s1 if the index is even, from s2 if the index is odd. If p1 or p2 is at the last character, insert the character from other string*/
	*(rs+i)='\0';                                                                   /*Null terminating the result string*/
	for(i=0;i<=l1+l2;i++)                                                           /*Copy the result string to s1*/
		*(s1+i)=*(rs+i);
	return s1;                                                                      /*Return s1's address*/
}
/*
@Purpose:	Read out the next string in the input file
@Para s : 	Buffer that holds the readout string
@Para f :	Pointer to the current input file
@Return	:	EOF if reached end of file, otherwise the length of the string
*/
int getString(string s,FILE* f){
    char c;                         /*c is to hold the character read in from the input file*/
    int pos,rs;                     /*pos is to hold the current position, rs is to hold the result from fscanf*/
    pos=0;                          /*Setting the first position to 0*/
    do{
        rs=fscanf(f,"%c",&c);       /*Read the next character*/
        if(c=='\n' || rs==EOF)      /*If a newline character is read=> string is terminated*/
            break;
        else
            *(s+pos++)=c;           /*Save the current character to the current position in s and increase pos by 1*/
    }while(1);
    *(s+pos)='\0';                  /*Null terminating s*/
    return rs==EOF?EOF:pos;         /*Return the length of the string or EOF*/
}

/*
@author : Huan Tran
@program: CP5
@purpose: parsing an assignment statement
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100          /*Maximum length of an assignment statement*/
#define OPERATOR 0              /*Index of operator group*/
#define SEPARATOR 1             /*Index of separator group*/
#define INITIAL_IDENTIFIER 2    /*Index of initial identifier group*/
#define IDENTIFIER 3            /*Index of identifier group*/
#define PARENTHESIS 5           /*Denote last level has parenthesis */
#define CONSTANT 4              /*Denote last level has constant */
#define VARIABLE 1              /*Denote last level has variable*/

int parse(int,char*,int,int);
int identifierCheck(char*,int);
int constantCheck(char*,int);
int isIt(int,char);
int isEnd(char*);

const char* INP="cp5in.txt";                /*Input file name*/
const char* GROUP[5]={"+-*/"
                    ," \t"
                    ,"_AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
                    ,"_0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
                    ,"0123456789"};
/*GROUP contains group of strings that are used to identify*/
int main()
{
    FILE *f;                /*f is input file*/
    char s[MAX_LENGTH];     /*s holds assignment statement*/
    f=fopen(INP,"r");
    do{
        fgets(s,MAX_LENGTH,f);          /*get the whole line*/
        if(parse(0,s,0,0))              /*parse the statement and dictate if it is valid*/
            printf("%s\nThis statement is legal.\n",s);
        else
            printf("%s\nThis statement is illegal.\n",s);
    }while(!feof(f));                   /*end of file yet?*/
    return 0;
}
/*
Check if the character c belong to the group specified by group id n
n is group id
c is the character
*/
int isIt(int n,char c){
    if(strchr(GROUP[n],c)==NULL)            /*search the character c if it presents in GROUP[n]*/
        return 0;
    else
        return 1;
}
/*Check if there is anything else after ;
s is the string
pos is the position after ;*/
int isEnd(char* c){
    for(;*c!='\0'&&isIt(SEPARATOR,*c);c++);     /*after ; must be separator or else it is not a valid statement*/
    return *c=='\0'?1:0;
}
/*
Check if the next characters starting at s is a valid identifier
s is the string
pos is the position where the indetifierCheck start checking
return the position of the last character of the valid indentifier
else return -1
*/
int identifierCheck(char* s, int pos){
    if(isIt(INITIAL_IDENTIFIER,s[pos++])){          /*first character of the string is valid?*/
        for(;isIt(IDENTIFIER,s[pos]);pos++);        /*is following characters of the string is valid?*/
        return --pos;
    }else
        return -1;                                  /*No*/
}
/*
Check if the next characters starting at s is a valid identifier
s is the string
pos is the position where the indetifierCheck start checking
return the position of the last character of the valid constant
else return -1
*/
int constantCheck(char* s, int pos){
    int flag=-1;
    if(s[pos]=='.')
        flag=pos,pos++;
    if(!isdigit(s[pos]))                    /*if the first character is not a digit, this is not a digit string*/
        return -1;
    else{
        /*Run until hit something not digit*/
        for(;isdigit(s[pos]);pos++);
        /*if that non-digit thing is a '.', not the last character
        and its next character is a digit, then
        , increase pos by 1
        ,and let it continue running since this might be a float constant*/
        if((s[pos]=='.')&&flag==-1){
            flag=pos;
            pos++;
            for(;isdigit(s[pos]);pos++);
            /*after the for, if s[pos] is not a digit,
             the return statement below will still return the last position of a valid constant*/
        }
        return --pos;
    }
}
/*The main parsing function. This function traverse the parsing tree documented at the end of this file.
It traverses each level to make sure the syntax is correct. If a level a correct, it call itself to check the next level.
The first call of the function will return the value returned by the deepest level the function reached.
If it is 0, the statement is illegal, or 1 for a legal statement
n is the level of parsing
s is the string to parse
pos is the current location
pra is the additional information from the last level*/
int parse(int n,char* s, int pos,int pra){
    int i,l;
    if(s[pos]=='\0')
        return 0;
    l=strlen(s);
    switch(n){
    case 0:                                     /*Ground level*/
        if(s[l-1]=='\n')                        /*If last character is \n, delete it, this is the consequence of using fgets()*/
            s[--l]='\0';
        if(l<4)                                  /*Minimum length for a legal assignment is 4*/
            return 0;
        for(i=0;i<l;i++)
            if(!isIt(SEPARATOR,s[i])){              /*Finding the next non separator character*/
                if((i=identifierCheck(s,i))!=-1)    /*Next characters are valid identifier ?*/
                    if(l==i+1)
                        return 0;                   /*No, return 0*/
                    else
                        return parse(1,s,++i,0);    /*If it is valid, continue to the next level*/
                else
                    return 0;
            }
        return 0;
    case 1:                                         /*Level 1*/
        for(;pos<l;pos++)
            if(!isIt(SEPARATOR,s[pos])){            /*Finding the next non separator character*/
                if(s[pos]=='=')                     /*Level 2 just need to find '=' if, it exist, move to the next level*/
                    return parse(2,s,++pos,0);
                else
                    return 0;
            }
        return 0;
    case 2:
        for(;pos<l;pos++)                           /*Level 2*/
            if(!isIt(SEPARATOR,s[pos])){            /*Finding the next non separator character*/
                i=constantCheck(s,pos);             /*Check if it is a constant*/
                if(i!=-1)
                    return parse(3,s,++i,CONSTANT);     /*yes, so move to next level with parameter CONSTANT*/
                else if(isIt(INITIAL_IDENTIFIER,s[pos]))        /*Check if it is an identifier*/
                        if((pos=identifierCheck(s,pos))!=-1)
                            return parse(3,s,++pos,VARIABLE);   /*move to the next level with parameter VARIABLE*/
                return 0;
            }
        return 0;
    case 3:
        for(;pos<l;pos++)                           /*Level 3*/
            if(!isIt(SEPARATOR,s[pos])){            /*Finding the next non separator character*/
                if(s[pos]==';')                     /*If this is ';', there should not be any non-separator characters at the end*/
                    return isEnd(s+pos+1);
                else if(isIt(OPERATOR,s[pos]))      /*If it is an operator*/
                    return parse(4,s,++pos,OPERATOR);
                else if(s[pos]=='(')                /*if it is a parenthesis*/
                            if(pra==VARIABLE)
                                return parse(4,s,++pos,PARENTHESIS);    /*Move to next level with parameter PARENTHESIS*/
                return 0;
            }
        return 0;
    case 4:                                         /*Level 4*/
        for(;pos<l;pos++)
            if(!isIt(SEPARATOR,s[pos])){            /*Finding the next non separator character*/
                i=constantCheck(s,pos);             /*Is it a constant*/
                if(i!=-1)
                    return parse(5,s,++i,pra==PARENTHESIS?PARENTHESIS:CONSTANT);        /*Move to the next level*/
                else if(isIt(INITIAL_IDENTIFIER,s[pos]))
                        if((pos=identifierCheck(s,pos))!=-1)
                            return parse(5,s,++pos,pra==PARENTHESIS?PARENTHESIS:VARIABLE);  /*Move to the next level */
                return 0;
            }
        return 0;
    case 5:                                          /*Level 5*/
        for(;pos<l;pos++)
            if(!isIt(SEPARATOR,s[pos])){                        /*Finding the next non separator character*/
                if(pra==PARENTHESIS)                            /*If the last level was a parenthesis*/
                    if(s[pos]==')')
                        return parse(6,s,++pos,PARENTHESIS);    /*Close the parenthesis*/
                    else
                        return 0;
                else if(s[pos]==';')                            /*If this is ;, it is terminating the statement*/
                        return isEnd(s+pos+1);                  /*Is there anything after ; ?*/
                return 0;
            }
    case 6:                                                 /*Level 6*/
        for(;pos<l;pos++)
            if(!isIt(SEPARATOR,s[pos]))                         /*Finding the next non separator character*/
                if(s[pos]==';')                                 /*This level only looks for ;*/
                    return isEnd(s+pos+1);
                else return 0;
        return 0;
    }
}


/*
The tree used to parse the statement!

0                            var
                              |
1                             =
                            /  \
                           /    \
2                     const       var
                    /    |       /   |   \
3                  ;    ope     ;   ope   (
                     /   |        / |    |    \
4                const  var  const var  const  var
                 |       |     |    |    |      |
5                ;       ;     ;    ;    )      )
                                         |      |
6                                        ;      ;
*/

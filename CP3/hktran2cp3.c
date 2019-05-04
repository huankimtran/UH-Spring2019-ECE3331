/*
***************************
@author: Huan Kim Tran
@UHID:  1831696
@Program: CP3
@Purpose: Converting a list a decimal number into their binary representations
*********************8*****
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASK 0x8000
#define DATA_LENGTH 32
#define MAX_STR_LENGTH 100
#define INTEGER_SPACE 17
#define DECIMAL_SPACE 17

typedef char* string;                                                   /*Named char* as string*/

void ReadNos();                                                         /*Function to allocate, initiate variables and begin the converting from a number to its binary representation process*/
string ConvertDecimal2Binary(string,string,int);                        /*Function to convert a number into its binary representation*/
void WriteBinaryNos(string,string,int);                                 /*Function to write out a the binary representation of a number in a nice way*/
char getSystemEdian();                                                  /*Function to check the current system Edianess (Big Edian or Little Edian)*/

const string INPUTFILE="cp3.txt";        	                            /*Name of the input file*/
const string OUTPUTFILE="cp3.out.txt";   	                            /*Name of the output file*/
FILE *inFile,*outFile;                  	                            /*inFile is a global file pointer pointing to the input file
                                                                          outFile is a global file pointer pointing to the output file*/

int main()
{
    ReadNos();                          	                            /*Calling ReadNos()*/
    return 0;
}
void ReadNos()
{
    char c,sBin[MAX_STR_LENGTH],s[MAX_STR_LENGTH];                      /*sBin -> to hold the binary form of number in s*/
    /*s -> accumulate character to form the current number*/
    /*c -> hold the current character*/
    int isFloat;                                                        /*isFloat -> flag bit to keep track of the current number type:floating point (1) or integer (0)*/
    isFloat=0;                                                          /*Assuming the number is an integer*/
    sBin[0]='\0';
    s[0]='\0';
    inFile=fopen(INPUTFILE,"r");                                        /*open file to read*/
    outFile=fopen(OUTPUTFILE,"w");                                      /*open file to write*/
    while(fscanf(inFile,"%c",&c)!=EOF)                                  /*Read the file until end of file*/
    {
        if(c=='.')                                                      /*numbers having '.' are floating point numbers*/
        {
            isFloat=1;                                                  /*Yes it is a floating point number*/
            strncat(s,&c,1);                                            /*Add the decimal point to s*/
        }
        else if((c=='\n')||(c==' '))                                    /*Detecting the end of a number*/
        {
            ConvertDecimal2Binary(s,sBin,isFloat);                      /*Decode the number into binary format and hold in sBin*/
            WriteBinaryNos(s,sBin,isFloat);                             /*Print out the binary and the number in a nice format to get +5 bonus*/
            s[0]='\0';                                                  /*Reseting the buffer for holding the next number*/
            isFloat=0;                                                  /*Reseting the flag for the next number*/
            sBin[0]='\0';                                               /*Reseting sBin*/
        }
        else
            strncat(s,&c,1);                                            /*adding the new digit to the end of s and make sure only 1 character is copied*/
    }
    /*if the input file end the last number by EOF*/
    if(strlen(s)>0)
    {
        ConvertDecimal2Binary(s,sBin,isFloat);                          /*Decode the number into binary format and hold in sBin*/
        WriteBinaryNos(s,sBin,isFloat);                                 /*Print out the binary and the number in a nice format to get +5 bonus*/

    }
    fclose(inFile);                                                     /*Close file*/
    fclose(outFile);                                                    /*Save the result*/
}
/*Converting the number stored in s to corresponding binary representation*/
/*s holds the number*/
/*sOut contains the returned binary format*/
/*isFloat=1 -> the number is a floating point number, otherwise it is an integer*/
string ConvertDecimal2Binary(string s,string sOut,int isFloat)
{
    float f;                                                            /* f is used for holding the float contained in s*/
    int i,j,sz;                                                         /* i,j are variables used for iterations*/
                                                                        /* sz is to hold the size of the type*/
    char E,*p,bit;                                                      /*E is to hold the Edian type of the current system, 1 is Little Edian and 0 is Big Edian*/
                                                                        /*p is to hold the first address of variable f or variable l, depending on the state of isFloat*/
                                                                        /*bit is to hold the '1' or '0' of the current bit*/
    long int l;                                                         /* l is used for holding the integer contained in s*/
    E=getSystemEdian();                                                 /* Find the Edianess of this system*/
    if(isFloat)                                                         /* Is s containing a non-integer number?*/
    {
        sscanf(s,"%f",&f);                                              /*Decode f*/
        p=&f;                                                           /*p holds the address of the first or last byte of f, depending on the system's Edianess*/
        sz=sizeof(f);                                                   /*How many bytes does f occupied?*/
    }
    else
    {
        sscanf(s,"%li",&l);                                             /*Decode it into a long int if it is a decimal number*/
        p=&l;                                                           /*Find the first cell's address*/
        sz=sizeof(l);                                                   /*Find the size*/
    }
    for(i=E?sz-1:0; i!=(E?-1:sz); (E?i--:i++))                          /*Looping forward or backward depending on the system's Edianess*/
        for(j=0; j<8; j++)                                              /*Iterating through it bit*/
        {
            bit=((*(p+i))&(0x80>>j)?'1':'0');                           /*Move the MASK 0x80=0b10000000 gradually to the right to decode the bit*/
            strncat(sOut,&bit,1);                                       /*Add the ASCII representation of the bit in to sOut*/
        }
    for(i=strlen(sOut)-DATA_LENGTH,j=0; j<=DATA_LENGTH; i++,j++)        /*Take only the last 32 bits*/
        sOut[j]=sOut[i];
    return sOut;                                                        /*returning the binary form of the number*/
}
/*Printing out the binary number in a nice format*/
/*s holds the number in original format*/
/*sOut holds the number in the binary format*/
/*isFloat=1 -> the number is a floating point number, otherwise it is an integer*/
void WriteBinaryNos(string s,string sOut,int isFloat)
{
    int i,j,dotPos;                                                     /*i,j iterating variables*/
    /*dotPos holds the place of the decimal point*/
    fprintf(outFile,"The binary representation of decimal");
    if(isFloat)
    {
        dotPos=0;
        j=strlen(s);
        for(i=0; i<j; i++)                                              /*Finding the decimal point position*/
        {
            fprintf(outFile," ");
            if(s[j-i-1]=='.')
                break;
        }
        fprintf(outFile,"%17s",s);                                      /*Printing out the original number*/
        for(j=DECIMAL_SPACE; j>i; j--)
            fprintf(outFile," ");
        fprintf(outFile,"is ");
        for(i=0; i<DATA_LENGTH; i++)
        {
            fprintf(outFile,"%c",sOut[i]);
            switch(i)                                                   /*Adding space to split the floating point binary*/
            {
            case 0:
            case 8:
                fprintf(outFile," ");
                break;
            }
        }
        fprintf(outFile,"\n");
    }
    else
        fprintf(outFile,"%17s%17s is %s\n",s," ",sOut);                 /*Printing out the integer numbers*/
}
/*Checking the system Edianess*/
char getSystemEdian()
{
    int a;                                                              /*int is a multi-byte data type. a=1 -> the first byte is 00000001 if this is a Little Endian system Otherwise, it is a Big Endian system*/
    char* p;                                                            /*pointer to holds the first address of a*/
    a=1;                                                                /*a= 0b00000000 00000000 00000000 00000001*/
    p=&a;                                                               /*Point the pointer to the first byte of a*/
    return (*p?1:0);                                                    /*Check the value of the first byte of a and return 1 if it is Little Edian and 0 for Big Endian*/
    /*p has type char because char always occupies 1 byte regardless of system, so deferencing a char pointer always return 1 byte of data.*/
}

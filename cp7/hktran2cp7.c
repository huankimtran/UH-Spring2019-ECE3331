/*
***************************
@author: Huan Kim Tran
@UHID:  1831696
@Program: CP7
@Purpose: Polynomials manipulations
*********************8*****
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define BUFFER 5
#define PRECISION 3
#define GET_TERM ((PTRTERM)malloc(sizeof(struct Term)))

const char* OUT_FILE= "cp7.out";    /*File out*/
const char* IN_FILE= "cp7_in.txt";  /*File in*/
struct Term
{
    double co;       /*Coefficient*/
    int de;         /*Degree*/
    struct Term *n; /*Next term*/
};
typedef struct Term* PTRTERM;  	/*Pointer to term type*/
typedef PTRTERM*  PTRPOLY;  	/*Poly cluster*/

PTRPOLY input_poly(FILE*);      /*Read in polynomials*/
PTRTERM add_poly(PTRPOLY);      /*Adding polynomials*/
PTRTERM mult_poly(PTRPOLY);     /*Multiplying polynomial*/
void print_poly(PTRTERM);       /*Printing polynomial*/
void sort(PTRTERM);             /*Sorting linked list*/
void combine(PTRTERM);          /*Combining term with the same degree*/
void free_up(PTRTERM);          /*Free linked list*/
void print_co(double,int);       /*Print double nicely*/
FILE *fi,*fo;                   /*fi is for input file, fo is for output file*/
int main()
{
    PTRPOLY p;                  /*The pair of polynomials for each process*/
    fi=fopen(IN_FILE,"r");      /*Open input file*/
    fo=fopen(OUT_FILE,"w");     /*Open output file*/
    while((p=input_poly(fi))&&!feof(fi))    /*Read in the polynomial and check if end of file reached*/
    {
        /*Cleaning and beautify the input polynomials*/
        sort(p[0]);
        sort(p[1]);
        combine(p[0]);
        combine(p[1]);
        /*Printign out the sum*/
        fprintf(fo,"The sum of polynomials::\n");
        print_poly(p[0]);
        fprintf(fo,"and\n");
        print_poly(p[1]);
        fprintf(fo,"is\n");
        print_poly(add_poly(p));
        /*Printing out the product*/
        fprintf(fo,"\nThe product of polynomials::\n");
        print_poly(p[0]);
        fprintf(fo,"and\n");
        print_poly(p[1]);
        fprintf(fo,"is\n");
        print_poly(mult_poly(p));
        fprintf(fo," \n");
        /*Free up space*/
        free_up(p[0]);
        free_up(p[1]);
    }
    /*Save files*/
    fclose(fi);
    fclose(fo);
    return 0;
}
/*Reading in the polynomial*/
/*f is pointer to the input file*/
PTRPOLY input_poly(FILE * f)
{
    PTRPOLY sys;        /*sys is to hold the pair of read-in polynomial*/
    PTRTERM ptr;        /*ptr is for Iterating variable through each term*/
    int ct,i,c,be,de;   /*ct is to hold file position*/
                        /*i is for iterating*/
                        /*c is to read in character by character*/
                        /*be is to hold the initial location before reading the degrees and coefficients*/
                        /*de is to hold the degree*/
    char *s,*p;         /*s is to hold the entrance of the string containing degrees and coefficients*/
                        /*p is to iterating through s*/
    double co;          /*co is to hold coefficients*/
    /*Allocating space for a cluster of two new polynomials*/
    sys=(PTRPOLY)calloc(2,sizeof(PTRTERM));
    /*Building the polynomials*/
    for(i=0; i<2; i++)
    {
        /*Getting a line*/
        be=ct=ftell(f);
        while((c=getc(f))!='\n'&&(c!=EOF));
        ct=ftell(f)-ct;
        /*Back to the original position*/
        fseek(f,be,SEEK_SET);
        ct+=BUFFER;
        /*Allocating memory to read in the entire line*/
        s=p=(char*)calloc(ct,1);
        /*Read in the line*/
        fgets(s,ct,f);
        /*Initialize the linked list*/
        sys[i]=ptr=NULL;
        /*Parsing the line*/
        do{
            /*Eliminate leading spaces*/
            for(;isspace(*p); p++);
            /*Getting coefficient and degree of each term*/
            if(sscanf(p," %lf%d",&co,&de)==2){
                if(sys[i]==NULL)
                    sys[i]=ptr=GET_TERM;
                else
                    ptr=(ptr->n=GET_TERM);
                ptr->de=de;
                ptr->co=co;
            }else
                break;
            /*Get passed current coefficient*/
            for(;!isspace(*p);p++);
            for(;isspace(*p);p++);
            /*Get passed current degree*/
            for(;!isspace(*p);p++);
        }while((*p!='\0')&&(*p!='\n'));
        /*Ending the linked list*/
        if(ptr!=NULL)
            ptr->n=NULL;
        /*Free space used to hold the line*/
        free(s);
    }
    /*Sort and clean the chains*/
    sort(sys[0]);
    sort(sys[1]);
    return sys;
}
/*Adding two polynomials*/
/*sys is to hold the pair of polynomials needed to be processed*/
PTRTERM add_poly(PTRPOLY sys)
{
    PTRTERM sum,p1,p2;  /*sum is to hold the sum polynomial*/
                        /*p1 and p2 is to loop through each input polynomial*/
    int i;              /*i is to iterate through each polynomial*/
    /*Get first element of the list*/
    sum=p2=GET_TERM;    /*Initializing*/
    /*Mixing 2 polynomials to sum*/
    for(i=0; i<2; i++)
        for(p1=sys[i]; p1!=NULL; p1=p1->n)
        {
            *p2=*p1;
            if(p1->n==NULL&&i==1)
                p2->n=NULL;
            else
                p2=(p2->n=GET_TERM);
        }
    /*Clean and sort the list*/
    sort(sum);
    /*Combine terms with the same degree*/
    combine(sum);
    return sum;
}
/*Multiplying two polynomials*/
/*sys is the pair of polynomials needed to be processed*/
PTRTERM mult_poly(PTRPOLY sys)
{
    PTRTERM p,p1,p2,mult;
    mult=p=NULL;
    for(p1=sys[0]; p1!=NULL; p1=p1->n)
        for(p2=sys[1]; p2!=NULL; p2=p2->n)
        {
            if(mult==NULL)
                mult=p=GET_TERM;
            else
                p=(p->n=GET_TERM);
            p->de=p1->de+p2->de;
            p->co=p1->co*p2->co;
        }
    p->n=NULL;
    /*Sort and combine terms with the same degree*/
    sort(mult);
    combine(mult);
    return mult;
}
/*Printing the double value with the maximum precision*/
/*d is the number needed to be printed*/
/*first is boolean variable 1 if this is the first term of the polynomial otherwise..*/
void print_co(double d,int first){
    int a,i;  /*a is to hold the integer version of d*/
                /*i is to hold the actual precision of d*/
    /*Detecting trailing zeros*/
    a=d*pow(10,PRECISION);
    for(i=3;a%10==0&&i>0;i--)
        a/=10;
    /*Printing out with correct precision. Ex:9.090 ->9.09*/
    switch(i){
    case 0:
        first?fprintf(fo,"%.lf",d):fprintf(fo,"%+.lf",d);
    break;
    case 1:
        first?fprintf(fo,"%.1lf",d):fprintf(fo,"%+.1lf",d);
    break;
    case 2:
        first?fprintf(fo,"%.2lf",d):fprintf(fo,"%+.2lf",d);
    break;
    case 3:
        first?fprintf(fo,"%.3lf",d):fprintf(fo,"%+.3lf",d);
    break;
    }
}
/*Printing the polynomial*/
/*p is the entrance of the polynomial*/
void print_poly(PTRTERM p)
{
    int pl=1;   /*pl is to determine if the current term is the first term*/
    /*Sort term in descending order*/
    /*NULL then quit*/
    if(p==NULL)
        return;
    /*Sort in descending order first*/
    sort(p);
    /*Print out term by term*/
    do{
        if(p->co!=0){
            if(p->de!=0){
                if(p->co!=1.000&&p->co!=-1.000)
                    print_co(p->co,pl); /*Print out coefficient when it is not 1 or -1*/
                else if(pl?(p->co<0?1:0):1) /*print out the sign of the first term correctly*/
                        fprintf(fo,"%c",(p->co>0?'+':'-'));
                if(p->de!=1)
                    fprintf(fo,"x^%d",p->de);
                else    /*If degree is 1, no need to print*/
                    fprintf(fo,"x");
            }else
                print_co(p->co,pl);
            /*Flip the bit of first term after processing the first term*/
            if(pl==1)
                pl=0;
        }
    }while((p=p->n)!=NULL);
    /*If no term has been printed, print 0*/
    if(pl)
        fprintf(fo,"0");
    fprintf(fo,"\n");
}
/*Sorting the linked list*/
/*p is the entrance of the linked list*/
void sort(PTRTERM p)
{
    PTRTERM p1;
    struct Term tmp;
    for(; p!=NULL&&p->n!=NULL; p=p->n)
        for(p1=p->n; p1!=NULL; p1=p1->n)
            if(p->de<p1->de)
            {
                tmp=*p;
                p->de=p1->de;
                p->co=p1->co;
                p1->de=tmp.de;
                p1->co=tmp.co;
            }
}
/*Combining term with equal degree*/
/*sum is the linked list containing the polynomial*/
void combine(PTRTERM sum)
{
    PTRTERM p1,p2;
    /*Combining term's coefficients*/
    if(sum==NULL)
        return;
    for(p1=sum; p1!=NULL&&p1->n!=NULL; p1=p1->n)
    {
        /*find next term with different degrees*/
        for(p2=p1->n; p2!=NULL&&p2->de==p1->de; p2=p1->n)
        {
            /*Adding coefficient*/
            p1->co+=p2->co;
            /*Elimiate the chain with identical degree*/
            p1->n=p2->n;
            free(p2);
        }
    }
}
/*Free up linked list chain by chain*/
/*p is the linked list needed to be freed*/
void free_up(PTRTERM p){
    PTRTERM tmp;
    for(;p!=NULL;tmp=p->n,free(p),p=tmp);
}

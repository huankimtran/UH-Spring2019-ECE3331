#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PARA_NUM 4  /*Number of operation*/

/*Linked list struct*/
struct DATASTR{
    int c;              /*The character of the current link*/
    struct DATASTR* n;  /*Pointer to the next link of the linked list*/
};
typedef struct DATASTR KEY;

void deleteChar(KEY**,char);/*Deleting a character from the file*/
void replaceChar(KEY*,char,char);/*Replace a character by another*/
void replaceFifth(KEY*,char);/*Replace every fifth position character*/
int main()
{
    char s[FILENAME_MAX];/*File name*/
    FILE *f;/*file pointer pointing to source file*/
    int c,c1,c2,i;/*Buffer variables*/
    KEY *data,*ini; /*data is the pointer to the current link of the linked list
                      ini is the pointer pointing to the first link of the list*/
    /*Do the tasks sequentially*/
    for(i=0;i<4;i++)
        switch(i){
        case 0:
            /*Get source file*/
            printf("Input source file name:");
            scanf("%s",s);
            f=fopen(s,"r+");
            if(f==NULL){
                printf("Error opening file");
                return 0;
            }else{
                /*Building up the linked list and assign data to each link*/
                data=(KEY*)malloc(sizeof(KEY));
                ini=data;
                while(!feof(f)){
                    c=getc(f);
                    data->c=c;
                    data->n=(KEY*)malloc(sizeof(KEY));
                    if(c!=EOF)
                        data=data->n;
                    else
                        data->n=ini;
                }
                data=ini;
            }
            fclose(f);
            break;
        case 1:
            /*Getting the character to be deleted*/
            printf("Input a character to be deleted:");
            while((c1=getchar())=='\n');
            /*Delete it*/
            deleteChar(&data,c1);
            break;
        case 2:
            /*Getting the characters to be replaced*/
            printf("Input a character to be replaced:");
            while((c1=getchar())=='\n');
            printf("Input a character to replace:");
            while((c2=getchar())=='\n');
            /*Replace them*/
            replaceChar(data,c1,c2);
            break;
        case 3:
            /*Getting the character to replace every fifth character*/
            printf("Input a character to replace every fifth position:");
            while((c1=getchar())=='\n');
            /*replace it*/
            replaceFifth(data,c1);
            break;
        }
    /*Outputting the modified data to the file*/
    f=fopen(s,"w");
    while(data->c!=EOF){
        putc(data->c,f);
        data=data->n;
    }
    /*save file*/
    fclose(f);
    return 0;
}
/*Deleting the character contained in r in the linked list data*/
/*data is the address of the entrance of the linked list*/
void deleteChar(KEY** data,char r)/*Deleting a character from the file*/
{
    KEY *i,*j,*ini;
    /*ini contains the new entrance of the linked list after replacements*/
    /*i and j are used for iterating through the linked list*/
    /*Making sure the entrance link does not contain the character needed to be replaced*/
    for(i=*data,ini=*data;i->c==r;i=i->n,ini=i);
    /*Iterating through the list and delete the character*/
    while(i->c!=EOF){
        /*If the next character is the character needed to be replace,
        then let this character point to the next link of the next link,
        repeat until the next link does not contain the character needed to be replaced*/
        for(j=i;j->n->c==r;j->n=j->n->n);
        /*Move forward along characters not needed to be replaced*/
        i=j->n;
    }
    i->n=ini;
    /*Setting the entrance of the linked list to the new entrace*/
    *data=ini;
}
/*Replacing the character in every link contained data equal to r by character b. Entrance to the linked list contains in pointer data*/
void replaceChar(KEY* data,char r,char b){
    KEY *i;
    /*i is used for iterating through the linked list*/
    i=data;
    /*Iterating through each link of the list and replace the data of the link if it equal to r by the character
    contained in b*/
    while(i->c!=EOF){
        /*Equal to r*/
        if(i->c==r)
            /*replace*/
            i->c=b;
        /*Move forward*/
        i=i->n;
    }
}
/*Replacing data in every fifth link by r. Entrance to the linked list contains in pointer data*/
void replaceFifth(KEY* data,char r){
    KEY *i;
    int pos=1;
    /*i is used for iterating through the linked list*/
    /*pos is used for keeping track of the position of the link*/
    i=data;
    while(i->c!=EOF){
        /*At every fifth link*/
        if(pos++%5==0)
            /*replace*/
            i->c=r;
        /*move to the next character*/
        i=i->n;
    }
}

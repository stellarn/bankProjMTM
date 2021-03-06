/* bank project
 Yahel Tsufim 304952898
 Noam Stolero 201581683
 file:bankIO.h
 */

#include "bank.h"
#include <ctype.h>

#define MASK_ODD	85
#define MASK_EVEN	170

FILE* target;


/*		GENERAL FUNCTIONS FOR CLIENT PRINT		*/

clientString* clientToString(client*);


clientString* copyClientStings(clientString*);


void deleteClientString(clientString* del);


/****************COMPRESS FILE FUNCTIONS DECLERATION****************/

/* compress and print a clientString tree */
void printCmpr(clientString *c);

/* comprss a name */
char* nameCmpr(char* str);

/* compress 8 bits into 5 bit char */
void compressor(char* str, char* cmpr);

/* convert  char into numric value */
char* charToNum(char* str);

/*************DECOMPRESS FILE FUNCTIONS DECLERATION****************/

/* convert numric value into char */
char* numToChar(char* str);


genTree* readDecmp (char*);


void decompressChar (char*decmp, char* str,int len);


char* nameDeCmpr(char* str,char len);



/************ENCRYPT / DECRYPT FILE	FUNCTION DECLERATION***********/
/*read a binary file into a string. return the string (char*) and the length of the file in bytes*/
char* readBinaryFile(FILE* file,long* byteSize);

/*encode / decode a given char string*/
char* textEncDec(char* text,long textSize);

/* encrypt / decipher the received char and return an encrypted / deciphered char */
char charEncDec(char* a);

/* receive a char array, length and a file. writing the array to the file. */
void writeBinaryFile(FILE* output,char* text,long length);


/************************		BEGINING OF CODE		*******************/

/************GLOBAL FILE* VERIABLE FUNCTIONS*************/

/* open file for read or write purposes. */
try openFile(char*path ,char* restrictions){

    if (target) /*if file was already open close it */
        closeFile();

    target=fopen(path, restrictions);   /* open file with relevant restrictions */
    if (target==NULL)
        return FAIL;
    return SUCCESS;
}

/*	lose a file	*/
void closeFile(){
    if(target)/*if file was not opened do nothing*/
        fclose(target);
    target=NULL;
}

/**********		PRINT CLIENTS TO FILE FUNCTIONS		**************/

/* convert a client struct to a clientString struct */
clientString* clientToString(client* c){
    clientString *cString=NULL;
    cString = ALLOC(clientString, 1); /* allocate a new client String struct.*/
    
    if (!c)
        return NULL;
    
    
    /* copy relevant fields */
    cString->familyName =str_dup(c->surname);
    strcpy(cString->clientID,c->cID);
    cString->clientAcc=c->accNum;
    return cString;
}


/* copy clientString to a another */
clientString* copyClientStings(clientString* source){
    clientString *target=NULL;
    if (!source)
        return NULL;
    target = ALLOC(clientString, 1); /* allocate a new client String struct.*/
    
    /* copy relevant fields */
    target->clientAcc=source->clientAcc;
    strcpy(target->clientID,source->clientID);
    target->familyName=str_dup(source->familyName);
    return target;
}


void printClientString(clientString* cs)
{
    fprintf(target,"%s %s %d\n",cs->familyName,cs->clientID,cs->clientAcc);
    
}


void printClientToFile(client* cs)
{
    int i;
    for (i=0; cs->surname[i]!='\0'; i++){
        cs->surname[i]=toupper( cs->surname[i]);
    }
    fprintf(target,"%s %s %d\n",cs->surname,cs->cID,cs->accNum);
}



/***********	SORT FILE FUNCTIONS		*******/

char* sortBySurename(char fileName[FILENAMESIZE])
{
    genTree* t = NULL;
    openFile(fileName, "r");
    t = readClientFromFile();
    strcat(fileName, ".sort");
    openFile(fileName, "w+");

    print_tree(t, (genPrint)(&printClientString));
    free_list(&t, (genDelete)&deleteClientString);
    closeFile();
    return fileName;

}

genTree* readClientFromFile()
{
    genTree* fileData=NULL;
    clientString newClient,*readclient;
    char surname[20],id[20];
    while((fscanf(target,"%s %s %d",surname,id, &newClient.clientAcc))>0){
        strcpy(newClient.clientID,id);
        newClient.familyName=surname;
        readclient = copyClientStings(&newClient);
        fileData = add_new_node(fileData,readclient,(genCmp)(&compareClientSurname));

    }
    return fileData;
}


comparison compareClientSurname(clientString* a,clientString* b)
{
    int res;
    res = strcmp(a->familyName,b->familyName);
    if(res>0)
        return GREATER;
    else if(res<0)
        return SMALLER;
    return EQUAL;
}

void deleteClientString(clientString* del)
{
    FREE(del->familyName);
    FREE(del);
}



/******************* COMPRESSION FUNCTIONS ******************/


/* convert  char into numric value */
char* charToNum(char* str)
{
    char* head = str;
    while(*str!='\0')
    {
        /* convert the letter into a number */
        *str = toupper(*str);
        *str = (*str - 'A' + 2);
        str++;
    }
    return head;
}


/* compress 8 bits into 5 bit char */
void compressor(char* str, char* cmpr){
    /* function recieve standrd char with 
     5bits info. compress it into chars with
     consecutive 5bits of info. */
    
    char mask=16, res = 0;
    int i=8, len=0;
    
    len=(int)strlen(str); /* number of chars to read */
    
    while (len--){
        
        for (mask=16; mask>0; mask>>=1){ /* check each bit from the 3rd till 0 */
            /* 8 bit counter */
            if (i==0){
                *(cmpr++)=res; /* save result and go to next char. */
                i=8;    /* reset bit counter */
                res=0;  /* reset result */
            }
            
            res<<=1;     /* each time a bit is read, shift res left */
            if ((*str & mask) !=0) /* if there is a bit to enter, the result wont be zero */
            {
                res |=1;    /* turn lsb */
            }
            i--;    /* dec bit counter */
        }
        str++;  /* next char to read */
    }
    /* make sure lest 5 bits move into place, and store them */
    if (i>0){
        res<<=i;
        *cmpr=res;
    }
}


/* comprss a name */
char* nameCmpr(char* str)
{
    char len, *cmpr=NULL;
    len = (char)strlen(str); /* get the max size for the compression variabl */
    cmpr=ALLOC(char, len);  /* will hold the compressed name */
    str = charToNum(str);   /* turn the chars into a 5bits number */
    cmpr++;
    compressor(str,cmpr);   /* compress str into cmpr */
    cmpr--;
    cmpr[0]=(len*5/8)+1;    /* first cell holds the size of the name in a full char */
    return cmpr;
}


/* compress and print a clientString tree */
void printCmpr(clientString *c)
{
    char *cmpr;
    short accNum;
    
    cmpr=nameCmpr(c->familyName);   /* compress surname */
    fwrite(cmpr, cmpr[0]+1, sizeof(char), target);  /* write the cmpressed name to file */
    FREE(cmpr);
    fwrite(c->clientID,CLIENTIDL,sizeof(char),target);  /* write the client id */
    accNum=(short)(c->clientAcc);   /* converte the account number into short */
    fwrite(&accNum, 1, sizeof(short), target); /* write the client acc */
}


/* compress a given file */
char* compressFile(char * fileName){
    
    genTree* t = NULL;
    openFile(fileName, "r");    /* open file to compress */
    t = readClientFromFile();   /* read data to a tree */
    
    strcat(fileName, ".cmpr");  /* create target file name */
    openFile(fileName, "w+");
    print_tree(t, (genPrint)(&printCmpr));  /* compress and print the tree */
    free_list(&t, (genDelete)&deleteClientString);  /* delete the tree */
    closeFile();
    
    return fileName;
}



/*************	ENCRYPT / DECRYPT FILE	FUNCTIONS	****************/


char* fileEncDec(char* fileName,char* addFileName)
{
	char* text=NULL;
	long fileSize=0;
	openFile(fileName, "r");
	text = readBinaryFile(target,&fileSize);
	text = textEncDec(text,fileSize);
	closeFile();
	strcat(fileName,addFileName);
	openFile(fileName,"w+");
	writeBinaryFile(target,text,fileSize);
	closeFile();
    FREE(text);
	return fileName;
}

/*read a binary file into a string. return the string (char*) and the length of the file in bytes*/
char* readBinaryFile(FILE* file,long* byteSize)
{
	char* text=NULL;
	fseek(file,0,SEEK_END);
	*byteSize = ftell(file);
	fseek(file,0,SEEK_SET);
	text = ALLOC(char,(int)*byteSize);
	fread(text,sizeof(char),*byteSize,file);
	return text;
}

/*encode / decode a given char string*/
char* textEncDec(char* text,long textSize)
{
	long i=0;
	for(i=0;i<textSize;i++){
		text[i] = charEncDec(text+i);
	}
	return text;
}

/* encrypt / decipher the received char and return an encrypted / deciphered char */
char charEncDec(char* a)
{
	char encrypt=0;
	char odd,even;
	odd = (*a & MASK_ODD) << 1;
	even = (*a & MASK_EVEN) >> 1;
	encrypt =  odd | even;
	return encrypt;
}

/* receive a char array, length and a file. writing the array to the file. */
void writeBinaryFile(FILE* output,char* text,long length)
{
	fwrite(text,sizeof(char),length,output);
}


/******************* DECOMPRESSION FUNCTIONS ******************/



char * decompressFile( char *fileName){
    genTree *t=NULL;
    char *decFile=NULL;
    decFile = fileEncDec(fileName,".dec");

    t=readDecmp(decFile);
    openFile(fileName, "w+");
    print_tree(t, (genPrint)(&printClientString));  /* compress and print the tree */
    free_list(&t, (genDelete)&deleteClientString);  /* delete the tree */
    closeFile();
    return fileName;
}


/* read a compressed file into a tree */
genTree* readDecmp (char* filename)
{
    clientString c ;
    genTree* t = NULL;
    char nameCmpr[MAXNAME], len=0;
    long maxBytes;

    openFile(filename, "r");    /* opend compressed file */
    fseek(target, 0, SEEK_END); /* get the number of bytes to read */
    maxBytes=ftell(target);
    fseek(target, 0, SEEK_SET); /* reset the seek pointer */

    while(maxBytes)
    {
        fread(&len, sizeof(char), 1, target); /* first read how many bytes contain the surname */
        maxBytes--;
        fread(nameCmpr,sizeof(char),len,target);    /* read the name chars */
        maxBytes-=len;
        c.familyName=nameDeCmpr(nameCmpr, len); /* decompress the name */
        fread(&c.clientID,sizeof(char),CLIENTIDL,target); /* read the client id */
        maxBytes-=CLIENTIDL;
        fread(&c.clientAcc,sizeof(short),1,target); /* read acc num */
        maxBytes-=sizeof(short);
        t=add_new_node(t, copyClientStings(&c), (genCmp)&compareClientSurname);   /* add the new created client string */
        FREE(c.familyName);
    }
    return t;
}


/* decompress a name */
char* nameDeCmpr(char* str,char len)
{
    char *decmp=NULL; /* will hold the decompressed name */
    decmp=ALLOC(char, len);

    decompressChar(decmp, str ,len); /* invoke decompression fumction */
    decmp =numToChar(decmp);    /* turn the numbers into chars */
    return decmp;
}

/* decompress a char 5 to 8 bits */
void decompressChar (char*decmp, char* str,int len){

    /* function receive chars holding
     consecutive 5bits of info. divide
     these bits into standrd 8bits char */
    unsigned char mask=128;
    char res=0;
    int i=5;

    /* len represent number of chars to read from */
    while (len--){
        for(mask=128; mask>0; mask>>=1){    /* check each bit from the 4th til 0 */

            if (i==0) /* if 5 bits read, use next char */
            {
                *(decmp++)=res; /* save result and go to next char. */
                i=5;    /* reset 5bit counter */
                res=0;  /* reset result char */
            }

            res<<=1;    /* each time a bit is read, shift res left */
            if((mask & *str) >0){   /* if there is a bit to enter, the result will be bigger zero */
                res|=1; /* turn lsb */
            }
            i--;
        }
        str++; /* next compressed char */
    }
    *decmp='\0';    /*end the string properly */

}


/* convert numric value into char */
char* numToChar(char* str)
{
    char* head = str;
    while(*str!='\0')
    {
        /* get the right letter */
        *str = (*str + 'A' - 2);
        str++;
    }
    return head;
}

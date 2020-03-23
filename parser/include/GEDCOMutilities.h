#ifndef GEDCOMUTILITIES_H
#define GEDCOMUTILITIES_H
#include "GEDCOMparser.h"


typedef struct{
    int lineNumber;
    char tag[31];
    char info[255];
}GEDCOMtag;
typedef enum Zero{
    HEAD = 1,
    SUBMM = 2,
    INDIV = 3,
    FAMILY = 4, 
    TRLR = 5,
}Zero;
List EVENT_GARBAGE;

typedef struct{
    int version;
    int eventFlag;
    int placFlag;
    int place;
    int date;
    bool famDate;
    bool famPlace;
    int indiCount;
    bool errorHeader;
    bool errorRecord;
    int eventCounter;
    Event *event;
}Checker;
typedef struct{
    char *tag;
    char * xref;
    char *reference;
    Family *family;
}ReferenceFamily;
typedef struct{
    int encoding;
    int version;
    int source;
    int submitterReference;
    bool name;
    bool address;
    bool individual;
    
   
}Flag;
typedef struct{

   char *tag;
   char * xref;
   char *reference; 
   Individual *individual;

}ReferenceIndividual;

typedef struct{

    List individuals;
    List families;
}StoreList;


typedef struct{
	
	int level;
	Individual * ptr;
	
}Descendant;
Individual *createIndividual();

Event *createEvent(char type[],char *place,char *date);


Family  *createFamily();

Field *createField(char *tag,char *value);


char *toLower(char string[]);

Submitter *createSubmitter(char *submitterName, char *address);


int getMonth(char *string);

int getYear(int year);

GEDCOMerror createGEDCOMerror(ErrorCode type,int line);

Header *createHeader(char source[],char *submitterName,char *address);

GEDCOMobject* createGEDCOMobject(char source[],char *submitterName,char *address);

void deleteSubmitter(void *toBeDeleted);

void deleteHeader(void *toBeDeleted);

int validateHeader(char *string);

int validateSubmitter(char *string);

int validateIndivdiual(char *string);

int validateFamily(char *string);

int checkTagLength(char *string);

int checkGEDCOMLine(char *string);

void setCharEncoding(Header *header,CharSet encoding);

void setSource(Header *header,char source[]);

void setGEDCVersion(Header*header,float gedcVersion);

CharSet convertStringToEncoding(char *string);

int checkFirstDigit(char *string);

GEDCOMerror parseHeader(GEDCOMtag tag[],int counter,GEDCOMobject **obj,Checker *check,Flag *flag);

int checkPointer(char *string);

void setName(char name[],Submitter *submitter);

GEDCOMerror parseSubmitter(GEDCOMtag gedcTag[],int counter,GEDCOMobject **obj,Flag *flag);

GEDCOMerror parseIndividual(GEDCOMtag gedcTag[], int counter, GEDCOMobject**obj,Checker* check,Individual *individual,StoreList *sl);

void setAddress(char address[],Submitter *submitter);

void setGivenName(char *givenName,Individual *individual);

void setSurname(char *surname,Individual*individual);


bool checkBothDigits(char *string);

bool lineNumberCheck(GEDCOMtag gedcTag[],int counter);

int compareAddress(const void *first,const void *second);

void deleteAddress(void *toBeDeleted);

char *printAddress(void *toBePrinted);

ReferenceIndividual * createIndividualReference(char reference[],char source[]);

ReferenceFamily *createFamilyReference(char reference[],char source[]);

GEDCOMerror parseFamily(GEDCOMtag gedcTag[],int counter,GEDCOMobject**obj,Checker *check,Family *family,StoreList *sl);
int compareReferencesForIndivdual(const void *first,const void *second);

int compareReferences(const void *first,const void *second);
bool compareReferencesForFamily(const void*first,const void *second);
int compareAddress(const void *first,const void *second);

StoreList * storeListFunc();

bool checkLowerCase(char *string);

bool customCompare(const void *first,const void *second);
void deleteStoreList(void *toBeDeleted);
char *
myfgets(char *dst, int max, FILE *fp);

void *getFamily(Individual *individual);

bool cmp(const void *first,const void *second);

void deleteStoreFamily(void * toBeDeleted);

void deleteStoreIndividual(void *toBeDeleted);

void fakeDelete(void * toBeDeleted);
void wrapperInsert(Event * event);



char * convertEncodingToString(CharSet type);


char *writeHeader(const GEDCOMobject * obj);

char *writeSubmitter(const GEDCOMobject *obj);

char * writeIndividual(const GEDCOMobject * obj,StoreList * sl);

char * writeFamily(const GEDCOMobject *obj , StoreList *sl);

ReferenceIndividual * storeIndividual();

ReferenceFamily * storeFamily();

Header * createHeader2();

Submitter * createSubmitter2();

GEDCOMobject * createGEDCOMobj();

List wrapperGetDescendantN(List * des2,const Individual *person,unsigned int maxGen);

List wrapperGetAncestorN(List * des2,const Individual *person,unsigned int maxGen);
Descendant * createDescendant();

int compareLevels(const void *a,const void*b);

int comparePointers(const void *a,const void*b);

int compareLists(const void *a,const void *b);

void writeToGEDCOM(char * fileName,char * subName,char *subAddress,char * encoding);


char * parseValidGEDCOM(char * filename);

char *IndiFamGEDCOMtoJSON(char * fileName,GEDCOMobject * obj);

char *sexAndFamilyIndToGEDCOM(Individual * indi);

char *iListToJSONWithSexAndFamilySize(List iList);

char *parseIndividualGEDCOM(char * fileName);

void addIndividualWrapper(char * filename, char * givenName,char * surname);

char *iListToJSONGivenAndSurn(char * fileName,List iList);

char * fileNameAndIndiJSON(char * fileName,Individual * indi);
char * fileNameAndIndWrapper(char * fileName);

char * wrappergListToJSON(char * filename,unsigned int maxGen,char *givenName,char * surname);

char * wrappergListToJSONAncestors(char * filename,unsigned int maxGen,char *givenName,char * surname);

char *parseIndividualGEDCOM2(char * filename);

char * iListToJSONWithSexAndFamilySize2(List iList);
char *sexAndFamilyIndToGEDCOM2(Individual * indi);


#endif 




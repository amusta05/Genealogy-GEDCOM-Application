#include <stdlib.h>

#include <stdio.h>

#include "LinkedListAPI.h"

#include <string.h>

#include <ctype.h>

#include "GEDCOMutilities.h"

#include "GEDCOMparser.h"


Individual * createIndividual() {

    Individual * individual = calloc(1, sizeof(Individual));
    individual -> givenName = calloc(1, sizeof(char) * 100);
    individual -> surname = calloc(1, sizeof(char) * 100);
    individual -> events = initializeList(printEvent, fakeDelete, compareEvents);
    individual -> families = initializeList(printFamily, fakeDelete, compareFamilies);
    individual -> otherFields = initializeList(printField, deleteField, compareFields);
    return individual;

}
Event * createEvent(char type[], char * place, char * date) {
    Event * event = calloc(1, sizeof(Event));
    strcpy(event -> type, type);
    if (strlen(date) == 0) {
        event -> date = calloc(1, sizeof(char) * strlen(date) + 2);
        event -> date = "";
        strcpy(event -> date, date);
    } else {
        event -> date = calloc(1, sizeof(char) * strlen(date) + 2);
        strcpy(event -> date, date);
    }
    if (strlen(place) == 0) {
        event -> place = calloc(1, sizeof(char) * strlen(place) + 2);
        event -> place = "";
        strcpy(event -> place, place);
    } else {
        event -> place = calloc(1, sizeof(char) * strlen(place) + 2);
        strcpy(event -> place, place);
    }
    event -> otherFields = initializeList(printField, deleteField, compareFields);
    return event;
}
Family * createFamily() {
    Family * family = calloc(1, sizeof(Family));

    family -> wife = NULL;
    family -> husband = NULL;
    family -> events = initializeList(printEvent, deleteEvent, compareEvents);
    family -> children = initializeList(printIndividual, fakeDelete, compareIndividuals);
    family -> otherFields = initializeList(printField, deleteField, compareFields);
    return family;
}

Field * createField(char * tag, char * value) {
    if (tag == NULL || strlen(tag) == 0) {
        return NULL;
    }
    if (value == NULL || strlen(value) == 0) {
        return NULL;
    }
    Field * field = calloc(1, sizeof(Field));
    if (tag != NULL) {
        field -> tag = calloc(1, strlen(tag) + 20);
    }
    strcpy(field -> tag, tag);
    if (value != NULL) {
        field -> value = calloc(1, strlen(value) + 20);
    }
    strcpy(field -> value, value);

    return field;
}

char * toLower(char string[]) {
    int i;
    for (i = 0; i < strlen(string); i++) {
        string[i] = tolower(string[i]);
    }
    return string;
}

Submitter * createSubmitter(char * submitterName, char * address) {
    Submitter * submitter = calloc(1, sizeof(Submitter) + strlen(address) + 100);
    strcpy(submitter -> submitterName, submitterName);
    strcpy(submitter -> address, address);
    submitter -> otherFields = initializeList(printField, deleteField, compareFields);
    return submitter;
}

int getMonth(char * string) {

    if (strcmp(string, "Jan") == 0) {
        return 31;
    } else if (strcmp(string, "Feb") == 0) {
        return 59;
    } else if (strcmp(string, "Mar") == 0) {
        return 90;
    } else if (strcmp(string, "Apr") == 0) {
        return 120;
    } else if (strcmp(string, "May") == 0) {
        return 151;
    } else if (strcmp(string, "Jun") == 0) {
        return 181;
    } else if (strcmp(string, "Jul") == 0) {
        return 212;
    } else if (strcmp(string, "Aug") == 0) {
        return 243;
    } else if (strcmp(string, "Sept") == 0) {
        return 273;
    } else if (strcmp(string, "Oct") == 0) {
        return 304;
    } else if (strcmp(string, "Nov") == 0) {
        return 334;
    } else if (strcmp(string, "Dec") == 0) {
        return 365;
    }
    return 0;
}
int getYear(int year) {
    return year * 365;
}
GEDCOMerror createGEDCOMerror(ErrorCode type, int line) {
    GEDCOMerror error;
    error.type = type;
    error.line = line;
    return error;
}
GEDCOMobject * createGEDCOMobject(char source[], char * submitterName, char * address) {
    GEDCOMobject * object = calloc(1, sizeof(GEDCOMobject));
    object -> header = createHeader(source, submitterName, address);
    object -> families = initializeList(printFamily, deleteFamily, compareFamilies);
    object -> individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    object -> submitter = createSubmitter(submitterName, address);
    return object;
}

Header * createHeader(char source[], char * submitterName, char * address) {
    Header * header = calloc(1, sizeof(Header));
    strcpy(header -> source, source);
    header -> gedcVersion = 0.0;
    header -> encoding = ASCII;
    header -> submitter = NULL;
    header -> otherFields = initializeList(printField, deleteField, compareFields);
    return header;

}

void deleteSubmitter(void * toBeDeleted) {
    Submitter * temp;
    if (toBeDeleted == NULL) {

        return;
    }
    temp = (Submitter * ) toBeDeleted;;
    if (strlen(temp -> address) != 0) {
        free(temp -> address);
    }
    clearList( & (temp -> otherFields));
    free(temp);

}
void deleteHeader(void * toBeDeleted) {
    Header * temp = NULL;
    if (toBeDeleted == NULL) {
        return;
    }
    temp = (Header * ) toBeDeleted;
    clearList( & (temp -> otherFields));
    free(temp);
}

int validateHeader(char * string) {
    char * str[] = {
        "HEAD",
        "SOUR",
        "VERS",
        "FILE",
        "FORM",
        "GEDC",
        "NAME",
        "CHAR",
        "CORP",
        "CONT",
        "CONC",
        "CROP",
        "DATE",
        "FORM",
        "DATA",
        "LANG",
        "NOTE",
        "CHAR",
        "PLAC",
        "TIME"
    };

    int i;
    int row = sizeof(str) / sizeof(str[0]);

    for (i = 0; i < row; i++) {
        if (strcmp(string, str[i]) == 0) {
            return 0;
        }
    }
    return -1;
}

int validateSubmitter(char * string) {

    char * str[] = {
        "SUBM",
        "NAME",
        "CONC",
        "CONT",
        "FAMF",
        "TEMP",
        "ANCE",
        "DESC",
        "ORDI",
        "RIN"
    };
    int i;
    int row = sizeof(str) / sizeof(str[0]);

    for (i = 0; i < row; i++) {
        if (strcmp(string, str[i]) == 0) {
            return 0;
        }
    }
    return -1;
}
int validateIndivdiual(char * string) {

    char * str[] = {
        "NAME",
        "GIVN",
        "SURN",
        "SEX",
        "BIRT",
        "DATE",
        "FAMC",
        "FAMS",
        "DEAT",
        "ALIA",
        "ANCI",
        "DESI",
        "RFN",
        "AFN",
        "REFN",
        "RIN",
        "CHR",
        "ADOP",
        "BURI",
        "BAPM",
        "CHRA",
        "NATU",
        "CENS",
        "GRAD"
    };
    int i;
    int row = sizeof(str) / sizeof(str[0]);

    for (i = 0; i < row; i++) {
        if (strcmp(string, str[i]) == 0) {
            return 0;
        }
    }
    return -1;
}
int validateFamily(char * string) {
    char * str[] = {
        "HUSB",
        "WIFE",
        "ANUL",
        "CNES",
        "DIV",
        "DIVF",
        "ENGA",
        "MARB",
        "MARC"
    };
    int i;
    int row = sizeof(str) / sizeof(str[0]);

    for (i = 0; i < row; i++) {
        if (strcmp(string, str[i]) == 0) {
            return 0;
        }
    }
    return -1;
}

int checkTagLength(char * string) {

    if (strlen(string) > 31) {
        return 1;
    }
    return 0;
}

int checkGEDCOMLine(char * string) {
    if (strlen(string) > 255) {
        return 1;
    }
    return 0;

}
void setCharEncoding(Header * header, CharSet encoding) {
    header -> encoding = encoding;
}
void setSource(Header * header, char source[]) {
    strcpy(header -> source, source);
}
void setGEDCVersion(Header * header, float gedcVersion) {
    header -> gedcVersion = gedcVersion;
}
CharSet convertStringToEncoding(char * string) {
    if (strcmp(string, "UTF-8") == 0) {
        return UTF8;
    } else if (strcmp(string, "ASCII") == 0) {
        return ASCII;
    } else if (strcmp(string, "UNICODE") == 0) {
        return UNICODE;
    } else if (strcmp(string, "ANSEL") == 0) {
        return ANSEL;
    }
    return ASCII;
}
int checkFirstDigit(char * string) {
    if (isdigit(string[0])) {
        return 0;
    }

    return 1;
}
GEDCOMerror parseHeader(GEDCOMtag gedcTag[], int counter, GEDCOMobject ** obj, Checker * check, Flag * f) {

    if (gedcTag == NULL || counter == -1) {
        return createGEDCOMerror(OTHER_ERROR, -1);
    }
    if (counter >= 1) {

        bool lineCheck = lineNumberCheck(gedcTag, counter);

        if (lineCheck == false) {

            return createGEDCOMerror(INV_RECORD, counter + 1);
        }
    }

    

    if (isdigit(gedcTag[counter].lineNumber)) {

        return createGEDCOMerror(INV_HEADER, counter);
    }

    if (strlen(gedcTag[counter].tag) == 0) {

        return createGEDCOMerror(INV_HEADER, counter);
    }

    

    Field * tempField = NULL;
    
    gedcTag[counter].info[strlen(gedcTag[counter].info) - 1] = '\0';

    if (gedcTag[counter].lineNumber == 1) {
        if (strcmp(gedcTag[counter].tag, "SOUR") == 0) {

            if (strlen(gedcTag[counter].info) == 0) {

                return createGEDCOMerror(INV_HEADER, counter);
            }
            setSource(( * obj) -> header, gedcTag[counter].info);
            f -> source = 1;

        } else if (strcmp(gedcTag[counter].tag, "CHAR") == 0) {
            if (strlen(gedcTag[counter].info) == 0) {

                return createGEDCOMerror(INV_HEADER, counter);

            }
           
            CharSet encoding = convertStringToEncoding(gedcTag[counter].info);
            setCharEncoding(( * obj) -> header, encoding);

            f -> encoding = 1;
        } else if (strcmp(gedcTag[counter].tag, "GEDC") == 0) {

            check -> version = 1;

        } else if (strcmp(gedcTag[counter].tag, "FILE") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "DATE") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "DEST") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);

        } else if (strcmp(gedcTag[counter].tag, "COPR") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);

        } else if (strcmp(gedcTag[counter].tag, "LANG") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "NOTE") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "SUBM") == 0) {

            f -> submitterReference = 1;
            
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "PLAC") == 0) {
            check -> placFlag = 1;
        } else {
            
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> header -> otherFields, tempField);
        }

    }

    if (gedcTag[counter].lineNumber == 2) {
        if (check -> version == 1) {

            if (strcmp(gedcTag[counter].tag, "VERS") == 0) {

                if (strlen(gedcTag[counter].info) == 0) {

                    return createGEDCOMerror(INV_HEADER, counter);

                }

               
                float gedcVersion = 0.0;
                
                

                gedcVersion = atof(gedcTag[counter].info);
                setGEDCVersion(( * obj) -> header, gedcVersion);
                f -> version = 1;

            }
        } else if (check -> version == 0 && strcmp(gedcTag[counter].tag, "VERS") == 0) {

            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "FORM") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "FORM") == 0 && check -> placFlag == 1) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else if (strcmp(gedcTag[counter].tag, "NAME") == 0) {
        
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & (( * obj) -> header -> otherFields), tempField);
            

        } else if (strcmp(gedcTag[counter].tag, "CORP") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
            
        } else if (strcmp(gedcTag[counter].tag, "DATA") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
            
        } else if (strcmp(gedcTag[counter].tag, "TIME") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
          
        } else if (strcmp(gedcTag[counter].tag, "FORM") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> header -> otherFields, tempField);
        } else {
           
            if (strlen(gedcTag[counter].info) == 0) {

                return createGEDCOMerror(INV_HEADER, counter);
            }
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> header -> otherFields, tempField);
            
        }
    }
    if (gedcTag[counter].lineNumber >= 3) {
        if (strlen(gedcTag[counter].info) == 0) {

            return createGEDCOMerror(INV_HEADER, counter);
        }

        tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
        insertBack( & ( * obj) -> header -> otherFields, tempField);
    }

    
    return createGEDCOMerror(OK, -1);
}

GEDCOMerror parseSubmitter(GEDCOMtag gedcTag[], int counter, GEDCOMobject ** obj, Flag * f) {

    bool lower = checkLowerCase(gedcTag[counter].tag);
    if (lower == false) {
        return createGEDCOMerror(INV_GEDCOM, -1);
    }
    if (gedcTag == NULL || counter == -1) {
        return createGEDCOMerror(OTHER_ERROR, -1);
    }
    if (isdigit(gedcTag[counter].lineNumber)) {

        return createGEDCOMerror(INV_HEADER, counter);
    }

    if (strlen(gedcTag[counter].tag) == 0) {

        return createGEDCOMerror(INV_HEADER, counter);
    }
    if (counter >= 1) {
        bool lineCheck = lineNumberCheck(gedcTag, counter);

        if (lineCheck == false) {

            return createGEDCOMerror(INV_RECORD, counter + 1);
        }
    }
    Field * tempField = NULL;

    if (gedcTag[counter].lineNumber == 1) {
        if (strcmp(gedcTag[counter].tag, "NAME") == 0) {
            char buffer[200];
            strcpy(buffer, gedcTag[counter].info);
            buffer[strlen(buffer) - 1] = '\0';
           

            if (strlen(gedcTag[counter].info) == 0)

            {
                return createGEDCOMerror(INV_HEADER, counter);

            }

            setName(buffer, ( * obj) -> submitter);
            f -> name = true;
        }
        if (strcmp(gedcTag[counter].tag, "ADDR") == 0) {

            if (strlen(gedcTag[counter].info) == 0) {

                return createGEDCOMerror(INV_HEADER, counter);

            }
            
            setAddress(gedcTag[counter].info, ( * obj) -> submitter);
            ( * obj) -> submitter -> address[strlen(( * obj) -> submitter -> address) - 1] = '\0';
            
            f -> address = true;
        }
        if (strcmp(gedcTag[counter].tag, "FAMF") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> submitter -> otherFields, tempField);

        }
        if (strcmp(gedcTag[counter].tag, "ANCE") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> submitter -> otherFields, tempField);

        }
        if (strcmp(gedcTag[counter].tag, "DESC") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> submitter -> otherFields, tempField);

        }
        if (strcmp(gedcTag[counter].tag, "ORDI") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & ( * obj) -> submitter -> otherFields, tempField);

        }
        if (strcmp(gedcTag[counter].tag, "RIN") == 0) {
            tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);

            insertBack( & ( * obj) -> submitter -> otherFields, tempField);

        }
    } else {

        if (strlen(gedcTag[counter].info) == 0) {

            return createGEDCOMerror(INV_HEADER, counter);

        }

        tempField = createField(gedcTag[counter].tag, gedcTag[counter].info);
        insertBack( & ( * obj) -> submitter -> otherFields, tempField);

    }

    return createGEDCOMerror(OK, -1);
}
int checkPointer(char * string) {
    if (string[0] == '@') {
        if (string[strlen(string) - 1] == '@' || string[strlen(string) - 2] == '@' || string[strlen(string) - 3] == '@')
            return 0;
    }
    return 1;

}
void setName(char name[], Submitter * submitter) {
    strcpy(submitter -> submitterName, name);
}
void setAddress(char address[], Submitter * submitter) {
    strcpy(submitter -> address, address);
}
void setGivenName(char * givenName, Individual * individual) {
    strcpy(individual -> givenName, givenName);
}
void setSurname(char * surname, Individual * individual) {

    strcpy(individual -> surname, surname);
}

GEDCOMerror parseIndividual(GEDCOMtag gedcTag[], int counter, GEDCOMobject ** obj, Checker * check, Individual * individual, StoreList * sl) {


    char * token = NULL;
    Field * temp = NULL;

    if (gedcTag[counter].lineNumber == 1) {
        if (strcmp(gedcTag[counter].tag, "NAME") == 0) {

            if (strlen(gedcTag[counter].info) == 0) {

                return createGEDCOMerror(INV_RECORD, counter);

            }
            char * temp = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
            strcpy(temp, gedcTag[counter].info);

            Individual * s1 = getFromBack(( * obj) -> individuals);

            if (strlen(temp) == 0) {
                strcpy(temp, "");
                setGivenName(temp, individual);
                setSurname(temp, individual);

            } else {

                token = strtok(temp, " /");
                
                if (token == NULL) {

                    token = calloc(1, sizeof(char) * 5);
                    strcpy(token, " ");
                    
                    strcpy(s1 -> givenName, token);
                   

                } else {
                    if (strcmp(token, "Unknown") == 0) //I regret this.
                    {
                        strcpy(s1 -> givenName, "");

                    } else {
                        strcpy(s1 -> givenName, token);
                    }

                }


                token = strtok(NULL, " /");

                if (token != NULL) {
                    
                    strcpy(s1 -> surname, token);
                     

                } else {
                    token = calloc(1, sizeof(char) * 5);
                    strcpy(token, " ");
                   
                    strcpy(s1 -> surname, token);
                   
                    free(token);
                }

            }

            //free(token);
            free(temp);

        } else if (!strcmp(gedcTag[counter].tag, "BIRT") || !strcmp(gedcTag[counter].tag, "CHR") || !strcmp(gedcTag[counter].tag, "DEAT") || !strcmp(gedcTag[counter].tag, "BURI") || !strcmp(gedcTag[counter].tag, "CREM") || !strcmp(gedcTag[counter].tag, "ADOP") || !strcmp(gedcTag[counter].tag, "BAPM") || !strcmp(gedcTag[counter].tag, "BARM") || !strcmp(gedcTag[counter].tag, "BASM") || !strcmp(gedcTag[counter].tag, "BLES") || !strcmp(gedcTag[counter].tag, "CHRA") || !strcmp(gedcTag[counter].tag, "CONF") || !strcmp(gedcTag[counter].tag, "FCOM") || !strcmp(gedcTag[counter].tag, "ORDN") || !strcmp(gedcTag[counter].tag, "NATU") || !strcmp(gedcTag[counter].tag, "EMIG") || !strcmp(gedcTag[counter].tag, "IMMI") || !strcmp(gedcTag[counter].tag, "CNES") || !strcmp(gedcTag[counter].tag, "PROB") || !strcmp(gedcTag[counter].tag, "WILL") || !strcmp(gedcTag[counter].tag, "GRAD") || !strcmp(gedcTag[counter].tag, "RETI") || !strcmp(gedcTag[counter].tag, "EVEN")) //LOL
        {


            if (check -> eventFlag == 1) {
                insertBack( & EVENT_GARBAGE, check -> event);
                insertBack( & individual -> events, check -> event);

                check -> eventFlag = 0;
                check -> place = 0;
                check -> date = 0;

            }

            Event * event = calloc(1, sizeof(Event));
            check -> event = event;
            check -> event -> otherFields = initializeList(printEvent, deleteEvent, compareEvents);
            strcpy(check -> event -> type, gedcTag[counter].tag);
            check -> eventFlag = 1;

        } else if (strcmp(gedcTag[counter].tag, "FAMS") == 0) {

            // do nothing

        } else if (strcmp(gedcTag[counter].tag, "FAMC") == 0) {

            // do nothing

        } else {
            char buffer[200];
            strcpy(buffer, gedcTag[counter].info);
            buffer[strlen(buffer) - 1] = '\0';
            temp = createField(gedcTag[counter].tag, buffer);
            insertBack( & individual -> otherFields, temp);

        }

    } else if (gedcTag[counter].lineNumber == 2) {

        if (check -> eventFlag == 1) {

            if (strcmp(gedcTag[counter].tag, "PLAC") == 0) {
                check -> event -> place = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
                strcpy(check -> event -> place, gedcTag[counter].info);
                check -> event -> place[strlen(check -> event -> place) - 1] = '\0';

                check -> place = 1;

            } else if (strcmp(gedcTag[counter].tag, "DATE") == 0) {
                //printf("Come hserer\n");
                check -> event -> date = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
                strcpy(check -> event -> date, gedcTag[counter].info);
                check -> event -> date[strlen(check -> event -> date) - 1] = '\0';
                check -> date = 1;

            }

        } else if (strcmp(gedcTag[counter].tag, "GIVN") == 0) {

            char * temp = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
            strcpy(temp, gedcTag[counter].info);

            Individual * s1 = getFromBack(( * obj) -> individuals);
            if (strlen(temp) == 0) {
                strcpy(temp, "");
                setGivenName(temp, s1);

            } else {

                setGivenName(temp, individual);
                s1 -> givenName[strlen(s1 -> givenName) - 1] = '\0';

            }
            free(temp);
        } else if (strcmp(gedcTag[counter].tag, "SURN") == 0) {

            char * temp = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
            strcpy(temp, gedcTag[counter].info);

            Individual * s1 = getFromBack(( * obj) -> individuals);
            if (strlen(temp) == 0) {
                strcpy(temp, "");
                setSurname(temp, s1);
                

            } else {

                setSurname(temp, individual);

                
                s1 -> surname[strlen(s1 -> surname) - 1] = '\0';

            }
            free(temp);
        } else {

            char buffer[200];
            strcpy(buffer, gedcTag[counter].info);
            buffer[strlen(buffer) - 1] = '\0';
            temp = createField(gedcTag[counter].tag, buffer);
            insertBack( & individual -> otherFields, temp);
        }

    } else {

        
        char buffer[200];
        strcpy(buffer, gedcTag[counter].info);
        buffer[strlen(buffer) - 1] = '\0';
        temp = createField(gedcTag[counter].tag, gedcTag[counter].info);
        insertBack( & individual -> otherFields, temp);

    }

    if ((check -> place == 1 || check -> place == 1) && !(gedcTag[counter].lineNumber == 2 && gedcTag[counter].lineNumber == 1)) {

        insertBack( & individual -> events, check -> event);
        check -> eventFlag = 0;
        check -> place = 0;
        check -> date = 0;

    }

    return createGEDCOMerror(OK, -1);

}
GEDCOMerror parseFamily(GEDCOMtag gedcTag[], int counter, GEDCOMobject ** obj, Checker * check, Family * family, StoreList * sl) {

    gedcTag[counter].info[strlen(gedcTag[counter].info) - 1] = '\0';
    bool lower = checkLowerCase(gedcTag[counter].tag);
    if (lower == false) {
        return createGEDCOMerror(INV_RECORD, counter);
    }
    if (counter == -1 || obj == NULL) {
        return createGEDCOMerror(OTHER_ERROR, -1);
    }
    ReferenceFamily * refFam = NULL;

    Field * temp;

    if (gedcTag[counter].lineNumber < 1) {
        return createGEDCOMerror(INV_RECORD, counter);
    }

    if (isdigit(gedcTag[counter].lineNumber)) {

        return createGEDCOMerror(INV_RECORD, counter);
    }

    if (strlen(gedcTag[counter].tag) == 0) {

        return createGEDCOMerror(INV_RECORD, counter);
    }
    if (counter >= 1) {
        bool lineCheck = lineNumberCheck(gedcTag, counter);

        if (lineCheck == false) {

            return createGEDCOMerror(INV_RECORD, counter);
        }
    }

    if (gedcTag[counter].lineNumber == 1) {

        if (strcmp(gedcTag[counter].tag, "HUSB") == 0) {

            int isPoint = checkPointer(gedcTag[counter].info);
            if (isPoint == 1) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            refFam = createFamilyReference(gedcTag[counter].info, gedcTag[counter].tag);

            refFam -> family = family;
            insertBack( & sl -> families, refFam);

        }
        else if (!strcmp("ANUL", gedcTag[counter].tag) || !strcmp("CNES", gedcTag[counter].tag) || !strcmp("DIV", gedcTag[counter].tag) || !strcmp("ENGA", gedcTag[counter].tag) || !strcmp("MARB", gedcTag[counter].tag) || !strcmp("MARC", gedcTag[counter].tag) || !strcmp("MARR", gedcTag[counter].tag) || !strcmp("MARL", gedcTag[counter].tag) || !strcmp("MARS", gedcTag[counter].tag)) {

            if (check -> eventFlag == 1) {

                insertBack( & family -> events, check -> event);
                check -> eventFlag = 0;
                check -> eventFlag = 0;
                check -> famPlace = false;
                check -> famDate = false;

            }
            check -> eventFlag = 1;
            check -> event = calloc(1, sizeof(Event));
            check -> event -> otherFields = initializeList(printEvent, deleteEvent, compareEvents);
            strcpy(check -> event -> type, gedcTag[counter].tag);

        } else if (strcmp(gedcTag[counter].tag, "WIFE") == 0) {

            int isPoint = checkPointer(gedcTag[counter].info);
            if (isPoint == 1) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }

            refFam = createFamilyReference(gedcTag[counter].info, gedcTag[counter].tag);

            refFam -> family = family;
            
            insertBack( & sl -> families, refFam);

        } else if (strcmp(gedcTag[counter].tag, "CHIL") == 0) {
            int isPoint = checkPointer(gedcTag[counter].info);
            if (isPoint == 1) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            refFam = createFamilyReference(gedcTag[counter].info, gedcTag[counter].tag);
            refFam -> family = family;
            insertBack( & sl -> families, refFam);

        }

    } else if (gedcTag[counter].lineNumber == 2) {

        if (check -> eventFlag == 1) {

            if (strcmp(gedcTag[counter].tag, "PLAC") == 0) {
                check -> event -> place = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
                strcpy(check -> event -> place, gedcTag[counter].info);
                check -> famPlace = true;

            } else if (strcmp(gedcTag[counter].tag, "DATE") == 0) {

                check -> event -> date = calloc(1, sizeof(char) * strlen(gedcTag[counter].info) + 1);
                strcpy(check -> event -> date, gedcTag[counter].info);
                check -> famDate = true;

            }

        } else {

            temp = createField(gedcTag[counter].tag, gedcTag[counter].info);
            insertBack( & family -> otherFields, temp);
        }
    }

    if ((check -> famPlace == true || check -> famDate == true) && !(gedcTag[counter].lineNumber == 2 && gedcTag[counter].lineNumber == 1)) {
        insertBack( & family -> events, check -> event);
        check -> eventFlag = 0;
        check -> famPlace = false;
        check -> famDate = 0;

    }

    return createGEDCOMerror(OK, -1);

}

bool checkBothDigits(char * string) {
    if (isdigit(string[0]) && isdigit(string[1])) {
        return false;
    }
    return true;
}
bool lineNumberCheck(GEDCOMtag gedcTag[], int counter) {
    if (gedcTag[counter].lineNumber == gedcTag[counter - 1].lineNumber || gedcTag[counter].lineNumber == gedcTag[counter - 1].lineNumber + 1 || gedcTag[counter].lineNumber == gedcTag[counter - 1].lineNumber - 1) {
        return true;
    }
    return false;
}
char * printAddress(void * toBePrinted) {
    //making the complier happy. This function is not neccessary.
    return NULL;
}
void deleteAddress(void * toBeDeleted) {
    //will come back to this later
}
ReferenceIndividual * createIndividualReference(char reference[], char source[]) {
    ReferenceIndividual * ref = calloc(1, sizeof(ReferenceIndividual));
    ref -> reference = calloc(1, sizeof(char) * 100);
    strcpy(ref -> reference, reference);

    ref -> tag = calloc(1, sizeof(char) * 100);
    strcpy(ref -> tag, source);
    ref -> individual = NULL;
    return ref;
}
ReferenceFamily * createFamilyReference(char reference[], char source[]) {
    ReferenceFamily * ref = calloc(1, sizeof(ReferenceFamily));
    ref -> reference = calloc(1, sizeof(char) * 100);
    strcpy(ref -> reference, reference);

    ref -> tag = calloc(1, sizeof(char) * 100);
    strcpy(ref -> tag, source);
    ref -> family = NULL;
    return ref;

}
int compareReferencesForIndivdual(const void * first,
    const void * second) {

    ReferenceIndividual * one = (ReferenceIndividual * ) first;
    ReferenceIndividual * two = (ReferenceIndividual * ) second;

    return strcmp(one -> reference, two -> reference);
}

bool compareReferencesForFamily(const void * first,
    const void * second) {
    ReferenceFamily * one = (ReferenceFamily * ) first;
    ReferenceFamily * two = (ReferenceFamily * ) second;

    return strcmp(one -> reference, two -> reference);
}
int compareAddress(const void * first,
    const void * second) {
    return 0;
}
StoreList * storeListFunc() {
    StoreList * sl = calloc(1, sizeof(StoreList));
    sl -> individuals = initializeList(printIndividual, deleteStoreIndividual, compareReferences);
    sl -> families = initializeList(printFamily, deleteStoreFamily, compareReferences);
    return sl;
}
int compareReferences(const void * first,
    const void * second) {

    ReferenceIndividual * indi = (ReferenceIndividual * ) first;
    ReferenceFamily * fam = (ReferenceFamily * ) second;

    if (strcmp(indi -> reference, fam -> reference) == 0) {
        return 0;
    } else if (strcmp(indi -> reference, fam -> reference) < 0) {
        return -1;
    } else {
        return 1;
    }
}
bool checkLowerCase(char * string) {

    int i = 0;

    for (i = 0; i < strlen(string); i++) {
        if (islower(string[i])) {
            return false;
        }
    }
    return true;
}
void deleteStoreList(void * toBeDeleted) {

    if (toBeDeleted == NULL) {
        return;
    }

    StoreList * s = (StoreList * ) toBeDeleted;
    clearList( & (s -> individuals));
    clearList( & (s -> families));
    free(s);
}
bool customCompare(const void * first,
    const void * second) {

    int x = compareIndividuals(first, second);

    if (x == 0) {
        return true;
    } else {
        return false;
    }

}
char *
    myfgets(char * dst, int max, FILE * fp) {
        int c;
        char * p;
        int temp;
        /* get max bytes or upto a newline */
        long int x = 0;
        //char temp; 
        for (p = dst, max--; max > 0; max--) {
            if ((c = fgetc(fp)) == EOF)
                break;
            * p++ = c;
            x = ftell(fp);
            temp = fgetc(fp);

            if (temp == EOF) {
                //fseek(fp,x,SEEK_SET);
                break;
            }
            if (c == '\n' && !(temp == '\r')) {
                fseek(fp, x, SEEK_SET);

                break;

            }
            if (c == '\r' && !(temp == '\n')) {
                fseek(fp, x, SEEK_SET);

                break;

            }

            fseek(fp, x, SEEK_SET);
        }

        * p = 0;
        if (p == dst || c == EOF)
            return NULL;
        return (p);
    }
void * getFamily(Individual * individual) {

    if (individual == NULL) {
        return NULL;
    }

    Family * fam = createFamily();
    fam -> husband = individual;
    fam -> wife = individual;

    void * data = findElement(individual -> families, customCompare, fam);

    return data;

}

void getFamilyBase(List list,
    const Individual * person) {

    Node * family = person -> families.head;

    while (family != NULL) {

        Node * children = ((Family * ) family -> data) -> children.head;
        int flag = 0;
        while (children != NULL) {

            Individual * child = (Individual * ) children -> data;

            if (flag == 1) {
                children = children -> next;

            }
            flag = 1;

            if (child != NULL) {
                
                insertBack( & list, child);	 

            }

        }

        family = family -> next;
    }
    return;

}
void deleteStoreFamily(void * toBeDeleted) {

    if (toBeDeleted == NULL) {

        return;
    }
    ReferenceFamily * temp = (ReferenceFamily * ) toBeDeleted;

    free(temp -> tag);
    free(temp -> reference);
    temp -> family = NULL;

    free(temp);
}

void deleteStoreIndividual(void * toBeDeleted) {

    if (toBeDeleted == NULL) {

        return;
    }
    ReferenceIndividual * temp = (ReferenceIndividual * ) toBeDeleted;

    free(temp -> tag);
    free(temp -> reference);
    temp -> individual = NULL;

    free(temp);

}
void fakeDelete(void * toBeDeleted) {

    return;
}
char * convertEncodingToString(CharSet type) {

    char * string = malloc(sizeof(char) * 100);
    if (type == ANSEL) {

        strcpy(string, "ANSEL");

    } else if (type == ASCII) {

        strcpy(string, "ASCII");

    } else if (type == UNICODE) {

        strcpy(string, "UNICODE");

    } else if (type == UTF8) {

        strcpy(string, "UTF-8");
    }

    return string;
}
char * writeHeader(const GEDCOMobject * obj) {

    int len = 0;
    char * string = NULL;

    if (strlen(obj -> header -> source) < 249) {

        len = len + strlen(obj -> header -> source);

    }
    char * encoding = convertEncodingToString(obj -> header -> encoding);
    len = len + strlen(encoding);

    string = calloc(1, sizeof(char) * len + 1000);

    sprintf(string, "0 HEAD\n1 SOUR %s\n1 GEDC\n2 VERS %.1lf\n1 CHAR %s\n1 SUBM @U1@\n", obj -> header -> source, obj -> header -> gedcVersion, encoding);
    obj -> header -> otherFields = initializeList(printField, deleteField, compareFields);

    return string;

}

char * writeSubmitter(const GEDCOMobject * obj) {

    int len = 0;

    char * string = NULL;
    if (strlen(obj -> submitter -> submitterName) < 61) {

        len = len + strlen(obj -> submitter -> submitterName);

    }
    string = calloc(1, sizeof(char) * len + 1000);

    if (strlen(obj -> submitter -> address) == 0) {

        sprintf(string, "0 @U1@ SUBM\n1 NAME %s\n", obj -> submitter -> submitterName);

    } else {
        obj -> header -> submitter = obj -> submitter;
        sprintf(string, "0 @U1@ SUBM\n1 NAME %s\n1 ADDR %s\n", obj -> submitter -> submitterName, obj -> submitter -> address);

    } 

    return string;

}
char * writeIndividual(const GEDCOMobject * obj, StoreList * sl) {

    if (obj == NULL) {
        return NULL;

    }
    int len = 999;
    int counter = 1;
    char * string = malloc(sizeof(char) * 100);
    strcpy(string, "");
    ReferenceIndividual * refIndi = NULL;
    ReferenceFamily * refFam = NULL;
    //char * indiC = NULL;
    int famCounter = 1;
    char * givenName = NULL;
    char * givenName2 = NULL;
    char * surname = NULL;
    char * surname2 = NULL;
    char * type = NULL;
    char * date = NULL;
    char * place = NULL;
    char * otherFields = NULL;
    char * husband = NULL;
    char * wife = NULL;
    char * children = NULL;

    Node * fam_node = obj -> families.head;
    while (fam_node != NULL) {

        refFam = storeFamily();
        Family * f = (Family * ) fam_node -> data;
        refFam -> family = f;
        refFam -> xref = malloc(sizeof(char) * 10000);
        strcpy(refFam -> xref, "");
        sprintf(refFam -> xref, "@F%d@", famCounter);
        insertBack( & sl -> families, refFam);
        famCounter++;
        fam_node = fam_node -> next;
    }
    Node * temp = obj -> individuals.head;
    while (temp != NULL) {
        refIndi = storeIndividual();
        Individual * indi = (Individual * ) temp -> data;
        refIndi -> individual = indi;
        refIndi -> xref = malloc(sizeof(char) * 100);
        strcpy(refIndi -> xref, "");
        sprintf(refIndi -> xref, "0 @I%d@ INDI\n", counter);
        strcat(string, refIndi -> xref);

        //Node * node  = indi->
        if (indi -> givenName != NULL || indi -> surname != NULL) {
            givenName = malloc(sizeof(char) * 200);
            surname = malloc(sizeof(char) * 200);
            strcpy(givenName, "");
            string = realloc(string, sizeof(char) * len);
            sprintf(givenName, "1 NAME %s ", indi -> givenName);
            strcat(string, givenName);
            sprintf(surname, "/%s/\n", indi -> surname);
            strcat(string, surname);

        }
        if (indi -> givenName != NULL && strcmp(indi -> givenName, " ") != 0) {
            givenName2 = malloc(sizeof(char) * 200);
            strcpy(givenName2, "");
            string = realloc(string, sizeof(char) * len);
            sprintf(givenName2, "1 GIVN %s\n", indi -> givenName);
            strcat(string, givenName2);

        }
        if (indi -> surname != NULL && strcmp(indi -> surname, " ") != 0) {

            surname2 = malloc(sizeof(char) * 200);
            strcpy(surname2, "");
            string = realloc(string, sizeof(char) * len);
            sprintf(surname2, "1 SURN %s\n", indi -> surname);
            strcat(string, surname2);

        }
        Node * event = indi -> events.head;

        while (event != NULL) {
            Event * e = (Event * )(event -> data);

            type = malloc(sizeof(char) * 200);
            string = realloc(string, sizeof(char) * len);
            sprintf(type, "1 %s\n", e -> type);
            strcat(string, type);
            if (e -> date != NULL) {
                date = malloc(sizeof(char) * 200);
                string = realloc(string, sizeof(char) * len);
                sprintf(date, "2 DATE %s\n", e -> date);
                strcat(string, date);

            }
            if (e -> place != NULL) {
                place = malloc(sizeof(char) * 200);
                string = realloc(string, sizeof(char) * len);
                sprintf(place, "2 PLAC %s\n", e -> place);
                strcat(string, place);

            }

            event = event -> next;

        }
        Node * other = indi -> otherFields.head;
        while (other != NULL) {
            Field * f = (Field * ) other -> data;
            if (f -> tag != NULL && f -> value != NULL) {
                otherFields = malloc(sizeof(char) * 200);
                strcpy(otherFields, "");
                string = realloc(string, sizeof(char) * len);
                sprintf(otherFields, "1 %s %s\n", f -> tag, f -> value);
                strcat(string, otherFields);
            }
            other = other -> next;
        }

        Node * n = sl -> families.head;
        while (n != NULL) {

            ReferenceFamily * rf = (ReferenceFamily * ) n -> data;
            if (rf -> family -> husband == indi) {
                husband = calloc(1, sizeof(char) * 200);
                strcpy(husband, "");
                string = realloc(string, sizeof(char) * len);
                sprintf(husband, "1 FAMS %s\n", rf -> xref);
                husband[strlen(husband)] = '\0';
                strcat(string, husband);
                strcpy(husband, "");

            } else if (rf -> family -> wife == indi) {

                wife = calloc(1, sizeof(char) * 200);
                strcpy(wife, "");
                string = realloc(string, sizeof(char) * len);
                sprintf(wife, "1 FAMS %s\n", rf -> xref);
                wife[strlen(wife)] = '\0';
                strcat(string, wife);
                strcpy(wife, "");
            } else {
                Node * child = rf -> family -> children.head;

                while (child != NULL) {

                    if (((Individual * )(child -> data)) == indi) {

                        children = calloc(1, sizeof(char) * 200);
                        strcpy(children, "");
                        string = realloc(string, sizeof(char) * len);
                        sprintf(children, "1 FAMC %s\n", rf -> xref);
                        rf -> xref[strlen(rf -> xref)] = '\0';
                        strcat(string, children);
                        //rf->xref[strlen(rf->xref)] = '\0';

                    }
                    

                    child = child -> next;
                }

            }

            n = n -> next;
        }

        temp = temp -> next;

        insertBack( & sl -> individuals, refIndi);
        len = len + 999;
        counter++;
    }

    sprintf(string, "%s", string);

    return string;

}
ReferenceIndividual * storeIndividual() {

    ReferenceIndividual * ref = malloc(sizeof(ReferenceIndividual));
    ref -> individual = NULL;
    ref -> xref = NULL;
    ref -> tag = NULL;
    ref -> reference = NULL;
    return ref;

}

ReferenceFamily * storeFamily() {

    ReferenceFamily * ref = malloc(sizeof(ReferenceFamily));
    ref -> family = NULL;
    ref -> xref = NULL;
    ref -> tag = NULL;
    ref -> reference = NULL;
    return ref;

}

char * writeFamily(const GEDCOMobject * obj, StoreList * sl) {

    if (obj == NULL) {

        return NULL;
    }

    ReferenceFamily * refFam = NULL;
    ReferenceIndividual * refIndi = NULL;
    char * string = malloc(sizeof(char) * 100);
    int famCounter = 1;
    int counter = 1;
    int len = 990;
    char * otherFields = NULL;
    char * type = NULL;
    char * place = NULL;
    char * date = NULL;
    char * husband = NULL;
    char * wife = NULL;
    char * children = NULL;

    Node * t = obj -> individuals.head;
    while (t != NULL) {
        refIndi = storeIndividual();
        Individual * indi = (Individual * ) t -> data;
        refIndi -> individual = indi;
        refIndi -> xref = malloc(sizeof(char) * 100);
        strcpy(refIndi -> xref, " ");
        sprintf(refIndi -> xref, "@I%d@", counter);

        
        insertBack( & sl -> individuals, refIndi);
        counter++;
        t = t -> next;
    }
    Node * fam_node = obj -> families.head;

    while (fam_node != NULL) {

        refFam = storeFamily();
        Family * f = (Family * ) fam_node -> data;
        refFam -> family = f;
        refFam -> xref = calloc(1, sizeof(char) * 10000);
        strcpy(refFam -> xref, "");
        sprintf(refFam -> xref, "0 @F%d@ FAM\n", famCounter);
        refFam -> xref[strlen(refFam -> xref)] = '\0';

        strcat(string, refFam -> xref);
        Node * other_fields = f -> otherFields.head;
        while (other_fields != NULL) {

            Field * field = (Field * ) other_fields;
            if (field -> tag != NULL && field -> value != NULL) {
                otherFields = malloc(sizeof(char) * 200);
                strcpy(otherFields, "");
                string = realloc(string, sizeof(char) * len);
                sprintf(otherFields, "1 %s %s\n", field -> tag, field -> value);
                strcat(string, otherFields);

            }
            other_fields = other_fields -> next;
        }

        Node * event = f -> events.head;
        while (event != NULL) {

            Event * e = (Event * ) event -> data;
            type = malloc(sizeof(char) * 200);
            string = realloc(string, sizeof(char) * len);
            sprintf(type, "1 %s\n", e -> type);
            strcat(string, type);
            if (e -> date != NULL) {
                date = malloc(sizeof(char) * 200);
                string = realloc(string, sizeof(char) * len);
                sprintf(date, "2 DATE %s\n", e -> date);
                strcat(string, date);

            }
            if (e -> place != NULL) {
                place = malloc(sizeof(char) * 200);
                string = realloc(string, sizeof(char) * len);
                sprintf(place, "2 PLAC %s\n", e -> place);
                strcat(string, place);

            }

            event = event -> next;
        }

        Node * c = sl -> individuals.head;
        while (c != NULL) {

            ReferenceIndividual * ri = (ReferenceIndividual * ) c -> data;
            
            if (refFam -> family -> husband == ri -> individual) {
                
                husband = malloc(sizeof(char) * 100);
                strcpy(husband, "");
                string = realloc(string, sizeof(char) * len);
                sprintf(husband, "1 HUSB %s\n", ri -> xref);
                husband[strlen(husband) - 1] = '\0';
                strcat(string, husband);

            } else if (ri -> individual == f -> wife) {
                wife = malloc(sizeof(char) * 100);
                strcpy(wife, "");
                string = realloc(string, sizeof(char) * len);
                sprintf(wife, "1 WIFE %s\n", ri -> xref);
                wife[strlen(wife) - 1] = '\0';
                strcat(string, wife);

            } else {

                Node * child_node = f -> children.head;
                while (child_node != NULL) {

                    if (((Individual * )(child_node -> data)) == ri -> individual) {

                        children = malloc(sizeof(char) * 200);
                        strcpy(children, "");
                        string = realloc(string, sizeof(char) * len);
                        sprintf(children, "1 CHIL %s\n", ri -> xref);
                        strcat(string, children);

                    }

                    child_node = child_node -> next;
                }

            }

            c = c -> next;
        }
        fam_node = fam_node -> next;
        famCounter++;
        insertBack( & sl -> families, refFam);
        len = len + 999;
    }

    sprintf(string, "%s", string);
    return string;

}

Header * createHeader2() {

    Header * header = malloc(sizeof(Header));
    header -> gedcVersion = 0.0;
    header -> encoding = ASCII;
    header -> submitter = NULL;
    header -> otherFields = initializeList(printField, deleteField, compareFields);
    return header;

}

Submitter * createSubmitter2() {

    Submitter * submitter = malloc(sizeof(Submitter));
    //submitter->address = NULL;
    submitter -> otherFields = initializeList(printField, deleteField, compareFields);
    return submitter;

}
GEDCOMobject * createGEDCOMobj() {

    GEDCOMobject * obj = malloc(sizeof(GEDCOMobject));
    obj -> header = createHeader2();
    obj -> submitter = createSubmitter2();
    obj -> families = initializeList(printFamily, fakeDelete, compareFamilies);
    obj -> individuals = initializeList(printIndividual, fakeDelete, compareIndividuals);
    return obj;

}
void getDescendantListNHelper(List * list,
    const Individual * person, unsigned int maxGen) {

    if (person == NULL || maxGen < 1) {

        return;

    }

}
Descendant * createDescendant() {

    Descendant * des = malloc(sizeof(Descendant));
    des -> level = 0;
    des -> ptr = NULL;
    return des;

}

int compareLevels(const void * a,
    const void * b) {

    Descendant * first = (Descendant * ) a;
    Descendant * second = (Descendant * ) b;

    if (first -> level > second -> level) {
        return 1;
    } else if (first -> level < second -> level) {
        return -1;
    }
    return 0;

}
int comparePointers(const void * a,
    const void * b) {

    if (a == b) {

        return 0;
    } else if (a < b) {

        return -1;
    } else {
        return 1;
    }

    return 0;

}
int compareLists(const void * a,
    const void * b) {

    Individual * first = (Individual * ) a;
    Individual * second = (Individual * ) b;

    if (strcmp(first -> surname, second -> surname) == 0) {

        return strcmp(first -> givenName, second -> givenName);

    }
    return strcmp(first -> surname, second -> surname);

}
void writeToGEDCOM(char * fileName, char * subName, char * subAddress, char * encoding) {
    //printf("fdsfsdfsdfsdfdsfsdfsd\n");
    if (fileName == NULL || subName == NULL || strlen(fileName) == 0 || strlen(subName) == 0) {

        return;
    }
    char temp[100] = "uploads/";

    GEDCOMobject * obj = malloc(sizeof(GEDCOMobject));
    Header * h = malloc(sizeof(Header));
    Submitter * s = malloc(sizeof(Submitter) + 255);
    char * string = calloc(1, sizeof(char) * 10000);
    strcpy(string, "");
    strcpy(h -> source, "Ancestry.com");
    h -> encoding = ASCII;
    h -> gedcVersion = 5.5;
    h -> submitter = s;
    h -> otherFields = initializeList(printField, deleteField, compareFields);
    strcpy(s -> submitterName, subName);
    strcpy(s -> address, subAddress);
    s -> otherFields = initializeList(printField, deleteField, compareFields);
    obj -> header = h;
    obj -> submitter = s;
    obj -> individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    obj -> families = initializeList(printFamily, deleteFamily, compareFamilies);

    strcat(temp, fileName);
    GEDCOMerror err = writeGEDCOM(temp, obj);
    if (err.type == OK) {
        return;
    } else {

    }

    return;

}
char * parseValidGEDCOM(char * fileName) {
    //printf("first one\n");
    if (strlen(fileName) == 0 || fileName == NULL) {

        char * string = calloc(1, sizeof(char) * 1000);
        strcpy(string, "");
        return string;
    }
    //printf("second one\n");
    char temp[100] = "./uploads/";
    strcat(temp, fileName);
    GEDCOMobject * obj = NULL;
    GEDCOMerror err = createGEDCOM(temp, & obj);
    char * string = NULL;

    if (err.type == OK) {

        string = IndiFamGEDCOMtoJSON(fileName, obj);

    }
    return string;

}
char * IndiFamGEDCOMtoJSON(char * fileName, GEDCOMobject * obj) {

    if (obj == NULL) {

        return NULL;

    }
    char * string = calloc(1, sizeof(char) * 100000);
    strcpy(string, "");
    char * encoding = convertEncodingToString(obj -> header -> encoding);
    
    sprintf(string, "{\"fileName\":\"%s\",\"source\":\"%s\",\"gedcVersion\":\"%.1lf\",\"encoding\":\"%s\",\"subName\":\"%s\",\"subAddress\":\"%s\",\"numOfIndi\":\"%d\",\"numOfFam\":\"%d\"}", fileName, obj -> header -> source, obj -> header -> gedcVersion, encoding, obj -> submitter -> submitterName, obj -> submitter -> address, obj -> individuals.length, obj -> families.length);
    return string;

}
char * sexAndFamilyIndToGEDCOM(Individual * indi) {

    if (indi == NULL) {
        return NULL;
    }

    char * string = calloc(1, sizeof(char) * 100000);
    char * save = malloc(sizeof(char) * 100);
    strcpy(save, "");
    int counter = 0;
    Node * temp = indi -> otherFields.head;
    while (temp != NULL) {

        Field * f = (Field * ) temp -> data;
        if (strcmp("SEX", f -> tag) == 0) {
            strcpy(save, f -> value);

        }

        temp = temp -> next;
    }
    if (strlen(save) == 0) {

        strcpy(save, "?");

    }
    Node * temp2 = indi -> families.head;
    while (temp2 != NULL) {

        Family * fam = (Family * ) temp2 -> data;
        if (fam -> wife == indi || fam -> husband == indi) {
            counter++;
            counter = counter + getLength(fam -> children);
            if (fam -> wife != NULL && fam -> husband != NULL) {

                counter++;

            }

        }
        temp2 = temp2 -> next;

    }
    if (counter < 1) {

        counter = 1;
    }
    strcpy(string, "");
    sprintf(string, "{\"givenName\":\"%s\",\"surname\":\"%s\",\"sex\":\"%s\",\"familySize\":\"%d\"}", indi -> givenName, indi -> surname, save, counter);

    return string;

}
char * parseIndividualGEDCOM(char * fileName) {

    if (fileName == NULL || strlen(fileName) == 0) {
        printf("it was null\n");
        return NULL;

    }
    char temp2[50] = "./uploads/";
    strcat(temp2, fileName);
    
    char * string = calloc(1, sizeof(char) * 100000);
    strcpy(string, "");
    GEDCOMobject * obj = NULL;
    char * temp = NULL;
    GEDCOMerror err = createGEDCOM(temp2, & obj);
    if (err.type == OK) {
        //printf("COMES HERE VICK\n");	
        temp = iListToJSONWithSexAndFamilySize(obj -> individuals);
        if (strlen(temp) == 0) {
            strcpy(temp, "");
            return temp;

        }

    }

    deleteGEDCOM(obj);
    
    return temp;

}
char * iListToJSONWithSexAndFamilySize(List iList) {

    Node * temp = iList.head;
    
    char * string = NULL;
    int len = 999;
    int counter = 0;
    char * string2 = calloc(1, sizeof(char) * 200000);
    strcpy(string2, "");
    strcat(string2, "[");
    while (temp != NULL) {

        Individual * indi = (Individual * ) temp -> data;
        string = sexAndFamilyIndToGEDCOM(indi);

        strcat(string2, string);
        if (counter != iList.length - 1) {
            strcat(string2, ",");
        }
       
        len = len + 999;
        counter++;
        temp = temp -> next;
    }

    strcat(string2, "]");
    
    return string2;

}
void addIndividualWrapper(char * filename, char * givenName, char * surname) {

    if (filename == NULL || strlen(filename) == 0 || givenName == NULL || strlen(givenName) == 0 || surname == NULL || strlen(surname) == 0) {
        printf("comes here\n");
        return;

    }
    char * string = calloc(1, sizeof(char) * 1000);
    char temp[50] = "./uploads/";
    char * str = calloc(1, sizeof(char) * 200000);
    strcpy(str, "");

    Individual * indi = createIndividual();
    strcpy(indi -> givenName, givenName);

    strcat(temp, filename);
    strcpy(indi -> surname, surname);
    GEDCOMobject * obj = NULL;
    GEDCOMerror err = createGEDCOM(temp, & obj);

    if (err.type == OK) {

        addIndividual(obj, (Individual * ) indi);

    }

    GEDCOMerror error = writeGEDCOM(temp, obj);
    if (error.type == OK) {
        return;

    } else {

    }

}
char * fileNameAndIndiJSON(char * fileName, Individual * indi) {
    if (strlen(fileName) == 0 || fileName == NULL || indi == NULL) {

        return NULL;

    }
    char * string = calloc(1, sizeof(char) * 100000);
    strcpy(string, "");

    sprintf(string, "{\"fileName\":\"%s\",\"givenName\":\"%s\",\"surname\":\"%s\"}", fileName, indi -> givenName, indi -> surname);
    return string;

}
char * iListToJSONGivenAndSurn(char * fileName, List iList) {

    Node * temp = iList.head;
    
    char * string = NULL;
    int len = 999;
    int counter = 0;
    char * string2 = calloc(1, sizeof(char) * 200000);
    strcpy(string2, "");
    strcat(string2, "[");
    while (temp != NULL) {

        Individual * indi = (Individual * ) temp -> data;
        string = fileNameAndIndiJSON(fileName, indi);

        strcat(string2, string);
        if (counter != iList.length - 1) {
            strcat(string2, ",");
        }
        
        len = len + 999;
        counter++;
        temp = temp -> next;
    }

    strcat(string2, "]");
    
    return string2;

}
char * fileNameAndIndWrapper(char * fileName) {

    if (fileName == NULL || strlen(fileName) == 0) {

        return NULL;

    }

    char temp[50] = "./uploads/";
    strcat(temp, fileName);
    char * string = calloc(1, sizeof(char) * 100000);
    strcpy(string, "");
    GEDCOMobject * obj = NULL;
    GEDCOMerror err = createGEDCOM(temp, & obj);
    if (err.type == OK) {	
        string = iListToJSONGivenAndSurn(temp, obj -> individuals);

    }
    deleteGEDCOM(obj);

    return string;

}

char * wrappergListToJSON(char * filename, unsigned int maxGen, char * givenName, char * surname) {

    if (filename == NULL || strlen(filename) == 0) {

        //printf("came here instead\n");
        char * string = malloc(sizeof(char) * 10);
        strcpy(string, "");
        return string;

    }
    GEDCOMobject * obj = NULL;
    char temp[200] = "./uploads/";
    strcat(temp, filename);
    List des;
    Individual * toBeSaved = NULL;
    createGEDCOM(temp, & obj);

    Node * temp2 = obj -> individuals.head;
    while (temp2 != NULL) {

        Individual * indi = (Individual * ) temp2 -> data;
        if (strcmp(indi -> givenName, givenName) == 0 && strcmp(indi -> surname, surname) == 0) {
            toBeSaved = indi;

        }

        temp2 = temp2 -> next;
    }
    if (toBeSaved != NULL) {
        
        des = getDescendantListN(obj, toBeSaved, maxGen);

    }
    char * temp3 = gListToJSON(des);
    return temp3;
}
char * wrappergListToJSONAncestors(char * filename, unsigned int maxGen, char * givenName, char * surname) {

    if (filename == NULL || strlen(filename) == 0) {

        char * string = malloc(sizeof(char) * 10);
        strcpy(string, "");
        return string;

    }
    GEDCOMobject * obj = NULL;
    char temp[200] = "./uploads/";
    strcat(temp, filename);
    List des;
    Individual * toBeSaved = NULL;
    createGEDCOM(temp, & obj);

    Node * temp2 = obj -> individuals.head;
    while (temp2 != NULL) {

        Individual * indi = (Individual * ) temp2 -> data;
        if (strcmp(indi -> givenName, givenName) == 0 && strcmp(indi -> surname, surname) == 0) {
            toBeSaved = indi;

        }

        temp2 = temp2 -> next;
    }

    if (toBeSaved != NULL) {
        
        des = getAncestorListN(obj, toBeSaved, maxGen);
        //printf("the length of des is %d\n",des.length);

    }
    char * temp3 = gListToJSON(des);
    return temp3;

}
char * parseIndividualGEDCOM2(char * filename) {

    if (strlen(filename) == 0 || filename == NULL) {

        return NULL;
    }
    char temp2[50] = "./uploads/";
    strcat(temp2, filename);
    char * string = calloc(1, sizeof(char) * 100000);
    strcpy(string, "");
    GEDCOMobject * obj = NULL;
    char * temp = NULL;
    GEDCOMerror err = createGEDCOM(temp2, & obj);
    if (err.type == OK) {	
        temp = iListToJSONWithSexAndFamilySize2(obj -> individuals);
        if (strlen(temp) == 0) {
            strcpy(temp, "");
            return temp;

        }

    }

    deleteGEDCOM(obj);
    return temp;

}
char * iListToJSONWithSexAndFamilySize2(List iList) {

    Node * temp = iList.head;
    char * string = NULL;
    int len = 999;
    int counter = 0;
    char * string2 = calloc(1, sizeof(char) * 200000);
    strcpy(string2, "");

    strcat(string2, "[");
    while (temp != NULL) {

        Individual * indi = (Individual * ) temp -> data;
        string = sexAndFamilyIndToGEDCOM2(indi);

        strcat(string2, string);
        if (counter != iList.length - 1) {
            strcat(string2, ",");
        }
        len = len + 999;
        counter++;
        temp = temp -> next;
    }
    strcat(string2, "]");
    return string2;

}
char * sexAndFamilyIndToGEDCOM2(Individual * indi) {

    if (indi == NULL) {
        return NULL;
    }

    char * string = calloc(1, sizeof(char) * 100000);
    char * save = malloc(sizeof(char) * 100);
    strcpy(save, "");
    int counter = 0;
    Node * temp = indi -> otherFields.head;
    while (temp != NULL) {

        Field * f = (Field * ) temp -> data;
        if (strcmp("SEX", f -> tag) == 0) {
            strcpy(save, f -> value);

        }

        temp = temp -> next;
    }
    if (strlen(save) == 0) {

        strcpy(save, "?");

    }
    Node * temp2 = indi -> families.head;
    while (temp2 != NULL) {

        Family * fam = (Family * ) temp2 -> data;
        if (fam -> wife == indi || fam -> husband == indi) {
            counter++;
            counter = counter + getLength(fam -> children);
            if (fam -> wife != NULL && fam -> husband != NULL) {

                counter++;

            }

        }
        temp2 = temp2 -> next;

    }
    if (counter < 1) {

        counter = 1;
    }
    strcpy(string, "");
    sprintf(string, "{\"givenName\":\"%s\",\"surname\":\"%s\",\"sex\":\"%s\",\"familySize\":\"%d\"}", indi -> givenName, indi -> surname, save, counter);

    return string;

}
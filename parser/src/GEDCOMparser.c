#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdbool.h>

#include <ctype.h>

#include "GEDCOMparser.h"

#include "GEDCOMutilities.h"


List des;

int desCounter = 0;
void wrapperInsert(Event * event) {
    insertBack( & EVENT_GARBAGE, event);
}
void deleteEvent(void * toBeDeleted) {
    Event * temp = NULL;
    if (toBeDeleted == NULL) {
        return;
    }

    temp = (Event * ) toBeDeleted;

    if (temp->place != NULL) {
        free(temp -> place);
    }
    if (temp->date != NULL) {
        free(temp -> date);
    }
    clearList( & (temp->otherFields));
    free(temp);

}
int compareEvents(const void * first,
    const void * second) {

    if (first == NULL || second == NULL) {
        return 0;
    }

    Event * one = (Event * ) first;
    Event * two = (Event * ) second;
    if (strlen(one->date) != 0 && strlen(two->date) != 0) {
        int year = 0;
        int year2 = 0;
        char month2[4];
        char month[4];
        int day = 0;
        int day2 = 0;
        int sumDayOne = 0;
        int sumDayTwo = 0;

        sscanf(one->date, "%d %s %d", &day, month, &year);

        sumDayOne = getMonth(month);
        sumDayOne = sumDayOne + getYear(year);
        sumDayOne = sumDayOne + day;

        sscanf(two->date, "%d %s %d", &day2, month2, & year2);
        sumDayTwo = getMonth(month2);
        sumDayTwo = sumDayTwo + getYear(year2);
        sumDayTwo = sumDayTwo + day2;

        if (sumDayOne < sumDayTwo) {
            return -1;
        } else if (sumDayOne > sumDayTwo) {
            return 1;
        } else {
            return 0;
        }

    } else {

        if (strcmp(one->type, two->type) < 0) {
            return -1;
        } else if (strcmp(one->type, two->type) > 0) {

            return 1;
        } else {
            return 0;
        }

    }
    return 0;
}
char * printEvent(void * toBePrinted) {
    char * string = NULL;
    Event * temp;
    string = calloc(1, sizeof(char) * 1000);
    temp = (Event * ) toBePrinted;
    sprintf(string, "%s %s %s\n", temp -> type, temp -> date, temp -> place);
    strcat(string, toString(temp -> otherFields));
    return string;

}
void deleteIndividual(void * toBeDeleted) {
    Individual * temp = NULL;
    if (toBeDeleted == NULL) {
        return;
    }

    temp = (Individual * ) toBeDeleted;
    free(temp->givenName);
    free(temp->surname);
    clearList( & (temp->families));
    clearList( & (temp->events));
    clearList( & (temp->otherFields));
    free(temp);

}
char * printIndividual(void * toBePrinted) {
    Individual * temp;

    char * string = NULL;
    if (toBePrinted == NULL) {
        return NULL;
    }

    temp = (Individual * ) toBePrinted;
    string = calloc(1, sizeof(char) * 1000);

    sprintf(string, "%s %s\n", temp->givenName, temp->surname);
    strcat(string, toString(temp -> events));
    strcat(string, toString(temp -> families));
    strcat(string, toString(temp -> otherFields));
    return string;

}
int compareIndividuals(const void * first,
    const void * second) {
    if (first == NULL || second == NULL) {
        return 0;
    }

    Individual * one = (Individual * ) first;
    Individual * two = (Individual * ) second;

    char * string1 = calloc(1, sizeof(char) * strlen(one -> givenName) + strlen(one -> surname) + 10);
    char * string2 = calloc(1, sizeof(char) * strlen(two -> givenName) + 2 + strlen(two -> surname) + 10);
    strcpy(string1, one -> givenName);
    strcat(string1, ",");
    strcat(string1, one -> surname);
    strcpy(string2, two -> givenName);
    strcat(string2, ",");
    strcat(string2, two -> surname);

    if (strcmp(string1, string2) < 0) {
        free(string1);
        free(string2);
        strcpy(string1, "");
        strcpy(string2, "");
        return -1;
    } else if (strcmp(string1, string2) > 0) {
        free(string1);
        free(string2);
        strcpy(string1, "");
        strcpy(string2, "");
        return 1;
    } else if (strcmp(string1, string2) == 0) {
        free(string1);
        free(string2);
        strcpy(string1, "");
        strcpy(string2, "");
        return 0;
    }
    return -1;
}
void deleteFamily(void * toBeDeleted) {
    Family * temp;
    if (toBeDeleted == NULL) {
        return;
    }

    temp = (Family * ) toBeDeleted;
    temp -> wife = NULL;
    temp -> husband = NULL;
    clearList( & (temp -> children));
    clearList( & (temp -> events));
    clearList( & (temp -> otherFields));
    free(temp);

}
int compareFamilies(const void * first,
    const void * second) {
    if (first == NULL || second == NULL) {
        return 0;
    }
    Family * one = (Family * ) first;
    Family * two = (Family * ) second;
    int counterOne = 0;
    int counterTwo = 0;
    if (one -> wife != NULL) {
        counterOne++;
    }
    if (one -> husband != NULL) {
        counterOne++;
    }
    if (two -> wife != NULL) {
        counterTwo++;
    }
    if (two -> husband != NULL) {
        counterTwo++;
    }
    counterOne = counterOne + getLength(one -> children);
    counterOne = counterOne + getLength(one -> otherFields);

    counterTwo = counterTwo + getLength(two -> children);
    counterTwo = counterTwo + getLength(two -> otherFields);

    if (counterOne < counterTwo) {
        return -1;
    } else if (counterOne > counterTwo) {
        return 1;
    }
    return 0;

}
char * printFamily(void * toBePrinted) {
    Family * temp;

    if (toBePrinted == NULL) {
        return NULL;
    }

    temp = (Family * ) toBePrinted;
    char * event = toString(temp -> events);
    char * child = toString(temp -> children);
    char * other = toString(temp -> otherFields);

    char * husband = calloc(1, sizeof(char) * 256);
    char * wife = calloc(1, sizeof(char) * 256);
    char * string = calloc(1, sizeof(char) * strlen(toString(temp -> events)) + strlen(toString(temp -> children)) + strlen(toString(temp -> otherFields)) + 1000);
    if (temp -> wife != NULL) {
        sprintf(wife, "%s %s", temp -> wife -> givenName, temp -> wife -> surname);
    } else if (temp -> wife == NULL) {
        strcpy(wife, "");
    }

    if (temp -> husband != NULL) {
        sprintf(husband, "%s %s", temp -> husband -> givenName, temp -> husband -> surname);
    } else if (temp -> husband == NULL) {
        strcpy(husband, "");
    }

    strcat(string, husband);
    strcat(string, wife);
    strcat(string, event);
    strcat(string, child);
    strcat(string, other);

    return string;

}
void deleteField(void * toBeDeleted) {
    Field * temp = NULL;

    if (toBeDeleted == NULL) {
        return;
    }
    temp = (Field * ) toBeDeleted;
    free(temp -> value);
    free(temp -> tag);
    free(temp);

}
int compareFields(const void * first,
    const void * second) {

    if (first == NULL || second == NULL) {
        return 0;
    }
    Field * one = (Field * ) first;
    Field * two = (Field * ) second;
    char * string1 = calloc(1, sizeof(char) * strlen(one -> tag) + 2 + strlen(one -> value) + 2);
    char * string2 = calloc(1, sizeof(char) * strlen(two -> tag) + 2 + strlen(two -> value) + 2);
    strcat(string1, one -> tag);
    strcat(string1, " ");
    strcat(string1, one -> value);
    string1 = toLower(string1);
    strcat(string2, two -> tag);
    strcat(string2, " ");
    strcat(string2, two -> value);
    string2 = toLower(string2);

    if (strcmp(string1, string2) < 0) {

        free(string1);
        free(string2);
        return -1;
    } else if (strcmp(one -> tag, two -> tag) > 0) {
        free(string1);
        free(string2);
        return 1;
    } else {
        free(string1);
        free(string2);
        return 0;
    }

}
char * printField(void * toBePrinted) {
    Field * temp;
    int len;
    char * string = NULL;
    if (toBePrinted == NULL) {
        return NULL;
    }
    temp = (Field * ) toBePrinted;
    len = strlen(temp -> tag) + strlen(temp -> value) + 10;
    string = calloc(1, sizeof(char) * len);
    sprintf(string, "%s %s\n", temp -> tag, temp -> value);
    return string;

}
char * printError(GEDCOMerror err) {
    char * string = calloc(1, sizeof(char) * 30);
    char * printErrorString = NULL;

    if (err.type == OK && err.line == -1) {
        strcpy(string, "Okay");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);

    } else if (err.type == INV_FILE && err.line == -1) {
        strcpy(string, "Invaild File");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);
    } else if (err.type == INV_GEDCOM && err.line == -1) {
        strcpy(string, "Invaild GEDCOM");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);
    } else if (err.type == INV_RECORD) {
        strcpy(string, "Invaild Record");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);
    } else if (err.type == OTHER_ERROR) {
        strcpy(string, "Other Error");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);
    } else if (err.type == INV_HEADER && err.line != -1) {
        char * num = calloc(1, sizeof(char) * 4);
        sprintf(num, "%d", err.line);
        strcat(string, "Invaild Header(Line ");
        strcat(string, num);
        strcat(string, ")");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);
        free(num);
    } else if (err.type == INV_RECORD && err.line != -1) {
        char * num = calloc(1, sizeof(char) * 4);
        sprintf(num, "%d", err.line);
        strcat(string, "Invaild Record(Line ");
        strcat(string, num);
        strcat(string, ")");
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
        free(string);
        free(num);
    } else {
        string = "Invaild Error";
        printErrorString = calloc(1, sizeof(char) * strlen(string) + 2);
        strcpy(printErrorString, string);
    }

    return printErrorString;
}
void deleteGEDCOM(GEDCOMobject * obj) {
    if (obj == NULL) {
        return;
    } else {
        deleteHeader(obj -> header);
        deleteSubmitter(obj -> submitter);
        clearList( & (obj -> individuals));
        clearList( & (EVENT_GARBAGE));
        clearList( & (obj -> families));
    }
    obj -> header = NULL;
    obj -> submitter = NULL;
    free(obj);

}

GEDCOMerror createGEDCOM(char * fileName, GEDCOMobject ** obj) {

    FILE * fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        return createGEDCOMerror(INV_FILE, -1);
    }
    if (strstr(fileName, ".ged") == 0) {
        return createGEDCOMerror(INV_FILE, -1);
    }
    EVENT_GARBAGE = initializeList(printEvent, deleteEvent, compareEvents);

    char str[300] = "";
    GEDCOMerror err;
    char buffer[256];
    char * token = NULL;
    int headCounter = 0;
    int i = 0;
    bool someBool = false;
    int headFlag = 0;
    char * temp = NULL;
    Zero flag;
    int submitterC = 0;
    int nameCount = 0;
    int sourceCount = 0;
    int gedcCount = 0;
    int encodingCount = 0;
    int isPointer = 0;
    Checker check;
    Flag f;
    check.eventFlag = 0;
    f.source = 0;
    f.version = 0;
    f.submitterReference = 0;
    f.address = false;
    int recordCount = 0;
    f.encoding = 0;
    ReferenceIndividual * refIndi = NULL;
    check.indiCount = 0;
    f.individual = false;
    f.name = 0;
    check.event = NULL;

    int submitterCount = 0;
    check.date = false;
    check.place = false;
    check.famPlace = false;
    check.famDate = false;
    check.version = 0;
    check.placFlag = 0;
    check.eventCounter = 0;
    int trlrCount = 0;
    Individual * individual;
    Family * family;
    check.place = false;

    check.famPlace = false;
    Node * tempIndi = NULL;
    Node * tempFam = NULL;
    StoreList * sl = storeListFunc();

    GEDCOMobject * tempObj = calloc(1, sizeof(GEDCOMobject));

    if (tempObj == NULL) {
        return createGEDCOMerror(OTHER_ERROR, -1);
    }

    if (strlen(fileName) == 0 || fileName == NULL) {
        return createGEDCOMerror(INV_FILE, -1);
    }
    if (strcmp(fileName, "") == 0) {
        return createGEDCOMerror(INV_FILE, -1);
    }

    tempObj -> header = createHeader("", "", "");
    tempObj -> families = initializeList(printFamily, deleteFamily, compareFamilies);
    tempObj -> individuals = initializeList(printIndividual, deleteIndividual, compareIndividuals);
    tempObj -> submitter = createSubmitter("", "");

    GEDCOMtag * gedcTag = calloc(1, sizeof(GEDCOMtag) * 1);
    while (myfgets(buffer, 300, fptr) != NULL) {
        if (buffer[0] == '\n') {
            continue;
        }
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (strlen(buffer) > 255) {
            return createGEDCOMerror(INV_RECORD, i + 1);
        }

        temp = calloc(1, sizeof(char) * strlen(buffer) + 1);
        strcpy(temp, buffer);
        token = strtok(temp, " ");
        int isDigit = checkFirstDigit(token);
        if (isDigit == 1) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        }

        bool doubleCheck = checkBothDigits(token);

        if (doubleCheck == false) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        }

        gedcTag[i].lineNumber = atoi(token);

        if (gedcTag[i].lineNumber < 0) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        }

        if (strcmp(token, "0") == 0 && headFlag == 1) {

            if (sourceCount == 1 && gedcCount == 1 && encodingCount == 1 && submitterC == 1) {

                token = strtok(NULL, " ");
                bool lower = checkLowerCase(token);
                if (lower == false) {
                    return createGEDCOMerror(INV_GEDCOM, -1);
                }
                if (strcmp(token, "TRLR") == 0 && headCounter == 1) {
                    flag = TRLR;
                    someBool = true;
                }
                isPointer = checkPointer(token);

                if (isPointer == 0) {
                    strcpy(gedcTag[i].info, token);
                    token = strtok(NULL, " ");
                    strcpy(gedcTag[i].tag, token);
                    bool lower = checkLowerCase(token);
                    if (lower == false) {
                        return createGEDCOMerror(INV_GEDCOM, -1);
                    }

                    if (strcmp(token, "INDI") == 0) {
                        flag = INDIV;
                        f.individual = true;
                        individual = createIndividual();
                        check.eventFlag = 0;
                        insertBack( & tempObj -> individuals, individual);
                        refIndi = createIndividualReference(gedcTag[i].info, token);
                        refIndi -> individual = individual;
                        insertBack( & sl -> individuals, refIndi);
                        recordCount++;

                    }

                    if (strcmp(token, "SUBM") == 0) {
                        flag = SUBMM;
                        submitterCount++;
                    }
                    if (strcmp(token, "FAM") == 0) {
                        flag = FAMILY;
                        check.date = false;
                        check.place = false;
                        check.eventFlag = 0;
                        family = createFamily();
                        insertBack( & tempObj -> families, family);
                        recordCount++;
                    }

                }
            } else {
                return createGEDCOMerror(INV_HEADER, i + 1);
            }
        }

        if (strcmp(token, "0") == 0) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            int tagLen = checkTagLength(token);
            if (tagLen == 1) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            strcpy(gedcTag[i].tag, token);
            if (strcmp(token, "HEAD") == 0) {
                flag = HEAD;
                headFlag = 1;
                token = strtok(NULL, " ");
                headCounter++;
                if (token != NULL) {
                    return createGEDCOMerror(INV_GEDCOM, -1);
                }
            }
        } else if (strcmp(token, "FAM") != 0 && strcmp(token, "INDI") != 0 && strcmp(token, "SUBM") != 0 && strcmp(token, "TRLR") != 0) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            int checker = checkTagLength(token);
            if (checker == 1) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }

            strcpy(gedcTag[i].tag, token);

            token = strtok(NULL, " ");

            if (gedcTag[i].lineNumber == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    return createGEDCOMerror(INV_GEDCOM, -1);
                }
            } else {
                while (token != NULL) {
                    strcat(str, token);
                    strcat(str, " ");
                    token = strtok(NULL, " ");
                }

                strcpy(gedcTag[i].info, str);
                strcpy(str, "");
            }
        }

        if (flag == HEAD) {

            GEDCOMerror error = parseHeader(gedcTag, i, & tempObj, & check, & f);
            if (error.type == INV_GEDCOM) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            if (error.type == INV_HEADER) {
                return createGEDCOMerror(INV_HEADER, i);
            }
            if (error.type == INV_RECORD) {
                return createGEDCOMerror(INV_RECORD, i + 1);
            }

            if (error.type == OK) {
                if (f.encoding == 1) {
                    encodingCount++;
                    f.encoding = 0;
                }
                if (f.version == 1) {
                    gedcCount++;
                    f.version = 0;
                }
                if (f.source == 1) {
                    sourceCount++;
                    f.source = 0;
                }
                if (f.submitterReference == 1) {
                    submitterC = 1;
                    f.submitterReference = 0;
                }
            }
        }
        if (flag == SUBMM) {

            GEDCOMerror gerr = parseSubmitter(gedcTag, i, & tempObj, & f);

            if (gerr.type == INV_GEDCOM) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            if (gerr.type == INV_HEADER) {
                return createGEDCOMerror(INV_HEADER, i);
            }
            if (gerr.type == OK) {
                if (f.name == true) {
                    nameCount++;
                    f.name = false;
                }
                if (f.address == true) {
                    f.address = false;
                }
                tempObj -> header -> submitter = tempObj -> submitter;
            }

        }
        if (flag == INDIV) {
            GEDCOMerror indi = parseIndividual(gedcTag, i, & tempObj, & check, individual, sl);
            if (indi.type == INV_GEDCOM) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            if (indi.type == INV_HEADER) {
                return createGEDCOMerror(INV_HEADER, i);
            }
            if (gedcTag[i].lineNumber == 0) {
                check.indiCount++;
            }
            recordCount++;
        }
        if (flag == FAMILY) {
            GEDCOMerror fam = parseFamily(gedcTag, i, & tempObj, & check, family, sl);
            if (fam.type == INV_GEDCOM) {
                return createGEDCOMerror(INV_GEDCOM, -1);
            }
            if (fam.type == INV_HEADER) {
                return createGEDCOMerror(INV_HEADER, i);
            }
            recordCount++;
        }
        if (flag == TRLR) {
            trlrCount++;
        }
        if (headCounter > 1) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        } else if (headCounter == 0) {
            if (gedcTag[i].lineNumber == 1) {
                return createGEDCOMerror(INV_HEADER, i + 1);
            }
            return createGEDCOMerror(INV_GEDCOM, -1);
        }
        if (sourceCount > 1 || gedcCount > 1 || encodingCount > 1) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        }
        if (nameCount > 1) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        }
        if (submitterCount > 1) {
            return createGEDCOMerror(INV_HEADER, i);
        }
        if (trlrCount > 1) {
            return createGEDCOMerror(INV_GEDCOM, -1);
        }
        i++;

        gedcTag = realloc(gedcTag, sizeof(GEDCOMtag) * (i + 1));
        free(temp);
    }

    for (int m = 0; m < i; m++) {
        if (gedcTag[m].lineNumber == 1 && strcmp(gedcTag[m].tag, "HEAD") == 0) {
            return createGEDCOMerror(INV_HEADER, m + 1);
        }
    }

    if (recordCount < 0) {
        return createGEDCOMerror(INV_RECORD, i);
    }

    if (trlrCount == 0) {
        return createGEDCOMerror(INV_GEDCOM, -1);
    }
    if (submitterCount == 0) {

        return createGEDCOMerror(INV_GEDCOM, -1);
    }
    char * buf = calloc(1, sizeof(char) * 100);
    char * buf2 = calloc(1, sizeof(char) * 100);

    tempFam = sl -> families.head;
    while (tempFam != NULL) {

        tempIndi = sl -> individuals.head;

        while (tempIndi != NULL) {

            strcpy(buf, ((ReferenceIndividual * ) tempIndi -> data) -> reference);
            strcpy(buf2, ((ReferenceFamily * ) tempFam -> data) -> reference);
            buf2[strlen(buf2)] = '\0';

            if (strcmp(buf, buf2) == 0) {
                if (strcmp(((ReferenceFamily * ) tempFam -> data) -> tag, "HUSB") == 0) {

                    Individual * indi = ((ReferenceIndividual * )(tempIndi -> data)) -> individual;
                    Family * fam = ((ReferenceFamily * ) tempFam -> data) -> family;
                    fam -> husband = indi;
                    insertBack( & (indi -> families), fam);

                } else if (strcmp(((ReferenceFamily * ) tempFam -> data) -> tag, "WIFE") == 0) {

                    Individual * indi = ((ReferenceIndividual * )(tempIndi -> data)) -> individual;
                    Family * fam = ((ReferenceFamily * ) tempFam -> data) -> family;

                    fam -> wife = indi;
                    insertBack( & (indi -> families), fam);

                } else if (strcmp(((ReferenceFamily * ) tempFam -> data) -> tag, "CHIL") == 0) {

                    Individual * indi = ((ReferenceIndividual * )(tempIndi -> data)) -> individual;
                    insertBack( & ((ReferenceFamily * )(tempFam -> data)) -> family -> children, indi);
                    insertBack( & indi -> families, ((ReferenceFamily * )(tempFam -> data)) -> family);
                }

            }
            strcpy(buf, " ");
            strcpy(buf2, " ");
            tempIndi = tempIndi -> next;
        }

        tempFam = tempFam -> next;

    }

    if (someBool == true) {
        free(buf);
        free(buf2);
        deleteStoreList(sl);
        free(gedcTag);
        ( * obj) = tempObj;
        fclose(fptr);

        des = initializeList(printIndividual, deleteIndividual, compareIndividuals);
        Node * temp = tempObj -> individuals.head;

        while (temp != NULL) {

            Individual * indi = (Individual * ) temp -> data;

            Node * other = indi -> otherFields.head;
            while (other != NULL) {

                Field * f = (Field * ) other -> data;
                int check = checkPointer(f -> value);
                if (check == 0) {
                    Node * s = deleteDataFromList( & indi -> otherFields, other -> data);
                    if (s == NULL) {
                        continue;
                    }

                }
                if ((f -> tag != NULL) && (strcmp(f -> tag, "SURN") == 0 || strcmp(f -> tag, "GIVN")) == 0) {
                    Node * s = deleteDataFromList( & indi -> otherFields, other -> data);
                    if (s == NULL) {
                        continue;
                    }

                }

                other = other -> next;
            }
            temp = temp -> next;
        }

    }
    err.type = OK;
    return err;

}
Individual * findPerson(const GEDCOMobject * familyRecord, bool( * compare)(const void * first,
        const void * second),
    const void * person) {
    if (familyRecord == NULL || person == NULL) {
        return NULL;
    }
    Node * temp = familyRecord -> individuals.head;
    while (temp != NULL) {

        if (compare(((Individual * )(temp -> data)), person) == true) {
            return (Individual * ) temp -> data;
        }
        temp = temp -> next;
    }

    return NULL;
}

char * printGEDCOM(const GEDCOMobject * obj) {
    if (obj == NULL) {
        return NULL;
    }
    char * string = calloc(1, sizeof(char) * 100);
    if (string == NULL) {
        return NULL;
    }
    strcpy(string, "GEDCOM");
    return string;
}

List getDescendants(const GEDCOMobject * familyRecord,
    const Individual * person) {

    if (person == NULL) {
        return initializeList(printIndividual, deleteIndividual, compareIndividuals);
    }
    if (familyRecord == NULL) {
        return initializeList(printIndividual, deleteIndividual, compareIndividuals);
    }
    Node * temp = person -> families.head;

    while (temp != NULL) {

        Family * f = (Family * ) temp -> data;
        if (f -> wife == person || f -> husband == person) {
            Family * fam = (Family * ) temp -> data;
            Node * node = fam -> children.head;

            while (node != NULL) {
                Individual * indi = ((Individual * ) node -> data);
                des = getDescendants(familyRecord, indi);
                insertBack( & des, indi);
                node = node -> next;
            }

        }
        temp = temp -> next;
    }

    return des;
}

ErrorCode validateGEDCOM(const GEDCOMobject * obj) {

    if (obj == NULL) {
        return INV_GEDCOM;
    }
    int indiCount = 0;
    int famCount = 0;
    int otherCount = 0;
    if (obj -> header == NULL || obj -> submitter == NULL) {
        return INV_GEDCOM;
    }

    if (strlen(obj -> header -> source) == 0) {
        return INV_HEADER;
    }
    if (strlen(obj -> header -> source) > 249) {
        return INV_HEADER;
    }
    if (obj -> header -> gedcVersion < 1.0) {
        return INV_HEADER;
    }
    char * string = convertEncodingToString(obj -> header -> encoding);

    if (strcmp(string, "UTF-8") != 0 && strcmp(string, "UNICODE") != 0 && strcmp(string, "ANSEL") != 0 && strcmp(string, "ASCII") != 0) {
        return INV_HEADER;
    }
    if (obj -> header -> submitter == NULL) {
        return INV_HEADER;
    }

    if (strlen(obj -> submitter -> submitterName) == 0) {

        return INV_RECORD;

    }
    if (strlen(obj -> submitter -> submitterName) > 60) {

        return INV_RECORD;

    }
    Node * temp = obj -> individuals.head;

    while (temp != NULL) {

        Individual * indi = (Individual * ) temp -> data;

        if (indi -> givenName != NULL) {
            if (strlen(indi -> givenName) > 200) {

                return INV_RECORD;

            }
        }
        if (indi -> surname != NULL) {
            if (strlen(indi -> surname) > 200) {

                return INV_RECORD;

            }

        }

        Node * event = indi -> events.head;

        while (event != NULL) {

            Event * e = (Event * ) event -> data;

            if (strlen(e -> type) > 4) {

                return INV_RECORD;

            }

            event = event -> next;
        }

        indiCount++;
        temp = temp -> next;
    }

    if (indiCount != obj -> individuals.length) {

        return INV_RECORD;
    }
    Node * families = obj -> families.head;

    while (families != NULL) {

        Family * fam = (Family * ) families -> data;

        Node * x = fam -> otherFields.head;
        while (x != NULL) {

            otherCount++;

            x = x -> next;
        }
        Node * event = fam -> events.head;
        while (event != NULL) {

            Event * e = (Event * ) event -> data;
            if (strlen(e -> type) > 4) {

                return INV_RECORD;

            }

            event = event -> next;
        }

        families = families -> next;
        famCount++;
    }

    if (famCount != obj -> families.length) {

        return INV_RECORD;
    }
    return OK;

}
GEDCOMerror writeGEDCOM(char * fileName,
    const GEDCOMobject * obj) {

    if (obj == NULL) {
        //printf("11\n");
        return createGEDCOMerror(WRITE_ERROR, -1);
    }

    //int len = 999;
    ErrorCode error;
    //char *string = NULL;
    char * headerString = NULL;
    char * submitterString = NULL;
    char * individualString = NULL;
    //char * familyString = NULL;

    ReferenceFamily * refFam = NULL;
    ReferenceIndividual * refIndi = NULL;
    char * string = malloc(sizeof(char) * 100);
    int famCounter = 1;
    int counter = 1;
    int len = 990;
    char * string2 = calloc(1, sizeof(char) * 10000);
    char * otherFields = NULL;
    char * type = NULL;
    char * place = NULL;
    char * date = NULL;
    char * husband = NULL;
    char * wife = NULL;
    char * children = NULL;

    FILE * fptr = fopen(fileName, "w");
    if (fptr == NULL) {
        // printf("GUCCI\n");
        return createGEDCOMerror(WRITE_ERROR, -1);

    }
    if (strlen(fileName) == 0) {
        //printf("dsfsdf\n");
        return createGEDCOMerror(WRITE_ERROR, -1);
    }
    if (strstr(fileName, ".ged") == 0) {

        return createGEDCOMerror(INV_FILE, -1);

    }
    StoreList * sl = storeListFunc();
    StoreList * sl2 = storeListFunc();
    headerString = writeHeader(obj);

    submitterString = writeSubmitter(obj);
    individualString = writeIndividual(obj, sl);
    string = calloc(1, sizeof(char) * 1000000);
    strcpy(string, "");

    strcat(string, headerString);

    strcat(string, submitterString);

    strcat(string, individualString);

    Node * t = obj -> individuals.head;
    while (t != NULL) {
        refIndi = storeIndividual();
        Individual * indi = (Individual * ) t -> data;
        refIndi -> individual = indi;
        refIndi -> xref = malloc(sizeof(char) * 100);
        strcpy(refIndi -> xref, " ");
        sprintf(refIndi -> xref, "@I%d@", counter);

        //printf("122..Indiii is %s\n", indi->givenName);
        insertBack( & sl2 -> individuals, refIndi);
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

        strcat(string2, refFam -> xref);
        Node * other_fields = f -> otherFields.head;
        while (other_fields != NULL) {

            Field * field = (Field * ) other_fields;
            if (field -> tag != NULL && field -> value != NULL) {
                otherFields = malloc(sizeof(char) * 200);
                strcpy(otherFields, "");
                string2 = realloc(string2, sizeof(char) * len);
                sprintf(otherFields, "1 %s %s\n", field -> tag, field -> value);
                strcat(string2, otherFields);

            }
            other_fields = other_fields -> next;
        }

        Node * event = f -> events.head;
        while (event != NULL) {

            Event * e = (Event * ) event -> data;
            type = malloc(sizeof(char) * 200);
            string2 = realloc(string2, sizeof(char) * len);
            sprintf(type, "1 %s\n", e -> type);
            strcat(string2, type);
            if (e -> date != NULL) {
                date = malloc(sizeof(char) * 200);
                string2 = realloc(string2, sizeof(char) * len);
                sprintf(date, "2 DATE %s\n", e -> date);
                strcat(string2, date);

            }
            if (e -> place != NULL) {
                place = malloc(sizeof(char) * 200);
                string2 = realloc(string2, sizeof(char) * len);
                sprintf(place, "2 PLAC %s\n", e -> place);
                strcat(string2, place);

            }

            event = event -> next;
        }

        Node * c = sl2 -> individuals.head;
        while (c != NULL) {

            ReferenceIndividual * ri = (ReferenceIndividual * ) c -> data;
           
            if (refFam -> family -> husband == ri -> individual) {
                 
                husband = malloc(sizeof(char) * 100);
                strcpy(husband, "");
                string2 = realloc(string2, sizeof(char) * len);
                sprintf(husband, "1 HUSB %s\n", ri -> xref);
               
                strcat(string2, husband);

            } else if (ri -> individual == f -> wife) {
                
                wife = malloc(sizeof(char) * 100);
                strcpy(wife, "");
                string2 = realloc(string2, sizeof(char) * len);
                sprintf(wife, "1 WIFE %s\n", ri -> xref);
                
                strcat(string2, wife);

            } else {

                Node * child_node = f -> children.head;
                while (child_node != NULL) {

                    if (((Individual * )(child_node -> data)) == ri -> individual) {

                        children = malloc(sizeof(char) * 200);
                        strcpy(children, "");
                        string2 = realloc(string2, sizeof(char) * len);
                        
                        sprintf(children, "1 CHIL %s\n", ri -> xref);
                        strcat(string2, children);
                        

                    }

                    child_node = child_node -> next;
                }

            }

            c = c -> next;
        }
        fam_node = fam_node -> next;
        famCounter++;
        insertBack( & sl2 -> families, refFam);
        len = len + 999;
    }
    sprintf(string2, "%s", string2);
    strcat(string, string2);

    strcat(string, "0 TRLR");

    
    fprintf(fptr, "%s", string);

    error = validateGEDCOM(obj);
    if (error == INV_GEDCOM) {
        
        return createGEDCOMerror(INV_GEDCOM, -1);

    }
    if (error == INV_HEADER) {

        return createGEDCOMerror(INV_HEADER, -1);
    }
    if (error == INV_RECORD) {

        return createGEDCOMerror(INV_RECORD, -1);

    }

    fclose(fptr);
    return createGEDCOMerror(OK, -1);

}

char * indToJSON(const Individual * ind) {

    if (ind == NULL) {
        char * string = calloc(1, sizeof(char) * 20000);
        strcpy(string, "");
        return string;
    }
    char * string = calloc(1, sizeof(char) * 20000);
    strcpy(string, "");
    if (ind -> givenName != NULL && ind -> surname != NULL) {

        sprintf(string, "{\"givenName\":\"%s\",\"surname\":\"%s\"}", ind -> givenName, ind -> surname);

    } else if (ind -> givenName != NULL && ((ind -> surname == NULL) || strcmp(ind -> surname, "") == 0)) {

        sprintf(string, "\"{\"givenName\":\"%s\",\"surname\":,\"\"}\"", ind -> givenName);

    } else if ((ind -> givenName == NULL || strcmp(ind -> givenName, "")) && ind -> surname != NULL) {

        sprintf(string, "\"{\"givenName\"\":,\"surname\":\"%s\"}\"", ind -> surname);

    } else if (ind -> givenName == NULL && !strcmp(ind -> givenName, "") && ind -> surname == NULL && !strcmp(ind -> surname, "")) {

        strcat(string, "\"{\"givenName\"\":,\"surname\":\"\"}\"");
        	 

    }
    return string;

}
Individual * JSONtoInd(const char * str) {

    char * string = calloc(1, sizeof(char) * 20000);
    if (str == NULL) {
        return NULL;

    }
    if (strlen(str) == 0) {

        return NULL;
    }
    strcpy(string, "");
    strcpy(string, str);
    char * token = NULL;
    Individual * ind = createIndividual();
    token = strtok(string, "\"{\"");
    
    if (strcmp(token, "givenName") != 0) {

        return NULL;
    }
    token = strtok(NULL, "\":\",");
    
    if (strcmp(token, "surname") == 0) {
        strcpy(ind -> givenName, "");

    } else {
        strcpy(ind -> givenName, token);
    }

    token = strtok(NULL, "\",\"");
    token = strtok(NULL, "\":\"}");
    if (token == NULL) {

        strcpy(ind -> surname, "");
    } else {

        strcpy(ind -> surname, token);
    }
    

    return ind;

}
GEDCOMobject * JSONtoGEDCOM(const char * str) {

    if (str == NULL) {
        return NULL;
    }

    if (strlen(str) == 0) {
        return NULL;
    }
    GEDCOMobject * obj = createGEDCOMobj();
    if (obj == NULL) {

        return NULL;
    }

    char * string = calloc(1, sizeof(char) * 20000);
    strcpy(string, "");
    int i = 0;
    strcpy(string, str);
    char ** array = NULL;
    array = malloc(sizeof(char * ) * 100);
    char * token = NULL;
    int j = 0;
    token = strtok(string, "\":{},");
    if (strcmp(token, "source") != 0) {

        return NULL;
    }
    

    while (token != NULL) {

        array[i] = malloc(sizeof(char) * 1000);
        strcpy(array[i], "");
        strcpy(array[i], token);

       
        i++;
        token = strtok(NULL, "\":{},");
        if (token == NULL) {

            break;
        }
    }

    for (j = 0; j < 9; j++) {
        if (strcmp(array[j], "source") == 0) {
            
            if (strcmp(array[j + 1], "gedcVersion") == 0) {

                return NULL;

            }

        }
        if (strcmp(array[j], "gedcVersion") == 0) {

            if (strcmp(array[j + 1], "encoding") == 0) {

                return NULL;
            }

        }
        if (strcmp(array[j], "encoding") == 0) {

            if (strcmp(array[j + 1], "subName") == 0) {

                return NULL;

            }

        }
        if (strcmp(array[j], "subName") == 0) {

            if (strcmp(array[j + 1], "subAddress") == 0) {

                return NULL;
            }

        }

    }

    for (j = 0; j < 9; j++) {

        if (strcmp(array[j], "source") == 0) {

            if (strcmp(array[j + 1], "") != 0) {

                strcpy(obj -> header -> source, array[j + 1]);

            }

        }
        if (strcmp(array[j], "gedcVersion") == 0) {

            if (strcmp(array[j + 1], "") != 0) {

                double x = atof(array[j + 1]);
                obj -> header -> gedcVersion = x;

            }

        }
        if (strcmp(array[j], "encoding") == 0) {

            if (strcmp(array[j + 1], "") != 0) {

                CharSet set = convertStringToEncoding(array[j + 1]);
                obj -> header -> encoding = set;

                obj -> header -> otherFields = initializeList(printField, deleteField, compareFields);

            }

        }

        if (strcmp(array[j], "subName") == 0) {

            if (strcmp(array[j + 1], "") != 0) {

                strcpy(obj -> submitter -> submitterName, array[j + 1]);
                obj -> header -> submitter = obj -> submitter;
                obj -> submitter -> otherFields = initializeList(printField, deleteField, compareFields);

            }

        }
        if (strcmp(array[j], "subAddress") == 0) {

            if (array[j + 1] == NULL) {

                
                strcpy(obj -> submitter -> address, "");

            } else {

                strcpy(obj -> submitter -> address, array[j + 1]);

            }

        }

    }
    return obj;

}

void addIndividual(GEDCOMobject * obj,
    const Individual * toBeAdded) {

    if (obj == NULL || toBeAdded == NULL) {

        return;

    }

    insertBack( & obj -> individuals, (Individual * ) toBeAdded);

}
char * iListToJSON(List iList) {

    Node * temp = iList.head;
    char * string = NULL;
    int len = 999;
    int counter = 0;
    char * string2 = malloc(sizeof(char) * 200000);
    strcpy(string2, "");
    strcat(string2, "[");
    while (temp != NULL) {

        Individual * indi = (Individual * ) temp -> data;
        string = indToJSON(indi);
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
char * gListToJSON(List gList) {
    Node * temp = gList.head;
    char * string = NULL;
    int len = 999;
    int counter = 0;
    char * string2 = calloc(1, sizeof(char) * 9999);
    strcpy(string2, "");
    strcat(string2, "[");
    while (temp != NULL) {

        List * li = temp -> data;
        string = iListToJSON( * li);
        if (strlen(string) == 0) {
            break;

        }
        
        strcat(string2, string);
        if (counter != gList.length - 1) {
            strcat(string2, ",");
        }
        
        len = len + 999;
        counter++;
        temp = temp -> next;
    }

    strcat(string2, "]");

    return string2;

}
List getDescendantListN(const GEDCOMobject * familyRecord,
    const Individual * person, unsigned int maxGen) {
    if (maxGen == -1) {
        maxGen = 9;

    }
    if (familyRecord == NULL || person == NULL) {
        return initializeList(printIndividual, fakeDelete, compareIndividuals);

    }
    if (familyRecord -> families.length == 0 || familyRecord -> individuals.length == 0) {

        return initializeList(printIndividual, fakeDelete, compareIndividuals);
    }
    Node * t = familyRecord -> families.head;
    if (((Family * ) t -> data) -> children.length == 0) {

        return initializeList(printIndividual, fakeDelete, compareIndividuals);
    }
    if (maxGen == 0) {
        maxGen = 50;

    }

    List masterList = initializeList(printIndividual, fakeDelete, compareIndividuals);
    List masterList2 = initializeList(printIndividual, fakeDelete, comparePointers);
    List * des2 = malloc(sizeof(List));
    * des2 = initializeList(printIndividual, fakeDelete, compareIndividuals);
    * des2 = wrapperGetDescendantN(des2, person, maxGen);

    Node * temp = des2 -> head;

    for (int i = 0; i < maxGen; i++) {

        List * l = malloc(sizeof(List));
        * l = initializeList(printIndividual, deleteIndividual, compareLists);
        insertBack( & masterList, l);
    }

    while (temp != NULL) {

        Descendant * d = (Descendant * ) temp -> data;
        Node * n = masterList.head;
        for (int i = 0; i < d -> level - 1 && n != NULL; i++) {
            n = n -> next;
        }
        if (n != NULL) {

            insertSorted((List * )(n -> data), d -> ptr);
        }
        temp = temp -> next;
    }
    Node * m = masterList.head;
    while (m != NULL) {
        insertFront( & masterList2, m -> data);
        m = m -> next;
    }
    Node * m1 = masterList2.head;
    while (m1 != NULL) {

        List l1 = * (List * ) m1 -> data;

        if (l1.length == 0) {

            Node * s = deleteDataFromList( & masterList2, m1 -> data);
            free(s);
        }

        m1 = m1 -> next;
    }
    return masterList2;

}
void deleteGeneration(void * toBeDeleted) {

}

int compareGenerations(const void * first,
    const void * second) {

    return 0;
}
char * printGeneration(void * toBePrinted) {

    char * string = malloc(sizeof(char) * 100);
    strcpy(string, "");
    return string;

}
List getAncestorListN(const GEDCOMobject * familyRecord,
    const Individual * person, int maxGen) {
    if (familyRecord == NULL || person == NULL) {

        return initializeList(printIndividual, deleteIndividual, compareIndividuals);
    }
    if (maxGen == 0) {

        maxGen = 50;
    }
    List masterList = initializeList(printIndividual, fakeDelete, compareIndividuals);
    List masterList2 = initializeList(printIndividual, fakeDelete, comparePointers);
    List * des3 = malloc(sizeof(List));
    * des3 = initializeList(printIndividual, fakeDelete, compareIndividuals);

    * des3 = wrapperGetAncestorN(des3, person, maxGen);

    if (familyRecord -> families.length == 0 || familyRecord -> individuals.length == 0) {

        return initializeList(printIndividual, fakeDelete, compareIndividuals);
    }
    Node * t = familyRecord -> families.tail;
    if (((Family * ) t -> data) -> children.length == 0) {
        return initializeList(printIndividual, fakeDelete, compareIndividuals);
    }
    for (int i = 0; i < maxGen; i++) {

        List * l = malloc(sizeof(List));
        * l = initializeList(printIndividual, deleteIndividual, compareLists);
        insertBack( & masterList, l);
    }

    Node * temp = des3 -> head;
    int flag = 0;
    while (temp != NULL) {

        Descendant * d = (Descendant * ) temp -> data;
        Node * n = masterList.head;
        for (int i = 0; i < d -> level - 1 && n != NULL; i++) {
            n = n -> next;
        }
        if (n != NULL) {

            Node * a = ((List * )(n -> data)) -> head;
            while (a != NULL) {

                Individual * ind = (Individual * ) a -> data;
                if (strcmp(d -> ptr -> givenName, ind -> givenName) == 0 && strcmp(d -> ptr -> surname, ind -> surname) == 0) {

                    flag = 1;
                }
                a = a -> next;
            }
            if (flag != 1) {
                insertSorted((List * )(n -> data), d -> ptr);
            }

        }
        temp = temp -> next;
        flag = 0;
    }
    Node * m = masterList.head;
    while (m != NULL) {
        insertFront( & masterList2, m -> data);
        m = m -> next;
    }
    Node * m1 = masterList2.head;
    while (m1 != NULL) {

        List l1 = * (List * ) m1 -> data;

        if (l1.length == 0) {

            Node * s = deleteDataFromList( & masterList2, m1 -> data);
            free(s);
        }

        Node * t = l1.head;
        while (t != NULL) {
            t = t -> next;
        }


        m1 = m1 -> next;
    }
    return masterList2;

}
List wrapperGetDescendantN(List * des2,
    const Individual * person, unsigned int maxGen) {

    if (person == NULL || maxGen < 0) {

        return *des2;

    }

    Node * temp = person -> families.head;

    while (temp != NULL) {

        Family * f = (Family * ) temp -> data;
        if (f -> wife == person || f -> husband == person) {
            Family * fam = (Family * ) temp -> data;
            Node * node = fam -> children.head;

            while (node != NULL) {
                Descendant * descendant = createDescendant();
                Individual * indi = ((Individual * ) node -> data);
                descendant -> level = maxGen;
                descendant -> ptr = indi;

                if (maxGen > 0) {

                    * des2 = wrapperGetDescendantN(des2, indi, maxGen - 1);
                    insertBack(des2, descendant);
                }
                node = node -> next;
            }

        }
        temp = temp -> next;
    }

    return *des2;

}
List wrapperGetAncestorN(List * des2,
    const Individual * person, unsigned int maxGen) {

    if (person == NULL) {
        return *des2;

    }
    Node * temp = person -> families.head;
    while (temp != NULL) {

        Family * fam = (Family * ) temp -> data;
        Node * child = fam -> children.head;
    
        while (child != NULL) {

            Individual * indi = (Individual * ) child -> data;
            
            if (indi == person) {
                
                if (maxGen > 0) {
                    if (fam -> husband != NULL) {

                        Descendant * d = createDescendant();
                        d -> level = maxGen;
                        d -> ptr = fam -> husband;
                        * des2 = wrapperGetAncestorN(des2, fam -> husband, maxGen - 1);
                        insertBack(des2, d);

                    }
                    if (fam -> wife != NULL) {

                        Descendant * d = createDescendant();
                        d -> level = maxGen;
                        d -> ptr = fam -> wife;
                        * des2 = wrapperGetAncestorN(des2, fam -> wife, maxGen - 1);
                        insertBack(des2, d);

                    }

                }

            }

            child = child -> next;
        }

        temp = temp -> next;
    }
    return *des2;
}
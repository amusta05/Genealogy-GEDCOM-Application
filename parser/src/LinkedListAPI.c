
#include <stdio.h>
#include "LinkedListAPI.h"
#include <string.h>
#include <stdlib.h>

List initializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second)){

    List list;
    list.head = NULL;
    list.tail = NULL;
    list.length = 0;
    list.printData = printFunction;
    list.compare = compareFunction;
    list.deleteData = deleteFunction;
    return list;
}

Node* initializeNode(void* data){

    Node *node = malloc(sizeof(Node));
    node->next = NULL;
    node->previous = NULL;
    node->data = data;
    return node;
}
void insertFront(List *list, void *toBeAdded){
    if(list == NULL){
         return;
     }
      
    Node*temp = initializeNode(toBeAdded);
    Node *itr;
    if(list->head == NULL){
        list->head = temp;
    	list->tail = temp;
        list->length++;
    }
    else{
        itr = list->head;
        list->head = temp;
        temp->next = itr;
        itr->previous  = list->head;
        list->length++;
    }
}
void insertBack(List *list, void *toBeAdded){

    if(list == NULL || toBeAdded == NULL){
        return;
    }

    Node* newNode = initializeNode(toBeAdded);
	
    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }
    else{
	    newNode->previous = list->tail;
        list->tail->next = newNode;
    	list->tail = newNode;
    }
    list->length++;

}


void insertSorted(List *list,void *toBeAdded){


    if (list == NULL || toBeAdded == NULL){
        return;
	}
	
	if (list->head == NULL){	
	    insertBack(list, toBeAdded);
		return;
	}
	
	if (list->compare(toBeAdded, list->head->data) <= 0){	
	    insertFront(list, toBeAdded);
		return;
	}
	
	if (list->compare(toBeAdded, list->tail->data) > 0){	
		insertBack(list, toBeAdded);
		return;
	}
	
    Node* currNode = list->head;
	
    while (currNode != NULL){
	    if (list->compare(toBeAdded, currNode->data) <= 0){   
		     Node* newNode = initializeNode(toBeAdded);
			 newNode->next = currNode;
			 newNode->previous = currNode->previous;
		     currNode->previous->next = newNode;
			 currNode->previous = newNode;
		     list->length++;
			return;
	    }
	    currNode = currNode->next;
	}
	
	
}


void* getFromFront(List list){
    if(list.head != NULL){
	    return list.head->data;
    }
	return NULL;
}

void* getFromBack(List list){
    if(list.tail != NULL){
	    return list.tail->data;
	}
	return NULL;
}
void clearList(List* list){

    if (list == NULL){
	    return;
	}
	
	if (list->head == NULL && list->tail == NULL){
	    return;
	}
	
	Node* tmp = NULL;
	
	while (list->head != NULL){
	    list->deleteData(list->head->data);
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
		
	}
	
	list->head = NULL;
	list->tail = NULL;

}
void* deleteDataFromList(List* list, void* toBeDeleted){

    if(list == NULL){
        return NULL;
    }
    if(toBeDeleted == NULL){
        return NULL;
    }
    Node *itr = list->head;
    void *data  = NULL;
    while(itr != NULL){
        data = itr->data;
        if(list->compare(data,toBeDeleted)==0){
  
       	    if(itr->previous == NULL){
          
       	   	    list->head = itr->next;
                if(itr->next != NULL){
       	   	        itr->next->previous = NULL;
                    list->length--;
                }
                list->deleteData(itr->data);
       	   	    free(itr);
                return data;   
       	   }
       	   else if(itr->next == NULL){
      
       	       list->tail = itr->previous;
       	   	   itr->previous->next = NULL;
               list->deleteData(itr->data);
       	   	   free(itr);
               list->length--;
               return data;
           
       	   }
       	   else {
       	       itr->previous->next = itr->next;
       	   	   itr->next->previous = itr->previous;
               list->deleteData(itr->data);
       	       free(itr);
               list->length--;
               return data;  	 
       	   }
       }
      
       itr = itr->next;
   }

   return NULL;

}
char* toString(List list){

    ListIterator iter = createIterator(list);
    char* str;
		
	str = (char*)malloc(sizeof(char));
	strcpy(str, "");
	
	void* elem;
	while( (elem = nextElement(&iter)) != NULL){
	    char* currDescr = list.printData(elem);
		int newLen = strlen(str)+50+strlen(currDescr);
		str = (char*)realloc(str, newLen);
		strcat(str, "\n");
		strcat(str, currDescr);
		
		free(currDescr);
	}
	
	return str;
}


ListIterator createIterator(List list){

    ListIterator listIterator;
    listIterator.current = list.head;
    return listIterator;

}
void* nextElement(ListIterator* iter){

    void *data = NULL;
    if(iter->current ){
        data = iter->current->data;
        iter->current = iter->current->next;
    }
    return data;
}
int getLength(List list)
{
    Node *temp = list.head;
    int length = 0;
    while(temp != NULL){
        length++;
        temp = temp->next;
    }
    return length;
}
void* findElement(List list, bool (*customCompare)(const void* first,const void* second), const void* searchRecord)
{
    Node *temp = list.head;
    
    while(temp != NULL)
    {
        if(customCompare(temp->data,searchRecord)== true){
            return temp->data;
        }
        else{
            temp = temp->next;
        }
    }
    return NULL;
}

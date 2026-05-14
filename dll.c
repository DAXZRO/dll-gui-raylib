//dll.c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dll.h"

node* head = NULL;
node* tail = NULL;

DLLStatus insert_beg(int x){
    node *newnode = (node*)malloc(sizeof(node));

    if(!newnode){
        return DLL_ALLOC_FAILED;
    }

    newnode->data = x;
    newnode->next = NULL;
    newnode->prev = NULL;

    if(!head){
        head = tail = newnode;
    }
    else{
        newnode->next = head;
        head-> prev = newnode;
        head = newnode;
    }

    return DLL_OK;
}

DLLStatus insert_mid(int pos, int x){
    if(pos <= 0){
        return DLL_INVALID_POS;
    }
    
    if(pos == 1){
        return insert_beg(x);
    }

    if(!head && pos != 1){
        return DLL_INVALID_POS;
    }
    node *temp = head;
    for(int i = 0; i < pos - 2; i++){
        if(!temp->next){
            return DLL_INVALID_POS;
        }
        temp = temp->next;
    }

    if(temp->next == NULL){
        return insert_end(x);
    }

    node *newnode = (node*)malloc(sizeof(node));
    if(!newnode){
        return DLL_ALLOC_FAILED;
    }

    newnode->data = x;
    newnode->next = NULL;
    newnode->prev = NULL;

    newnode->next = temp->next;
    newnode->prev = temp;
    temp->next->prev = newnode;
    temp->next = newnode;

    return DLL_OK;
}

DLLStatus insert_end(int x){
    node *newnode = (node*)malloc(sizeof(node));

    if(!newnode){
        return DLL_ALLOC_FAILED;
    }

    newnode->data = x;
    newnode->next = NULL;
    newnode->prev = NULL;

    if(!head){
        head = tail = newnode;
    }
    else{
        newnode->prev = tail;
        tail->next = newnode;
        tail = newnode;
    }

    return DLL_OK;
}

DLLStatus insert_before(int x, int y){
    if(!head){
        return DLL_EMPTY;
    }

    node *temp = head;

    while(temp){
        if(temp->data == y){
            break;
        }
        temp = temp->next;
    }

    if(!temp){
        return DLL_ELEMENT_NOT_FOUND;
    }

    node *newnode = (node*)malloc(sizeof(node));

    if(!newnode){
        return DLL_ALLOC_FAILED;
    }

    newnode->data = x;

    if(temp->prev == NULL){
        newnode->next = head;
        newnode->prev = NULL;
        head->prev = newnode;
        head = newnode;
        return DLL_OK;
    }

    newnode->next = temp;
    newnode->prev = temp->prev;
    temp->prev->next = newnode;
    temp->prev = newnode;
    
    return DLL_OK;
}

DLLStatus insert_after(int x, int y){
    if(!head){
        return DLL_EMPTY;
    }

    node *temp = head;

    while(temp){
        if(temp->data == y){
            break;
        }
        temp = temp->next;
    }

    if(!temp){
        return DLL_ELEMENT_NOT_FOUND;
    }

    node *newnode = (node*)malloc(sizeof(node));
    if(!newnode){
        return DLL_ALLOC_FAILED;
    }
    newnode->data = x;


    if(temp->next == NULL){
        newnode->next = NULL;
        newnode->prev = tail;
        tail->next = newnode;
        tail = newnode;
        return DLL_OK;
    }

    newnode->next = temp->next;
    newnode->prev = temp;
    temp->next->prev = newnode;
    temp->next = newnode;

    return DLL_OK;
}

DLLStatus del_beg(){
    // empty list
    if(!head){
        return DLL_EMPTY;
    }
    // single node
    if(head == tail){
        free(head);
        head = tail = NULL;
        return DLL_OK;
    }

    // general case
    node *temp = head;
    head = head->next;
    head->prev = NULL;
    free(temp);

    return DLL_OK;
}

DLLStatus del_mid(int pos){
    // invalid position
    if(pos <= 0){
        return DLL_INVALID_POS;
    }

    // empty list 
    if (!head){
        return DLL_EMPTY;
    }

    // single node
    if(pos == 1){
        return del_beg();
    }

    node *temp = head;
    for(int i = 0; i < pos - 2; i++){
        if(temp->next == NULL){
            return DLL_INVALID_POS;
        }
        temp = temp->next;
    }

    node *del = temp->next;
    
    // invalid pos
    if(del == NULL){
        return DLL_INVALID_POS;
    }

    // last node 
    if(del->next == NULL){
        return del_end();
    }

    // general case
    temp->next = del->next;
    del->next->prev = temp;
    free(del);

    return DLL_OK;
}

DLLStatus del_end(){
    // empty list
    if(!head){
        return DLL_EMPTY;
    }
    // single node
    if(head == tail){
        free(head);
        head = tail = NULL;
        return DLL_OK;
    }

    // general case
    node* temp = tail;
    tail = tail->prev;
    tail->next = NULL;
    free(temp);
    
    return DLL_OK; 
}

DLLStatus del_element(int x){
    if(!head){
        return DLL_EMPTY;
    }

    int found = 0;

    node *temp = head;
    while(temp != NULL){
        if(temp->data == x){
            found = 1;
            break;
        }
        temp = temp->next;
    }

    if(found == 0){
        return DLL_ELEMENT_NOT_FOUND;
    }

    if(temp->prev == NULL){
        return del_beg();
    }

    if(temp->next == NULL){
        return del_end();
    }

    node *temp1 = temp->prev;
    temp1->next = temp->next;
    temp->next->prev = temp1;
    free(temp);

    return DLL_OK;
}

DLLStatus del_all_occurances(int x){
    if(!head){
        return DLL_EMPTY;
    }

    int found = 0;
    node *temp = head;

    while(temp){

        if(temp->data == x){

            found = 1;
            node *next = temp->next; 

            if(temp->prev == NULL){
                del_beg(); 
                temp = next; 
                continue;
            }

            if(temp->next == NULL){
                del_end(); 
                temp = next; 
                continue;
            }

            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;

            free(temp);
            temp = next;
        }
        else{
            temp = temp->next;
        }
    }

    if(!found){
        return DLL_ELEMENT_NOT_FOUND;
    }

    return DLL_OK;
}

DLLStatus del_duplicates(void){
    if(!head){
        return DLL_EMPTY;
    }

    if(!head->next){
        return DLL_OK;
    }

    sort();

    node* temp = head;

    while(temp && temp->next){

        if(temp->data == temp->next->data){

            node *del = temp->next;
            temp->next = del->next;

            if(del->next == NULL){
                tail = temp;
            }
            else{
                del->next->prev = temp;
            }
            free(del);
        }
        else{ 
            temp = temp->next;
        }
    }
    return DLL_OK;
}

DLLStatus update(int pos, int x){   
    if(pos <= 0){
        return DLL_INVALID_POS;
    }

    if(!head){
        return DLL_EMPTY;
    }

    node *temp = head;
    for(int i = 0; i < pos - 1; i++){
        if(temp->next == NULL){
            return DLL_INVALID_POS;
        }
        temp = temp->next;
    }

    temp->data = x;
    return DLL_OK;
}

DLLStatus sort(void){
    if(!head){
        return DLL_EMPTY;
    }

    if(head->next == NULL){
        return DLL_OK;
    }

    int total = node_counter(&total);
    
    for(int i = 0; i < total; i++){
        node *temp = head; 
        
        for(int j = 0; j < total - i - 1; j++){    
            if(temp->next != NULL && temp->data > temp->next->data){
                int t = temp->data;
                temp->data = temp->next->data;
                temp->next->data = t;
            }
            temp = temp->next;
        }
    }
    
    return DLL_OK;
}

DLLStatus clear_list(void){
    if(!head){
        return DLL_OK;
    }
    node *temp = head;
    while(temp){
        node *next = temp->next;
        free(temp);
        temp = next;
    }
    head = tail = NULL;

    return DLL_OK;
}

DLLStatus node_counter(int *result){
    int count = 0;
    node *temp = head;
    
    while(temp){
        count += 1;
        temp = temp->next;
    }

    *result = count;
    return DLL_OK;
}

DLLStatus search(int x, int *result){
    if(!head){
        return DLL_EMPTY;
    }
    int count = 0;
    node *temp = head;

    while(temp){
        count += 1;
        if(temp->data == x){
            break;
        }
        temp = temp->next;
    }

    if(!temp){
        return DLL_ELEMENT_NOT_FOUND;
    }

    *result = count;
    return DLL_OK;
}

DLLStatus get_element(int pos, int *result){
    if(pos <= 0){
        return DLL_INVALID_POS;
    }
    
    if(!head){
        return DLL_EMPTY;
    }

    node *temp = head;
    for(int i = 0; i < pos - 1; i++){
        if(temp->next == NULL){
            return DLL_INVALID_POS;
        }
        temp = temp->next;
    }

    *result = temp->data;
    return DLL_OK;
}
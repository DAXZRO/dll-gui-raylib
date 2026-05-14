#include <stdio.h>
#include <stdlib.h>

typedef struct DLLNode{
    int data;
    struct DLLNode* next;
    struct DLLNode* prev;
} node;

node* head = NULL;
node* tail = NULL;

typedef enum{
    DLL_OK,
    DLL_EMPTY,
    DLL_INVALID_POS,
    DLL_NOT_FOUND,
    DLL_ELEMENT_NOT_FOUND
}DLLStatus;

void menu(void);
void insert(int x);
DLLStatus del_beg(void);
DLLStatus del_mid(int pos);
DLLStatus del_end(void);
DLLStatus del_element(int x);
DLLStatus del_all_occurances(int x);
DLLStatus del_duplicates(void);
void display(void);
const char* status_msg(DLLStatus status);

int main(){
    DLLStatus status;

    insert(1);
    insert(2);
    insert(3);
    insert(4);
    insert(5);
    insert(6);
    insert(7);
    insert(8);
    insert(9);
    insert(10);
    display();

    while(1){
        int choice;
        menu();
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:{
                status = del_beg();
                break;    
            }
            case 2:{
                int pos;
                printf("Enter position: ");
                scanf("%d", &pos);
                status = del_mid(pos);
                break;
            }
            case 3:{
                status = del_end();
                break;
            }
            case 4:{
                display();
                break;
            }
            default:{
                return 0;
            }
        }
        const char *msg = status_msg(status);
        printf("%s", msg);
        display();
    }
    return 0;
}

void menu(void){
    printf("\n MENU \n");
    printf("1. Delete beginning\n");
    printf("2. Delete middle\n");
    printf("3. Delete end\n");
    printf("4. Display\n");
}   

void insert(int x){
    node* new = (node*)malloc(sizeof(node));
    if(new == NULL)
        return;

    new->data = x;
    new->next = NULL;
    new->prev = tail;

    if(!head){
        head = tail = new;
    }
    else{
        tail->next = new;
        tail = new;
    }
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

void display(void){
    node *temp = head;

    printf("\nNULL <- ");

    while(temp){
        printf("%d", temp->data);

        if(temp->next)
            printf(" <-> ");

        temp = temp->next;
    }

    printf(" -> NULL\n");
}

const char* status_msg(DLLStatus status){

    switch(status){

        case DLL_OK:
            return "DLL_OK";

        case DLL_NOT_FOUND:
            return "DLL_NOT_FOUND";

        case DLL_INVALID_POS:
            return "DLL_INVALID_POSITION";

        case DLL_EMPTY:
            return "DLL_EMPTY";

        default:
            return "UNKNOWN_STATUS";
    }
}
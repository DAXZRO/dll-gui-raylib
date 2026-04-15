//dll.c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dll.h"
struct node{
    int data;
    struct node* prev;
    struct node* next;
};

struct node* head=NULL;
struct node* tail=NULL;

void insert_beginning(int x){
    struct node* new=(struct node*)malloc(sizeof(struct node));
    new->data=x;
    new->next=head;
    new->prev=NULL;
    if(head!=NULL)
        head->prev=new;
    else tail=new;
    head=new;
}
void insert_random(int pos, int x){

    if(pos <= 1 || head == NULL){
        insert_beginning(x);
        return;
    }

    struct node* new = malloc(sizeof(struct node));
    new->data = x;

    struct node* temp = head;

    for(int i = 1; i < pos-1 && temp->next != NULL; i++)
        temp = temp->next;

    new->next = temp->next;
    new->prev = temp;

    if(temp->next != NULL)
        temp->next->prev = new;
    else
        tail = new;

    temp->next = new;
}
void insert_last(int x){
    struct node* new=(struct node *)malloc(sizeof(struct node));
    if(head==NULL){
        head=new;
        tail=new;
        new->data=x;
        new->next=NULL;
        new->prev=NULL;
        return;
    }
    new->data=x;
    new->next=NULL;
    new->prev=tail;
    tail->next=new;
    tail=new;
}
void delete_beginning(){
    if(head==NULL){
        printf("ERROR: EMPTY LIST");
        return;
    }
    struct node* temp=head;
    head=head->next;
    head->prev=NULL;
    free(temp);
}
void delete_random(int pos){
    if(head==NULL){
        printf("ERROR: EMPTY LIST");
        return;
    }
    struct node*temp=head;
    for(int i=0;i<pos-2;i++) temp=temp->next;
    struct node* temp1=temp->next;
    temp->next=temp1->next;
    temp1->next->prev=temp;
    free(temp1); 
}
void delete_last(){
    if(head==NULL){
        printf("ERROR: EMPTY LIST");
        return;
    }
    struct node* temp=tail;
    tail=tail->prev;
    tail->next=NULL;
    free(temp);
}
void del_first_occ(int x){
    if(head==NULL){
        printf("ERROR: EMPTY LIST");
        return;
    }
    struct node* temp=head;
    while(temp->next->data!=x) temp=temp->next;
    struct node* temp1=temp->next;
    temp->next=temp1->next;
    temp1->prev=temp;
    free(temp1);
}
int counter(){
    struct node* temp=tail;
    int count=0;
    while(temp!=NULL) {
        count++;
        temp=temp->prev;
    }
    return count;
}
void sort(){
    if(head == NULL) return;
    int n=counter();
    for(int i = 0; i < n - 1; i++){
        struct node* temp = head;
        for(int j = 0; j < n - i - 1; j++){
            if(temp->next != NULL && temp->data > temp->next->data){
                int t = temp->data;
                temp->data = temp->next->data;
                temp->next->data = t;
            }
            temp = temp->next;
        }
    }
}
int search(int x){
    int found=0,loc=1;
    struct node* temp=head;
    int n=counter();
    for(int i=0;i<n;i++){
        if(temp->data==x){
            found=1;
            break;
        }
        temp=temp->next;
        loc++;
    }
    if(found) return loc;
    else return -1;
}
void reverse(){
    struct node* start=head;
    struct node* end=tail;
    int n=counter();
    for(int i=0;i<n/2;i++){
        int temp=start->data;
        start->data=end->data;
        end->data=temp;
        start=start->next;
        end=end->prev;
    }
    printf("After REVERSAL ");
}
int get_ele(int pos){

    if(head == NULL)
        return -1;

    struct node* temp = head;
    int i = 1;

    while(temp != NULL && i < pos){
        temp = temp->next;
        i++;
    }

    if(temp == NULL)
        return -1;

    return temp->data;
}
void update(int pos,int x){
    struct node* temp=head;
    for(int i=0;i<pos-1;i++) temp=temp->next;
    temp->data=x;
}
void del_all_occ(int x){
    struct node* temp=head;
    while(head!=NULL && head->data==x){
        delete_beginning();
    }
    while(tail!=NULL && tail->data==x){
        delete_last();   
    }
    while(temp->next!=NULL){
        if(temp->next->data==x){
            struct node* temp1=temp->next;
            temp->next=temp1->next;
            temp1->next->prev=temp;
            free(temp1);
        }
        else temp=temp->next;
    }
}
void del_dupes(int x){
    struct node* temp=head;
    int count=0;
    while(temp->next!=NULL){
        if(temp->next->data==x){
            count++;
            if(count>1){
                if(temp->next==tail) delete_last();
                else{
                    struct node*temp1=temp->next;
                    temp->next=temp1->next;
                    temp1->next->prev=temp;
                    free(temp1);
                }
            }
            else temp=temp->next;
        }
        else temp=temp->next;
    }
}
void insert_after(int x, int y){
    struct node* new=(struct node*)malloc(sizeof(struct node));
    struct node*temp=head;
    while(temp->data!=x){
        temp=temp->next;
        if(temp==NULL){
            printf("Element Not Found");
            return;
        }
    }
    if(temp==tail){
        insert_last(y);
        return;
    }
    new->data=y;
    new->next=temp->next;
    new->prev=temp;
    temp->next->prev=new;
    temp->next=new;
}
void insert_before(int x, int y){
    struct node* new=(struct node*)malloc(sizeof(struct node));
    struct node*temp=head;
    while(temp->data!=x){
        temp=temp->next;
        if(temp==NULL){
            printf("Element Not Found");
            return;
        }
    }
    if(temp==head){
        insert_beginning(y);
        return;
    }
    new->data=y;
    new->next=temp;
    new->prev=temp->prev;
    temp->prev->next=new;
    temp->prev=new;
}
void clear_list(){
    struct node *temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }   
    tail = NULL;
}
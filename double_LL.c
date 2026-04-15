//double linked list
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
struct node{
    int data;
    struct node* prev;
    struct node* next;
};
struct node* head=NULL;
struct node* tail=NULL;
int input(const char* text);
void insert_beginning(int x);
void insert_random(int pos, int x);
void insert_last(int x);
void insert_before(int x, int y);
void insert_after(int x, int y);
void delete_beginning();
void delete_random(int pos);
void delete_last();
void del_first_occ(int x);
void del_all_occ(int x);
void del_dupes(int x);
int get_ele(int pos);
void update(int pos, int x);
void print(void);
void rev_print(void);
int counter(void);
void sort(void);
void search(int x);
void reverse(void);
void menu(void);
int main(){
    while(1){
        menu();
        int x;
        int choice=input("your choice");
        switch(choice){
            case 1: {
                    printf("1. Order of insertion\n");
                    printf("2. Reversed order of insertion\n");
                    int ask=input("your choice"),n;
                    int exit=1;
                    while(exit==1){
                        switch(ask){
                            case 1: {
                                    int a;
                                    n=input("no. of elements");
                                    printf("Enter elements: ");
                                    for(int i=0;i<n;i++){
                                        scanf("%d",&a);
                                        insert_last(a);
                                    }
                                    exit=0;
                                    break;
                                }
                            case 2: {
                                    n=input("no. of elements");
                                    int b;
                                    printf("Enter elements: ");
                                    for(int i=0;i<n;i++){
                                        scanf("%d",&b);
                                        insert_beginning(b);
                                    }
                                    exit=0;
                                    break;
                                }
                            default:{
                                    printf("INVALID CHOICE");
                                    break;
                                }
                        }
                    }
                    printf("List successfully created");
                    break;
                }
            case 2:{ 
                    x=input("value");
                    insert_beginning(x);
                    printf("Element successfully entered");
                    break;
                }
            case 3: {
                    int pos=input("position");
                    x=input("element");
                    insert_random(pos,x);
                    printf("Element successfulyy entered");
                    break;
                }
            case 4: {
                    x=input("value");
                    insert_last(x);
                    printf("Element succesfully entered");
                    break;
                }
            case 5: {
                    delete_beginning();
                    printf("Element successfully deleted");
                    break;
                }
            case 6: {
                    int posi=input("position");
                    delete_random(posi);
                    printf("Element succesfully deleted");
                    break;
                }
            case 7:{ 
                    delete_last();
                    printf("Element successfully deleted");
                    break;
                }
            case 8: {
                    int c=input("element");
                    del_first_occ(c);
                    break;
                }
            case 9: {
                    print();
                    break;
                }
            case 10:{
                    int total=counter();
                    printf("Total no. of nodes: %d",total);
                    break;
                }
            case 11:{
                    sort();
                    print();
                    break;    
                }
            case 12:int d=input("value to search");
                    search(d);
                    break;
            case 13:reverse();
                    break;
            case 14:{
                    int h=input("location element");
                    int h1=input("element to input");
                    insert_before(h,h1);
                    printf("Element successfully entered");
                    break;
                }
            case 15:{
                    int j=input("location element");
                    int j1=input("element to input");
                    insert_after(j,j1);
                    printf("Element successfully entered");
                    break;
                }
            case 16:{
                    rev_print();
                    break;
                }
            case 17:{
                    int f=input("element");
                    del_dupes(f);
                    printf("Successfully deleted duplicate entries");
                    break;
                }
            case 18:{
                    int pos2=input("position");
                    int g=get_ele(pos2);
                    printf("Element at position %d = %d",pos2,g);
                    break;
                }
            case 19:{
                    int pos1=input("position");
                    int e=input("element");
                    update(pos1,e);
                    printf("List successfully updated");
                    break;
                }
            case 20:{
                    int i=input("element");
                    del_all_occ(i);
                    printf("elements successfully deleted");
                    break;
                }
            case 21:{
                    printf("Program ended");
                    return 0;
                }
            default:printf("Invalid Choice");
        }
    }
    return 0;
}
void menu(){
    printf("\n=============================================\n");
    printf("============= LINKED LIST MENU =============\n");
    printf("=============================================\n");
    printf("1. Quick Insert\n");
    printf("2. Insert at Beginning\n");
    printf("3. Insert at Random Position\n");
    printf("4. Insert at Last\n");
    printf("5. Delete from Beginning\n");
    printf("6. Delete from Random Position\n");
    printf("7. Delete from Last\n");
    printf("8. Delete First Occurrence of Element\n");
    printf("9. Print List\n");
    printf("10. Count Nodes\n");
    printf("11. Sort List\n");
    printf("12. Search\n");
    printf("13, Reverse\n");
    printf("14. Insert Before\n");
    printf("15. Insert After\n");
    printf("16. Print Reverse\n");
    printf("17. Remove Duplicates\n");
    printf("18. Get Element at Position\n");
    printf("19. Update Element at Position\n");
    printf("20. Delete all occurrences of Element\n");
    printf("21. Exit\n");
    printf("=============================================\n");
}
int input(const char* text){
    int var;
    while(1){
        printf("Enter %s: ",text);
        if(scanf("%d",&var)==1) break;
        printf("\nERROR: INVALID INPUT\n");
        printf("ENTER AGAIN\n");
        while(getchar()!='\n');
    }
    return var;
}
void insert_beginning(int x){
    struct node* new=(struct node*)malloc(sizeof(struct node));
    new->data=x;
    new->next=head;
    new->prev=NULL;
    head->prev=new;  
    head=new;
}
void insert_random(int pos, int x){
    struct node* new=(struct node*)malloc(sizeof(struct node));
    new->data=x;
    new->next=NULL;
    new->prev=NULL;
    struct node* temp=head;
    for(int i=0;i<pos-2;i++) temp=temp->next;
    new->next=temp->next;
    new->prev=temp;
    temp->next->prev=new;
    temp->next=new;
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
void print(){
    struct node* temp=head;
    printf("LIST IS: ");
    while(temp!=NULL) {
        printf("%d ",temp->data);
        temp=temp->next;
    }
    printf("\n");
}
void rev_print(){
    struct node* temp=tail;
    printf("REVSERSED LIST IS: ");
    while(temp!=NULL) {
        printf("%d ",temp->data);
        temp=temp->prev;
    }
    printf("\n");
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
void search(int x){
    int found=0,loc=1;
    struct node* temp=head;
    int n=counter();
    for(int i=0;i<n;i++){
        if(temp->data==x){
            found=1;
            break;
        }
        temp=temp->next;
    }
    if(found) printf("Element found at %d",loc);
    else printf("ELement not found");
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
    print();
}
int get_ele(int pos){
    struct node* temp=head;
    for(int i=0;i<pos-1;i++) temp=temp->next;
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
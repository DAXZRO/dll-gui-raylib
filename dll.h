//dll.h
#ifndef DLL_H
#define DLL_H

struct node{
    int data;
    struct node* prev;
    struct node* next;
};

extern struct node* head;
extern struct node* tail;

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
int counter(void);
void sort(void);
int search(int x);
void reverse(void);
void clear_list(void);

#endif
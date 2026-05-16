//dll.h
#ifndef DLL_H
#define DLL_H

typedef enum{
    DLL_OK,
    DLL_EMPTY,
    DLL_INVALID_POS,
    DLL_NOT_FOUND,
    DLL_ELEMENT_NOT_FOUND,
    DLL_ALLOC_FAILED,
    DLL_INVALID_INPUT
}DLLStatus;

typedef struct node{
    int data;
    struct node *prev;
    struct node *next;
} node;

extern struct node* head;
extern struct node* tail;

// INSERTION
DLLStatus insert_beg(int x);
DLLStatus insert_mid(int pos, int x);
DLLStatus insert_end(int x);
DLLStatus insert_before(int x, int y);
DLLStatus insert_after(int x, int y);

// DELETION
DLLStatus del_beg(void);
DLLStatus del_mid(int pos);
DLLStatus del_end(void);
DLLStatus del_element(int x);
DLLStatus del_all_occurances(int x);
DLLStatus del_duplicates(void);

// UPDATION
DLLStatus update(int pos, int x);
DLLStatus sort(void);
DLLStatus clear_list(void);

// INFORMATION
DLLStatus node_counter(int *result);
DLLStatus search(int x, int *result);
DLLStatus get_element(int pos, int *result);

#endif
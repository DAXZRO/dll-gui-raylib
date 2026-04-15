//MAIN
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "raylib.h"
#include "dll.h"
//macros are undesirable
//macros should be all caps
#define main_menu 0
#define make_menu 1
#define insert_menu 2
#define delete_menu 3
#define update_menu 4
#define info_menu 5
#define exit_menu 6
#define op_menu 7
#define op_make_menu 8

void drawmainmenu(int *menu);
void drawmakemenu(int *menu);
void drawinsertmenu(int *menu);
void drawdelmenu(int *menu);
void drawupdatemenu(int *menu);
void drawinfomenu(int *menu);
int drawexitmenu(int *menu);
void drawopmenu(int location, int *menu);
void drawop_make_menu(int location, int *menu);
void drawDLL(Rectangle box);
void hover(Rectangle box);
void runoperation();
//compiler may not start from 0
typedef enum {
    OP_INSERT_BEGIN = 0,
    OP_INSERT_RANDOM,
    OP_INSERT_LAST,
    OP_INSERT_AFTER,
    OP_INSERT_BEFORE,

    OP_DEL_BEGIN,
    OP_DEL_RANDOM,
    OP_DEL_LAST,
    OP_DEL_FIRST_OCC,
    OP_DEL_ALL_OCC,
    OP_DEL_DUPES,

    OP_UPDATE,
    OP_SORT,
    OP_REVERSE,

    OP_SEARCH,
    OP_GET,
    OP_COUNT,
    OP_PRINT,

    OP_OG_ORDER,
    OP_REV_ORDER
} Operation;

Operation currentOp;

typedef struct {
    const char *title;
    int inputs;
    const char *label1;
    const char *label2;
} OperationInfo;

OperationInfo opinfo[]={
    {"INSERT AT BEGINNING",1,"Value",NULL},
    {"INSERT AT POSITION",2,"Position","Value"},
    {"INSERT AT LAST",1,"Value",NULL},
    {"INSERT AFTER X",2,"Location","Value"},
    {"INSERT BEFORE X",2,"Location","Value"},

    {"DELETE BEGINNING",0,NULL,NULL},
    {"DELETE POSITION",1,"Position",NULL},
    {"DELETE LAST",0,NULL,NULL},
    {"DELETE FIRST OCCURRENCE",1,"Value",NULL},
    {"DELETE ALL OCCURRENCES",1,"Value",NULL},
    {"DELETE DUPLICATES",1,"Value",NULL},

    {"UPDATE",2,"Position","Value"},
    {"SORT",0,NULL,NULL},
    {"REVERSE",0,NULL,NULL},

    {"SEARCH ELEMENT",1,"Value",NULL},
    {"GET ELEMENT",1,"Position",NULL},
    {"TOTAL NODES",0,NULL,NULL},
    {"",-1,NULL,NULL},

    {"ORDER OF INSERTION",2,"Number","Values"},
    {"REVERSED ORDER",2,"Number","Values"},
};
//buffer may overflow in low power devices
char buffer1[100] = "";
char buffer2[100] = "";
char temp_buffer[100] = "";
char outputmsg[50] = "";
int value = 0;
int position = 0;
int ele = 0;
int elements[100] = {0};
int ret_value = 0;
int activebox = 0;
int loc;
int showdll = 0;

int main(){
    InitWindow(800,600,"DLL Program");
    SetTargetFPS(60);
    int current_menu = main_menu;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(current_menu){ 
            case 0:{
                drawmainmenu(&current_menu);
                break;
            }
            case 1:{
                drawmakemenu(&current_menu);
                break;
            }
            case 2:{
                drawinsertmenu(&current_menu);
                break;
            }
            case 3:{
                drawdelmenu(&current_menu);
                break;
            }
            case 4:{
                drawupdatemenu(&current_menu);
                break;
            }
            case 5:{
                drawinfomenu(&current_menu);
                break;
            }
            case 6:{
                int end=drawexitmenu(&current_menu);
                if(end==1){
                    CloseWindow();
                    return 0;
                }
                break;
            }
            case 7:{
                drawopmenu(loc,&current_menu);
                break;
            }
            case 8:{
                drawop_make_menu(loc,&current_menu);
            }
        }
        EndDrawing();
    }
    CloseWindow();
}
void hover(Rectangle box){
    bool isHover = CheckCollisionPointRec(GetMousePosition(),box);
    if(isHover){
        DrawRectangleRec(box,GRAY);
    }
    else{
        DrawRectangleRec(box,LIGHTGRAY);
    }
}
void drawmainmenu(int *menu){
    Rectangle makebtn = {300, 150, 200, 50};
    Rectangle insbtn = {300, 220, 200, 50};
    Rectangle delbtn = {300, 290, 200, 50};
    Rectangle updbtn = {300, 360, 200, 50};
    Rectangle infobtn = {300, 430, 200, 50};
    Rectangle exitbtn = {300, 500, 200, 50};

    Rectangle Buttons[6]= {makebtn, insbtn, delbtn, updbtn, infobtn, exitbtn};
    char const* btnlabels[6]={"MAKE DLL", "INSERT", "DELETE", "UPDATE", "INFO", "EXIT"};
    for(int i=0;i<6;i++){
        hover(Buttons[i]);
        if(i==5){ //curly(not good practice)
            DrawText(btnlabels[i],Buttons[i].x+20,Buttons[i].y+15,20,RED);}
        else DrawText(btnlabels[i],Buttons[i].x+20,Buttons[i].y+15,20,BLACK);
        if(CheckCollisionPointRec(GetMousePosition(),Buttons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0) *menu=make_menu; 
            if(i==1) *menu=insert_menu;
            if(i==2) *menu=delete_menu;
            if(i==3) *menu=update_menu;
            if(i==4) *menu=info_menu;
            if(i==5) *menu=exit_menu;
        }
    }
    DrawText("DLL PROGRAM",300,80,30,BLACK);
}
void drawmakemenu(int *menu){
    loc=1;
    Rectangle opt1 = {300, 150, 250, 50};
    Rectangle opt2 = {300, 220, 250, 50};
    Rectangle back = {300, 290, 250, 50};

    Rectangle optarr[3]={opt1, opt2, back};
    char const* optlabels[3]={"ORIGINAL ORDER", "REVERSED ORDER", "BACK"};

    for(int i=0; i<3; i++){
        hover(optarr[i]);
        DrawText(optlabels[i], optarr[i].x+20, optarr[i].y+15, 20, BLACK);

        if(CheckCollisionPointRec(GetMousePosition(),optarr[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0) {currentOp = OP_OG_ORDER; *menu = op_make_menu;}
            if(i==1) {currentOp = OP_REV_ORDER; *menu = op_make_menu;}
            if(i==2) *menu=main_menu;
        }
    }
    DrawText("MAKE MENU", 300, 80, 30, BLACK);
}
void drawinsertmenu(int *menu){
    loc=2;
    Rectangle opt1 = {300, 150, 200, 50};
    Rectangle opt2 = {300, 220, 200, 50};
    Rectangle opt3 = {300, 290, 200, 50};
    Rectangle opt4 = {300, 360, 200, 50};
    Rectangle opt5 = {300, 430, 200, 50};
    Rectangle back = {300, 500, 200, 50};

    Rectangle optarr[6] = {opt1, opt2, opt3, opt4, opt5, back};
    char const* optlabels[6] = {"BEGINNING", "RANDOM", "LAST", "AFTER X", "BEFORE X", "BACK"};
    for(int i=0;i<6;i++){
        hover(optarr[i]);
        DrawText(optlabels[i], optarr[i].x+20, optarr[i].y+15, 20, BLACK);
        if(CheckCollisionPointRec(GetMousePosition(),optarr[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0){ currentOp = OP_INSERT_BEGIN; *menu = op_menu; }
            if(i==1){ currentOp = OP_INSERT_RANDOM; *menu = op_menu; }
            if(i==2){ currentOp = OP_INSERT_LAST; *menu = op_menu; }
            if(i==3){ currentOp = OP_INSERT_AFTER; *menu = op_menu; }
            if(i==4){ currentOp = OP_INSERT_BEFORE; *menu = op_menu; }
            if(i==5) *menu=main_menu;
        }
    }
    DrawText("INSERT MENU",300,80,30,BLACK);
}
void drawdelmenu(int *menu){
    loc=3;
    Rectangle opt1 = {300, 100, 200, 50};
    Rectangle opt2 = {300, 170, 200, 50};
    Rectangle opt3 = {300, 240, 200, 50};
    Rectangle opt4 = {300, 310, 200, 50};
    Rectangle opt5 = {300, 380, 200, 50};
    Rectangle opt6 = {300, 450, 200, 50};
    Rectangle back = {300, 520, 200, 50};

    Rectangle optarr[7] = {opt1, opt2, opt3, opt4, opt5, opt6, back};
    char const *optlabels[7] = {"BEGINNING", "RANDOM", "LAST", "First Occ", "LAST Occ", "DUPLICATES", "BACK"};
    for(int i=0;i<7;i++){
        hover(optarr[i]);
        DrawText(optlabels[i], optarr[i].x+20, optarr[i].y+15, 20, BLACK);

        if(CheckCollisionPointRec(GetMousePosition(), optarr[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0) {currentOp = OP_DEL_BEGIN; *menu = op_menu;}
            if(i==1) {currentOp = OP_DEL_RANDOM; *menu = op_menu;}
            if(i==2) {currentOp = OP_DEL_LAST; *menu = op_menu;}
            if(i==3) {currentOp = OP_DEL_FIRST_OCC; *menu = op_menu;}
            if(i==4) {currentOp = OP_DEL_ALL_OCC; *menu = op_menu;}
            if(i==5) {currentOp = OP_DEL_DUPES; *menu = op_menu;}
            if(i==6) *menu=main_menu;
        }
    }
    DrawText("DELETE MENU", 300, 60, 30, BLACK);
}
void drawupdatemenu(int *menu){
    loc=4;
    Rectangle opt1 = {300, 150, 250, 50};
    Rectangle opt2 = {300, 220, 250, 50};
    Rectangle opt3 = {300, 290, 250, 50};
    Rectangle back = {300, 360, 250, 50};

    Rectangle optarr[4]={opt1, opt2, opt3, back};
    char const* optlabels[4]={"UPDATE", "SORT", "REVERSE", "BACK"};

    for(int i=0;i<4;i++){
        hover(optarr[i]);
        DrawText(optlabels[i], optarr[i].x+20, optarr[i].y+15, 20, BLACK);
        if(CheckCollisionPointRec(GetMousePosition(),optarr[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0) {currentOp = OP_UPDATE; *menu = op_menu;}
            if(i==1) {currentOp = OP_SORT; *menu = op_menu;}
            if(i==2) {currentOp = OP_REVERSE; *menu = op_menu;}
            if(i==3) *menu=main_menu;
        }
    }
    DrawText("UPDATE MENU", 300, 80, 30, BLACK);
}
void drawinfomenu(int *menu){
    loc=5;
    Rectangle opt1 = {300, 150, 200, 50};
    Rectangle opt2 = {300, 220, 200, 50};
    Rectangle opt3 = {300, 290, 200, 50};
    Rectangle opt4 = {300, 360, 200, 50};
    Rectangle back = {300, 430, 200, 50};

    Rectangle optarr[5]={opt1, opt2, opt3, opt4, back};
    char const* optlabels[5]={"SEARCH", "GET ELEMENT(X)", "COUNT", "PRINT","BACK"};

    for(int i=0; i<5; i++){
        hover(optarr[i]);
        DrawText(optlabels[i], optarr[i].x+20, optarr[i].y+15, 20, BLACK);

        if(CheckCollisionPointRec(GetMousePosition(),optarr[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0) {currentOp = OP_SEARCH; *menu = op_menu;}
            if(i==1) {currentOp = OP_GET; *menu = op_menu;}
            if(i==2) {currentOp = OP_COUNT; *menu = op_menu;}
            if(i==3) {currentOp = OP_PRINT; *menu = op_menu;}
            if(i==4) *menu=main_menu;
        }
    }
    DrawText("INFO MENU", 300, 80, 30, BLACK);
}
int drawexitmenu(int *menu){
    Rectangle yes = {300, 150, 200, 50};
    Rectangle no = {300, 220, 200, 50};

    Rectangle optarr[2]={yes, no};
    char const* optlabels[2]={"YES", "NO"};

    for(int i=0; i<2; i++){
        
        if(i==0) DrawRectangleRec(optarr[i],RED);
        else DrawRectangleRec(optarr[i],GRAY);
        DrawText(optlabels[i], optarr[i].x+20, optarr[i].y+15, 20, BLACK);
        if(CheckCollisionPointRec(GetMousePosition(),optarr[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if(i==0) return 1;
            if(i==1){
                *menu=main_menu;
                return 2;
            }
        }
    }
    DrawText("DO YOU WANT TO EXIT THE PROGRAM?", 85, 80, 30, BLACK);
    return 0;
}
void drawopmenu(int location,int *menu){
    const char* menu_title = opinfo[currentOp].title;
    DrawText(menu_title,20,15,30,BLACK);

    Rectangle back = {300,500,200,50};
    Rectangle inputbox1 = {300,100,200,40};
    Rectangle inputbox2 = {300,150,200,40};
    Rectangle execute= {0};
    Rectangle visualBox = {0};
    Rectangle visualBox1 = {0};
    Rectangle clear = {0};
    
    DrawRectangleRec(back,RED);
    DrawText("BACK", back.x+60, back.y+15,20,BLACK);

    if(CheckCollisionPointRec(GetMousePosition(),back) &&
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        *menu = location;
        buffer1[0]='\0';
        buffer2[0]='\0';
        outputmsg[0]='\0';
        value=0;
        position=0;
        activebox=0;
        showdll=0;
    }

    if(CheckCollisionPointRec(GetMousePosition(),inputbox1) &&
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        activebox=0;
    }

    if(CheckCollisionPointRec(GetMousePosition(),inputbox2) &&
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        activebox=1;
    }
   
    int key = GetCharPressed();
    
    while(key > 0){
        if(isdigit(key)){
            if(activebox == 0){
                int len = strlen(buffer1);
                if(len < 99){
                    buffer1[len] = key;
                    buffer1[len+1] = '\0';
                }
            }
            else if(opinfo[currentOp].inputs == 2){ //2 == opinfo.....(defensive prog)
                int len = strlen(buffer2);
                if(len < 99){
                    buffer2[len] = key;
                    buffer2[len+1] = '\0';
                }
            }
        }
        key = GetCharPressed();
    }
    if(IsKeyPressed(KEY_BACKSPACE)){
        if(activebox==0){
            int len = strlen(buffer1);
            if(len>0) buffer1[len-1] = '\0';
        }
        else if(opinfo[currentOp].inputs == 2){
            int len = strlen(buffer2);
            if(len>0) buffer2[len-1] = '\0';
        }
    }
    if(activebox==0 && IsKeyPressed(KEY_DOWN)) activebox=1;
    if(IsKeyPressed(KEY_UP)) activebox=0;

    if(IsKeyPressed(KEY_ENTER)){
        if(opinfo[currentOp].inputs == 1){
            value = atoi(buffer1);
            runoperation();
            showdll=1;
        }
        else if(opinfo[currentOp].inputs == 2){
            if(activebox==0) activebox=1;
            else{
                position = atoi(buffer1);
                value = atoi(buffer2);
                runoperation();
                showdll=1;
            }
        }
    }
    if(currentOp==OP_PRINT){
        DrawText("DOUBLE LINKED LIST VISUAL REP",
        20,
        15,
        30,
        BLACK
        );
    visualBox = (Rectangle){50,50,700,400};
    DrawRectangleLinesEx(visualBox,2,BLACK);
    showdll=1;
    }
    else if(currentOp == OP_COUNT || currentOp == OP_SEARCH || currentOp == OP_GET){
        showdll = 0;

        execute = (Rectangle){20,150,200,40};
        DrawRectangleRec(execute,GRAY);
        DrawText("EXECUTE", execute.x+20, execute.y+15,20,WHITE);

        visualBox = (Rectangle){20,200,760,300};
        DrawRectangleLinesEx(visualBox,2,BLACK);

        if(currentOp == OP_COUNT){
            DrawText(outputmsg,visualBox.x+20,visualBox.y+20,30,BLACK);
        }
        else{
            DrawText(TextFormat("ENTER %s",opinfo[currentOp].label1),20,100,30,BLACK);
            DrawRectangleRec(inputbox1, activebox==0? GRAY : LIGHTGRAY);
            DrawText(buffer1,inputbox1.x+10,inputbox1.y+15,20,BLACK);
            DrawText(TextFormat("%s = %d",opinfo[currentOp].label1,value),550,100,20,BLACK);
            DrawText(outputmsg,visualBox.x+20,visualBox.y+20,30,BLACK);
        }
    }
    else if(opinfo[currentOp].inputs==0){
        execute = (Rectangle){20,50,200,40};
        DrawRectangleRec(execute,GRAY);
        DrawText("EXECUTE", execute.x+20, execute.y+15,20,WHITE);

        clear = (Rectangle){230,50,100,40};
        DrawRectangleRec(clear,RED);
        DrawText("CLEAR", clear.x+20, clear.y+15,20,WHITE);

        visualBox1 = (Rectangle){20,100,760,200};
        DrawText("CURRENT STATE",visualBox1.x+20, visualBox1.y+15,20,BLACK);
        DrawRectangleLinesEx(visualBox1,2,RED);
        if(showdll==1) drawDLL(visualBox1);

        visualBox = (Rectangle){20,350,760,200};
        DrawText("NEW STATE",visualBox.x+20,visualBox.y+15,20,BLACK);
        DrawRectangleLinesEx(visualBox,2,BLACK);

        DrawText(outputmsg,visualBox.x+20,visualBox.y+20,30,BLACK);

    }
    else if(opinfo[currentOp].inputs==1){
        DrawText(TextFormat("ENTER %s",opinfo[currentOp].label1),20,100,30,BLACK);
        DrawRectangleRec(inputbox1, activebox==0? GRAY : LIGHTGRAY);
        DrawText(buffer1,inputbox1.x+10,inputbox1.y+15,20,BLACK);
        DrawText(TextFormat("%s = %d",opinfo[currentOp].label1,value),550,100,20,BLACK);
 
        execute = (Rectangle){20,150,200,40};
        DrawRectangleRec(execute,GRAY);
        DrawText("EXECUTE", execute.x+20, execute.y+15,20,WHITE);

        clear = (Rectangle){230,150,100,40};
        DrawRectangleRec(clear,RED);
        DrawText("CLEAR", clear.x+20, clear.y+15,20,WHITE);

        visualBox = (Rectangle){20,200,760,300};
        DrawRectangleLinesEx(visualBox,2,BLACK);
    }
    else if(opinfo[currentOp].inputs==2){
        DrawText(TextFormat("ENTER %s",opinfo[currentOp].label1),20,100,30,BLACK);
        DrawRectangleRec(inputbox1, activebox==0? GRAY : LIGHTGRAY);
        DrawText(buffer1,inputbox1.x+10,inputbox1.y+15,20,BLACK);
        DrawText(TextFormat("%s = %d",opinfo[currentOp].label1,position),550,100,20,BLACK);

        DrawText(TextFormat("ENTER %s",opinfo[currentOp].label2),20,150,30,BLACK);
        DrawRectangleRec(inputbox2, activebox==1? GRAY : LIGHTGRAY);
        DrawText(buffer2,inputbox2.x+10,inputbox2.y+15,20,BLACK);
        DrawText(TextFormat("%s = %d",opinfo[currentOp].label2,value),550,150,20,BLACK);
        
        execute = (Rectangle){20,200,200,40};
        DrawRectangleRec(execute,GRAY);
        DrawText("EXECUTE", execute.x+20, execute.y+15,20,WHITE);

        clear = (Rectangle){230,200,100,40};
        DrawRectangleRec(clear,RED);
        DrawText("CLEAR", clear.x+20, clear.y+15,20,WHITE);

        visualBox = (Rectangle){20,250,760,300};
        DrawRectangleLinesEx(visualBox,2,BLACK);
    }
    else if(currentOp == OP_OG_ORDER || currentOp == OP_REV_ORDER){
        return;
    }

    if(CheckCollisionPointRec(GetMousePosition(),execute) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(opinfo[currentOp].inputs == 1){
            int input = atoi(buffer1);
            switch(currentOp){
                case OP_INSERT_BEGIN:
                case OP_INSERT_LAST:
                case OP_SEARCH:
                case OP_DEL_FIRST_OCC:
                case OP_DEL_ALL_OCC:
                case OP_DEL_DUPES:
                    value = input;
                    break;

                case OP_GET:
                case OP_DEL_RANDOM:
                    position = input;
                    break;

                default:
                    value = input;
            }
        }
        else if(opinfo[currentOp].inputs == 2){
            position = atoi(buffer1);
            value = atoi(buffer2);
        }
        if(currentOp == OP_COUNT || currentOp == OP_SEARCH || currentOp == OP_GET){ 
            runoperation();
            showdll=0;
        }
        else{
            runoperation();
            showdll = 1;
        }
    }

    if(CheckCollisionPointRec(GetMousePosition(),clear) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        clear_list();
        showdll=1;
    }

    if(showdll) drawDLL(visualBox);
}
void drawop_make_menu(int location, int *menu){
    if(menu == NULL) {return;}
     const char* menu_title = opinfo[currentOp].title;
    DrawText(menu_title,20,15,30,BLACK);

    Rectangle back = {300,500,200,50};
    Rectangle inputbox1 = {350,100,200,40};
    Rectangle inputbox2 = {300,150,200,40};
    Rectangle execute= {20,200,150,40};
    Rectangle visualBox = {0};
    Rectangle clear = {200,200,150,40};
    
    DrawRectangleRec(back,RED);
    DrawText("BACK", back.x+60, back.y+15,20,BLACK);

    if(CheckCollisionPointRec(GetMousePosition(),back) &&
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        *menu = location;
        buffer1[0]='\0';
        buffer2[0]='\0';
        outputmsg[0]='\0';
        value=0;
        position=0;
        activebox=0;
        showdll=0;
    }

    if(CheckCollisionPointRec(GetMousePosition(),inputbox1) &&
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        activebox=0;
    }

    if(CheckCollisionPointRec(GetMousePosition(),inputbox2) &&
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        activebox=1;
    }
   
    int key= GetCharPressed();
    
    while(key > 0){
        if(isdigit(key) || key == ' '){
            if(activebox == 0){
                int len = strlen(buffer1);
                if(len < 19){
                    buffer1[len] = key;
                    buffer1[len+1] = '\0';
                }
            }
            else if(opinfo[currentOp].inputs == 2){
                int len = strlen(buffer2);
                if(len < 19){
                    buffer2[len] = key;
                    buffer2[len+1] = '\0';
                }
            }
        }
        key = GetCharPressed();
    }
    if(IsKeyPressed(KEY_BACKSPACE)){
        if(activebox==0){
            int len = strlen(buffer1);
            if(len>0) buffer1[len-1] = '\0';
        }
        else if(opinfo[currentOp].inputs == 2){
            int len = strlen(buffer2);
            if(len>0) buffer2[len-1] = '\0';
        }
    }
    if(activebox==0 && IsKeyPressed(KEY_DOWN)) activebox=1;
    if(IsKeyPressed(KEY_UP)) activebox=0;

    if(IsKeyPressed(KEY_ENTER)){
        if(activebox==0) activebox=1;
        ele = atoi(buffer1);
    }
 
    DrawText("Enter no. of inputs",20,100,30,BLACK);
    DrawText("Enter elements",20,150,30,BLACK);
    DrawRectangleRec(inputbox1, activebox==0? GRAY : LIGHTGRAY);
    DrawText(buffer1,inputbox1.x+10,inputbox1.y+15,20,BLACK);
    DrawRectangleRec(inputbox2, activebox==1? GRAY : LIGHTGRAY);
    DrawText(buffer2,inputbox2.x+10,inputbox2.y+15,20,BLACK);
    DrawRectangleRec(execute,GRAY);
    DrawText("EXECUTE",execute.x+20,execute.y+15,20,WHITE);
    DrawRectangleRec(back,RED);
    DrawText("BACK",back.x+20,back.y+15,20,WHITE);
    DrawRectangleRec(clear,RED);
    DrawText("CLEAR",clear.x+20,clear.y+15,20,WHITE);        
    visualBox = (Rectangle){20,250,760,300};
    DrawRectangleLinesEx(visualBox,2,BLACK);
    DrawText("DEBUG EXEC BTN", execute.x, execute.y - 20, 10, RED);
    if(CheckCollisionPointRec(GetMousePosition(),execute) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        ele = atoi(buffer1);
        printf("EXEC CLICKED\n");
        strcpy(temp_buffer,buffer2);
        char* token = strtok(temp_buffer," ");
        int count=0;
        while(token != NULL){
            elements[count++] = atoi(token);
            token = strtok(NULL, " ");
        }
        for(int i=0;i<ele;i++) printf("%d ",elements[i]);
        for(int i=0;i<ele;i++){
            value = elements[i];
            runoperation();
        }
        showdll=1;
    }
    if(CheckCollisionPointRec(GetMousePosition(),clear) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        clear_list();
        showdll=1;
    }
    if(showdll) drawDLL(visualBox);
}
void runoperation(){
    switch(currentOp){
        case OP_INSERT_BEGIN:
            insert_beginning(value);
            break;

        case OP_INSERT_RANDOM:
            insert_random(position, value);
            break;

        case OP_INSERT_LAST:
            insert_last(value);
            break;

        case OP_INSERT_AFTER:
            insert_after(position, value);
            break;

        case OP_INSERT_BEFORE:
            insert_before(position, value);
            break;

        case OP_DEL_BEGIN:
            delete_beginning();
            break;

        case OP_DEL_RANDOM:
            delete_random(position);
            break;

        case OP_DEL_LAST:
            delete_last();
            break;

        case OP_DEL_FIRST_OCC:
            del_first_occ(value);
            break;

        case OP_DEL_ALL_OCC:
            del_all_occ(value);
            break;

        case OP_DEL_DUPES:
            del_dupes(value);
            break;

        case OP_UPDATE:
            update(position, value);
            break;

        case OP_SORT:
            sort();
            break;

        case OP_REVERSE:
            reverse();
            break;

        case OP_GET:
            ret_value = get_ele(position);
            if(ret_value==-1){
                sprintf(outputmsg,"EMPTY LIST");
            }
            else{
                sprintf(outputmsg,"ELEMENT AT POSITION %d = %d",position,ret_value);
            }
            break;

        case OP_SEARCH:
            ret_value = search(value);
            if(ret_value==-1){
                sprintf(outputmsg,"ELEMENT %d NOT FOUND",value);
            }
            else{
                sprintf(outputmsg,"ELEMENT %d FOUND AT POS %d",value,ret_value);
            }
            break;

        case OP_COUNT:
            ret_value = counter();
            if(ret_value==0){
                sprintf(outputmsg,"EMPTY LIST");
            }
            else{
                sprintf(outputmsg,"TOTAL NO. OF NODES = %d",ret_value);
            }     
            break;

        case OP_OG_ORDER:{
            insert_last(value);
            break;
        }

        case OP_REV_ORDER:{
            insert_beginning(value);
            break;
        }
    }
}
void drawDLL(Rectangle box){
    struct node *temp = head;

    int x = box.x + 20;
    int y = box.y + 40;

    while(temp != NULL){
        Rectangle nodeBox = {x, y, 80, 40};

        DrawRectangleRec(nodeBox, SKYBLUE);
        DrawRectangleLines(nodeBox.x,nodeBox.y,nodeBox.width,nodeBox.height,BLACK);

        DrawText(TextFormat("%d",temp->data),
                 nodeBox.x + 25,
                 nodeBox.y + 10,
                 20,
                 BLACK);

        if(temp->next != NULL){
            DrawLine(x + 80, y + 20, x + 120, y + 20, BLACK);
            DrawLine(x + 120, y + 30, x + 80, y + 30, BLACK);
        }

        x += 120;

        if(x > box.x + box.width - 120){
            x = box.x + 20;
            y += 70;
        }

        temp = temp->next;
    }
}
//gcc main.c dll.c -o DLL_GUI.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
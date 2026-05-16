#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "raylib.h"
#include "dll.h"

typedef enum{
    MENU_MAIN = 0,
    MENU_MAKE,
    MENU_INSERT,
    MENU_DELETE,
    MENU_UPDATE,
    MENU_INFO,
    MENU_EXIT,
    MENU_OPERATIONS,
    MENU_NONE
}MenuState;

typedef enum {
    OP_OG_ORDER = 0,
    OP_REV_ORDER,

    OP_INSERT_BEGIN,
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
    OP_CLEAR,

    OP_SEARCH,
    OP_GET,
    OP_COUNT,
    OP_PRINT,
    
    OP_NONE
} Operation;

typedef struct{
    MenuState PrevMenu;
    MenuState CurrentMenu;
    Operation CurrentOp;

    char buffer1[100];
    char buffer2[100];
    char temp_buffer[100];

    int position;
    int value;
    int ret_value;
    int elements[100];

    int scroll_offset;

    bool show_popup;
    char popup_msg[100];
    char output_msdg[100];

    int activebox;
    bool show_dll;
    bool should_exit;

    DLLStatus LastStatus;

    Rectangle displaybox;
}AppState; 

typedef struct {
    const char *title;
    int inputs;
    const char *label1;
    const char *label2;
    bool display_output;
    bool text_output;
} OperationInfo;

typedef struct{
    const char *label;
    MenuState target_menu;
    Operation op;
}ButtonInfo;

typedef struct{
    const char *title;
    int button_count;
    ButtonInfo buttons[10];
} MenuInfo;

OperationInfo opinfo[] = {
    {"ORDER OF INSERTION", 2, "NUMBER", "VALUES", true, false},
    {"REVERSED ORDER", 2, "NUMBER", "VALUES", true, false},

    {"INSERT AT BEGINNING", 1, "VALUES", NULL, true, false},
    {"INSERT AT POSITION", 2, "Position", "VALUES", true, false},
    {"INSERT AT LAST", 1, "VALUES", NULL, true, false},
    {"INSERT AFTER X", 2, "POSITION", "VALUES", true, false},
    {"INSERT BEFORE X", 2, "POSITION", "VALUES", true, false},

    {"DELETE BEGINNING", 0, NULL, NULL, true, false},
    {"DELETE POSITION", 1, "POSITION", NULL, true, false},
    {"DELETE LAST", 0, NULL, NULL,true, false},
    {"DELETE FIRST OCCURRENCE", 1, "VALUES", NULL, true, false},
    {"DELETE ALL OCCURRENCES", 1, "VALUES", NULL, true, false},
    {"DELETE DUPLICATES", 0, NULL, NULL, true, false},

    {"UPDATE", 2, "POSITION", "VALUES", true, false},
    {"SORT", 0, NULL, NULL, true, false},
    {"CLEAR", 0, NULL, NULL, true, false},

    {"SEARCH ELEMENT", 1, "VALUES", NULL, false, true},
    {"GET ELEMENT", 1, "POSITION", NULL, false, true},
    {"TOTAL NODES", 0, NULL, NULL, false, true},
    {"DISPLAY", 0, NULL, NULL, true, false}
};

MenuInfo menus[] = {
    {"MAIN MENU", 6, {{"MAKE DLL", MENU_MAKE, OP_NONE}, 
                      {"INSERT MENU", MENU_INSERT, OP_NONE}, 
                      {"DELETE MENU", MENU_DELETE, OP_NONE},
                      {"UPDATE MENU", MENU_UPDATE, OP_NONE},
                      {"INFORMATION MENU", MENU_INFO, OP_NONE},
                      {"EXIT PROGRAM", MENU_EXIT, OP_NONE}}},

    {"MAKE DLL MENU", 3, {{"ORIGINAL ORDER", MENU_OPERATIONS, OP_OG_ORDER},
                          {"REVERSED ORDER", MENU_OPERATIONS, OP_REV_ORDER},
                          {"BACK", MENU_MAIN, OP_NONE}}},

    {"INSERT MENU", 6, {{"BEGINNING", MENU_OPERATIONS, OP_INSERT_BEGIN},
                        {"MIDDLE", MENU_OPERATIONS, OP_INSERT_RANDOM},
                        {"LAST", MENU_OPERATIONS, OP_INSERT_LAST},
                        {"INSERT BEFORE X", MENU_OPERATIONS, OP_INSERT_BEFORE},
                        {"INSERT AFTER X", MENU_OPERATIONS, OP_INSERT_AFTER},
                        {"BACK", MENU_MAIN, OP_NONE}}},

    {"DELETE MENU", 7, {{"BEGINNING", MENU_OPERATIONS, OP_DEL_BEGIN},
                        {"MIDDLE", MENU_OPERATIONS, OP_DEL_RANDOM},
                        {"LAST", MENU_OPERATIONS, OP_DEL_LAST},
                        {"FIRST OCCURRENCE", MENU_OPERATIONS, OP_DEL_FIRST_OCC},
                        {"ALL OCCURRENCES", MENU_OPERATIONS, OP_DEL_ALL_OCC},
                        {"DUPLICATES", MENU_OPERATIONS, OP_DEL_DUPES},
                        {"BACK", MENU_MAIN, OP_NONE}}},

    {"UPDATE MENU", 4, {{"UPDATE BY POSITION", MENU_OPERATIONS, OP_UPDATE},
                        {"SORT", MENU_OPERATIONS, OP_SORT},
                        {"CLEAR", MENU_OPERATIONS, OP_CLEAR},
                        {"BACK", MENU_MAIN, OP_NONE}}},

    {"INFORMATION MENU", 5, {{"SEARCH VALUE", MENU_OPERATIONS, OP_SEARCH},
                             {"GET POSITION", MENU_OPERATIONS, OP_GET},
                             {"COUNT NODES", MENU_OPERATIONS, OP_COUNT},
                             {"DISPLAY LIST", MENU_OPERATIONS, OP_PRINT},
                             {"BACK", MENU_MAIN, OP_NONE}}},

    {"EXIT PROGRAM", 2, {{"YES", MENU_NONE, OP_NONE},
                         {"NO", MENU_MAIN, OP_NONE}}},

    {"OPERATIONS MENU", 3, {{"EXECUTE", MENU_NONE, OP_NONE},
                            {"CLEAR", MENU_NONE, OP_CLEAR},
                            {"BACK", MENU_NONE, OP_NONE}}},

    {"ERROR", 1, {{"OK", MENU_NONE, OP_NONE}}}
};

Rectangle GetMenuButton(int index);
Rectangle GetExecute(int inputs);
Rectangle GetClear(int inputs);
Rectangle GetBack(int inputs);
Rectangle GetInputBoxes(int index);

void UpdateApp(AppState *app);
void DrawApp(AppState *app);
void DrawMenu(MenuState *curr_menu);
void DrawOperationsMenu(AppState *app);
void DrawList(Rectangle displaybox, int scroll_offset);
void DrawPopup(AppState *app);
void DisplayOutput(AppState *app);
void ResetInputState(AppState *app);
void RunOperation(AppState *app);

const char* PopupMsg(AppState *app);
const char* OutputMsg(Operation Op);

bool AllowSpaces(Operation Op);

int main(){
    InitWindow(800, 600, "DLL PROGRAM");
    SetTargetFPS(60);
    AppState app = {0};

    app.PrevMenu = MENU_NONE;
    app.CurrentMenu = MENU_MAIN;
    app.CurrentOp = OP_NONE;
    app.show_popup = false;
    app.show_dll = false;
    app.activebox = -1;
    app.scroll_offset = 0;
    app.should_exit = false;

    while(!WindowShouldClose() && !app.should_exit){

        UpdateApp(&app);
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawApp(&app);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
Rectangle GetMenuButton(int index){

    int btn_w = 250, btn_h = 50, start_y = 100, spacing = 20;
    int x = (800 - btn_w) / 2;
    int y = start_y + index * (btn_h + spacing);

    Rectangle rect = {x, y, btn_w, btn_h};

    return rect;
}

Rectangle GetExecute(int inputs){
    Rectangle execute = {0};
    switch(inputs){
        case 0:{
            execute = (Rectangle){20, 50, 200, 40};
            return execute;
        }
        case 1:{
            execute = (Rectangle){20, 150, 200, 40};
            return execute;
        }
        case 2:{
            execute = (Rectangle){20, 200, 200, 40};
            return execute;
        }
    }
}

Rectangle GetClear(int inputs){
    Rectangle clear = {0};
    switch(inputs){
        case 0:{
            clear = (Rectangle){230, 50, 100, 40};
            return clear;
        }
        case 1:{
            clear = (Rectangle){230, 150, 100, 40};
            return clear;
        }
        case 2:{
            clear = (Rectangle){230, 200, 100, 40};
            return clear;
        }
    }
}

Rectangle GetBack(int inputs){
    Rectangle back = {0};
    switch(inputs){
        case 0:{
            back = (Rectangle){340, 50, 100, 40};
            return back;
        }
        case 1:{
            back = (Rectangle){340, 150, 100, 40};
            return back;
        }
        case 2:{
            back = (Rectangle){340, 200, 100, 40};
            return back;
        }
    }
}

Rectangle GetInputBoxes(int index){

    int btn_w = 300, btn_h = 40, start_y = 90, spacing = 20;
    int x = 250;
    int y = start_y + index * (btn_h + spacing);

    Rectangle rect = {x, y, btn_w, btn_h};

    return rect;
}

void ResetInputState(AppState *app){
    app->activebox = -1;
    app->buffer1[0] = '\0';
    app->buffer2[0] = '\0';
    app->CurrentMenu = app->PrevMenu;
    app->CurrentOp = OP_NONE;
    memset(app->elements, 0, sizeof(app->elements));
    app->output_msdg[0] = '\0';
    app->popup_msg[0] = '\0';
    app->position = 0;
    app->ret_value = 0;
    app->show_dll = false;
    app->show_popup = false;
    app->temp_buffer[0] = '\0';
    app->value = 0;
    app->scroll_offset = 0;
}

bool AllowSpaces(Operation Op){
    if(Op == OP_OG_ORDER || Op == OP_REV_ORDER){
        return true;
    } 
    else{
        return false;
    }
}

void UpdateApp(AppState *app){
    MenuState curr_menu = app->CurrentMenu;
    Operation curr_op = app->CurrentOp;

    if(curr_menu != MENU_OPERATIONS){
        for(int i = 0; i < menus[curr_menu].button_count; i++){
            Rectangle btn = GetMenuButton(i);
            if(CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(curr_menu == MENU_EXIT && i == 0){
                    app->should_exit = true;
                    return;
                }
                if(menus[curr_menu].buttons[i].target_menu == MENU_OPERATIONS){
                    app->PrevMenu = curr_menu;
                }
                app->CurrentMenu = menus[curr_menu].buttons[i].target_menu;
                app->CurrentOp = menus[curr_menu].buttons[i].op;
            } 
        }
    }
    else{
        if(app->show_popup){
            Rectangle ok_btn = {320, 320, 160, 40};
            if(CheckCollisionPointRec(GetMousePosition(), ok_btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                app->show_popup = false;
            }
        }
        else{
            Rectangle execute = GetExecute(opinfo[curr_op].inputs);
            Rectangle clear = GetClear(opinfo[curr_op].inputs);
            Rectangle back = GetBack(opinfo[curr_op].inputs);

            if(CheckCollisionPointRec(GetMousePosition(), execute) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){

                RunOperation(app);
                
                app->show_popup = true;

                if(app->CurrentMenu == MENU_INFO && app->CurrentOp != OP_PRINT){
                    app->show_dll = false;
                }
                else{
                    app->show_dll = true;
                }
                app->buffer1[0] = '\0';
                app->buffer2[0] = '\0';
                app->temp_buffer[0] = '\0';
                memset(app->elements, 0, sizeof(app->elements));
                app->value = 0;
                app->position = 0;
            }
            if(CheckCollisionPointRec(GetMousePosition(), clear) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                app->buffer1[0] = '\0';
                app->buffer2[0] = '\0';
                app->temp_buffer[0] = '\0';
                memset(app->elements, 0, sizeof(app->elements));
                app->value = 0;
                app->position = 0;
            }
            if(CheckCollisionPointRec(GetMousePosition(), back) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                ResetInputState(app);
            }
            if(opinfo[app->CurrentOp].inputs == 1){
                Rectangle inputbox = GetInputBoxes(0);
                if(CheckCollisionPointRec(GetMousePosition(), inputbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    app->activebox = 0;
                }
            }
            if(opinfo[app->CurrentOp].inputs == 2){
                for(int i = 0; i < 2; i++){
                    Rectangle inputbox = GetInputBoxes(i);
                    if(CheckCollisionPointRec(GetMousePosition(), inputbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        if(i == 0){
                            app->activebox = 0;
                        }
                        else{
                            app->activebox = 1;
                        }
                    }
                }
            }
            if(CheckCollisionPointRec(GetMousePosition(), app->displaybox)){
                app->scroll_offset -= GetMouseWheelMove() * 20;
                if(app->scroll_offset < 0){
                    app->scroll_offset = 0;
                }
            }
            int key = GetCharPressed();
            while(key > 0){
                char *buffer = NULL;
                if(app->activebox == 0){
                    buffer = app->buffer1;
                }
                else if(app->activebox == 1){
                    buffer = app->buffer2;
                }
                else{
                    return;
                }
                if(isdigit(key)){
                    int len = strlen(buffer); 
                    if(len < 99){
                        buffer[len] = key;
                        buffer[len + 1] = '\0';
                    }
                }
                else if(key == '-'){
                    int len = strlen(buffer);
                    if(len < 99){
                        if(len == 0 || buffer[len - 1] == ' '){
                            buffer[len] = '-';
                            buffer[len + 1] = '\0';
                        }
                    }
                }
                else if(key == ' ' && AllowSpaces(app->CurrentOp) && app->activebox == 1){
                    int len = strlen(buffer);

                    if(len > 0 && buffer[len - 1] != ' ' && buffer[len - 1] != '-'){
                        buffer[len] = ' ';
                        buffer[len + 1] = '\0';
                    }
                }
                key = GetCharPressed();
            }
            if(IsKeyPressed(KEY_BACKSPACE)){
                if(app->activebox == 0){
                    int len = strlen(app->buffer1);
                    if(len > 0) {app->buffer1[len - 1] = '\0';}
                }
                else if(app->activebox == 1){
                    int len = strlen(app->buffer2);
                    if(len > 0) {app->buffer2[len - 1] = '\0';}
                }
            }
        } 
    } 
}

void DrawApp(AppState *app){
    if(app->CurrentMenu != MENU_OPERATIONS){
        DrawMenu(&app->CurrentMenu);
    }
    else{
        DrawOperationsMenu(app);
        if(opinfo[app->CurrentOp].display_output){
            BeginScissorMode(app->displaybox.x, app->displaybox.y, app->displaybox.width, app->displaybox.height);
            DrawList(app->displaybox, app->scroll_offset);
            EndScissorMode();
        }
        if(opinfo[app->CurrentOp].text_output){
            DisplayOutput(app);
        }
        if(app->show_popup){
            DrawPopup(app);
        }
    }
}

void DrawMenu(MenuState *curr_menu){
    DrawText(menus[*curr_menu].title, 300, 20, 30, BLACK);
    
    for(int i = 0; i < menus[*curr_menu].button_count; i++){
        Rectangle btn = GetMenuButton(i);
        DrawRectangleRec(btn, GRAY);
        DrawText(menus[*curr_menu].buttons[i].label, btn.x + 20, btn.y + 15, 20, BLACK);
    }
}

void DrawOperationsMenu(AppState *app){
    MenuState curr_menu = app->CurrentMenu; 
    Operation curr_op = app->CurrentOp;

    DrawText(opinfo[curr_op].title, 10, 10, 30, BLACK);

    int inputs = opinfo[curr_op].inputs;
    Rectangle execute = GetExecute(inputs);
    Rectangle clear = GetClear(inputs);
    Rectangle back = GetBack(inputs);
    Rectangle displaybox = {0};
    
    switch(inputs){
        case 0:{
            DrawRectangleRec(execute, GRAY);
            DrawText("EXECUTE", execute.x + 20, execute.y + 15, 20, BLACK);

            DrawRectangleRec(clear, GRAY);
            DrawText("CLEAR", clear.x + 20, clear.y + 15, 20, BLACK);

            DrawRectangleRec(back, GRAY);
            DrawText("BACK", back.x + 20, back.y + 15, 20, BLACK);

            displaybox = (Rectangle){20, 100, 760, 450};
            app->displaybox = displaybox;
            DrawRectangleLinesEx(displaybox, 2, BLACK);
            
            break;
        }
        case 1:{
            DrawText(TextFormat("ENTER %s", opinfo[curr_op].label1), 10, 100, 25, BLACK);
            
            DrawRectangleRec(execute, GRAY);
            DrawText("EXECUTE", execute.x + 20, execute.y + 15, 20, BLACK);
            
            DrawRectangleRec(clear, GRAY);
            DrawText("CLEAR", clear.x + 20, clear.y + 15, 20, BLACK);
            
            DrawRectangleRec(back, GRAY);
            DrawText("BACK", back.x + 20, back.y + 15, 20, BLACK);

            Rectangle inputbox = GetInputBoxes(0);
            DrawRectangleRec(inputbox, app->activebox == 0 ? GRAY : LIGHTGRAY);
            DrawText(TextFormat("%s",app->buffer1), inputbox.x + 10, inputbox.y + 10, 20, BLACK);

            displaybox = (Rectangle){20, 200, 750, 350};
            app->displaybox = displaybox;
            DrawRectangleLinesEx(displaybox, 2, BLACK);

            break;
        }
        case 2:{
            DrawText(TextFormat("ENTER %s", opinfo[curr_op].label1), 10, 100, 25, BLACK);
            DrawText(TextFormat("ENTER %s", opinfo[curr_op].label2), 10, 160, 25, BLACK);
            
            DrawRectangleRec(execute, GRAY);
            DrawText("EXECUTE", execute.x + 20, execute.y + 15, 20, BLACK);
            
            DrawRectangleRec(clear, GRAY);
            DrawText("CLEAR", clear.x + 20, clear.y + 15, 20, BLACK);
            
            DrawRectangleRec(back, GRAY);
            DrawText("BACK", back.x + 20, back.y + 15, 20, BLACK);

            for(int i = 0; i < 2; i++){
                Rectangle inputbox = GetInputBoxes(i);
                DrawRectangleRec(inputbox, app->activebox == i ? GRAY : LIGHTGRAY);
                if(i == 0) {DrawText(TextFormat("%s",app->buffer1), inputbox.x + 10, inputbox.y + 10, 20, BLACK);}
                else {DrawText(TextFormat("%s",app->buffer2), inputbox.x + 10, inputbox.y + 10, 20, BLACK);}
            }

            displaybox = (Rectangle){20, 250, 760, 300};
            app->displaybox = displaybox;
            DrawRectangleLinesEx(displaybox, 2, BLACK);

            break;
        }
    }
}

void RunOperation(AppState *app){
    switch(app->CurrentOp){
        case OP_OG_ORDER:{
            int n = atoi(app->buffer1);

            strcpy(app->temp_buffer, app->buffer2);
            
            char *token = strtok(app->temp_buffer, " ");
            int count = 0;
            
            while(token != NULL){
                app->elements[count++] = atoi(token);
                token = strtok(NULL, " ");
            }
            if(count != n){
                app->LastStatus = DLL_INVALID_INPUT;
                return;
            }
            app->LastStatus = DLL_OK;               
            for(int i = 0; i < n; i++){
                app->value = app->elements[i];
                DLLStatus status = insert_end(app->value);
                if(status != DLL_OK){
                    app->LastStatus = status;
                    break;
                }
            }
            break;
        }
        case OP_REV_ORDER:{
            int n = atoi(app->buffer1);

            strcpy(app->temp_buffer, app->buffer2);
            
            char *token = strtok(app->temp_buffer, " ");
            int count = 0;
            
            while(token != NULL){
                app->elements[count++] = atoi(token);
                token = strtok(NULL," ");
            }
            if(count != n){
                app->LastStatus = DLL_INVALID_INPUT;
                return;
            }
            app->LastStatus = DLL_OK;
            for(int i = 0; i < n; i++){
                app->value = app->elements[i];
                DLLStatus status = insert_beg(app->value);
                if(status != DLL_OK){
                    app->LastStatus = status;
                    break;
                }
            }
            break;
        }
        case OP_INSERT_BEGIN:{
            app->value = atoi(app->buffer1);
            app->LastStatus = insert_beg(app->value);
            break;
        }
        case OP_INSERT_RANDOM:{
            app->position = atoi(app->buffer1);
            app->value = atoi(app->buffer2);
            app->LastStatus = insert_mid(app->position, app->value);
            break;
        }
        case OP_INSERT_LAST:{
            app->value = atoi(app->buffer1);
            app->LastStatus = insert_end(app->value);
            break;
        }
        case OP_INSERT_AFTER:{
            app->position = atoi(app->buffer1);
            app->value = atoi(app->buffer2);
            app->LastStatus = insert_after(app->value, app->position);
            break;
        }
        case OP_INSERT_BEFORE:{
            app->position = atoi(app->buffer1);
            app->value = atoi(app->buffer2);
            app->LastStatus = insert_before(app->value, app->position);
            break;
        }
        case OP_DEL_BEGIN:{
            app->LastStatus = del_beg();
            break;
        }
        case OP_DEL_RANDOM:{
            app->position = atoi(app->buffer1);
            app->LastStatus = del_mid(app->position);
            break;
        }
        case OP_DEL_LAST:{
            app->LastStatus = del_end();
            break;
        }
        case OP_DEL_FIRST_OCC:{
            app->value = atoi(app->buffer1);
            app->LastStatus = del_element(app->value);
            break;
        }
        case OP_DEL_ALL_OCC:{
            app->value = atoi(app->buffer1);
            app->LastStatus = del_all_occurances(app->value);
            break;
        }
        case OP_DEL_DUPES:{
            app->LastStatus = del_duplicates();
            break;
        }
        case OP_UPDATE:{
            app->position = atoi(app->buffer1);
            app->value = atoi(app->buffer2);
            app->LastStatus = update(app->position, app->value);
            break;
        }
        case OP_SORT:{
            app->LastStatus = sort();
            break;
        }
        case OP_CLEAR:{
            app->LastStatus = clear_list();
            break;
        }
        case OP_SEARCH:{
            app->value = atoi(app->buffer1);
            app->LastStatus = search(app->value, &app->ret_value);
            break;
        }
        case OP_GET:{
            app->position = atoi(app->buffer1);
            app->LastStatus = get_element(app->position, &app->ret_value);
            break;
        }
        case OP_COUNT:{
            app->LastStatus = node_counter(&app->ret_value);
            break;
        }
        case OP_PRINT:{
            
            break;
        }
        case OP_NONE:{
            break;
        }
    }
}

const char* PopupMsg(AppState *app){
    switch(app->LastStatus){
        case DLL_OK:{return "OPERATION SUCCESSFUL!";}
        case DLL_EMPTY:{return "ERROR: EMPTY LIST";}
        case DLL_INVALID_POS:{return "ERROR: INVALID POSITION";}
        case DLL_NOT_FOUND:{return "ERROR: LIST NOT FOUND";}
        case DLL_ELEMENT_NOT_FOUND:{return "ERROR: ELEMENT NOT FOUND";}
        case DLL_ALLOC_FAILED:{return "ERROR: NODE ALLOCATION FAILED";}
        case DLL_INVALID_INPUT:{return "ERROR: INVALID INPUT";}
        default:{return "ERROR NOT FOUND";}
    }
}

const char* OutputMsg(Operation Op){
    switch(Op){
        case OP_SEARCH:{return "Positon of Element is: ";}
        case OP_GET:{return "Element Found at position: ";}
        case OP_COUNT:{return "Total No. of Nodes are: ";}
        default:{return " ";}
    }
}

void DrawList(Rectangle displaybox, int scroll_offset){
    struct node *temp = head;

    int x = displaybox.x + 20, y = displaybox.y + 40 - scroll_offset;

    while(temp != NULL){
        Rectangle nodebox = {x, y, 80, 40};

        DrawRectangleRec(nodebox, SKYBLUE);
        DrawRectangleLines(nodebox.x, nodebox.y, nodebox.width, nodebox.height, BLACK);

        DrawText(TextFormat("%d", temp->data), nodebox.x + 25, nodebox.y + 10, 20, BLACK);

        if(temp->next != NULL){
            DrawLine(x + 80, y+ 20, x + 120, y + 20, BLACK);
            DrawLine(x + 120, y + 30, x + 80, y + 30, BLACK);
        }
        x += 120;

        if(x > displaybox.x + displaybox.width - 120){
            x = displaybox.x + 20;
            y += 70;
        }

        temp = temp->next;
    }
}

void DrawPopup(AppState *app){
    const char *msg = PopupMsg(app);
    
    DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.5f));
    
    int box_w = 500, box_h = 200;
    int x = (800 - box_w) / 2, y = (600 - box_h) / 2; // x = 200, y = 200 
    
    DrawRectangle(x, y, box_w, box_h, RAYWHITE);
    DrawRectangleLines(x, y, box_w, box_h, BLACK);
    DrawText(msg, x + 80, y + 40, 30, BLACK);
    
    Rectangle ok_btn = {x + 120, y + 120, 160, 40};
    DrawRectangleRec(ok_btn, GRAY);
    DrawText("OK", ok_btn.x + 10, ok_btn.y + 10, 20, BLACK); 

}

void DisplayOutput(AppState *app){
    const char* msg = OutputMsg(app->CurrentOp);
    DrawText(TextFormat("%s %d",msg, app->ret_value), app->displaybox.x + 20, app->displaybox.y + 20, 30, BLACK);
}
//gcc frontend_update.c main.c -o DLL_GUI.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
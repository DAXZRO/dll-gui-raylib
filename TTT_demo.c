#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


void draw_board(void);

int main(){
    InitWindow(1280,720,"TIC TAC TOE");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board();
        EndDrawing();
    }
    CloseWindow();
}

void draw_board(void){
    int screenW = 1280, screenH = 720;
    int boardSize = 600;

    int startX = (screenW - boardSize) / 2;
    int startY = (screenH - boardSize) / 2;

    int cell = boardSize / 3;

    for (int i = 1; i < 3; i++) {
        int x = startX + i * cell;
        DrawLine(x, startY, x, startY + boardSize, BLACK);
    }

    for (int i = 1; i < 3; i++) {
        int y = startY + i * cell;
        DrawLine(startX, y, startX + boardSize, y, BLACK);
    }
}
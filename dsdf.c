#include <raylib.h>
#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const Color BACKGROUND  = {031, 031, 031, 255};
int size = 20;

Rectangle slider = { 50, 50, 200, 20 };
unsigned int amount = 1;

typedef struct {
    Vector2 pos;
    bool type;
    int size;
} Shape;

unsigned int used = 0;
unsigned int max = 200;
Shape *drawing = NULL;

void add_shape(bool type) {
    if(++used >= max) {
        max*=2;
        drawing = (Shape *)realloc(drawing, sizeof(Shape)*max);
    }
    drawing[used] = (Shape){
        GetMousePosition(),
        type,
        size
    };

    if(drawing[used-1].pos.x == drawing[used].pos.x &&
        drawing[used-1].pos.y == drawing[used].pos.y &&
        drawing[used-1].type == drawing[used].type &&
        drawing[used-1].size == drawing[used].size) {
        used--;
        drawing[used] = (Shape){
            (Vector2){0,0},
            0,
            0
        };
    }
}

int point_dist(unsigned int index) {
    return sqrt((GetMouseX()-drawing[index].pos.x)*(GetMouseX()-drawing[index].pos.x)+(GetMouseY()-drawing[index].pos.y)*(GetMouseY()-drawing[index].pos.y));
}

void draw_distance(void) {
    if(amount == 100)
        amount = used;

    unsigned int closest = used;
    for(int i = 1; i < used; i++) { // WHY ONE????
        if(point_dist(i) < point_dist(closest))
           closest = i;
    }

    for(int i = 1; i < amount; i++) {
        closest = i;
        DrawLineV(GetMousePosition(), drawing[closest].pos, WHITE);
        unsigned int distance = point_dist(closest);
        Vector2 midpoint = {GetMouseX()-(GetMouseX()-drawing[closest].pos.x)/2, GetMouseY()-(GetMouseY()-drawing[closest].pos.y)/2};
        DrawText(TextFormat("%d", distance), midpoint.x, midpoint.y, size, WHITE);
    }

    if(amount == used)
        amount = 100;
}

void draw_slider(bool colliding) {
    if(colliding) {
        float sliderValue = (float)(GetMouseX() - slider.x) / slider.width;
        amount = (int)(sliderValue * 100);
    }
    DrawRectangleRec(slider, LIGHTGRAY);
    DrawRectangle(slider.x + slider.width * (amount / 100.0f) - 5, slider.y - 5, 10, slider.height + 10, MAROON);
    DrawText(TextFormat("amount: %s", amount == 100 ? "All" : TextFormat("%d", amount)), 10, 10, 20, WHITE);
}

void draw(bool type) {
    if(type)
        DrawCircleV(GetMousePosition(), size, GRAY);
    else
        DrawRectangleV(GetMousePosition(), (Vector2){size, size}, GRAY);

    for(int i = 0; i < used; i++) {
        if(drawing[i].type)
            DrawCircleV(drawing[i].pos, drawing[i].size, GRAY);
        else
            DrawRectangleV(drawing[i].pos, (Vector2){drawing[i].size, drawing[i].size}, GRAY);
    }
    if(used)
        draw_distance();

}

int main(void) {
    drawing = (Shape *)malloc(sizeof(Shape)*max);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "dsdf");
    bool type = true;
    bool colliding = false;
    while (!WindowShouldClose()) {
        size += GetMouseWheelMove()*2;
        if(size <= 0)
            size = 0;
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            type = !type;
        if(CheckCollisionPointRec(GetMousePosition(), slider) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            colliding = true;
        else
            colliding = false;
        if(!colliding && size && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            add_shape(type);

        BeginDrawing();
            ClearBackground(BACKGROUND);
            draw(type);
            draw_slider(colliding);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

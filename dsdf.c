#include <raylib.h>
#include "raygui.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SHAPE_MAX 200

const Color BACKGROUND  = {031, 031, 031, 255};
int size = 20;

typedef struct {
    Vector2 pos;
    bool type;
    int size;
} Shape;

unsigned int used = 0;
unsigned int max = SHAPE_MAX;
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
}

void clear(void) {
    used = 0;
    max = SHAPE_MAX;
    drawing = (Shape *)realloc(drawing, sizeof(Shape)*max);
}

int point_dist(unsigned int index) {
    return sqrt((GetMouseX()-drawing[index].pos.x)*(GetMouseX()-drawing[index].pos.x)+(GetMouseY()-drawing[index].pos.y)*(GetMouseY()-drawing[index].pos.y));
}

void draw_distance(void) {
    unsigned int closest = used;
    for(int i = 1; i < used; i++) { // WHY ONE????
        if(point_dist(i) < point_dist(closest))
            closest = i;
    }

    DrawLineV(GetMousePosition(), drawing[closest].pos, WHITE);
    unsigned int distance = point_dist(closest);
    Vector2 midpoint = {GetMouseX()-(GetMouseX()-drawing[closest].pos.x)/2, GetMouseY()-(GetMouseY()-drawing[closest].pos.y)/2};
    DrawText(TextFormat("%d", distance), midpoint.x, midpoint.y, size, WHITE);
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
    while (!WindowShouldClose()) {
        size += GetMouseWheelMove()*2;
        if(size <= 0)
            size = 0;
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            type = !type;
        if(size && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            add_shape(type);
        if(IsKeyPressed(KEY_SPACE))
            clear();

        BeginDrawing();
            ClearBackground(BACKGROUND);
            draw(type);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

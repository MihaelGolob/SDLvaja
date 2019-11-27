#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <math.h>

#include <SDL2/SDL.h>
#include "Window.h"

using namespace std;
// PRVA NALOGA: ______________________________________________________________________________________
/*
struct El{
    int x,y;
    struct El *next;
}*start = nullptr;

void napolniSeznam();
void narisiKvadrate(Window window, short stKvadratkov, short zadnjiKvadratek);
void izpisSeznama();

void drawGrid(Window window, int rows, int columns);

int main(int argc, char *argv[]) {
    srand(time(0));
    int st = rand()%91 + 10;
    for(int i = 0; i < st; i++)
        napolniSeznam();
    izpisSeznama();

    unsigned int time, lastTime = 0;
    unsigned int cas, zadnjiCas = 0;
    short stKvadratkov = 0, zadnjiKvadratek = 0, casNastajanja = 200;

    Window window;
    window.InitWindow("SDL window!", 800, 800);
    window.InitRenderer();
    SDL_Event event;
    bool quit = false;

    while(!quit){
        // poll events:
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                quit = true;
        }

        // loop functions:
        //clear surface
        SDL_RenderClear(window.Renderer);

        //time functions
        time = SDL_GetTicks();
        if(time > lastTime + casNastajanja){
            lastTime = time;
            stKvadratkov++;
        }
        cas = SDL_GetTicks();
        if(cas > zadnjiCas + 1000){
            zadnjiCas = cas;
            zadnjiKvadratek++;
        }

        //draw

        SDL_SetRenderDrawColor(window.Renderer, 255, 255, 255, 255);
        drawGrid(window, 20,20);
        narisiKvadrate(window, stKvadratkov, zadnjiKvadratek);


        //update frame
        SDL_SetRenderDrawColor(window.Renderer, 13, 95, 110, 255);
        SDL_RenderPresent(window.Renderer);
    }

    window.Close();

    return 0;
}

void napolniSeznam(){
    struct El *tmp = new struct El;
    tmp -> x = rand()%20 + 1;
    tmp -> y = rand()%20 + 1;

    if(start == nullptr){
        start = tmp;
        tmp -> next = nullptr;
    }
    else{
        tmp -> next = start;
        start = tmp;
    }
}

void narisiKvadrate(Window window, short stKvadratkov, short zadnjiKvadratek){
    struct El *tmp = start;
    int i = 0;
    while(tmp != nullptr){
        if(i < stKvadratkov && i > zadnjiKvadratek){
            SDL_Rect rect;
            rect.x = tmp->y * (window.width/20);
            rect.y = tmp->x * (window.height/20);
            rect.w = window.width/20;
            rect.h = window.height/20;

            SDL_RenderFillRect(window.Renderer, &rect);
        }

        tmp = tmp->next;
        i++;
    }
}

void izpisSeznama(){
    struct El *tmp = start;

    while(tmp != nullptr){
        cout << "(" << tmp->x << "," << tmp->y << ")   ";

        tmp = tmp->next;
    }
}

void drawGrid(Window window, int rows, int columns){
    int x1 = 0, y1 = 0;
    int x2 = window.width, y2 = 0;
    for(int i = 0; i < rows; i++){
        y1 += window.height/rows;
        y2 = y1;
        SDL_RenderDrawLine(window.Renderer, x1, y1, x2, y2);
    }
    y1 = 0;
    y2 = window.height;
    x1 = 0;
    for(int i = 0; i < columns; i++){
        x1 += window.width/columns;
        x2 = x1;
        SDL_RenderDrawLine(window.Renderer, x1, y1, x2, y2);
    }
}
*/

 // DRUGA NALOGA: ____________________________________________________________________________________________

 struct El{
    int x,y;
    struct El *next, *prev;
}*start = nullptr, *endS = nullptr;

void napolniSeznam();
void izpisSeznama();
void move(SDL_Rect &,Window);
void checkPosition(SDL_Rect &, Window);
bool najdi(int n, int m);

void drawGrid(Window window, int rows, int columns);

const int SPEED = 10;
int DELAY = 0;
int oldX = 1000;

int main(int argc, char *argv[]) {
    srand(time(0));
    int st = rand()%91 + 10;
    for(int i = 0; i < st; i++)
        napolniSeznam();
    izpisSeznama();

    struct El *tmp = start;
    unsigned int time, oldTime=0;
    int m = 0, n = 0;
    short delay = 50;

    Window window;
    window.InitWindow("SDL window!", 800, 800);
    window.InitRenderer();
    SDL_Event event;
    bool quit = false, reset = false;
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = window.width/20;
    rect.h = window.height/20;

    while(!quit){
        // poll events:
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // loop functions:
        //clear surface
        SDL_RenderClear(window.Renderer);

        //other stuff

        //draw
        time = SDL_GetTicks();
        if(time > oldTime + DELAY){
            if(DELAY == 1000){
                DELAY = 0;
            }

            move(rect, window);
            checkPosition(rect,window);

            oldTime = time;
        }

        SDL_SetRenderDrawColor(window.Renderer, 255, 255, 255, 255);
        drawGrid(window, 20,20);
        SDL_RenderFillRect(window.Renderer, &rect);

        //update frame
        SDL_SetRenderDrawColor(window.Renderer, 13, 95, 110, 255);
        SDL_RenderPresent(window.Renderer);
    }

    window.Close();

    return 0;
}

void move(SDL_Rect &rect, Window window){
    rect.y += SPEED;
    if(rect.y >= window.width){
        rect.y = 0;
        rect.x += window.height/20;
    }
}

void checkPosition(SDL_Rect &rect, Window window){
    int x = floor(rect.y / (window.width / 20));
    int y = floor(rect.x / (window.height / 20));
    if(x != oldX && najdi(x,y)){
        rect.y = x*(window.width / 20);
        rect.x = y*(window.width / 20);
        DELAY = 1000;
        oldX = x;
    }
}

bool najdi(int n, int m){
    struct El *tmp = start;
    while(tmp != nullptr){
        if(tmp->y-1 == m && tmp->x-1 == n){
            cout << "found it! " << m << "," << n << "   " << tmp->y-1 << "," << tmp->x-1 << endl;
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

void napolniSeznam(){
    struct El *tmp = new struct El;
    tmp -> x = rand()%20 + 1;
    tmp -> y = rand()%20 + 1;

    struct El *p = start;
    while(p != nullptr && p->y < tmp->y)
        p = p -> next;

    if(start == nullptr){
        start = tmp;
        endS = tmp;
        tmp -> next = nullptr;
        tmp -> prev = nullptr;
    }
    else if(p == nullptr){
        endS -> next = tmp;
        tmp -> prev = endS;
        tmp -> next = nullptr;
        endS = tmp;
    }
    else if(p == start){
        start -> prev = tmp;
        tmp -> next = start;
        tmp -> prev = nullptr;
        start = tmp;
    }
    else{
        tmp -> next = p;
        tmp -> prev = p -> prev;
        p -> prev -> next = tmp;
        p -> prev = tmp;
    }
}

void izpisSeznama(){
    struct El *tmp = start;

    while(tmp != nullptr){
        cout << "(" << tmp->x << "," << tmp->y << ")   ";

        tmp = tmp->next;
    }
}

void drawGrid(Window window, int rows, int columns) {
    int x1 = 0, y1 = 0;
    int x2 = window.width, y2 = 0;
    for (int i = 0; i < rows; i++) {
        y1 += window.height / rows;
        y2 = y1;
        SDL_RenderDrawLine(window.Renderer, x1, y1, x2, y2);
    }
    y1 = 0;
    y2 = window.height;
    x1 = 0;
    for (int i = 0; i < columns; i++) {
        x1 += window.width / columns;
        x2 = x1;
        SDL_RenderDrawLine(window.Renderer, x1, y1, x2, y2);
    }
}
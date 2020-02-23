#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.h"
#include "Player/Player.h"
#include "Text/Text.h"
#include "Ally/Ally.h"
#include "LevelManager.h"
#include "Texture/Texture.h"

using namespace std;

const int HEIGHT = 600;
const int WIDTH = 1000;

// global variables
vector<Tree> allTrees = {};
vector<Enemy> enemies = {};
vector<Ally> allies = {};

int level = 1;
int numDeadTrees = 0;
int oldNum = 0;

double deltaTime;

Text levelText;
Text clearanceText;

// function prototypes
void drawBackground(Window, SDL_Texture *);
void drawObjects();
void createTrees(Window &, Texture *);
void printText(Text &);

int main(int argc, char *argv[]) {
    srand(time(0)); // set seed for random

    Window window; // initialize new Window class
    window.InitWindow("SDL", WIDTH, HEIGHT); // create new Window (more details in Window.cpp)
    window.InitRenderer(); // initialize renderer (more details in Window.cpp)
    window.InitImage();
    window.InitText();
    SDL_Event event; // new event
    bool quit = false;

    // open background texture:
    SDL_Texture *background = IMG_LoadTexture(window.Renderer, "../Assets/environment/grass1.png");
    if(background == nullptr)
        window.logError("TEXTURE");

    // create textures for sprites
    auto *allyTexture = new Texture("../Assets/enemy/",4,window);
    auto *enemyTexture = new Texture("../Assets/ally/", 4, window);

    // create Level Manager
    LevelManager levelManager(allyTexture, enemyTexture, &window);
    levelManager.startLevel();

    // create all trees:
    auto *treeTexture = new Texture("../Assets/environment/", 4, true, window);
    createTrees(window, treeTexture);

    // create player
    auto *playerTexture = new Texture("../Assets/hero/", 6, window);
    Player player(WIDTH/2,HEIGHT/2,2,3, playerTexture,window); // create player

    // make a color for text
    SDL_Color color = {255, 255, 255,255};
    Text treePercentage("100%", "../Assets/fonts/raleway/Raleway-Light.ttf", WIDTH-120,HEIGHT-60,50,color,window);

    // initialize text class for printing out level
    levelText.init("level " + to_string(level), "../Assets/fonts/raleway/Raleway-Light.ttf", 10, 10, 40, color, window);
    // text for printing if level is cleared or failed, leave it empty for now
    color = {179, 24, 16, 255};
    clearanceText.init(" ", "../Assets/fonts/raleway/Raleway-Light.ttf", WIDTH / 2 - 300, HEIGHT / 2 - 50, 70, color, window);

    unsigned long oldTime = SDL_GetTicks();

    while(!quit){
        //calculate deltaTime:
        unsigned long delta = SDL_GetTicks() - oldTime;
        deltaTime = delta/1000.0;
        oldTime = SDL_GetTicks();

        // poll events:
        while(SDL_PollEvent(&event)){ // go through all events
            if(event.type == SDL_QUIT)  // if the X button is pressed - quit
            {
                quit = true;
            }
            if(event.type == SDL_WINDOWEVENT) { // change window width, and height if resized
                SDL_GetWindowSize(window.Window, &window.width, &window.height);
            }

            player.input(event);
        }
        // loop functions:

        SDL_RenderClear(window.Renderer); // clear surface
        levelManager.checkLevel(); // check if the level is cleared

        // draw all objects

        drawBackground(window, background);
        drawObjects();

        player.draw(); // drawn at the end so the player is always on top

        printText(treePercentage);
        clearanceText.draw();
        //update frame
        SDL_SetRenderDrawColor(window.Renderer, 0, 0, 0, 255); // set color for background
        SDL_RenderPresent(window.Renderer); // update frame
    }

    SDL_Quit();
    window.Close(); // close window

    delete enemyTexture;
    delete allyTexture;
    delete treeTexture;

    return 0;
}

void drawObjects(){
    for(auto &e : enemies){
        e.draw();
    }
    for(auto &t : allTrees) {
        t.draw();
    }
    for (auto &a : allies) {
        a.draw();
    }
}

void drawBackground(Window window, SDL_Texture *background){
    int x = 0, y = 0, w = 0, h = 0;
    SDL_QueryTexture(background, nullptr, nullptr, &w, &h);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    while(rect.y < window.height){
        SDL_RenderCopy(window.Renderer, background, nullptr, &rect);

        rect.x += w;
        if(rect.x >= window.width) {
            rect.x = 0;
            rect.y += h;
        }
    }
}

void printText(Text &treePercentage){
    if(oldNum != numDeadTrees){
        oldNum = numDeadTrees;
        float perc = (float)(allTrees.size()-numDeadTrees)/(float)allTrees.size();
        int per = perc*100;
        string text = to_string(per);
        text += "%";
        treePercentage.changeText(text);
    }
    treePercentage.draw();
    levelText.draw();
}

void createTrees(Window &window, Texture *texture){
    int x = 0, y = 0;
    int treeWidth = 50;
    int treeHeight = 50;
    int id = 0;
    while(y <= HEIGHT - treeHeight){
        Tree tree(x,y,treeWidth,treeHeight, id, texture, window);
        allTrees.push_back(tree);
        x += treeWidth + 5;
        id++;
        if(x > WIDTH-treeWidth){
            x = 0;
            y += treeHeight + 3;
        }
    }
}

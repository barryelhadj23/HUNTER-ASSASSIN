#include <iostream>
#include <SDL.h>
#include "core/config.h"
#include "../map/map.h"
//Mon joueur
struct Player {
    SDL_Texture* tex;
    SDL_Rect source;
    SDL_Rect dest;
};

//Structure Point
struct Position {
    int x;
    int y;
};

//La structure qui regroupe toutes definition du jeux
struct Contenu {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    //Nombre de sprite a utilise 7 car on revien a la position de depart (1,2,3,4,5,6,1)
    Position player_left_clips[7];
    Position player_right_clips[7];
    Position player_up_clips[7];
    Position player_down_clips[7];
    Position attackUp[7];
    bool moving_left;
    bool moving_right;
    bool moving_up;
    bool moving_down;
    bool attack;
};

Contenu  contenu;

//Je charge mes image source (pour en faire une animation video
void loadClips() {
    for (int i = 0; i < 6; ++i) {
         /*contenu.player_left_clips[i].x = i * PLAYER_WIDTH;
         contenu.player_left_clips[i].y = PLAYER_HEIGHT;
         contenu.player_right_clips[i].x = i * PLAYER_WIDTH;
         contenu.player_right_clips[i].y = PLAYER_HEIGHT * 2;
         contenu.player_up_clips[i].x = i * PLAYER_WIDTH;
         contenu.player_up_clips[i].y = PLAYER_HEIGHT * 3;
         contenu.player_down_clips[i].x = i * PLAYER_WIDTH;
         contenu.player_down_clips[i].y = 0;*/

        contenu.player_down_clips[i].x = i * PLAYER_WIDTH;
        contenu.player_down_clips[i].y = PLAYER_HEIGHT * 10;
        contenu.player_right_clips[i].y = PLAYER_HEIGHT * 11;
        contenu.player_right_clips[i].x = i * PLAYER_WIDTH;
        contenu.player_left_clips[i].x = i*PLAYER_WIDTH;
        contenu.player_left_clips[i].y = PLAYER_HEIGHT * 9;
        contenu.player_up_clips[i].x = i*PLAYER_WIDTH;
        contenu.player_up_clips[i].y = PLAYER_HEIGHT * 8;

        //Attacking
        contenu.attackUp[i].y = PLAYER_HEIGHT*22;
        contenu.attackUp[i].x = (i+1)*PLAYER_WIDTH;

    }
}

void Attaque(int direction) {

}

void handleInput() {
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    contenu.moving_left = state[SDL_SCANCODE_LEFT] > 0;
    contenu.moving_right = state[SDL_SCANCODE_RIGHT] > 0;
    contenu.moving_up = state[SDL_SCANCODE_UP] > 0;
    contenu.moving_down = state[SDL_SCANCODE_DOWN] > 0;
    contenu.attack = state[SDL_SCANCODE_Q] > 0;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SDL_DestroyWindow(contenu.window);
            SDL_Quit();
            exit(0);
        }
    }
}


void updatePlayer() {
    int animation_speed = SDL_GetTicks() / 175;
    int idx = animation_speed % 6;
    float vitesse=2.4;

    if ( contenu.moving_up) {
         contenu.player.source.x =  contenu.player_up_clips[idx].x;
         contenu.player.source.y =  contenu.player_up_clips[idx].y;
         contenu.player.dest.y -= vitesse;
    }
    if ( contenu.moving_left) {

         contenu.player.source.x =  contenu.player_left_clips[idx].x;
         contenu.player.source.y =  contenu.player_left_clips[idx].y;
         contenu.player.dest.x -= vitesse;
    }
    if ( contenu.moving_down) {
        contenu.player.source.x = contenu.player_down_clips[idx].x;
        contenu.player.source.y = contenu.player_down_clips[idx].y;
        contenu.player.dest.y += vitesse;
    }
    if ( contenu.moving_right) {
        contenu.player.source.x = contenu.player_right_clips[idx].x;
        contenu.player.source.y = contenu.player_right_clips[idx].y;
        contenu.player.dest.x += vitesse;
    }
}


int main(int argc, char* args[]) {
     SDL_Init(SDL_INIT_VIDEO);

     contenu.window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
     contenu.renderer = SDL_CreateRenderer( contenu.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

     SDL_Surface* player_img = SDL_LoadBMP("../data/sprite_Player.bmp");

     contenu.player.tex = SDL_CreateTextureFromSurface( contenu.renderer, player_img);
     contenu.player.source = {0, PLAYER_HEIGHT*4, PLAYER_WIDTH, PLAYER_HEIGHT};
     contenu.player.dest = {100, 100, PLAYER_WIDTH , PLAYER_HEIGHT };
     SDL_FreeSurface(player_img);

    loadClips();
    Map mapGame;
    mapGame.initAllRectangle();

    while (true) {
        SDL_SetRenderDrawColor(contenu.renderer, 255,255,255, 255);
        SDL_RenderClear(contenu.renderer);
        handleInput();
        updatePlayer();
        SDL_RenderClear(contenu.renderer);
        //mapGame.makeMap(contenu.renderer);
        SDL_RenderCopy( contenu.renderer,  contenu.player.tex, & contenu.player.source, & contenu.player.dest);
        SDL_RenderPresent( contenu.renderer);
    }

    SDL_DestroyWindow( contenu.window);
    SDL_DestroyRenderer( contenu.renderer);
    SDL_Quit();


    return 0;
}

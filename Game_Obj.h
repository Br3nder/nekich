//
// Created by N3k14 on 07.06.2022.
//

#ifndef UNTITLED2_GAME_OBJ_H
#define UNTITLED2_GAME_OBJ_H
#include <SDL_ttf.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <iostream>

extern SDL_Renderer *ren;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Game_Obj
{
protected:
    const int LENGTH;
    SDL_Rect area;
public:
    Game_Obj(): LENGTH(40) {area.h = area.w = LENGTH;}
    void get_xy(int x, int y){area.x = x; area.y = y;}
    const int length(){return LENGTH;}
    virtual void create_obj() = 0;
    virtual void draw_obj() = 0;
    ~Game_Obj(){};
};

class Loader: public Game_Obj
{
private:
    SDL_Texture *txtr_loader;
public:
    Loader(){txtr_loader = nullptr;}
    void create_obj();
    void draw_obj();
    int set_x(){return area.x;}
    int set_y(){return area.y;}
    void move_obj(short);
    ~Loader(){SDL_DestroyTexture(txtr_loader);}
};

class Box: public Game_Obj
{
private:
    SDL_Texture *txtr_box;
public:
    Box(){txtr_box = nullptr;}
    void create_obj();
    void draw_obj();
    int set_x(){return area.x;}
    int set_y(){return area.y;}
    void move_obj(short);
    ~Box(){SDL_DestroyTexture(txtr_box);}
};

class Goal: public Game_Obj
{
private:
    SDL_Texture *txtr_goal;
public:
    Goal(){txtr_goal = nullptr;}
    void create_obj();
    void draw_obj();
    int set_x(){return area.x;}
    int set_y(){return area.y;}
    ~Goal(){SDL_DestroyTexture(txtr_goal);}
};

class Wall: public Game_Obj
{
private:
    SDL_Texture *txtr_wall;
public:
    Wall(){txtr_wall = nullptr;}
    void create_obj();
    void draw_obj();
    int set_x(){return area.x;}
    int set_y(){return area.y;}
    ~Wall(){SDL_DestroyTexture(txtr_wall);}
};

class Background_Game: public Game_Obj
{
private:
    SDL_Texture *txtr_bckgrnd;
public:
    Background_Game(){txtr_bckgrnd = nullptr;}
    void create_obj();
    void draw_obj();
    void get_wh(int w, int h){area.w = w; area.h = h;}
    void draw_bckgrnd();
    void move_obj(short);
    ~Background_Game(){SDL_DestroyTexture(txtr_bckgrnd);}
};


#endif //UNTITLED2_GAME_OBJ_H

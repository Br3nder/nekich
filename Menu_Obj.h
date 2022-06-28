//
// Created by N3k14 on 06.06.2022.
//

#ifndef UNTITLED2_MENU_OBJ_H
#define UNTITLED2_MENU_OBJ_H
#include <SDL_ttf.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

extern SDL_Renderer *ren;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Menu_Obj
{
protected:
    SDL_Rect area;
public:
    Menu_Obj(int tx, int ty){area.x = tx; area.y = ty;}
    Menu_Obj(int tx, int ty, int w, int h){area.h = h; area.w = w; area.x = tx; area.y = ty;}
    virtual void draw_obj() = 0;
    ~Menu_Obj(){}
};

class Text: public Menu_Obj
{
private:
    TTF_Font *text;
    SDL_Texture *txtr_word;
    SDL_Color color;
public:
    Text(int tx, int ty): Menu_Obj(tx, ty){color.r = color.b = color.g = 255; txtr_word = nullptr; text = nullptr;}
    void create_obj(const char*, int);
    void draw_obj();
    void clear_obj(){SDL_DestroyTexture(txtr_word); TTF_CloseFont(text); text = nullptr;}
    ~Text(){SDL_DestroyTexture(txtr_word); TTF_CloseFont(text); text = nullptr;}
};

class Button_Main: public Menu_Obj
{
private:
    SDL_Texture *txtr_but_main;
public:
    Button_Main(int tx, int ty): Menu_Obj(tx, ty, 230, 46){txtr_but_main = nullptr;}
    void create_obj();
    void draw_obj();
    ~Button_Main(){SDL_DestroyTexture(txtr_but_main);}
};

class Button_Lvl: public Menu_Obj
{
private:
    SDL_Texture *txtr_but_lvl;
public:
    Button_Lvl(int tx, int ty): Menu_Obj(tx, ty, 192, 46){txtr_but_lvl = nullptr;}
    void create_obj();
    void draw_obj();
    ~Button_Lvl(){SDL_DestroyTexture(txtr_but_lvl);}
};

class Enter_Name: public Menu_Obj
{
private:
    SDL_Texture *txtr_but_name;
public:
    Enter_Name(int tx, int ty): Menu_Obj(tx, ty, 534, 80){txtr_but_name = nullptr;}
    void create_obj();
    void draw_obj();
    ~Enter_Name(){SDL_DestroyTexture(txtr_but_name);}
};

class Arrow: public Menu_Obj
{
private:
    const int DISTANCE;
    SDL_Texture *txtr_arrow;
public:
    Arrow(int tx, int ty): DISTANCE(80), Menu_Obj(tx, ty, 40, 50){txtr_arrow = nullptr;}
    void create_obj();
    void draw_obj();
    void move_obj(short);
    ~Arrow(){SDL_DestroyTexture(txtr_arrow);}
};

class Score: public Menu_Obj
{
private:
    SDL_Texture *txtr_score;
public:
    Score(int tx, int ty): Menu_Obj(tx, ty, 188, 104){txtr_score = nullptr;}
    void create_obj();
    void draw_obj();
    ~Score(){SDL_DestroyTexture(txtr_score);}
};

class Background_Menu: public Menu_Obj
{
private:
    const int DISTANCE;
    SDL_Texture *txtr_bckgrnd;
public:
    Background_Menu(int tx = 0, int ty = 0, int tw = 0, int th = 0): DISTANCE(80), Menu_Obj(tx, ty, tw, th){txtr_bckgrnd = nullptr;}
    void create_obj();
    void draw_obj();
    void move_obj(short);
    void draw_bckgrnd();
    ~Background_Menu(){SDL_DestroyTexture(txtr_bckgrnd);}
};

class Description
{
private:
    SDL_Texture *txtr_desc;
public:
    Description(){}
    void create_obj();
    void draw_obj();
    ~Description(){SDL_DestroyTexture(txtr_desc);}
};

class SplashScreen
{
private:
    SDL_Texture *txtr_scr;
public:
    SplashScreen(){}
    void create_obj();
    void draw_obj();
    ~SplashScreen(){SDL_DestroyTexture(txtr_scr);}
};

#endif //UNTITLED2_MENU_OBJ_H

//
// Created by N3k14 on 06.06.2022.
//
#include "Menu_Obj.h"
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

void Text::create_obj(const char* str, int size)
{
    text = TTF_OpenFont("ofont.ru_Times New Roman.ttf", size);
    SDL_Surface *text_surf = TTF_RenderText_Solid(text, str, color);
    txtr_word = SDL_CreateTextureFromSurface( ren, text_surf );
    area.x -= text_surf->w/2;
    area.w = text_surf->w;
    area.h = text_surf->h;
    SDL_FreeSurface( text_surf );
}

void Text::draw_obj()
{
    SDL_RenderCopy(ren, txtr_word, nullptr, &area);
}

void Button_Main::create_obj()
{
    SDL_Surface *but_main_surf = IMG_Load( "button.png" );
    txtr_but_main = SDL_CreateTextureFromSurface( ren, but_main_surf );
    SDL_FreeSurface( but_main_surf );
}

void Button_Main::draw_obj()
{
    SDL_RenderCopy(ren, txtr_but_main, nullptr, &area);
}

void Button_Lvl::create_obj()
{
    SDL_Surface *but_lvl_surf = IMG_Load( "lvl.png" );
    txtr_but_lvl = SDL_CreateTextureFromSurface( ren, but_lvl_surf );
    SDL_FreeSurface( but_lvl_surf );
}

void Button_Lvl::draw_obj()
{
    SDL_RenderCopy(ren, txtr_but_lvl, nullptr, &area);
}

void Enter_Name::create_obj()
{
    SDL_Surface *but_name_surf = IMG_Load( "name.png" );
    txtr_but_name = SDL_CreateTextureFromSurface( ren, but_name_surf );
    SDL_FreeSurface( but_name_surf );
}

void Enter_Name::draw_obj()
{
    SDL_RenderCopy(ren, txtr_but_name, nullptr, &area);
}

void Arrow::create_obj()
{
    SDL_Surface *arrow_surf = IMG_Load( "arrow.png" );
    txtr_arrow = SDL_CreateTextureFromSurface( ren, arrow_surf );
    SDL_FreeSurface( arrow_surf );
}

void Arrow::draw_obj()
{
    SDL_RenderCopy(ren, txtr_arrow, nullptr, &area);
}

void Arrow::move_obj(short side)
{
    switch(side)
    {
        case 0: area.y+=DISTANCE; break;
        case 1: area.y-=DISTANCE;
    }
    SDL_RenderCopy(ren, txtr_arrow, nullptr, &area);
}

void Score::create_obj()
{
    SDL_Surface *score_surf = IMG_Load( "score.png" );
    txtr_score = SDL_CreateTextureFromSurface( ren, score_surf );
    SDL_FreeSurface( score_surf );
}

void Score::draw_obj()
{
    SDL_RenderCopy(ren, txtr_score, nullptr, &area);
}

void Background_Menu::create_obj()
{
    SDL_Surface *bckgrnd_surf = IMG_Load( "menu.png" );
    txtr_bckgrnd = SDL_CreateTextureFromSurface( ren, bckgrnd_surf );
    SDL_FreeSurface( bckgrnd_surf );
}

void Background_Menu::draw_obj()
{
    SDL_RenderCopy(ren, txtr_bckgrnd, &area, &area);
}

void Background_Menu::move_obj(short side)
{
    SDL_RenderCopy(ren, txtr_bckgrnd, &area, &area);
    switch(side)
    {
        case 0: area.y+=DISTANCE; break;
        case 1: area.y-=DISTANCE;
    }
}

void Background_Menu::draw_bckgrnd()
{
    SDL_RenderCopy(ren, txtr_bckgrnd, nullptr, nullptr);
}

void Description::create_obj()
{
    SDL_Surface *desc_surf = IMG_Load( "description.png" );
    txtr_desc = SDL_CreateTextureFromSurface( ren, desc_surf );
    SDL_FreeSurface( desc_surf );
}

void Description::draw_obj()
{
    SDL_RenderCopy(ren, txtr_desc, nullptr, nullptr);
}

void SplashScreen::create_obj()
{
    SDL_Surface *scr_surf = IMG_Load( "splashscreen.png" );
    txtr_scr = SDL_CreateTextureFromSurface( ren, scr_surf );
    SDL_FreeSurface( scr_surf );
}

void SplashScreen::draw_obj()
{
    SDL_RenderCopy(ren, txtr_scr, nullptr, nullptr);
}
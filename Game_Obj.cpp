//
// Created by N3k14 on 07.06.2022.
//

#include "Game_Obj.h"

#include <SDL.h>

using namespace std;

void Loader::create_obj()
{
    SDL_Surface* loader_surf = IMG_Load( "loader.png" );
    txtr_loader = SDL_CreateTextureFromSurface( ren, loader_surf );
    SDL_FreeSurface( loader_surf );
}

void Loader::draw_obj()
{
    SDL_RenderCopy(ren, txtr_loader, nullptr, &area);

}

void Loader::move_obj(short side)
{
    switch(side)
    {
        case 0: area.y-=LENGTH; break;
        case 1: area.y+=LENGTH; break;
        case 2: area.x-=LENGTH; break;
        case 3: area.x+=LENGTH;
    }
    SDL_RenderCopy(ren, txtr_loader, nullptr, &area);
}

void Box::create_obj()
{
    SDL_Surface* box_surf = IMG_Load( "box.png" );
    txtr_box = SDL_CreateTextureFromSurface( ren, box_surf );
    SDL_FreeSurface( box_surf );
}

void Box::draw_obj()
{
    SDL_RenderCopy(ren, txtr_box, nullptr, &area);
}

void Box::move_obj(short side)
{
    switch(side)
    {
        case 0: area.y-=LENGTH; break;
        case 1: area.y+=LENGTH; break;
        case 2: area.x-=LENGTH; break;
        case 3: area.x+=LENGTH;
    }
    SDL_RenderCopy(ren, txtr_box, nullptr, &area);
}

void Goal::create_obj()
{
    SDL_Surface* goal_surf = IMG_Load( "goal.png" );
    txtr_goal = SDL_CreateTextureFromSurface( ren, goal_surf );
    SDL_FreeSurface( goal_surf );
}

void Goal::draw_obj()
{
    SDL_RenderCopy(ren, txtr_goal, nullptr, &area);
}

void Wall::create_obj()
{
    SDL_Surface* wall_surf = IMG_Load( "wall.png" );
    txtr_wall = SDL_CreateTextureFromSurface( ren, wall_surf );
    SDL_FreeSurface( wall_surf );
}

void Wall::draw_obj()
{
    SDL_RenderCopy(ren, txtr_wall, nullptr, &area);
}

void Background_Game::create_obj()
{
    SDL_Surface* bckgrnd_surf = IMG_Load( "background.png" );
    txtr_bckgrnd = SDL_CreateTextureFromSurface( ren, bckgrnd_surf );
    SDL_FreeSurface( bckgrnd_surf );
}

void Background_Game::draw_obj()
{
    SDL_RenderCopy(ren, txtr_bckgrnd, &area, &area);
}

void Background_Game::draw_bckgrnd()
{
    SDL_RenderCopy(ren, txtr_bckgrnd, nullptr, nullptr);
}

void Background_Game::move_obj(short side)
{
    SDL_RenderCopy(ren, txtr_bckgrnd, &area, &area);
    switch(side)
    {
        case 0: area.y-=LENGTH; break;
        case 1: area.y+=LENGTH; break;
        case 2: area.x-=LENGTH; break;
        case 3: area.x+=LENGTH;
    }
}
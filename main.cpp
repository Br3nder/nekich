#include <iostream>
#include <string>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Menu_Obj.h"
#include "Game_Obj.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Renderer *ren = nullptr;
SDL_Window *window = nullptr;
TTF_Font *font = nullptr;

struct datas {
    char nickname[10];
    int sec;
};

bool init(); // инициализация SDL и структур
void splash_screen(); //
string new_name(); // Окно для ввода имени игрока
short create_menu(); // создание главного меню
short create_levels(); // создание меню с выбором уровня
string create_map(short); // создание карты
short pause(int *, int); // режим паузы
void game_description(); // описание игры
void game_continue(char (*)[32], int, int); // продолжить игру
short move_arrow(Arrow, int); // движение стрелки для выбора нужного пункта в меню
int draw_walls1(char *); // рисование стен для 1 уровня
int draw_walls2(char *); // рисование стен для 2 уровня
int draw_walls3(char *); // рисование стен для 3 уровня
void draw_boxes1(char *, Box *); // рисование коробок для 1 уровня
void draw_boxes2(char *, Box *); // рисование коробок для 2 уровня
void draw_boxes3(char *, Box *); // рисование коробок для 3 уровня
void draw_goals1(char *, Goal *); // рисование целей для 1 уровня
void draw_goals2(char *, Goal *); // рисование целей для 2 уровня
void draw_goals3(char *, Goal *); // рисование целей для 3 уровня
void draw_loader1(Loader *); // рисование рузчика для 1 уровня
void draw_loader2(Loader *); // рисование рузчика для 2 уровня
void draw_loader3(Loader *); // рисование рузчика для 3 уровня
string game_process(char (*)[32], Box *, Goal *, Loader *, int, Background_Game &); // ...
void cout_file(string, string, char*, short);
void cin_file(string);
void BubbleSort(datas *);
void hotkeys(short);
void quit(); // df

int (*draw_w[3])(char *) = {draw_walls1, draw_walls2, draw_walls3};
void (*draw_b[3])(char *, Box *) = {draw_boxes1, draw_boxes2, draw_boxes3};
void (*draw_g[3])(char *, Goal *) = {draw_goals1, draw_goals2, draw_goals3};
void (*draw_l[3])(Loader *) = {draw_loader1, draw_loader2, draw_loader3};

string files[3] = {"res1.bin", "res2.bin", "res3.bin"};
datas mas[3][10];

short num_of_rec[3] = {0};

int main(int argc, char *argv[]) {
    if (!init()) {
        quit();
        system("pause");
        return 1;
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 10; j++)
        {
            mas[i][j].sec = 0;
            strcpy_s(mas[i][j].nickname,  _countof(mas[i][j].nickname), "computer");
        }
    splash_screen();
    short lvl;
//    string name[10];
    int number = -1;
    char name[11];
    strcpy_s(name, _countof(name), new_name().c_str());
    short action;
    do {
        action = create_menu();
        switch (action) {
            case 0:
                lvl = create_levels();
                if (lvl != 3) {
                    string t = create_map(lvl);
                    if (t != "nt")
                        cout_file(files[lvl], t, name, lvl);
                }
                break;
            case 1:
                lvl = create_levels();
                if (lvl != 3)
                    cin_file(files[lvl]);
                break;
            case 2:
                strcpy_s(name, _countof(name), new_name().c_str());
                break;
            case 3:
                game_description();
        }
    } while (action != 4);
    for (int i = 0; i < 3; i++)
        remove(files[i].c_str());
    return 0;
}

bool init() {
    bool ok = true;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Can't init SDL: " << SDL_GetError() << endl;
        ok = false;
    }
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Can't create window: " << SDL_GetError() << endl;
        ok = false;
    }
    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        cout << "Can't create renderer: " << SDL_GetError() << endl;
        ok = false;
    }
    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags)) {
        std::cout << "Can't init image: " << IMG_GetError() << std::endl;
        ok = false;
    }
    if (TTF_Init() < 0) {
        cout << "Can't init SDL_ttf: " << TTF_GetError() << endl;
        ok = false;
    }
    return ok;
}

void splash_screen() {
    SplashScreen scr;
    scr.create_obj();
    scr.draw_obj();
    SDL_RenderPresent(ren);
    bool run = true;
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                run = false;
                break;
            }
    }
}

string new_name() {
    Text text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);
    Background_Menu m(SCREEN_WIDTH / 2 - 267, SCREEN_HEIGHT / 4, 534, 80);
    Enter_Name en(SCREEN_WIDTH / 2 - 267, SCREEN_HEIGHT / 4);
    m.create_obj();
    text.create_obj("Enter name", 28);
    en.create_obj();
    m.draw_bckgrnd();
    en.draw_obj();
    text.draw_obj();
    hotkeys(0);
    SDL_RenderPresent(ren);
    string name = "";
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            //        if( event.type == SDL_QUIT )
            //            run = false;
            if (event.type == SDL_TEXTINPUT && name.size() < 10) {
                name += event.text.text;
                m.draw_obj();
                en.draw_obj();
                Text text1(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 20);
                text1.create_obj(name.c_str(), 28);
                text1.draw_obj();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && name.size()) {
                name.pop_back();
                m.draw_obj();
                en.draw_obj();
                if (name.size()) {
                    Text text1(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 20);
                    text1.create_obj(name.c_str(), 28);
                    text1.draw_obj();
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                return name;
            SDL_RenderPresent(ren);
        }
    }
}

short create_menu() {
    Background_Menu m;
    m.create_obj();
    m.draw_bckgrnd();
    hotkeys(1);
    char a[5][9] = {"start", "records", "rename", "options", "exit"};
    int i = 0;
    for (char *punkt: a) {
        Button_Main btn(SCREEN_WIDTH / 2 - 115, SCREEN_HEIGHT / 3 + i * 80);
        Text text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 + i++ * 80 + 6);
        btn.create_obj();
        text.create_obj(punkt, 28);
        btn.draw_obj();
        text.draw_obj();
    }
    Arrow arrow(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 3);
    arrow.create_obj();
    arrow.draw_obj();
    SDL_RenderPresent(ren);
    return move_arrow(arrow, 4);
}

short create_levels() {
    Background_Menu m;
    m.create_obj();
    m.draw_bckgrnd();
    hotkeys(1);
    char a[4][11] = {"level 1", "level 2", "level 3", "come back"};
    int i = 0;
    for (char *punkt: a) {
        Button_Lvl btn_lvl(SCREEN_WIDTH / 2 - 96, SCREEN_HEIGHT / 3 + i * 80);
        Text text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 + i++ * 80 + 6);
        btn_lvl.create_obj();
        text.create_obj(punkt, 28);
        btn_lvl.draw_obj();
        text.draw_obj();
    }
    Arrow arrow(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 3);
    arrow.create_obj();
    arrow.draw_obj();
    SDL_RenderPresent(ren);
    return move_arrow(arrow, 3);
}

short move_arrow(Arrow arrow, int count) {
    Background_Menu m(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 3, 40, 50);
    m.create_obj();
    short but = 0;
    bool run = true;
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP && but) {
                but--;
                m.move_obj(1);
                arrow.move_obj(1);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN && but < count) {
                but++;
                m.move_obj(0);
                arrow.move_obj(0);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                run = false;
                break;
            }
            SDL_RenderPresent(ren);
        }
    }
    return but;
}

string create_map(short lvl) {

    char game_field[18][32] = {0};
    Background_Game fond;
    fond.create_obj();
    fond.draw_bckgrnd();
    int size = draw_w[lvl](game_field[0]);
    Goal *goals = new Goal[size];
    draw_g[lvl](game_field[0], goals);
    Box *boxes = new Box[size];
    draw_b[lvl](game_field[0], boxes);
    Loader *loader = new Loader;
    draw_l[lvl](loader);
    string nam = game_process(game_field, boxes, goals, loader, size, fond);
    delete[] boxes;
    delete[] goals;
    delete loader;
    return nam;
}

int draw_walls1(char *lvl1) {
    Wall walls[54];
    short i;
    for (i = 0; i < 54; i++)
        walls[i].create_obj();
    i = 0;
    const int l = walls[i].length();
    walls[i++].get_xy(l * 10, l * 5);
    walls[i++].get_xy(l * 11, l * 5);
    walls[i++].get_xy(l * 12, l * 5);
    walls[i++].get_xy(l * 13, l * 5);
    walls[i++].get_xy(l * 14, l * 5);
    walls[i++].get_xy(l * 15, l * 5);
    walls[i++].get_xy(l * 16, l * 5);
    walls[i++].get_xy(l * 17, l * 5);
    walls[i++].get_xy(l * 18, l * 5);
    walls[i++].get_xy(l * 19, l * 5);
    walls[i++].get_xy(l * 20, l * 5);
    walls[i++].get_xy(l * 21, l * 5);
    walls[i++].get_xy(l * 20, l * 6);
    walls[i++].get_xy(l * 21, l * 6);
    walls[i++].get_xy(l * 20, l * 7);
    walls[i++].get_xy(l * 21, l * 7);
    walls[i++].get_xy(l * 21, l * 8);
    walls[i++].get_xy(l * 21, l * 9);
    walls[i++].get_xy(l * 21, l * 10);
    walls[i++].get_xy(l * 21, l * 11);
    walls[i++].get_xy(l * 21, l * 12);
    walls[i++].get_xy(l * 21, l * 13);
    walls[i++].get_xy(l * 20, l * 13);
    walls[i++].get_xy(l * 19, l * 13);
    walls[i++].get_xy(l * 19, l * 12);
    walls[i++].get_xy(l * 18, l * 12);
    walls[i++].get_xy(l * 18, l * 13);
    walls[i++].get_xy(l * 17, l * 13);
    walls[i++].get_xy(l * 16, l * 13);
    walls[i++].get_xy(l * 15, l * 13);
    walls[i++].get_xy(l * 15, l * 12);
    walls[i++].get_xy(l * 14, l * 13);
    walls[i++].get_xy(l * 13, l * 13);
    walls[i++].get_xy(l * 13, l * 12);
    walls[i++].get_xy(l * 12, l * 13);
    walls[i++].get_xy(l * 11, l * 13);
    walls[i++].get_xy(l * 10, l * 13);
    walls[i++].get_xy(l * 10, l * 12);
    walls[i++].get_xy(l * 10, l * 11);
    walls[i++].get_xy(l * 10, l * 10);
    walls[i++].get_xy(l * 14, l * 10);
    walls[i++].get_xy(l * 10, l * 9);
    walls[i++].get_xy(l * 12, l * 9);
    walls[i++].get_xy(l * 14, l * 9);
    walls[i++].get_xy(l * 10, l * 8);
    walls[i++].get_xy(l * 12, l * 8);
    walls[i++].get_xy(l * 14, l * 8);
    walls[i++].get_xy(l * 16, l * 8);
    walls[i++].get_xy(l * 17, l * 8);
    walls[i++].get_xy(l * 10, l * 7);
    walls[i++].get_xy(l * 14, l * 7);
    walls[i++].get_xy(l * 17, l * 7);
    walls[i++].get_xy(l * 10, l * 6);
    walls[i++].get_xy(l * 12, l * 6);
    for (i = 0; i < 54; i++)
        *(lvl1 + 32 * walls[i].set_y() / l + walls[i].set_x() / l) = 1;
    for (i = 0; i < 54; i++)
        walls[i].draw_obj();
    return 4;
}

int draw_walls2(char *lvl1) {
    Wall walls[62];
    short i;
    for (i = 0; i < 62; i++)
        walls[i].create_obj();
    i = 0;
    const int l = walls[i].length();
    walls[i++].get_xy(l * 8, l * 9);
    walls[i++].get_xy(l * 9, l * 9);
    walls[i++].get_xy(l * 10, l * 9);
    walls[i++].get_xy(l * 10, l * 8);
    walls[i++].get_xy(l * 10, l * 7);
    walls[i++].get_xy(l * 11, l * 7);
    walls[i++].get_xy(l * 12, l * 7);
    walls[i++].get_xy(l * 12, l * 6);
    walls[i++].get_xy(l * 12, l * 5);
    walls[i++].get_xy(l * 12, l * 4);
    walls[i++].get_xy(l * 13, l * 4);
    walls[i++].get_xy(l * 14, l * 4);
    walls[i++].get_xy(l * 15, l * 4);
    walls[i++].get_xy(l * 16, l * 4);
    walls[i++].get_xy(l * 16, l * 5);
    walls[i++].get_xy(l * 16, l * 6);
    walls[i++].get_xy(l * 16, l * 7);
    walls[i++].get_xy(l * 17, l * 7);
    walls[i++].get_xy(l * 17, l * 8);
    walls[i++].get_xy(l * 17, l * 9);
    walls[i++].get_xy(l * 17, l * 10);
    walls[i++].get_xy(l * 18, l * 10);
    walls[i++].get_xy(l * 18, l * 9);
    walls[i++].get_xy(l * 19, l * 9);
    walls[i++].get_xy(l * 20, l * 9);
    walls[i++].get_xy(l * 21, l * 9);
    walls[i++].get_xy(l * 22, l * 9);
    walls[i++].get_xy(l * 23, l * 9);
    walls[i++].get_xy(l * 23, l * 10);
    walls[i++].get_xy(l * 23, l * 11);
    walls[i++].get_xy(l * 23, l * 12);
    walls[i++].get_xy(l * 23, l * 13);
    walls[i++].get_xy(l * 22, l * 13);
    walls[i++].get_xy(l * 21, l * 13);
    walls[i++].get_xy(l * 20, l * 13);
    walls[i++].get_xy(l * 19, l * 13);
    walls[i++].get_xy(l * 18, l * 13);
    walls[i++].get_xy(l * 18, l * 12);
    walls[i++].get_xy(l * 18, l * 14);
    walls[i++].get_xy(l * 17, l * 14);
    walls[i++].get_xy(l * 16, l * 14);
    walls[i++].get_xy(l * 15, l * 14);
    walls[i++].get_xy(l * 14, l * 14);
    walls[i++].get_xy(l * 13, l * 14);
    walls[i++].get_xy(l * 12, l * 14);
    walls[i++].get_xy(l * 12, l * 13);
    walls[i++].get_xy(l * 12, l * 12);
    walls[i++].get_xy(l * 11, l * 12);
    walls[i++].get_xy(l * 10, l * 12);
    walls[i++].get_xy(l * 9, l * 12);
    walls[i++].get_xy(l * 8, l * 12);
    walls[i++].get_xy(l * 8, l * 11);
    walls[i++].get_xy(l * 8, l * 10);
    walls[i++].get_xy(l * 12, l * 10);
    walls[i++].get_xy(l * 12, l * 9);
    walls[i++].get_xy(l * 14, l * 10);
    walls[i++].get_xy(l * 15, l * 10);
    walls[i++].get_xy(l * 14, l * 9);
    walls[i++].get_xy(l * 15, l * 9);
    walls[i++].get_xy(l * 14, l * 12);
    walls[i++].get_xy(l * 15, l * 12);
    walls[i++].get_xy(l * 16, l * 12);
    for (i = 0; i < 62; i++)
        *(lvl1 + 32 * walls[i].set_y() / l + walls[i].set_x() / l) = 1;
    for (i = 0; i < 62; i++)
        walls[i].draw_obj();
    return 6;
}

int draw_walls3(char *lvl1) {
    Wall walls[62];
    short i;
    for (i = 0; i < 62; i++)
        walls[i].create_obj();
    i = 0;
    const int l = walls[i].length();
    walls[i++].get_xy(l * 9, l * 4);
    walls[i++].get_xy(l * 10, l * 4);
    walls[i++].get_xy(l * 11, l * 4);
    walls[i++].get_xy(l * 12, l * 4);
    walls[i++].get_xy(l * 13, l * 4);
    walls[i++].get_xy(l * 14, l * 4);
    walls[i++].get_xy(l * 14, l * 5);
    walls[i++].get_xy(l * 14, l * 6);
    walls[i++].get_xy(l * 14, l * 7);
    walls[i++].get_xy(l * 15, l * 4);
    walls[i++].get_xy(l * 16, l * 4);
    walls[i++].get_xy(l * 17, l * 4);
    walls[i++].get_xy(l * 18, l * 4);
    walls[i++].get_xy(l * 19, l * 4);
    walls[i++].get_xy(l * 20, l * 4);
    walls[i++].get_xy(l * 20, l * 5);
    walls[i++].get_xy(l * 21, l * 5);
    walls[i++].get_xy(l * 22, l * 5);
    walls[i++].get_xy(l * 22, l * 6);
    walls[i++].get_xy(l * 22, l * 7);
    walls[i++].get_xy(l * 22, l * 8);
    walls[i++].get_xy(l * 22, l * 9);
    walls[i++].get_xy(l * 21, l * 9);
    walls[i++].get_xy(l * 22, l * 10);
    walls[i++].get_xy(l * 22, l * 11);
    walls[i++].get_xy(l * 22, l * 12);
    walls[i++].get_xy(l * 22, l * 13);
    walls[i++].get_xy(l * 21, l * 13);
    walls[i++].get_xy(l * 20, l * 13);
    walls[i++].get_xy(l * 19, l * 13);
    walls[i++].get_xy(l * 18, l * 13);
    walls[i++].get_xy(l * 17, l * 13);
    walls[i++].get_xy(l * 16, l * 13);
    walls[i++].get_xy(l * 16, l * 12);
    walls[i++].get_xy(l * 15, l * 13);
    walls[i++].get_xy(l * 14, l * 13);
    walls[i++].get_xy(l * 13, l * 13);
    walls[i++].get_xy(l * 12, l * 13);
    walls[i++].get_xy(l * 11, l * 13);
    walls[i++].get_xy(l * 11, l * 12);
    walls[i++].get_xy(l * 11, l * 11);
    walls[i++].get_xy(l * 11, l * 10);
    walls[i++].get_xy(l * 12, l * 10);
    walls[i++].get_xy(l * 13, l * 10);
    walls[i++].get_xy(l * 14, l * 10);
    walls[i++].get_xy(l * 14, l * 9);
    walls[i++].get_xy(l * 10, l * 10);
    walls[i++].get_xy(l * 9, l * 10);
    walls[i++].get_xy(l * 9, l * 9);
    walls[i++].get_xy(l * 9, l * 8);
    walls[i++].get_xy(l * 9, l * 7);
    walls[i++].get_xy(l * 9, l * 6);
    walls[i++].get_xy(l * 9, l * 5);
    walls[i++].get_xy(l * 16, l * 10);
    walls[i++].get_xy(l * 16, l * 9);
    walls[i++].get_xy(l * 17, l * 10);
    walls[i++].get_xy(l * 16, l * 7);
    walls[i++].get_xy(l * 17, l * 7);
    walls[i++].get_xy(l * 18, l * 7);
    walls[i++].get_xy(l * 19, l * 7);
    walls[i++].get_xy(l * 18, l * 8);
    walls[i++].get_xy(l * 19, l * 8);
    for (i = 0; i < 62; i++)
        *(lvl1 + 32 * walls[i].set_y() / l + walls[i].set_x() / l) = 1;
    for (i = 0; i < 62; i++)
        walls[i].draw_obj();
    return 10;
}

void draw_boxes1(char *lvl1, Box *boxes) {
    short i;
    for (i = 0; i < 4; i++)
        boxes[i].create_obj();
    i = 0;
    const int l = boxes[i].length();
    boxes[i++].get_xy(l * 12, l * 7);
    boxes[i++].get_xy(l * 15, l * 11);
    boxes[i++].get_xy(l * 17, l * 9);
    boxes[i++].get_xy(l * 19, l * 10);
    for (i = 0; i < 4; i++)
        *(lvl1 + 32 * boxes[i].set_y() / l + boxes[i].set_x() / l) = 2;
    for (i = 0; i < 4; i++)
        boxes[i].draw_obj();
}

void draw_boxes2(char *lvl1, Box *boxes) {
    short i;
    for (i = 0; i < 6; i++)
        boxes[i].create_obj();
    i = 0;
    const int l = boxes[i].length();
    boxes[i++].get_xy(l * 10, l * 11);
    boxes[i++].get_xy(l * 13, l * 11);
    boxes[i++].get_xy(l * 13, l * 8);
    boxes[i++].get_xy(l * 13, l * 6);
    boxes[i++].get_xy(l * 15, l * 8);
    boxes[i++].get_xy(l * 15, l * 7);
    for (i = 0; i < 6; i++)
        *(lvl1 + 32 * boxes[i].set_y() / l + boxes[i].set_x() / l) = 2;
    for (i = 0; i < 6; i++)
        boxes[i].draw_obj();
}

void draw_boxes3(char *lvl1, Box *boxes) {
    short i;
    for (i = 0; i < 10; i++)
        boxes[i].create_obj();
    i = 0;
    const int l = boxes[i].length();
    boxes[i++].get_xy(l * 13, l * 11);
    boxes[i++].get_xy(l * 16, l * 11);
    boxes[i++].get_xy(l * 18, l * 11);
    boxes[i++].get_xy(l * 20, l * 11);
    boxes[i++].get_xy(l * 18, l * 10);
    boxes[i++].get_xy(l * 20, l * 10);
    boxes[i++].get_xy(l * 19, l * 9);
    boxes[i++].get_xy(l * 19, l * 6);
    boxes[i++].get_xy(l * 16, l * 6);
    boxes[i++].get_xy(l * 15, l * 7);
    for (i = 0; i < 10; i++)
        *(lvl1 + 32 * boxes[i].set_y() / l + boxes[i].set_x() / l) = 2;
    for (i = 0; i < 10; i++)
        boxes[i].draw_obj();
}

void draw_goals1(char *lvl1, Goal *goals) {
    short i;
    for (i = 0; i < 4; i++)
        goals[i].create_obj();
    i = 0;
    const int l = goals[i].length();
    goals[i++].get_xy(l * 11, l * 9);
    goals[i++].get_xy(l * 16, l * 12);
    goals[i++].get_xy(l * 20, l * 12);
    goals[i++].get_xy(l * 15, l * 8);
    for (i = 0; i < 4; i++)
        *(lvl1 + 32 * goals[i].set_y() / l + goals[i].set_x() / l) = 3;
    for (i = 0; i < 4; i++)
        goals[i].draw_obj();
}

void draw_goals2(char *lvl1, Goal *goals) {
    short i;
    for (i = 0; i < 6; i++)
        goals[i].create_obj();
    i = 0;
    const int l = goals[i].length();
    goals[i++].get_xy(l * 21, l * 12);
    goals[i++].get_xy(l * 22, l * 12);
    goals[i++].get_xy(l * 21, l * 11);
    goals[i++].get_xy(l * 22, l * 11);
    goals[i++].get_xy(l * 21, l * 10);
    goals[i++].get_xy(l * 22, l * 10);
    for (i = 0; i < 6; i++)
        *(lvl1 + 32 * goals[i].set_y() / l + goals[i].set_x() / l) = 3;
    for (i = 0; i < 6; i++)
        goals[i].draw_obj();
}

void draw_goals3(char *lvl1, Goal *goals) {
    short i;
    for (i = 0; i < 10; i++)
        goals[i].create_obj();
    i = 0;
    const int l = goals[i].length();
    goals[i++].get_xy(l * 10, l * 5);
    goals[i++].get_xy(l * 10, l * 6);
    goals[i++].get_xy(l * 10, l * 7);
    goals[i++].get_xy(l * 10, l * 8);
    goals[i++].get_xy(l * 10, l * 9);
    goals[i++].get_xy(l * 11, l * 5);
    goals[i++].get_xy(l * 11, l * 6);
    goals[i++].get_xy(l * 11, l * 7);
    goals[i++].get_xy(l * 11, l * 8);
    goals[i++].get_xy(l * 11, l * 9);
    for (i = 0; i < 10; i++)
        *(lvl1 + 32 * goals[i].set_y() / l + goals[i].set_x() / l) = 3;
    for (i = 0; i < 10; i++)
        goals[i].draw_obj();
}

void draw_loader1(Loader *loader) {
    const int l = loader->length();
    loader->create_obj();
    loader->get_xy(l * 17, l * 10);
    loader->draw_obj();
}

void draw_loader2(Loader *loader) {
    const int l = loader->length();
    loader->create_obj();
    loader->get_xy(l * 19, l * 12);
    loader->draw_obj();
}

void draw_loader3(Loader *loader) {
    const int l = loader->length();
    loader->create_obj();
    loader->get_xy(l * 16, l * 8);
    loader->draw_obj();
}

// 0 - свободно, 1 - стена, 2 - коробка, 3 - цель, 4 - коробка на цели.
string game_process(char (*g_f)[32], Box *boxes, Goal *goals, Loader *loader, int size, Background_Game &fnd) {
    Background_Game redraw_clock;
    redraw_clock.create_obj();
    redraw_clock.get_xy(SCREEN_WIDTH - 140, 40);
    redraw_clock.get_wh(110, 50);
    Text t(SCREEN_WIDTH - 170, 40);
    t.create_obj("Time: ", 28);
    t.draw_obj();
    string time_text = "00:0/";
    const int l = boxes[0].length();
    int count_goals = size;
    int i, y, x;
    fnd.get_xy(loader->set_x(), loader->set_y());
    hotkeys(2);
    SDL_RenderPresent(ren);
    int start_t = clock(), real_t;
    int sec = -1;
    short choice;
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            y = loader->set_y() / l;
            x = loader->set_x() / l;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP && g_f[y - 1][x] != 1) {
                if (g_f[y - 1][x] == 0 || g_f[y - 1][x] == 3) // если сверху свободно
                {
                    fnd.move_obj(0);
                    loader->move_obj(0);
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
                if ((g_f[y - 1][x] == 2 || g_f[y - 1][x] == 4) && g_f[y - 2][x] != 1 && g_f[y - 2][x] != 2 &&
                           g_f[y - 2][x] != 4)// если сверху коробка и дальше свободно
                {
                    fnd.move_obj(0);
                    fnd.move_obj(0);
                    fnd.get_xy(x * l, (y - 1) * l);
                    loader->move_obj(0);
                    for (i = 0; i < size - 1 && (boxes[i].set_x() / l != x || boxes[i].set_y() / l != y - 1); i++);
                    boxes[i].move_obj(0);
                    if (g_f[y - 2][x] == 0)
                        g_f[y - 2][x] = 2;
                    else {
                        g_f[y - 2][x] = 4;
                        count_goals--;
                    }
                    if (g_f[y - 1][x] == 4) {
                        g_f[y - 1][x] = 3;
                        count_goals++;
                    } else
                        g_f[y - 1][x] = 0;
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
            } // движение вверх
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN && g_f[y + 1][x] != 1) {
                if (g_f[y + 1][x] == 0 || g_f[y + 1][x] == 3) {
                    fnd.move_obj(1);
                    loader->move_obj(1);
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
                if ((g_f[y + 1][x] == 2 || g_f[y + 1][x] == 4) && g_f[y + 2][x] != 1 && g_f[y + 2][x] != 2 &&
                    g_f[y + 2][x] != 4) {
                    fnd.move_obj(1);
                    fnd.move_obj(1);
                    fnd.get_xy(x * l, (y + 1) * l);
                    loader->move_obj(1);
                    for (i = 0; i < size - 1 && (boxes[i].set_x() / l != x || boxes[i].set_y() / l != y + 1); i++);
                    boxes[i].move_obj(1);
                    if (g_f[y + 2][x] == 0)
                        g_f[y + 2][x] = 2;
                    else {
                        g_f[y + 2][x] = 4;
                        count_goals--;
                    }
                    if (g_f[y + 1][x] == 4) {
                        g_f[y + 1][x] = 3;
                        count_goals++;
                    } else
                        g_f[y + 1][x] = 0;
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
            }// движение вниз
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT && g_f[y][x - 1] != 1) {
                if (!g_f[y][x - 1] || g_f[y][x - 1] == 3) {
                    fnd.move_obj(2);
                    loader->move_obj(2);
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
                if ((g_f[y][x - 1] == 2 || g_f[y][x - 1] == 4) && g_f[y][x - 2] != 1 && g_f[y][x - 2] != 2 &&
                    g_f[y][x - 2] != 4) {
                    fnd.move_obj(2);
                    fnd.move_obj(2);
                    fnd.get_xy((x - 1) * l, y * l);
                    loader->move_obj(2);
                    for (i = 0; i < size - 1 && (boxes[i].set_x() / l != x - 1 || boxes[i].set_y() / l != y); i++);
                    boxes[i].move_obj(2);
                    if (g_f[y][x - 2] == 0)
                        g_f[y][x - 2] = 2;
                    else {
                        g_f[y][x - 2] = 4;
                        count_goals--;
                    }
                    if (g_f[y][x - 1] == 4) {
                        g_f[y][x - 1] = 3;
                        count_goals++;
                    } else
                        g_f[y][x - 1] = 0;
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
            }// движение влево
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT && g_f[y][x + 1] != 1) {
                if (!g_f[y][x + 1] || g_f[y][x + 1] == 3) {
                    fnd.move_obj(3);
                    loader->move_obj(3);
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
                if ((g_f[y][x + 1] == 2 || g_f[y][x + 1] == 4) && g_f[y][x + 2] != 1 && g_f[y][x + 2] != 2 &&
                    g_f[y][x + 2] != 4) {
                    fnd.move_obj(3);
                    fnd.move_obj(3);
                    fnd.get_xy((x + 1) * l, y * l);
                    loader->move_obj(3);
                    for (i = 0; i < size - 1 && (boxes[i].set_x() / l != x + 1 || boxes[i].set_y() / l != y); i++);
                    boxes[i].move_obj(3);
                    if (g_f[y][x + 2] == 0)
                        g_f[y][x + 2] = 2;
                    else {
                        g_f[y][x + 2] = 4;
                        count_goals--;
                    }
                    if (g_f[y][x + 1] == 4) {
                        g_f[y][x + 1] = 3;
                        count_goals++;
                    } else
                        g_f[y][x + 1] = 0;
                    if (g_f[y][x] == 3) {
                        for (i = 0; i < size - 1 && (goals[i].set_x() / l != x || goals[i].set_y() / l != y); i++);
                        goals[i].draw_obj();
                    }
                }
            }// движение вправо
            if (!count_goals) {
                Score score(SCREEN_WIDTH / 2 - 94, SCREEN_HEIGHT / 2 - 52);
                Text text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 44);
                Text finally_time(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 5);
                score.create_obj();
                text.create_obj("Your time", 40);
                finally_time.create_obj(time_text.c_str(), 40);
                score.draw_obj();
                text.draw_obj();
                finally_time.draw_obj();
                SDL_RenderPresent(ren);
                while (true) {
                    while (SDL_PollEvent(&event))
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                            return time_text;
                }
            } // если все ящики расставлен по местам
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                choice = pause(&sec, start_t);
                if (choice)
                    return "nt";
                else {
                    game_continue(g_f, x, y);
                    t.draw_obj();
                    hotkeys(2);
                    Text time(SCREEN_WIDTH - 100, 40);
                    time.create_obj(time_text.c_str(), 28);
                    time.draw_obj();
                }
            } // если нажата пауза
        }
        if (sec != (clock() - start_t) / CLOCKS_PER_SEC) // подсчет времени
        {
            redraw_clock.draw_obj();
            sec++;
            if (time_text[4] == '9') {
                if (time_text[3] == '5') {
                    if (time_text[1] == '9') {
                        time_text[0] = (int) time_text[0] + 1;
                        time_text[1] = 48;
                    } else
                        time_text[1] = (int) time_text[1] + 1;
                    time_text[3] = time_text[4] = 48;
                } else {
                    time_text[3] = (int) time_text[3] + 1;
                    time_text[4] = 48;
                }
            } else
                time_text[4] = (int) time_text[4] + 1;
            Text time(SCREEN_WIDTH - 100, 40);
//            time.clear_obj();
            time.create_obj(time_text.c_str(), 28);
            time.draw_obj();
        }
        SDL_RenderPresent(ren);
    }
}

short pause(int *sec, int start_t) {
    Background_Menu m;
    m.create_obj();
    m.draw_bckgrnd();
    hotkeys(1);
    char a[2][13] = {"continue", "back to menu"};
    int i = 0;
    for (char *punkt: a) {
        Button_Main btn(SCREEN_WIDTH / 2 - 115, SCREEN_HEIGHT / 3 + i * 80);
        Text text(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3 + i++ * 80 + 6);
        btn.create_obj();
        text.create_obj(punkt, 28);
        btn.draw_obj();
        text.draw_obj();
    }
    Arrow arrow(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 3);
    arrow.create_obj();
    arrow.draw_obj();
    SDL_RenderPresent(ren);
    short temp = move_arrow(arrow, 1);
    *sec = (clock() - start_t) / CLOCKS_PER_SEC;
    return temp;
}

void game_continue(char (*g_f)[32], int x, int y) {
    Background_Game fond;
    fond.create_obj();
    fond.draw_bckgrnd();
    Loader loader;
    Wall wall;
    Box box;
    Goal goal;
    const int l = loader.length();
    wall.create_obj();
    box.create_obj();
    goal.create_obj();
    loader.create_obj();
    for (int j = 0; j < 18; j++)
        for (int t = 0; t < 32; t++)
            switch (g_f[j][t]) {
                case 1:
                    wall.get_xy(t * l, j * l);
                    wall.draw_obj();
                    break;
                case 2:
                    box.get_xy(t * l, j * l);
                    box.draw_obj();
                    break;
                case 3:
                    goal.get_xy(t * l, j * l);
                    goal.draw_obj();
                    break;
                case 4:
                    box.get_xy(t * l, j * l);
                    box.draw_obj();
            }
    loader.get_xy(x * l, y * l);
    loader.draw_obj();
}

void game_description() {
    Description desc;
    desc.create_obj();
    desc.draw_obj();
    hotkeys(0);
    SDL_RenderPresent(ren);
    bool run = true;
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                run = false;
                break;
            }
    }
}

void cout_file(string sfile, string res, char *name, short levl)
{
    int sec = ((int) res[0] - 48) * 600 + ((int) res[1] - 48) * 60 + ((int) res[3] - 48) * 10 + ((int) res[4] - 48);
    datas new_res;
    new_res.sec = sec;
    strcpy_s(new_res.nickname, _countof(new_res.nickname), name);
    if (mas[levl][0].sec == 0)
        mas[levl][0] = new_res;
    else
        if(mas[levl][9].sec > sec)
            mas[levl][9] = new_res;
    BubbleSort(mas[levl]);
    ofstream filer(sfile.c_str(), ios::out | ios::binary);
    for (int i = 0; i < 10; i++)
        filer.write((char*) &mas[levl][i], sizeof(datas));
    filer.close();
}

void cin_file(string sfile)
{
    Background_Menu m;
    m.create_obj();
    m.draw_bckgrnd();
    hotkeys(0);
    ifstream filer(sfile.c_str(), ios::in | ios::binary);
    datas d;
    int j = 0, c = 0;
    for (int i = 0; i < 10; i++)
    {
        if(!filer.read((char *) &d, sizeof(datas)))
            break;
        c++;
        if (d.sec)
        {
            Text text_n(200, 150 + j*40);
            text_n.create_obj(d.nickname, 28);
            text_n.draw_obj();
            char temp[6] = "00:00";
            temp[4] = (char)(d.sec%10+48);
            temp[3] = (char)(d.sec/10%6+48);
            temp[1] = (char)(d.sec/60%10+48);
            temp[0] = (char)(d.sec/600+48);
            Text text_s(SCREEN_WIDTH-200, 150 + j++*40);
            text_s.create_obj(temp, 28);
            text_s.draw_obj();
        }
    }
    if(!c)
    {
        Text text_m(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        text_m.create_obj("No records", 40);
        text_m.draw_obj();
    }
    else
    {
        Text text_m(SCREEN_WIDTH/2, 50);
        Text text_n(200, 100);
        Text text_s(SCREEN_WIDTH-200, 100);
        text_m.create_obj("Records", 40);
        text_n.create_obj("nickname", 28);
        text_s.create_obj("time", 28);
        text_m.draw_obj();
        text_n.draw_obj();
        text_s.draw_obj();
    }
    filer.close();
    SDL_RenderPresent(ren);
    bool run = true;
    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
            {
                run = false;
                break;
            }
        }
    }
}

void BubbleSort(datas *a) {
    int i, j, n = 10;
    datas x;
    for (i = 1; i < n; i++)
        for (j = n - 1; j >= i; j--)
            if (a[j - 1].sec > a[j].sec) {
                x = a[j - 1];
                a[j - 1] = a[j];
                a[j] = x;
            }
}

void hotkeys(short cl)
{
    if(cl < 2)
    {
        Text text1(SCREEN_WIDTH/2, SCREEN_HEIGHT-80);
        text1.create_obj("«Enter» choose", 24);
        text1.draw_obj();
    }
    if(cl)
    {
        Text text2(SCREEN_WIDTH-200, SCREEN_HEIGHT/2-25);
        Text text3(SCREEN_WIDTH-200, SCREEN_HEIGHT/2+25);
        text2.create_obj("«up arrow» up", 24);
        text3.create_obj("«down arrow» down", 24);
        text2.draw_obj();
        text3.draw_obj();
    }
    if(cl == 2)
    {
        Text text2(SCREEN_WIDTH-200, SCREEN_HEIGHT/2-75);
        Text text3(SCREEN_WIDTH-200, SCREEN_HEIGHT/2+75);
        Text text4(SCREEN_WIDTH-200, SCREEN_HEIGHT/2+125);
        text2.create_obj("«right arrow» right", 24);
        text3.create_obj("«left arrow» left", 24);
        text4.create_obj("«space» pause", 24);
        text2.draw_obj();
        text3.draw_obj();
        text4.draw_obj();
    }
}

void quit() {
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(ren);
    ren = nullptr;
}


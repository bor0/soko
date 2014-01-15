/*
This file is part of Soko.

Soko is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Soko is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Soko. If not, see <http://www.gnu.org/licenses/>.

*/
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <math.h>

#include "soko_sdl.h"
#include "game.h"

#define ASSET_WIDTH 32
#define ASSET_HEIGHT 32

SDL_Surface *img_loader(char *file) {
    SDL_Surface *pic;

    pic = IMG_Load(file);

    if (pic == NULL) {
        return NULL;
    }

    pic = SDL_DisplayFormat(pic);

    return pic;
}

void render(SDL_Surface *screen, SDL_Surface **assets, level *map) {
    SDL_Rect tmp = { 0, 0, ASSET_WIDTH, ASSET_HEIGHT };
    int i, j, beacon = 0;

    SDL_FillRect(screen, NULL, 0);

    for (i = 0; i < map->width; i++) {
        for (j = 0; j < map->height; j++) {
            tmp.x = i*ASSET_WIDTH;
            tmp.y = j*ASSET_HEIGHT;
            SDL_BlitSurface(assets[map->data[j][i]], NULL, screen, &tmp);
            if (map->data[j][i] == LEVEL_BEACON) beacon = 1;
        }
    }

    /* shouldn't do victory update on rendering stuff */
    if (!beacon && !map->player_on_beacon) {
        map->win = 1;
    }

    SDL_Flip(screen);
}

void sokosdl_main(level *map) {
    SDL_Event event;
    SDL_Surface *screen, *assets[7];
    int run = 1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return;

    SDL_WM_SetCaption("Soko v1.0 by Boro Sitnikovski", NULL);

    screen = SDL_SetVideoMode(ASSET_WIDTH * map->width, ASSET_HEIGHT * map->height, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

    assets[LEVEL_EMPTY] = img_loader("gfx/empty.png");
    assets[LEVEL_WALL] = img_loader("gfx/wall.png");
    assets[LEVEL_TERRAIN] = img_loader("gfx/terrain.png");
    assets[LEVEL_BOX] = img_loader("gfx/box.png");
    assets[LEVEL_BEACON] = img_loader("gfx/beacon.png");
    assets[LEVEL_B_BEACON] = img_loader("gfx/b_beacon.png");
    assets[LEVEL_SOKOBAN] = img_loader("gfx/sokoban.png");

    if (SDL_Init(SDL_INIT_AUDIO) < 0) return;

    /* setup audio mode */
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    Mix_Music *mus;
    mus = Mix_LoadMUS("music/untzuntz.mp3");
    Mix_PlayMusic(mus, -1);

    while (run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            soko_play(map, 0, -1);
                            break;
                        case SDLK_DOWN:
                            soko_play(map, 0, 1);
                            break;
                        case SDLK_LEFT:
                            soko_play(map, -1, 0);
                            break;
                        case SDLK_RIGHT:
                            soko_play(map, 1, 0);
                            break;
                        case SDLK_ESCAPE:
                            run = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    run = 0;
                    break;
                default:
                    break;
            }
        }
        render(screen, assets, map);
        if (map->win) {
            printf("You won!\n");
            break;
        }
    }

    Mix_FreeMusic(mus);
}

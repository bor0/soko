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

SDL_Surface *_img_loader(char *file) {
    SDL_Surface *pic;

    pic = IMG_Load(file);

    if (pic == NULL) {
        return NULL;
    }

    pic = SDL_DisplayFormat(pic);

    return pic;
}

void _render(SDL_Surface *screen, SDL_Surface **assets, int asset_width, int asset_height, level *map) {
    SDL_Rect tmp = { 0, 0, asset_width, asset_height };
    int i, j;

    SDL_FillRect(screen, NULL, 0);

    for (i = 0; i < map->width; i++) {
        for (j = 0; j < map->height; j++) {
            tmp.x = i * asset_width;
            tmp.y = j * asset_height;
            SDL_BlitSurface(assets[map->data[j][i]], NULL, screen, &tmp);
        }
    }

    SDL_Flip(screen);
}

void sokosdl_main(level *map, int asset_width, int asset_height) {
    SDL_Event event;
    SDL_Surface *screen, *assets[9];
    int run = 1;
    Mix_Music *mus = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return;

    SDL_WM_SetCaption("Soko v1.0 by Boro Sitnikovski", NULL);

    screen = SDL_SetVideoMode(asset_width * map->width, asset_height * map->height, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

    assets[LEVEL_EMPTY] = _img_loader("gfx/empty.png");
    assets[LEVEL_WALL] = _img_loader("gfx/wall.png");
    assets[LEVEL_TERRAIN] = _img_loader("gfx/terrain.png");
    assets[LEVEL_BOX] = _img_loader("gfx/box.png");
    assets[LEVEL_BEACON] = _img_loader("gfx/beacon.png");
    assets[LEVEL_B_BEACON] = _img_loader("gfx/b_beacon.png");
    assets[LEVEL_SOKOBAN] = _img_loader("gfx/sokoban.png");
    assets[LEVEL_DOOR] = _img_loader("gfx/door.png");
    assets[LEVEL_KEY] = _img_loader("gfx/key.png");

    if (SDL_Init(SDL_INIT_AUDIO) < 0) return;

    /* setup audio mode */
    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) != -1) {
        mus = Mix_LoadMUS("music/untzuntz.ogg");
        Mix_PlayMusic(mus, -1);
    }

    while (run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            play(map, 0, -1);
                            break;
                        case SDLK_DOWN:
                            play(map, 0, 1);
                            break;
                        case SDLK_LEFT:
                            play(map, -1, 0);
                            break;
                        case SDLK_RIGHT:
                            play(map, 1, 0);
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
        _render(screen, assets, asset_width, asset_height, map);
        if (check_win(map) == 1) {
            printf("You won!\n");
            run = 0;
        }
    }

    if (mus) {
        Mix_CloseAudio();
        Mix_FreeMusic(mus);
    }
}

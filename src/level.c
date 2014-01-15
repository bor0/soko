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
#include <stdio.h>
#include <stdlib.h>
#include "level.h"

void free_level(level **map) {
    int i;

    for (i = 0; i < (*map)->width; i++) {
        free((*map)->data[i]);
    }

    free((*map)->data);
    free(*map);
}

level *read_level(char *level_name) {
    char buffer[512];
    FILE *t;
    int i, j, soko = 0, tmp;
    level *map;

    sprintf(buffer, "levels/%s.dat", level_name);

    t = fopen(buffer, "r");

    if (!t) {
        return NULL;
    }

    map = (level *)malloc(sizeof(level));

    if (fscanf(t, "%d %d", &map->width, &map->height) != 2) {
        fclose(t);
        return NULL;
    }

    map->data = (char **)malloc(sizeof(char *) * map->height);

    for (i = 0; i < map->height; i++) {
        map->data[i] = malloc(sizeof(char) * map->width);

        for (j = 0; j < map->width; j++) {
            /* file does not contain enough information */
            if (feof(t) || fscanf(t, "%d", &tmp) != 1) {
                fclose(t);
                free_level(&map);
                return NULL;
            }

            /* handle overflow or underflow */
            if (tmp > LEVEL_KEY || tmp < LEVEL_EMPTY) {
                tmp = LEVEL_WALL;
            }

            map->data[i][j] = (unsigned char)tmp;

            if (map->data[i][j] == LEVEL_SOKOBAN) {
                /* soko already specified elsewhere. treat other soko values as walls */
                if (soko == 1) {
                    map->data[i][j] = LEVEL_WALL;
                } else {
                    soko = 1;
                    map->player_x = i;
                    map->player_y = j;
                }
            }
        }

    }

    fclose(t);

    /* if there is no soko specified in map assign it to pos (0, 0) */
    if (!soko) {
        map->player_x = map->player_y = 0;
        map->data[0][0] = LEVEL_SOKOBAN;
    }

    map->player_on_beacon = map->win = map->key_count = 0;

    return map;
}


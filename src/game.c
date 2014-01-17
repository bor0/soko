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
#include "game.h"
#include <stdio.h>

static int _bounds_check(struct level *map, int x, int y)
{
    if (x < 0 || y < 0 || x >= map->width || y >= map->height) return 0;
    return 1;
}

/* this function assumes bounds are valid */
static void _update_soko(struct level *map, int new_x, int new_y)
{
    /* if player is on beacon, update old position with beacon */
    if (map->soko.player_on_beacon) {
        map->data[map->soko.y][map->soko.x] = LEVEL_BEACON;
        map->soko.player_on_beacon = 0;
    }
    /* else update the position with terrain */
    else {
        map->data[map->soko.y][map->soko.x] = LEVEL_TERRAIN;
    }

    map->soko.x = new_x;
    map->soko.y = new_y;

    /* if player steps on a beacon, set player_on_beacon */
    if (map->data[new_y][new_x] == LEVEL_BEACON) {
        map->soko.player_on_beacon = 1;
    }

    /* update new position with soko */
    map->data[new_y][new_x] = LEVEL_SOKOBAN;
}

/* this function assumes bounds are valid */
static int _update_box(struct level *map, int box_x, int box_y, int new_player_x, int new_player_y)
{
    int ret = 0;

    switch (map->data[box_y][box_x]) {
    /* convert beacon to boxed beacon */
    case LEVEL_BEACON:
        map->data[box_y][box_x] = LEVEL_B_BEACON;
        ret = 1;
        break;
    /* convert terrain to box */
    case LEVEL_TERRAIN:
        map->data[box_y][box_x] = LEVEL_BOX;
        ret = 1;
        break;
    /* can't push against other things */
    default:
        break;
    }

    return ret;

}

void play(struct level *map, int dx, int dy)
{
    int new_player_x = map->soko.x + dx;
    int new_player_y = map->soko.y + dy;
    int box_x, box_y;

    if (!_bounds_check(map, new_player_x, new_player_y)) {
        return;
    }

    switch (map->data[new_player_y][new_player_x]) {
    /* handle getting a key, just increase key count and update soko position */
    case LEVEL_KEY:
        map->soko.key_count++;
        _update_soko(map, new_player_x, new_player_y);
        break;
    /* handle opening doors */
    case LEVEL_DOOR:
        if (map->soko.key_count > 0) {
            map->soko.key_count--;
            _update_soko(map, new_player_x, new_player_y);
        }
        break;
    /* no special cases to handle, just update soko's position (beacon is handled within update function) */
    case LEVEL_BEACON:
    case LEVEL_TERRAIN:
        _update_soko(map, new_player_x, new_player_y);
        break;
    /* when pushing a boxed beacon, set beacon flag to 1 (in case move is successful and we step into a beacon) */
    case LEVEL_B_BEACON:
        box_x = new_player_x + dx;
        box_y = new_player_y + dy;
        if (_bounds_check(map, box_x, box_y)) {
            /* if it is a successful box update and move, set beacon flag */
            if (_update_box(map, box_x, box_y, new_player_x, new_player_y)) {
                _update_soko(map, new_player_x, new_player_y);
                map->soko.player_on_beacon = 1;
            }
        }
        break;
    case LEVEL_BOX:
        box_x = new_player_x + dx;
        box_y = new_player_y + dy;
        if (_update_box(map, box_x, box_y, new_player_x, new_player_y)) {
            _update_soko(map, new_player_x, new_player_y);
        }
        break;
    /* can't push any other object */
    default:
        break;
    }

    char *direction = (dx == -1) ? "LEFT" : (dx == 1) ? "RIGHT" : (dy == -1) ? "UP" : "DOWN";
    printf("%s\t[%d key(s), pos: (%d, %d)]\n", direction, map->soko.key_count, map->soko.x, map->soko.y);
}

int check_win(struct level *map)
{
    int i, j, beacon = 0;

    /* iterate through the whole map to see if there is an existing beacon */
    for (i = 0; i < map->width; i++) {
        for (j = 0; j < map->height; j++) {
            if (map->data[j][i] == LEVEL_BEACON) {
                beacon = 1;
                break;
            }
        }
    }

    /* no beacons, but it's a win only if the player is not sitting on a beacon (which will hide the beacon) */
    if (!beacon && !map->soko.player_on_beacon) {
        return 1;
    }

    return 0;
}

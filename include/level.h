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
#pragma once
struct player {
    int x, y, key_count, player_on_beacon;
};

struct level {
    int width, height;
    char **data;
    struct player soko;
};

enum { LEVEL_EMPTY, LEVEL_WALL, LEVEL_TERRAIN, LEVEL_BOX, LEVEL_BEACON, LEVEL_B_BEACON, LEVEL_SOKOBAN, LEVEL_DOOR, LEVEL_KEY };

void free_level(struct level **);
struct level *read_level(char *);

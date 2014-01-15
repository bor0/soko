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
typedef struct level {
    int width, height;
    int player_x, player_y;
    char player_on_beacon;
    char win;
    char **data;
} level;

enum { LEVEL_EMPTY, LEVEL_WALL, LEVEL_TERRAIN, LEVEL_BOX, LEVEL_BEACON, LEVEL_B_BEACON, LEVEL_SOKOBAN };

void free_level(level **);
level *read_level(char *);


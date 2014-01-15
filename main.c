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
#include "soko_sdl.h"

int main(int argc, char **argv) {
    FILE *t;
    level *map;
    int asset_width = 32, asset_height = 32;

    if (argc < 2) {
        printf("Usage: %s <level name>\n", argv[0]);
        return 0;
    }

    map = read_level(argv[1]);

    if (map == NULL) {
        printf("Error: Cannot parse level.\n");
        return 0;
    }

    t = fopen("assets.cfg", "r");

    if (t) {
        fscanf(t, "%d %d", &asset_width, &asset_height);
        fclose(t);
    }

    sokosdl_main(map, asset_width, asset_height);

    free_level(map);

}

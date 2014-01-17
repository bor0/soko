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
#include <string.h>

#include "level.h"
#include "soko_sdl.h"

int main(void) {
    FILE *t;
    struct level *map;
    char level_name[512];
    int asset_width = 32, asset_height = 32, i;

    printf("Soko v1.0 by Boro Sitnikovski\n=============================\nEnter a valid level name (levels/*.dat): ");

    if (fgets(level_name, 512, stdin) == NULL) {
        printf("Enter a valid level, m8.");
        return 0;
    }

    /* remove newlines */
    i = strlen(level_name) - 1;
    while (level_name[i] == '\r' || level_name[i] == '\n') i--;
    level_name[i+1] = '\0';

    /* attempt to parse level */
    map = read_level(level_name);

    if (map == NULL) {
        printf("Error: Cannot parse level.\n");
        return 0;
    }

    /* parse assets */
    t = fopen("assets.cfg", "r");
    if (t) {
        fscanf(t, "%d %d", &asset_width, &asset_height);
        fclose(t);
    }

    /* main SDL loop */
    sokosdl_main(map, asset_width, asset_height);

    /* release level */
    free_level(&map);

    return 1;

}

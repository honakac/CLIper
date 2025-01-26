/*
 * CLIper - Lightweight CLI tool for managing notes in the terminal.
 * Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "include/cliper.h"
#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char *get_ccurent_date(time_t now)
{
    struct tm *local;
    static char date_str[128];
    local = localtime(&now);

    sprintf(date_str, "%04d.%02d.%02d - %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

    return date_str;
}

void cliper_append(cliper_db *db, int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Error: Title is required.\n");
        return;
    }

    cliper_note note;
    memset(&note, 0, sizeof(note));

    note.iat = time(NULL);
    strcpy(note.title, argv[2]);
    if (argc == 4)
        strcpy(note.description, argv[3]);

    db_append(db, &note);
    db_save(db);

    printf("Note appended successfully!\n");
}

void cliper_read_all(cliper_db *db)
{
    for (size_t i = 0; i < db->pos / BLOCK_SIZE; i++) {
        cliper_note note;
        db_read(db, i, &note);

        printf("[%04lu] %s (at %s)\n", i, note.title, get_ccurent_date((time_t) note.iat));
        if (*note.description)
            printf("  - %s\n", note.description);
    }
}

void cliper_remove(cliper_db *db, int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Error: Index is required.\n");
        return;
    }

    size_t index = strtoul(argv[2], NULL, 10);
    db_remove(db, index);
    db_save(db);

    printf("Note with index %lu removed successfully!\n", index);
}
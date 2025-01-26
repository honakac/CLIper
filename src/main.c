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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/cliper.h"
#include "include/data.h"
#include "../config.h"

void usage(char *progname)
{
    fprintf(stderr, "Usage: %s <COMMAND>...\n", progname);
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  append <TITLE> [DESCRIPTION]   Append new note\n");
    fprintf(stderr, "  read                           Read all notes\n");
    fprintf(stderr, "  clear INDEX                    Clear note by index\n\n");
    fprintf(stderr, "  help                           Show this message\n");
    fprintf(stderr, "  version                        Show version\n");
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        usage(argv[0]);
    
    cliper_db db = db_init();

    if (!strcmp(argv[1], "append"))
        cliper_append(&db, argc, argv);
    else if (!strcmp(argv[1], "read"))
        cliper_read_all(&db);
    else if (!strcmp(argv[1], "clear"))
        cliper_remove(&db, argc, argv);
    else if (!strcmp(argv[1], "version")) {
        fprintf(stderr, "CLIper version " CLIPER_VERSION "\n");
        fprintf(stderr, "Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>\n");
        fprintf(stderr, "License: GNU General Public License v3 or later\n");
    }
    else
        usage(argv[0]);

    db_close(&db);
    
    return 0;
}
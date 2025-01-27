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
#include "include/cliper.h"
#include "include/data.h"

#define CLIPER_VERSION "v1.1.1"

void usage(char *progname)
{
    fprintf(stderr, "Usage: %s <COMMAND>...\n", progname);
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  append <TITLE> [DESCRIPTION]   Append new note\n");
    fprintf(stderr, "  search KEYWORD                 Search for notes by keyword\n");
    fprintf(stderr, "  clear INDEX                    Clear note by index\n");
    fprintf(stderr, "  read                           Read all notes\n\n");
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
    else if (!strcmp(argv[1], "clear"))
        cliper_remove(&db, argc, argv);
    else if (!strcmp(argv[1], "search"))
        cliper_search(&db, argc, argv);
    else if (!strcmp(argv[1], "read"))
        cliper_read_all(&db);
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
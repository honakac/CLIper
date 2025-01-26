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
#include <getopt.h>
#include "include/cliper.h"
#include "../config.h"

typedef enum {
    CM_NONE,
    CM_APPEND,
    CM_WRITE,
    CM_READ_ALL,
    CM_CLEAR_ALL,
    CM_CLEAR_LINE
} CLIPER_MODES;

void usage(char *progname)
{
    fprintf(stderr, "Usage: %s [OPTIONS]...\n", progname);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -t TITLE        Set title\n");
    fprintf(stderr, "  -d DESCRIPTION  Set description\n");
    fprintf(stderr, "  -h              Show this message and exit\n");
    fprintf(stderr, "  -v              Print version information and exit\n");

    fprintf(stderr, "Options(Modes):\n");
    fprintf(stderr, "  -a              Set append mode\n");
    fprintf(stderr, "  -w              Set write mode (clear file and write)\n");
    fprintf(stderr, "  -r              Set read mode (read all notes)\n");
    fprintf(stderr, "  -c              Set Clear mode (clear file)\n");
    fprintf(stderr, "  -C INDEX        Set Clear line mode (clear line by index)\n");
    exit(1);
}

int main(int argc, char **argv)
{
    int opt;
    struct {
        char *title;
        char *description;
        size_t index;
        CLIPER_MODES mode;
    } options = {NULL, NULL, -1, CM_NONE};

    while ((opt = getopt(argc, argv, "acwrhvt:d:C:")) != -1) {
        switch (opt) {
            case 'a': options.mode = CM_APPEND; break;
            case 'w': options.mode = CM_WRITE; break;
            case 'r': options.mode = CM_READ_ALL; break;
            case 'c': options.mode = CM_CLEAR_ALL; break;
            case 'C': options.mode = CM_CLEAR_LINE; options.index = (size_t) atoll(optarg); break;

            case 't': options.title = optarg; break;
            case 'd': options.description = optarg; break;

            case 'v':
                fprintf(stderr, "CLIper version " CLIPER_VERSION "\n");
                fprintf(stderr, "Copyright (C) 2025 Andrey Stekolnikov <honakac@yandex.ru>\n");
                fprintf(stderr, "License: GNU General Public License v3 or later\n");
                return 0;
            case 'h':
            default:
                usage(argv[0]);
        }
    }

    switch (options.mode) {
        case CM_APPEND:
        case CM_WRITE:
            cliper_write(options.mode == CM_WRITE, options.title, options.description);
            break;
        
        case CM_CLEAR_ALL:
            fclose(fopen(TEMP_FILE, "w"));
            break;

        case CM_CLEAR_LINE:
            cliper_clear_line(options.index);
            break;

        case CM_READ_ALL:
            cliper_read_all();
            break;

        default:
            fprintf(stderr, "Error: Invalid mode\n");
            usage(argv[0]);
    }

    return 0;
}
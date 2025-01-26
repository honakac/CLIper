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

#define PAGE_SIZE 512

static char *get_ccurent_date()
{
    time_t now;
    struct tm *local;
    static char date_str[128];
    now = time(NULL);
    local = localtime(&now);

    sprintf(date_str, "%04d.%02d.%02d - %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

    return date_str;
}

static char *cliper_read_data(FILE *fp)
{
    struct {
        char *buffer;
        size_t pos;
        size_t size;
    } notes = {malloc(PAGE_SIZE), 0, PAGE_SIZE};
    if (!notes.buffer) perror("malloc");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (notes.pos + 1 >= notes.size) {
            notes.buffer = realloc(notes.buffer, (notes.size += PAGE_SIZE));

            if (!notes.buffer) perror("realloc");
        }

        notes.buffer[notes.pos++] = ch;
        notes.buffer[notes.pos] = 0;
    }

    return notes.buffer;
}
static char **cliper_read_lines(FILE *fp)
{
    char *buffer = cliper_read_data(fp);
    struct {
        char **buffer;
        size_t pos;
        size_t size;
    } lines = {malloc(PAGE_SIZE * sizeof(char*)), 0, PAGE_SIZE};
    if (!lines.buffer) perror("malloc");

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        if (lines.pos + 1 >= lines.size) {
            lines.buffer = realloc(lines.buffer, (lines.size += PAGE_SIZE) * sizeof(char*));

            if (!lines.buffer) perror("realloc");
        }

        lines.buffer[lines.pos++] = line;
        lines.buffer[lines.pos] = NULL;
        line = strtok(NULL, "\n");
    }

    return lines.buffer;
}

static FILE *cliper_open_file(char *mode)
{
    FILE *file = fopen(TEMP_FILE, mode);
    if (!file) perror("fopen");

    return file;
}

void cliper_write(char overwrite, const char *title, const char *description)
{
    if (title == NULL) {
        fprintf(stderr, "Error: Title cannot be NULL\n");
        return;
    }

    FILE *fp = cliper_open_file(overwrite ? "w" : "a");
    
    if (description == NULL)
        fprintf(fp, "[%s]: %s\n", get_ccurent_date(), title);
    else
        fprintf(fp, "[%s]: %s - %s\n", get_ccurent_date(), title, description);

    fclose(fp);
}

void cliper_read_all()
{
    FILE *fp = cliper_open_file("r");

    char **lines = cliper_read_lines(fp);

    size_t countLine = 1;
    for (char **ptr = lines; *ptr; ptr++)
        fprintf(stdout, "%04zu: %s\n", countLine++, *ptr);

    free(lines);
    fclose(fp);
}

void cliper_clear_line(long long index)
{
    if (index <= 0) {
        fprintf(stderr, "Error: Index cannot be null\n");
        return;
    }

    FILE *fp = cliper_open_file("r");
    char **lines = cliper_read_lines(fp);

    fclose(fp);
    fp = cliper_open_file("w");

    size_t countLine = 1;
    char isCleared = 0;
    for (char **ptr = lines; *ptr; ptr++, countLine++) {
        if (countLine != index)
            fprintf(fp, "%s\n", *ptr);
        else
            isCleared = 1;
    }

    free(lines);
    fclose(fp);

    if (!isCleared) {
        fprintf(stderr, "Error: Line with index %llu not found\n", index);
        exit(1);
    } else {
        fprintf(stdout, "Line with index %llu cleared\n", index);
    }
}
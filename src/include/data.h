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

#pragma once

#include <stdio.h>
#include <stddef.h>

#define EXP_SIZE (sizeof(size_t))
#define IAT_SIZE (sizeof(size_t))
#define TITLE_SIZE 128
#define DESCRIPTION_SIZE 128

#define BLOCK_SIZE (IAT_SIZE+EXP_SIZE+TITLE_SIZE+DESCRIPTION_SIZE)

#define DB_RELATIVE_PATH "/.config/cliper_db.bin"

#define MEMORY_BLOCK_SIZE (BLOCK_SIZE*5)

typedef struct {
    size_t iat;
    size_t exp;
    char title[TITLE_SIZE];
    char description[DESCRIPTION_SIZE];
} cliper_note;

typedef struct {
    FILE *fp;
    char path[1024];

    void *buffer;
    size_t pos;
    size_t size;
} cliper_db;

void db_load(cliper_db *db);
cliper_db db_init();
void db_close(cliper_db *db);
void db_append(cliper_db *db, cliper_note *note);
void db_remove(cliper_db *db, size_t index);
void db_read(cliper_db *db, size_t index, cliper_note *note);
void db_save(cliper_db *db);
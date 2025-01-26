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
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include "include/data.h"

void db_check_realloc(cliper_db *db)
{
    if (db->pos + MEMORY_BLOCK_SIZE >= db->size) {
        db->buffer = realloc(db->buffer, (db->size += MEMORY_BLOCK_SIZE));

        if (!db->buffer) {
            fprintf(stderr, "Error: allocation memory failed\n");
            exit(1);
        }
    }
}

char db_validate_note(cliper_note *note)
{
    if (*note->title == 0 || note->iat == 0)
        return 1;
    return 0;
}

void db_load(cliper_db *db)
{
    fseek(db->fp, 0, SEEK_END);

    size_t size = ftell(db->fp);
    fseek(db->fp, 0, SEEK_SET);

    if (size % BLOCK_SIZE != 0) {
        fprintf(stderr, "Error: database file size is not a multiple of block size\n");
        exit(1);
    }

    for (size_t i = 0; i < size; i += BLOCK_SIZE) {
        db_check_realloc(db);
        fread(db->buffer + db->pos, 1, BLOCK_SIZE, db->fp);

        cliper_note note = *((cliper_note*) (db->buffer + db->pos));
        if (db_validate_note(&note)) {
            fprintf(stderr, "Error: database file is corrupted\n");
            exit(1);
        }

        db->pos += BLOCK_SIZE;
    }
}

cliper_db db_init()
{
    struct passwd *pw = getpwuid(getuid());

    char datadir[1024];
    memset(datadir, 0, sizeof(datadir));
    strcpy(datadir, pw->pw_dir);
    strcat(datadir, DB_RELATIVE_PATH);

    cliper_db db = {fopen(datadir, "a+"), {}, malloc(MEMORY_BLOCK_SIZE), 0, MEMORY_BLOCK_SIZE};
    memset(db.buffer, 0, MEMORY_BLOCK_SIZE);
    memcpy(db.path, datadir, sizeof(db.path));

    if (!db.fp)     perror("Error opening database file");
    if (!db.buffer) perror("Error allocating memory for database buffer");

    db_load(&db);

    return db;
}

void db_append(cliper_db *db, cliper_note *note)
{
    db_check_realloc(db);

    if (db_validate_note(note)) {
        fprintf(stderr, "Error: invalid note data\n");
        return;
    }

    memcpy(db->buffer + db->pos, note, BLOCK_SIZE);
    db->pos += BLOCK_SIZE;
}

void db_save(cliper_db *db)
{
    fclose(db->fp);
    db->fp = fopen(db->path, "w+");

    for (size_t i = 0; i < db->pos; i += BLOCK_SIZE) {
        cliper_note *note = db->buffer + i;

        if (*note->title != 0)
            fwrite(db->buffer + i, 1, BLOCK_SIZE, db->fp);
    }
    
    fseek(db->fp, 0, SEEK_END);
}

void db_read(cliper_db *db, size_t index, cliper_note *note)
{
    if (index * BLOCK_SIZE >= db->pos) {
        fprintf(stderr, "Error: index out of range\n");
        return;
    }

    memcpy(note, db->buffer + index * BLOCK_SIZE, BLOCK_SIZE);

    if (db_validate_note(note)) {
        fprintf(stderr, "Error: note data is corrupted\n");
        exit(1);
    }
}

void db_remove(cliper_db *db, size_t index)
{
    if (index * BLOCK_SIZE >= db->pos) {
        fprintf(stderr, "Error: index out of range\n");
        return;
    }

    cliper_note *note = db->buffer + index * BLOCK_SIZE;
    memset(note->title, 0, TITLE_SIZE);
}

void db_close(cliper_db *db)
{
    if (db->fp) fclose(db->fp);
    if (db->buffer) free(db->buffer);

    db->pos = 0;
    db->size = 0;
}
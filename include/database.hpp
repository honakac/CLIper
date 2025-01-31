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
#include <fstream>
#include <string>
#include <vector>

typedef struct {
  size_t timestamp;
  char tags[128];
  char title[128];
  char content[1024];
} Note;

typedef std::vector<Note> NoteList;

class Database {
private:
  std::string path;
  NoteList notes;

public:
  void openDB(const std::string &path);
  void readDB();
  void writeDB();
  void closeDB();

  void addNote(Note &note);
  NoteList &getNotes() {return notes;}
};

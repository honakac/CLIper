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

#include "include/database.hpp"
#include <filesystem>
#include <iostream>

static void touchFile(const std::string &path) {
  std::ofstream file(path);
  file.close();
}

void Database::openDB(const std::string &path) {
  if (!std::filesystem::exists(path))
    touchFile(path);

  this->path = path;
  notes.clear();
}

void Database::readDB() {
  std::ifstream file(path, std::fstream::binary);

  Note note;
  while (file.read((char *)&note, sizeof(note)))
    notes.push_back(note);

  file.close();
}

void Database::writeDB() {
  std::ofstream file(path, std::fstream::binary);

  for (const auto &note : notes)
    file.write((char *)&note, sizeof(note));

  file.close();
}

void Database::closeDB() {
  notes.clear();
  path.clear();
}

void Database::addNote(Note &note) {
  notes.push_back(note);
  writeDB();
}

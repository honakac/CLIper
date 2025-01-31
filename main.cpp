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

#include "include/cliper.hpp"

int main(int argc, char **argv) {
  struct {
    std::string db_path="cliper_global_notes.bin";
  } options;
  Cliper cliper;

  cliper.db.openDB(GET_CONFIG_DIR() + options.db_path);
  cliper.db.readDB();

  cliper.runCommand(argc, argv);

  return 0;
}

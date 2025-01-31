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
#include "database.hpp"
#define CLIPER_VERSION "v1.2"

#ifdef __linux__
#define GET_CONFIG_DIR() (std::string(getenv("HOME")) + "/.config/")
#else
#error "Unsupported platform. Please use Linux for building CLIper."
#endif

class Cliper {
public:
  Database db;

  void commandAppend(int argc, char **argv);
  void commandList(int argc, char **argv);
  void commandRemove(int argc, char **argv);
  void commandSearch(int argc, char **argv);

  void runCommand(int argc, char **argv);
};

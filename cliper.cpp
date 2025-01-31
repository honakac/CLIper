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

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <getopt.h>
#include "include/cliper.hpp"
#include "include/ansi_colors.h"

static char *get_ccurent_date(time_t now)
{
  struct tm *local;
  static char date_str[128];
  local = localtime(&now);

  sprintf(date_str, "%04d.%02d.%02d - %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

  return date_str;
}
static void cliper_print_note(size_t index, Note &note)
{
  printf("[" YEL "%03lu" CRESET "] " GRN "%s" CRESET " (" CYN "%s" CRESET ")\n", index, note.title, get_ccurent_date((time_t) note.timestamp));
  if (note.tags[0])
    printf("  /" BLU "%s" CRESET "/\n", note.tags);
  if (note.content[0])
    printf("  - %s\n", note.content);
}

static void usage(const char *program) {
  fprintf(stderr, "Usage: %s COMMAND [OPTIONS]...\n", program);
  fprintf(stderr, "Commands:\n");

  fprintf(stderr, "\tlist                                    Show list of notes\n");
  fprintf(stderr, "\tremove  -i INDEX                        Remove a note by index\n");
  fprintf(stderr, "\tsearch  -k KEYWORD                      Search notes by keyword\n");
  fprintf(stderr, "\tappend  -t TITLE [-c CONTENT] [-T TAG]  Append new note\n\n");

  fprintf(stderr, "\thelp                                    Show this message\n");
  fprintf(stderr, "\tversion                                 Show version\n");
  std::exit(0);
}

inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}
inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}
static std::string trim(std::string s) {
  ltrim(s);
  rtrim(s);
  return s;
}

static std::string clear_tags(std::string srctags) {
  srctags += ',';
  std::vector<std::string> tags{};
  std::string out{};
  std::string temp_tag{};

  for (auto iter = srctags.begin(); iter != srctags.end(); iter++) {
    if (*iter == ' ')
      continue;
    if (*iter == ',') {
      std::string s = trim(temp_tag);
      if (!s.empty())
        tags.push_back(s);
      temp_tag.clear();
    }
    else
      temp_tag += *iter;
  }

  for (auto &elem : tags)
    out += elem + ",";
  if (tags.size() != 0)
    out.erase(out.end()-1); // remove ','

  return out;
}

void Cliper::runCommand(int argc, char **argv) {
  if (argc < 2)
    usage(argv[0]);

  if (strcmp(argv[1], "append") == 0)
    commandAppend(argc, argv);
  else if (strcmp(argv[1], "list") == 0)
    commandList(argc, argv);
  else if (strcmp(argv[1], "remove") == 0)
    commandRemove(argc, argv);
  else if (strcmp(argv[1], "search") == 0)
    commandSearch(argc, argv);
  else
    usage(argv[0]);
}

void Cliper::commandSearch(int argc, char **argv) {
  struct {
    char *keyword;
  } options;

  int opt;
  while ((opt = getopt(argc, argv, "k:")) != -1) {
    switch (opt) {
      case 'k':
        options.keyword = optarg;
        break;
      default:
        usage(argv[0]);
    }
  }

  if (strlen(options.keyword) == 0) {
    fprintf(stderr, "Keyword is required\n");
    usage(argv[0]);
  }

  size_t index = 0;
  for (auto &elem : db.getNotes()) {
    char data[sizeof(elem.content)+sizeof(elem.tags)+sizeof(elem.title)+1];
    strcpy(data, elem.content);
    strcat(data, elem.tags);
    strcat(data, elem.title);

    if (strstr(data, options.keyword) != NULL)
      cliper_print_note(index, elem);

    index++;
  }
}

void Cliper::commandRemove(int argc, char **argv) {
  struct {
    long index = -1;
  } options;

  int opt;
  while ((opt = getopt(argc, argv, "i:")) != -1) {
    switch (opt) {
      case 'i':
        options.index = atol(optarg);
        break;
      default:
        usage(argv[0]);
    }
  }

  auto &notes = db.getNotes();

  if (options.index < 0) {
    fprintf(stderr, "Index is required\n");
    usage(argv[0]);
  }
  if (options.index >= notes.size())
    throw std::runtime_error("Index out of range");

  notes.erase(notes.begin() + options.index);
  db.writeDB();
}

void Cliper::commandList(int argc, char **argv) {
  size_t index = 0;
  for (auto &elem : db.getNotes()) {
    cliper_print_note(index++, elem);
  }
}

void Cliper::commandAppend(int argc, char **argv) {
  Note note = {
    .timestamp = (size_t) time(NULL),
  };
  memset(note.title, 0, sizeof(note.title));
  memset(note.content, 0, sizeof(note.content));
  memset(note.tags, 0, sizeof(note.tags));
  std::string temp_tags{};

  int opt;
  while ((opt = getopt(argc, argv, "t:c:T:")) != -1) {
    switch (opt) {
      case 't':
        if (strlen(optarg) < sizeof(note.title))
          strcpy(note.title, optarg);
        else
          throw std::runtime_error("Title too long");
        break;
      case 'c':
        if (strlen(optarg) < sizeof(note.content))
          strcpy(note.content, optarg);
        else
          throw std::runtime_error("Content too long");
        break;
      case 'T':
        if (strlen(optarg) < sizeof(note.tags))
          temp_tags += std::string(optarg) + ',';
        else
          throw std::runtime_error("Tags too long");
        break;
      default:
        usage(argv[0]);
    }
  }

  if (strlen(note.title) == 0) {
    fprintf(stderr, "Title is required\n");
    usage(argv[0]);
  }

  temp_tags = clear_tags(temp_tags);
  if (temp_tags.size() > sizeof(note.tags))
    throw std::runtime_error("Tags too long");

  strcpy(note.tags, temp_tags.c_str());

  db.addNote(note);
  db.writeDB();
}

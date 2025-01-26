<hr>
<div align="center">
  <img src="https://www.aschey.tech/tokei/github/honakac/CLIper">
  <img src="https://img.shields.io/github/languages/code-size/honakac/CLIper.svg">
</div>
<hr>
<br>

# CLIper
CLIper - Lightweight CLI tool for managing notes in the terminal.<br>
CLIper is a simple command-line tool that allows you to manage notes directly from your terminal. It provides basic functionality for creating, reading, and clearing notes.

# Compilation:
To compile CLIper, you need a C compiler. Use the following command:

```
make
```

## Examples:

```bash
./cliper append "Meeting" "Discuss project timeline" # Append note
./cliper read                                        # Show list notes
./cliper clear 1                                     # Remove first element
```

> [!INFO]
> Make sure you have the necessary permissions to read and write files in the directory `/tmp/`.

<br>
<hr>
<br>

CLIper is distributed under the GNU General Public License v3 or later. For more details, see the LICENSE file or visit https://www.gnu.org/licenses/.

For bug reports or feature requests, please contact Andrey Stekolnikov at **honakac@yandex.ru**.
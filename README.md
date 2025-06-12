# ft_nm

`ft_nm` is a reimplementation of the Unix `nm` command in C. It displays the symbol table of ELF binaries (executables, object files, and shared libraries) for both 32-bit and 64-bit architectures. This tool is useful for inspecting compiled binaries, debugging, and reverse engineering.

---

## Features

- **ELF Support:** Handles both 32-bit and 64-bit ELF files.
- **Symbol Table Display:** Shows symbol names, values, and types, mimicking the output of the system `nm`.
- **Flag Support:** Implements common `nm` flags:
  - `-a` : Display all symbol table entries, including debugger-only symbols.
  - `-g` : Display only external (global) symbols.
  - `-u` : Display only undefined symbols.
  - `-r` : Reverse the sort order.
  - `-p` : Do not sort; display in symbol-table order.
- **Multiple Files:** Accepts multiple files as arguments, displaying results for each.
- **Error Handling:** Gracefully handles invalid files, wrong architectures, and truncated binaries.

---

## Project Structure

```
ft_nm/
├── Makefile
├── README.md
├── test.sh
├── header/
│   └── ft_nm.h
├── srcs/
│   ├── arg_check.c
│   ├── get_symbol_type.c
│   ├── lst.c
│   ├── main.c
│   ├── parse_table_32b.c
│   ├── parse_table_64b.c
│   ├── print_nm.c
│   ├── sort.c
│   └── test_function.c
├── tests_binaries/
│   ├── 32bitnm
│   ├── easy_test
│   ├── error_header
│   ├── header
│   ├── header_and_prog
│   ├── header_and_prog_copy
│   ├── header_copy
│   ├── header_offset_error
│   ├── libasm.so
│   ├── main.o
│   ├── minishell
│   ├── not_so_easy_test
│   ├── philo
│   ├── unterminated_string
│   └── wrong_arch
```

---

## Building

To build the project, simply run:

```sh
make
```

This will compile the `ft_nm` executable.

To clean object files:

```sh
make clean
```

To clean all build artifacts and the executable:

```sh
make fclean
```

To rebuild from scratch:

```sh
make re
```

---

## Usage

```sh
./ft_nm [options] <file1> [file2 ...]
```

**Examples:**

```sh
./ft_nm ./tests_binaries/easy_test
./ft_nm -g -u ./tests_binaries/main.o
./ft_nm -r -p ./tests_binaries/32bitnm ./tests_binaries/philo
```

---

## Testing

A set of test binaries is provided in the `tests_binaries/` directory. You can use `test.sh` to run automated tests (if implemented).

---

## Implementation Notes

- The code is modular, with separate files for argument parsing, ELF parsing, symbol sorting, and output formatting.
- All ELF parsing is done manually, without relying on external libraries.
- The project is written in standard C and should compile on any Unix-like system with GCC.

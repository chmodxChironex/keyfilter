# Keyfilter

A C program for address filtering and character suggestions based on a given prefix.  
Originally developed as part of a university assignment.
## Features

- Reads up to 42 addresses from standard input.
- Case-insensitive matching.
- If one address matches exactly → outputs `Found: <ADDRESS>`.
- If multiple match → suggests next valid characters as `Enable: <CHARS>`.
- If no match → prints `Not found`.

## Build & Run (Linux / WSL only)

```bash
make
./keyfilter <PREFIX> < input.txt  # run manually
./test_keyfilter.sh               # or run tests automatically


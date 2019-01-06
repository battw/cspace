# -ggdb flag produces debugging info for the  gdb
clang -ggdb -lncurses -Wall -Wextra -std=c11 scratch.c -o scratch &&
./scratch

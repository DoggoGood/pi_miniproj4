#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO: add edge genertion
// FIX: marked might not work

void print_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    for (int i = 0; i < (2 * n) + 1; i++) {
        for (int j = 0; j < (2 * n) + 1; j++) {
            printf("%d ", t[i][j]);
        }
        printf("\n");
    }
}

void dfs_gen(int n, int t[(2 * n) + 1][(2 * n) + 1], int x, int y, int prev_x,
             int prev_y, int start_x, int end_x) {
    int (*marked)[(2 * n) + 1] = calloc((2 * n) + 1, sizeof *marked);
    int how_many_divisions = 0;
    int how_many_blocked = 0;
    int start_y = 0;
    int end_y = 2 * n + 1;
    if (t[x + 1][y] == 1 || x + 1 == end_y ||
        (x + 1 == prev_x && y == prev_y)) {
        how_many_blocked++;
    }
    if (t[x - 1][y] == 1 || x - 1 == start_y ||
        (x + 1 == prev_x && y == prev_y)) {
        how_many_blocked++;
    }
    if (t[x][y - 1] == 1 || (x + 1 == prev_x && y == prev_y)) {
        how_many_blocked++;
    }
    if (t[x][y + 1] == 1 || (x + 1 == prev_x && y == prev_y)) {
        how_many_blocked++;
    }
    how_many_divisions = (rand() % (4 - how_many_blocked)) + 1;
    if (how_many_divisions == 3) {
        if (x + 1 != prev_x && y != prev_y && !marked[x + 1][y] &&
            t[x + 1][y] != 1) {
            marked[x + 1][y] = 1;
            dfs_gen(n, t, x + 1, y, x, y, start_x, end_x);
        }
        if (x - 1 != prev_x && y != prev_y && !marked[x - 1][y] &&
            t[x - 1][y] != 1) {
            marked[x - 1][y] = 1;
            dfs_gen(n, t, x - 1, y, x, y, start_x, end_x);
        }
        if (x != prev_x && y + 1 != prev_y && !marked[x][y + 1] &&
            t[x][y + 1] != 1) {
            marked[x][y + 1] = 1;
            dfs_gen(n, t, x, y + 1, x, y, start_x, end_x);
        }
        if (x != prev_x && y - 1 != prev_y && !marked[x][y - 1] &&
            t[x][y - 1] != 1) {
            marked[x][y - 1] = 1;
            dfs_gen(n, t, x, y - 1, x, y, start_x, end_x);
        }
    } else {
        // 1 == up
        // 2 == left
        // 3 == right
        // 4 == down
        while (how_many_divisions--) {
            int direction = (rand() % 4) + 1;
            if (direction == 1) {
            }
        }
    }
}

void generate_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    int start_x = rand() % n;
    int end_x = rand() % n;
    for (int i = 0; i < (2 * n) + 1; i++) {
        if (i == start_x * 2 + 1) {
            t[0][i] = 0;
        } else {
            t[0][i] = 1;
        }
        if (i == end_x * 2 + 1) {
            t[(2 * n)][i] = 0;
        } else {
            t[(2 * n)][i] = 1;
        }
        t[i][0] = 1;
        t[i][(2 * n)] = 1;
    }
    for (int i = 1; i < (2 * n); i++) {
        for (int j = 1; j < (2 * n); j++) {
            if (i % 2 == 0 && j % 2 == 0) {
                t[i][j] = 1;
            }
        }
    }
}

int main() {
    srand(time(NULL));
    int n;
    printf("Proszę podać rozmiar labiryntu: ");
    scanf("%d", &n);
    int size = (2 * n) + 1;
    int (*t)[size] = calloc(size, sizeof *t);
    generate_labyrynth(n, t);
    print_labyrynth(n, t);
    free(t);
}

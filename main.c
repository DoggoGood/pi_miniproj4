#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    for (int i = 0; i < (2 * n) + 1; i++) {
        for (int j = 0; j < (2 * n) + 1; j++) {
            printf("%d ", t[i][j]);
        }
        printf("\n");
    }
}

void dfs_gen(int n, int t[(2 * n) + 1][(2 * n) + 1],
             int visited[(2 * n) + 1][(2 * n) + 1], int x, int y, int start_x,
             int end_x) {
    int directions[4][2] = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};
    visited[y][x] = 1;
    int random_index = rand() % 4;
    for (int i = 0; i < 4; i++) {
        int dir_x = directions[(random_index + i) % 4][0];
        int dir_y = directions[(random_index + i) % 4][1];
        int new_x = x + dir_x;
        int new_y = y + dir_y;
        if (new_x > 0 && new_x < (2 * n) && new_y > 0 && new_y < (2 * n) &&
            visited[new_y][new_x] == 0) {
            t[y + dir_y / 2][x + dir_x / 2] = 0;
            dfs_gen(n, t, visited, new_x, new_y, start_x, end_x);
        }
    }
}

void generate_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    int start_x = rand() % n;
    int end_x = rand() % n;
    int size = (2 * n) + 1;
    for (int i = 0; i < size; i++) {
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
            if (i % 2 == 0 || j % 2 == 0) {
                t[i][j] = 1;
            }
        }
    }
    int (*visited)[size] = calloc(size, sizeof(int[size]));
    dfs_gen(n, t, visited, (2 * start_x) + 1, 1, (2 * start_x) + 1,
            (2 * end_x) + 1);
    free(visited);
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

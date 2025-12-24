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

void generate_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    int start_x = rand() % n;
    int start_y = 0;
    int end_x = rand() % n;
    int end_y = n - 1;
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

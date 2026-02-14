#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE 3
#define WIN 21

/* --- Draw field --- */
void Draw(int ball_x, int ball_y,
          int left_p, int right_p,
          int left_s, int right_s) {
    int x = 0, y = 0;

    printf("Score: %d - %d\n", left_s, right_s);

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (x == ball_x && y == ball_y) {
                printf("O");
            } else if (x == 2 && (y == left_p || y == left_p + 1 || y == left_p + 2)) {
                printf("|");
            } else if (x == WIDTH - 3 && (y == right_p || y == right_p + 1 || y == right_p + 2)) {
                printf("|");
            } else if (x == WIDTH / 2) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\nA/Z - Left paddle | K/M - Right paddle | Space - Skip turn\n");
}

/* --- Handle input --- */
void HandleInput(char input, int *left_p, int *right_p) {
    if ((input == 'a' || input == 'A') && *left_p > 0) {
        (*left_p)--;
    }
    if ((input == 'z' || input == 'Z') && *left_p < HEIGHT - PADDLE) {
        (*left_p)++;
    }
    if ((input == 'k' || input == 'K') && *right_p > 0) {
        (*right_p)--;
    }
    if ((input == 'm' || input == 'M') && *right_p < HEIGHT - PADDLE) {
        (*right_p)++;
    }
}

/* --- Update ball --- */
void UpdateBall(int *ball_x, int *ball_y,
                int *dx, int *dy,
                int left_p, int right_p,
                int *left_s, int *right_s) {
    *ball_x += *dx;
    *ball_y += *dy;

    if (*ball_y <= 0 || *ball_y >= HEIGHT - 1) {
        *dy = -*dy;
    }

    if (*ball_x == 3 && (*ball_y == left_p || *ball_y == left_p + 1 || *ball_y == left_p + 2)) {
        *dx = -*dx;
    }
    if (*ball_x == WIDTH - 4 &&
        (*ball_y == right_p || *ball_y == right_p + 1 || *ball_y == right_p + 2)) {
        *dx = -*dx;
    }

    if (*ball_x <= 0) {
        (*right_s)++;
        *ball_x = WIDTH / 2;
        *ball_y = HEIGHT / 2;
        *dx = -*dx;
    }

    if (*ball_x >= WIDTH - 1) {
        (*left_s)++;
        *ball_x = WIDTH / 2;
        *ball_y = HEIGHT / 2;
        *dx = -*dx;
    }
}

/* --- Check winner --- */
int CheckWinner(int left_s, int right_s) {
    if (left_s >= WIN) return 1;
    if (right_s >= WIN) return 2;
    return 0;
}

/* --- Main --- */
int main(void) {
    int ball_x = WIDTH / 2;
    int ball_y = HEIGHT / 2;
    int dx = 1, dy = 1;
    int left_p = HEIGHT / 2 - 1;
    int right_p = HEIGHT / 2 - 1;
    int left_s = 0, right_s = 0;
    char input = 0;
    int winner = 0;

    while (!winner) {
        Draw(ball_x, ball_y, left_p, right_p, left_s, right_s);

        if (scanf(" %c", &input) != 1) return 0;

        HandleInput(input, &left_p, &right_p);
        UpdateBall(&ball_x, &ball_y, &dx, &dy, left_p, right_p, &left_s, &right_s);
        winner = CheckWinner(left_s, right_s);
    }

    Draw(ball_x, ball_y, left_p, right_p, left_s, right_s);

    if (winner == 1) {
        printf("Left player wins\n");
    } else {
        printf("Right player wins\n");
    }

    return 0;
}
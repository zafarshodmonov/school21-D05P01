#include <ncurses.h>
#include <unistd.h>  /* usleep */

#define WIDTH 80
#define HEIGHT 25
#define PADDLE 3
#define WIN 21
#define DELAY 100000 /* 100ms */

void Draw(int ball_x, int ball_y,
          int left_p, int right_p,
          int left_s, int right_s) {
    clear();
    mvprintw(0, 0, "Score: %d - %d", left_s, right_s);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == ball_x && y == ball_y) {
                mvaddch(y + 1, x, 'O');
            } else if (x == 2 && (y >= left_p && y <= left_p + 2)) {
                mvaddch(y + 1, x, '|');
            } else if (x == WIDTH - 3 && (y >= right_p && y <= right_p + 2)) {
                mvaddch(y + 1, x, '|');
            } else if (x == WIDTH / 2) {
                mvaddch(y + 1, x, '|');
            }
        }
    }

    mvprintw(HEIGHT + 2, 0, "A/Z - Left | K/M - Right | Q - Quit");
    refresh();
}

void HandleInput(int ch, int *left_p, int *right_p) {
    if ((ch == 'a' || ch == 'A') && *left_p > 0) (*left_p)--;
    if ((ch == 'z' || ch == 'Z') && *left_p < HEIGHT - PADDLE) (*left_p)++;
    if ((ch == 'k' || ch == 'K') && *right_p > 0) (*right_p)--;
    if ((ch == 'm' || ch == 'M') && *right_p < HEIGHT - PADDLE) (*right_p)++;
}

void UpdateBall(int *ball_x, int *ball_y,
                int *dx, int *dy,
                int left_p, int right_p,
                int *left_s, int *right_s) {

    *ball_x += *dx;
    *ball_y += *dy;

    if (*ball_y <= 0 || *ball_y >= HEIGHT - 1) *dy = -*dy;

    if (*ball_x == 3 && (*ball_y >= left_p && *ball_y <= left_p + 2)) *dx = -*dx;
    if (*ball_x == WIDTH - 4 && (*ball_y >= right_p && *ball_y <= right_p + 2)) *dx = -*dx;

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

int CheckWinner(int left_s, int right_s) {
    if (left_s >= WIN) return 1;
    if (right_s >= WIN) return 2;
    return 0;
}

int main(void) {
    int ball_x = WIDTH / 2, ball_y = HEIGHT / 2;
    int dx = 1, dy = 1;
    int left_p = HEIGHT / 2 - 1, right_p = HEIGHT / 2 - 1;
    int left_s = 0, right_s = 0;
    int winner = 0;

    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);  /* Non-blocking input */
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    while (!winner) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        HandleInput(ch, &left_p, &right_p);
        UpdateBall(&ball_x, &ball_y, &dx, &dy, left_p, right_p, &left_s, &right_s);
        winner = CheckWinner(left_s, right_s);

        Draw(ball_x, ball_y, left_p, right_p, left_s, right_s);

        usleep(DELAY);
    }

    clear();
    if (winner == 1) {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 7, "Left player wins!");
    } else if (winner == 2) {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 8, "Right player wins!");
    } else {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "Game quit.");
    }
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    endwin();

    return 0;
}
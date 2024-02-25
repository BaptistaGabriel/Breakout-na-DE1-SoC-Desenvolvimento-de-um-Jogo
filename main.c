#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>
#include <intelfpgaup/accel.h>

#define RACKET_WIDTH 44
#define BORDER_WEIGHT 5
#define TOP_MARGIN 40
#define RACKET_HEIGHT 3
#define video_BLACK 0x00
#define BALL_SIZE 1
#define BALL_VEL 1
#define RACKET_VEL 6
#define WIDTH 319
#define HEIGHT 239
#define QUANT_BRICKS 30

#define video_YELLOW 0xFFE0
#define video_RED 0xF800
#define video_GREEN 0x07E0
#define video_BLUE 0x041F

int screen_x, screen_y;
int char_x, char_y;

int a = 1;
int b = 2;
int c;
int d = 0;
float rate = 0.78;
int e;
int ptr_x;
int ptr_y;
int ptr_z;
int ptr_ready;
int ptr_tap;
int ptr_dtap;
int ptr_msg;

// X1 X2 Y1 Y2 color visible

typedef struct
{
    int x1;
    int x2;
    int y1;
    int y2;
    short color;
    int visible;
} Brick;

void print_text(int, int, int, int, unsigned);
void renderBrick(Brick);
Brick createBrick(int, int, int, int, short, int);
void fillBricks(int, int, int, Brick *, short);
void renderBricks(Brick *, int);
void verifyBlockCollision(Brick *, int, int, int *, int *);
int verifyQuantBlocks(Brick *);
void showStartScreen();
void showLoseScreen();
void showWinScreen();
void renderScenario(int, int, int, short, short);
void racketCollision(int *, int, int, int);
void ballRacketCollision(int, int, int, int, int *, int *, int *);
void ballBorderCollision(int, int, int *, int *, int *);
void showLifeSquare(int);
void renderLifes(int);

volatile sig_atomic_t stop;

void catchSIGINT(int signum)
{
    stop = 1;
}

/* This program draws randomly-generated lines on the VGA display. The code uses
 * the character device drivers IntelFPGAUP/video and IntelFPGAUP/KEY. It draws a
 * random line each time a pushbutton KEY is pressed. The coordinates of each new
 * line are displayed on the bottom right of the screen. Exit by typing ^C. */

int main(int argc, char *argv[])
{

    time_t t; // used to seed the rand() function

    // catch SIGINT from ^C, instead of having it abruptly close this program
    signal(SIGINT, catchSIGINT);
    srand((unsigned)time(&t)); // seed the rand function

    // Open the character device drivers
    if (!KEY_open() || !video_open() || !accel_open())
        return -1;

    video_read(&screen_x, &screen_y, &char_x, &char_y); // get screen & text size
    video_erase();                                      // erase any text on the screen

    int bg_width = 319;
    int bg_height = 239;
    int border_weight = 5;
    int racket_x = bg_width / 2;
    int racket_y = bg_height - 10;

    int ball_x = bg_width / 2;
    int ball_y = bg_height / 2;

    int cima = 1;
    int direita = 1;
    int movVertical = 1;

    int key_infos;
    int pause = 0;
    int start_screen = 1;
    int lose_screen = 0;
    int win_screen = 0;

    int life = 3;
    int lifeBefore;
    int hasBricks;

    accel_format(a, b);
    accel_calibrate();

    Brick bricks[50];
    fillBricks(10, 3, 6, bricks, video_WHITE);

    while (!stop)
    {
        // mostrar tela inicial
        while (!stop && start_screen == 1)
        {
            KEY_read(&key_infos);
            video_clear();
            showStartScreen();
            if (key_infos == 2)
            {
                start_screen = 0;
            }
        }

        video_erase();
        video_clear();

        // pausa
        KEY_read(&key_infos);

        if (key_infos == 1)
        {
            pause = 1;
        }

        while (!stop && pause == 1)
        {
            KEY_read(&key_infos);
            if (key_infos == 1)
            {
                pause = 0;
            }
        }

        // leitura do acelerometro
        accel_read(&ptr_ready, &ptr_tap, &ptr_dtap, &ptr_x, &ptr_y, &ptr_z, &ptr_msg);

        video_clear(); // clear current VGA Back buffer
        renderScenario(WIDTH, HEIGHT, BORDER_WEIGHT, video_BLACK, video_WHITE);

        // raquete
        video_box(racket_x, racket_y, racket_x + RACKET_WIDTH, racket_y + RACKET_HEIGHT, video_WHITE);

        hasBricks = 1;
        int steps;
        for (steps = 0; steps < BALL_VEL; steps++)
        {
            // renderBrick(brick);
            // BOLA
            video_box(ball_x, ball_y, ball_x + BALL_SIZE, ball_y + BALL_SIZE, video_YELLOW);
            verifyBlockCollision(bricks, ball_x, ball_y, &cima, &direita);
            renderBricks(bricks, QUANT_BRICKS);

            // Colisão com a barra
            // if (ball_x > racket_x && ball_x < racket_x + RACKET_WIDTH && ball_y + 2 == bg_height - border_weight - 4) cima = -1;
            ballRacketCollision(ball_x, ball_y, racket_x, racket_y, &cima, &direita, &movVertical);

            lifeBefore = life;
            ballBorderCollision(ball_x, ball_y, &cima, &direita, &life);

            renderLifes(life);

            if (lifeBefore != life && life != 0)
            {
                // reinicia o movimento da bola
                ball_x = bg_width / 2;
                ball_y = bg_height / 2;
            }
            else if (life == 0)
            {
                lose_screen = 1;
            }

            // verifica se ganhou o jogo
            hasBricks = verifyQuantBlocks(bricks);
            if (hasBricks == 0)
            {
                win_screen = 1;
            }

            // movimentação da bola
            if (cima == 1 && movVertical == 0)
            {
                ball_y += 1;
            }
            else if (movVertical == 0)
            {
                ball_y -= 1;
            }

            if (direita == 1 && movVertical == 0)
            {
                ball_x += 1;
            }
            else if (movVertical == 0)
            {
                ball_x -= 1;
            }

            if (movVertical == 1 && cima == 1)
            {
                ball_y += 1;
            }
            else if (movVertical == 1)
            {
                ball_y -= 1;
            }
        }

        video_show(); // swap Front/Back to display the line
        // movimentação da barra
        // -=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=
        printf("valor %d \n", ptr_x);

        // mostrar tela de vitória
        while (!stop && win_screen == 1)
        {
            KEY_read(&key_infos);
            video_clear();
            showWinScreen();
            if (key_infos == 2)
            {
                lose_screen = 0;
                start_screen = 1;
                life = 3;

                ball_x = bg_width / 2;
                ball_y = bg_height / 2;

                cima = 1;
                direita = 1;
                movVertical = 0;

                pause = 0;
                start_screen = 1;
                lose_screen = 0;
                win_screen = 0;
                fillBricks(10, 4, 6, bricks, video_WHITE);
            }
        }

        // mostrar tela de "você perdeu"
        while (!stop && lose_screen == 1)
        {
            KEY_read(&key_infos);
            video_clear();
            showLoseScreen();
            if (key_infos == 2)
            {
                lose_screen = 0;
                start_screen = 1;
                life = 3;

                ball_x = bg_width / 2;
                ball_y = bg_height / 2;

                cima = 1;
                direita = 1;
                movVertical = 0;

                pause = 0;
                start_screen = 1;
                lose_screen = 0;
                win_screen = 0;
                fillBricks(10, 4, 6, bricks, video_WHITE);
            }
        }

        if (start_screen == 1)
            continue;

        // movimentação da raquete
        if (ptr_x <= 10 && ptr_x >= -10)
            ptr_x = 0; // estabilização

        // if (ptr_x >= 1)
        //     ptr_x = RACKET_VEL;
        // if (ptr_x <= -1)
        //     ptr_x = -RACKET_VEL;

        racket_x += ptr_x / 10; // movimentação

        if (racket_x + RACKET_WIDTH >= screen_x - border_weight - border_weight)
        {
            racket_x = screen_x - (RACKET_WIDTH + border_weight + 1);
        }
        else if (racket_x <= border_weight)
        {
            racket_x = border_weight;
        }

        // colisão da raquete com as paredes
        racketCollision(&racket_x, RACKET_WIDTH, screen_x, BORDER_WEIGHT);
    }

    video_close();
    accel_close();
    KEY_close();
    printf("\nExiting sample program\n");
    return 0;
}

Brick createBrick(int x1, int y1, int x2, int y2, short color, int visible)
{

    Brick brick;

    brick.x1 = x1;
    brick.y1 = y1;
    brick.x2 = x2;
    brick.y2 = y2;
    brick.visible = visible;
    brick.color = color;

    return brick;
}

void renderBrick(Brick brick)
{
    if (brick.visible == 1)
    {
        video_box(brick.x1, brick.y1, brick.x2, brick.y2, brick.color);
    }
}

void fillBricks(int quantBricks, int quantRows, int margin, Brick *bricks, short color)
{
    int i;
    int j;
    int block_x_size = (WIDTH - BORDER_WEIGHT * 2) / quantBricks;
    int index = 0;
    for (i = 1; i < quantRows + 1; i++)
    {
        for (j = 1; j < quantBricks + 1; j++)
        {
            Brick brick;
            brick.x1 = margin + BORDER_WEIGHT + block_x_size * (j - 1);
            brick.y1 = TOP_MARGIN + margin + BORDER_WEIGHT + 20 * (i - 1);
            brick.x2 = BORDER_WEIGHT + block_x_size * j;
            brick.y2 = TOP_MARGIN + BORDER_WEIGHT + 20 * i;
            brick.visible = 1;
            brick.color = color;
            bricks[index] = brick;

            index++;
        }
    }
}

void renderBricks(Brick *bricks, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        renderBrick(bricks[i]);
    }
}

void verifyBlockCollision(Brick *blocks, int ball_x, int ball_y, int *vert, int *hori)
{
    int i;
    for (i = 0; i < QUANT_BRICKS; i++)
    {
        if (blocks[i].visible == 1)
        {
            // verifica se bateu em cima ou em baixo
            if (ball_x <= blocks[i].x2 && ball_x >= blocks[i].x1)
            {
                if ((ball_y - 1) == blocks[i].y2)
                {
                    blocks[i].visible = 0;
                    *vert = 1;
                }
                else if ((ball_y + 1) == blocks[i].y1)
                {
                    blocks[i].visible = 0;
                    *vert = -1;
                }
            }

            // verifica se bateu na direita ou na esquerda
            if (ball_y >= blocks[i].y1 && ball_y <= blocks[i].y2)
            {
                if ((ball_x + 1) == blocks[i].x1)
                {
                    blocks[i].visible = 0;
                    *hori = -1;
                }
                else if ((ball_x - 1) == blocks[i].x2)
                {
                    blocks[i].visible = 0;
                    *hori = 1;
                }
            }

            // verifica se bateu na diagonal
            if (ball_x - 1 == blocks[i].x2 && ball_y - 1 == blocks[i].y2)
            {
                blocks[i].visible = 0;
                *hori = 1;
                *vert = 1;
            }

            if (ball_x + 1 == blocks[i].x1 && ball_y + 1 == blocks[i].y1)
            {
                blocks[i].visible = 0;
                *hori = -1;
                *vert = -1;
            }

            if (ball_x - 1 == blocks[i].x2 && ball_y + 1 == blocks[i].y1)
            {
                blocks[i].visible = 0;
                *hori = 1;
                *vert = -1;
            }

            if (ball_x + 1 == blocks[i].x1 && ball_y - 1 == blocks[i].y2)
            {
                blocks[i].visible = 0;
                *hori = -1;
                *vert = 1;
            }
        }
    }
}

// verifica se há algum bloco restante
int verifyQuantBlocks(Brick *bricks)
{
    int i;
    int hasBlocks = 0;
    for (i = 0; i < QUANT_BRICKS; i++)
    {
        if (bricks[i].visible == 1)
        {
            hasBlocks = 1;
        }
    }

    return hasBlocks;
}

void showStartScreen()
{
    video_erase(); // erase any text on the screen
    char startMessage[] = "PRESS KEY 1 TO START GAME";
    video_text((80 / 2) - (strlen(startMessage) / 2), 60 / 2 - 1, startMessage);
    video_show();
}

void showLoseScreen()
{
    video_clear();
    video_erase(); // erase any text on the screen
    char startMessage[] = "YOU LOSE";
    video_text((80 / 2) - (strlen(startMessage) / 2), 60 / 2 - 1, startMessage);
    video_show();
}

void showWinScreen()
{
    video_clear();
    video_erase(); // erase any text on the screen
    char startMessage[] = "YOU WIN";
    video_text((80 / 2) - (strlen(startMessage) / 2), 60 / 2 - 1, startMessage);
    video_show();
}

void renderScenario(int maxWidth, int maxHeight, int borderWeight, short bgColor, short borderColor)
{
    video_box(maxWidth, maxHeight, 0, TOP_MARGIN, borderColor);
    video_box(maxWidth - borderWeight, maxHeight, borderWeight, TOP_MARGIN + borderWeight, bgColor);
}

void racketCollision(int *racket_x, int racket_width, int screen_x, int border_weight)
{
    if ((*racket_x) + racket_width >= screen_x - border_weight - border_weight)
    {
        *racket_x = screen_x - (racket_width + border_weight + 1);
    }
    else if ((*racket_x) <= border_weight)
    {
        *racket_x = border_weight;
    }
}

void ballRacketCollision(int ball_x, int ball_y, int racket_x, int racket_y, int *vert, int *hori, int *movVertical)
{
    if ((ball_x >= racket_x && ball_x <= racket_x + (RACKET_WIDTH / 3) && ball_y + 1 == HEIGHT - BORDER_WEIGHT - 6))
    {
        *vert = -1;
        *hori = -1;
        *movVertical = 0;
    }

    if ((ball_x >= racket_x + (RACKET_WIDTH / 3) && ball_x <= racket_x + (RACKET_WIDTH / 3) * 2 && ball_y + 1 == HEIGHT - BORDER_WEIGHT - 6))
    {
        *vert = -1;
        *movVertical = 1;
    }

    if ((ball_x >= racket_x + (RACKET_WIDTH / 3) * 2 && ball_x <= racket_x + RACKET_WIDTH && ball_y + 1 == HEIGHT - BORDER_WEIGHT - 6))
    {
        *vert = -1;
        *hori = 1;
        *movVertical = 0;
    }

    if (ball_x + 2 == racket_x && ball_y + 11 >= HEIGHT)
    {
        *hori = *hori * -1;
        *vert = *vert * -1;
        *movVertical = 0;
    }
    if (ball_x - RACKET_WIDTH == racket_x && ball_y + 11 >= HEIGHT)
    {
        *hori = *hori * 1;
        *vert = *vert * -1;
        *movVertical = 0;
    }
}

void ballBorderCollision(int ball_x, int ball_y, int *vert, int *hori, int *life)
{
    // Colisão com o lado direito
    if (ball_x + 1 == WIDTH - BORDER_WEIGHT)
        *hori = -1;

    // Colisão com o topo
    if (ball_y + 1 == TOP_MARGIN + BORDER_WEIGHT)
        *vert = 1;

    // Colisão com o lado esquerdo
    if (ball_x - 1 == BORDER_WEIGHT)
        *hori = 1;

    // Colisão com o chão = perde uma vida
    if (ball_y - 1 == HEIGHT - BORDER_WEIGHT - 6)
        *life -= 1;
}

void showLifeSquare(int start_position)
{
    video_box(start_position, 15, start_position + 1, 16, video_RED);
    video_box(start_position + 1, 16, start_position + 2, 17, video_RED);
    video_box(start_position + 2, 17, start_position + 3, 18, video_RED);
    video_box(start_position + 3, 18, start_position + 4, 19, video_RED);

    video_box(start_position, 15, start_position + 1, 16, video_RED);
    video_box(start_position + 1, 16, start_position + 2, 17, video_RED);
    video_box(start_position + 2, 17, start_position + 3, 18, video_RED);
    video_box(start_position + 3, 18, start_position + 4, 19, video_RED);

    video_box(start_position, 15, start_position + 1, 16, video_RED);
    video_box(start_position + 1, 16, start_position + 2, 17, video_RED);
    video_box(start_position + 2, 17, start_position + 3, 18, video_RED);
    video_box(start_position + 3, 18, start_position + 4, 19, video_RED);

    video_box(start_position, 15, start_position + 1, 16, video_RED);
    video_box(start_position + 1, 16, start_position + 2, 17, video_RED);
    video_box(start_position + 2, 17, start_position + 3, 18, video_RED);
    video_box(start_position + 3, 18, start_position + 4, 19, video_RED);
}

void renderLifes(int life)
{
    if (life == 3)
    {
        showLifeSquare(BORDER_WEIGHT);
        showLifeSquare(BORDER_WEIGHT + 10);
        showLifeSquare(BORDER_WEIGHT + 20);
    }
    else if (life == 2)
    {
        showLifeSquare(BORDER_WEIGHT);
        showLifeSquare(BORDER_WEIGHT + 10);
    }
    else if (life == 1)
    {
        showLifeSquare(BORDER_WEIGHT);
    }
}
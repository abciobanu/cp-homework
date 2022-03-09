/* Ciobanu Andrei-Bogdan 312CB */

#include "solver.h"
#include "utils.h"

#include <stdlib.h>

#define DIGITS 9 /* numarul de cifre */
#define DIGIT_SIZE 5 /* cifra este de dimensiune 5x5 */
#define SQUARE_SIZE 8 /* dimensiunea unui patrat din grila, fara o margine */
#define BLOCKS_PER_LINE 3 /* numarul de sectiuni 3x3 pe o linie / coloana */
#define BLOCK_SIZE 3 /* o sectiune este de dimensiune 3x3 */

/* inverseaza culorile de la start_pos la start_pos + len */
void reverse_colors(Color *array, int start_pos, int len)
{
    for (int k = 0; k < len / 2; ++k) {
        Color temp = array[start_pos + k];
        array[start_pos + k] = array[start_pos + len - 1 - k];
        array[start_pos + len - 1 - k] = temp;
    }
}

/* oglindeste cifrele din grila */
void mirror_digits(bmp *bitmap)
{
    int width = bitmap->infoheader->width;
    int height = bitmap->infoheader->height;

    for (int i = 2; i < height - 2; ++i) {
        if (i % SQUARE_SIZE < 1 || i % SQUARE_SIZE == SQUARE_SIZE - 1) {
            continue; /* margine neagra/gri sau pixel alb */
        }

        for (int j = 2; j < width - 2; j += SQUARE_SIZE) {
            reverse_colors(bitmap->pixel_array, i * width + j, DIGIT_SIZE);
        }
    }
}

/* identifica cifra care incepe la [pos_y, pos_x] - coltul din stanga sus */
int identify_digit(bmp *bitmap, int pos_x, int pos_y)
{
    /*
        campul blue este 175 => pixelul este roz (r: 255, g: 175, b: 175)
        campul blue este 255 => pixelul este alb (r: 255, g: 255, b: 255)
    */
    int width = bitmap->infoheader->width;
    Color *pixels = bitmap->pixel_array;

    if (pixels[pos_y * width + pos_x].blue == 255) {
        if (pixels[pos_y * width + pos_x + 4].blue == 175) {
            return 1;
        }

        return -1; // patrat gol sau cu alta forma
    } else if (pixels[(pos_y + 3) * width + pos_x + 4].blue == 255) {
        return 2;
    } else if (pixels[(pos_y + 2) * width + pos_x].blue == 255) {
        return 7;
    } else if (pixels[(pos_y + 1) * width + pos_x].blue == 255) {
        return 3;
    } else if (pixels[pos_y * width + pos_x + 1].blue == 255) {
        return 4;
    } else if (pixels[(pos_y + 1) * width + pos_x + 4].blue == 255) {
        if (pixels[(pos_y + 3) * width + pos_x].blue == 255) {
            return 5;
        }

        return 6;
    } else if (pixels[(pos_y + 3) * width + pos_x].blue == 175) {
        if (pixels[(pos_y + 2) * width + pos_x + 2].blue == 175) {
            return 8;
        }

        return -1; // 0 sau alta forma
    }

    return 9;
}

/* returneaza grila de sudoku cu numerele din bitmap identificate */
int *get_grid_from_bitmap(bmp *bitmap)
{
    int width = bitmap->infoheader->width;
    int height = bitmap->infoheader->height;

    int *grid = malloc(DIGITS * DIGITS * sizeof(int));
    if (grid == NULL) {
        fprintf(stderr, "Couldn't allocate memory for sudoku grid!\n");
        return NULL;
    }

    for (int i = 2; i < width; i += SQUARE_SIZE) {
        for (int j = 2; j < height; j += SQUARE_SIZE) {
            int digit = identify_digit(bitmap, j, i);
            grid[i / SQUARE_SIZE * DIGITS + j / SQUARE_SIZE] = digit;
        }
    }

    return grid;
}

/* verificarea corectitudinii completarii pe orizontala */
int horizontal_check(int *grid)
{
    int *used_digits = calloc(DIGITS, sizeof(int));
    if (used_digits == NULL) {
        return 1;
    }

    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            /* verificare forma nepermisa in grila */
            if (grid[i * DIGITS + j] < 0) {
                free(used_digits);

                return 1;
            } else if (used_digits[grid[i * DIGITS + j] - 1] == 0) {
                ++used_digits[grid[i * DIGITS + j] - 1];
            } else {
                free(used_digits);

                return 1;
            }
        }

        /* resetare vector de frecventa */
        for (int k = 0; k < DIGITS; ++k) {
            used_digits[k] = 0;
        }
    }

    free(used_digits);

    return 0;
}

/* verificarea corectitudinii completarii pe verticala */
int vertical_check(int *grid)
{
    int *used_digits = calloc(DIGITS, sizeof(int));
    if (used_digits == NULL) {
        return 1;
    }

    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            if (used_digits[grid[j * DIGITS + i] - 1] == 0) {
                ++used_digits[grid[j * DIGITS + i] - 1];
            } else {
                free(used_digits);

                return 1;
            }
        }

        /* resetare vector de frecventa */
        for (int k = 0; k < DIGITS; ++k) {
            used_digits[k] = 0;
        }
    }

    free(used_digits);

    return 0;
}

/* verificarea corectitudinii completarii in sectiuni 3x3 */
int block_check(int *grid)
{
    int *used_digits = calloc(DIGITS, sizeof(int));
    if (used_digits == NULL) {
        return 1;
    }

    /* verificare sectiuni 3x3 pe coloane */
    for (int col = 0; col < BLOCKS_PER_LINE; ++col) {
        for (int i = 0; i < BLOCK_SIZE; ++i) {
            for (int j = 0; j < BLOCK_SIZE; ++j) {
                if (used_digits[grid[i * DIGITS + j] - 1] == 0) {
                    ++used_digits[grid[i * DIGITS + j] - 1];
                } else {
                    free(used_digits);

                    return 1;
                }
            }

            /* resetare vector de frecventa */
            for (int k = 0; k < DIGITS; ++k) {
                used_digits[k] = 0;
            }
        }
    }

    free(used_digits);

    return 0;
}

/* verificare grila de sudoku */
int check_grid(bmp *bitmap)
{
    int *sudoku_grid = get_grid_from_bitmap(bitmap);
    if (sudoku_grid == NULL) {
        return 1;
    }

    int result = horizontal_check(sudoku_grid);
    /* gresit completat pe orizontala */
    if (result == 1) {
        free(sudoku_grid);

        return 1;
    }

    result = vertical_check(sudoku_grid);
    /* gresit completat pe verticala */
    if (result == 1) {
        free(sudoku_grid);

        return 1;
    }

    result = block_check(sudoku_grid);
    /* gresit completat in sectiunile 3x3 */
    if (result == 1) {
        free(sudoku_grid);

        return 1;
    }

    free(sudoku_grid);

    return 0;
}

/* rezolvare task-uri 1, 2 si 3 */
int solve_task123(char *input_path)
{
    /* deschide fisierul de input */
    FILE *input_file = fopen(input_path, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Couldn't open file %s!\n", input_path);
        return 1;
    }

    /* obtine obiectul JSON */
    cJSON *json = get_json_from_file(input_file);
    fclose(input_file);
    if (json == NULL) {
        return 1;
    }

    /* obtine structura bmp din JSON */
    bmp *bitmap = get_bmp_from_json(json);
    cJSON_Delete(json);
    if (bitmap == NULL) {
        return 1;
    }

    /* output task 1 */
    int result = write_to_bitmap_file(bitmap, input_path, 1);
    if (result == 1) {
        free_bmp(bitmap);

        return 1;
    }

    /* rezolvare task 2 */
    mirror_digits(bitmap);

    /* output task 2 */
    result = write_to_bitmap_file(bitmap, input_path, 2);
    if (result == 1) {
        free_bmp(bitmap);

        return 1;
    }

    /* rezolvare task 3 */
    int game_state = check_grid(bitmap);
    free_bmp(bitmap);

    /* output task 3 */
    result = write_to_json_file(input_path, game_state);
    if (result == 1) {
        return 1;
    }

    return 0;
}

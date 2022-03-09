#include "util/scrabble.h"
#include "util/print_board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32
#define PLAYERS 2
#define SUBSTRINGS 2
#define SUBSTRING_LENGTH 3

#define NO_BONUS 0
#define BONUS_2X 1
#define BONUS_3X 2
#define BONUS_ALL 3

void populate_board(char board[BOARD_SIZE][BOARD_SIZE], char character)
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = character;
}

unsigned int read_substrings(char substrings[][SUBSTRING_LENGTH])
{
    char buffer[BUFFER_SIZE];

    for (unsigned int i = 0; i < SUBSTRINGS; ++i) {
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            return 0;

        strncpy(substrings[i], buffer, SUBSTRING_LENGTH - 1);
    }

    return 1;
}

char *get_word_attributes(int *y, int *x, int *direction, char *buffer)
{
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        return NULL;

    size_t buffer_len = strlen(buffer);
    if (buffer[buffer_len - 1] == '\n')
        buffer[buffer_len - 1] = '\0';

    char *word_attr = strtok(buffer, " ");
    *y = atoi(word_attr);

    word_attr = strtok(NULL, " ");
    *x = atoi(word_attr);

    word_attr = strtok(NULL, " ");
    *direction = atoi(word_attr);

    word_attr = strtok(NULL, " "); // ultimul din linie este cuvantul
    return word_attr;
}

unsigned int get_bonus_type(char *word, char substrings[][SUBSTRING_LENGTH])
{
    unsigned int bonus_type = 0;

    if (strstr(word, substrings[0]) != NULL)
        ++bonus_type; // cuvantul are substringul "XX" in compozitie
    if (strstr(word, substrings[1]) != NULL)
        if (strlen(strstr(word, substrings[1])) == 2)
            bonus_type += 2; // cuvantul se termina cu substringul "YY"

    return bonus_type;
}

int check_bonus(int y, int x, int bonus_type)
{
    int bonus = bonus_board[y][x];
    if (bonus == bonus_type || bonus_type == BONUS_ALL)
        return bonus + 1; // multiplicator in functie de bonus

    return 1; // multiplicator 1; punctajul nu se dubleaza sau tripleaza
}

int process_word(int task, char *word, int y, int x, int direction,
                 char substrings[][SUBSTRING_LENGTH])
{
    const int score_points[] = {
        1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3,
        1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
    };

    int bonus_multiplier = 1;
    int bonus_type = NO_BONUS;
    if (task == 3 || task >= 5)
        bonus_type = get_bonus_type(word, substrings);

    int score = 0;
    size_t word_len = strlen(word);
    for (unsigned int i = 0; i < word_len; ++i) {
        if (direction == 0 && (task == 3 || task >= 5)) // pe orizontala
            bonus_multiplier *= check_bonus(y, x + i, bonus_type);
        else if (direction == 1 && (task == 3 || task >= 5)) // pe verticala
            bonus_multiplier *= check_bonus(y + i, x, bonus_type);

        if (task == 2 || task == 3 || task >= 5) {
            char letter = *(word + i);
            score += score_points[letter - 'A'];
        }
    }

    return score * bonus_multiplier;
}

void print_score(unsigned int scores[PLAYERS])
{
    printf("Player 1: %u Points\n", scores[0]);
    printf("Player 2: %u Points\n", scores[1]);
}

int is_used(char *word, char list[][BOARD_SIZE], size_t list_size)
{
    for (unsigned int i = 0; i < list_size; ++i)
        if (strcmp(word, list[i]) == 0)
            return 1;

    return 0;
}

int check_space(int y, int x, int direction, size_t length,
                char board[][BOARD_SIZE])
{
    for (unsigned int i = 1; i < length; ++i)
        if (direction == 0) { // orizontala
            if (x + i == BOARD_SIZE || board[y][x + i] != '.')
                return 0; // iese din tabla sau se suprapune cu alte cuvinte
        }
        else if (direction == 1) { // verticala
            if (y + i == BOARD_SIZE || board[y + i][x] != '.')
                return 0; // iese din tabla sau se suprapune cu alte cuvinte
        }

    return 1;
}

void place_word(char *word, int y, int x, int direction,
                char board[][BOARD_SIZE])
{
    size_t word_len = strlen(word);
    for (unsigned int i = 0; i < word_len; ++i)
        if (direction == 0) // orizontala
            board[y][x + i] = word[i];
        else // verticala
            board[y + i][x] = word[i];
}

int is_best(int score, int y, int x, int best_score, int best_y, int best_x)
{
    int first_condition = (score > best_score);
    int aux_condition = (score == best_score);
    int second_condition = (aux_condition && y < best_y);
    int third_condition = (aux_condition && y == best_y && x < best_x);

    return first_condition || second_condition || third_condition;
}

int find_next_word(char board[][BOARD_SIZE], char used_words[][BOARD_SIZE],
                   size_t used_words_size, int task, unsigned int score[],
                   char substrings[][SUBSTRING_LENGTH])
{
    static char found_words[NUM_WORDS][BOARD_SIZE];
    static size_t found_words_size = 0;

    struct {
        char *word;
        int x, y, direction, score;
    } best_word = {NULL, 0, 0, 0, 0};

    for (int i = 0; i < NUM_WORDS; ++i) {
        if (is_used(words[i], used_words, used_words_size))
            continue;
        if (is_used(words[i], found_words, found_words_size))
            continue;

        size_t word_len = strlen(words[i]);
        for (int j = 0; j < BOARD_SIZE; ++j)
            for (int k = 0; k < BOARD_SIZE; ++k)
                if (board[j][k] == words[i][0])
                    for (int direction = 0; direction < 2; ++direction) {
                        // verifica daca este spatiu pe orizontala / verticala
                        int is_space = check_space(j, k, direction, word_len,
                                                   board);

                        if (is_space && task == 4) {
                            place_word(words[i], j, k, direction, board);
                            return 1;
                        } else if (is_space && task >= 5) {
                            // calculeaza punctajul pentru cuvantul curent
                            int w_score = process_word(task, words[i], j, k,
                                                       direction, substrings);

                            if (is_best(w_score, j, k, best_word.score,
                                        best_word.y, best_word.x)) {
                                best_word.word = words[i];
                                best_word.x = k;
                                best_word.y = j;
                                best_word.direction = direction;
                                best_word.score = w_score;
                            }
                        }
                    }
    }

    if (task == 5 && best_word.score + score[1] < score[0]) {
        // nu s-a gasit un cuvant care aduce castig sau remiza
        printf("Fail!\n");
        return 0;
    } else if (task == 6) {
        size_t best_word_size = strlen(best_word.word);
        strncpy(found_words[found_words_size], best_word.word, best_word_size);
        ++found_words_size;

        score[1] += best_word.score; // actualizeaza punctajul pentru Player 2
    }

    place_word(best_word.word, best_word.y, best_word.x, best_word.direction,
               board);

    return 1;
}

int read_lines(char board[][BOARD_SIZE], int task)
{
    char substrings[SUBSTRINGS][SUBSTRING_LENGTH] = {0};
    if (task >= 3 && !read_substrings(substrings))
        return 0;

    unsigned int score[PLAYERS] = {0};
    char used_words[NUM_WORDS][BOARD_SIZE] = {0};

    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        return 0;

    unsigned int number_of_words = atoi(buffer);

    for (unsigned int i = 0; i < number_of_words; ++i) {
        // proceseaza linia curenta
        int y, x, direction;
        char *word = get_word_attributes(&y, &x, &direction, buffer);
        if (word == NULL)
            return 0;

        place_word(word, y, x, direction, board);
        strncpy(used_words[i], word, strlen(word));

        int word_score = process_word(task, word, y, x, direction, substrings);
        if (task == 2 || task == 3 || task == 5)
            score[i % 2] += word_score;
        else if (task == 6) {
            score[0] += word_score; // actualizeaza scorul pentru Player 1
            find_next_word(board, used_words, i + 1, task, score, substrings);
        }
    }

    if (task == 1)
        print_board(board);
    else if (task == 2 || task == 3)
        print_score(score);
    else if (task == 4 || task == 5) {
        int found = find_next_word(board, used_words, number_of_words, task,
                                   score, substrings);
        if (found)
            print_board(board);
    } else if (task == 6) {
        print_board(board);
        printf("Player %d Won!\n", (score[0] > score[1]) ? 1 : 2);
    }

    return 1;
}

int main()
{
    char game_board[BOARD_SIZE][BOARD_SIZE];
    populate_board(game_board, '.');

    int task = fgetc(stdin) - '0';
    while (fgetc(stdin) != '\n')
        ;

    if (task == 0)
        print_board(game_board);
    else if (!read_lines(game_board, task))
        return 1;

    return 0;
}
#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state()
{
    // TODO: Implement this function.
    game_state_t *default_state = malloc(sizeof(game_state_t));
    const unsigned int MAXN = 21;
    default_state->num_rows = 18;
    default_state->num_snakes = 1;
    default_state->snakes = malloc(sizeof(snake_t));
    default_state->snakes->tail_row = 2;
    default_state->snakes->tail_col = 2;
    default_state->snakes->head_row = 2;
    default_state->snakes->head_col = 4;
    default_state->snakes->live = true;
    default_state->num_snakes = 1;
    default_state->board = malloc(sizeof(char *) * default_state->num_rows);
    char *rows[3] = {"####################", "#                  #", "# d>D    *         #"};
    for (int i = 0; i < default_state->num_rows; i++)
    {
        default_state->board[i] = malloc(sizeof(char) * MAXN);
        if (i == 0 || i == default_state->num_rows - 1)
        {
            strcpy(default_state->board[i], rows[0]);
        }
        else if (i == 2)
        {
            strcpy(default_state->board[i], rows[2]);
        }
        else
        {
            strcpy(default_state->board[i], rows[1]);
        }
    }
    return default_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
    // TODO: Implement this function.
    for (int i = 0; i < state->num_rows; i++)
    {
        free(state->board[i]);
    }
    free(state->board);
    free(state->snakes);
    free(state);
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
    // TODO: Implement this function.
    for (int i = 0; i < state->num_rows; i++)
    {
        fprintf(fp, "%s\n", state->board[i]);
    }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
    FILE *f = fopen(filename, "w");
    print_board(state, f);
    fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col)
{
    return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
    state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
    // TODO: Implement this function.
    return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
    // TODO: Implement this function.
    return c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x';
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
    // TODO: Implement this function.
    return is_tail(c) || is_head(c) || c == '^' || c == '<' || c == 'v' || c == '>';
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
    // TODO: Implement this function.
    if (c == '^')
    {
        return 'w';
    }
    else if (c == '<')
    {
        return 'a';
    }
    else if (c == 'v')
    {
        return 's';
    }
    else if (c == '>')
    {
        return 'd';
    }
    return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
    // TODO: Implement this function.
    if (c == 'W')
    {
        return '^';
    }
    else if (c == 'A')
    {
        return '<';
    }
    else if (c == 'S')
    {
        return 'v';
    }
    else if (c == 'D')
    {
        return '>';
    }
    return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
    // TODO: Implement this function.
    if (c == 'v' || c == 's' || c == 'S')
    {
        return cur_row + 1;
    }
    else if (c == '^' || c == 'w' || c == 'W')
    {
        return cur_row - 1;
    }
    return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
    // TODO: Implement this function.
    if (c == '>' || c == 'd' || c == 'D')
    {
        return cur_col + 1;
    }
    else if (c == '<' || c == 'a' || c == 'A')
    {
        return cur_col - 1;
    }
    return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
    // TODO: Implement this function.
    unsigned int current_row_id = state->snakes[snum].head_row;
    unsigned int current_col_id = state->snakes[snum].head_col;
    char current_char = get_board_at(state, current_row_id, current_col_id);
    unsigned int next_row_id = get_next_row(current_row_id, current_char);
    unsigned int next_col_id = get_next_col(current_col_id, current_char);
    return get_board_at(state, next_row_id, next_col_id);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
    // TODO: Implement this function.
    // Update the head on the game board
    unsigned int current_row_id = state->snakes[snum].head_row;
    unsigned int current_col_id = state->snakes[snum].head_col;
    char current_char = get_board_at(state, current_row_id, current_col_id);
    unsigned int next_row_id = get_next_row(current_row_id, current_char);
    unsigned int next_col_id = get_next_col(current_col_id, current_char);
    // 原本是 head 的地方要变成 body, 需要经过 head_to_body 的转化
    set_board_at(state, current_row_id, current_col_id, head_to_body(current_char));
    // 原本不是 head 的地方要直接变成原来的 head
    set_board_at(state, next_row_id, next_col_id, current_char);
    // Update the head in the snake_t struct
    state->snakes[snum].head_row = next_row_id;
    state->snakes[snum].head_col = next_col_id;
    return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
    // TODO: Implement this function.
    // Update the tail on the game board
    unsigned int current_row_id = state->snakes[snum].tail_row;
    unsigned int current_col_id = state->snakes[snum].tail_col;
    char current_char = get_board_at(state, current_row_id, current_col_id);
    unsigned int next_row_id = get_next_row(current_row_id, current_char);
    unsigned int next_col_id = get_next_col(current_col_id, current_char);
    char next_char = get_board_at(state, next_row_id, next_col_id);
    // 原本是 tail 的地方要变成空白
    set_board_at(state, current_row_id, current_col_id, ' ');
    // 原本不是 tail 的地方要变成 tail, tail 的具体指向需要根据之前此处字符的方向来确定
    set_board_at(state, next_row_id, next_col_id, body_to_tail(next_char));
    // Update the tail in the snake_t struct
    state->snakes[snum].tail_row = next_row_id;
    state->snakes[snum].tail_col = next_col_id;
    return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
    // TODO: Implement this function.
    for (unsigned int i = 0; i < state->num_snakes; i++)
    {
        // The head moves into a fruit
        if (next_square(state, i) == '*')
        {
            update_head(state,
                        i); // We don't update tail in this case to grow the current snake by 1.
            add_food(state);
        }
        else if (is_snake(next_square(state, i)) || next_square(state, i) == '#')
        {
            // The snake is dead
            state->snakes[i].live = false;
            unsigned int snake_head_row = state->snakes[i].head_row;
            unsigned int snake_head_col = state->snakes[i].head_col;
            set_board_at(state, snake_head_row, snake_head_col, 'x');
            // we neither update_head() nor update_tail() in this case
        }
        else
        {
            update_head(state, i);
            update_tail(state, i);
        }
    }
    return;
}

/* Task 5.1 */
char *read_line(FILE *fp)
{
    // TODO: Implement this function.
    const long unsigned int MAXN = 500000;
    char *line = malloc(sizeof(char) * MAXN);
    if (fgets(line, (int) MAXN, fp) != NULL)
    {
        return line;
    }
    return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp)
{
    // TODO: Implement this function.
    const long unsigned int MAX_ROWS = 500000;
    char *line = NULL;
    game_state_t *new_game = malloc(sizeof(game_state_t));
    new_game->num_snakes = 0; // will be updated in Task 6
    new_game->snakes = NULL;  // will be updated in Task 6
    new_game->num_rows = 0;
    new_game->board = malloc(sizeof(char *) * MAX_ROWS);
    while ((line = read_line(fp)))
    {
        line[strlen(line) - 1] = '\0';
        const long unsigned int length = strlen(line);
        new_game->board[new_game->num_rows] = malloc(sizeof(char) * (length + 1));
        strcpy(new_game->board[new_game->num_rows], line);
        new_game->num_rows++;
    }
    new_game->board = realloc(new_game->board, sizeof(char *) * new_game->num_rows);
    return new_game;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
    // TODO: Implement this function.
    unsigned int current_row_id = state->snakes[snum].tail_row;
    unsigned int current_col_id = state->snakes[snum].tail_col;
    char current_char = get_board_at(state, current_row_id, current_col_id);

    while (!is_head(current_char))
    {
        current_row_id = get_next_row(current_row_id, current_char);
        current_col_id = get_next_col(current_col_id, current_char);
        current_char = get_board_at(state, current_row_id, current_col_id);
    }
    state->snakes[snum].head_row = current_row_id;
    state->snakes[snum].head_col = current_col_id;
    return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
    // TODO: Implement this function.
    const long unsigned int MAX_SNAKES = 500000;
    state->snakes = malloc(sizeof(snake_t) * MAX_SNAKES);
    unsigned int num_snakes = 0;
    for (unsigned int i = 0; i < state->num_rows; i++)
    {
        for (unsigned int j = 0, length = (unsigned int) strlen(state->board[i]); j < length; j++)
        {
            if (is_tail(get_board_at(state, i, j)))
            {
                state->snakes[num_snakes].tail_row = i;
                state->snakes[num_snakes].tail_col = j;
                state->snakes[num_snakes].live = true;
                find_head(state, num_snakes);
                num_snakes++;
            }
        }
    }
    state->num_snakes = num_snakes;
    state->snakes = realloc(state->snakes, sizeof(snake_t) * num_snakes);
    return state;
}

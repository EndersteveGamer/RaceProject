#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/**
 * A structure to store the tiles of the game area
 */
struct GameArea {
    size_t width;
    int *tiles;
};

/**
 * A structure to store the position of the player, its velocity and its score
 */
struct Player {
    size_t x;
    size_t y;
    int xVelocity;
    int yVelocity;
    int score;
};

/**
 * A structure to store the position of the target, and its dimensions
 */
struct Target {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
};

/**
 * Creates a game area from its width
 * @param width The width of the game area
 * @return The newly created game area
 */
struct GameArea *game_area_create(const size_t width) {
    struct GameArea *new = malloc(sizeof(struct GameArea));
    new->width = width;
    new->tiles = calloc(width * width, sizeof(int));
    return new;
}

/**
 * Destroys the game area by freeing its memory
 * @param self The game area to destroy
 */
void game_area_destroy(struct GameArea *self) {
    free(self->tiles);
    free(self);
}

/**
 * Returns a pointer to a game tile found with its coordinates
 * @param self The game area of the tile
 * @param x The x coordinate of the tile
 * @param y The y coordinate of the tile
 * @return A pointer to the tile
 */
int *game_area_get_tile(const struct GameArea *self, const size_t x, const size_t y) {
    return &self->tiles[x + y * self->width];
}

/**
 * Creates a new player from its starting coordinates
 * @param startingX The starting x coordinate of the player
 * @param startingY The starting y coordinate of the player
 * @return A pointer to the newly created player
 */
struct Player *player_create(size_t startingX, size_t startingY) {
    struct Player *new = malloc(sizeof(struct Player));
    new->x = startingX;
    new->y = startingY;
    new->xVelocity = 0;
    new->yVelocity = 0;
    return new;
}

/**
 * Destroys the player by freeing its memory
 * @param self The player to destroy
 */
void player_destroy(struct Player *self) {
    free(self);
}

/**
 * Moves the player by applying its velocity
 * @param self The player to move
 */
void player_tick_movement(struct Player *self) {
    self->x += self->xVelocity;
    self->y += self->yVelocity;
}

/**
 * The main function, getting informations from the server and playing the game automatically
 * @return 0 if everything executed successfully, the error code otherwise
 */
int main() {
    setbuf(stdout, NULL);
    char buf[BUFSIZE];

    struct GameArea *gameArea;

    // Get the size of the game area
    fgets(buf, BUFSIZE, stdin);
    int size = (int) strtol(buf,(char**) NULL, 10);

    // Create game area
    gameArea = game_area_create(size);

    // Get the tile scores
    for (int i = 0; i < size * size; ++i) {
        fgets(buf, BUFSIZE, stdin);
        int value = (int) strtol(buf,(char**) NULL, 10);
        size_t x = i % size;
        size_t y = (i - x) / size;
        *game_area_get_tile(gameArea, x, y) = value;
    }

    // get the position
    fgets(buf, BUFSIZE, stdin);
    int px = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int py = (int) strtol(buf,(char**) NULL, 10);

    // get the objective
    fgets(buf, BUFSIZE, stdin);
    int x = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int y = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int w = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int h = (int) strtol(buf,(char**) NULL, 10);
    int vx = 0;
    int vy = 0;

    for (;;) {
        // compute new px and new py
        // TODO
        printf("%i\n%i\n", px, py);
        // get the response
        fgets(buf, BUFSIZE, stdin);
        if (strcmp(buf, "ERROR\n") == 0) {
            break;
        }
        // TODO
    }
    return 0;
}

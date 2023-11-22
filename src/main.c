#include <stdio.h>
#include <stdlib.h>

struct gameArea {
    size_t width;
    int *tiles;
};

struct Player {
    size_t x;
    size_t y;
    int xVelocity;
    int yVelocity;
    int score;
};

struct Target {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
};

struct gameArea *game_area_create(const size_t width) {
    struct gameArea *new = malloc(sizeof(struct gameArea));
    new->width = width;
    new->tiles = calloc(width * width, sizeof(int));
    return new;
}

void game_area_destroy(struct gameArea *self) {
    free(self->tiles);
    free(self);
}

int *game_area_get_tile(const struct gameArea *self, const size_t x, const size_t y) {
    return &self->tiles[x + y * self->width];
}

struct Player *player_create(size_t startingX, size_t startingY) {
    struct Player *new = malloc(sizeof(struct Player));
    new->x = startingX;
    new->y = startingY;
    new->xVelocity = 0;
    new->yVelocity = 0;
    return new;
}

void player_destroy(struct Player *self) {
    free(self);
}

void player_tick_movement(struct Player *self) {
    self->x += self->xVelocity;
    self->y += self->yVelocity;
}

int main() {
    printf("Hello, World!\n");
    return 0;
}

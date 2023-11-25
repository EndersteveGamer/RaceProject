#include <stdlib.h>
#include "game_area.h"

struct GameArea *game_area_create(const size_t width) {
    struct GameArea *new = malloc(sizeof(struct GameArea));
    new->width = width;
    new->tiles = calloc(width * width, sizeof(int));
    return new;
}

void game_area_destroy(struct GameArea *self) {
    free(self->tiles);
    free(self);
}

int *game_area_get_tile_score(const struct GameArea *self, const size_t x, const size_t y) {
    return &self->tiles[x + y * self->width];
}

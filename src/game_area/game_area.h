#include "../vector/vector.h"

/**
 * A structure to store the tiles of the game area
 */
struct GameArea {
    size_t width;
    int *tiles;
};

/**
 * Creates a game area from its width
 * @param width The width of the game area
 * @return The newly created game area
 */
struct GameArea *game_area_create(size_t width);

/**
 * Destroys the game area by freeing its memory
 * @param self The game area to destroy
 */
void game_area_destroy(struct GameArea *self);

/**
 * Returns a pointer to a game tile found with its coordinates
 * @param self The game area of the tile
 * @param x The x coordinate of the tile
 * @param y The y coordinate of the tile
 * @return A pointer to the tile
 */
int *game_area_get_tile_score(const struct GameArea *self, const struct Vector *tile_position);

/**
 * A structure to store the position of the player, its velocity and its score
 */
struct Player {
    struct Vector *position;
    struct Vector *velocity;
    int score;
};

/**
 * Creates a new player and sets its starting coordinates and score
 * @param startingX The starting x coordinate of the player
 * @param startingY The starting y coordinate of the player
 * @return A pointer to the newly created player
 */
struct Player *player_create(size_t startingX, size_t startingY);

/**
 * Destroys a player by freeing its memory
 * @param self The player to destroy
 */
void player_destroy(struct Player *self);

/**
 * Moves the player by applying its velocity
 * @param self The player to move
 */
void player_tick_movement(struct Player *self);

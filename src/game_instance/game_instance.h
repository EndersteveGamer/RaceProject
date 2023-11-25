/**
 * A structure representing a game instance, storing everything necessary to handle the game in a single place
 */
struct GameInstance {
    struct Player *player;
    struct GameArea *gameArea;
    struct Target *target;
};

/**
 * Destroys a game instance by freeing its memory
 * @param self
 */
void game_instance_destroy(struct GameInstance *self);

/**
 * Ticks the game by making decisions and moving the player
 * @param self The game instance to tick
 */
void game_instance_tick(struct GameInstance *self);

/**
 * Decides if and how to change the velocity of the player to get the best score
 * @param self The game instance
 */
void change_velocity(struct GameInstance *self);

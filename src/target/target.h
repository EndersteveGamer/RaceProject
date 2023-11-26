/**
 * A structure to store the position of the target, and its dimensions
 */
struct Target {
    struct Vector *position;
    struct Vector *size;
};

/**
 * Create a new target
 * @param x The x position of the target
 * @param y The y position of the target
 * @param width The width of the target
 * @param height The height of the target
 * @return The newly created target
 */
struct Target *target_create(size_t x, size_t y, size_t width, size_t height);

/**
 * Destroys a target by freeing its memory
 * @param self The target to destroy
 */
void target_destroy(struct Target *self);

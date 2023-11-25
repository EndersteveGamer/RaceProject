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
 * Create a new target
 * @param x The x position of the target
 * @param y The y position of the target
 * @param width The width of the target
 * @param height The height of the target
 * @return The newly created target
 */
struct Target *target_create(size_t x, size_t y, size_t width, size_t height);

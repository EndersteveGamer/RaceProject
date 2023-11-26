/**
 * A struct representing a vector
 */
struct Vector {
    int x;
    int y;
};

/**
 * Creates a vector from its x and y coordinates
 * @param x The x coordinates of the vector
 * @param y The y coordinates of the vector
 * @return The newly created vector
 */
struct Vector *vec_create(int x, int y);

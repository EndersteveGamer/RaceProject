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

/**
 * Adds two vectors in another
 * @param vec1 The first vector to add
 * @param vec2 The second vector to add
 * @param res The vector holding the result
 */
void vec_add(struct Vector *vec1, struct Vector *vec2, struct Vector *res);

/**
 * Substracts two vectors in another
 * @param vec1 The first vector to substract
 * @param vec2 The second vector to substract
 * @param res The vector holding the result
 */
void vec_substract(struct Vector *vec1, struct Vector *vec2, struct Vector *res);

/**
 * Multiplies the coordinates of a vector by a value
 * @param self The vector to multiply
 * @param multiplier The multiplier
 */
void vec_mult(struct Vector *self, double multiplier);

/**
 * Copies the coordinates of a vector in another
 * @param src The vector to copy
 * @param dest The vector to copy the other into
 */
void vec_copy(struct Vector *src, struct Vector *dest);

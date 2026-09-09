#ifndef MATRIX_H
# define MATRIX_H

/**
 * @brief Represents a 4x4 matrix.
 *
 * @li The matrix is formatted as a 4x4 row-major matrix.
 * @li Matrix definition: data[row][column], column vector representation.
 * @li transformations: v' = M * v
 * @li composition: M = A * B
 */
typedef struct s_matrix_4f
{
	float m[4][4];
}	__attribute__((aligned(16))) t_matrix_4f;

/**
 * Returns the identity matrix.
 * @brief Returns the identity matrix.
 * @li Each element on the diagonal is 1, and the rest are 0.
 * @li The matrix is formatted as a 4x4 row-major matrix.
 * @li Matrix definition: data[row][column], column vector representation.
 * @li transformations: v' = M * v
 * @li composition: M = A * B
 *
 * | 1 0 0 0 | \n
 * | 0 1 0 0 | \n
 * | 0 0 1 0 | \n
 * | 0 0 0 1 |
 *
 * @return t_matrix_4f The identity matrix.
 */
t_matrix_4f	matrix_4f_identity();

/**
 * @brief Multiplies two matrices.
 * @param a The first matrix.
 * @param b The second matrix.
 * @return t_matrix_4f The result of the multiplication.
 */
t_matrix_4f matrix_4f_mult(t_matrix_4f a, t_matrix_4f b);

/**
 * @brief Transposes a matrix.
 * @param mat The matrix to transpose.
 * @return t_matrix_4f The transposed matrix.
 */
t_matrix_4f	matrix_4f_transpose(t_matrix_4f mat);

/**
 * @brief Translates a matrix.
 * @param mat The matrix to translate.
 * @param x The translation along the x-axis.
 * @param y The translation along the y-axis.
 * @param z The translation along the z-axis.
 * @return t_matrix_4f The translated matrix.
 */
t_matrix_4f	matrix_4f_translate(t_matrix_4f mat, float x, float y, float z);

#endif

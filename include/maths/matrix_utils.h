#ifndef MATRIX_UTILS_H
# define MATRIX_UTILS_H

# include "maths/matrix.h"
# include "maths/vectors.h"

/**
 * @brief Transforms a 4D vector using a 4x4 matrix.
 *
 * @param vec The vector to transform.
 * @param mat The matrix to use for transformation.
 * @return t_vec_4f The transformed vector.
 *
 * @todo Implement matrix vector mult.
 */
t_vec_4f	matrix_4f_transform_vec_4f(t_vec_4f vec, t_matrix_4f mat);

#endif

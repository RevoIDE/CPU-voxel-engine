#ifndef MATRIX_UTILS_H
# define MATRIX_UTILS_H

# include "maths/matrix.h"
# include "maths/vectors.h"

/**
 * @brief Transforms a 4D vector using a 4x4 matrix.
 *
 * @param result The transformed vector.
 * @param vec The vector to transform.
 * @param mat The matrix to use for transformation.
 */
void	matrix_4f_transform_vec_4f(t_vec_4f *restrict result, const t_vec_4f *restrict vec, const t_matrix_4f *restrict mat);

#endif

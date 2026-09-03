#ifndef VECTORS_H
# define VECTORS_H

/**
 * @brief Represents a 4D vector.
 *
 * @li x The x component of the vector.
 * @li y The y component of the vector.
 * @li z The z component of the vector.
 * @li w The w component of the vector.
 */
typedef struct s_vec_4f
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_vec_4f;

/**
 * @brief Creates a 4D vector with the given components.
 *
 * @param x The x component of the vector.
 * @param y The y component of the vector.
 * @param z The z component of the vector.
 * @param w The w component of the vector.
 *
 * @return The created vector.
 */
t_vec_4f	vec_4f(float x, float y, float z, float w);

/**
 * @brief Adds two 4D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The result of the addition.
 */
t_vec_4f	vec_4f_add(t_vec_4f a, t_vec_4f b);

/**
 * @brief Subtracts two 4D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The result of the subtraction.
 */
t_vec_4f	vec_4f_sub(t_vec_4f a, t_vec_4f b);

/**
 * @brief Multiplies two 4D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The result of the multiplication.
 */
t_vec_4f	vec_4f_mul(t_vec_4f a, t_vec_4f b);

/**
 * @brief Divides two 4D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The result of the division.
 */
t_vec_4f	vec_4f_div(t_vec_4f a, t_vec_4f b);

/**
 * @brief Computes the cross product of two 4D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The result of the cross product.
 */
t_vec_4f	vec_4f_cross(t_vec_4f a, t_vec_4f b);

/**
 * @brief Computes the dot product of two 4D vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The result of the dot product.
 */
float		vec_4f_dot(t_vec_4f a, t_vec_4f b);

/**
 * @brief Computes the length of a 4D vector.
 *
 * @param a The vector.
 *
 * @return The length of the vector.
 */
float		vec4f_len(t_vec_4f a);

#endif

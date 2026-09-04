#include "maths/matrix.h"
#include <math.h>

t_matrix_4f	matrix_4f_identity()
{
	t_matrix_4f mat = { 0 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			mat.m[i][j] = (i == j) ? 1.0f : 0.0f;

	return mat;
}

t_matrix_4f	matrix_4f_mult(t_matrix_4f a, t_matrix_4f b)
{
    t_matrix_4f	result = { 0 };

    for (int i = 0; i < 4; i++)
    	for (int k = 0; k < 4; k++)
     		for (int j = 0; j < 4; j++)
        		result.m[i][j] += a.m[i][k] * b.m[k][j];

    return result;
}

t_matrix_4f	matrix_4f_transpose(t_matrix_4f mat)
{
	t_matrix_4f result = { 0 };

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.m[j][i] = mat.m[i][j];

	return result;
}

t_matrix_4f	matrix_4f_translate(t_matrix_4f mat, float x, float y, float z)
{
	t_matrix_4f T = matrix_4f_identity();

	T.m[0][3] = x;
	T.m[1][3] = y;
	T.m[2][3] = z;

	return matrix_4f_mult(mat, T);
}

t_matrix_4f	matrix_4f_projection(float fov, float aspect, float near, float far)
{
	t_matrix_4f result = { 0 };

	float tan_half_fov = tanf(fov * 0.5f);

	result.m[0][0] = 1.0f / (aspect * tan_half_fov);
	result.m[1][1] = 1.0f / tan_half_fov;
	result.m[2][2] = -(far + near) / (far - near);
	result.m[2][3] = -1.0f;
	result.m[3][2] = -(2.0f * far * near) / (far - near);

	return result;
}

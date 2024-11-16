#ifndef _3DMATH_H
#define _3DMATH_H
#include <utility>
#include <memory.h>
#include <string.h>
#include <Types.h>
#include "GBI.h"

typedef f32 Vec __attribute__((vector_size(16)));
struct Mtx
{
	Vec v[4];

	Vec& operator[](int i) { return v[i]; }
	const Vec& operator[](int i) const { return v[i]; }
};

void Normalize(Vec& v);

inline Mtx MultMatrix(Mtx m0, const Mtx& m1)
{
#if 0
	int i;
	Mtx mdest;
	for (i = 0; i < 4; i++)
	{
		mdest[0][i] = m0[0][i] * m1[0][0] + m0[1][i] * m1[0][1] + m0[2][i] * m1[0][2] + m0[3][i] * m1[0][3];
		mdest[1][i] = m0[0][i] * m1[1][0] + m0[1][i] * m1[1][1] + m0[2][i] * m1[1][2] + m0[3][i] * m1[1][3];
		mdest[2][i] = m0[0][i] * m1[2][0] + m0[1][i] * m1[2][1] + m0[2][i] * m1[2][2] + m0[3][i] * m1[2][3];
		mdest[3][i] = m0[3][i] * m1[3][3] + m0[2][i] * m1[3][2] + m0[1][i] * m1[3][1] + m0[0][i] * m1[3][0];
	}

	return mdest;
#else
	Vec x = m0[0] * m1[0][0];
	x += m0[1] * m1[0][1];
	x += m0[2] * m1[0][2];
	x += m0[3] * m1[0][3];

	Vec y = m0[0] * m1[1][0];
	y += m0[1] * m1[1][1];
	y += m0[2] * m1[1][2];
	y += m0[3] * m1[1][3];

	Vec z = m0[0] * m1[2][0];
	z += m0[1] * m1[2][1];
	z += m0[2] * m1[2][2];
	z += m0[3] * m1[2][3];

	Vec w = m0[0] * m1[3][0];
	w += m0[1] * m1[3][1];
	w += m0[2] * m1[3][2];
	w += m0[3] * m1[3][3];

	return { x, y, z, w };
#endif
}

inline void MultMatrix2(Mtx& m0, Mtx m1)
{
	m0 = MultMatrix(m0, m1);
}

inline void TransformVectorNormalize(Vec& vec, Mtx mtx)
{
	Vec vres;
	vres[0] = mtx[0][0] * vec[0] + mtx[1][0] * vec[1] + mtx[2][0] * vec[2];
	vres[1] = mtx[0][1] * vec[0] + mtx[1][1] * vec[1] + mtx[2][1] * vec[2];
	vres[2] = mtx[0][2] * vec[0] + mtx[1][2] * vec[1] + mtx[2][2] * vec[2];
	vec[0] = vres[0];
	vec[1] = vres[1];
	vec[2] = vres[2];

	Normalize(vec);
}

inline void InverseTransformVectorNormalize(Vec src, Vec& dst, Mtx mtx)
{
	dst[0] = mtx[0][0] * src[0] + mtx[0][1] * src[1] + mtx[0][2] * src[2];
	dst[1] = mtx[1][0] * src[0] + mtx[1][1] * src[1] + mtx[1][2] * src[2];
	dst[2] = mtx[2][0] * src[0] + mtx[2][1] * src[1] + mtx[2][2] * src[2];

	Normalize(dst);
}

inline void InverseTransformVectorNormalizeN(Vec src[], Vec dst[], Mtx mtx, u32 count)
{
	for (u32 i = 0; i < count; i++)
	{
		InverseTransformVectorNormalize(src[i], dst[i], mtx);
	}
}

inline void Normalize(Vec& v)
{
	float len;
	Vec mult = v * v;
	len = mult[0] + mult[1] + mult[2];
	if (len != 0.0) {
		len = sqrtf(len);
		Vec vlen{ len, len, len, 1.f };
		v /= vlen;
	}
}

inline float DotProduct(Vec v0, Vec v1)
{
	Vec mult = v0 * v1;
	return mult[0] + mult[1] + mult[2];
}

inline Vec DotProductV(Vec v0, Vec v1)
{
	Vec mult = v0 * v1;
	Vec m0 = __builtin_shufflevector(mult, mult, 0, 0, 0, 0);
	Vec m1 = __builtin_shufflevector(mult, mult, 1, 1, 1, 1);
	Vec m2 = __builtin_shufflevector(mult, mult, 2, 2, 2, 2);
	return m0 + m1 + m2;
}

inline float GetFloatMatrixElement(s16 _int, u16 _fract)
{
	const s32 element = (_int << 16) | _fract;
	return _FIXED2FLOAT(element, 16);
}

inline std::pair<s16, u16> GetIntMatrixElement(f32 _elem)
{
	const s32 value = static_cast<s32>(_elem * 65536.0f);
	return std::pair<s16, u16>(static_cast<s16>(value >> 16), static_cast<u16>(value & 0xFFFF));
}

#endif

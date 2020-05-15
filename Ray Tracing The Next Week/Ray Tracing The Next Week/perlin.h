#pragma once
#ifndef PERLIN_H
#define PERLIN_H
#include "Utility.h"
inline double trilinear_interp(double c[2][2][2], double u, double v, double w);

class perlin {
public:
	perlin() {
		ranVec3 = new vec3[point_count];
		for (int i = 0; i < point_count; i++)
		{
			ranVec3[i] = unit_vector(random_vec3(-1, 1));
		}
		perm_x = perlin_generate_perm();
		perm_y = perlin_generate_perm();
		perm_z = perlin_generate_perm();
	}

	~perlin()
	{
		delete[] ranVec3;
		delete[] perm_x;
		delete[] perm_y;
		delete[] perm_z;
	}

	double noise(const Point3& p) const{
		auto u = p.x() - floor(p.x());
		auto v = p.y() - floor(p.y());
		auto w = p.z() - floor(p.z());
		//u = u * u * (3 - 2 * u);//hermitian smoothing
		//v = v * v * (3 - 2 * v);
		//w = w * w * (3 - 2 * w);
		u = u * u * u * (6 * u * u - 15 * u + 10);
		v = v * v * v * (6 * v * v - 15 * v + 10);
		w = w * w * w * (6 * w * w - 15 * w + 10);
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranVec3[
							perm_x[(i + di) & 255] ^//&��λ������������±귶Χ��0~255��,^:�������
							perm_y[(j + dj) & 255] ^
							perm_z[(k + dk) & 255]
					];
		return perlin_interp(c, u, v, w);
	}

	double turb(const Point3& p, int depth = 7)const {
		auto accum = 0.0;
		auto tmp_p = p;
		auto weight = 1.0;
		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(tmp_p);
			weight *= 0.5;
			tmp_p *= 2;
		}
		return fabs(accum);
	}

private:
	static const int point_count = 256;
	vec3* ranVec3;//0.0~1.0�����������
	int* perm_x;//0~255�������ϣ��
	int* perm_y;
	int* perm_z;
	//����һά��0~255�Ĺ�ϣ��,Ҫ��������ظ�
	static int* perlin_generate_perm() {
		auto p = new int[point_count];
		for (int i = 0; i < point_count; i++)
		{
			p[i] = i;
		}
		permute(p, point_count);
		return p;
	}
	//���������ҹ�ϣ���е�ֵ
	static void permute(int* p, int n) {
		for (int i = n - 1; i > 0; i--)
		{
			int target = random_int(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	inline double perlin_interp(vec3 c[2][2][2], double u, double v, double w) const{
		//auto uu = u * u * (3 - 2 * u);
		//auto vv = v * v * (3 - 2 * v);
		//auto ww = w * w * (3 - 2 * w);
		auto uu = u * u * u * (6 * u * u - 15 * u + 10);
		auto vv = v * v * v * (6 * v * v - 15 * v + 10);
		auto ww = w * w * w * (6 * w * w - 15 * w + 10);
		auto accum = 0.0;
		for(int i = 0;i<2;i++)
			for(int j = 0;j<2;j++)
				for (int k = 0; k < 2; k++) {
					vec3 weight_v(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu)) 
						* (j * vv + (1 - j) * (1 - vv)) 
						* (k * ww + (1 - k) * (1 - ww)) 
						* dot(c[i][j][k], weight_v);
				}
		return accum;
	}
};
//�����Բ�ֵ  trilinear interpolation
inline double trilinear_interp(double c[2][2][2], double u, double v, double w) {
	auto accum = 0.0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				accum += (i * u + (1 - i) * (1 - u)) *
						(j * v + (1 - j) * (1 - v)) *
						(k * w + (1 - k) * (1 - w)) * c[i][j][k];
	return accum;
}

#endif // !PERLIN_H


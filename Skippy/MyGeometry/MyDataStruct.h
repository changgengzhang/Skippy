#ifndef MY_DATASTRUCT_H
#define MY_DATASTRUCT_H

// 参考trimesh2的数据结构
struct Face {
	unsigned int v[3];

	Face() {}
	Face(const unsigned int &v0, const unsigned int &v1, const unsigned int &v2)
	{
		v[0] = v0; v[1] = v1; v[2] = v2;
	}
	Face(const unsigned int *v_)
	{
		v[0] = v_[0]; v[1] = v_[1]; v[2] = v_[2];
	}
	template <class S> explicit Face(const S &x)
	{
		v[0] = x[0];  v[1] = x[1];  v[2] = x[2];
	}
	unsigned int &operator[] (unsigned int i) { return v[i]; }
	const unsigned int &operator[] (unsigned int i) const { return v[i]; }
	operator const unsigned int * () const { return &(v[0]); }
	operator const unsigned int * () { return &(v[0]); }
	operator unsigned int * () { return &(v[0]); }
	unsigned int indexof(int v_) const
	{
		return (v[0] == v_) ? 0 :
			(v[1] == v_) ? 1 :
			(v[2] == v_) ? 2 : -1;
	}
};

#endif

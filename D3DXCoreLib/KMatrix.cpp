#include "KMatrix.h"
void KMatrix::XRotate(float Radian)
{
	float Cos = cos(Radian);
	float Sin = sin(Radian);
	_22 = Cos;
	_23 = Sin;
	_32 = -Sin;
	_33 = Cos;
}
void KMatrix::YRotate(float Radian)
{
	float Cos = cos(Radian);
	float Sin = sin(Radian);
	_11 = Cos;
	_13 = -Sin;
	_31 = Sin;
	_33 = Cos;
}
void KMatrix::ZRotate(float Radian)
{
	float Cos = cos(Radian);
	float Sin = sin(Radian);
	_11 = Cos;
	_12 = Sin;
	_21 = -Sin;
	_22 = Cos;
}
void KMatrix::Translation(const KVector3& v)
{
	_41 = v.x;
	_42 = v.y;
	_43 = v.z;
}
void KMatrix::Translation(float x, float y, float z)
{
	_41 = x;
	_42 = y;
	_43 = z;
}
void KMatrix::Scale(const KVector3& v)
{
	_11 = v.x;
	_22 = v.y;
	_33 = v.z;
}
void KMatrix::Scale(float x, float y, float z)
{
	_11 = x;
	_22 = y;
	_33 = z;
}
void KMatrix::Identity()
{
	_11 = _12 = _13 = _14 = 0.0f;
	_21 = _22 = _23 = _24 = 0.0f;
	_31 = _32 = _33 = _34 = 0.0f;
	_41 = _42 = _43 = _44 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}
KMatrix KMatrix::Transpose()
{
	KMatrix mat;
	mat._11 = _11; mat._12 = _21; mat._13 = _31; mat._14 = _41;
	mat._21 = _12; mat._22 = _22; mat._23 = _32; mat._24 = _42;
	mat._31 = _13; mat._32 = _23; mat._33 = _33; mat._34 = _43;
	mat._41 = _14; mat._42 = _24; mat._43 = _34; mat._44 = _44;
	return mat;
}
KMatrix KMatrix::ViewLookAt(KVector3& pos, KVector3& target, KVector3& up)
{
	KMatrix mat;
	KVector3 Direction = (target - pos).Normal(); // z축
	KVector3 RightVector = (up ^ Direction).Normal(); // x축
	KVector3 UpVector = (Direction ^ RightVector).Normal(); //y축

	_11 = RightVector.x; _12 = UpVector.x; _13 = Direction.x;
	_21 = RightVector.y; _22 = UpVector.y; _23 = Direction.y;
	_31 = RightVector.z; _32 = UpVector.z; _33 = Direction.z;

	_41 = -(pos.x * _11 + pos.y * _21 + pos.z * _31);
	_42 = -(pos.x * _12 + pos.y * _22 + pos.z * _32);
	_43 = -(pos.x * _13 + pos.y * _23 + pos.z * _33);

	memcpy((void*)&mat, this, 16 * sizeof(float));
	return mat;
}
KMatrix KMatrix::CreateViewLook(KVector3& pos, KVector3& target, KVector3& up)
{
	KMatrix mat;
	KVector3 Direction = target - pos;
	Direction = Direction.Normal();
	float dot = up | Direction;
	//직교의 근사화 작업
	KVector3 UpVector = up - (Direction * dot);
	UpVector = UpVector.Normal();
	KVector3 RightVector = UpVector ^ Direction;

	_11 = RightVector.x; _12 = UpVector.x; _13 = Direction.x;
	_21 = RightVector.y; _22 = UpVector.y; _23 = Direction.y;
	_31 = RightVector.z; _32 = UpVector.z; _33 = Direction.z;

	_41 = -(pos.x * _11 + pos.y * _21 + pos.z * _31);
	_42 = -(pos.x * _12 + pos.y * _22 + pos.z * _32);
	_43 = -(pos.x * _13 + pos.y * _23 + pos.z * _33);

	memcpy((void*)&mat, this, 16 * sizeof(float));
	return mat;
}
KMatrix KMatrix::PerspectiveFovLH(float NearPlane, float FarPlane, float fov, float aspect)
{
	float h, w, q;
	h = 1 / tan(fov * 0.5f);
	w = h / aspect;
	q = FarPlane / (FarPlane - NearPlane);

	KMatrix mat;
	memset(this, 0, sizeof(KMatrix));
	_11 = w;
	_22 = h;
	_33 = q;
	_43 = -q * NearPlane;
	_34 = 1;

	memcpy((void*)&mat, this, 16 * sizeof(float));
	return mat;
}
KMatrix KMatrix::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	KMatrix matY;
	matY.YRotate(yaw);
	KMatrix matX;
	matX.XRotate(pitch);
	KMatrix matZ;
	matZ.ZRotate(roll);
	KMatrix ret = matZ * matX * matY;
	return ret;
}
KMatrix KMatrix::operator * (KMatrix const& mat)
{
	KMatrix OutMat;
	for (int Col = 0; Col < 4; Col++)
	{
		for (int Row = 0; Row < 4; Row++)
		{
			OutMat.m[Row][Col] = m[Row][0] * mat.m[0][Col] + m[Row][1] * mat.m[1][Col] + m[Row][2] * mat.m[2][Col] + m[Row][3] * mat.m[3][Col];
		}
	}
	return OutMat;
}
KMatrix::KMatrix()
{
	Identity();
};
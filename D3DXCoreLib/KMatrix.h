#pragma once
#include "KVector.h"
struct float4x4
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
class KMatrix : public float4x4
{
public:
	void XRotate(float Radian);
	void YRotate(float Radian);
	void ZRotate(float Radian);
	void Translation(const KVector3& v);
	void Translation(float x, float y, float z);
	void Scale(const KVector3& v);
	void Scale(float x, float y, float z);
	void Identity();
	KMatrix Transpose();
	KMatrix ViewLookAt(KVector3& pos, KVector3& target, KVector3& up);//외적을 이용해 뷰 행렬 계산
	KMatrix CreateViewLook(KVector3& pos, KVector3& target, KVector3& up);//내적을 이용한 뷰 행렬 계산
	KMatrix PerspectiveFovLH(float NearPlane, float FarPlane, float fov, float aspect);
	static KMatrix RotationYawPitchRoll(float yaw, float pitch, float roll);
	KMatrix operator * (KMatrix const& mat);
	KMatrix();
};


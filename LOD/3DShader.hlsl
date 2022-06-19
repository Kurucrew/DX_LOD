cbuffer cbdata
{
	matrix g_World : packoffset(c0);
	matrix g_View : packoffset(c4);
	matrix g_Projection : packoffset(c8);
	float  g_fTimer : packoffset(c12.z);
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
VS_OUTPUT VS(float3 p: POSITION, float4 c : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 vLocal = float4(p, 1.0f);
	float4 vWorld = mul(vLocal, g_World);

	float t = cos(g_fTimer * 0.2f);
	vWorld.y = cos(t * vWorld.x * 0.1f) + sin(t * vWorld.z * 0.1f);

	float4 vView = mul(vWorld, g_View);
	float4 vProj = mul(vView, g_Projection);

	output.p = vProj;
	output.c.x = 1.0f;
	output.c.y = 1.0f;
	output.c.z = 1.0f;
	output.c.w = 1.0f;
	return output;
}
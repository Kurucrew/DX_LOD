struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};

float4 PS(VS_OUTPUT vertecies) : SV_TARGET
{
	return vertecies.c;
}
float4 main(uint vI : SV_VERTEXID) : SV_POSITION
{
	float2 texcoord = float2(vI & 1, vI >> 1);
	return float4((texcoord.x - 0.5) * 20, -(texcoord.y - 0.5) * 20, 1, 1.01);
}

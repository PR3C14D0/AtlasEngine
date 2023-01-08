struct VertexOutput
{
    float4 position : SV_Position;
    float2 tCoord : TEXCOORD0;
    float4 nml : NORMAL0;
};

cbuffer ConstantBuffer : register(b0) {
    matrix Model;
    matrix View;
    matrix Projection;
}

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

VertexOutput VertexMain(float4 position : POSITION, float2 tCoord : TEXCOORD, float4 nml : NORMAL)
{
    VertexOutput output;
    output.position = mul(position, Model);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    output.nml = nml;
    output.tCoord = tCoord;
    return output;
}

float4 PixelMain(VertexOutput input) : SV_Target
{
    return tex.Sample(samplerState, float2(input.tCoord.x, input.tCoord.y * -1.f));
    //return float4(input.tCoord.x, input.tCoord.y, 0.f, 1.f);
}
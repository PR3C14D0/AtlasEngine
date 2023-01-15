struct VertexOutput
{
    float4 position : SV_Position;
    float2 tCoord : TEXCOORD0;
    float3 nml : NORMAL0;
    float4 vertexPos : POSITION0;
};

cbuffer ConstantBuffer : register(b0) {
    matrix Model;
    matrix View;
    matrix Projection;
    float4 LightPos;
    float4 AmbientColor;
}

Texture2D tex : register(t[0]);
SamplerState samplerState : register(s[0]);

VertexOutput VertexMain(float4 position : POSITION, float2 tCoord : TEXCOORD, float4 nml : NORMAL)
{
    VertexOutput output;
    output.position = mul(position, Model);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    output.tCoord = tCoord;

    output.vertexPos = mul(position, Model);
    output.nml = normalize(mul(float3(nml.xyz), (float3x3)Model));

    return output;
}

float4 PixelMain(VertexOutput input) : SV_Target
{
    float4 diffuseColor = tex.Sample(samplerState, float2(input.tCoord.x, 1.f - input.tCoord.y));

    float3 lightDirection = normalize(LightPos.xyz - input.vertexPos.xyz);

    float diffuseIntensity = dot(lightDirection, input.nml);
    diffuseIntensity = saturate(diffuseIntensity);

    float4 ambientColor = AmbientColor;
    ambientColor += diffuseIntensity;
    
    float4 diffuseFinal = diffuseColor * saturate(ambientColor);
    return diffuseFinal;
}
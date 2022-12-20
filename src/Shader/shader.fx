struct VertexOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VertexOutput VertexMain(float4 position : POSITION, float4 color : COLOR)
{
    VertexOutput output;
    output.position = position;
    output.color = color;
    return output;
}

float4 PixelMain(VertexOutput input) :  SV_Target
{
    return input.color;
}
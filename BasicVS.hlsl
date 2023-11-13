struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
};

struct VertexShaderInput
{
    float32_t4 position : POSITIONT0;
};


VertexShaderOutput main( VertexShaderInput input ) : SV_POSITION
{
    VertexShaderOutput output;
    output.position = input.position;
    return output;
}
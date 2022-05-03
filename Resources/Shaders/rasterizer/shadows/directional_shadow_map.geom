#version 460
    
layout(triangles, invocations = NUM_CASCADES) in;
layout(triangle_strip, max_vertices = 3) out;
    
layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[NUM_CASCADES];
};
    
void main()
{          
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}  
#version 430

//in vec2 Frag_UV;
// in vec4 Frag_Color;

// uniform sampler2D Texture;

layout (location = 0) out vec4 Out_Color;

void main()
{
    Out_Color = vec4(0.0, 0.0, 1.0, 1.0);
    // Out_Color = Frag_Color;
}
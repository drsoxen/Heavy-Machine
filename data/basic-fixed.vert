#version 130
uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

in vec3 a_Vertex;
in vec2 a_TexCoord0;
in vec3 a_Normal;

out vec4 color;
out vec2 texCoord;
out vec4 position;
out vec3 normal;

float lerp(float value1, float value2, float amount)
{
    float result = 0;
    result = value1 + (value2 - value1) * amount;
    return result;
}


void main(void) 
{
	color = vec4(0,0,0,1);
    position = modelview_matrix * vec4(a_Vertex, 1.0);
    normal = normalize(normal_matrix * a_Normal);
    texCoord = a_TexCoord0;
    gl_Position = projection_matrix * position;
}




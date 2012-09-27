#version 130

precision highp float;

uniform mat4 modelview_matrix;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emissive;
uniform float material_shininess;
uniform float time;

uniform struct
{
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec4 ambient;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
}
lights[4];

uniform sampler2D texture0;

uniform bool Lights;

in vec4 color;
in vec2 texCoord;
in vec4 position;
in vec3 normal;


void main(void)
{
	vec4 FinalColor = color;

	for (int loop = 0; loop < 4; loop++)
	{
		// Ambient
		vec4 FinalAmbient = material_ambient * lights[loop].ambient;

		// Diffuse
		vec3 LightVector = (vec3(modelview_matrix * lights[loop].position) - position.xyz);
		float LightDistance = length(LightVector);
		LightVector = normalize(LightVector);
		float LdotN = max(dot(LightVector, normal), 0);
		vec4 FinalDiffuse = lights[loop].diffuse * LdotN;

		// Specular
		vec3 specReflect = normalize(-reflect(LightVector, normal));
		float RdotE = max(dot(specReflect, normalize(-position.xyz)), 0);
		vec4 FinalSpecular = lights[loop].specular * pow(RdotE,material_shininess);

		float Att = 1 / (
			(lights[loop].constant_attenuation) + // Constant
			(lights[loop].linear_attenuation * LightDistance) + // Linear
			(lights[loop].quadratic_attenuation * LightDistance * LightDistance)); // Quadratic

		FinalColor += (FinalDiffuse + FinalAmbient + FinalSpecular) * Att;
	}

	gl_FragColor = texture(texture0, texCoord) * (Lights ? FinalColor : vec4(1,1,1,1));
}
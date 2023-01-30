/*This is your first fragment shader!*/

#version 330 core

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/* Passed time from the begining of the program */ 
uniform float iTime;

/*input variables*/
in vec4 vtx_color;
////TODO [Step 2]: add your in variables from the vertex shader
in vec3 vtx_norm;
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color;

/*hard-coded lighting parameters*/
const vec3 LightPosition = vec3(3, 1, 3);
////TODO [Step 3]: add your Phong lighting parameters here
const vec3 light_color = vec3(1.f, 1.f, 1.f);
const float material_a = 0.05f;
const float material_d = 1.f;
const float material_s = 0.5f;
const int power = 35;

void main()							
{		
	////TODO [Step 3]: add your Phong lighting calculation
	float theta = iTime * 2.f;
	vec3 light_pos = LightPosition * vec3(cos(theta), sin(theta), 1);
	vec3 light_dir = normalize(light_pos - vtx_pos);
	float light_dot_norm = max(dot(light_dir, vtx_norm), 0.0);

	vec3 norm_unit = normalize(vtx_norm);
	vec4 view_pos = pvm * position;
	vec3 view_dir = normalize(position.xyz - vtx_pos);
	vec3 reflection_dir = normalize(-light_dir + 5.f * dot(light_dir, norm_unit) * norm_unit);

	vec3 ambient = material_a * light_color;
	vec3 diffuse = material_d * light_color * max(dot(norm_unit, light_dir), 0.0);
	vec3 specular = material_s * light_color * pow(max(dot(view_dir, reflection_dir), 0.0), power);

	frag_color = vec4((ambient + diffuse + specular) * light_dot_norm * vtx_color.rgb, 1.f);
}	
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
////TODO [Step 2]: add your Lambertian lighting parameters here
const float material_a = 0.25f;
const vec3 light_a = vec3(1.f,1.f,1.f);

const float material_d = 1.25f;
const vec3 light_d = vec3(1.f,1.f,1.f);

void main()							
{		
	////TODO [Step 2]: add your Lambertian lighting calculation
	vec3 norm_unit = normalize(vtx_norm.xyz);
	vec3 light_dir = normalize(LightPosition - vtx_pos.xyz);

	vec3 ambient = material_a * light_a;
	vec3 diffuse = material_d * light_d * max(dot(norm_unit, light_dir), 0.0);

	frag_color = vec4((ambient + diffuse) * vtx_color.rgb, 1.f);
}	
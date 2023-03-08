//SKYBOX VERT

#version 330 core

#define PI 3.14159265

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*input variables*/
layout (location=0) in vec4 pos;		/*vertex position*/
layout (location=1) in vec4 color;		/*vertex color*/
layout (location=2) in vec4 normal;		/*vertex normal*/
layout (location=3) in vec4 uv;			/*vertex uv*/
layout (location=4) in vec4 tangent;	/*vertex tangent*/

uniform float iTime;

/*output variables*/
out vec3 vtx_pos;
out vec3 norm;
out vec2 uv2;

void main()
{
	float orbit_speed = -0.3;
	float theta = orbit_speed * iTime;
	mat3 rot_mat = mat3(vec3(cos(theta), 0.f, -sin(theta)), vec3(0.f, 1.f, 0.f), vec3(sin(theta), 0.f, cos(theta)));
	gl_Position=pvm*vec4(rot_mat * pos.xyz,1.f);
	vtx_pos = rot_mat * pos.xyz;
	norm = rot_mat * normal.xyz;
	uv2 = uv.xy;
}


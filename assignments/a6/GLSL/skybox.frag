//SKYBOX FRAG

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

/*uniform variables*/
uniform float iTime;					////time
uniform sampler2D tex;			////texture

in vec3 vtx_pos;
in vec3 norm;
in vec2 uv2;

out vec4 frag_color;

void main()
{
	// frag_color = vec4(norm * 0.5 + 0.5,1.f);
	vec2 tex_size = textureSize(tex, 0);
	frag_color = vec4(texture(tex, uv2).xyz, 1.f);
	// frag_color = vec4(vtx_pos, 1.f);
}

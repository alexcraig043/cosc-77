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
uniform sampler2D tex_caustic;	////texture

in vec3 vtx_pos;
in vec3 norm_vtx;
in vec2 uv_vtx;

out vec4 frag_color;

void main()
{
	vec3 tex_color = texture(tex, uv_vtx).xyz;

	vec2 caustic_uv_a = uv_vtx * 2 + vec2(0.0, -0.05 * iTime);
	vec3 caustic_color_a = texture(tex_caustic, caustic_uv_a).xyz;

	vec2 caustic_uv_b = uv_vtx * 1 + vec2(0.05 * iTime, 0.05 * iTime);
	vec3 caustic_color_b = texture(tex_caustic, caustic_uv_b).xyz;

	vec3 final_color = tex_color + 0.1 * caustic_color_a + 0.1 * caustic_color_b;
	frag_color = vec4(final_color, 1.f);
}

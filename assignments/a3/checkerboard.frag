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

/*input variables*/
//// TODO: declare the input fragment attributes here
in vec4 uv_vec;
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color;

void main()							
{						
	vec3 col = vec3(1.0);

	//// TODO: produce a checkerboard texture on the sphere with the input vertex uv
	float pi = 3.14159265359;
	
	float u = atan(vtx_pos.z, vtx_pos.x) / (2.0 * pi);
	float v = acos(vtx_pos.y) / pi;

	float u_floor = floor(10. * u);
	float v_floor = floor(6. * v);

	if(mod(u_floor + v_floor, 2.0) == 0.0) {
		col = vec3(1.0);
	} else {
		col = vec3(0.0);
	}

	frag_color = vec4(col, 1.0);
}	
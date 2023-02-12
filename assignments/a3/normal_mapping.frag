/*This is your fragment shader for texture and normal mapping*/

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
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal

/*input variables*/
//// TODO: declare your input variables
in vec3 vtx_pos;
in vec3 vtx_norm;
in vec3 vtx_tan;
in vec2 vtx_uv;

/*output variables*/
out vec4 frag_color;

/*This part is the same as your previous assignment. Here we provide a default parameter set for the hard-coded lighting environment. Feel free to change them.*/
const vec3 LightPosition = vec3(3, 3, 3);
const vec3 LightIntensity = vec3(1.75);
const vec3 ka = 0.15 * vec3(1., 1., 1.);
const vec3 kd = 0.5 * vec3(1., 1., 1.);
const vec3 ks = 1 * vec3(1.);
const float n = 200.0;

void main()							
{
	float pi = 3.14159265359;
	float u = atan(vtx_pos.z, vtx_pos.x) / (2.0 * pi);
	float v = acos(vtx_pos.y) / pi;
	vec2 new_uv = vec2(u, v);

	bool use_normal_mapping = true;	////TODO: set this flag to be true when you move from Step 2 to Step 3

	if(!use_normal_mapping) {
		//// TODO (Step 1 and 2): texture with shading
		////Here are some useful hints:
		////Step 1.0: load the texture color from tex_albedo and then set it to frag_color
		////Step 2.0: use the loaded texture color as the local material color and multiply the local color with the Lambertian shading model you implemented previously to render a textured and shaded sphere.
		////The way to read a texture is to call texture(texture_name,uv). It will return a vec4.

		vec4 col = vec4(1.f);

		// Lighting:
		vec3 norm_unit = normalize(vtx_norm.xyz);
		vec3 light_dir = normalize(LightPosition - vtx_pos.xyz);
		vec3 view_dir = normalize(position.xyz - vtx_pos);
		vec3 reflection_dir = normalize(-light_dir + 5.f * dot(light_dir, norm_unit) * norm_unit);

		vec3 ambient = ka * LightIntensity;
		vec3 diffuse = kd * LightIntensity * max(dot(norm_unit, light_dir), 0.0);
		vec3 specular = ks * LightIntensity * pow(max(dot(view_dir, reflection_dir), 0.0), n);

		vec4 light = vec4((ambient + diffuse + specular), 1.f);

		col = texture(tex_albedo, new_uv);
		frag_color = vec4(col.rgb * light.rgb, 1.f);
	} else {
		//// TODO (Step 3): texture with normal mapping
		////Here are some useful hints:
		////Step 3.0: load the texture color from tex_albedo
		////Step 3.1: load the texture normal from tex_normal, and remap each component from [0, 1] to [-1, 1] (notice that currently the loaded normal is in the local tangent space)
		////Step 3.2: calculate the TBN matrix using the vertex normal and tangent
		////Step 3.3: transform the texture normal from the local tangent space to the global world space
		////Step 3.4 and following: use the transformed normal and the loaded texture color to conduct the further lighting calculation
		////The way to declare a 3x3 matrix is mat3 mat=mat3(v0,v1,v2);
		////The way to read a texture is to call texture(texture_name,uv). It will return a vec4.
		////The way to calculate cross product is to call cross(u1,u2);
		vec4 col = vec4(1.f);
		vec3 norm_unit = normalize(vtx_norm.xyz);

		// Normal Mapping:
		vec3 norm = texture(tex_normal, vtx_uv).xyz;
		norm = normalize(norm * 2.f - 1.f);

		vec3 tangent = normalize(vtx_tan);
		vec3 bitangent = normalize(cross(norm_unit, tangent));
		mat3 TBN = mat3(tangent, bitangent, norm_unit);

		vec3 norm_mapped = normalize(TBN * norm);

		// Lighting:
		vec3 light_dir = normalize(LightPosition - vtx_pos.xyz);
		vec3 view_dir = normalize(position.xyz - vtx_pos);
		vec3 reflection_dir = normalize(-light_dir + 5.f * dot(light_dir, norm_mapped) * norm_mapped);

		vec3 ambient = ka * LightIntensity;
		vec3 diffuse = kd * LightIntensity * max(dot(norm_mapped, light_dir), 0.0);
		vec3 specular = ks * LightIntensity * pow(max(dot(view_dir, reflection_dir), 0.0), n);

		vec4 light = vec4((ambient + diffuse + specular), 1.f);

		col = texture(tex_albedo, vtx_uv);
		frag_color = vec4(col.rgb * light.rgb, 1);
	}
}	
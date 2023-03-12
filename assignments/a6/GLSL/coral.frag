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
uniform sampler2D tex_caustic;			////texture caustic

/*input variables*/
//// TODO: declare your input variables
in vec3 pos_vtx;
in vec3 norm_vtx;
in vec3 tan_vtx;
in vec2 uv_vtx;

/*output variables*/
out vec4 frag_color;

/*This part is the same as your previous assignment. Here we provide a default parameter set for the hard-coded lighting environment. Feel free to change them.*/
const vec3 LightPosition = vec3(-70, -70, 30);
const vec3 LightIntensity = vec3(1);
const vec3 ka = 0.1*vec3(1., 1., 1.);
const vec3 kd = 0.7*vec3(1., 1., 1.);
const vec3 ks = vec3(2.);
const float n = 400.0;

void main()							
{		
	bool use_normal_mapping = true;	////TODO: set this flag to be true when you move from Step 2 to Step 3

	if(!use_normal_mapping){
		//// TODO (Step 1 and 2): texture with shading
		////Here are some useful hints:
		////Step 1.0: load the texture color from tex_albedo and then set it to frag_color
		////Step 2.0: use the loaded texture color as the local material color and multiply the local color with the Lambertian shading model you implemented previously to render a textured and shaded sphere.
		////The way to read a texture is to call texture(texture_name,uv). It will return a vec4.

		
		float pi = 3.14159265359;
		vec4 col = vec4(1.f);

		// calculating the u and the v coordinates 
		float u = atan(pos_vtx.z, pos_vtx.x) / (2.0 * pi);
		float v = acos(pos_vtx.y) / pi;
		vec2 uv_n = vec2(u, v);
		// calculating the texture color
		col = texture(tex_albedo, uv_n);

		//Step 2:
		vec3 light_direction = normalize(LightPosition - pos_vtx.xyz);
		vec3 norm = normalize(norm_vtx.xyz);
		vec3 view = normalize(position.xyz - pos_vtx.xyz);
		vec3 reflection = normalize(-light_direction + 2.f * dot(light_direction, norm) * norm);

		vec3 amb_light = ka * LightIntensity;
		vec3 diff_light = kd * LightIntensity * max(dot(norm, light_direction), 0.0);
		vec3 spec_light = ks * LightIntensity * pow(max(dot(view, reflection), 0.0), n);

		vec4 all_light = vec4((amb_light + diff_light + spec_light), 1.f);

		frag_color = vec4(all_light.rgb * col.rgb, 1.f);

		//frag_color = vec4(col.rgb, 1.f);

		

	}
	else{
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
		// float pi = 3.14159265359;
		vec4 col = vec4(1.f);

		// Offset albedo to make wave distortion effect
		vec2 tex_uv = uv_vtx;
		tex_uv.x += sin(2.0 * uv_vtx.y + 0.1 * iTime) / 20.0;

		// texture color
		col = texture(tex_albedo, tex_uv);
		
		// normal mapping
		vec3 norm = texture(tex_normal, tex_uv).xyz;
		norm = normalize(norm * 2.f - 1.f);

		// Defining local coordinate system
		vec3 vtx_tangent = tan_vtx.xyz;
		vec3 vtx_normal = norm_vtx.xyz;
		vec3 vtx_bitangent = cross(vtx_normal, vtx_tangent);
		mat3 TBN = mat3(vtx_tangent, vtx_bitangent, vtx_normal);

		// lighting
		vec3 light_direction = normalize(LightPosition - pos_vtx.xyz);
		vec3 norm_mapped = normalize(TBN * norm);
		vec3 view = normalize(position.xyz - pos_vtx.xyz);
		vec3 reflection = normalize(-light_direction + 2.f * dot(light_direction, norm_mapped) * norm_mapped);

		vec3 amb_light = ka * LightIntensity;
		vec3 diff_light = kd * LightIntensity * max(dot(norm_mapped, light_direction), 0.0);
		vec3 spec_light = ks * LightIntensity * pow(max(dot(view, reflection), 0.0), n);

		
		vec4 all_light = vec4((amb_light + diff_light + spec_light), 1.f);

		// caustic
		vec2 caustic_uv_a = uv_vtx * 1 + vec2(0.0, -0.05 * iTime);
		vec3 caustic_color_a = texture(tex_caustic, caustic_uv_a).xyz;

		vec3 final_color = all_light.rgb * col.rgb + 0.25 * caustic_color_a;

		frag_color = vec4(final_color, 1.f);
	}
}	
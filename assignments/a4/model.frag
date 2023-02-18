#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

/*uniform variables*/
uniform float iTime;					////time

/*input variables*/
in vec3 vtx_pos;

/*input variables*/
out vec4 frag_color;

///////////// Part 1a /////////////////////
/* Create a function that takes in an xy coordinate and returns a 'random' 2d vector. (There is no right answer)
   Feel free to find a hash function online. Use the commented function to check your result */
vec2 hash2(vec2 v)
{
	vec2 rand = vec2(1,1);
	
	// Your implementation starts here

	// rand = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    // rand = -1.0 + 2 * 1.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);

	// Your implementation ends here
	rand =  vec2(sin(v.x * (cos(7869.876 * v.x + 1234.9867 * v.y) * sin(v.x * v.y * 123124.14 + 98798.675) * 12352.345)) , cos(v.y * (cos(23233.1242 * v.x + 23532.15232 * v.y) * sin(v.x * v.y * 8757.876) * 23453.231)));

	return rand;
}

///////////// Part 1b /////////////////////
/*  Using i, f, and m, compute the perlin noise at point v */
float perlin_noise(vec2 v) 
{
	float noise = 0.0;
	// Your implementation starts here
	vec2 i = floor(v);
	vec2 f = fract(v);

	// Compute the indices of the four neighboring grid points
	vec2 p0 = vec2(i.x, i.y);
	vec2 p1 = vec2(i.x + 1.0, i.y);
	vec2 p2 = vec2(i.x, i.y + 1.0);
	vec2 p3 = vec2(i.x + 1.0, i.y + 1.0);

	// Compute the offset between the point and the hashed grid points
	vec2 g0 = hash2(p0);
	vec2 g1 = hash2(p1);
	vec2 g2 = hash2(p2);
	vec2 g3 = hash2(p3);

	vec2 v0 = v - p0;
	vec2 v1 = v - p1;
	vec2 v2 = v - p2;
	vec2 v3 = v - p3;

	// Compute the dot product between the offset and the hashed grid points
	float m1 = mix(dot(v0, g0), dot(v1, g1), smoothstep(0.0, 1.0, f.x));
	float m2 = mix(dot(v2, g2), dot(v3, g3), smoothstep(0.0, 1.0, f.x));

	noise = mix(m1, m2, smoothstep(0.0, 1.0, f.y));	

	// Your implementation ends here
	return noise;
}

///////////// Part 1c /////////////////////
/*  Given a point v and an int num, compute the perlin noise octave for point v with octave num
	num will be greater than 0 */
float noiseOctave(vec2 v, int num)
{
	float sum = 0.0;
    float amplitude = 1.0;
	float frequency = 1.0;
	
    for (int i = 0; i < num; i++) {
        sum += amplitude * perlin_noise(frequency * v);
        frequency *= 2.0;
        amplitude *= 0.5;
    }

    return sum;
}

///////////// Part 2a /////////////////////
/* create a function that takes in a 2D point and returns a height using perlin noise 
    There is no right answer. Think about what functions will create what shapes.
    If you want steep mountains with flat tops, use a function like sqrt(noiseOctave(v,num)). 
    If you want jagged mountains, use a function like e^(noiseOctave(v,num))
    You can also add functions on top of each other and change the frequency of the noise
    by multiplying v by some value other than 1*/
float height(vec2 v){
    float h = 0;
	// Your implementation starts here
	h = 0.5 + 1.0 * (noiseOctave(vtx_pos.xy, 6));

	// Your implementation ends here
	return h;
}

///////////// Part 2b /////////////////////
/* compute the normal vector at v by find the points d to the left/right and d forward/backward 
    and using a cross product. Be sure to normalize the result */
vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	// Your implementation starts here

	vec3 v1 = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y)));
	vec3 v2 = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y)));
	vec3 v3 = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d)));
	vec3 v4 = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d)));

	normal_vector = normalize(cross(v1 - v2, v3 - v4));

	// Your implementation ends here
	return normal_vector;
}

///////////// Part 2c /////////////////////
/* complete the get_color function by setting emissiveColor using some function of height/normal vector/noise */
/* put your Phong/Lambertian lighting model here to synthesize the lighting effect onto the terrain*/
vec3 get_color(vec2 v)
{
	vec3 vtx_normal = compute_normal(v, 0.01);
	vec3 emissiveColor = vec3(0.5,0,0.5);
	vec3 lightingColor= vec3(1.,1.,1.);

	// Your implementation starts here
	float height = height(v);

	if (height < 0.3) { // Water
		emissiveColor = mix(vec3(0.1, 0.3, 0.6), vec3(0.2, 0.4, 0.8), height / 0.15);
	} else if (height < 0.4) { // Sand
		emissiveColor = mix(vec3(1.0, 0.7, 0.5), vec3(0.8, 0.7, 0.5), (height - 0.3) / 0.15);
	} else if (height < 0.5) { // Grass
		emissiveColor = mix(vec3(0.1, 0.4, 0.1), vec3(0.05, 0.5, 0.05), (height - 0.4) / 0.1);
	} else if (height < 0.95) { // Mountain
		emissiveColor = mix(vec3(0.05, 0.5, 0.05), vec3(.2, .4, .1), (height - 0.5) / 0.15);
	} else { // Snow
		emissiveColor = vec3(1.0, 1.0, 1.0);
	}

	// Make steep parts more brown
	emissiveColor = mix(emissiveColor, vec3(0.5, 0.3, 0.1), pow(abs(vtx_normal.y), 2.0));

	/*This part is the same as your previous assignment. Here we provide a default parameter set for the hard-coded lighting environment. Feel free to change them.*/
	const vec3 LightPosition = vec3(3, 1, 3);
	const vec3 LightIntensity = vec3(20);
	const vec3 ka = 0.1*vec3(1., 1., 1.);
	const vec3 kd = 0.7*vec3(1., 1., 1.);
	const vec3 ks = vec3(2.);
	const float n = 400.0;

	vec3 normal = normalize(vtx_normal.xyz);
	vec3 lightDir = LightPosition - vtx_pos;
	float _lightDist = length(lightDir);
	vec3 _lightDir = normalize(lightDir);
	vec3 _localLight = LightIntensity / (_lightDist * _lightDist);
	vec3 ambColor = ka;
	lightingColor = kd * _localLight * max(0., dot(_lightDir, normal));

	// Your implementation ends here

    return emissiveColor * lightingColor;

}

void main()
{
	frag_color = vec4(get_color(vtx_pos.xy),1.f);
}
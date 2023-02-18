#version 330 core

// Add parts 1a-c first, then implement part 2a

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
layout (location=0) in vec4 pos;
layout (location=2) in vec4 normal;
layout (location=3) in vec4 uv;
layout (location=4) in vec4 tangent;

/*output variables*/
out vec3 vtx_pos;		////vertex position in the world space

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
    You can also add functions on top of eachother and change the frequency of the noise
    by multiplying v by some value other than 1*/
float height(vec2 v){
    float h = 0;
	// Your implementation starts here
	h = 0.5 + 1.0 * (noiseOctave(pos.xy, 6));

	// Your implementation ends here
	return h;
}

void main()												
{
	vtx_pos = (vec4(pos.xy, height(pos.xy), 1)).xyz;
	gl_Position = pvm * vec4(vtx_pos,1);
}
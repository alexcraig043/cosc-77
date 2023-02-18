#version 330 core

// Parts 1a, 1b, 1c

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

in vec3 vtx_normal;
in vec3 vtx_pos;

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

void main()
{
	vec3 color = 0.5 + 0.5 * (noiseOctave(vtx_pos.xy, 1))  * vec3(1,1,1); // visualize perlin noise
	frag_color=vec4(color,1.f);
}
// //#####################################################################
// // Main
// // Dartmouth COSC 77/177 Computer Graphics, technical project starter code
// //#####################################################################
// #include <iostream>

// #include "Common.h"
// #include "Driver.h"
// #include "Particles.h"
// #include "OpenGLMesh.h"
// #include "OpenGLCommon.h"
// #include "OpenGLWindow.h"
// #include "OpenGLViewer.h"
// #include "OpenGLMarkerObjects.h"
// #include "OpenGLParticles.h"
// #include "TinyObjLoader.h"

// class TechProjDriver : public Driver, public OpenGLViewer
// {
// 	using Base = Driver;
// public:
// 	Array<TriangleMesh<3>* > triangle_meshes;

// 	virtual void Initialize()
// 	{
// 		OpenGLViewer::Initialize();
// 	}

// 	virtual void Run()
// 	{
// 		OpenGLViewer::Run();
// 	}

// 	////Initialize data in the scene
// 	virtual void Initialize_Data()
// 	{
// 		Init_Shaders();
// 		Init_Textures();
// 		Init_Background();

// 		Init_Bunny_Mesh();
// 		Init_Plane_Mesh();
// 		Init_Segment_Mesh();
// 		Init_Sphere_Mesh();

// 		Init_Lighting();
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Specifying your own lighting effects in this function
// 	////In this example we initialized two directional lights, the same as Assignment 2
// 	void Init_Lighting() {
// 		auto dir_light = OpenGLUbos::Add_Directional_Light(glm::vec3(-1.f, -1.f, -1.f));
// 		dir_light->dif = glm::vec4(.9,.8,.7, 1.0);

// 		dir_light = OpenGLUbos::Add_Directional_Light(glm::vec3(1.f, -1.f, -1.f));
// 		dir_light->dif = glm::vec4(.7,.7,.7, 1.0);

// 		OpenGLUbos::Set_Ambient(glm::vec4(.01f, .01f, .02f, 1.f));
// 		OpenGLUbos::Update_Lights_Ubo();	
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Write your own vertex shader and fragment shader and add it to the shader library
// 	void Init_Shaders()
// 	{
// 		Add_Shader("shaders/background.vert","shaders/background.frag","background");
// 		Add_Shader("shaders/shared.vert", "shaders/lamb_textured.frag", "lamb_tex");
// 		Add_Shader("shaders/shared.vert", "shaders/lamb.frag", "lamb");
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Read a texture from a image and add it to the texture library
// 	void Init_Textures()
// 	{
// 		Add_Texture("models/bunny.jpg","bunny");
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Initialize the background shader
// 	void Init_Background()
// 	{
// 		OpenGLBackground* opengl_background=Add_Interactive_Object<OpenGLBackground>();
// 		opengl_background->shader_name="background";
// 		opengl_background->Initialize();
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Init a mesh and bind shader and texture
// 	////To make your own mesh, copy this function and modify the variables
// 	void Init_Bunny_Mesh() 
// 	{
// 		////Initialize the mesh file, shader, and texture of the mesh
// 		std::string mesh_file_name = "models/bunny.obj";
// 		std::string shader_name = "lamb_tex";
// 		std::string texture_name = "bunny";

// 		////Read mesh from obj file
// 		OpenGLTriangleMesh* opengl_tri_mesh = Add_Interactive_Object<OpenGLTriangleMesh>();
// 		Read_Mesh(mesh_file_name, opengl_tri_mesh->mesh);
// 		Rescale(opengl_tri_mesh->mesh.Vertices(),1.);
// 		Translate_Center_To(opengl_tri_mesh->mesh.Vertices(),Vector3::Ones()*.5);

// 		////Initialize the model matrix
// 		opengl_tri_mesh->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

// 		////Other mesh initialization setups
// 		Set_Mesh_Default_Options(opengl_tri_mesh);

// 		////Bind an initialized shader to the mesh
// 		opengl_tri_mesh->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader(shader_name));

// 		////Bind a texture to the mesh
// 		opengl_tri_mesh->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture(texture_name));

// 		////Add the triangle mesh to the array to access the mesh later
// 		TriangleMesh<3>* triangle_mesh=&opengl_tri_mesh->mesh;
// 		triangle_meshes.push_back(triangle_mesh);
// 	}

// 	void Init_Plane_Mesh()
// 	{
// 		////Initialize the mesh file, shader, and texture of the mesh
// 		std::string shader_name = "lamb";

// 		OpenGLTriangleMesh* opengl_tri_mesh = Add_Interactive_Object<OpenGLTriangleMesh>();
// 		////Create a mesh with vertices on a 5x5 lattice
// 		Create_Plane_Mesh(5,5,1.,&opengl_tri_mesh->mesh,0,2);
// 		Translate_Center_To(opengl_tri_mesh->mesh.Vertices(),Vector3::Zero());

// 		////Initialize the model matrix
// 		opengl_tri_mesh->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

// 		////Other mesh initialization setups
// 		Set_Mesh_Default_Options(opengl_tri_mesh);

// 		////Bind an initialized shader to the mesh
// 		opengl_tri_mesh->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader(shader_name));

// 		////Add the triangle mesh to the array to access the mesh later
// 		TriangleMesh<3>* triangle_mesh=&opengl_tri_mesh->mesh;
// 		triangle_meshes.push_back(triangle_mesh);		
// 	}

// 	void Init_Segment_Mesh()
// 	{
// 		OpenGLSegmentMesh* opengl_seg_mesh = Add_Interactive_Object<OpenGLSegmentMesh>();
// 		////Create a mesh with vertices on a 5x5 lattice
// 		SegmentMesh<3>* segment_mesh=&opengl_seg_mesh->mesh;
// 		std::vector<Vector3>& vertices=segment_mesh->Vertices();
// 		std::vector<Vector2i>& segments=segment_mesh->Elements();
// 		vertices.push_back(Vector3::Zero());
// 		vertices.push_back(Vector3::Unit(1));
// 		vertices.push_back(Vector3(-1,2,0));
// 		vertices.push_back(Vector3(1,2,0));
// 		segments.push_back(Vector2i(0,1));
// 		segments.push_back(Vector2i(1,2));
// 		segments.push_back(Vector2i(1,3));

// 		////Other mesh initialization setups
// 		opengl_seg_mesh->color=OpenGLColor(1,1,0,1);
// 		opengl_seg_mesh->line_width=2.f;

// 		opengl_seg_mesh->Set_Data_Refreshed();
// 		opengl_seg_mesh->Initialize();

// 		////OpenGLSegmentMesh has a default shader, so you don't need to bind one in the driver
// 	}

// 	void Init_Sphere_Mesh()
// 	{
// 		////Initialize the mesh file, shader, and texture of the mesh
// 		std::string shader_name = "lamb";

// 		OpenGLTriangleMesh* opengl_tri_mesh = Add_Interactive_Object<OpenGLTriangleMesh>();
// 		Create_Sphere_Mesh(0.5,&opengl_tri_mesh->mesh,4);
// 		Translate_Center_To(opengl_tri_mesh->mesh.Vertices(),Vector3::Zero());

// 		////Initialize the model matrix
// 		opengl_tri_mesh->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1, .5, 0));

// 		////Other mesh initialization setups
// 		Set_Mesh_Default_Options(opengl_tri_mesh);

// 		////Bind an initialized shader to the mesh
// 		opengl_tri_mesh->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader(shader_name));

// 		////Add the triangle mesh to the array to access the mesh later
// 		TriangleMesh<3>* triangle_mesh=&opengl_tri_mesh->mesh;
// 		triangle_meshes.push_back(triangle_mesh);		
// 	}

// 	void Sync_Simulation_And_Visualization_Data()
// 	{
// 		////if you want to animate your meshes or objects, implement this function
		
// 	}

// 	////update simulation and visualization for each time step
// 	virtual void Toggle_Next_Frame()
// 	{
// 		Sync_Simulation_And_Visualization_Data();
// 		OpenGLViewer::Toggle_Next_Frame();
// 	}

// 	////Keyboard interaction
// 	virtual void Initialize_Common_Callback_Keys()
// 	{
// 		OpenGLViewer::Initialize_Common_Callback_Keys();
// 		Bind_Callback_Key('a',&Keyboard_Event_A_Func,"press A");
// 	}

// 	////Implement your own keyboard function, 
// 	virtual void Keyboard_Event_A()
// 	{
// 		std::cout<<"A pressed"<<std::endl;
// 	}
// 	Define_Function_Object(TechProjDriver,Keyboard_Event_A);

// protected:
// 	void Set_Mesh_Default_Options(OpenGLTriangleMesh* opengl_tri_mesh)
// 	{
// 		Set_Polygon_Mode(opengl_tri_mesh, PolygonMode::Fill);
// 		Set_Shading_Mode(opengl_tri_mesh, ShadingMode::Custom);
// 		opengl_tri_mesh->Set_Data_Refreshed();
// 		opengl_tri_mesh->Initialize();
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Add a shader to the shader library
// 	void Add_Shader(const std::string vtx_shader_file_name,const std::string frag_shader_file_name,const std::string shader_name)
// 	{
// 		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vtx_shader_file_name,frag_shader_file_name,shader_name);
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Add a texture to the texture library
// 	void Add_Texture(const std::string texture_file_name,const std::string texture_name)
// 	{
// 		OpenGLTextureLibrary::Instance()->Add_Texture_From_File(texture_file_name, texture_name);
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Read an obj mesh
// 	void Read_Mesh(const std::string mesh_file_name,TriangleMesh<3>& read_mesh)
// 	{
// 		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
// 		Obj::Read_From_Obj_File(mesh_file_name, meshes);
// 		read_mesh=*meshes[0];
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Initialize a plane mesh
// 	void Create_Plane_Mesh(const int m,const int n,const real dx,TriangleMesh<3>* mesh,int axis_0=0,int axis_1=1)
// 	{
// 		mesh->elements.resize(2*(m-1)*(n-1));int t=0;
// 		for(int i=1;i<=m-1;i++)for(int j=1;j<=n-1;j++){ // counterclockwise node ordering
// 			if(i%2){mesh->elements[t++]=Vector3i(i+m*(j-1),i+1+m*(j-1),i+m*j);mesh->elements[t++]=Vector3i(i+1+m*(j-1),i+1+m*j,i+m*j);}
// 			else{mesh->elements[t++]=Vector3i(i+m*(j-1),i+1+m*(j-1),i+1+m*j);mesh->elements[t++]=Vector3i(i+m*(j-1),i+1+m*j,i+m*j);}}
// 		for(size_type i=0;i<mesh->elements.size();i++){mesh->elements[i]-=Vector3i::Ones();
// 		/*swap y and z*/int tmp=mesh->elements[i][1];mesh->elements[i][1]=mesh->elements[i][2];mesh->elements[i][2]=tmp;}
// 		for(int j=0;j<n;j++)for(int i=0;i<m;i++){Vector3 pos=Vector3::Zero();pos[axis_0]=(real)i*dx;pos[axis_1]=(real)j*dx;mesh->Vertices().push_back(pos);}
// 	}
	
// 	//////////////////////////////////////////////////////////////////////////
// 	////Calculate the bounding box of a set of vertices
// 	void Bounding_Box(const Array<Vector3>& vertices,Vector3& min_corner,Vector3& max_corner)
// 	{
// 		min_corner=Vector3::Ones()*(1e7);max_corner=Vector3::Ones()*(-1e7);
// 		for(auto& v:vertices){
// 			min_corner=min_corner.cwiseMin(v);
// 			max_corner=max_corner.cwiseMax(v);}
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Rescale the points to a box with longest_length
// 	void Rescale(Array<Vector3>& vertices,const real longest_length)
// 	{
// 		Vector3 min_corner,max_corner;
// 		Bounding_Box(vertices,min_corner,max_corner);
// 		Vector3 length=max_corner-min_corner;
// 		int axis=0;if(length[1]>length[axis])axis=1;if(length[2]>length[axis])axis=2;
// 		real rescale=(length[axis]>(real)0)?longest_length/length[axis]:(real)1;for(auto& v:vertices)v*=rescale;
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Calculate the center
// 	Vector3 Center(const Array<Vector3>& vertices)
// 	{
// 		Vector3 sum=Vector3::Zero();for(auto& v:vertices)sum+=v;
// 		return sum/=(real)vertices.size();
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Translate the center to the target position
// 	void Translate_Center_To(Array<Vector3>& vertices,const Vector3 target)
// 	{
// 		Vector3 center=Center(vertices);
// 		for(auto& v:vertices)v+=(target-center);
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	////Initialize a sphere mesh
// 	void Create_Sphere_Mesh(const real r,/*rst*/TriangleMesh<3>* mesh,const int sub)
// 	{
// 		Initialize_Icosahedron_Mesh(r,mesh);for(int i=0;i<sub;i++)Subdivide(mesh);
// 		for(auto& v:mesh->Vertices()){real length=v.norm();real rs=r/length;v*=rs;}
// 	}

// 	void Initialize_Icosahedron_Mesh(const real scale,/*rst*/TriangleMesh<3>* mesh)
// 	{
// 		////http://donhavey.com/blog/tutorials/tutorial-3-the-icosahedron-sphere/
// 		const real tao=1.61803399f;
// 		real vtx_pos[12][3]={{1,tao,0},{-1,tao,0},{1,-tao,0},{-1,-tao,0},{0,1,tao},{0,-1,tao},{0,1,-tao},{0,-1,-tao},{tao,0,1},{-tao,0,1},{tao,0,-1},{-tao,0,-1}};
// 		int ele[20][3]={{0,1,4},{1,9,4},{4,9,5},{5,9,3},{2,3,7},{3,2,5},{7,10,2},{0,8,10},{0,4,8},{8,2,10},{8,4,5},{8,5,2},{1,0,6},{11,1,6},{3,9,11},{6,10,7},{3,11,7},{11,6,7},{6,0,10},{9,1,11}};		

// 		mesh->Clear();
// 		int vtx_num=12;mesh->Vertices().resize(vtx_num);for(int i=0;i<vtx_num;i++){mesh->Vertices()[i]=Vector3(vtx_pos[i][0],vtx_pos[i][1],vtx_pos[i][2])*scale;}
// 		int ele_num=20;mesh->elements.resize(ele_num);for(int i=0;i<ele_num;i++)mesh->elements[i]=Vector3i(ele[i][0],ele[i][1],ele[i][2]);
// 	}

// 	void Subdivide(TriangleMesh<3>* mesh)
// 	{
// 		Hashtable<Vector2i,int> edge_vtx_hashtable;
// 		for(int k=0;k<mesh->elements.size();k++){
// 			for(int j=0;j<3;j++){
// 				Vector2i e(mesh->elements[k][j%3],mesh->elements[k][(j+1)%3]);
// 				if(e[0]<e[1])continue;
// 				Vector3 pos=(real).5*(mesh->Vertices()[e[0]]+mesh->Vertices()[e[1]]);
// 				mesh->Vertices().push_back(pos);
// 				int i=(int)mesh->Vertices().size()-1;
// 				edge_vtx_hashtable.insert(std::make_pair(e,i));
// 			}
// 		}

// 		auto n=mesh->elements.size();
// 		for(auto i=0;i<n;i++){const Vector3i v=mesh->elements[i];int v3,v4,v5;
// 			{auto search=edge_vtx_hashtable.find(Sorted(Vector2i(v[0],v[1])));if(search==edge_vtx_hashtable.end())continue;v3=search->second;}
// 			{auto search=edge_vtx_hashtable.find(Sorted(Vector2i(v[1],v[2])));if(search==edge_vtx_hashtable.end())continue;v4=search->second;}
// 			{auto search=edge_vtx_hashtable.find(Sorted(Vector2i(v[2],v[0])));if(search==edge_vtx_hashtable.end())continue;v5=search->second;}
// 			mesh->elements.push_back(Vector3i(v[0],v3,v5));
// 			mesh->elements.push_back(Vector3i(v3,v[1],v4));
// 			mesh->elements.push_back(Vector3i(v5,v4,v[2]));
// 			mesh->elements[i]=Vector3i(v3,v4,v5);}
// 	}
// };

// int main(int argc,char* argv[])
// {
// 	int driver=1;

// 	switch(driver){
// 	case 1:{
// 		TechProjDriver driver;
// 		driver.Initialize();
// 		driver.Run();	
// 	}break;
// 	}
// }

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>		//For sin, cos...
#include <vector>

#include <random>
std::mt19937_64 eng;  // a core engine class 
std::uniform_real_distribution<float> pi(0,M_PI);   
std::uniform_real_distribution<float> dis(-1,1);
std::uniform_real_distribution<float> dis2(-0.01,0.01);


#include <thread>
#include <mutex>
//a global instance of std::mutex to protect global variable
std::mutex myMutex;

#define GLM_ENABLE_EXPERIMENTAL

//GLM
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/string_cast.hpp> //to string

//include own Shader
// #include "Shader.h"

#include "./tech_proj/Boid.h"

using namespace std;

//--------------------------------------
// CONFIG
//-------------------------------------
const int WIDTH = 1280, HEIGHT = 720; // initial size of window
double zZoom = -20; //initial zoom

const int AMOUNT_OF_BOIDS = 250;
const int GENERATION_RADIUS_FOR_BOIDS = 400;

const int PERCEPTION_RADIUS_COHESION 		= 20;
const int PERCEPTION_RADIUS_ALIGNMENT 	= 20;
const int PERCEPTION_RADIUS_SEPARATION 	= 10;

const float WEIGHT_FAKTOR_RADIUS_TOCENTER_FORCE = 25.0f;

const float MAX_FORCE_MAGNITUDE = 0.01;
const float MAX_SPEED = 0.5;

//--------------------------------------



vector<Boid> BOIDS; //vector with boid objects
bool RUNNING = true; //To stop threads later (clean after yourself)

glm::mat4 *setupBoids(int amount , float radius_max){
	/*! Generate model matrices and boids
	Function that generates a number of boids in a sphere with a 
	specific radius.

	@param amount Number of boids to create
	@param radius_max The maximum radius at which the boids get generated  
	*/

	float x, y, z, rotAngle;
	glm::vec3 position;
	glm::vec3 rotVec;
	glm::mat4* modelMatrices = new glm::mat4[amount];


	cout << "Generating positions for Boids: " << endl;
	for (int i = 0; i < amount; ++i){
		//Generating random points inside a sphere with "kugelkoordinaten"
		glm::mat4 model(1.0f);
		do{
			x = dis(eng) * radius_max;
			y = dis(eng) * radius_max;
			z = dis(eng) * radius_max;
		}while(x*x + y*y + z*z > radius_max);
		position = glm::vec3(x,y,z);

		cout << x << " : "<< y<<" : "<<z <<endl;
		//Adding points to model matrix
		model = glm::translate(model, position);

		//Generate random angle and rotate by random vec3
		rotVec = glm::vec3(dis2(eng),dis2(eng),dis2(eng));


		Boid my_boid(position, rotVec); //(pos,vel)
		BOIDS.push_back(my_boid); 
		BOIDS[i].setModelMatrix(model);

	}
	return modelMatrices;
} 



void timestep(double function_call_delay){
	double startTime = clock()/(CLOCKS_PER_SEC/1000); //Start time in ms
	while(RUNNING){
		while( (clock()/(CLOCKS_PER_SEC/1000)) - startTime > function_call_delay){

			//-------------------------------------
			// Apply Force
			//-------------------------------------
			for (int i = 0; i < BOIDS.size(); ++i){
				BOIDS[i].applyforce(BOIDS[i].force_cohesion());
				BOIDS[i].applyforce(BOIDS[i].force_alignment());
				BOIDS[i].applyforce(BOIDS[i].force_separation());
				BOIDS[i].applyforce(BOIDS[i].force_toCenter());
			}

			//-------------------------------------
			// Do timestep !
			//-------------------------------------
			std::lock_guard<std::mutex> guard(myMutex); // the access to this function is mutually exclusive
			for (int i = 0; i < BOIDS.size(); ++i){		
				BOIDS[i].timestep(0.9f);
				BOIDS[i].resetforce(); /*After the timestep is calculated reset force (acceleration)*/
			}

			startTime = clock()/(CLOCKS_PER_SEC/1000); //Reset time
		}
	}
}


int main(void){
	GLFWwindow* window;
	// glfwSetErrorCallback(error_callback);
	/* Initialize the library */
	if (!glfwInit()){
		printf("Glfw could not be initialized!\n");
		exit(EXIT_FAILURE);
	}

	//Set OpenGl to use version 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Boids", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Add GLFW Callbacks */
	// glfwSetKeyCallback(					window, key_callback); //Add (Escape) key handler
	// glfwSetCursorPosCallback(		window, cursor_callback); //Add cursor callback
	// glfwSetCursorEnterCallback(	window, cursorEnter_callback);
	// glfwSetMouseButtonCallback(	window, mouseButton_callback);
	// glfwSetScrollCallback(			window, scroll_callback);

	glfwSetInputMode(						window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize OpenGL context" << std::endl;
	    return -1;
	}
	gladLoadGL();

	cout << "OpenGL shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	glEnable(GL_DEBUG_OUTPUT);
	// glDebugMessageCallback( MessageCallback, 0 );

	///////////////////////////////////////////////////////////
	//Setup GL Vertecies
	//////////////////////////////////////////////////////////
	//enable alpha support
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Build and compile our shader program
	// Shader ourShader( "./res/shaders/core.vs", "./res/shaders/core.frag" );
	
	GLfloat vertices2[] =
	{
	// Positions         // Colors					
	 0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   //Bottom Layer
	 1.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   //Bottom Layer
	 0.0f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   //Bottom Layer

	 0.0f,  0.0f,  0.5f,	 0.0f, 1.0f, 0.0f,   //Side 1
	 0.0f,  0.5f,  0.0f, 	 0.0f, 1.0f, 0.0f,
	 1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,

	 0.0f,  0.0f,  0.5f,	 0.0f, 0.0f, 1.0f,   //Side 2
	 0.0f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,
	 1.0f,  0.0f,  0.0f,   0.0f, 0.0f, 1.0f,

	 0.0f,  0.5f,  0.0f,	 1.0f, 1.0f, 0.0f,   //Side 3
	 0.0f, -0.5f,  0.0f,   1.0f, 1.0f, 0.0f,
	 0.0f,  0.0f,  0.5f,   1.0f, 1.0f, 0.0f,

	};

	GLuint VBO, VAO;
	glGenVertexArrays( 1, &VAO);
	glGenBuffers( 1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointe
	glBindVertexArray( VAO );

	//Bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices2 ), vertices2, GL_STATIC_DRAW );

	//Position attribute
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) 0 );
	glEnableVertexAttribArray( 0 );
	//Color attribute
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) ( 3 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );	

	glBindVertexArray( 0 ); // Unbind VAO (it's always a good thing to unbind any buffer/array //to prevent strange bugs)
	glEnable(GL_DEPTH_TEST);  

	//Init rng
	srand (time(NULL));


	//-----------------------------------------------------------------------------
	// Setup Boids
	//-----------------------------------------------------------------------------
	glm::mat4* modelMatrices;
	modelMatrices = setupBoids(AMOUNT_OF_BOIDS, GENERATION_RADIUS_FOR_BOIDS);


	//--------------------------------------
	// Timestep function threaded (async & timed)
	//--------------------------------------
	std::thread t1(timestep, 20);




	/*FPS counter*/
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//------------------------------------------
		//*fps*
  	//------------------------------------------
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 5.0 ){ // If last prinf() was more than 5 sec ago
			// printf and reset timer
			cout << 5000.0/double(nbFrames) <<"ms/frame (" << nbFrames/5 << " fps)" << endl;
			nbFrames = 0;
			lastTime += 5.0;
		}
		

		/* Set Viewport to window size */
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);


		/* clear color and depth buffer */ 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Backgroundcolor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use our Shader (Shader.h)
		// ourShader.Use();

		// Set projections matrix
		glm::mat4 projection = glm::frustum(-1.0, 1.0, -1.0, 1.0, 1.1, 2000.0); //set identity
		// ourShader.setMat4( "projection", projection); //Push into gpu

		//------------------------------------------
		// Handle Camera callback (src/callback.cpp)
		//------------------------------------------
		glm::mat4 camera(1.0f); //Set uniform

		camera = glm::translate( camera, glm::vec3(0.0f, 0.0f, zZoom));	//from callback
		// camera = glm::rotate( camera, (float)yRot, glm::vec3( 0.05f, 0.0f, 0.0f ) ); //from callback
		// camera = glm::rotate( camera, (float)xRot, glm::vec3( 0.0f, 0.05f, 0.0f ) ); //from callback

		// ourShader.setMat4( "camera", camera); //Push into gpu



		//-------------------------------------
		// Draw containers (Boids)
		//-------------------------------------
		for (int i = 0; i < AMOUNT_OF_BOIDS; ++i)
		{
			//cout << glm::to_string(modelMatrices[i]);
			std::lock_guard<std::mutex> guard(myMutex);
			// ourShader.setMat4("model",BOIDS[i].getModelMatrix());
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 12);
			glBindVertexArray(0);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	RUNNING = false; //To stop all other threads
	glfwTerminate();
	return 0;


}


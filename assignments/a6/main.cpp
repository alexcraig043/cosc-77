
//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Common.h"
#include "Driver.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "TinyObjLoader.h"
// #include  <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include  <glm/glm.hpp>
// #include <glm/vec3.hpp> // glm::vec3
// #include <glm/vec4.hpp> // glm::vec4
// #include <glm/mat4x4.hpp> // glm::mat4
// #include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
// #include <glm/ext/matrix_clip_space.hpp> // glm::perspective
// #include <glm/ext/scalar_constants.hpp> // glm::pi




#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class FinalProjectDriver : public Driver, public OpenGLViewer
{using Base=Driver;
	std::vector<OpenGLTriangleMesh*> mesh_object_array;						////mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
		draw_bk=false;						////turn off the default background and use the customized one
		draw_axes=false;						////if you don't like the axes, turn them off!
		startTime=clock();
		OpenGLViewer::Initialize();
	}

	void Add_Shaders()
	{
		////format: vertex shader name, fragment shader name, shader name
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/background.vert","./GLSL/background.frag","background");	

		////SHADOW TODO: uncomment next three lines to import shadow shaders
		//OpenGLShaderLibrary::Instance()->Add_Shader_From_File("object_1_shadow.vert","./GLSL/object_1_shadow.frag","object_1_shadow");	
		//OpenGLShaderLibrary::Instance()->Add_Shader_From_File("object_2_shadow.vert","./GLSL/object_2_shadow.frag","object_2_shadow");	
		//OpenGLShaderLibrary::Instance()->Add_Shader_From_File("object_3_shadow.vert","./GLSL/object_3_shadow.frag","object_3_shadow");	

		////SHADOW TODO: comment out next three lines
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/object_1.vert","./GLSL/object_1.frag","object_1");	
		// OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/object_2.vert","./GLSL/object_2.frag","object_2");	
		// OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/object_3.vert","./GLSL/object_3.frag","object_3");	

		// skybox
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/skybox.vert","./GLSL/skybox.frag","skybox");
		//fish
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/fish.vert","./GLSL/fish.frag","fish");
		// coral
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("./GLSL/coral.vert","./GLSL/coral.frag","coral");


	}

	void Add_Textures()
	{
		////format: image name, texture name
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/earth_albedo.png", "object_1_albedo");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/earth_normal.png", "object_1_normal");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/earth_albedo.png", "object_2_albedo");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/earth_normal.png", "object_2_normal");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/earth_albedo.png", "object_3_albedo");		
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/earth_normal.png", "object_3_normal");

		// my textures
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/uw_back.jpg", "back");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/uw_front.jpg", "front");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/uw_bottom.jpg", "bottom");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/uw_up.jpg", "up");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/uw_left.jpg", "left");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/uw_right.jpg", "right");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/starfish_albedo.jpg", "starfish_albedo");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/starfish_normal.jpg", "starfish_normal");
		// OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/fish_albedo.jpg", "fish_albedo");
		// OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/fish_normal.jpg", "fish_normal");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/fish.jpg", "fish_albedo");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/fish.jpg", "fish_normal");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/coral.jpg", "coral_albedo");
		OpenGLTextureLibrary::Instance()->Add_Texture_From_File("./assets/coral.jpg", "coral_normal");


	}
	void Scale_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,float scale)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			v*=scale;
		}
	}

	void Translate_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,const Vector3& translate)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			v+=translate;
		}
	}

	// //rotate the vertices of an object by a given angle
	// void Rotate_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,float angle)
	// {
	// 	std::vector<Vector3>& vertices=obj->mesh.Vertices();		
	// 	for(auto& v:vertices){
	// 		v=Rotation<3>(angle,Vector3(0,1,0))*v;
	// 	}
	// }

	void Rotate_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj, float angle)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			//float cos1 = cos(angle) * v.getX();
			// v = v[0];
			// v*= cos(angle);

			v = Vector3((cos(angle) * v[0] - sin(angle) * v[1]), (sin(angle) * v[0] + cos(angle) * v[1]), v[2]);
		}
	}

	void Add_Background()
	{
		OpenGLBackground* opengl_background=Add_Interactive_Object<OpenGLBackground>();
		opengl_background->shader_name="background";
		opengl_background->Initialize();
	}

	////this is an example of adding a mesh object read from obj file
	int Add_Object_1()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		////read mesh file
		std::string obj_file_name="./assets/starfish.obj";
		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		

		////This is an example showing how to access and modify the values of vertices on the CPU end.
		//std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		//int vn=(int)vertices.size();
		//for(int i=0;i<vn;i++){
		//	vertices[i]+=Vector3(1.,0.,0.);
		//}

		////This is an example of creating a 4x4 matrix for GLSL shaders. Notice that the matrix is column-major (instead of row-major!)
		////The code for passing the matrix to the shader is in OpenGLMesh.h
		mesh_obj->model_matrix=
			glm::mat4(1.f,0.f,0.f,0.f,		////column 0
					  0.f,1.f,0.f,0.f,		////column 1
					  0.f,0.f,1.f,0.f,		////column 2
					  0.f,1.f,0.f,1.f);		////column 3	////set the translation in the last column

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_1_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_1"));
		
		////set up texture
		mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("starfish_albedo"));
		mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("starfish_normal"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: set Shading Mode to Shadow
		
		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();	
		mesh_object_array.push_back(mesh_obj);
		int obj_idx = (int)mesh_object_array.size()-1;
		auto obj=mesh_object_array[obj_idx];
		Scale_Vertex_Position_For_Mesh_Object(obj,0.08f);
		Vector3 trans = Vector3(1.f, -9.f, -1.f);
		Translate_Vertex_Position_For_Mesh_Object(obj,trans);
		Rotate_Vertex_Position_For_Mesh_Object(obj, 45.f);
		return obj_idx;
	}

	////this is an example of adding a mesh object read from obj file
	int Add_Fish()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		////read mesh file
		std::string obj_file_name="./assets/fish.obj";
		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		

		////This is an example showing how to access and modify the values of vertices on the CPU end.
		//std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		//int vn=(int)vertices.size();
		//for(int i=0;i<vn;i++){
		//	vertices[i]+=Vector3(1.,0.,0.);
		//}

		////This is an example of creating a 4x4 matrix for GLSL shaders. Notice that the matrix is column-major (instead of row-major!)
		////The code for passing the matrix to the shader is in OpenGLMesh.h
		mesh_obj->model_matrix=
			glm::mat4(1.f,0.f,0.f,0.f,		////column 0
					  0.f,1.f,0.f,0.f,		////column 1
					  0.f,0.f,1.f,0.f,		////column 2
					  0.f,1.f,0.f,1.f);		////column 3	////set the translation in the last column

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_1_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("fish"));
		
		////set up texture
		mesh_obj->Add_Texture("fish_albedo", OpenGLTextureLibrary::Get_Texture("fish_albedo"));
		mesh_obj->Add_Texture("fish_normal", OpenGLTextureLibrary::Get_Texture("fish_normal"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: set Shading Mode to Shadow
		
		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();	
		mesh_object_array.push_back(mesh_obj);
		int obj_idx = (int)mesh_object_array.size()-1;
		auto obj=mesh_object_array[obj_idx];
		Scale_Vertex_Position_For_Mesh_Object(obj,0.1f);
		Vector3 trans = Vector3(1.f, -1.f, -1.f);
		Translate_Vertex_Position_For_Mesh_Object(obj,trans);
		Rotate_Vertex_Position_For_Mesh_Object(obj, 45.f);
		return obj_idx;
	}

	////this is an example of adding a mesh object read from obj file
	int Add_Coral()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		////read mesh file
		std::string obj_file_name="./assets/coral.obj";
		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		

		////This is an example showing how to access and modify the values of vertices on the CPU end.
		//std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		//int vn=(int)vertices.size();
		//for(int i=0;i<vn;i++){
		//	vertices[i]+=Vector3(1.,0.,0.);
		//}

		////This is an example of creating a 4x4 matrix for GLSL shaders. Notice that the matrix is column-major (instead of row-major!)
		////The code for passing the matrix to the shader is in OpenGLMesh.h
		mesh_obj->model_matrix=
			glm::mat4(cos(90),sin(90),0.f,0.f,		////column 0
					  -sin(90),cos(90),0.f,0.f,		////column 1
					  0.f,0.f,1.f,0.f,		////column 2
					  0.f,1.f,0.f,1.f);		////column 3	////set the translation in the last column

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_1_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("coral"));
		
		////set up texture
		mesh_obj->Add_Texture("coral_albedo", OpenGLTextureLibrary::Get_Texture("coral_albedo"));
		mesh_obj->Add_Texture("coral_normal", OpenGLTextureLibrary::Get_Texture("coral_normal"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: set Shading Mode to Shadow
		
		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();	
		mesh_object_array.push_back(mesh_obj);
		int obj_idx = (int)mesh_object_array.size()-1;
		auto obj=mesh_object_array[obj_idx];
		Scale_Vertex_Position_For_Mesh_Object(obj,0.05f);
		Vector3 trans = Vector3(3.f, -8.f, -1.f);
		Translate_Vertex_Position_For_Mesh_Object(obj,trans);
		//Rotate_Vertex_Position_For_Mesh_Object(obj, 45.f);
		return obj_idx;
	}

	

	// ////this is an example of adding a spherical mesh object generated analytically
	// int Add_Object_2()
	// {
	// 	auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

	// 	real radius=1.;
	// 	Initialize_Sphere_Mesh(radius,&mesh_obj->mesh,3);		////add a sphere with radius=1. if the obj file name is not specified
		
	// 	////set up shader
	// 	//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_2_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
	// 	mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_2"));
		
	// 	////set up texture
	// 	mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("object_2_albedo"));
	// 	mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("object_2_normal"));
		
	// 	Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
	// 	Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow
		
	// 	////initialize
	// 	mesh_obj->Set_Data_Refreshed();
	// 	mesh_obj->Initialize();	
	// 	mesh_object_array.push_back(mesh_obj);
	// 	return (int)mesh_object_array.size()-1;
	// }

	// ////this is an example of adding an object with manually created triangles and vertex attributes
	// int Add_Object_3()
	// {
	// 	auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
	// 	auto& mesh=mesh_obj->mesh;

	// 	////vertex position
	// 	std::vector<Vector3> triangle_vertices={Vector3(-1,-1,-1),Vector3(1,-1,-1),Vector3(-1,-1,1),Vector3(1,-1,1)};
	// 	std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
	// 	vertices=triangle_vertices;
			
	// 	////vertex color
	// 	std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
	// 	vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

	// 	////vertex normal
	// 	std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
	// 	vtx_normal={Vector3(0.,1.,0.),Vector3(0.,1.,0.),Vector3(0.,1.,0.),Vector3(0.,1.,0.)};

	// 	////vertex uv
	// 	std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
	// 	uv={Vector2(0.,0.),Vector2(1.,0.),Vector2(0.,1.),Vector2(1.,1.)};

	// 	////mesh elements
	// 	std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
	// 	elements={Vector3i(0,1,3),Vector3i(0,3,2)};

	// 	////set up shader
	// 	//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
	// 	mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3"));
		
	// 	////set up texture
	// 	mesh_obj->Add_Texture("tex_albedo", OpenGLTextureLibrary::Get_Texture("object_3_albedo"));
	// 	mesh_obj->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("object_3_normal"));
	// 	Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
	// 	Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow

	// 	////initialize
	// 	mesh_obj->Set_Data_Refreshed();
	// 	mesh_obj->Initialize();	
	// 	mesh_object_array.push_back(mesh_obj);
	// 	return (int)mesh_object_array.size()-1;
	// }
	int Add_Skybox_bottom()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////vertex position
		std::vector<Vector3> triangle_vertices={Vector3(-10,-10,-10),Vector3(10,-10,-10),Vector3(10,-10,10),Vector3(-10,-10,10)};
		std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		vertices=triangle_vertices;

		////vertex color
		std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
		vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		////vertex normal
		std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
		vtx_normal={Vector3(1.,1.,1.).normalized(),Vector3(-1.,1.,1.).normalized(),Vector3(-1.,1.,-1.).normalized(),Vector3(1.,1.,-1.).normalized()};

		////vertex uv
		std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
		uv={Vector2(0.,1.),Vector2(1.,1.),Vector2(1.,0.),Vector2(0.,0.)};

		////mesh elements
		std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
		elements={Vector3i(0,2,1),Vector3i(0,3,2)};

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));

		////set up texture
		mesh_obj->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture("bottom"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow
		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	int Add_Skybox_up()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////vertex position
		std::vector<Vector3> triangle_vertices={Vector3(-10,10,-10),Vector3(10,10,-10),Vector3(10,10,10),Vector3(-10,10,10)};
		std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		vertices=triangle_vertices;

		////vertex color
		std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
		vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		////vertex normal
		std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
		vtx_normal={Vector3(1.,-1.,1.).normalized(),Vector3(-1.,-1.,1.).normalized(),Vector3(-1.,-1.,-1.).normalized(),Vector3(1.,-1.,-1.).normalized()};

		////vertex uv
		std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
		uv={Vector2(0.,0.),Vector2(1.,0.),Vector2(1.,1.),Vector2(0.,1.)};

		////mesh elements
		std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
		elements={Vector3i(0,1,2),Vector3i(0,2,3)};

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));

		////set up texture
		mesh_obj->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture("up"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow

		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	int Add_Skybox_back()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////vertex position
		std::vector<Vector3> triangle_vertices={Vector3(-10,-10,-10),Vector3(-10,-10,10),Vector3(-10,10,10),Vector3(-10,10,-10)};
		std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		vertices=triangle_vertices;

		////vertex color
		std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
		vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		////vertex normal
		std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
		vtx_normal={Vector3(1.,1.,1.).normalized(),Vector3(1.,1.,-1.).normalized(),Vector3(1.,-1.,-1.).normalized(),Vector3(1.,-1.,1.).normalized()};

		////vertex uv
		std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
		uv={Vector2(0.,1.),Vector2(1.,1.),Vector2(1.,0.),Vector2(0.,0.)};

		////mesh elements
		std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
		elements={Vector3i(0,2,1),Vector3i(0,3,2)};

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));
		////set up texture
		mesh_obj->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture("back"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow

		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	int Add_Skybox_front()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////vertex position
		std::vector<Vector3> triangle_vertices={Vector3(10,-10,-10),Vector3(10,-10,10),Vector3(10,10,10),Vector3(10,10,-10)};
		std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		vertices=triangle_vertices;

		////vertex color
		std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
		vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		////vertex normal
		std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
		vtx_normal={Vector3(-1.,1.,1.).normalized(),Vector3(-1.,1.,-1.).normalized(),Vector3(-1.,-1.,-1.).normalized(),Vector3(-1.,-1.,1.).normalized()};

		////vertex uv
		std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
		uv={Vector2(1.,1.),Vector2(0.,1.),Vector2(0.,0.),Vector2(1.,0.)};

		////mesh elements
		std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
		elements={Vector3i(0,1,2),Vector3i(0,2,3)};

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));

		////set up texture
		mesh_obj->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture("front"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow


		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	int Add_Skybox_left()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////vertex position
		std::vector<Vector3> triangle_vertices={Vector3(10,-10,-10),Vector3(-10,-10,-10),Vector3(-10,10,-10),Vector3(10,10,-10)};
		std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		vertices=triangle_vertices;

		////vertex color
		std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
		vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		////vertex normal
		std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
		vtx_normal={Vector3(-1.,1.,1.).normalized(),Vector3(1.,1.,1.).normalized(),Vector3(1.,-1.,1.).normalized(),Vector3(-1.,-1.,1.).normalized()};

		////vertex uv
		std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
		uv={Vector2(0.,1.),Vector2(1.,1.),Vector2(1.,0.),Vector2(0.,0.)};

		////mesh elements
		std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
		elements={Vector3i(0,2,1),Vector3i(0,3,2)};

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));

		////set up texture
		mesh_obj->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture("left"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow

		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	int Add_Skybox_right()
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////vertex position
		std::vector<Vector3> triangle_vertices={Vector3(10,-10,10),Vector3(-10,-10,10),Vector3(-10,10,10),Vector3(10,10,10)};
		std::vector<Vector3>& vertices=mesh_obj->mesh.Vertices();
		vertices=triangle_vertices;

		////vertex color
		std::vector<Vector4f>& vtx_color=mesh_obj->vtx_color;
		vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f),Vector4f(1.f,1.f,0.f,1.f)};

		////vertex normal
		std::vector<Vector3>& vtx_normal=mesh_obj->vtx_normal;
		vtx_normal={Vector3(-1.,1.,-1.).normalized(),Vector3(1.,1.,-1.).normalized(),Vector3(1.,-1.,-1.).normalized(),Vector3(-1.,-1.,-1.).normalized()};

		////vertex uv
		std::vector<Vector2>& uv=mesh_obj->mesh.Uvs();
		uv={Vector2(1.,1.),Vector2(0.,1.),Vector2(0.,0.),Vector2(1.,0.)};

		////mesh elements
		std::vector<Vector3i>& elements=mesh_obj->mesh.Elements();
		elements={Vector3i(0,1,2),Vector3i(0,2,3)};

		////set up shader
		//mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("object_3_shadow"));//Shadow TODO: uncomment this line and comment next line to use shadow shader
		mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("skybox"));

		////set up texture
		mesh_obj->Add_Texture("tex", OpenGLTextureLibrary::Get_Texture("right"));
		Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
		Set_Shading_Mode(mesh_obj,ShadingMode::Texture);//SHADOW TODO: Set Shading Mode to Shadow

		////initialize
		mesh_obj->Set_Data_Refreshed();
		mesh_obj->Initialize();
		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}




	//// Use this function to set up lighting only if you are using Shadow mode
	void Init_Lighting() {
		auto dir_light = OpenGLUbos::Add_Directional_Light(glm::vec3(-1.f, -1.f, -1.f));//Light direction
		dir_light->dif = glm::vec4(.9,.8,.7, 1.0);//diffuse reflection color
		dir_light->Set_Shadow();////SHADOW TODO: turn on the shadow by uncommenting this line, currenly our base code only supports shadow for one light

		//This is an example to add a second directional light
		//auto dir_light_2 = OpenGLUbos::Add_Directional_Light(glm::vec3(1.,1.,1.));//the parameter is light direction
		//dir_light_2->dif = glm::vec4(.9,.9,.9,1.0);
		//dir_light_2->spec = glm::vec4(.2,.2,.2,1.0);

		//This is an example to add a point light
		//auto point_light = OpenGLUbos::Add_Point_Light(glm::vec3(1.,1.,1.));//the parameter is the position of point light
		//point_light->dif = glm::vec4(.9,.8,.7, 1.0);

		//This is an example to add a spot light
		//auto spot_light = OpenGLUbos::Add_Spot_Light(glm::vec3(1.,1.,1.),glm::vec3(1.,1.,1.));//first param: position, second param: direction
		//spot_light->dif = glm::vec4(.9,.8,.7, 1.0);

		OpenGLUbos::Set_Ambient(glm::vec4(.01f, .01f, .02f, 1.f));
		OpenGLUbos::Update_Lights_Ubo();	
	}

	virtual void Initialize_Data()
	{
		Add_Shaders();
		Add_Textures();

		//Add_Background();
		Add_Object_1();
		Add_Skybox_back();
		Add_Skybox_front();
		Add_Skybox_bottom();
		Add_Skybox_up();
		Add_Skybox_left();
		Add_Skybox_right();
		Add_Fish();
		Add_Coral();
		// Add_Object_3();

		//Init_Lighting(); ////SHADOW TODO: uncomment this line
	}

	////Goto next frame 
	virtual void Toggle_Next_Frame()
	{
		for (auto& mesh_obj : mesh_object_array) {
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	FinalProjectDriver driver;
	driver.Initialize();
	driver.Run();	
}

#endif
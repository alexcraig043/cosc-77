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
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class ShaderDriver : public Driver, public OpenGLViewer
{using Base=Driver;
	std::vector<OpenGLTriangleMesh*> mesh_object_array;						////mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
		////For those who are unhappy about the current background color: Here is a secret passage. 
		////Goto OpenGLShaderProgrammcpp Line 239 and 240, change the two colors, and you will get a different background.
		draw_bk=true;						////this flag specifies a customized way to draw the background. If you turn it off, there is no background.
		draw_axes=false;					////if you don't like the axes, turn them off!
		startTime = clock();
		OpenGLViewer::Initialize();
	}

	////This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		std::cout<<"load tri_mesh from obj file, #vtx: "<<mesh_obj->mesh.Vertices().size()<<", #ele: "<<mesh_obj->mesh.Elements().size()<<std::endl;		

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a sphere mesh
	int Add_Sphere_Object(const double radius=1.)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Initialize_Sphere_Mesh(radius,&mesh_obj->mesh,3);		////add a sphere with radius=1. if the obj file name is not specified

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a triangle (the in-class demo)
	int Add_Triangle_Object(const std::vector<Vector3>& vertices)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////manually initialize the vertices and elements for a triangle mesh
		mesh.Vertices().resize(3);
		for(int i=0;i<vertices.size();i++)mesh.Vertices()[i]=vertices[i];
		mesh.Elements().resize(1);mesh.Elements()[0]=Vector3i(0,1,2);

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function demonstrates how to manipulate the vertex array of a mesh on the CPU end.
	////The updated vertices will be sent to GPU for rendering automatically.
	void Translate_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,const Vector3& translate)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			v+=translate;
		}
	}

	void Scale_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,float scale)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			v*=scale;
		}
	}

	////This function demonstrates how to manipulate the color and normal arrays of a mesh on the CPU end.
	////The updated colors and normals will be sent to GPU for rendering automatically.
	void Update_Vertex_Color_And_Normal_For_Mesh_Object(OpenGLTriangleMesh* obj, int season)
	{
		int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
		std::vector<Vector3i>& elements=obj->mesh.Elements();		////you might find this array also useful

		std::vector<Vector4f>& vtx_color=obj->vtx_color;
		vtx_color.resize(vn);
		std::fill(vtx_color.begin(),vtx_color.end(),Vector4f::Zero());

		////TODO [Step 0]: update the color for each vertex.
		////NOTICE: This code updates the vertex color array on the CPU end. The array will then be sent to GPU and read it the vertex shader as v_color.
		////You don't need to implement the CPU-GPU data transfer code
		for(int i=0;i<vn;i++){
			//vtx_color[i] = Vector4f(sin(i), cos(i), sin(i / 2), 1.0);

			////specify color for each vertex by season
			if (season == 0)
				vtx_color[i]=Vector4f(1.,0.,0.,1.);
			else if (season == 1)
				vtx_color[i]=Vector4f(1.,1.,0.,1.);		
			else if (season == 2)
				vtx_color[i]=Vector4f(0.,0.,1.,1.);
			else
				vtx_color[i]=Vector4f(0.,1.,0.,1.);	
		}

		std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		vtx_normal.resize(vn);
		std::fill(vtx_normal.begin(),vtx_normal.end(),Vector3::Zero());

		//TODO: update the normal for each vertex
		//NOTICE: This code updates the vertex normal array on the CPU end. The array will then be sent to GPU and read it the vertex shader as normal.
		//This is a default implementation of vertex normal that works for a sphere centered around the origin only.
		// for(int i=0;i<vn;i++){
		// 	vtx_normal[i]=Vector3(vertices[i][0],vertices[i][1],vertices[i][2]);
		// }	

		////TODO [Step 1]: Comment the default implementation and uncomment the following function and implement it to calculate mesh normals.
		Update_Vertex_Normal(vertices,elements,vtx_normal);
	}

	////TODO [Step 1]: implement your function to update vertex normals
	void Update_Vertex_Normal(const std::vector<Vector3>& vertices,const std::vector<Vector3i>& elements,std::vector<Vector3>& normals)
	{
		////TODO [Step 1]: your implementation to calculate the normal vector for each mesh vertex
		// Initialize a map from vertex to incident triangles
		std::unordered_map<int, std::vector<int>> vtx_tri_map;

		// Initialize a vector of triangle normals
		std::vector<Vector3> tri_norms;
		// Resize the vector to the number of triangles
		tri_norms.resize(elements.size());
		// Fill the vector with vector3's of zeros
		std::fill(tri_norms.begin(),tri_norms.end(),Vector3::Zero());

		// Calculate the triangle normals:
		// Loop over all triangles in mesh
		for(int i = 0; i < elements.size(); i++) {
			// Get the triangle
			Vector3i tri = elements[i];

			// Get vertices a, b, and c of triangle
			int vtx_a = tri[0];
			int vtx_b = tri[1];
			int vtx_c = tri[2];

			// Get two vectors of the edges of the triangle
			Vector3 edge_ab = vertices[vtx_a] - vertices[vtx_b];
			Vector3 edge_ac = vertices[vtx_a] - vertices[vtx_c];

			// Calculate the unit normal of the triangle as the cross product of the two edge vectors
			tri_norms[i] = edge_ab.cross(edge_ac).normalized();

			// Create a vector of triangle's vertices
			std::vector<int> vtx{vtx_a, vtx_b, vtx_c};

			// Fill out vtx_tri_map:
			// Loop over original vertices
			for(int j = 0; j < 3; j++){
				// If the vtx is not in the map
				if(vtx_tri_map.find(vtx[j]) == vtx_tri_map.end()) { 
					// Initialize a vector of indices of incident triangles to that vertex
					std::vector<int> incident_tri;
					// Add the index of the triangle to the vector
					incident_tri.push_back(i);
					// Add the vector to the map
					vtx_tri_map[vtx[j]] = incident_tri;
				}
				// If the vtx is in the map
				else {
					// Add the index of triangle to the vector of incident triangles indices
					vtx_tri_map[vtx[j]].push_back(i);
				}
			}
		}

		// Calculate the vertex normals:
		// Loop over all vertices in mesh
		for(int i = 0; i < vertices.size(); i++){
			// Initialize a normal vector to the vertex filled with zeroes
			Vector3 vtx_norm = Vector3::Zero();
			// Loop over all incident triangles to the vertex
			for(int j = 0; j < vtx_tri_map[i].size(); j++){
				// Add the normal of each incident triangle to the vertex normal
				vtx_norm += tri_norms[vtx_tri_map[i][j]];
			}
			// Normalize the vertex normal to make it a unit vector
			normals[i] = vtx_norm.normalized();
		}
	}

	virtual void Initialize_Data()
	{
		////Add a sphere mesh
		{
			int obj_idx=Add_Sphere_Object();
			auto obj=mesh_object_array[obj_idx];
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj, 0);	
		}

		////Add an obj mesh
		////TODO [Step 4]: uncomment this part and use your own mesh for Step 4.
		// {
		// 	int obj_idx=Add_Obj_Mesh_Object("guy.obj");
		// 	auto obj=mesh_object_array[obj_idx];
		// 	Update_Vertex_Color_And_Normal_For_Mesh_Object(obj, 0);		
		// }

		////If you want to put multiple objects in the scene, uncomment this block. It will add another sphere mesh in the scene.
		{
			int obj_idx=Add_Sphere_Object();	////add a sphere
			auto obj=mesh_object_array[obj_idx];
			Translate_Vertex_Position_For_Mesh_Object(obj,Vector3::Unit(0)*3.);
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj, 1);		
		}
		{
			int obj_idx=Add_Sphere_Object();	////add a sphere
			auto obj=mesh_object_array[obj_idx];
			Translate_Vertex_Position_For_Mesh_Object(obj,Vector3::Unit(0)*-6.);
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj, 2);		
		}
		{
			int obj_idx=Add_Sphere_Object();	////add a sphere
			auto obj=mesh_object_array[obj_idx];
			Translate_Vertex_Position_For_Mesh_Object(obj,Vector3::Unit(0)*-3.);
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj, 3);		
		}
		{
			int obj_idx=Add_Obj_Mesh_Object("guy.obj");
			auto obj=mesh_object_array[obj_idx];
			Scale_Vertex_Position_For_Mesh_Object(obj,0.25);
			Translate_Vertex_Position_For_Mesh_Object(obj,Vector3::Unit(0)*6.);
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj, 1);		
		}

		//////Add a manually built triangle mesh (with a single triangle). This is the demo code I showed in class.
		//// You don't need this part for your homework. Just put them here for your reference.
		//{
		//	std::vector<Vector3> triangle_vertices={Vector3(0,0,0),Vector3(1,0,0),Vector3(1,1,0)};
		//	int obj_idx=Add_Triangle_Object(triangle_vertices);	////add a sphere
		//	auto obj=mesh_object_array[obj_idx];
		//	
		//	////specify the vertex colors on the CPU end
		//	std::vector<Vector4f>& vtx_color=obj->vtx_color;
		//	vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f)};

		//	std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		//	vtx_normal={Vector3(0.,0.,1.),Vector3(0.,0.,1.),Vector3(0.,0.,1.)};
		//}

		////initialize shader
		////TODO [Step 2,3,4]: switch the shaders by changing the file names here. We use the helloworld shader by default. 
		////You need to switch them to my_lambertian and my_phong for step 2,3, and 4. 
		std::string vertex_shader_file_name="my_phong.vert";
		std::string fragment_shader_file_name="my_phong.frag";
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vertex_shader_file_name,fragment_shader_file_name,"a2_shader");

		////bind the shader with each mesh object in the object array
		for(auto& mesh_obj: mesh_object_array){
			mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a2_shader"));
			Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
			Set_Shading_Mode(mesh_obj,ShadingMode::A2);
			mesh_obj->Set_Data_Refreshed();
			mesh_obj->Initialize();	
		}
	}

	//// Go to next frame 
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
	ShaderDriver driver;
	driver.Initialize();
	driver.Run();	
}

#endif
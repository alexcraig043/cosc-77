#ifndef __LoopSubdivision_h__
#define __LoopSubdivision_h__
#include <unordered_map>
#include <vector>
#include "Mesh.h"

inline void LoopSubdivision(TriangleMesh<3>& mesh)
{
	std::vector<Vector3>& old_vtx=mesh.Vertices();
	std::vector<Vector3i>& old_tri=mesh.Elements();
	std::vector<Vector3> new_vtx;		////vertex array for the new mesh
	std::vector<Vector3i> new_tri;		////element array for the new mesh

	new_vtx=old_vtx;	////copy all the old vertices to the new_vtx array

	////step 1: add mid-point vertices and triangles
	////for each old triangle, 
	////add three new vertices (in the middle of each edge) to new_vtx 
	////add four new triangles to new_tri

	// Initialize maps
	std::unordered_map<Vector2i,int> edge_vtx_map;
	std::unordered_map<Vector2i,std::vector<int> > edge_tri_map;
	std::unordered_map<int,std::vector<int> > vtx_vtx_map;

	// For each old triangle
	for (int i = 0; i < old_tri.size(); i++){
		// Get the original triangle
		const Vector3i tri_original = old_tri[i];
		// Initialize a new triangle
		Vector3i tri_new;

		// For each vtx in the old triangle
		for (int j = 0; j < 3; j++) {
			// Get the edge between vtx j and (j+1) % 3 (so that vtx 2 is connected to vtx 0)
			Vector2i edge(tri_original[j], tri_original[(j + 1) % 3]);
			// Make sure the edge is in the correct orientation
			if (edge[0] > edge[1]) {
				int tmp = edge[0];
				edge[0] = edge[1];
				edge[1] = tmp;
			}

			// Fill out vtx_vtx map:
			// If vtx is not in map
			if (vtx_vtx_map.find(tri_original[j]) == vtx_vtx_map.end()) {
				// Initialize a vector of ints
				std::vector<int> vtx_vec;
				// Add the other two vtxs to the vector
				vtx_vec.push_back(tri_original[(j + 1) % 3]);
				vtx_vec.push_back(tri_original[(j + 2) % 3]);
				// Add the vector to the map
				vtx_vtx_map[tri_original[j]] = vtx_vec;
			}
			// If vtx is in map
			else {
				// Add the other two vtxs to the vector
				vtx_vtx_map[tri_original[j]].push_back(tri_original[(j + 1) % 3]);
				vtx_vtx_map[tri_original[j]].push_back(tri_original[(j + 2) % 3]);
			}

			// Fill out the edge_tri map:
			// If edge is not in map
			if (edge_tri_map.find(edge) == edge_tri_map.end()) {
				// Initialize a vector of ints
				std::vector<int> tri_vec;
				// Add the original triangle's index to the vector
				tri_vec.push_back(i);
				// Add the vector to the map
				edge_tri_map[edge] = tri_vec;
			}
			// If edge is in map
			else {
				// Add the original triangle's index to the vector
				edge_tri_map[edge].push_back(i);
			}

			// Fill out the edge_vtx map:
			// If edge is not in map
			if (edge_vtx_map.find(edge) == edge_vtx_map.end()) {
				// Add the new vertex's index to the map
				edge_vtx_map[edge] = new_vtx.size();
				// Add the new vertex's index to the new triangle
				tri_new[j] = new_vtx.size();
				// Add the new vertex to the new_vtx array
				new_vtx.push_back((old_vtx[edge[0]] + old_vtx[edge[1]]) / 2);
			}
			// If edge is in map
			else {
				// Add the new vertex to the new_vtx array
				new_vtx.push_back((old_vtx[edge[0]] + old_vtx[edge[1]]) / 2);
			}
		}

		new_tri.push_back(Vector3i(tri_original[0], tri_new[0], tri_new[2]));
		new_tri.push_back(Vector3i(tri_original[1], tri_new[0], tri_new[1]));
		new_tri.push_back(Vector3i(tri_original[2], tri_new[1], tri_new[2]));
		new_tri.push_back(Vector3i(tri_new[0], tri_new[1], tri_new[2]));
	}

	////step 2: update the position for each new mid-point vertex: 
	////for each mid-point vertex, find its two end-point vertices A and B, 
	////and find the two opposite-side vertices on the two incident triangles C and D,
	////then update the new position as .375*(A+B)+.125*(C+D)

	/*your implementation here*/

	////step 3: update vertices of each old vertex
	////for each old vertex, find its incident old vertices, and update its position according its incident vertices

	/*your implementation here*/

	////update subdivided vertices and triangles onto the input mesh
	old_vtx=new_vtx;
	old_tri=new_tri;
}

#endif
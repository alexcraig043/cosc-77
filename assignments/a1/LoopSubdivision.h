#ifndef __LoopSubdivision_h__
#define __LoopSubdivision_h__
#include <unordered_map>
#include <vector>
#include "Mesh.h"
#include <iostream>

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
	std::unordered_map<Vector2i, std::vector<int> > edge_tri_map;
	std::unordered_map<int, std::vector<int>> vtx_vtx_map;

	// For each old triangle
	for (int i = 0; i < old_tri.size(); i++) {
		// Get the original triangle
		const Vector3i tri_original = old_tri[i];

		// Initialize a new triangle
		Vector3i tri_new;

		// For each edge in the old triangle
		for (int j = 0; j < 3; j++) {
			// Get the edge between vtx j and (j+1) % 3 (so that vtx 2 is connected to vtx 0)
			Vector2i edge(tri_original[j], tri_original[(j + 1) % 3]);
			// Make sure the edge is in the correct orientation
			if (edge[0] > edge[1]) {
				int tmp = edge[0];
				edge[0] = edge[1];
				edge[1] = tmp;
			}

			// Fill out the edge_vtx map:
			// If edge is not in map
			if (edge_vtx_map.find(edge) == edge_vtx_map.end()) {
				// Set midpoint 
				Vector3 midpoint = .5 * (old_vtx[edge[0]] + old_vtx[edge[1]]);
				// Add the new vertex to the new_vtx array
				new_vtx.push_back(midpoint);
				// Add the new vertex's index to the map
				edge_vtx_map[edge] = new_vtx.size() - 1;
				// Add the new vertex's index to the new triangle
				tri_new[j] = new_vtx.size() - 1;
			}
			// If edge is in map
			else {
				// Add the new vertex's index to the new triangle
				tri_new[j] = edge_vtx_map[edge];
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
				// Push the original triangle's index to the map
				edge_tri_map[edge].push_back(i);
			}

			// Fill out vtx_vtx map:
			// If vtx is not in map
			if (vtx_vtx_map.find(tri_original[j]) == vtx_vtx_map.end()) {
				// Initialize a vector of ints
				std::vector<int> vtx_vec;
				// Add the other two vtx's to the vector
				vtx_vec.push_back(tri_original[(j + 1) % 3]);
				vtx_vec.push_back(tri_original[(j + 2) % 3]);
				// Add the vector to the map
				vtx_vtx_map[tri_original[j]] = vtx_vec;
			}
			// If vtx is in map
			else {
				// Add the other two vtx's to the vector
				vtx_vtx_map[tri_original[j]].push_back(tri_original[(j + 1) % 3]);
				vtx_vtx_map[tri_original[j]].push_back(tri_original[(j + 2) % 3]);
			}
		}

		// Add the new triangles to the new_tri array
		new_tri.push_back(Vector3i(tri_original[0], tri_new[0], tri_new[2]));
		new_tri.push_back(Vector3i(tri_original[1], tri_new[1], tri_new[0]));
		new_tri.push_back(Vector3i(tri_original[2], tri_new[2], tri_new[1]));
		new_tri.push_back(Vector3i(tri_new[0], tri_new[1], tri_new[2]));
	}

	////step 2: update the position for each new mid-point vertex: 
	////for each mid-point vertex, find its two end-point vertices A and B, 
	////and find the two opposite-side vertices on the two incident triangles C and D,
	////then update the new position as .375*(A+B)+.125*(C+D)

	std::vector<Vector3> temp_vtx = new_vtx;

	// Iterate through the edge_vtx_map
	for(auto& elem: edge_vtx_map){
		// Get the edge
		Vector2i edge = elem.first;
		// Get the index of the new vertex
		int new_vtx_idx = elem.second;

		// Get the index of the two end-point vertices
		int vtx_A_idx = edge[0];
		int vtx_B_idx = edge[1];

		// Get the two incident triangles' indices
		std::vector<int> incident_tri = edge_tri_map[edge];
		// Get the two incident triangles
		Vector3i tri_A = old_tri[incident_tri[0]];
		Vector3i tri_B = old_tri[incident_tri[1]];

		// Assign -1 so they are invalid indices by default
		int vtx_C_idx = -1;
		int vtx_D_idx = -1;

		for (int i = 0; i < 3; i++) {
			if (vtx_A_idx != tri_A[i] && vtx_B_idx != tri_A[i]) {
				vtx_C_idx = tri_A[i];
			}
			if (vtx_A_idx != tri_B[i] && vtx_B_idx != tri_B[i]) {
				vtx_D_idx = tri_B[i];
			}
		}
		
		// Update the new vertex's position
		temp_vtx[new_vtx_idx] = .375 * (new_vtx[vtx_A_idx] + new_vtx[vtx_B_idx]) + .125 * (new_vtx[vtx_C_idx] + new_vtx[vtx_D_idx]);
	}


	// // ////step 3: update vertices of each old vertex
	// // ////for each old vertex, find its incident old vertices, and update its position according its incident vertices

	// Iterate through the vtx_vtx_map
	for(auto& elem: vtx_vtx_map){
		// Get the index of the central old vertex
		int old_vtx_idx = elem.first;
		// Get the vector of the old vertex's incident vertices
		std::vector<int> incident_vtx = elem.second;
		// Get the number of incident vertices
		int num_incident_vtx = incident_vtx.size();

		// Calculate the beta value
		float beta;
		if(num_incident_vtx > 3) {
			beta = 3. / (8. * num_incident_vtx);
		}
		else {
			beta = 3. / 16.;
		}
		// Iterate through the incident vertices and update values
		temp_vtx[old_vtx_idx] = (1 - num_incident_vtx * beta) * new_vtx[old_vtx_idx];
		for (int i = 0; i < num_incident_vtx; i++) {
			temp_vtx[old_vtx_idx] += beta * old_vtx[incident_vtx[i]];
		}
	}

	////update subdivided vertices and triangles onto the input mesh
	new_vtx = temp_vtx;
	old_vtx=new_vtx;
	old_tri=new_tri;
}

#endif
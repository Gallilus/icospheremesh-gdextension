#ifndef ICO_SPHERE_MESH_H
#define ICO_SPHERE_MESH_H

#include <godot_cpp/classes/array_mesh.hpp>
//#include <godot_cpp/core/binder_common.hpp>

namespace godot {

class IcoSphereMesh : public ArrayMesh {
    GDCLASS(IcoSphereMesh, ArrayMesh)
    

public:
    IcoSphereMesh();
    ~IcoSphereMesh();

    /** @brief Updates the mesh, needs to be called every time a property schanges*/
    void update_mesh();
    
    void set_subdivisions(int p_subdivisions);
    int get_subdivisions() const;
    void set_diameter(float p_diameter);
    float get_diameter() const;
  //  void set_core_vertices(Array p_core_vertices);
  //  Array get_core_vertices() const;




protected:

    static void _bind_methods();

    /** @brief The amount of subdivisions for the icosphere. Starting at 20 vertices each subdivision will multiply the amount of vertices by 4 so 20 vertices will become 80, 320, 1280, 5120, 20480, 81920, 327680, 1310720, 5242880, 20971520 */
    int _subdivisions = 2;
    /** @brief The diameter of the icosphere */
    float _diameter = 1.0f;

    const float X=.525731112119133606f;
    const float Z=.850650808352039932f;
    const float N=0.f;

    /** @brief the hard coded coordinates for initiation*/
    PackedVector3Array _core_vertices;
    /** @brief the list of positions of all vertices*/
    PackedVector3Array _vertices;

    /** @brief the hard coded list for initiation*/
    PackedVector3Array _core_triangles;
    /** @brief the list _vertices[index] creating the triangels*/
    PackedVector3Array _triangles;
    // the list of triangles as ints
    PackedInt32Array _triangles_list;

    

    // the list of uvs
    PackedVector2Array _uvs;
    // the list of normals
    PackedFloat32Array _normals;

    // the lookup table for created vertices
    Dictionary _lookup;

    // manage the vertices and prevent duplicates
    int _vertex_for_edge(int first, int second);
    // get a subdevision of the _triangles
    PackedVector3Array _get_subdivided();
    // get the uv coordinates for a given cartesian coordinate
    Vector2 get_uv(Vector3 cartesian);
    
   };

}

#endif
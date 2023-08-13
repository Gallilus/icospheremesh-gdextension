#include "ico_sphere_mesh.h"

godot::IcoSphereMesh::IcoSphereMesh() {
        _core_vertices.resize(12);
        _core_vertices.set(0, Vector3(-X,N,Z));
        _core_vertices.set(1, Vector3(X,N,Z));
        _core_vertices.set(2, Vector3(-X,N,-Z));
        _core_vertices.set(3, Vector3(X,N,-Z));
        _core_vertices.set(4, Vector3(N,Z,X));
        _core_vertices.set(5, Vector3(N,Z,-X));
        _core_vertices.set(6, Vector3(N,-Z,X));
        _core_vertices.set(7, Vector3(N,-Z,-X));
        _core_vertices.set(8, Vector3(Z,X,N));
        _core_vertices.set(9, Vector3(-Z,X, N));
        _core_vertices.set(10, Vector3(Z,-X,N));
        _core_vertices.set(11, Vector3(-Z,-X, N));

        _core_triangles.resize(20);
        _core_triangles[0] = Vector3(0,4,1);
        _core_triangles[1] = Vector3(0,9,4);
        _core_triangles[2] = Vector3(9,5,4);
        _core_triangles[3] = Vector3(4,5,8);
        _core_triangles[4] = Vector3(4,8,1);
        _core_triangles[5] = Vector3(8,10,1);
        _core_triangles[6] = Vector3(8,3,10);
        _core_triangles[7] = Vector3(5,3,8);
        _core_triangles[8] = Vector3(5,2,3);
        _core_triangles[9] = Vector3(2,7,3);
        _core_triangles[10] = Vector3(7,10,3);
        _core_triangles[11] = Vector3(7,6,10);
        _core_triangles[12] = Vector3(7,11,6);
        _core_triangles[13] = Vector3(11,0,6);
        _core_triangles[14] = Vector3(0,1,6);
        _core_triangles[15] = Vector3(6,1,10);
        _core_triangles[16] = Vector3(9,0,11);
        _core_triangles[17] = Vector3i(9,11,2);
        _core_triangles[18] = Vector3i(9,2,5);
        _core_triangles[19] = Vector3i(7,2,11);
}

godot::IcoSphereMesh::~IcoSphereMesh() {
}

void godot::IcoSphereMesh::_init() {
    update_mesh();
}

void godot::IcoSphereMesh::update_mesh()
{
    _triangles = _core_triangles;
    _triangles_list.clear();
    _vertices = _core_vertices;
    _uvs.clear();
    for (int i = 0; i < _core_vertices.size(); i++) {
        _uvs.push_back(get_uv(_core_vertices[i]));
    }

    for (int i = 0; i < _subdivisions; i++) {
        _triangles = _get_subdivided();
    }
    // scale vertices
    for (int i = 0; i < _vertices.size(); i++) {
        _vertices.set(i, _vertices[i] * _diameter);
    }
    // convert triangles to poolintarray
    for (int i = 0; i < _triangles.size(); i++) {
        _triangles_list.push_back(_triangles[i].x);
        _triangles_list.push_back(_triangles[i].y);
        _triangles_list.push_back(_triangles[i].z);
    }
    // initialize the array mesh
    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = _vertices;
    arrays[Mesh::ARRAY_INDEX] = _triangles_list;
    arrays[Mesh::ARRAY_TEX_UV] = _uvs;
    // create the mesh
    clear_surfaces();
    add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
    emit_changed();
}

void godot::IcoSphereMesh::set_subdivisions(int p_subdivisions) {
    p_subdivisions = CLAMP(p_subdivisions, 0, 6);
    if (p_subdivisions == _subdivisions)
        return;
    _subdivisions = p_subdivisions;
    update_mesh();
}

int godot::IcoSphereMesh::get_subdivisions() const {
    return _subdivisions;
}

void godot::IcoSphereMesh::set_diameter(float p_diameter) {
    if (p_diameter == _diameter)
        return;
    _diameter = p_diameter;
    update_mesh();
}

float godot::IcoSphereMesh::get_diameter() const {
    return _diameter;
}

void godot::IcoSphereMesh::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_subdivisions", "subdivisions"), &IcoSphereMesh::set_subdivisions);
    ClassDB::bind_method(D_METHOD("get_subdivisions"), &IcoSphereMesh::get_subdivisions);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "subdivisions", PROPERTY_HINT_RANGE, "0,6,1"), "set_subdivisions", "get_subdivisions");
    ClassDB::bind_method(D_METHOD("set_diameter", "diameter"), &IcoSphereMesh::set_diameter);
    ClassDB::bind_method(D_METHOD("get_diameter"), &IcoSphereMesh::get_diameter);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "diameter", PROPERTY_HINT_RANGE, "0.01,100,0.01"), "set_diameter", "get_diameter");
}

godot::Vector2 godot::IcoSphereMesh::get_uv(godot::Vector3 cartesian) {
    float u = 0.5 + (atan2(cartesian.x, cartesian.z) / (2 * Math_PI));
    float v = 0.5 - (asin(cartesian.y) / Math_PI);
    return Vector2(u, v);
}

int godot::IcoSphereMesh::_vertex_for_edge(int first, int second) {
    Array key = Array::make(first, second);
    if (first > second) {
        key = Array::make(second, first);
    }
    // add _lookup index if it doesn't exist
    if (!_lookup.has(key)) {
        _lookup[key] = _vertices.size();
    }
    // create new vertex
    if (_lookup[key]) {
        Vector3 edge0 = _vertices[first];
        Vector3 edge1 = _vertices[second];
        Vector3 point = (edge0 + edge1).normalized();
        _vertices.push_back(point);
        _uvs.push_back(get_uv(point));
    }

    return _lookup[key];
}

godot::PackedVector3Array godot::IcoSphereMesh::_get_subdivided() {
    _lookup.clear();
    
    PackedVector3Array new_triangles;
    Array mid = Array::make(0, 0, 0);

    for (int i = 0; i < _triangles.size(); i++) {
        Array original_triangle;
        original_triangle.append(_triangles[i].x);
        original_triangle.append(_triangles[i].y);
        original_triangle.append(_triangles[i].z);
        for (int edge = 0; edge < 3; edge++) {
            int first = original_triangle[edge];
            int second = original_triangle[(edge + 1) % 3];
            mid[edge] = _vertex_for_edge(first, second);
        }
        new_triangles.push_back(Vector3(original_triangle[0], mid[0], mid[2]));
        new_triangles.push_back(Vector3(original_triangle[1], mid[1], mid[0]));
        new_triangles.push_back(Vector3(original_triangle[2], mid[2], mid[1]));
        new_triangles.push_back(Vector3(mid[0], mid[1], mid[2]));
    }

    return new_triangles;
}

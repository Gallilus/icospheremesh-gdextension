#ifndef ICO_SPHERE_MESH_H
#define ICO_SPHERE_MESH_H

#include <godot_cpp/classes/array_mesh.hpp>

namespace godot {

class IcoSphereMesh : public ArrayMesh {
    GDCLASS(IcoSphereMesh, ArrayMesh)

public:
    IcoSphereMesh() {}
    ~IcoSphereMesh() {}

    

    //void _process(double delta);

private:
    //double time_passed;

protected:
    static void _bind_methods() {}

};

}

#endif
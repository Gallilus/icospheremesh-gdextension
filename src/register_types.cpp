#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "ico_sphere_mesh.h"

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level == GDEXTENSION_INITIALIZATION_CORE) {
		// level core is not supported by GDExtension
	}
	if (p_level == GDEXTENSION_INITIALIZATION_SERVERS) {
		// Load dependencies here
	}
	if (p_level == GDEXTENSION_INITIALIZATION_SCENE) {
		// Default initialization level register classes here using ClassDB::register_class<T>();
		ClassDB::register_class<IcoSphereMesh>();
	}
	if (p_level == GDEXTENSION_INITIALIZATION_EDITOR) {
		// This level is never used by the core project
	}
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != GDEXTENSION_INITIALIZATION_SCENE) {
		return;
	}
}

extern "C" {
	// Initialization
	GDExtensionBool GDE_EXPORT icospheremesh_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
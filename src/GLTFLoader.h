#ifndef _H_GLTF_LOADER_
#define _H_GLTF_LOADER_

#include "cgltf.h"

cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* handle);

#endif

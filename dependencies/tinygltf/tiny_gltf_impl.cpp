#define TINYGLTF_IMPLEMENTATION

/*Edit by Samantha on 09-09-2020:
Commenting out the definitions for STBI's implementation and write implementation.
When linking against both STB Image and TinyGLTF as static libs, these will
cause multiple definition linker errors (since STB Image's static lib already 
defines its own implementation).

//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
*/

// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"
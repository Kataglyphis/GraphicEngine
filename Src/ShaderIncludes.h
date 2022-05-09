#pragma once
#include <vector>

class ShaderIncludes
{

public:

	ShaderIncludes();

	~ShaderIncludes();

private:

    std::vector<const char*> includeNames = {
            "host_device_shared.h",
            "Globals.glsl",
            "Matlib.glsl",
            "microfacet.glsl",
            "ShadingLibrary.glsl",
            "disney.glsl",
            "frostbite.glsl",
            "pbrBook.glsl",
            "phong.glsl",
            "unreal4.glsl"
    };

    std::vector<const char*> file_locations_relative = {
            "/Src/host_device_shared.h",
            "/Resources/Shaders/common/Globals.glsl",
            "/Resources/Shaders/common/Matlib.glsl",
            "/Resources/Shaders/common/microfacet.glsl",
            "/Resources/Shaders/common/ShadingLibrary.glsl" ,
            "/Resources/Shaders/brdf/disney.glsl",
            "/Resources/Shaders/brdf/frostbite.glsl",
            "/Resources/Shaders/brdf/pbrBook.glsl",
            "/Resources/Shaders/brdf/phong.glsl",
            "/Resources/Shaders/brdf/unreal4.glsl"
    };

};


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
            "unreal4.glsl",
            "clouds.glsl",
            "grad_noise.glsl",
            "worley_noise.glsl",
            "bindings.h",
            "GlobalValues.h"
    };

    std::vector<const char*> file_locations_relative = {
            "/Src/host_device_shared.h",
            "/Resources/Shaders/common/Globals.glsl",
            "/Resources/Shaders/common/Matlib.glsl",
            "/Resources/Shaders/pbr/microfacet.glsl",
            "/Resources/Shaders/common/ShadingLibrary.glsl" ,
            "/Resources/Shaders/pbr/brdf/disney.glsl",
            "/Resources/Shaders/pbr/brdf/frostbite.glsl",
            "/Resources/Shaders/pbr/brdf/pbrBook.glsl",
            "/Resources/Shaders/pbr/brdf/phong.glsl",
            "/Resources/Shaders/pbr/brdf/unreal4.glsl",
            "/Resources/Shaders/clouds/clouds.glsl",
            "/Resources/Shaders/common/grad_noise.glsl",
            "/Resources/Shaders/common/worley_noise.glsl",
            "/Src/bindings.h",
            "/Src/GlobalValues.h",
    };

};


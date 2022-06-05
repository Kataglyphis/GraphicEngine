#pragma once
#include <vector>
#include <glad/glad.h>

// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt
class ShaderIncludes {

  public:
  ShaderIncludes();

  ~ShaderIncludes();

  private:
  std::vector<const char*> includeNames = { "host_device_shared.h",
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
    "GlobalValues.h",
    "directional_light.glsl",
    "light.glsl",
    "material.glsl",
    "point_light.glsl" };

  std::vector<const char*> file_locations_relative = { "/Src/host_device_shared.h",
    "/Resources/Shaders/common/Matlib.glsl",
    "/Resources/Shaders/pbr/microfacet.glsl",
    "/Resources/Shaders/common/ShadingLibrary.glsl",
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
    "/Resources/Shaders/common/directional_light.glsl",
    "/Resources/Shaders/common/light.glsl",
    "/Resources/Shaders/common/material.glsl",
    "/Resources/Shaders/common/point_light.glsl" };
};

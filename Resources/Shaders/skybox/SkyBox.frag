#version 460

layout(location = 2) out vec3 g_albedo;
layout(location = 3) out vec3  g_material_id;

in vec3 tex_coords;
in vec4 world_pos;

uniform samplerCube skybox;
uniform int skyBoxMaterialID;

void main()
  {

    g_albedo = texture(skybox, tex_coords).xyz;
    g_material_id = vec3(skyBoxMaterialID);

  }
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
    //g_albedo = vec3(0.0f,1.0f,0.0f);
    g_material_id = vec3(skyBoxMaterialID);

  }
#version 330

layout(location = 2) out vec3 g_albedo;

in vec3 tex_coords;
in vec4 world_pos;

uniform samplerCube skybox;

void main()
  {

    g_albedo = texture(skybox, tex_coords).xyz;

  }
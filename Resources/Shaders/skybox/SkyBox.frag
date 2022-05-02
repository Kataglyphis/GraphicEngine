#version 460

layout(location = 2) out vec3 g_albedo;

in vec3 tex_coords;
in vec4 world_pos;

uniform samplerCube skybox;

void main()
  {

    g_albedo = texture(skybox, tex_coords).xyz;
    g_albedo = vec3(0.0f,1.0f,0.0f);

  }
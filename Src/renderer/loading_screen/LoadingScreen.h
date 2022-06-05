#pragma once
#include "Quad.h"
#include "ShaderProgram.h"
#include "Texture.h"

class LoadingScreen {
  public:
  LoadingScreen();

  void init();
  void render();

  ~LoadingScreen();

  private:
  //everything necessary for the loading screen
  Quad loading_screen_quad;
  Texture loading_screen_tex;
  Texture logo_tex;

  std::shared_ptr<ShaderProgram> loading_screen_shader_program;

  void create_shader_program();
};

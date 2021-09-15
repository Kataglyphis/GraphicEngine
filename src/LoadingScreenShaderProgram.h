#pragma once
#include "ShaderProgram.h"
class LoadingScreenShaderProgram :
    public ShaderProgram
{
public: 

    LoadingScreenShaderProgram();

    ~LoadingScreenShaderProgram();

private:

    void retrieve_uniform_locations();

};


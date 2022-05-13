#pragma once
#include "ShaderProgram.h"
#include "host_device_shared.h"

#include <GL/glew.h>

class ComputeShaderProgram :
    public ShaderProgram
{
public:

    ComputeShaderProgram();

    void reload();

    ~ComputeShaderProgram();

private:

};


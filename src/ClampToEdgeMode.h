#pragma once
#include "TextureWrappingMode.h"
#include "glErrorChecker.h"

class ClampToEdgeMode :
    public TextureWrappingMode
{
public:

    ClampToEdgeMode();

    void activate();

    ~ClampToEdgeMode();

private:
    glErrorChecker glErrorChecker;
};


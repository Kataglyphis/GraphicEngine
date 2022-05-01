#pragma once
#include "TextureWrappingMode.h"
#include "DebugApp.h"

class ClampToEdgeMode :
    public TextureWrappingMode
{
public:

    ClampToEdgeMode();

    void activate();

    ~ClampToEdgeMode();

private:
    DebugApp DebugApp_ins;
};


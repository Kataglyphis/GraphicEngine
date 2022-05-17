#pragma once
#include "TextureWrappingMode.h"

class ClampToEdgeMode :
    public TextureWrappingMode
{
public:

    ClampToEdgeMode();

    void activate() override;

    ~ClampToEdgeMode();

private:

};


#pragma once
#include "TextureWrappingMode.h"
class RepeatMode :
    public TextureWrappingMode
{
public:
    RepeatMode();

    void activate();

    ~RepeatMode();

private:
};


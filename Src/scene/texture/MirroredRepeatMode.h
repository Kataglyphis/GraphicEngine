#pragma once
#include "TextureWrappingMode.h"
class MirroredRepeatMode :
    public TextureWrappingMode
{
public:

    MirroredRepeatMode();

    void activate() override;

    ~MirroredRepeatMode();

private:

};


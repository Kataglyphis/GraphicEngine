#pragma once
#include "TextureWrappingMode.h"
class RepeatMode : public TextureWrappingMode {
 public:
  RepeatMode();

  void activate() override;

  ~RepeatMode();

 private:
};

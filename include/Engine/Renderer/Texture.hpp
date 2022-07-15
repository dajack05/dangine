#pragma once

#include <glm/fwd.hpp>
#include <string>
#include <vector>

class Texture {
public:
  void load(const std::string &filename);
  void bind();
  inline bool isLoaded() { return texID > 0; }
  glm::vec2 getSize();
  ~Texture();

private:
  unsigned int texID = 0;

  unsigned int mTexWidth = 0;
  unsigned int mTexHeight = 0;

  void loadFile(std::vector<unsigned char> &buffer,
                const std::string &filename);
};
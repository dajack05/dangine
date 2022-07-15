#include <Engine/Renderer/Texture.hpp>
#include <picopng.cpp>

#include <Engine/Config.hpp>
#include <Engine/Util/Logger.hpp>
#include <fstream>
#include <glm/vec2.hpp>

#include <glad/glad.h>

void Texture::loadFile(
    std::vector<unsigned char> &buffer,
    const std::string &filename) // designed for loading files from
                                 // hard disk in an std::vector
{
  std::ifstream file;
  file.open(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

  if (!file.is_open()) {
    Log::Error("Failed to open " + filename);
    return;
  }

  // get filesize
  std::streamsize size = 0;
  if (file.seekg(0, std::ios::end).good())
    size = file.tellg();
  if (file.seekg(0, std::ios::beg).good())
    size -= file.tellg();

  // read contents of the file into the vector
  if (size > 0) {
    buffer.resize((size_t)size);
    file.read((char *)(&buffer[0]), size);
  } else
    buffer.clear();
}

glm::vec2 Texture::getSize() { return {mTexWidth, mTexHeight}; }

void Texture::load(const std::string &filename) {
  std::vector<unsigned char> image_buffer;
  loadFile(image_buffer, IMAGE_PATH + filename + ".png");

  std::vector<unsigned char> data;
  unsigned long width = 0;
  unsigned long height = 0;
  auto result = decodePNG(data, width, height, &image_buffer[0],
                          image_buffer.size(), true);

  if (result != 0) {
    Log::Error("Failed to load texture:", result);
    return;
  }

  mTexWidth = width;
  mTexHeight = height;

  Log::Debug("Loaded " + filename);

  // Now do the OGL stuff
  glGenTextures(1, &texID);
  bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, &data[0]);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, texID); }

Texture::~Texture() { glDeleteTextures(1, &texID); }
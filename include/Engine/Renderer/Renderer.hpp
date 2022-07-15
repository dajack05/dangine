#pragma once

#include <Engine/Renderer/Shader.hpp>
#include <glm/mat4x4.hpp>

class Quad;
class GLFWwindow;
class Sprite;

class Renderer {
public:
  Renderer(const char *window_title, const unsigned int window_width,
           const unsigned int window_height);
  ~Renderer();

  inline bool ready() { return mReady; }

  void update();
  void clear();
  void present();

  void drawQuad(const Quad &quad, const glm::mat4 &modelMatrix);
  void drawSprite(Sprite *sprite);

  void drawLine(const glm::vec2 &from, const glm::vec2 &to,
                const glm::vec3 &color);
  void drawRect(const glm::vec2 &from, const glm::vec2 &to,
                const glm::vec3 &color);

  bool didWindowClose();

  inline double getDelta() { return mDelta; }

  void setFramebufferSize(unsigned int x, unsigned int y);

  glm::vec2 mCameraLocation{0.0f, 0.0f};
  float mCameraScale = 1.0f;

private:
  int window_width = 1;
  int window_height = 1;
  GLFWwindow *mWindow = nullptr;
  bool mReady = false;

  glm::mat4 view;
  glm::mat4 projection;

  unsigned int quadVAO = 0;
  unsigned int quadPosVBO = 0;
  unsigned int quadUvVBO = 0;

  Shader lineShader;
  unsigned int lineVAO = 0;
  unsigned int lineVBO = 0;

  unsigned int framebufferObj = 0;
  unsigned int frameBufferTexture = 0;
  
  Shader mScreenShader;
  glm::ivec2 mFramebufferSize{160, 144};

  double mLastTime = -1.0;
  double mDelta = 1.0;
  double mTime = 0.0;

  void drawFramebufferQuad();

  void bindFramebuffer();
  void bindWindowBuffer();

  void setupLines();

  static void OnWindowResize(GLFWwindow *window, int width, int height);

  void initQuad();
  void initFramebuffer();

  void setupShader(Shader *shader);
};
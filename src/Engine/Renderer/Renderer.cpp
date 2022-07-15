#include <Engine/Renderer/Renderer.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <Engine/Input/Input.hpp>
#include <Engine/Renderer/Quad.hpp>
#include <Engine/Renderer/Texture.hpp>
#include <Engine/Scene/Sprite.hpp>
#include <Engine/Util/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>

std::string GetError() {
  const char *text;
  int code = glfwGetError(&text);
  if (code != GLFW_NO_ERROR) {
    return std::string(text);
  }
  return "";
}

void Renderer::setupShader(Shader *shader) {
  shader->setVec3Attr("vPos", quadPosVBO);
  shader->setVec2Attr("vUvs", quadUvVBO);
  shader->isFirstRun = false;
}

void Renderer::initQuad() {
  glGenVertexArrays(1, &quadVAO);
  glBindVertexArray(quadVAO);

  glGenBuffers(1, &quadPosVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVerts), &QuadVerts[0],
               GL_STATIC_DRAW);

  glGenBuffers(1, &quadUvVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadUvVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QuadUvs), &QuadUvs[0], GL_STATIC_DRAW);

  // Load data
  mScreenShader.load("screen");
  mScreenShader.use();
  setupShader(&mScreenShader);
}

void loadFramebuffer(glm::ivec2 mFramebufferSize, unsigned int frameBufferTexture) {
  glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mFramebufferSize.x, mFramebufferSize.y,
               0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         frameBufferTexture, 0);
}

void Renderer::initFramebuffer() {

  glGenFramebuffers(1, &framebufferObj);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferObj);

  glGenTextures(1, &frameBufferTexture);

  loadFramebuffer(mFramebufferSize, frameBufferTexture);

  auto bufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (bufferStatus != GL_FRAMEBUFFER_COMPLETE) {
    Log::Error("Framebuffer not complete");
    Log::Error(bufferStatus);
    return;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  bindWindowBuffer();
}

Renderer::Renderer(const char *window_title, const unsigned int window_width,
                   const unsigned int window_height) {
  Log::Info("Renderer Created");

  if (glfwInit() != GLFW_TRUE) {
    Log::Error("Failed to init GLFW");
    Log::Error(GetError());
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

  mWindow = glfwCreateWindow(window_width, window_height, window_title, nullptr,
                             nullptr);
  if (mWindow == nullptr) {
    Log::Error("Failed to create GLFW Window");
    Log::Error(GetError());
    return;
  }

  glfwMakeContextCurrent(mWindow);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  glfwSwapInterval(1);

  initQuad();

  initFramebuffer();

  glClearColor(0.0F, 0.2F, 0.4F, 1.0F);

  mCameraScale = 1.0F;

  int width, height;
  glfwGetWindowSize(mWindow, &width, &height);

  glViewport(0, 0, width, height);
  projection = glm::ortho(0.0, width * 1.0, 0.0, height * 1.0, 0.1, 100.0);

  view = glm::translate(glm::mat4(1.0F), {0.0F, 0.0F, -1.0F});

  // Texture stuff
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetWindowUserPointer(mWindow, this);

  Renderer::OnWindowResize(mWindow, width, height);

  glfwSetWindowSizeCallback(mWindow, Renderer::OnWindowResize);

  Input::Setup(mWindow);

  setupLines();

  mReady = true;
}

void Renderer::update() {
  view = glm::mat4(1.0f);
  view = glm::scale(view, {mCameraScale, mCameraScale, mCameraScale});
  view = glm::translate(
      view, {-(mCameraLocation - (glm::vec2(mFramebufferSize) / 2.0F)), 0.0f});
}

Renderer::~Renderer() {
  glDeleteFramebuffers(1, &framebufferObj);
  glDeleteBuffers(1, &quadPosVBO);
  glDeleteBuffers(1, &quadUvVBO);
  glDeleteVertexArrays(1, &quadVAO);

  if (mWindow == nullptr) {
    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
  }
  glfwTerminate();
  Log::Info("Renderer Destroyed");
}

void Renderer::drawSprite(Sprite *sprite) {
  if (sprite == nullptr) {
    Log::Error("Renderer::drawSprite | sprite == null");
    return;
  }

  Quad quad;
  quad.shader = &sprite->getShader();
  quad.texture = &sprite->getTexture();
  quad.size = {sprite->imageScale, sprite->imageScale};
  quad.tint = sprite->tint;
  quad.uv = sprite->uv;

  drawQuad(quad, sprite->globalTransform.getMatrix());
}

void Renderer::drawQuad(const Quad &quad, const glm::mat4 &modelMatrix) {
  if (quad.shader == nullptr) {
    return;
  }

  glBindVertexArray(quadVAO);

  if (quad.texture != nullptr) {
    quad.texture->bind();
  }

  quad.shader->use();
  quad.shader->setVec3Attr("vPos", quadPosVBO);
  quad.shader->setVec2Attr("vUvs", quadUvVBO);

  auto model = glm::scale(modelMatrix, glm::vec3(quad.size, 1.0f));

  glm::mat4 MVP = projection * view * model;

  quad.shader->setInt("colorTexture", 0);
  quad.shader->setVec4("UVPosSize", quad.uv);
  quad.shader->setVec3("tint", quad.tint);
  quad.shader->setMat4("MVP", MVP);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::drawFramebufferQuad() {
  glBindVertexArray(quadVAO);

  glBindTexture(GL_TEXTURE_2D, frameBufferTexture);

  mScreenShader.use();
  auto vPosPos = mScreenShader.setVec3Attr("vPos", quadPosVBO);
  auto vUvsPos = mScreenShader.setVec2Attr("vUvs", quadUvVBO);

  mScreenShader.setInt("colorTexture", 0);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

// void Renderer::drawFramebufferQuad() {
//   glBindVertexArray(quadVAO);

//   mScreenShader.use();
//   glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
//   mScreenShader.setInt("colorTexture", 0);
//   mScreenShader.setFloat("time", mTime);

//   glDrawArrays(GL_TRIANGLES, 0, 6);
// }

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::present() {

  bindWindowBuffer();

  glViewport(0, 0, window_width, window_height);

  clear();

  drawFramebufferQuad();

  glfwSwapBuffers(mWindow);

  bindFramebuffer();

  glViewport(0, 0, mFramebufferSize.x, mFramebufferSize.y);

  glfwPollEvents();

  if (mLastTime < 0.0) {
    mLastTime = glfwGetTime();
  }

  double now = glfwGetTime();
  mDelta = now - mLastTime;
  mLastTime = now;

  mTime += mDelta;
}

bool Renderer::didWindowClose() {
  if (mWindow == nullptr) {
    return false;
  }
  return glfwWindowShouldClose(mWindow) == 1;
}

void Renderer::setFramebufferSize(unsigned int x, unsigned int y) {
  mFramebufferSize = {x, y};
  glfwSetWindowAspectRatio(mWindow, x, y);
  projection = glm::ortho(0.0F, x * 1.0F, 0.0F, y * 1.0F, 0.1F, 100.0F);
  loadFramebuffer(mFramebufferSize, frameBufferTexture);
}

void Renderer::OnWindowResize(GLFWwindow *window, int width, int height) {
  Renderer *renderer = (Renderer *)glfwGetWindowUserPointer(window);

  renderer->window_width = width;
  renderer->window_height = height;
}

void Renderer::bindFramebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferObj);
}

void Renderer::bindWindowBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
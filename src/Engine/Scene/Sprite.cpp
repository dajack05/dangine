#include <Engine/Scene/Sprite.hpp>

#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Scene/Animator.hpp>
#include <Engine/Util/Logger.hpp>

Sprite::Sprite() {
  // Set default shader
  setShader("sprite_default");
}

void Sprite::setShader(const std::string &filename) { shader.load(filename); }
void Sprite::setImage(const std::string &filename) { texture.load(filename); }

void Sprite::update(double delta) {
  Node::update(delta);

  // Find a better way for this?
  auto animators = getChildrenOfType<Animator>();
  if (animators.size() > 0) {
    auto size_x = 1.0F / mColumns;
    auto size_y = 1.0F / mRows;
    auto anim = animators[0]->getCurrentAnim();
    if (anim) {
      auto x = size_x * (anim->frame % mColumns);
      auto y = size_y * floor(anim->frame / mColumns);
      uv = {x, y, size_x, size_y};
    }
  }
}

void Sprite::draw(Renderer &renderer) {
  if (visible) {
    if (texture.isLoaded()) {
      renderer.drawSprite(this);
    }
  }
  Node::draw(renderer);
}
#include <Engine/Scene/Animator.hpp>

#include <Engine/Util/Logger.hpp>

void Animator::update(double delta) {
  auto anim = getCurrentAnim();
  if (anim) {
    counter = counter + delta;
    if (counter > (1.0F / anim->speed)) {
      counter = 0.0F;
      anim->frame += isBackward ? -1 : 1;
      if (anim->frame > anim->end || anim->frame < anim->start) {
        anim->frame = isBackward ? anim->end : anim->start;
      }
    }
  }
  Node::update(delta);
}

void Animator::addAnimation(const std::string &name, int start, int end,
                            int speed) {
  AnimationState s{start, start, end, speed};
  mAnimations.insert({name, s});
}

void Animator::play(const std::string &name) {
  auto anim = getAnim(name);
  if (anim) {
    mCurrentAnimIdx = name;
  }
}

void Animator::playForward(const std::string &name) {
  isBackward = false;
  play(name);
}

void Animator::playBackward(const std::string &name) {
  isBackward = true;
  play(name);
}

void Animator::stop() { mCurrentAnimIdx = -1; }

AnimationState *Animator::getAnim(const std::string &name) {
  auto result = mAnimations.find(name);
  if (result == mAnimations.end()) {
    Log::Error("Failed to find anim: \"" + name + "\"");
    return nullptr;
  }
  return &result->second;
}

AnimationState *Animator::getCurrentAnim() { return getAnim(mCurrentAnimIdx); }
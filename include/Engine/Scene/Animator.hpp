#pragma once
#include <Engine/Scene/Node.hpp>

#include <map>
#include <string>

struct AnimationState {
  int frame = 0;
  int start = 0;
  int end = 0;
  int speed = 1;
};

/*

I'd like to abstract this class further.

Ideally, I'd like to be able to assign the animator
to work on any arbitrary value.

Then you could use an animator to animate colors, positions, animations,
whatever!

*/

class Animator : public Node {
public:
  Animator() = default;

  void update(double delta);

  void addAnimation(const std::string &name, int start, int end,
                    int speed = 10);
  void playForward(const std::string &name);
  void playBackward(const std::string &name);
  AnimationState *getCurrentAnim();
  void stop();

private:
  float counter = 0.0F;
  bool isBackward = false;
  std::string mCurrentAnimIdx = "";
  std::map<std::string, AnimationState> mAnimations;

  void play(const std::string &name);

  AnimationState *getAnim(const std::string &name);
};
#include "dispatchkit/dispatchkit.hpp"
#include "dispatchkit/register_function.hpp"
#include "glm/common.hpp"
#include "utility/utility.hpp"
#include <Engine/Scripting/Register.hpp>

#include<chaiscript.hpp>
#include<glm/glm.hpp>

void RegisterMath(chaiscript::ChaiScript *context){
  context->add(chaiscript::fun([](float f){return glm::sin(f);}), "sin");
  context->add(chaiscript::fun([](float f){return glm::cos(f);}), "cos");

  context->add(chaiscript::fun([](float d){return glm::radians(d);}), "radians");
  context->add(chaiscript::fun([](float r){return glm::degrees(r);}), "degrees");

  context->add(chaiscript::fun([](float f){return (int)glm::round(f);}), "round");
  context->add(chaiscript::fun([](float f){return (int)glm::floor(f);}), "floor");
  context->add(chaiscript::fun([](float f){return (int)glm::ceil(f);}), "ceil");
      // context->register_namespace(
      // [](chaiscript::Namespace &math) {
      //   math["sin"] = chaiscript::var(chaiscript::fun([](double t) { return (double)std::sin(t); }));
      //   math["cos"] = chaiscript::var(chaiscript::fun([](double t) { return (double)std::cos(t); }));

      //   math["radians"] = chaiscript::var(chaiscript::fun([](double deg) { return (double)glm::radians(deg); }));
      //   math["radians"] = chaiscript::var(chaiscript::fun([](float deg) { return (double)glm::radians(deg); }));

      //   math["degrees"] = chaiscript::var(chaiscript::fun([](double rad) { return (double)glm::degrees(rad); }));
      //   math["degrees"] = chaiscript::var(chaiscript::fun([](float rad) { return (double)glm::degrees(rad); }));

      //   math["round"] = chaiscript::var(chaiscript::fun([](float t) { return (double)glm::round(t); }));
      //   math["round"] = chaiscript::var(chaiscript::fun([](double t) { return (double)glm::round(t); }));
      // },
      // "Math");
}
#pragma once

#include <glm/glm.hpp>

class Shader;
struct Texture;

const float QuadVerts[6*3] = {
  // X       Y      Z  
	  -0.5f,  -0.5f, -0.5f,
		 0.5f,  -0.5f, -0.5f,
		-0.5f,   0.5f, -0.5f,
                                       
	   0.5f,  -0.5f, -0.5f,
		-0.5f,   0.5f, -0.5f,
		 0.5f,   0.5f, -0.5f,
};

const float QuadUvs[6*2]={
  // U     V    
  0.0f, 1.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f,
};

struct Quad {
  Shader *shader;
  Texture *texture;
  glm::vec2 position{0, 0};
  glm::vec2 size{1, 1};
  glm::vec3 tint{1, 1, 1};
  glm::vec4 uv{0.0f,0.0f,1.0f,1.0f};
  float angle = 0.0f;
};
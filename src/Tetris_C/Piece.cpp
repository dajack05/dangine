#include <Tetris_C/Piece.hpp>

#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Util/Logger.hpp>
#include <Tetris_C/Board.hpp>
#include <ctime>
#include <glm/glm.hpp>
#include <random>

const glm::ivec2 PieceMaps[PIECE::PIECE_COUNT][4] = {
    {{0, -2}, {0, -1}, {0, 0}, {0, 1}},  // LINE
    {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},   // TEE
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},    // SQUARE
    {{0, 0}, {0, -1}, {-1, 0}, {-1, 1}}, // S
    {{0, 0}, {1, 0}, {1, 1}, {0, -1}},   // Z
    {{0, 0}, {0, 1}, {0, 2}, {1, 0}},    // L
    {{0, 0}, {0, 1}, {0, 2}, {-1, 0}},   // J
};

Piece::Piece(PIECE piece) {

  std::srand(std::time(nullptr));

  mPiece = piece;

  if (mPiece == PIECE_COUNT) {
    reSeed();
  }

  mSprite.getTexture().load("tetris_parts");
  mSprite.imageScale = 8.0F;
  mSprite.uv = {(std::rand() % 4) * 0.25F, 0.0F, 0.25F, 1.0F};
  addChild(&mSprite);
}

void Piece::update(double delta) { Node::update(delta); }

void Piece::draw(Renderer &renderer) {
  for (int i = 0; i < 4; i++) {
    auto part_pos = PieceMaps[mPiece][i];
    mSprite.transform.position = part_pos * (int)mSprite.imageScale;
    mSprite.transform.angle = -transform.angle;
    mSprite.update(1.0);
    mSprite.draw(renderer);
  }
}

void Piece::reSeed() {
  auto old_value = mPiece;

  for (int i = 0; i < std::rand() % 10; i++) {
    std::rand();
  }

  while (mPiece == old_value) {
    mPiece = PIECE(std::rand() % PIECE::PIECE_COUNT);
  }

  mSprite.uv = {(std::rand() % 4) * 0.25F, 0.0F, 0.25F, 1.0F};
}

void Piece::place(Board &board) {

  for (auto point : getPoints()) {

    glm::ivec2 _point =
        (point * glm::ivec2{1, 1}) +
        glm::ivec2(transform.position / mSprite.imageScale) * glm::ivec2(1, -1);

    if (_point.x >= 0 && _point.x < BOARD_WIDTH && _point.y >= 0 &&
        _point.y < BOARD_HEIGHT) {
      board.set(_point.x, _point.y, mSprite.uv.x * 4);
    }
  }
}

PieceSize Piece::getBounds() {
  PieceSize max{0, 0, 0, 0};

  for (auto point : getPoints()) {

    if (point.x < max.left) {
      max.left = point.x;
    } else if (point.x > max.right) {
      max.right = point.x;
    }

    if (point.y < max.bottom) {
      max.bottom = point.y;
    } else if (point.y > max.top) {
      max.top = point.y;
    }
  }

  max.top = abs(max.top);
  max.bottom = abs(max.bottom);
  max.left = abs(max.left);
  max.right = abs(max.right);

  return max;
}

std::array<glm::ivec2, 4> Piece::getPoints() {
  std::array<glm::ivec2, 4> data;
  for (int i = 0; i < 4; i++) {
    auto map = PieceMaps[mPiece][i] * glm::ivec2(1, -1);

    auto a = glm::radians(globalTransform.angle);
    data[i] = glm::ivec2( round(map.x * cos(-a)) - round(map.y * sin(-a)),
                          round(map.y * cos(-a)) + round(map.x * sin(-a)));
  }
  return data;
}
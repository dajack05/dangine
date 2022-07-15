#pragma once

#include <Engine/Scene/Sprite.hpp>
#include <array>

class Renderer;
struct Board;

enum PIECE {
  LINE,
  TEE,
  SQUARE,
  S,
  Z,
  L,
  J,
  PIECE_COUNT,
};

struct PieceSize {
  int top;
  int right;
  int bottom;
  int left;
};

class Piece : public Node {
public:
  Piece(PIECE piece = PIECE_COUNT);

  void reSeed();
  void place(Board &board);

  PieceSize getBounds();
  std::array<glm::ivec2, 4> getPoints();

  void update(double delta);
  void draw(Renderer &renderer);

  inline void setPiece(PIECE piece) { mPiece = piece; }
  inline void setVisible(const bool &visible) { mSprite.visible = visible; }

private:
  PIECE mPiece = PIECE::LINE;
  Sprite mSprite;
};
#include <Engine/Input/Input.hpp>
#include <Engine/Input/Keys.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Scene/Node.hpp>
#include <Engine/Scene/Sprite.hpp>
#include <Engine/Util/Logger.hpp>
#include <Tetris_C/Board.hpp>
#include <Tetris_C/Piece.hpp>

#define WIDTH 8 * 10
#define HEIGHT 8 * 20
#define SCALE 4

#define DEBUG

int main(int argc, char **argv) {

  Renderer renderer("Tetrus", WIDTH * SCALE, HEIGHT * SCALE);
  renderer.setFramebufferSize(WIDTH, HEIGHT);
  renderer.mCameraLocation = {36, -76};

  Node root;

  Piece piece;
  piece.transform.position = {5 * 8, 0 * 8};
  root.addChild(&piece);

  Board board;
  for (int i = 0; i < 4; i++) {
    board.set(0, 19 - i, 1);
    board.set(1, 19 - i, 1);
    board.set(2, 19 - i, 1);
    board.set(3, 19 - i, 1);
    board.set(4, 19 - i, 1);
    board.set(5, 19 - i, 1);
    board.set(6, 19 - i, 1);
    board.set(7, 19 - i, 1);
    board.set(8, 19 - i, 1);
  }

  Sprite boardSprite;
  boardSprite.setImage("tetris_parts");
  boardSprite.imageScale = 8;

  float speed = 0.2F;
  float counter = 0.0F;

  while (!renderer.didWindowClose() && !Input::IsKeyDown(KEY_ESCAPE)) {
    counter += renderer.getDelta();
    if (counter > speed) {
      piece.transform.position.y -= 8.0F;
      counter = 0;
    }

    if (Input::IsKeyJustDown(KEY_SPACE)) {
      piece.place(board);
    }

    if (Input::IsKeyJustDown(KEY_Z)) {
      piece.transform.angle += 90.0F;
    } else if (Input::IsKeyJustDown(KEY_X)) {
      piece.transform.angle -= 90.0F;
    }

    float oldXPosition = piece.transform.position.x;
    if (Input::IsKeyJustDown(KEY_RIGHT)) {
      piece.transform.position.x += 8;
    }
    if (Input::IsKeyJustDown(KEY_LEFT)) {
      piece.transform.position.x -= 8;
    }

    auto size = piece.getBounds();

    // Keep within horizonal playfield
    if (piece.transform.position.x < size.left * 8) {
      piece.transform.position.x = size.left * 8;
    } else if (piece.transform.position.x >
               (board.size.first - size.right - 1) * 8) {
      piece.transform.position.x = (board.size.first - size.right - 1) * 8;
    }

    // Allow sliding against other pieces
    for (auto point : piece.getPoints()) {
      const auto piecePosX = round(point.x + (piece.transform.position.x / 8));
      const auto piecePosY = round(point.y + (piece.transform.position.y / 8));

      // Need to cancel last X move?
      if (board.at(piecePosX, -piecePosY - 1) >= 0) {
        piece.transform.position.x = oldXPosition;
        break;
      }
    }

    if (Input::IsKeyJustDown(KEY_DOWN)) {
      counter = 1000.0F;
    }

    renderer.clear();

    renderer.update();

    root.update(renderer.getDelta());

    board.checkLines();

    root.draw(renderer);

    // Detect when we hit something
    for (const auto point : piece.getPoints()) {
      glm::ivec2 mPoint =
          (point * glm::ivec2(1, -1)) + glm::ivec2(piece.globalTransform.position / 8.0F);

#ifdef DEBUG
      glm::vec2 rPoint = (glm::vec2(mPoint) - glm::vec2(0.5)) * 8.0F;
      renderer.drawRect(rPoint, rPoint + glm::vec2(8, 8), glm::vec3{1, 0, 0});
#endif

      if (board.at(mPoint.x, -mPoint.y + 1) >= 0 ||
          -mPoint.y + 1 >= board.size.second) {
        piece.place(board);

        piece.reSeed();

        piece.transform.position = {5 * 8, 0 * 8};
        piece.update(1.0);
        break;
      }
    }

    // Draw Map
    boardSprite.globalTransform.position = {-1000, -1000};
    boardSprite.draw(renderer);
    for (int y = 0; y < board.size.second; y++) {
      for (int x = 0; x < board.size.first; x++) {
        int cell = board.at(x, y);
        if (cell < 0)
          continue;

        boardSprite.globalTransform.position = glm::vec2{x, -y} * 8.0F;
        boardSprite.uv = {cell * 0.25, 0, 0.25, 1.0};
        boardSprite.draw(renderer);

#ifdef DEBUG
        glm::vec2 rectPos = boardSprite.globalTransform.position - glm::vec2(4);
        renderer.drawRect(rectPos, rectPos + glm::vec2(8, 8), {0, 1, 0});
#endif
      }
    }

    renderer.present();
  }

  return 0;
}
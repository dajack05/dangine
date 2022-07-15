#pragma once

#include <Engine/Scene/Node.hpp>
#include <Engine/Scene/Sprite.hpp>

#include <array>
#include <memory>
#include <string>
#include <vector>

#define MAX_TILESET_SIZE 10

class Renderer;
class Sprite;
class CollisionWorld;
class CollisionBody;
class CollisionShape;

typedef std::vector<unsigned int> LayerData;

struct MapInfo {
  int map_width;
  int map_height;
  int tileset_count;
  int layer_count;
};

struct TilesetInfo {
  int index;

  std::string image;
  int id_offset;

  int tile_width;
  int tile_height;
  int tile_count;

  int rows;
  int cols;

  int image_width;
  int image_height;
};

class TileMap : public Node {
public:
  void update(double delta);
  void draw(Renderer &renderer);

  void load(const std::string &name, CollisionWorld &world);

private:
  MapInfo mMap;
  std::array<Sprite, MAX_TILESET_SIZE> mSprites;
  std::array<TilesetInfo, MAX_TILESET_SIZE> mTilesets;
  std::vector<LayerData> mLayers;

  std::vector<std::shared_ptr<CollisionBody>> bodies;
  std::vector<std::shared_ptr<CollisionShape>> shapes;

  void populateMapInfo(const std::string &line);
  void processLayers(const std::vector<std::string> &map_lines, CollisionWorld &world);
  void processTilesets(const std::vector<std::string> &map_lines);

  void processTileLayer(const std::string &line);
  void processColliderLayer(const std::string &line, CollisionWorld &world);
  void processCollider(const char &type, const int *data, CollisionWorld &world);

  int findTileset(const int &id);
};
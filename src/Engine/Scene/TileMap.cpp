#include <Engine/Scene/TileMap.hpp>

#include <Engine/Config.hpp>
#include <Engine/Physics/CollisionBody.hpp>
#include <Engine/Physics/CollisionShape.hpp>
#include <Engine/Physics/CollisionWorld.hpp>
#include <Engine/Physics/Shapes/CircleShape.hpp>
#include <Engine/Physics/Shapes/RectShape.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Util/Logger.hpp>

#include <fstream>

void TileMap::populateMapInfo(const std::string &line) {
  std::vector<std::string> parts;
  std::string s;
  for (char c : line) {
    if (c == ',') {
      parts.push_back(s);
      s = "";
    } else {
      s += c;
    }
  }

  mMap.map_width = std::stoi(parts[0]);
  mMap.map_height = std::stoi(parts[1]);
  mMap.tileset_count = std::stoi(parts[2]);
  mMap.layer_count = std::stoi(parts[3]);
}

void TileMap::processLayers(const std::vector<std::string> &map_lines,
                            CollisionWorld &world) {
  for (int i = 0; i < mMap.layer_count; i++) {
    auto layer_data = map_lines[mMap.tileset_count + 1 + i];

    char first_char = layer_data[0];
    // Ignore if commented

    switch (first_char) {
    case '#':
      break;
    case 'T':
      processTileLayer(layer_data);
      break;
    case 'C':
      processColliderLayer(layer_data, world);
      break;
    }
  }
}

void TileMap::processTileLayer(const std::string &line) {
  mLayers.push_back(LayerData{});
  LayerData &layer = mLayers[mLayers.size() - 1];

  std::string s;
  for (unsigned int x = 1; x < line.size(); x++) {
    unsigned int tileIDX = 0;

    if (line[x] == ',') {
      layer.push_back(std::stoi(s));
      s = "";
      continue;
    }

    s += line[x];
  }
}

void TileMap::processColliderLayer(const std::string &line,
                                   CollisionWorld &world) {
  std::vector<std::string> parts;
  std::string s;
  char current_type_char = ' ';
  for (unsigned int x = 1; x < line.size(); x++) {

    if (parts.size() % 4 == 0 && current_type_char == ' ') {
      current_type_char = line[x];
      continue;
    }

    if (line[x] == ',') {
      parts.push_back(s);
      s = "";

      // Time to process shape?
      if (parts.size() % 4 == 0) {
        int items[4];
        for (int i = 0; i < 4; i++) {
          items[i] = std::stoi(parts[i]);
        }
        processCollider(current_type_char, items, world);

        current_type_char = ' ';
        parts.clear();
      }

      continue;
    }

    s += line[x];
  }
}

void TileMap::processCollider(const char &type, const int *data,
                              CollisionWorld &world) {
  switch (type) {
  case 'R': {
    // Rectangle
    glm::vec2 pos = {data[0], -data[1]};
    glm::vec2 extents = {data[2] / 2.0F, data[3] / 2.0F};
    pos += extents * glm::vec2{1, -1};
    pos -= glm::vec2(8,-8);

    auto body = std::make_shared<CollisionBody>();
    body->isStatic = true;
    body->origin = pos;

    auto rect = std::make_shared<RectShape>();

    rect->extents = extents;

    body->setShape(rect.get());

    shapes.push_back(rect);

    world.addBody(body);
    bodies.push_back(body);

    Log::Debug(data[0], ",", data[1], ",", data[2], ",", data[3]);
    Log::Debug("Rect:", body->origin.x, ",", body->origin.y, ",",
               rect->extents.x, ",", rect->extents.y);

  } break;
  case 'C':
    // Circle
    glm::vec2 pos = {data[0], -data[1]};
    float radius = data[2] / 2.0F;
    pos += glm::vec2{radius, -radius};
    pos -= glm::vec2(8,-8);

    auto body = std::make_shared<CollisionBody>();
    body->isStatic = true;
    body->origin = pos;

    auto circle = std::make_shared<CircleShape>();

    circle->radius = radius;

    body->setShape(circle.get());

    shapes.push_back(circle);

    world.addBody(body);
    bodies.push_back(body);

    break;
  }
}

void TileMap::processTilesets(const std::vector<std::string> &map_lines) {
  for (int i = 0; i < mMap.tileset_count; i++) {
    auto tileset_data = map_lines[1 + i];

    Log::Debug("Creating Tileset");

    TilesetInfo *tileset = &mTilesets[i];
    Sprite *sprite = &mSprites[i];

    std::vector<std::string> parts;

    std::string s;
    for (unsigned int x = 0; x < tileset_data.size(); x++) {
      unsigned int tileIDX = 0;

      if (tileset_data[x] == ',') {
        parts.push_back(s);
        s = "";
        continue;
      }

      s += tileset_data[x];
    }

    auto ext_idx = parts[0].find_last_of(".");

    tileset->index = i;

    tileset->image = parts[0].substr(0, ext_idx);
    tileset->tile_width = std::stoi(parts[1]);
    tileset->tile_height = std::stoi(parts[2]);
    tileset->id_offset = std::stoi(parts[3]);

    sprite->setImage(tileset->image);
    auto size = sprite->getTexture().getSize();
    tileset->image_width = size.x;
    tileset->image_height = size.y;
    sprite->imageScale = tileset->tile_width;

    tileset->cols = tileset->image_width / tileset->tile_width;
    tileset->rows = tileset->image_height / tileset->tile_height;

    tileset->tile_count = tileset->rows * tileset->cols;
    Log::Debug("Done Creating Tileset");
  }
}

int TileMap::findTileset(const int &id) {
  for (int i = 0; i < mTilesets.size(); i++) {
    if (id >= mTilesets[i].id_offset &&
        id < mTilesets[i].id_offset + mTilesets[i].tile_count) {
      return i;
    }
  }

  // Default
  return 0;
}

void TileMap::load(const std::string &name, CollisionWorld &world) {
  Log::Debug("Loading \"", MAP_PATH + name + "\"");
  const auto mapPath = MAP_PATH + name + ".dum";

  std::fstream file;
  file.open(mapPath);

  if (!file.is_open()) {
    Log::Error("Failed to open ", mapPath);
    return;
  }

  std::vector<std::string> map_lines;
  std::string _line;
  while (std::getline(file, _line)) {
    map_lines.push_back(_line);
  }
  file.close();

  Log::Debug("Parsing \"", mapPath, "\"");

  populateMapInfo(map_lines[0]);

  processLayers(map_lines, world);

  processTilesets(map_lines);
}

void TileMap::update(double delta) { Node::update(delta); }

void TileMap::draw(Renderer &renderer) {
  Node::draw(renderer);

  for (auto layer : mLayers) {
    for (int y = 0; y < mMap.map_height; y++) {
      for (int x = 0; x < mMap.map_width; x++) {
        int tile_idx = layer[x + y * mMap.map_width] - 1;
        if (tile_idx >= 0) {
          auto &tileset = mTilesets[findTileset(tile_idx)];
          auto &sprite = mSprites[tileset.index];
          tile_idx = tile_idx - tileset.id_offset;

          auto tile_x = tile_idx % tileset.cols;
          auto tile_y = floor(tile_idx / tileset.cols);

          auto uv_width = 1.0F / tileset.cols;
          auto uv_height = 1.0F / tileset.rows;

          sprite.globalTransform.position =
              globalTransform.position +
              glm::vec2{x * tileset.tile_width, -y * tileset.tile_height};

          sprite.uv = {tile_x * uv_width, tile_y * uv_height, uv_width,
                       uv_height};

          sprite.draw(renderer);
        }
      }
    }
  }
}

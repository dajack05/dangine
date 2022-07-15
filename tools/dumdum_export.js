var tile_index_offsets = [];

function WriteMap(map, filename) {
    const file = new TextFile(filename, TextFile.WriteOnly);

    WriteHeader(map, file);
    WriteTilesets(map, file);

    for (let i = 0; i < map.layerCount; ++i) {
        const layer = map.layerAt(i);
        if(!layer.visible){
            continue;
        }
        if (layer.isTileLayer) {
            WriteTileLayer(layer, file);
        } else if (layer.isObjectLayer) {
            WriteColliderLayer(layer, file);
        }
    }

    file.commit();
}

function WriteHeader(map, file) {
    let layer_count = 0;

    for(let i = 0; i<map.layerCount; i++){
        if(map.layerAt(i).visible){
            layer_count++;
        }
    }

    let line = "";
    line += map.width + ",";
    line += map.height + ",";
    line += map.tilesets.length + ",";
    line += layer_count + ",";
    file.writeLine(line);
}

function WriteTilesets(map, file) {
    let offset = 0;
    for (const tileset of map.tilesets) {
        let line = "";
        const parts = tileset.image.split("/");
        line += parts[parts.length - 1] + ",";
        line += tileset.tileWidth + ",";
        line += tileset.tileHeight + ",";
        line += offset + ",";
        file.writeLine(line);

        tile_index_offsets.push(offset);

        offset += tileset.tileCount;
    }
}

const SHAPE_RECT = 0;
const SHAPE_CIRCLE = 3;
function WriteColliderLayer(layer, file) {
    let line = "C";
    for (const obj of layer.objects) {
        switch (obj.shape) {
            case SHAPE_RECT:
                line += `R${obj.x},${obj.y},${obj.width},${obj.height},`;
                break;
            case SHAPE_CIRCLE:
                line += `C${obj.x},${obj.y},${obj.width},${obj.height},`;
                break;
        }
    }
    file.writeLine(line);
}

function WriteTileLayer(layer, file) {
    let line = "T";
    for (let y = 0; y < layer.height; y++) {
        for (let x = 0; x < layer.width; x++) {
            const tile = layer.tileAt(x, y);
            if (tile) {
                const tileset_index = layer.map.tilesets.indexOf(tile.tileset);
                const tileset_offset = tile_index_offsets[tileset_index];
                line += (tile.id + 1 + tileset_offset) + ",";
            } else {
                line += "0,";
            }
        }
    }
    file.writeLine(line);
}

tiled.registerMapFormat("dumdum", {
    name: "DumDum Tilemap Format",
    extension: "dum",
    write: WriteMap,
});
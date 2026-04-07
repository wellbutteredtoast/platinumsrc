//
// asset_types.hpp -> Forward declarations and definitions for all asset types
//

#ifndef ASSET_TYPES_HPP
#define ASSET_TYPES_HPP

#include "lm/vmath.h"

/**
 * @brief A loaded texture asset.
 * @todo Define texture format, dimensions, and raw data storage.
 */
struct asset_texture {
    unsigned int width;
    unsigned int height;
    unsigned char* data; // owned by this struct
};

/**
 * @brief A loaded 3D mesh asset.
 * @todo Define vertex/index buffer layout.
 */
struct asset_mesh {
    // vertex and index data goes here
};

/**
 * @brief A loaded audio asset.
 * @todo Decide between fully-loaded vs. streaming audio.
 * @note Streaming is recommended for music on <512MB RAM targets.
 */
struct asset_audio {
    // audio PCM data or stream handle goes here
};

/**
 * @brief A loaded level/map asset.
 * @todo Define level geometry, entity spawn list, and metadata.
 */
struct asset_level {
    // level geometry and entity data goes here
};

#endif // ASSET_TYPES_HPP
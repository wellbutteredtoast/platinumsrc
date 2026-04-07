//
// asset_loader.hpp -> Per-type loading interface
//
// Implement one concrete loader per asset type.
// Loaders are stateless; they just know how to read a file
// and produce an asset struct. The cache owns the result.
//

#ifndef ASSET_LOADER_HPP
#define ASSET_LOADER_HPP

#include "asset_types.hpp"

/**
 * @brief Abstract base for all asset loaders.
 *
 * Follows a similar pattern to base_entity -- extend per asset type.
 * Loaders are stateless and should not hold onto loaded data themselves.
 *
 * @tparam T The asset type this loader produces.
 */
template<typename T>
class asset_loader {
    public:
        /**
         * @brief Loads an asset from disk by file path.
         * @param path Null-terminated file path string. Never null.
         * @return Heap-allocated asset on success, NULL on failure.
         * @note Caller (the cache) takes ownership of the returned pointer.
         */
        virtual T* load(const char* path) = 0;

        /**
         * @brief Unloads a previously loaded asset.
         * @param asset The asset to destroy. Never null.
         * @note The cache calls this -- do not call directly.
         */
        virtual void unload(T* asset) = 0;

        virtual ~asset_loader() {}
};

/** @brief Loader for asset_texture. */
class texture_loader : public asset_loader<asset_texture> {
    public:
        asset_texture* load(const char* path);
        void unload(asset_texture* asset);
};

/** @brief Loader for asset_mesh. */
class mesh_loader : public asset_loader<asset_mesh> {
    public:
        asset_mesh* load(const char* path);
        void unload(asset_mesh* asset);
};

/** @brief Loader for asset_audio. */
class audio_loader : public asset_loader<asset_audio> {
    public:
        asset_audio* load(const char* path);
        void unload(asset_audio* asset);
};

/** @brief Loader for asset_level. */
class level_loader : public asset_loader<asset_level> {
    public:
        asset_level* load(const char* path);
        void unload(asset_level* asset);
};

#endif // ASSET_LOADER_HPP
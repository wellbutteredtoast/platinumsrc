//
// asset_mgr/asset_handle.hpp -> Lightweight handle to a managed asset
//
// Handles are the only way user code should reference assets.
// Never hold raw pointers to assets -- the cache owns them.
//

#ifndef ASSET_HANDLE_HPP
#define ASSET_HANDLE_HPP

#include <stddef.h> // NULL

/**
 * @brief Lightweight non-owning handle to a cache-managed asset.
 *
 * Wraps a raw pointer to an asset owned by the asset cache.
 * Handles do not control asset lifetime -- the cache does.
 * A handle can become invalid if the cache unloads the asset;
 * always check is_valid() before dereferencing in non-hot paths.
 *
 * @tparam T The asset type this handle refers to, e.g. asset_texture.
 *
 * @note Intentionally kept minimal for compat and resource use.
 */
template<typename T>
class asset_handle {
    public:
        /**
         * @brief Constructs an invalid (null) handle.
         */
        asset_handle() : ptr(NULL) {}

        /**
         * @brief Constructs a handle from a raw cache-owned pointer.
         * @param p Pointer to an asset owned by the asset cache. May be NULL.
         */
        explicit asset_handle(T* p) : ptr(p) {}

        /**
         * @brief Returns whether this handle points to a valid asset.
         * @return True if the handle is non-null.
         */
        bool is_valid() const { return ptr != NULL; }

        /**
         * @brief Dereferences the handle to access the asset.
         * Behaviour is undefined if the handle is invalid.
         * @return Reference to the managed asset.
         */
        T& get() const { return *ptr; }

        /**
         * @brief Dereferences the handle to access the asset.
         * Behaviour is undefined if the handle is invalid.
         * @return Pointer to the managed asset.
         */
        T* operator->() const { return ptr; }

        /**
         * @brief Invalidates this handle by setting it to null.
         * Does not unload the asset -- that is the cache's responsibility.
         */
        void invalidate() { ptr = NULL; }

    private:
        T* ptr;
};

#endif // ASSET_HANDLE_HPP
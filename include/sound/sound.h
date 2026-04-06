//
// sound.h -> OpenAL abstraction layer for PlatinumSrc
//
//   Wraps device/context lifecycle, buffer management, and positional
//   source control into a flat C-style API.
//
//   Typical usage:
//       snd_ctx ctx;
//       snd_init(&ctx, NULL);          // NULL = default device
//
//       snd_buf buf;
//       snd_buf_load(&buf, AL_FORMAT_MONO16, pcm_data, pcm_size, 44100);
//
//       snd_src src;
//       snd_src_create(&src);
//       snd_src_bind(&src, &buf);
//       snd_src_set_gain(&src, 1.0f);
//       snd_src_play(&src);
//
//       // ... later ...
//       snd_src_destroy(&src);
//       snd_buf_free(&buf);
//       snd_quit(&ctx);
//

#ifndef _SOUND_H
#define _SOUND_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** Wraps the ALC device + context pair that underpins all AL calls. */
typedef struct {
    ALCdevice  *device;
    ALCcontext *ctx;
} snd_ctx;

/** A PCM audio buffer uploaded to the AL driver. */
typedef struct {
    ALuint  id;     /* AL buffer object handle */
    ALenum  format; /* e.g. AL_FORMAT_MONO16, AL_FORMAT_STEREO16 */
    ALsizei freq;   /* sample rate in Hz */
} snd_buf;

/** A positional sound emitter. */
typedef struct {
    ALuint id; /* AL source object handle */
} snd_src;

// ---------------------------------------------------------------------------
// System init / shutdown
// ---------------------------------------------------------------------------

/**
 * @brief Open an audio device and make its context current.
 *
 * @param ctx         Output context; must not be NULL.
 * @param device_name Name of the ALC device to open, or NULL for the default.
 * @return 0 on success, -1 on failure.
 */
int snd_init(snd_ctx *ctx, const char *device_name);

/**
 * @brief Destroy the context and close the device.
 *
 * @param ctx Previously initialised context.
 */
void snd_quit(snd_ctx *ctx);

// ---------------------------------------------------------------------------
// Buffer management
// ---------------------------------------------------------------------------

/**
 * @brief Upload raw PCM data into an AL buffer.
 *
 * @param buf    Output buffer object; must not be NULL.
 * @param format AL format constant (AL_FORMAT_MONO16, AL_FORMAT_STEREO16, …).
 * @param data   Pointer to PCM sample data.
 * @param size   Size of data in bytes.
 * @param freq   Sample rate in Hz.
 * @return 0 on success, -1 on failure.
 */
int snd_buf_load(snd_buf *buf, ALenum format, const void *data, ALsizei size, ALsizei freq);

/**
 * @brief Release the AL buffer and zero the struct.
 *
 * @param buf Buffer to free.
 */
void snd_buf_free(snd_buf *buf);

// ---------------------------------------------------------------------------
// Source lifecycle
// ---------------------------------------------------------------------------

/**
 * @brief Allocate a new AL source.
 *
 * @param src Output source; must not be NULL.
 * @return 0 on success, -1 on failure.
 */
int snd_src_create(snd_src *src);

/**
 * @brief Delete the AL source and zero the struct.
 *
 * @param src Source to destroy.
 */
void snd_src_destroy(snd_src *src);

// ---------------------------------------------------------------------------
// Source configuration
// ---------------------------------------------------------------------------

/**
 * @brief Attach a buffer to a source (replaces any previously bound buffer).
 *
 * @param src Source to configure.
 * @param buf Buffer to bind; pass NULL to detach.
 */
void snd_src_bind(snd_src *src, const snd_buf *buf);

/**
 * @brief Set the world-space position of a source.
 */
void snd_src_set_pos(snd_src *src, float x, float y, float z);

/**
 * @brief Set the velocity of a source (used for Doppler).
 */
void snd_src_set_vel(snd_src *src, float x, float y, float z);

/**
 * @brief Set the linear gain (volume) of a source.  1.0 = unattenuated.
 */
void snd_src_set_gain(snd_src *src, float gain);

/**
 * @brief Set the pitch multiplier of a source.  1.0 = normal.
 */
void snd_src_set_pitch(snd_src *src, float pitch);

/**
 * @brief Enable or disable looping for a source.
 *
 * @param loop Non-zero to enable looping, 0 to disable.
 */
void snd_src_set_loop(snd_src *src, int loop);

/**
 * @brief Query the current playback state of a source.
 *
 * @return One of: AL_INITIAL, AL_PLAYING, AL_PAUSED, AL_STOPPED.
 */
ALenum snd_src_state(const snd_src *src);

// ---------------------------------------------------------------------------
// Source playback control
// ---------------------------------------------------------------------------

/** Start or resume playback. */
void snd_src_play(snd_src *src);

/** Pause playback (resumable with snd_src_play). */
void snd_src_pause(snd_src *src);

/** Stop playback and rewind to the beginning. */
void snd_src_stop(snd_src *src);

// ---------------------------------------------------------------------------
// Listener
// ---------------------------------------------------------------------------

/**
 * @brief Set the listener world-space position.
 */
void snd_listener_set_pos(float x, float y, float z);

/**
 * @brief Set the listener velocity (used for Doppler).
 */
void snd_listener_set_vel(float x, float y, float z);

/**
 * @brief Set the listener orientation.
 *
 * @param at Forward direction vector (length 3).
 * @param up Up direction vector (length 3).
 */
void snd_listener_set_ori(const float at[3], const float up[3]);

/**
 * @brief Set the listener master gain.  1.0 = unattenuated.
 */
void snd_listener_set_gain(float gain);

#endif /* _SOUND_H */

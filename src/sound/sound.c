//
// sound.c -> OpenAL abstraction layer implementation
//

#include "sound/sound.h"

#include <string.h>

// ---------------------------------------------------------------------------
// System init / shutdown
// ---------------------------------------------------------------------------

int snd_init(snd_ctx *ctx, const char *device_name) {
    ctx->device = alcOpenDevice(device_name);
    if (!ctx->device) return -1;

    ctx->ctx = alcCreateContext(ctx->device, NULL);
    if (!ctx->ctx) {
        alcCloseDevice(ctx->device);
        ctx->device = NULL;
        return -1;
    }

    alcMakeContextCurrent(ctx->ctx);
    return 0;
}

void snd_quit(snd_ctx *ctx) {
    alcMakeContextCurrent(NULL);
    if (ctx->ctx)    { alcDestroyContext(ctx->ctx);   ctx->ctx   = NULL; }
    if (ctx->device) { alcCloseDevice(ctx->device);   ctx->device = NULL; }
}

// ---------------------------------------------------------------------------
// Buffer management
// ---------------------------------------------------------------------------

int snd_buf_load(snd_buf *buf, ALenum format, const void *data, ALsizei size, ALsizei freq) {
    alGenBuffers(1, &buf->id);
    if (alGetError() != AL_NO_ERROR) return -1;

    alBufferData(buf->id, format, data, size, freq);
    if (alGetError() != AL_NO_ERROR) {
        alDeleteBuffers(1, &buf->id);
        buf->id = 0;
        return -1;
    }

    buf->format = format;
    buf->freq   = freq;
    return 0;
}

void snd_buf_free(snd_buf *buf) {
    if (buf->id) {
        alDeleteBuffers(1, &buf->id);
        memset(buf, 0, sizeof(*buf));
    }
}

// ---------------------------------------------------------------------------
// Source lifecycle
// ---------------------------------------------------------------------------

int snd_src_create(snd_src *src) {
    alGenSources(1, &src->id);
    return (alGetError() == AL_NO_ERROR) ? 0 : -1;
}

void snd_src_destroy(snd_src *src) {
    if (src->id) {
        alDeleteSources(1, &src->id);
        src->id = 0;
    }
}

// ---------------------------------------------------------------------------
// Source configuration
// ---------------------------------------------------------------------------

void snd_src_bind(snd_src *src, const snd_buf *buf) {
    alSourcei(src->id, AL_BUFFER, buf ? (ALint)buf->id : AL_NONE);
}

void snd_src_set_pos(snd_src *src, float x, float y, float z) {
    alSource3f(src->id, AL_POSITION, x, y, z);
}

void snd_src_set_vel(snd_src *src, float x, float y, float z) {
    alSource3f(src->id, AL_VELOCITY, x, y, z);
}

void snd_src_set_gain(snd_src *src, float gain) {
    alSourcef(src->id, AL_GAIN, gain);
}

void snd_src_set_pitch(snd_src *src, float pitch) {
    alSourcef(src->id, AL_PITCH, pitch);
}

void snd_src_set_loop(snd_src *src, int loop) {
    alSourcei(src->id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

ALenum snd_src_state(const snd_src *src) {
    ALint state;
    alGetSourcei(src->id, AL_SOURCE_STATE, &state);
    return (ALenum)state;
}

// ---------------------------------------------------------------------------
// Source playback control
// ---------------------------------------------------------------------------

void snd_src_play(snd_src *src)  { alSourcePlay(src->id);  }
void snd_src_pause(snd_src *src) { alSourcePause(src->id); }
void snd_src_stop(snd_src *src)  { alSourceStop(src->id);  }

// ---------------------------------------------------------------------------
// Listener
// ---------------------------------------------------------------------------

void snd_listener_set_pos(float x, float y, float z) {
    alListener3f(AL_POSITION, x, y, z);
}

void snd_listener_set_vel(float x, float y, float z) {
    alListener3f(AL_VELOCITY, x, y, z);
}

void snd_listener_set_ori(const float at[3], const float up[3]) {
    float ori[6] = { at[0], at[1], at[2], up[0], up[1], up[2] };
    alListenerfv(AL_ORIENTATION, ori);
}

void snd_listener_set_gain(float gain) {
    alListenerf(AL_GAIN, gain);
}

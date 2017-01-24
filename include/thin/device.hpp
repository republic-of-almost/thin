#ifndef THIN_INCLUDED_
#define THIN_INCLUDED_


#include <stdint.h>
#include <stddef.h>


#ifdef THIN_API_OGL

struct Device
{

// ---------------------------------------------------------------[ General ]--

void
initialize();

// ------------------------------------------------------------------[ Misc ]--


// -------------------------------------------------------------------[ VAO ]--

uintptr_t
genVertexArray();

void
genVertexArrays(const size_t count, uintptr_t vaos[]);

uintptr_t
createVertexArray();

void
createVertexArrays(const size_t count, uintptr_t vaos[]);

void
bindVertexArray(const uintptr_t vao);

void
deleteVertexArray(const uinptr_t vao);

void
deleteVertexArrays(const size_t count, uintptr_t vaos[]);

// -----------------------------------------------------------------[ Clear ]--

void
clearColor(const float r, const float g, const float b, const float a = 1.f);

void
clearColor(const float color[4]);

void
clear(const GLbitfield mask);

void
clear(const bool color, const bool depth, const bool stencil = false);

void
clearColorBuffer();

void
clearDepthBuffer();

void
clearStencilBuffer();

void
getClearColor(float out_color[4]);

// ---------------------------------------------------------------[ Texture ]--

uintptr_t
genTexture();

void
genTextures(const size_t count, uintptr_t textures[]);

uintptr_t
createTexture();

void
createTextures(const size_t count, uintptr_t textures[]);

void
bindTexture(const GLenum target, const uintptr_t texture);

void
bindTextures(const GLenum first_target, const size_t count, const uintptr_t textures[]);

void
deleteTexture(const uintptr_t texture);

void
deleteTextures(const size_t count, const uintptr_t textures[]);

// ---------------------------------------------------------------[ Shaders ]--


// ---------------------------------------------------------------[ Buffers ]--

uintptr_t
genBuffer();

void
genBuffers(const size_t count, uintptr_t buffers[]);

uintptr_t
createBuffer();

void
createBuffers(const size_t count, uintptr_t buffers[]);

void
bindBuffer(const GLenum target, const uintptr_t buffer);

void
bufferData(const GLeum target, const GLsizeiptr size, const GLvoid *data , const GLenum useage);

// --------------------------------------------------------[ Vertex Attribs ]--

};

#endif // thin ogl


#endif // inc guard

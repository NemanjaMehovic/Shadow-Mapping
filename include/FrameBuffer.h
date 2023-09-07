#pragma once


class FrameBuffer
{
    unsigned int frameBuffer;
    unsigned int textureColorBuffer;
    unsigned int textureDepthBuffer;
public:
    static void UseDefaultBuffer();
    FrameBuffer(const unsigned int& SCR_WIDTH, const unsigned int& SCR_HEIGHT);
    ~FrameBuffer();
    void useBuffer() const;
    unsigned int getRgbTexture() const;
    unsigned int getDepthTexture() const;
};
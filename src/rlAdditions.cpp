#include "rlAdditions.h"

#include "rlgl.h"
#include "raymath.h"

// Initializes a camery using a View & Projection matrix
void BeginViewProjectionMode(Matrix view, Matrix projection)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
    rlLoadIdentity();               // Reset current matrix (projection)

    rlMultMatrixf(MatrixToFloat(projection));

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

    rlMultMatrixf(MatrixToFloat(view));      // Multiply modelview matrix by view matrix (camera)

    rlEnableDepthTest();            // Enable DEPTH_TEST for 3D
}

// Ends View/Projection mode returns to default 2D orthographic mode
void EndViewProjectionMode(void)
{
    EndMode3D();
    // rlDrawRenderBatchActive();      // Update and draw internal render batch

    // rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    // rlPopMatrix();                  // Restore previous matrix (projection) from matrix stack

    // rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    // rlLoadIdentity();               // Reset current matrix (modelview)

    // rlMultMatrixf(MatrixToFloat(CORE.Window.screenScale)); // Apply screen scaling if required

    // rlDisableDepthTest();           // Disable DEPTH_TEST for 2D
}

RenderTexture2D LoadShadowMapTexture(int width, int height) {
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(width, height);   // Load an empty framebuffer

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create color texture (default to RGBA)
        target.texture.id = rlLoadTexture(NULL, width, height, RL_PIXELFORMAT_UNCOMPRESSED_R32, 1);
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = RL_PIXELFORMAT_UNCOMPRESSED_R32;
        target.texture.mipmaps = 1;

        // Create depth renderbuffer/texture
        target.depth.id = rlLoadTextureDepth(width, height, true);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach color texture and depth renderbuffer/texture to FBO
        rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

        rlDisableFramebuffer();
    }
    else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

    return target;
}
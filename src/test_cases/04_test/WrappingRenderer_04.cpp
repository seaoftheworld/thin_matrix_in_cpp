#include "WrappingRenderer_04.h"

void WrappingRenderer_04::specificSettingsOn() {
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE); // Usually enabled by default in driver

    // glEnable(GL_CULL_FACE);  // requried the model to be enclosed, otherwise
    // glCullFace(GL_BACK);     // will not be correctly displayed ???
    // glCullFace(GL_FRONT);

    // glEnable(GL_CLIP_DISTANCE0);  // cannot be turned off after enabled in some drivers ???
}

void WrappingRenderer_04::specificSettingsOff() {
}

#include "Core/Renderers/Model/ModelRendererMultiLights.h"

class WrappingRenderer_02 {
    std::vector<Light *> lights;

public:

    WrappingRenderer_02() {
        lights.clear();
    }
    ~WrappingRenderer_02() {
        lights.clear();
    }

    void addLight(Light *light) {
        lights.push_back(light);
    }

public:
    MultiLightsEntityRenderer entityRenderer;

    void specificSettingsOn() {
        glClearColor(0.7f, 0.7f, 0.8f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_MULTISAMPLE); // Usually enabled by default in driver

        // glEnable(GL_CULL_FACE);  // requried the model to be enclosed, otherwise
        // glCullFace(GL_BACK);     // will not be correctly displayed ???
        // glCullFace(GL_FRONT);
    }
    void specificSettingsOff() {}
    
    void process() {
        prepare();
        entityRenderer.run(lights, NULL);
    }

private:
    void prepare() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

#pragma once

// #include "Core/Renderers/Model/ModelRendererBasic.h"
// #include "Core/Renderers/Model/ModelRendererSpecular.h"
#include "Core/Renderers/Model/ModelRendererMultiLights.h"

#include "Core/Renderers/Terrain/TerrainRenderer.h"
#include "Core/Renderers/Skybox/SkyboxRenderer.h"
#include "Core/Renderers/Gui/GuiRenderer.h"

#define PROFILING (0)
#include "Core/profile.h"

class WrappingRenderer {  // need a base-wrapping-renderer ???

    std::vector<Light *> lights;

public:
    WrappingRenderer() {
        lights.clear();
    }
    ~WrappingRenderer() {
        lights.clear();
    }

    void addLights(Light *light) {
        lights.push_back(light);
    }

public:
    // BasicEntityRenderer   entityRenderer;
    // SpecularEntityRenderer   entityRenderer;
    MultiLightsEntityRenderer entityRenderer;

    TerrainRenderer terrainRenderer;
    SkyboxRenderer   skyboxRenderer;
    GuiRenderer         guiRenderer;
    // WaterRenderer water;

    void specificSettingsOff();
    void specificSettingsOn();

    void process(Light &light) {
        prepare();
            // entityRenderer.run();
            // entityRenderer.run(*lights[0]);
            entityRenderer.run(lights);

            // Light test_light; {
            //     // float position[Light::Position::max_pos] = {0.0f, 1.0f, 2.0f};
            //     // float position[Light::Position::max_pos] = {20.0f, 0.0f, 20.0f};
            //     float position[Light::Position::max_pos] = {10.0f, 10.0f, 20.0f};
            //     // float color[Light::Color::max_color] = {1.6f, 1.2f, 1.6f};
            //     float color[Light::Color::max_color] = { 1.6f, 1.6f, 1.6f };
            //     float dummy_attenuation[Light::Attenuation::max_att] = {0.0f, 0.0f, 0.0f};
            //     test_light.setValues(&position, &color, &dummy_attenuation);
            // }
            // terrainRenderer.run(test_light);

            terrainRenderer.run(lights);

            skyboxRenderer.run();
            guiRenderer.run();
    }

private:
    void prepare() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

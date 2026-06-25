#pragma once
#include <vector>
#include <memory>
#include "Model.hpp"

class Scene {
public:
    void addModel(std::shared_ptr<Model> model) {
        models.push_back(model);
    }

    void draw(GLuint shader) const {
        for (auto& model : models)
            model->draw(shader);
    }

    void drawOcclude(GLuint shader) const {
        for (auto& model : models)
            model->drawOcclude(shader);
    }

    std::vector<std::shared_ptr<Model>> models;
};

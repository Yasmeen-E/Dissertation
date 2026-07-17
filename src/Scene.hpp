#pragma once
#include <vector>
#include <memory>
#include "Model.hpp"
#include "PlaneModel.hpp"

class Scene {
public:
    void addModel(std::shared_ptr<Model> model) {
        models.push_back(model);
    }

     void addGround(std::shared_ptr<GroundPlane> ground) {
        grounds.push_back(ground);
    }


    void draw(GLuint shader) const {
        for (auto& model : models)
            model->draw(shader);
            for (auto& ground : grounds)
            ground->draw(shader);
    }

    void drawOcclude(GLuint shader) const {
        for (auto& model : models)
            model->drawOcclude(shader);
        for (auto& ground : grounds)
            ground->drawOcclude(shader);
    }

    std::vector<std::shared_ptr<Model>> models;
    std::vector<std::shared_ptr<GroundPlane>> grounds;
};

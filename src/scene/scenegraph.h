/*
 * Copyright (c) 2020 Vsevolod Kremianskii
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "glm/vec3.hpp"

#include "../render/types.h"

#include "octree.h"

namespace reone {

namespace scene {

class CameraSceneNode;
class LightSceneNode;
class ModelNodeSceneNode;
class SceneNode;

class SceneGraph : public render::IRenderable {
public:
    SceneGraph(const render::GraphicsOptions &opts);

    void render() const override;

    void clear();

    void addRoot(const std::shared_ptr<SceneNode> &node);
    void removeRoot(const std::shared_ptr<SceneNode> &node);

    void build();
    void prepareFrame();

    void setActiveCamera(const std::shared_ptr<CameraSceneNode> &camera);

    // Lights

    void getLightsAt(const glm::vec3 &position, std::vector<LightSceneNode *> &lights) const;

    const glm::vec3 &ambientLightColor() const;
    void setAmbientLightColor(const glm::vec3 &color);

    // END Lights

private:
    render::GraphicsOptions _opts;
    std::vector<std::shared_ptr<SceneNode>> _roots;
    std::vector<ModelNodeSceneNode *> _opaqueMeshes;
    std::vector<ModelNodeSceneNode *> _transparentMeshes;
    std::vector<LightSceneNode *> _lights;
    std::shared_ptr<CameraSceneNode> _activeCamera;
    glm::vec3 _ambientLightColor { 0.5f };
    uint32_t _textureId { 0 };
    Octree _octree;

    SceneGraph(const SceneGraph &) = delete;
    SceneGraph &operator=(const SceneGraph &) = delete;

    void refreshMeshesAndLights();
};

} // namespace scene

} // namespace reone

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

#include "selectoverlay.h"

#include "../../render/font.h"
#include "../../render/fonts.h"
#include "../../render/mesh/quad.h"
#include "../../render/shaders.h"
#include "../../render/texture.h"
#include "../../render/textures.h"
#include "../../resource/resources.h"

#include "../game.h"

#include "colors.h"

using namespace std;

using namespace reone::render;
using namespace reone::resource;

namespace reone {

namespace game {

static const int kTitleBarWidth = 200;
static const int kTitleBarPadding = 3;

SelectionOverlay::SelectionOverlay(Game *game) : _game(game) {
    if (!game) {
        throw invalid_argument("game must not be null");
    }
}

void SelectionOverlay::load() {
    _font = Fonts::instance().get("dialogfont16x16");
    _friendlyReticle = Textures::instance().get("friendlyreticle", TextureType::GUI);
    _friendlyReticle2 = Textures::instance().get("friendlyreticle2", TextureType::GUI);
    _reticleHeight = _friendlyReticle2->height();
}

void SelectionOverlay::update() {
    shared_ptr<Area> area(_game->module()->area());
    ObjectSelector &selector = area->objectSelector();

    Camera *camera = _game->getActiveCamera();
    glm::mat4 projection(camera->sceneNode()->projection());
    glm::mat4 view(camera->sceneNode()->view());

    int hilightedObjectId = selector.hilightedObjectId();
    if (hilightedObjectId != -1) {
        _hilightedScreenCoords = area->getSelectableScreenCoords(hilightedObjectId, projection, view);
        _hasHilighted = _hilightedScreenCoords.z < 1.0f;
    } else {
        _hasHilighted = false;
    }

    int selectedObjectId = selector.selectedObjectId();
    if (selectedObjectId != -1) {
        _selectedScreenCoords = area->getSelectableScreenCoords(selectedObjectId, projection, view);
        _selectedTitle = area->find(selectedObjectId)->title();
        _hasSelected = _selectedScreenCoords.z < 1.0f;
    } else {
        _hasSelected = false;
    }
}

void SelectionOverlay::render() const {
    if (_hasHilighted) {
        drawReticle(*_friendlyReticle, _hilightedScreenCoords);
    }
    if (_hasSelected) {
        drawReticle(*_friendlyReticle2, _selectedScreenCoords);
        drawTitleBar();
    }
}

void SelectionOverlay::drawReticle(Texture &texture, const glm::vec3 &screenCoords) const {
    const GraphicsOptions &opts = _game->options().graphics;
    int width = texture.width();
    int height = texture.height();

    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3((opts.width * screenCoords.x) - width / 2, (opts.height * (1.0f - screenCoords.y)) - height / 2, 0.0f));
    transform = glm::scale(transform, glm::vec3(width, height, 1.0f));

    LocalUniforms locals;
    locals.model = move(transform);

    Shaders::instance().activate(ShaderProgram::GUIGUI, locals);

    texture.bind(0);

    Quad::getDefault().renderTriangles();

    texture.unbind(0);
}

void SelectionOverlay::drawTitleBar() const {
    if (_selectedTitle.empty()) return;

    const GraphicsOptions &opts = _game->options().graphics;
    float barHeight = _font->height() + kTitleBarPadding;
    {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(opts.width * _selectedScreenCoords.x - kTitleBarWidth / 2, opts.height * (1.0f - _selectedScreenCoords.y) - _reticleHeight / 2 - barHeight, 0.0f));
        transform = glm::scale(transform, glm::vec3(kTitleBarWidth, barHeight, 1.0f));

        LocalUniforms locals;
        locals.model = move(transform);
        locals.color = glm::vec3(0.0f);
        locals.alpha = 0.5f;

        Shaders::instance().activate(ShaderProgram::GUIWhite, locals);

        Quad::getDefault().renderTriangles();
    }
    {
        glm::mat4 transform(1.0f);
        transform = glm::translate(
            transform,
            glm::vec3(
                opts.width * _selectedScreenCoords.x,
                opts.height * (1.0f - _selectedScreenCoords.y) - (_reticleHeight + barHeight) / 2,
                0.0f));

        glm::vec3 color(getBaseColor(_game->version()));

        _font->render(_selectedTitle, transform, color);
    }
}

} // namespace game

} // namespace reone

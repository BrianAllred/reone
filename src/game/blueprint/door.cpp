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

#include "door.h"

#include <boost/algorithm/string.hpp>

using namespace std;

using namespace reone::resource;

namespace reone {

namespace game {

void DoorBlueprint::load(const GffStruct &utd) {
    _tag = utd.getString("Tag");
    boost::to_lower(_tag);

    _lockable = utd.getInt("Lockable", 0) != 0;
    _locked = utd.getInt("Locked", 0) != 0;
    _genericType = utd.getInt("GenericType");
    _static = utd.getInt("Static", 0) != 0;

    _onOpen = utd.getString("OnOpen");
    _onFailToOpen = utd.getString("OnFailToOpen");
}

bool DoorBlueprint::isLockable() const {
    return _lockable;
}

bool DoorBlueprint::isLocked() const {
    return _locked;
}

bool DoorBlueprint::isStatic() const {
    return _static;
}

const string &DoorBlueprint::tag() const {
    return _tag;
}

int DoorBlueprint::genericType() const {
    return _genericType;
}

const string &DoorBlueprint::onOpen() const {
    return _onOpen;
}

const string &DoorBlueprint::onFailToOpen() const {
    return _onFailToOpen;
}

} // namespace game

} // namespace reone

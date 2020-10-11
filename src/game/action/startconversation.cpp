/*
 * Copyright � 2020 Vsevolod Kremianskii
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

#include "startconversation.h"

using namespace std;

namespace reone {

namespace game {

StartConversationAction::StartConversationAction(const shared_ptr<Object> &object, const string &dialogResRef) :
    ObjectAction(ActionType::StartConversation, object),
    _dialogResRef(dialogResRef) {
}

const string &StartConversationAction::dialogResRef() const {
    return _dialogResRef;
}

} // namespace game

} // namespace reone

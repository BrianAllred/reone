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

#include "streamwriter.h"

#include <cstring>
#include <stdexcept>

#include "endianutil.h"

using namespace std;

namespace reone {

StreamWriter::StreamWriter(const shared_ptr<ostream> &stream, Endianess endianess) :
    _stream(stream),
    _endianess(endianess) {

    if (!stream) {
        throw invalid_argument("stream must not be null");
    }
}

void StreamWriter::putByte(uint8_t val) {
    _stream->put(val);
}

void StreamWriter::putInt64(int64_t val) {
    put(val);
}

void StreamWriter::putCString(const string &str) {
    int len = strnlen(&str[0], str.length());
    _stream->write(&str[0], len);
    _stream->put('\0');
}

template <class T>
void StreamWriter::put(T val) {
    fixEndianess(val);
    char buf[sizeof(T)];
    memcpy(buf, &val, sizeof(T));
    _stream->write(buf, sizeof(T));
}

template <class T>
void StreamWriter::fixEndianess(T &val) {
    if (!isSameEndianess()) {
        swapBytes(val);
    }
}

bool StreamWriter::isSameEndianess() const {
    return _endianess == Endianess::Little;
}

} // namespace reone

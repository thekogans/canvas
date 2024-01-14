// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_canvas.
//
// libthekogans_canvas is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_canvas is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_canvas. If not, see <http://www.gnu.org/licenses/>.

#include <cmath>
#include <algorithm>
#include "thekogans/util/Types.h"
#include "thekogans/canvas/RGBAColor.h"
#include "thekogans/canvas/HSLAColor.h"
#include "thekogans/canvas/HSLAConverter.h"

namespace thekogans {
    namespace canvas {

        template<>
        f32HSLAColor Converter<f32HSLAColor>::Convert (const f32HSLAColor &inColor) {
            return inColor;
        }

        template<>
        f32HSLAColor Converter<f32HSLAColor>::Convert (const f32RGBAColor &inColor) {
            util::f32 r = inColor.r;
            util::f32 g = inColor.g;
            util::f32 b = inColor.b;
            util::f32 min = std::min (r, std::min (g, b));
            util::f32 max = std::max (r, std::max (g, b));
            util::f32 delta = max - min;
            util::f32 l = (max + min) / 2.0f;
            if (delta == 0.0f) {
                return f32HSLAColor (0.0f, 0.0f, l, inColor.a);
            }
            else {
                util::f32 s = l < 0.5f ? delta / (max + min) : delta / (1.0f - std::abs (2.0f * l - 1.0f));
                util::f32 h = r == max ? (g - b) / delta : g == max ? (b - r) / delta + 2.0f : (r - g) / delta + 4.0f;
                return f32HSLAColor (
                    fmod (60.0f * h + 360.0f, 360.0f),
                    s * 100.f,
                    l * 100.f,
                    inColor.a);
            }
        }

    } // namespace canvas
} // namespace thekogans

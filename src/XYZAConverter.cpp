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
#include "thekogans/util/Types.h"
#include "thekogans/canvas/RGBAColor.h"
#include "thekogans/canvas/XYZAColor.h"
#include "thekogans/canvas/XYZAConverter.h"

namespace thekogans {
    namespace canvas {

        template<>
        f32XYZAColor Converter<f32XYZAColor>::Convert (const f32XYZAColor &inColor) {
            return inColor;
        }

        template<>
        f32XYZAColor Converter<f32XYZAColor>::Convert (const f32RGBAColor &inColor) {
            util::f32 r = inColor.r;
            util::f32 g = inColor.g;
            util::f32 b = inColor.b;
            r = ((r > 0.04045f) ? pow ((r + 0.055f) / 1.055f, 2.4f) : (r / 12.92f)) * 100.0f;
            g = ((g > 0.04045f) ? pow ((g + 0.055f) / 1.055f, 2.4f) : (g / 12.92f)) * 100.0f;
            b = ((b > 0.04045f) ? pow ((b + 0.055f) / 1.055f, 2.4f) : (b / 12.92f)) * 100.0f;
            return f32XYZAColor (
                r * 0.4124564f + g * 0.3575761f + b * 0.1804375f,
                r * 0.2126729f + g * 0.7151522f + b * 0.0721750f,
                r * 0.0193339f + g * 0.1191920f + b * 0.9503041f,
                inColor.a);
        }

    } // namespace canvas
} // namespace thekogans

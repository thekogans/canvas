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
#include "thekogans/canvas/RGBAConverter.h"

namespace thekogans {
    namespace canvas {

        template<>
        f32RGBAColor Converter<f32RGBAColor>::Convert (const f32RGBAColor &inColor) {
            return inColor;
        }

        template<>
        f32RGBAColor Converter<f32RGBAColor>::Convert (const f32XYZAColor &inColor) {
            util::f32 x = inColor.x / 100.0f;
            util::f32 y = inColor.y / 100.0f;
            util::f32 z = inColor.z / 100.0f;
            util::f32 r = x * 3.2404542f + y * -1.5371385f + z * -0.4985314f;
            util::f32 g = x * -0.9692660f + y * 1.8760108f + z * 0.0415560f;
            util::f32 b = x * 0.0556434f + y * -0.2040259f + z * 1.0572252f;
            r = ((r > 0.0031308f) ? (1.055f * pow (r, 1.0f / 2.4f) - 0.055) : (12.92f * r)) * 255.0f;
            g = ((g > 0.0031308f) ? (1.055f * pow (g, 1.0f / 2.4f) - 0.055) : (12.92f * g)) * 255.0f;
            b = ((b > 0.0031308f) ? (1.055f * pow (b, 1.0f / 2.4f) - 0.055) : (12.92f * b)) * 255.0f;
            return f32RGBAColor (r, g, b, inColor.a);
        }

    } // namespace canvas
} // namespace thekogans

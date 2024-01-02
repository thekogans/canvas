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
#include "thekogans/canvas/HSLAColor.h"
#include "thekogans/canvas/RGBAConverter.h"

namespace thekogans {
    namespace canvas {

        template<>
        ui8RGBAColor Converter<ui8RGBAColor>::Convert (const f32RGBAColor &inColor) {
            return ui8RGBAColor (
                (typename ui8RGBAColor::ComponentType)(inColor.r * 255.0f),
                (typename ui8RGBAColor::ComponentType)(inColor.g * 255.0f),
                (typename ui8RGBAColor::ComponentType)(inColor.b * 255.0f),
                (typename ui8RGBAColor::ComponentType)(inColor.a * 255.0f));
        }

        namespace {
            inline util::ui8 Convertui16Toui8 (util::ui16 value) {
                static const util::ui8 masks[4] = {0, 0, 1, 1};
                return
                    (masks[(value >> 0) & 3] << 0) |
                    (masks[(value >> 2) & 3] << 1) |
                    (masks[(value >> 4) & 3] << 2) |
                    (masks[(value >> 6) & 3] << 3) |
                    (masks[(value >> 8) & 3] << 4) |
                    (masks[(value >> 10) & 3] << 5) |
                    (masks[(value >> 12) & 3] << 6) |
                    (masks[(value >> 14) & 3] << 7);
            }
        }

        template<>
        ui8RGBAColor Converter<ui8RGBAColor>::Convert (const ui16RGBAColor &inColor) {
            return ui8RGBAColor (
                Convertui16Toui8 (inColor.r),
                Convertui16Toui8 (inColor.g),
                Convertui16Toui8 (inColor.b),
                Convertui16Toui8 (inColor.a));
        };

        template<>
        f32RGBAColor Converter<f32RGBAColor>::Convert (const ui8RGBAColor &inColor) {
            return f32RGBAColor (inColor.r / 255.f, inColor.g, inColor.b, inColor.a);
        }

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
            r = r > 0.0031308f ? 1.055f * pow (r, 1.0f / 2.4f) - 0.055f : 12.92f * r;
            g = g > 0.0031308f ? 1.055f * pow (g, 1.0f / 2.4f) - 0.055f : 12.92f * g;
            b = b > 0.0031308f ? 1.055f * pow (b, 1.0f / 2.4f) - 0.055f : 12.92f * b;
            return f32RGBAColor (r, g, b, inColor.a);
        }

        namespace {
            util::f32 Hue_2_RGB (
                    util::f32 v1,
                    util::f32 v2,
                    util::f32 vh) {
                if (vh < 0.0f) {
                    vh += 1.0f;
                }
                if (vh > 1.0f) {
                    vh -= 1.0f;
                }
                if (6.0f * vh < 1.0f) {
                    return v1 + (v2 - v1) * 6.0f * vh;
                }
                if (2.0f * vh < 1.0f) {
                    return v2;
                }
                if (3.0f * vh < 2.0f) {
                    return v1 + (v2 - v1) * (2.0f / 3.0f - vh) * 6.0f;
                }
                return v1;
            }
        }

        template<>
        f32RGBAColor Converter<f32RGBAColor>::Convert (const f32HSLAColor &inColor) {
            util::f32 h = inColor.h / 360.0f;
            util::f32 s = inColor.s / 100.0f;
            util::f32 l = inColor.l / 100.0f;
            if (s == 0.0f) {
                return f32RGBAColor (l, l, l, inColor.a);
            }
            else {
                util::f32 temp2 = l < 0.5f ? l + l * s : l + s - l * s;
                util::f32 temp1 = 2.0f * l - temp2;
                return f32RGBAColor (
                    Hue_2_RGB (temp1, temp2, h + 1.0f / 3.0f),
                    Hue_2_RGB (temp1, temp2, h),
                    Hue_2_RGB (temp1, temp2, h - 1.0f / 3.0f),
                    inColor.a);
            }
        }

    } // namespace canvas
} // namespace thekogans

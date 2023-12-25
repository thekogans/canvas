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

#if !defined (__thekogans_canvas_Pixel_h)
#define __thekogans_canvas_Pixel_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct RGBAPixel {
            typedef T ComponentType;

            ComponentType r;
            ComponentType g;
            ComponentType b;
            ComponentType a;

            RGBAPixel (const Color<ComponentType> &color = Color<ComponentType> ()) :
                r (color.r),
                g (color.g),
                b (color.b),
                a (color.a) {}

            inline Color<ComponentType> ToColor () const {
                return Color<ComponentType> (r, g, b, a);
            }

            inline RGBAPixel &operator = (const Color<ComponentType> &color) {
                r = color.r;
                g = color.g;
                b = color.b;
                a = color.a;
                return *this;
            }
        };

        typedef RGBAPixel<util::ui8> ui8RGBAPixel;
        typedef RGBAPixel<util::ui16> ui16RGBAPixel;
        typedef RGBAPixel<util::ui32> ui32RGBAPixel;
        typedef RGBAPixel<util::ui64> ui64RGBAPixel;
        typedef RGBAPixel<util::f32> f32RGBAPixel;
        typedef RGBAPixel<util::f64> f64RGBAPixel;

        /// \brief
        /// Validate assumptions about RGBAPixel component packing.
        static_assert (
            sizeof (ui8RGBAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16RGBAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32RGBAPixel) ==  THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64RGBAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32RGBAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64RGBAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about RGBAPixel component packing.");

        template<typename T>
        struct BGRAPixel {
            typedef T ComponentType;

            ComponentType b;
            ComponentType g;
            ComponentType r;
            ComponentType a;

            BGRAPixel (const Color<ComponentType> &color = Color<ComponentType> ()) :
                b (color.b),
                g (color.g),
                r (color.r),
                a (color.a) {}

            inline Color<ComponentType> ToColor () const {
                return Color<ComponentType> (r, g, b, a);
            }

            inline BGRAPixel &operator = (const Color<ComponentType> &color) {
                b = color.b;
                g = color.g;
                r = color.r;
                a = color.a;
                return *this;
            }
        };

        typedef BGRAPixel<util::ui8> ui8BGRAPixel;
        typedef BGRAPixel<util::ui16> ui16BGRAPixel;
        typedef BGRAPixel<util::ui32> ui32BGRAPixel;
        typedef BGRAPixel<util::ui64> ui64BGRAPixel;
        typedef BGRAPixel<util::f32> f32BGRAPixel;
        typedef BGRAPixel<util::f64> f64BGRAPixel;

        /// \brief
        /// Validate assumptions about BGRAPixel component packing.
        static_assert (
            sizeof (ui8BGRAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16BGRAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32BGRAPixel) ==  THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64BGRAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32BGRAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64BGRAPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about BGRAPixel component packing.");

        template<typename T>
        struct ARGBPixel {
            typedef T ComponentType;

            ComponentType a;
            ComponentType r;
            ComponentType g;
            ComponentType b;

            ARGBPixel (const Color<ComponentType> &color = Color<ComponentType> ()) :
                a (color.a),
                r (color.r),
                g (color.g),
                b (color.b) {}

            inline Color<ComponentType> ToColor () const {
                return Color<ComponentType> (r, g, b, a);
            }

            inline ARGBPixel &operator = (const Color<ComponentType> &color) {
                a = color.a;
                r = color.r;
                g = color.g;
                b = color.b;
                return *this;
            }
        };

        typedef ARGBPixel<util::ui8> ui8ARGBPixel;
        typedef ARGBPixel<util::ui16> ui16ARGBPixel;
        typedef ARGBPixel<util::ui32> ui32ARGBPixel;
        typedef ARGBPixel<util::ui64> ui64ARGBPixel;
        typedef ARGBPixel<util::f32> f32ARGBPixel;
        typedef ARGBPixel<util::f64> f64ARGBPixel;

        /// \brief
        /// Validate assumptions about ARGBPixel component packing.
        static_assert (
            sizeof (ui8ARGBPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16ARGBPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32ARGBPixel) ==  THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64ARGBPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32ARGBPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64ARGBPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about ARGBPixel component packing.");

        template<typename T>
        struct ABGRPixel {
            typedef T ComponentType;

            ComponentType a;
            ComponentType b;
            ComponentType g;
            ComponentType r;

            ABGRPixel (const Color<ComponentType> &color = Color<ComponentType> ()) :
                a (color.a),
                b (color.b),
                g (color.g),
                r (color.r) {}

            inline Color<ComponentType> ToColor () const {
                return Color<ComponentType> (r, g, b, a);
            }

            inline ABGRPixel &operator = (const Color<ComponentType> &color) {
                a = color.a;
                b = color.b;
                g = color.g;
                r = color.r;
                return *this;
            }
        };

        typedef ABGRPixel<util::ui8> ui8ABGRPixel;
        typedef ABGRPixel<util::ui16> ui16ABGRPixel;
        typedef ABGRPixel<util::ui32> ui32ABGRPixel;
        typedef ABGRPixel<util::ui64> ui64ABGRPixel;
        typedef ABGRPixel<util::f32> f32ABGRPixel;
        typedef ABGRPixel<util::f64> f64ABGRPixel;

        /// \brief
        /// Validate assumptions about ABGRPixel component packing.
        static_assert (
            sizeof (ui8ABGRPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16ABGRPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32ABGRPixel) ==  THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64ABGRPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32ABGRPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64ABGRPixel) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about ABGRPixel component packing.");

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Pixel_h)

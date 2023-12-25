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

        typedef RGBAPixel<util::ui8> RGBAPixelui8;
        typedef RGBAPixel<util::ui16> RGBAPixelui16;
        typedef RGBAPixel<util::ui32> RGBAPixelui32;
        typedef RGBAPixel<util::ui64> RGBAPixelui64;
        typedef RGBAPixel<util::f32> RGBAPixelf32;
        typedef RGBAPixel<util::f64> RGBAPixelf64;

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

        typedef BGRAPixel<util::ui8> BGRAPixelui8;
        typedef BGRAPixel<util::ui16> BGRAPixelui16;
        typedef BGRAPixel<util::ui32> BGRAPixelui32;
        typedef BGRAPixel<util::ui64> BGRAPixelui64;
        typedef BGRAPixel<util::f32> BGRAPixelf32;
        typedef BGRAPixel<util::f64> BGRAPixelf64;

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

        typedef ARGBPixel<util::ui8> ARGBPixelui8;
        typedef ARGBPixel<util::ui16> ARGBPixelui16;
        typedef ARGBPixel<util::ui32> ARGBPixelui32;
        typedef ARGBPixel<util::ui64> ARGBPixelui64;
        typedef ARGBPixel<util::f32> ARGBPixelf32;
        typedef ARGBPixel<util::f64> ARGBPixelf64;

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

        typedef ABGRPixel<util::ui8> ABGRPixelui8;
        typedef ABGRPixel<util::ui16> ABGRPixelui16;
        typedef ABGRPixel<util::ui32> ABGRPixelui32;
        typedef ABGRPixel<util::ui64> ABGRPixelui64;
        typedef ABGRPixel<util::f32> ABGRPixelf32;
        typedef ABGRPixel<util::f64> ABGRPixelf64;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Pixel_h)

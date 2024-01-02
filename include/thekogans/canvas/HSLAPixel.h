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

#if !defined (__thekogans_canvas_HSLAPixel_h)
#define __thekogans_canvas_HSLAPixel_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/HSLAColor.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct HSLAPixel {
            typedef T ComponentType;
            typedef HSLAColor<ComponentType> ColorType;
            typedef f32HSLAColor ConverterColorType;

            ComponentType h;
            ComponentType s;
            ComponentType l;
            ComponentType a;

            /// \brief
            /// ctor.
            /// NOTE: We don't initialize anything here. We want to be as close as we can to POTs.
            /// Smart compilers will understand that and will elide the ctor when we allocate large
            /// arrays of pixels (see \see{Framebuffer}). It's undestood that initialization will be
            /// performed soon aftor allocation and doing it twice would just be a waste of time.
            HSLAPixel () {}
            HSLAPixel (const ColorType &color) :
                h (color.h),
                s (color.s),
                l (color.l),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (h, s, l, a);
            }

            inline HSLAPixel &operator = (const ColorType &color) {
                h = color.h;
                s = color.s;
                l = color.l;
                a = color.a;
                return *this;
            }
        };

        typedef HSLAPixel<util::f32> f32HSLAPixel;

        template<typename T>
        struct LSHAPixel {
            typedef T ComponentType;
            typedef HSLAColor<ComponentType> ColorType;
            typedef f32HSLAColor ConverterColorType;

            ComponentType l;
            ComponentType s;
            ComponentType h;
            ComponentType a;

            /// \brief
            /// ctor.
            LSHAPixel () {}
            LSHAPixel (const ColorType &color) :
                l (color.l),
                s (color.s),
                h (color.h),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (h, s, l, a);
            }

            inline LSHAPixel &operator = (const ColorType &color) {
                l = color.l;
                s = color.s;
                h = color.h;
                a = color.a;
                return *this;
            }
        };

        typedef LSHAPixel<util::f32> f32LSHAPixel;

        template<typename T>
        struct AHSLPixel {
            typedef T ComponentType;
            typedef HSLAColor<ComponentType> ColorType;
            typedef f32HSLAColor ConverterColorType;

            ComponentType a;
            ComponentType h;
            ComponentType s;
            ComponentType l;

            /// \brief
            /// ctor.
            AHSLPixel () {}
            AHSLPixel (const ColorType &color) :
                a (color.a),
                h (color.h),
                s (color.s),
                l (color.l) {}

            inline ColorType ToColor () const {
                return ColorType (h, s, l, a);
            }

            inline AHSLPixel &operator = (const ColorType &color) {
                a = color.a;
                h = color.h;
                s = color.s;
                l = color.l;
                return *this;
            }
        };

        typedef AHSLPixel<util::f32> f32AHSLPixel;

        template<typename T>
        struct ALSHPixel {
            typedef T ComponentType;
            typedef HSLAColor<ComponentType> ColorType;
            typedef f32HSLAColor ConverterColorType;

            ComponentType a;
            ComponentType l;
            ComponentType s;
            ComponentType h;

            /// \brief
            /// ctor.
            ALSHPixel () {}
            ALSHPixel (const ColorType &color) :
                a (color.a),
                l (color.l),
                s (color.s),
                h (color.h) {}

            inline ColorType ToColor () const {
                return ColorType (h, s, l, a);
            }

            inline ALSHPixel &operator = (const ColorType &color) {
                a = color.a;
                l = color.l;
                s = color.s;
                h = color.h;
                return *this;
            }
        };

        typedef ALSHPixel<util::f32> f32ALSHPixel;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_HSLAPixel_h)

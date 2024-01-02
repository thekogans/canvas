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

#if !defined (__thekogans_canvas_XYZAPixel_h)
#define __thekogans_canvas_XYZAPixel_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/XYZAColor.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct XYZAPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef f32XYZAColor ConverterColorType;

            ComponentType x;
            ComponentType y;
            ComponentType z;
            ComponentType a;

            /// \brief
            /// ctor.
            /// NOTE: We don't initialize anything here. We want to be as close as we can to POTs.
            /// Smart compilers will understand that and will elide the ctor when we allocate large
            /// arrays of pixels (see \see{Framebuffer}). It's undestood that initialization will be
            /// performed soon aftor allocation and doing it twice would just be a waste of time.
            XYZAPixel () {}
            XYZAPixel (const ColorType &color) :
                x (color.x),
                y (color.y),
                z (color.z),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (x, y, z, a);
            }

            inline XYZAPixel &operator = (const ColorType &color) {
                x = color.x;
                y = color.y;
                z = color.z;
                a = color.a;
                return *this;
            }
        };

        typedef XYZAPixel<util::f32> f32XYZAPixel;

        template<typename T>
        struct ZYXAPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef f32XYZAColor ConverterColorType;

            ComponentType z;
            ComponentType y;
            ComponentType x;
            ComponentType a;

            /// \brief
            /// ctor.
            ZYXAPixel () {}
            ZYXAPixel (const ColorType &color) :
                z (color.z),
                y (color.y),
                x (color.x),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (x, y, z, a);
            }

            inline ZYXAPixel &operator = (const ColorType &color) {
                x = color.z;
                y = color.y;
                x = color.x;
                a = color.a;
                return *this;
            }
        };

        typedef ZYXAPixel<util::f32> f32ZYXAPixel;

        template<typename T>
        struct AXYZPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef f32XYZAColor ConverterColorType;

            ComponentType a;
            ComponentType x;
            ComponentType y;
            ComponentType z;

            /// \brief
            /// ctor.
            AXYZPixel () {}
            AXYZPixel (const ColorType &color) :
                a (color.a),
                x (color.x),
                y (color.y),
                z (color.z) {}

            inline ColorType ToColor () const {
                return ColorType (x, y, z, a);
            }

            inline AXYZPixel &operator = (const ColorType &color) {
                a = color.a;
                x = color.x;
                y = color.y;
                z = color.z;
                return *this;
            }
        };

        typedef AXYZPixel<util::f32> f32AXYZPixel;

        template<typename T>
        struct AZYXPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef f32XYZAColor ConverterColorType;

            ComponentType a;
            ComponentType z;
            ComponentType y;
            ComponentType x;

            /// \brief
            /// ctor.
            AZYXPixel () {}
            AZYXPixel (const ColorType &color) :
                a (color.a),
                z (color.z),
                y (color.y),
                x (color.x) {}

            inline ColorType ToColor () const {
                return ColorType (x, y, z, a);
            }

            inline AZYXPixel &operator = (const ColorType &color) {
                a = color.a;
                z = color.z;
                y = color.y;
                x = color.x;
                return *this;
            }
        };

        typedef AZYXPixel<util::f32> f32AZYXPixel;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAPixel_h)

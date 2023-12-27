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
#include "thekogans/canvas/Framebuffer.h"
#include "thekogans/canvas/Frame.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct XYZAPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef XYZAColor<util::f32> ConverterColorType;

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

        typedef XYZAPixel<util::ui8> ui8XYZAPixel;
        typedef XYZAPixel<util::ui16> ui16XYZAPixel;
        typedef XYZAPixel<util::ui32> ui32XYZAPixel;
        typedef XYZAPixel<util::ui64> ui64XYZAPixel;
        typedef XYZAPixel<util::f32> f32XYZAPixel;
        typedef XYZAPixel<util::f64> f64XYZAPixel;

        typedef Framebuffer<ui8XYZAPixel> ui8XYZAFramebuffer;
        typedef Framebuffer<ui16XYZAPixel> ui16XYZAFramebuffer;
        typedef Framebuffer<ui32XYZAPixel> ui32XYZAFramebuffer;
        typedef Framebuffer<ui64XYZAPixel> ui64XYZAFramebuffer;
        typedef Framebuffer<f32XYZAPixel> f32XYZAFramebuffer;
        typedef Framebuffer<f64XYZAPixel> f64XYZAFramebuffer;

        typedef Frame<ui8XYZAPixel> ui8XYZAFrame;
        typedef Frame<ui16XYZAPixel> ui16XYZAFrame;
        typedef Frame<ui32XYZAPixel> ui32XYZAFrame;
        typedef Frame<ui64XYZAPixel> ui64XYZAFrame;
        typedef Frame<f32XYZAPixel> f32XYZAFrame;
        typedef Frame<f64XYZAPixel> f64XYZAFrame;

        template<typename T>
        struct ZYXAPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef XYZAColor<util::f32> ConverterColorType;

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

        typedef ZYXAPixel<util::ui8> ui8ZYXAPixel;
        typedef ZYXAPixel<util::ui16> ui16ZYXAPixel;
        typedef ZYXAPixel<util::ui32> ui32ZYXAPixel;
        typedef ZYXAPixel<util::ui64> ui64ZYXAPixel;
        typedef ZYXAPixel<util::f32> f32ZYXAPixel;
        typedef ZYXAPixel<util::f64> f64ZYXAPixel;

        typedef Framebuffer<ui8ZYXAPixel> ui8ZYXAFramebuffer;
        typedef Framebuffer<ui16ZYXAPixel> ui16ZYXAFramebuffer;
        typedef Framebuffer<ui32ZYXAPixel> ui32ZYXAFramebuffer;
        typedef Framebuffer<ui64ZYXAPixel> ui64ZYXAFramebuffer;
        typedef Framebuffer<f32ZYXAPixel> f32ZYXAFramebuffer;
        typedef Framebuffer<f64ZYXAPixel> f64ZYXAFramebuffer;

        typedef Frame<ui8ZYXAPixel> ui8ZYXAFrame;
        typedef Frame<ui16ZYXAPixel> ui16ZYXAFrame;
        typedef Frame<ui32ZYXAPixel> ui32ZYXAFrame;
        typedef Frame<ui64ZYXAPixel> ui64ZYXAFrame;
        typedef Frame<f32ZYXAPixel> f32ZYXAFrame;
        typedef Frame<f64ZYXAPixel> f64ZYXAFrame;

        template<typename T>
        struct AXYZPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef XYZAColor<util::f32> ConverterColorType;

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

        typedef AXYZPixel<util::ui8> ui8AXYZPixel;
        typedef AXYZPixel<util::ui16> ui16AXYZPixel;
        typedef AXYZPixel<util::ui32> ui32AXYZPixel;
        typedef AXYZPixel<util::ui64> ui64AXYZPixel;
        typedef AXYZPixel<util::f32> f32AXYZPixel;
        typedef AXYZPixel<util::f64> f64AXYZPixel;

        typedef Framebuffer<ui8AXYZPixel> ui8AXYZFramebuffer;
        typedef Framebuffer<ui16AXYZPixel> ui16AXYZFramebuffer;
        typedef Framebuffer<ui32AXYZPixel> ui32AXYZFramebuffer;
        typedef Framebuffer<ui64AXYZPixel> ui64AXYZFramebuffer;
        typedef Framebuffer<f32AXYZPixel> f32AXYZFramebuffer;
        typedef Framebuffer<f64AXYZPixel> f64AXYZFramebuffer;

        typedef Frame<ui8AXYZPixel> ui8AXYZFrame;
        typedef Frame<ui16AXYZPixel> ui16AXYZFrame;
        typedef Frame<ui32AXYZPixel> ui32AXYZFrame;
        typedef Frame<ui64AXYZPixel> ui64AXYZFrame;
        typedef Frame<f32AXYZPixel> f32AXYZFrame;
        typedef Frame<f64AXYZPixel> f64AXYZFrame;

        template<typename T>
        struct AZYXPixel {
            typedef T ComponentType;
            typedef XYZAColor<ComponentType> ColorType;
            typedef XYZAColor<util::f32> ConverterColorType;

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

        typedef AZYXPixel<util::ui8> ui8AZYXPixel;
        typedef AZYXPixel<util::ui16> ui16AZYXPixel;
        typedef AZYXPixel<util::ui32> ui32AZYXPixel;
        typedef AZYXPixel<util::ui64> ui64AZYXPixel;
        typedef AZYXPixel<util::f32> f32AZYXPixel;
        typedef AZYXPixel<util::f64> f64AZYXPixel;

        typedef Framebuffer<ui8AZYXPixel> ui8AZYXFramebuffer;
        typedef Framebuffer<ui16AZYXPixel> ui16AZYXFramebuffer;
        typedef Framebuffer<ui32AZYXPixel> ui32AZYXFramebuffer;
        typedef Framebuffer<ui64AZYXPixel> ui64AZYXFramebuffer;
        typedef Framebuffer<f32AZYXPixel> f32AZYXFramebuffer;
        typedef Framebuffer<f64AZYXPixel> f64AZYXFramebuffer;

        typedef Frame<ui8AZYXPixel> ui8AZYXFrame;
        typedef Frame<ui16AZYXPixel> ui16AZYXFrame;
        typedef Frame<ui32AZYXPixel> ui32AZYXFrame;
        typedef Frame<ui64AZYXPixel> ui64AZYXFrame;
        typedef Frame<f32AZYXPixel> f32AZYXFrame;
        typedef Frame<f64AZYXPixel> f64AZYXFrame;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAPixel_h)

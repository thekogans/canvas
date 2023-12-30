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

#if !defined (__thekogans_canvas_YUVAPixel_h)
#define __thekogans_canvas_YUVAPixel_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/YUVAColor.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct YUVAPixel {
            typedef T ComponentType;
            typedef YUVAColor<ComponentType> ColorType;
            typedef f32YUVAColor ConverterColorType;

            ComponentType y;
            ComponentType u;
            ComponentType v;
            ComponentType a;

            /// \brief
            /// ctor.
            /// NOTE: We don't initialize anything here. We want to be as close as we can to POTs.
            /// Smart compilers will understand that and will elide the ctor when we allocate large
            /// arrays of pixels (see \see{Framebuffer}). It's undestood that initialization will be
            /// performed soon aftor allocation and doing it twice would just be a waste of time.
            YUVAPixel () {}
            YUVAPixel (const ColorType &color) :
                y (color.y),
                u (color.u),
                v (color.v),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline YUVAPixel &operator = (const ColorType &color) {
                y = color.y;
                u = color.u;
                v = color.v;
                a = color.a;
                return *this;
            }
        };

        typedef YUVAPixel<util::ui8> ui8YUVAPixel;
        typedef YUVAPixel<util::ui16> ui16YUVAPixel;
        typedef YUVAPixel<util::ui32> ui32YUVAPixel;
        typedef YUVAPixel<util::ui64> ui64YUVAPixel;
        typedef YUVAPixel<util::f32> f32YUVAPixel;
        typedef YUVAPixel<util::f64> f64YUVAPixel;

        /// \brief
        /// Validate assumptions about ui8YUVAPixel component packing.
        /// This check is only important for ui8 yuva framebuffers as these
        /// are used in opengl and windowing systems. Many of these have
        /// specific alignment (4 byte boundary) requeirements. Since
        /// we want to be able to directly write our framebuffers to these
        /// systems, we need to make sure we satisfy their requirements.
        /// Framebuffers built with pixel types which use other color
        /// spaces are particularly important to applications doing image
        /// manipulation and as such will not have alignment requirements.
        const std::size_t THEKOGANS_CANVAS_YUVAPIXEL_COMPONENT_COUNT = 4;
        static_assert (
            sizeof (ui8YUVAPixel) == THEKOGANS_CANVAS_YUVAPIXEL_COMPONENT_COUNT * util::UI8_SIZE,
            "Invalid assumption about ui8YUVAPixel component packing.");

        template<typename T>
        struct VUYAPixel {
            typedef T ComponentType;
            typedef YUVAColor<ComponentType> ColorType;
            typedef f32YUVAColor ConverterColorType;

            ComponentType v;
            ComponentType u;
            ComponentType y;
            ComponentType a;

            /// \brief
            /// ctor.
            VUYAPixel () {}
            VUYAPixel (const ColorType &color) :
                v (color.v),
                u (color.u),
                y (color.y),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline VUYAPixel &operator = (const ColorType &color) {
                v = color.v;
                u = color.u;
                y = color.y;
                a = color.a;
                return *this;
            }
        };

        typedef VUYAPixel<util::ui8> ui8VUYAPixel;
        typedef VUYAPixel<util::ui16> ui16VUYAPixel;
        typedef VUYAPixel<util::ui32> ui32VUYAPixel;
        typedef VUYAPixel<util::ui64> ui64VUYAPixel;
        typedef VUYAPixel<util::f32> f32VUYAPixel;
        typedef VUYAPixel<util::f64> f64VUYAPixel;

        template<typename T>
        struct AYUVPixel {
            typedef T ComponentType;
            typedef YUVAColor<ComponentType> ColorType;
            typedef f32YUVAColor ConverterColorType;

            ComponentType a;
            ComponentType y;
            ComponentType u;
            ComponentType v;

            /// \brief
            /// ctor.
            AYUVPixel () {}
            AYUVPixel (const ColorType &color) :
                a (color.a),
                y (color.y),
                u (color.u),
                v (color.v) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline AYUVPixel &operator = (const ColorType &color) {
                a = color.a;
                y = color.y;
                u = color.u;
                v = color.v;
                return *this;
            }
        };

        typedef AYUVPixel<util::ui8> ui8AYUVPixel;
        typedef AYUVPixel<util::ui16> ui16AYUVPixel;
        typedef AYUVPixel<util::ui32> ui32AYUVPixel;
        typedef AYUVPixel<util::ui64> ui64AYUVPixel;
        typedef AYUVPixel<util::f32> f32AYUVPixel;
        typedef AYUVPixel<util::f64> f64AYUVPixel;

        template<typename T>
        struct AVUYPixel {
            typedef T ComponentType;
            typedef YUVAColor<ComponentType> ColorType;
            typedef f32YUVAColor ConverterColorType;

            ComponentType a;
            ComponentType v;
            ComponentType u;
            ComponentType y;

            /// \brief
            /// ctor.
            AVUYPixel () {}
            AVUYPixel (const ColorType &color) :
                a (color.a),
                v (color.v),
                u (color.u),
                y (color.y) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline AVUYPixel &operator = (const ColorType &color) {
                a = color.a;
                v = color.v;
                u = color.u;
                y = color.y;
                return *this;
            }
        };

        typedef AVUYPixel<util::ui8> ui8AVUYPixel;
        typedef AVUYPixel<util::ui16> ui16AVUYPixel;
        typedef AVUYPixel<util::ui32> ui32AVUYPixel;
        typedef AVUYPixel<util::ui64> ui64AVUYPixel;
        typedef AVUYPixel<util::f32> f32AVUYPixel;
        typedef AVUYPixel<util::f64> f64AVUYPixel;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_YUVAPixel_h)

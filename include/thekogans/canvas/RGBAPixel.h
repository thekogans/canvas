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

#if !defined (__thekogans_canvas_RGBAPixel_h)
#define __thekogans_canvas_RGBAPixel_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/RGBAColor.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct RGBAPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef f32RGBAColor ConverterColorType;

            ComponentType r;
            ComponentType g;
            ComponentType b;
            ComponentType a;

            /// \brief
            /// ctor.
            /// NOTE: We don't initialize anything here. We want to be as close as we can to POTs.
            /// Smart compilers will understand that and will elide the ctor when we allocate large
            /// arrays of pixels (see \see{Framebuffer}). It's undestood that initialization will be
            /// performed soon aftor allocation and doing it twice would just be a waste of time.
            RGBAPixel () {}
            RGBAPixel (const ColorType &color) :
                r (color.r),
                g (color.g),
                b (color.b),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline RGBAPixel &operator = (const ColorType &color) {
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
        typedef RGBAPixel<util::f32> f32RGBAPixel;

        /// \brief
        /// Validate assumptions about ui8RGBAPixel component packing.
        /// This check is only important for ui8 rgba framebuffers as these
        /// are used in opengl and windowing systems. Many of these have
        /// specific alignment (4 byte boundary) requeirements. Since
        /// we want to be able to directly write our framebuffers to these
        /// systems, we need to make sure we satisfy their requirements.
        /// Framebuffers built with pixel types which use other color
        /// spaces are particularly important to applications doing image
        /// manipulation and as such will not have alignment requirements.
        const std::size_t THEKOGANS_CANVAS_RGBAPIXEL_COMPONENT_COUNT = 4;
        static_assert (
            sizeof (ui8RGBAPixel) == THEKOGANS_CANVAS_RGBAPIXEL_COMPONENT_COUNT * util::UI8_SIZE,
            "Invalid assumption about ui8RGBAPixel component packing.");

        template<typename T>
        struct BGRAPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef f32RGBAColor ConverterColorType;

            ComponentType b;
            ComponentType g;
            ComponentType r;
            ComponentType a;

            /// \brief
            /// ctor.
            BGRAPixel () {}
            BGRAPixel (const ColorType &color) :
                b (color.b),
                g (color.g),
                r (color.r),
                a (color.a) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline BGRAPixel &operator = (const ColorType &color) {
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
        typedef BGRAPixel<util::f32> f32BGRAPixel;

        template<typename T>
        struct ARGBPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef f32RGBAColor ConverterColorType;

            ComponentType a;
            ComponentType r;
            ComponentType g;
            ComponentType b;

            /// \brief
            /// ctor.
            ARGBPixel () {}
            ARGBPixel (const ColorType &color) :
                a (color.a),
                r (color.r),
                g (color.g),
                b (color.b) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline ARGBPixel &operator = (const ColorType &color) {
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
        typedef ARGBPixel<util::f32> f32ARGBPixel;

        template<typename T>
        struct ABGRPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef f32RGBAColor ConverterColorType;

            ComponentType a;
            ComponentType b;
            ComponentType g;
            ComponentType r;

            /// \brief
            /// ctor.
            ABGRPixel () {}
            ABGRPixel (const ColorType &color) :
                a (color.a),
                b (color.b),
                g (color.g),
                r (color.r) {}

            inline ColorType ToColor () const {
                return ColorType (r, g, b, a);
            }

            inline ABGRPixel &operator = (const ColorType &color) {
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
        typedef ABGRPixel<util::f32> f32ABGRPixel;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBAPixel_h)

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
#include "thekogans/canvas/Framebuffer.h"
#include "thekogans/canvas/Frame.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct RGBAPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef RGBAColor<util::f32> ConverterColorType;

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
        typedef RGBAPixel<util::ui64> ui64RGBAPixel;
        typedef RGBAPixel<util::f32> f32RGBAPixel;
        typedef RGBAPixel<util::f64> f64RGBAPixel;

        typedef Framebuffer<ui8RGBAPixel> ui8RGBAFramebuffer;
        typedef Framebuffer<ui16RGBAPixel> ui16RGBAFramebuffer;
        typedef Framebuffer<ui32RGBAPixel> ui32RGBAFramebuffer;
        typedef Framebuffer<ui64RGBAPixel> ui64RGBAFramebuffer;
        typedef Framebuffer<f32RGBAPixel> f32RGBAFramebuffer;
        typedef Framebuffer<f64RGBAPixel> f64RGBAFramebuffer;

        typedef Frame<ui8RGBAPixel> ui8RGBAFrame;
        typedef Frame<ui16RGBAPixel> ui16RGBAFrame;
        typedef Frame<ui32RGBAPixel> ui32RGBAFrame;
        typedef Frame<ui64RGBAPixel> ui64RGBAFrame;
        typedef Frame<f32RGBAPixel> f32RGBAFrame;
        typedef Frame<f64RGBAPixel> f64RGBAFrame;

        /// \brief
        /// Validate assumptions about RGBAPixel component packing.
        static_assert (
            sizeof (ui8RGBAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16RGBAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32RGBAPixel) ==  THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64RGBAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32RGBAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64RGBAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about RGBAPixel component packing.");

        template<typename T>
        struct BGRAPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef RGBAColor<util::f32> ConverterColorType;

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
        typedef BGRAPixel<util::ui64> ui64BGRAPixel;
        typedef BGRAPixel<util::f32> f32BGRAPixel;
        typedef BGRAPixel<util::f64> f64BGRAPixel;

        typedef Framebuffer<ui8BGRAPixel> ui8BGRAFramebuffer;
        typedef Framebuffer<ui16BGRAPixel> ui16BGRAFramebuffer;
        typedef Framebuffer<ui32BGRAPixel> ui32BGRAFramebuffer;
        typedef Framebuffer<ui64BGRAPixel> ui64BGRAFramebuffer;
        typedef Framebuffer<f32BGRAPixel> f32BGRAFramebuffer;
        typedef Framebuffer<f64BGRAPixel> f64BGRAFramebuffer;

        typedef Frame<ui8BGRAPixel> ui8BGRAFrame;
        typedef Frame<ui16BGRAPixel> ui16BGRAFrame;
        typedef Frame<ui32BGRAPixel> ui32BGRAFrame;
        typedef Frame<ui64BGRAPixel> ui64BGRAFrame;
        typedef Frame<f32BGRAPixel> f32BGRAFrame;
        typedef Frame<f64BGRAPixel> f64BGRAFrame;

        /// \brief
        /// Validate assumptions about BGRAPixel component packing.
        static_assert (
            sizeof (ui8BGRAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16BGRAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32BGRAPixel) ==  THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64BGRAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32BGRAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64BGRAPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about BGRAPixel component packing.");

        template<typename T>
        struct ARGBPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef RGBAColor<util::f32> ConverterColorType;

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
        typedef ARGBPixel<util::ui64> ui64ARGBPixel;
        typedef ARGBPixel<util::f32> f32ARGBPixel;
        typedef ARGBPixel<util::f64> f64ARGBPixel;

        typedef Framebuffer<ui8ARGBPixel> ui8ARGBFramebuffer;
        typedef Framebuffer<ui16ARGBPixel> ui16ARGBFramebuffer;
        typedef Framebuffer<ui32ARGBPixel> ui32ARGBFramebuffer;
        typedef Framebuffer<ui64ARGBPixel> ui64ARGBFramebuffer;
        typedef Framebuffer<f32ARGBPixel> f32ARGBFramebuffer;
        typedef Framebuffer<f64ARGBPixel> f64ARGBFramebuffer;

        typedef Frame<ui8ARGBPixel> ui8ARGBFrame;
        typedef Frame<ui16ARGBPixel> ui16ARGBFrame;
        typedef Frame<ui32ARGBPixel> ui32ARGBFrame;
        typedef Frame<ui64ARGBPixel> ui64ARGBFrame;
        typedef Frame<f32ARGBPixel> f32ARGBFrame;
        typedef Frame<f64ARGBPixel> f64ARGBFrame;

        /// \brief
        /// Validate assumptions about ARGBPixel component packing.
        static_assert (
            sizeof (ui8ARGBPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16ARGBPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32ARGBPixel) ==  THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64ARGBPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32ARGBPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64ARGBPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about ARGBPixel component packing.");

        template<typename T>
        struct ABGRPixel {
            typedef T ComponentType;
            typedef RGBAColor<ComponentType> ColorType;
            typedef RGBAColor<util::f32> ConverterColorType;

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
        typedef ABGRPixel<util::ui64> ui64ABGRPixel;
        typedef ABGRPixel<util::f32> f32ABGRPixel;
        typedef ABGRPixel<util::f64> f64ABGRPixel;

        typedef Framebuffer<ui8ABGRPixel> ui8ABGRFramebuffer;
        typedef Framebuffer<ui16ABGRPixel> ui16ABGRFramebuffer;
        typedef Framebuffer<ui32ABGRPixel> ui32ABGRFramebuffer;
        typedef Framebuffer<ui64ABGRPixel> ui64ABGRFramebuffer;
        typedef Framebuffer<f32ABGRPixel> f32ABGRFramebuffer;
        typedef Framebuffer<f64ABGRPixel> f64ABGRFramebuffer;

        typedef Frame<ui8ABGRPixel> ui8ABGRFrame;
        typedef Frame<ui16ABGRPixel> ui16ABGRFrame;
        typedef Frame<ui32ABGRPixel> ui32ABGRFrame;
        typedef Frame<ui64ABGRPixel> ui64ABGRFrame;
        typedef Frame<f32ABGRPixel> f32ABGRFrame;
        typedef Frame<f64ABGRPixel> f64ABGRFrame;

        /// \brief
        /// Validate assumptions about ABGRPixel component packing.
        static_assert (
            sizeof (ui8ABGRPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16ABGRPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32ABGRPixel) ==  THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64ABGRPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32ABGRPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64ABGRPixel) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about ABGRPixel component packing.");

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBAPixel_h)

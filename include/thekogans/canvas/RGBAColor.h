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

#if !defined (__thekogans_canvas_RGBAColor_h)
#define __thekogans_canvas_RGBAColor_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/ComponentConverter.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct RGBAColor {
            typedef T ComponentType;
            typedef RGBAColor<T> Type;

            ComponentType r;
            ComponentType g;
            ComponentType b;
            ComponentType a;

            RGBAColor () {}
            RGBAColor (
                ComponentType r_,
                ComponentType g_,
                ComponentType b_,
                ComponentType a_) :
                r (r_),
                g (g_),
                b (b_),
                a (a_) {}

            template<typename ComponentConverter>
            RGBAColor<typename ComponentConverter::OutComponentType> ConvertComponents () const {
                return RGBAColor<typename ComponentConverter::OutComponentType> (
                    ComponentConverter::Convert (r),
                    ComponentConverter::Convert (g),
                    ComponentConverter::Convert (b),
                    ComponentConverter::Convert (a));
            }
        };

        typedef RGBAColor<util::ui8> ui8RGBAColor;
        typedef RGBAColor<util::ui16> ui16RGBAColor;
        typedef RGBAColor<util::ui32> ui32RGBAColor;
        typedef RGBAColor<util::ui64> ui64RGBAColor;
        typedef RGBAColor<util::f32> f32RGBAColor;
        typedef RGBAColor<util::f64> f64RGBAColor;

        const std::size_t THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT = 4;

        /// \brief
        /// Validate assumptions about RGBAColor component packing.
        /// This check is only important for rgba framebuffers as these
        /// are used in opengl and windowing systems. Many of these have
        /// specific alignment (4 byte boundary) requeirements. Since
        /// we want to be able to directly write our framebuffers to these
        /// systems, we need to make sure we satisfy their requirements.
        /// Framebuffers built with pixel types which use other color
        /// spaces are particularly important to applications doing image
        /// manipulation and as such will not have alignment requirements.
        static_assert (
            sizeof (ui8RGBAColor) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16RGBAColor) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32RGBAColor) ==  THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64RGBAColor) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32RGBAColor) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64RGBAColor) == THEKOGANS_CANVAS_RGBACOLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about RGBAColor component packing.");

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBAColor_h)

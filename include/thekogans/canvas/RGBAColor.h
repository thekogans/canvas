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

namespace thekogans {
    namespace canvas {

        template<
            typename T,
            typename U>
        struct DefaultComponentConverter {
            typedef T InComponentType;
            typedef U OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return (OutComponentType)value;
            }
        };

        template<typename T>
        struct Defaultf32ComponentConverter {
            typedef T InComponentType;
            typedef util::f32 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return (OutComponentType)value / 255.0f;
            }
        };

        template<typename T>
        struct Defaultf64ComponentConverter {
            typedef T InComponentType;
            typedef util::f64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return (OutComponentType)value / 255.0;
            }
        };

        // RGBAColor<util::ui8> conversion specializations.

        struct ui8Toui16ShiftComponentConverter {
            typedef util::ui8 InComponentType;
            typedef util::ui16 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_MK_UI16 (value, 0);
            }
        };

        struct ui8Toui32ShiftComponentConverter {
            typedef util::ui8 InComponentType;
            typedef util::ui32 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (value, 0), 0);
            }
        };

        struct ui8Toui64ShiftComponentConverter {
            typedef util::ui8 InComponentType;
            typedef util::ui64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (value, 0), 0), 0);
            }
        };

        struct ui8Toui16ScaleComponentConverter {
            typedef util::ui8 InComponentType;
            typedef util::ui16 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[2] = {0, 3};
                return
                    (masks[(value >> 0) & 1] << 0) |
                    (masks[(value >> 1) & 1] << 2) |
                    (masks[(value >> 2) & 1] << 4) |
                    (masks[(value >> 3) & 1] << 6) |
                    (masks[(value >> 4) & 1] << 8) |
                    (masks[(value >> 5) & 1] << 10) |
                    (masks[(value >> 6) & 1] << 12) |
                    (masks[(value >> 7) & 1] << 14);
            }
        };

        struct ui8Toui32ScaleComponentConverter {
            typedef util::ui8 InComponentType;
            typedef util::ui32 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[2] = {0, 15};
                return
                    (masks[(value >> 0) & 1] << 0) |
                    (masks[(value >> 1) & 1] << 4) |
                    (masks[(value >> 2) & 1] << 8) |
                    (masks[(value >> 3) & 1] << 12) |
                    (masks[(value >> 4) & 1] << 16) |
                    (masks[(value >> 5) & 1] << 20) |
                    (masks[(value >> 6) & 1] << 24) |
                    (masks[(value >> 7) & 1] << 28);
            }
        };

        struct ui8Toui64ScaleComponentConverter {
            typedef util::ui8 InComponentType;
            typedef util::ui64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[2] = {0, 255};
                return
                    (masks[(value >> 0) & 1] << 0) |
                    (masks[(value >> 1) & 1] << 8) |
                    (masks[(value >> 2) & 1] << 16) |
                    (masks[(value >> 3) & 1] << 24) |
                    (masks[(value >> 4) & 1] << 32) |
                    (masks[(value >> 5) & 1] << 40) |
                    (masks[(value >> 6) & 1] << 48) |
                    (masks[(value >> 7) & 1] << 56);
            }
        };

        // RGBAColor<util::ui16> conversion specializations.

        struct ui16Toui8ShiftComponentConverter {
            typedef util::ui16 InComponentType;
            typedef util::ui8 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_UI16_GET_UI8_AT_INDEX (value, 0);
            }
        };

        struct ui16Toui32ShiftComponentConverter {
            typedef util::ui16 InComponentType;
            typedef util::ui32 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_MK_UI32 (value, 0);
            }
        };

        struct ui16Toui64ShiftComponentConverter {
            typedef util::ui16 InComponentType;
            typedef util::ui64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (value, 0), 0);
            }
        };

        struct ui16Toui8ScaleComponentConverter {
            typedef util::ui16 InComponentType;
            typedef util::ui8 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[4] = {0, 0, 1, 1};
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
        };

        struct ui16Toui32ScaleComponentConverter {
            typedef util::ui16 InComponentType;
            typedef util::ui32 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[2] = {0, 3};
                return
                    (masks[(value >> 0) & 1] << 0) |
                    (masks[(value >> 1) & 1] << 2) |
                    (masks[(value >> 2) & 1] << 4) |
                    (masks[(value >> 3) & 1] << 6) |
                    (masks[(value >> 4) & 1] << 8) |
                    (masks[(value >> 5) & 1] << 10) |
                    (masks[(value >> 6) & 1] << 12) |
                    (masks[(value >> 7) & 1] << 14) |
                    (masks[(value >> 8) & 1] << 16) |
                    (masks[(value >> 9) & 1] << 18) |
                    (masks[(value >> 10) & 1] << 20) |
                    (masks[(value >> 11) & 1] << 22) |
                    (masks[(value >> 12) & 1] << 24) |
                    (masks[(value >> 13) & 1] << 26) |
                    (masks[(value >> 14) & 1] << 28) |
                    (masks[(value >> 15) & 1] << 30);
            }
        };

        struct ui16Toui64ScaleComponentConverter {
            typedef util::ui16 InComponentType;
            typedef util::ui64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[2] = {0, 15};
                return
                    (masks[(value >> 0) & 1] << 0) |
                    (masks[(value >> 1) & 1] << 4) |
                    (masks[(value >> 2) & 1] << 8) |
                    (masks[(value >> 3) & 1] << 12) |
                    (masks[(value >> 4) & 1] << 16) |
                    (masks[(value >> 5) & 1] << 20) |
                    (masks[(value >> 6) & 1] << 24) |
                    (masks[(value >> 7) & 1] << 28) |
                    (masks[(value >> 8) & 1] << 32) |
                    (masks[(value >> 9) & 1] << 36) |
                    (masks[(value >> 10) & 1] << 40) |
                    (masks[(value >> 11) & 1] << 44) |
                    (masks[(value >> 12) & 1] << 48) |
                    (masks[(value >> 13) & 1] << 52) |
                    (masks[(value >> 14) & 1] << 56) |
                    (masks[(value >> 15) & 1] << 60);
            }
        };

        // RGBAColor<util::ui32> conversion specializations.

        struct ui32Toui8ShiftComponentConverter {
            typedef util::ui32 InComponentType;
            typedef util::ui8 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (value, 0);
            }
        };

        struct ui32Toui16ShiftComponentConverter {
            typedef util::ui32 InComponentType;
            typedef util::ui16 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_UI32_GET_UI16_AT_INDEX (value, 0);
            }
        };

        struct ui32Toui64ShiftComponentConverter {
            typedef util::ui32 InComponentType;
            typedef util::ui64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                return THEKOGANS_UTIL_MK_UI64 (value, 0);
            }
        };

        struct ui32Toui8ScaleComponentConverter {
            typedef util::ui32 InComponentType;
            typedef util::ui8 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[16] = {
                    0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1
                };
                return
                    (masks[(value >> 0) & 15] << 0) |
                    (masks[(value >> 4) & 15] << 1) |
                    (masks[(value >> 8) & 15] << 2) |
                    (masks[(value >> 12) & 15] << 3) |
                    (masks[(value >> 16) & 15] << 4) |
                    (masks[(value >> 20) & 15] << 5) |
                    (masks[(value >> 24) & 15] << 6) |
                    (masks[(value >> 28) & 15] << 7);
            }
        };

        struct ui32Toui16ScaleComponentConverter {
            typedef util::ui32 InComponentType;
            typedef util::ui16 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[4] = {0, 0, 1, 1};
                return
                    (masks[(value >> 0) & 3] << 0) |
                    (masks[(value >> 2) & 3] << 1) |
                    (masks[(value >> 4) & 3] << 2) |
                    (masks[(value >> 6) & 3] << 3) |
                    (masks[(value >> 8) & 3] << 4) |
                    (masks[(value >> 10) & 3] << 5) |
                    (masks[(value >> 12) & 3] << 6) |
                    (masks[(value >> 14) & 3] << 7) |
                    (masks[(value >> 16) & 3] << 8) |
                    (masks[(value >> 18) & 3] << 9) |
                    (masks[(value >> 20) & 3] << 10) |
                    (masks[(value >> 22) & 3] << 11) |
                    (masks[(value >> 24) & 3] << 12) |
                    (masks[(value >> 26) & 3] << 13) |
                    (masks[(value >> 28) & 3] << 14) |
                    (masks[(value >> 30) & 3] << 15);
            }
        };

        struct ui32Toui64ScaleComponentConverter {
            typedef util::ui32 InComponentType;
            typedef util::ui64 OutComponentType;
            static OutComponentType Convert (InComponentType value) {
                static const OutComponentType masks[2] = {0, 3};
                return
                    (masks[(value >> 0) & 1] << 0) |
                    (masks[(value >> 1) & 1] << 2) |
                    (masks[(value >> 2) & 1] << 4) |
                    (masks[(value >> 3) & 1] << 6) |
                    (masks[(value >> 4) & 1] << 8) |
                    (masks[(value >> 5) & 1] << 10) |
                    (masks[(value >> 6) & 1] << 12) |
                    (masks[(value >> 7) & 1] << 14) |
                    (masks[(value >> 8) & 1] << 16) |
                    (masks[(value >> 9) & 1] << 18) |
                    (masks[(value >> 10) & 1] << 20) |
                    (masks[(value >> 11) & 1] << 22) |
                    (masks[(value >> 12) & 1] << 24) |
                    (masks[(value >> 13) & 1] << 26) |
                    (masks[(value >> 14) & 1] << 28) |
                    (masks[(value >> 15) & 1] << 30) |
                    (masks[(value >> 16) & 1] << 32) |
                    (masks[(value >> 17) & 1] << 34) |
                    (masks[(value >> 18) & 1] << 36) |
                    (masks[(value >> 19) & 1] << 38) |
                    (masks[(value >> 20) & 1] << 40) |
                    (masks[(value >> 21) & 1] << 42) |
                    (masks[(value >> 22) & 1] << 44) |
                    (masks[(value >> 23) & 1] << 46) |
                    (masks[(value >> 24) & 1] << 48) |
                    (masks[(value >> 25) & 1] << 50) |
                    (masks[(value >> 26) & 1] << 52) |
                    (masks[(value >> 27) & 1] << 54) |
                    (masks[(value >> 28) & 1] << 56) |
                    (masks[(value >> 29) & 1] << 58) |
                    (masks[(value >> 30) & 1] << 60) |
                    (masks[(value >> 31) & 1] << 62);
            }
        };

        // RGBAColor<util::ui64> conversion specializations.

        template<typename T>
        struct RGBAColor {
            typedef T ComponentType;

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

            /// NOTE: The design of this interface is guided foremost
            /// by the need to do component type conversion. It is not
            /// meant to be used to convert between different color
            /// spaces (where more often than not one needs all
            /// components at the same time). To that end it is
            /// understood that all components are going to be treated
            /// equally and independent of each other. Color space conversion
            /// is designed in to the interface of generic pixel. Each pixel
            /// type is required to have a ctor that takes a color reference.
            /// Diferent ctors can be used to convert an incoming color in to
            /// whatever the internal pixel color space might be. Each pixel
            /// type is also required to supply a ToColor method. That method
            /// can be used to convert whatever the internal pixel color space
            /// is to generec rgba color (which is what this is).
            /// See \see{RGBAPixel}... for details.
            template<typename ComponentConverter>
            RGBAColor<typename ComponentConverter::OutComponentType> Convert () const {
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

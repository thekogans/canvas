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

#if !defined (__thekogans_canvas_Color_h)
#define __thekogans_canvas_Color_h

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

        // Color<util::ui8> conversion specializations.

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

        // Color<util::ui16> conversion specializations.

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

        // Color<util::ui32> conversion specializations.
        // Color<util::ui64> conversion specializations.

        template<typename T>
        struct Color {
            typedef T ComponentType;

            ComponentType r;
            ComponentType g;
            ComponentType b;
            ComponentType a;

            Color (
                ComponentType r_ = 0,
                ComponentType g_ = 0,
                ComponentType b_ = 0,
                ComponentType a_ = 0) :
                r (r_),
                g (g_),
                b (b_),
                a (a_) {}

            template<typename ComponentConverter>
            Color<typename ComponentConverter::OutComponentType> Convert () const {
                return Color<typename ComponentConverter::OutComponentType> (
                    ComponentConverter::Convert (r),
                    ComponentConverter::Convert (g),
                    ComponentConverter::Convert (b),
                    ComponentConverter::Convert (a));
            }
        };

        typedef Color<util::ui8> ui8Color;
        typedef Color<util::ui16> ui16Color;
        typedef Color<util::ui32> ui32Color;
        typedef Color<util::ui64> ui64Color;
        typedef Color<util::f32> f32Color;
        typedef Color<util::f64> f64Color;

        const std::size_t THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT = 4;

        /// \brief
        /// Validate assumptions about Color component packing.
        static_assert (
            sizeof (ui8Color) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (ui16Color) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (ui32Color) ==  THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (ui64Color) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (f32Color) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (f64Color) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about Color component packing.");

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Color_h)

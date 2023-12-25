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

        template<typename T, typename U>
        struct DefaultComponentConverter {
            inline U operator () (T value) {
                return (U)value;
            }
        };

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

            template<
                typename U,
                typename ComponentConverter = DefaultComponentConverter<ComponentType, U>>
            Color<U> Convert () const {
                return Color<U> (
                    ComponentConverter (r),
                    ComponentConverter (g),
                    ComponentConverter (b),
                    ComponentConverter (a));
            }
        };

        // Color<util::ui8> conversion specializations.

        struct ui8Toui16ShiftComponentConverter {
            inline util::ui16 operator () (util::ui8 value) const {
                return THEKOGANS_UTIL_MK_UI16 (value, 0);
            }
        };

        template<>
        template<>
        inline Color<util::ui16> Color<util::ui8>::Convert<util::ui16, ui8Toui16ShiftComponentConverter> () const {
            return Color<util::ui16> (
                ui8Toui16ShiftComponentConverter () (r),
                ui8Toui16ShiftComponentConverter () (g),
                ui8Toui16ShiftComponentConverter () (b),
                ui8Toui16ShiftComponentConverter () (a));
        }

        struct ui8Toui16ScaleComponentConverter {
            inline util::ui16 operator () (util::ui8 value) const {
                static const util::ui16 masks[2] = {0, 3};
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

        template<>
        template<>
        inline Color<util::ui16> Color<util::ui8>::Convert<util::ui16, ui8Toui16ScaleComponentConverter> () const {
            return Color<util::ui16> (
                ui8Toui16ScaleComponentConverter () (r),
                ui8Toui16ScaleComponentConverter () (g),
                ui8Toui16ScaleComponentConverter () (b),
                ui8Toui16ScaleComponentConverter () (a));
        }

        struct ui8Toui32ShiftComponentConverter {
            inline util::ui32 operator () (util::ui8 value) const {
                return THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (value, 0), 0);
            }
        };

        template<>
        template<>
        inline Color<util::ui32> Color<util::ui8>::Convert<util::ui32, ui8Toui32ShiftComponentConverter> () const {
            return Color<util::ui32> (
                ui8Toui32ShiftComponentConverter () (r),
                ui8Toui32ShiftComponentConverter () (g),
                ui8Toui32ShiftComponentConverter () (b),
                ui8Toui32ShiftComponentConverter () (a));
        }

        struct ui8Toui32ScaleComponentConverter {
            inline util::ui32 operator () (util::ui8 value) const {
                static const util::ui32 masks[2] = {0, 15};
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

        template<>
        template<>
        inline Color<util::ui32> Color<util::ui8>::Convert<util::ui32, ui8Toui32ScaleComponentConverter> () const {
            return Color<util::ui32> (
                ui8Toui32ScaleComponentConverter () (r),
                ui8Toui32ScaleComponentConverter () (g),
                ui8Toui32ScaleComponentConverter () (b),
                ui8Toui32ScaleComponentConverter () (a));
        }

        template<>
        template<>
        inline Color<util::ui64> Color<util::ui8>::Convert<util::ui64> () const {
            return Color<util::ui64> (
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (r, 0), 0), 0),
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (g, 0), 0), 0),
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (b, 0), 0), 0),
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (THEKOGANS_UTIL_MK_UI16 (a, 0), 0), 0));
        }

        struct ui8Toui64ScaleComponentConverter {
            inline util::ui32 operator () (util::ui8 value) const {
                static const util::ui64 masks[2] = {0, 255};
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

        template<>
        template<>
        inline Color<util::ui64> Color<util::ui8>::Convert<util::ui64, ui8Toui64ScaleComponentConverter> () const {
            return Color<util::ui64> (
                ui8Toui64ScaleComponentConverter () (r),
                ui8Toui64ScaleComponentConverter () (g),
                ui8Toui64ScaleComponentConverter () (b),
                ui8Toui64ScaleComponentConverter () (a));
        }

        template<>
        template<>
        inline Color<util::f32> Color<util::ui8>::Convert<util::f32> () const {
            return Color<util::f32> (
                (util::f32)r / 255.0f,
                (util::f32)g / 255.0f,
                (util::f32)b / 255.0f,
                (util::f32)a / 255.0f);
        }

        template<>
        template<>
        inline Color<util::f64> Color<util::ui8>::Convert<util::f64> () const {
            return Color<util::f64> (
                (util::f64)r / 255.0f,
                (util::f64)g / 255.0f,
                (util::f64)b / 255.0f,
                (util::f64)a / 255.0f);
        }

        // Color<util::ui16> conversion specializations.

        template<>
        template<>
        inline Color<util::ui8> Color<util::ui16>::Convert<util::ui8> () const {
            return Color<util::ui8> (
                THEKOGANS_UTIL_UI16_GET_UI8_AT_INDEX (r, 0),
                THEKOGANS_UTIL_UI16_GET_UI8_AT_INDEX (g, 0),
                THEKOGANS_UTIL_UI16_GET_UI8_AT_INDEX (b, 0),
                THEKOGANS_UTIL_UI16_GET_UI8_AT_INDEX (a, 0));
        }

        template<>
        template<>
        inline Color<util::ui32> Color<util::ui16>::Convert<util::ui32> () const {
            return Color<util::ui32> (
                THEKOGANS_UTIL_MK_UI32 (r, 0),
                THEKOGANS_UTIL_MK_UI32 (g, 0),
                THEKOGANS_UTIL_MK_UI32 (b, 0),
                THEKOGANS_UTIL_MK_UI32 (a, 0));
        }

        template<>
        template<>
        inline Color<util::ui64> Color<util::ui16>::Convert<util::ui64> () const {
            return Color<util::ui64> (
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (r, 0), 0),
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (g, 0), 0),
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (b, 0), 0),
                THEKOGANS_UTIL_MK_UI64 (THEKOGANS_UTIL_MK_UI32 (a, 0), 0));
        }

        template<>
        template<>
        inline Color<util::f32> Color<util::ui16>::Convert<util::f32> () const {
            return Color<util::f32> (
                (util::f32)r / 255.0f,
                (util::f32)g / 255.0f,
                (util::f32)b / 255.0f,
                (util::f32)a / 255.0f);
        }

        template<>
        template<>
        inline Color<util::f64> Color<util::ui16>::Convert<util::f64> () const {
            return Color<util::f64> (
                (util::f64)r / 255.0f,
                (util::f64)g / 255.0f,
                (util::f64)b / 255.0f,
                (util::f64)a / 255.0f);
        }

        // Color<util::ui32> conversion specializations.

        template<>
        template<>
        inline Color<util::ui8> Color<util::ui32>::Convert<util::ui8> () const {
            return Color<util::ui8> (
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (r, 0),
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (g, 0),
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (b, 0),
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (a, 0));
        }

        template<>
        template<>
        inline Color<util::ui16> Color<util::ui32>::Convert<util::ui16> () const {
            return Color<util::ui16> (
                THEKOGANS_UTIL_UI32_GET_UI16_AT_INDEX (r, 0),
                THEKOGANS_UTIL_UI32_GET_UI16_AT_INDEX (g, 0),
                THEKOGANS_UTIL_UI32_GET_UI16_AT_INDEX (b, 0),
                THEKOGANS_UTIL_UI32_GET_UI16_AT_INDEX (a, 0));
        }

        template<>
        template<>
        inline Color<util::ui64> Color<util::ui32>::Convert<util::ui64> () const {
            return Color<util::ui64> (
                THEKOGANS_UTIL_MK_UI64 (r, 0),
                THEKOGANS_UTIL_MK_UI64 (g, 0),
                THEKOGANS_UTIL_MK_UI64 (b, 0),
                THEKOGANS_UTIL_MK_UI64 (a, 0));
        }

        template<>
        template<>
        inline Color<util::f32> Color<util::ui32>::Convert<util::f32> () const {
            return Color<util::f32> (
                (util::f32)r / 255.0f,
                (util::f32)g / 255.0f,
                (util::f32)b / 255.0f,
                (util::f32)a / 255.0f);
        }

        template<>
        template<>
        inline Color<util::f64> Color<util::ui32>::Convert<util::f64> () const {
            return Color<util::f64> (
                (util::f64)r / 255.0f,
                (util::f64)g / 255.0f,
                (util::f64)b / 255.0f,
                (util::f64)a / 255.0f);
        }

        // Color<util::ui64> conversion specializations.

        template<>
        template<>
        inline Color<util::ui8> Color<util::ui64>::Convert<util::ui8> () const {
            return Color<util::ui8> (
                THEKOGANS_UTIL_UI64_GET_UI8_AT_INDEX (r, 0),
                THEKOGANS_UTIL_UI64_GET_UI8_AT_INDEX (g, 0),
                THEKOGANS_UTIL_UI64_GET_UI8_AT_INDEX (b, 0),
                THEKOGANS_UTIL_UI64_GET_UI8_AT_INDEX (a, 0));
        }

        template<>
        template<>
        inline Color<util::ui16> Color<util::ui64>::Convert<util::ui16> () const {
            return Color<util::ui16> (
                THEKOGANS_UTIL_UI64_GET_UI16_AT_INDEX (r, 0),
                THEKOGANS_UTIL_UI64_GET_UI16_AT_INDEX (g, 0),
                THEKOGANS_UTIL_UI64_GET_UI16_AT_INDEX (b, 0),
                THEKOGANS_UTIL_UI64_GET_UI16_AT_INDEX (a, 0));
        }

        template<>
        template<>
        inline Color<util::ui32> Color<util::ui64>::Convert<util::ui32> () const {
            return Color<util::ui32> (
                THEKOGANS_UTIL_UI64_GET_UI32_AT_INDEX (r, 0),
                THEKOGANS_UTIL_UI64_GET_UI32_AT_INDEX (g, 0),
                THEKOGANS_UTIL_UI64_GET_UI32_AT_INDEX (b, 0),
                THEKOGANS_UTIL_UI64_GET_UI32_AT_INDEX (a, 0));
        }

        template<>
        template<>
        inline Color<util::f32> Color<util::ui64>::Convert<util::f32> () const {
            return Color<util::f32> (
                (util::f32)r / 255.0f,
                (util::f32)g / 255.0f,
                (util::f32)b / 255.0f,
                (util::f32)a / 255.0f);
        }

        template<>
        template<>
        inline Color<util::f64> Color<util::ui64>::Convert<util::f64> () const {
            return Color<util::f64> (
                (util::f64)r / 255.0f,
                (util::f64)g / 255.0f,
                (util::f64)b / 255.0f,
                (util::f64)a / 255.0f);
        }

        const std::size_t THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT = 4;

        /// \brief
        /// Validate assumptions about color component packing.
        static_assert (
            sizeof (Color<util::ui8>) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI8_SIZE &&
            sizeof (Color<util::ui16>) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI16_SIZE &&
            sizeof (Color<util::ui32>) ==  THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI32_SIZE &&
            sizeof (Color<util::ui64>) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::UI64_SIZE &&
            sizeof (Color<util::f32>) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F32_SIZE &&
            sizeof (Color<util::f64>) == THEKOGANS_CANVAS_COLOR_COMPONENT_COUNT * util::F64_SIZE,
            "Invalid assumption about Color component packing.");

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Color_h)

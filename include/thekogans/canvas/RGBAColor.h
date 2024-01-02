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

namespace thekogans {
    namespace canvas {

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

            static const RGBAColor Black;
        };

        template<typename T>
        const RGBAColor<T> RGBAColor<T>::Black (0, 0, 0, 0);

        typedef RGBAColor<util::ui8> ui8RGBAColor;
        typedef RGBAColor<util::ui16> ui16RGBAColor;
        typedef RGBAColor<util::ui32> ui32RGBAColor;
        typedef RGBAColor<util::f32> f32RGBAColor;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBAColor_h)

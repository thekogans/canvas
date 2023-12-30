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

#if !defined (__thekogans_canvas_YUVAColor_h)
#define __thekogans_canvas_YUVAColor_h

#include "thekogans/util/Types.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct YUVAColor {
            typedef T ComponentType;

            ComponentType y;
            ComponentType u;
            ComponentType v;
            ComponentType a;

            YUVAColor () {}
            YUVAColor (
                ComponentType y_,
                ComponentType u_,
                ComponentType v_,
                ComponentType a_) :
                y (y_),
                u (u_),
                v (v_),
                a (a_) {}

            static const YUVAColor Black;
        };

        template<typename T>
        const YUVAColor<T> YUVAColor<T>::Black (0, 0, 0, 0);

        typedef YUVAColor<util::ui8> ui8YUVAColor;
        typedef YUVAColor<util::ui16> ui16YUVAColor;
        typedef YUVAColor<util::ui32> ui32YUVAColor;
        typedef YUVAColor<util::ui64> ui64YUVAColor;
        typedef YUVAColor<util::f32> f32YUVAColor;
        typedef YUVAColor<util::f64> f64YUVAColor;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_YUVAColor_h)

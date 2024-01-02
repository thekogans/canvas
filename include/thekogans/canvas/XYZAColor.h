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

#if !defined (__thekogans_canvas_XYZAColor_h)
#define __thekogans_canvas_XYZAColor_h

#include "thekogans/util/Types.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct XYZAColor {
            typedef T ComponentType;
            typedef XYZAColor<util::f32> ConverterColorType;

            ComponentType x;
            ComponentType y;
            ComponentType z;
            ComponentType a;

            XYZAColor () {}
            XYZAColor (
                ComponentType x_,
                ComponentType y_,
                ComponentType z_,
                ComponentType a_) :
                x (x_),
                y (y_),
                z (z_),
                a (a_) {}

            static const XYZAColor Black;
        };

        template<typename T>
        const XYZAColor<T> XYZAColor<T>::Black (0, 0, 0, 0);

        typedef XYZAColor<util::f32> f32XYZAColor;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAColor_h)

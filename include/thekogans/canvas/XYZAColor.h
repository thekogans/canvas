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
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/ComponentConverter.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct XYZAColor {
            typedef T ComponentType;
            typedef XYZAColor<T> Type;

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

            template<typename ComponentConverter>
            XYZAColor<typename ComponentConverter::OutComponentType> ConvertComponents () const {
                return XYZAColor<typename ComponentConverter::OutComponentType> (
                    ComponentConverter::Convert (x),
                    ComponentConverter::Convert (y),
                    ComponentConverter::Convert (z),
                    ComponentConverter::Convert (a));
            }
        };

        typedef XYZAColor<util::ui8> ui8XYZAColor;
        typedef XYZAColor<util::ui16> ui16XYZAColor;
        typedef XYZAColor<util::ui32> ui32XYZAColor;
        typedef XYZAColor<util::ui64> ui64XYZAColor;
        typedef XYZAColor<util::f32> f32XYZAColor;
        typedef XYZAColor<util::f64> f64XYZAColor;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAColor_h)
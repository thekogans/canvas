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

#if !defined (__thekogans_canvas_XYZAFrame_h)
#define __thekogans_canvas_XYZAFrame_h

#include "thekogans/canvas/Frame.h"
#include "thekogans/canvas/XYZAPixel.h"

namespace thekogans {
    namespace canvas {

        typedef Frame<ui8XYZAPixel> ui8XYZAFrame;
        typedef Frame<ui16XYZAPixel> ui16XYZAFrame;
        typedef Frame<ui32XYZAPixel> ui32XYZAFrame;
        typedef Frame<ui64XYZAPixel> ui64XYZAFrame;
        typedef Frame<f32XYZAPixel> f32XYZAFrame;
        typedef Frame<f64XYZAPixel> f64XYZAFrame;

        typedef Frame<ui8ZYXAPixel> ui8ZYXAFrame;
        typedef Frame<ui16ZYXAPixel> ui16ZYXAFrame;
        typedef Frame<ui32ZYXAPixel> ui32ZYXAFrame;
        typedef Frame<ui64ZYXAPixel> ui64ZYXAFrame;
        typedef Frame<f32ZYXAPixel> f32ZYXAFrame;
        typedef Frame<f64ZYXAPixel> f64ZYXAFrame;

        typedef Frame<ui8AXYZPixel> ui8AXYZFrame;
        typedef Frame<ui16AXYZPixel> ui16AXYZFrame;
        typedef Frame<ui32AXYZPixel> ui32AXYZFrame;
        typedef Frame<ui64AXYZPixel> ui64AXYZFrame;
        typedef Frame<f32AXYZPixel> f32AXYZFrame;
        typedef Frame<f64AXYZPixel> f64AXYZFrame;

        typedef Frame<ui8AZYXPixel> ui8AZYXFrame;
        typedef Frame<ui16AZYXPixel> ui16AZYXFrame;
        typedef Frame<ui32AZYXPixel> ui32AZYXFrame;
        typedef Frame<ui64AZYXPixel> ui64AZYXFrame;
        typedef Frame<f32AZYXPixel> f32AZYXFrame;
        typedef Frame<f64AZYXPixel> f64AZYXFrame;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAFrame_h)

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

#if !defined (__thekogans_canvas_RGBAFrame_h)
#define __thekogans_canvas_RGBAFrame_h

#include "thekogans/canvas/Frame.h"
#include "thekogans/canvas/RGBAPixel.h"

namespace thekogans {
    namespace canvas {

        typedef Frame<ui8RGBAPixel> ui8RGBAFrame;
        typedef Frame<ui16RGBAPixel> ui16RGBAFrame;
        typedef Frame<ui32RGBAPixel> ui32RGBAFrame;
        typedef Frame<ui64RGBAPixel> ui64RGBAFrame;
        typedef Frame<f32RGBAPixel> f32RGBAFrame;
        typedef Frame<f64RGBAPixel> f64RGBAFrame;

        typedef Frame<ui8BGRAPixel> ui8BGRAFrame;
        typedef Frame<ui16BGRAPixel> ui16BGRAFrame;
        typedef Frame<ui32BGRAPixel> ui32BGRAFrame;
        typedef Frame<ui64BGRAPixel> ui64BGRAFrame;
        typedef Frame<f32BGRAPixel> f32BGRAFrame;
        typedef Frame<f64BGRAPixel> f64BGRAFrame;

        typedef Frame<ui8ARGBPixel> ui8ARGBFrame;
        typedef Frame<ui16ARGBPixel> ui16ARGBFrame;
        typedef Frame<ui32ARGBPixel> ui32ARGBFrame;
        typedef Frame<ui64ARGBPixel> ui64ARGBFrame;
        typedef Frame<f32ARGBPixel> f32ARGBFrame;
        typedef Frame<f64ARGBPixel> f64ARGBFrame;

        typedef Frame<ui8ABGRPixel> ui8ABGRFrame;
        typedef Frame<ui16ABGRPixel> ui16ABGRFrame;
        typedef Frame<ui32ABGRPixel> ui32ABGRFrame;
        typedef Frame<ui64ABGRPixel> ui64ABGRFrame;
        typedef Frame<f32ABGRPixel> f32ABGRFrame;
        typedef Frame<f64ABGRPixel> f64ABGRFrame;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBAFrame_h)

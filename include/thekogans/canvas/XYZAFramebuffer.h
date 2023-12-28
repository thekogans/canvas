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

#if !defined (__thekogans_canvas_XYZAFramebuffer_h)
#define __thekogans_canvas_XYZAFramebuffer_h

#include "thekogans/canvas/Framebuffer.h"
#include "thekogans/canvas/XYZAPixel.h"

namespace thekogans {
    namespace canvas {

        typedef Framebuffer<ui8XYZAPixel> ui8XYZAFramebuffer;
        typedef Framebuffer<ui16XYZAPixel> ui16XYZAFramebuffer;
        typedef Framebuffer<ui32XYZAPixel> ui32XYZAFramebuffer;
        typedef Framebuffer<ui64XYZAPixel> ui64XYZAFramebuffer;
        typedef Framebuffer<f32XYZAPixel> f32XYZAFramebuffer;
        typedef Framebuffer<f64XYZAPixel> f64XYZAFramebuffer;

        typedef Framebuffer<ui8ZYXAPixel> ui8ZYXAFramebuffer;
        typedef Framebuffer<ui16ZYXAPixel> ui16ZYXAFramebuffer;
        typedef Framebuffer<ui32ZYXAPixel> ui32ZYXAFramebuffer;
        typedef Framebuffer<ui64ZYXAPixel> ui64ZYXAFramebuffer;
        typedef Framebuffer<f32ZYXAPixel> f32ZYXAFramebuffer;
        typedef Framebuffer<f64ZYXAPixel> f64ZYXAFramebuffer;

        typedef Framebuffer<ui8AXYZPixel> ui8AXYZFramebuffer;
        typedef Framebuffer<ui16AXYZPixel> ui16AXYZFramebuffer;
        typedef Framebuffer<ui32AXYZPixel> ui32AXYZFramebuffer;
        typedef Framebuffer<ui64AXYZPixel> ui64AXYZFramebuffer;
        typedef Framebuffer<f32AXYZPixel> f32AXYZFramebuffer;
        typedef Framebuffer<f64AXYZPixel> f64AXYZFramebuffer;

        typedef Framebuffer<ui8AZYXPixel> ui8AZYXFramebuffer;
        typedef Framebuffer<ui16AZYXPixel> ui16AZYXFramebuffer;
        typedef Framebuffer<ui32AZYXPixel> ui32AZYXFramebuffer;
        typedef Framebuffer<ui64AZYXPixel> ui64AZYXFramebuffer;
        typedef Framebuffer<f32AZYXPixel> f32AZYXFramebuffer;
        typedef Framebuffer<f64AZYXPixel> f64AZYXFramebuffer;

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAFramebuffer_h)

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

#if !defined (__thekogans_canvas_RGBAFramebuffer_h)
#define __thekogans_canvas_RGBAFramebuffer_h

#include "thekogans/canvas/Framebuffer.h"
#include "thekogans/canvas/RGBAPixel.h"

namespace thekogans {
    namespace canvas {

        typedef Framebuffer<ui8RGBAPixel> ui8RGBAFramebuffer;
        typedef Framebuffer<ui16RGBAPixel> ui16RGBAFramebuffer;
        typedef Framebuffer<ui32RGBAPixel> ui32RGBAFramebuffer;
        typedef Framebuffer<ui64RGBAPixel> ui64RGBAFramebuffer;
        typedef Framebuffer<f32RGBAPixel> f32RGBAFramebuffer;
        typedef Framebuffer<f64RGBAPixel> f64RGBAFramebuffer;

        typedef Framebuffer<ui8BGRAPixel> ui8BGRAFramebuffer;
        typedef Framebuffer<ui16BGRAPixel> ui16BGRAFramebuffer;
        typedef Framebuffer<ui32BGRAPixel> ui32BGRAFramebuffer;
        typedef Framebuffer<ui64BGRAPixel> ui64BGRAFramebuffer;
        typedef Framebuffer<f32BGRAPixel> f32BGRAFramebuffer;
        typedef Framebuffer<f64BGRAPixel> f64BGRAFramebuffer;

        typedef Framebuffer<ui8ARGBPixel> ui8ARGBFramebuffer;
        typedef Framebuffer<ui16ARGBPixel> ui16ARGBFramebuffer;
        typedef Framebuffer<ui32ARGBPixel> ui32ARGBFramebuffer;
        typedef Framebuffer<ui64ARGBPixel> ui64ARGBFramebuffer;
        typedef Framebuffer<f32ARGBPixel> f32ARGBFramebuffer;
        typedef Framebuffer<f64ARGBPixel> f64ARGBFramebuffer;

        typedef Framebuffer<ui8ABGRPixel> ui8ABGRFramebuffer;
        typedef Framebuffer<ui16ABGRPixel> ui16ABGRFramebuffer;
        typedef Framebuffer<ui32ABGRPixel> ui32ABGRFramebuffer;
        typedef Framebuffer<ui64ABGRPixel> ui64ABGRFramebuffer;
        typedef Framebuffer<f32ABGRPixel> f32ABGRFramebuffer;
        typedef Framebuffer<f64ABGRPixel> f64ABGRFramebuffer;

        ui8RGBAFramebuffer::SharedPtr FromPNGBuffer (
            const util::ui8 *buffer,
            std::size_t size);
        ui8RGBAFramebuffer::SharedPtr FromPNGFile (const std::string &path);
        ui8RGBAFramebuffer::SharedPtr FromJPGBuffer (
            const util::ui8 *buffer,
            std::size_t size);
        ui8RGBAFramebuffer::SharedPtr FromJPGFile (const std::string &path);
        ui8RGBAFramebuffer::SharedPtr FromBMPBuffer (
            const util::ui8 *buffer,
            std::size_t size);
        ui8RGBAFramebuffer::SharedPtr FromBMPFile (const std::string &path);

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBAFramebuffer_h)

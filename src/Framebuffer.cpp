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

#include "thekogans/canvas/RGBAPixel.h"
#include "thekogans/canvas/RGBAColor.h"
#include "thekogans/canvas/RGBAConverter.h"
#include "thekogans/canvas/XYZAPixel.h"
#include "thekogans/canvas/XYZAColor.h"
#include "thekogans/canvas/XYZAConverter.h"

namespace thekogans {
    namespace canvas {

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64RGBAFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64BGRAFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ARGBFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ABGRFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8XYZAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16XYZAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32XYZAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64XYZAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32XYZAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64XYZAFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ZYXAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ZYXAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ZYXAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ZYXAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ZYXAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ZYXAFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8AXYZFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16AXYZFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32AXYZFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64AXYZFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32AXYZFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64AXYZFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8AZYXFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16AZYXFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32AZYXFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64AZYXFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32AZYXFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64AZYXFramebuffer, util::SpinLock)

        void foo () {
            ui8RGBAFramebuffer::SharedPtr fb1 (new ui8RGBAFramebuffer (util::Rectangle::Extents (10, 10)));
            f32XYZAFramebuffer::SharedPtr fb2 = fb1->Convert<f32XYZAPixel, DefaultComponentConverter<util::ui8, util::f32>> ();
            f32RGBAFramebuffer::SharedPtr fb3 = fb2->Convert<f32RGBAPixel, DefaultComponentConverter<util::f32, util::f32>> ();
            ui8RGBAFramebuffer::SharedPtr fb4 = fb3->Convert<ui8RGBAPixel, DefaultComponentConverter<util::f32, util::ui8>> ();
        }

    } // namespace canvas
} // namespace thekogans

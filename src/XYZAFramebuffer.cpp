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

#include "thekogans/canvas/XYZAFramebuffer.h"

namespace thekogans {
    namespace canvas {

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

    } // namespace canvas
} // namespace thekogans

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

#include "thekogans/canvas/RGBAFrame.h"

namespace thekogans {
    namespace canvas {

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8RGBAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16RGBAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32RGBAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64RGBAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32RGBAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64RGBAFrame, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8BGRAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16BGRAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32BGRAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64BGRAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32BGRAFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64BGRAFrame, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ARGBFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ARGBFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ARGBFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ARGBFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ARGBFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ARGBFrame, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ABGRFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ABGRFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ABGRFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ABGRFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ABGRFrame, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ABGRFrame, util::SpinLock)

    } // namespace canvas
} // namespace thekogans
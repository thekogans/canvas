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

#if !defined (__thekogans_canvas_Color_h)
#define __thekogans_canvas_Color_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/Config.h"

namespace thekogans {
    namespace canvas {

        struct _LIB_THEKOGANS_CANVAS_DECL Color {
            thekogans::util::ui8 r;
            thekogans::util::ui8 g;
            thekogans::util::ui8 b;
            thekogans::util::ui8 a;

            Color (
                thekogans::util::ui8 r_ = 0,
                thekogans::util::ui8 g_ = 0,
                thekogans::util::ui8 b_ = 0,
                thekogans::util::ui8 a_ = 0) :
                r (r_),
                g (g_),
                b (b_),
                a (a_) {}
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Color_h)

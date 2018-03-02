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

#if !defined (__thekogans_canvas_DrawUtils_h)
#define __thekogans_canvas_DrawUtils_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"
#include "thekogans/canvas/Rectangle.h"
#include "thekogans/canvas/RGBImage.h"

namespace thekogans {
    namespace canvas {

        _LIB_THEKOGANS_CANVAS_DECL void _LIB_THEKOGANS_CANVAS_API DrawRectangle (
            RGBImage &image,
            const Rectangle &rectangle,
            const Color &color,
            thekogans::util::ui32 width = 1);

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_DrawUtils_h)

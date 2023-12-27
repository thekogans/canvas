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

#if !defined (__thekogans_canvas_XYZAConverter_h)
#define __thekogans_canvas_XYZAConverter_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/RGBAColor.h"
#include "thekogans/canvas/XYZAColor.h"
#include "thekogans/canvas/Converter.h"

namespace thekogans {
    namespace canvas {

        template<>
        struct Converter<f32XYZAColor> {
            typedef util::f32 ComponentType;
            typedef RGBAColor<ComponentType> IntermediateColorType;
            typedef f32XYZAColor OutColorType;

            template<typename InColorType>
            static OutColorType Convert (const InColorType &inColor);
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_XYZAConverter_h)

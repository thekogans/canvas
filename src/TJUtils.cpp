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

#include "thekogans/canvas/TJUtils.h"

namespace thekogans {
    namespace canvas {

        _LIB_THEKOGANS_CANVAS_DECL int _LIB_THEKOGANS_CANVAS_API ComponentIndicesToTJPixelFormat (
                util::ui32 componentIndices,
                util::ui32 pixelStride) {
            if (pixelStride == 3) {
                return
                    componentIndices == RGBImage::R0G1B2A3 ? TJPF_RGB :
                    componentIndices == RGBImage::R2G1B0A3 ? TJPF_BGR : 0;
            }
            return
                componentIndices == RGBImage::R0G1B2A3 ? TJPF_RGBX :
                componentIndices == RGBImage::R2G1B0A3 ? TJPF_BGRX :
                componentIndices == RGBImage::R1G2B3A0 ? TJPF_XRGB :
                componentIndices == RGBImage::R3G2B1A0 ? TJPF_XBGR : 0;
        }

    } // namespace canvas
} // namespace thekogans

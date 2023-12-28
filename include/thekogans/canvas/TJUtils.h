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

#if !defined (__thekogans_canvas_TJUtils_h)
#define __thekogans_canvas_TJUtils_h

#include <turbojpeg.h>
#include "thekogans/util/Types.h"
#include "thekogans/util/Exception.h"
#include "thekogans/canvas/Config.h"
//#include "thekogans/canvas/RGBImage.h"

namespace thekogans {
    namespace canvas {

        struct _LIB_THEKOGANS_CANVAS_DECL TJCompressHandle {
            tjhandle handle;
            TJCompressHandle () :
                    handle (tjInitCompress ()) {
                if (handle == 0) {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "%s", tjGetErrorStr ());
                }
            }
            ~TJCompressHandle () {
                tjDestroy (handle);
            }
        };

        struct _LIB_THEKOGANS_CANVAS_DECL TJDecompressHandle {
            tjhandle handle;
            TJDecompressHandle () :
                    handle (tjInitDecompress ()) {
                if (handle == 0) {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "%s", tjGetErrorStr ());
                }
            }
            ~TJDecompressHandle () {
                tjDestroy (handle);
            }
        };

        _LIB_THEKOGANS_CANVAS_DECL int _LIB_THEKOGANS_CANVAS_API ComponentIndicesToTJPixelFormat (
            util::ui32 componentIndices,
            util::ui32 pixelStride);

        // inline int ChromaToTJChroma (RGBImage::Chroma chroma) {
        //     return
        //         chroma == RGBImage::CHROMA_NONE ? TJSAMP_444 :
        //         chroma == RGBImage::CHROMA_2x1 ? TJSAMP_422 :
        //         chroma == RGBImage::CHROMA_1x2 ? TJSAMP_440 :
        //         chroma == RGBImage::CHROMA_2x2 ? TJSAMP_420 : 0;
        // }

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_TJUtils_h)

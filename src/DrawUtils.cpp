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

#include "thekogans/canvas/DrawUtils.h"

namespace thekogans {
    namespace canvas {

        namespace {
            // FIXME: clip rectangle to image bounds.
            void DrawRectangleHelper (
                    RGBImage &image,
                    const Rectangle &rectangle,
                    const std::vector<util::ui8> &pixel) {
                if (!rectangle.IsDegenerate ()) {
                    util::ui8 *data = image.GetData ();
                    util::ui32 pixelStride = image.GetPixelStride ();
                    util::ui32 rowStride = image.GetRowStride ();
                    const util::ui8 *pixelPtr = &pixel[0];
                    {
                        util::ui8 *topLeft = data +
                            rectangle.origin.y * rowStride +
                            rectangle.origin.x * pixelStride;
                        util::ui8 *bottomLeft = data +
                            (rectangle.origin.y + rectangle.extents.height - 1) * rowStride +
                            rectangle.origin.x * pixelStride;
                        util::ui32 width = rectangle.extents.width;
                        while (width-- != 0) {
                            memcpy (topLeft, pixelPtr, pixelStride);
                            topLeft += pixelStride;
                            memcpy (bottomLeft, pixelPtr, pixelStride);
                            bottomLeft += pixelStride;
                        }
                    }
                    {
                        util::ui8 *topLeft = data +
                            rectangle.origin.y * rowStride +
                            rectangle.origin.x * pixelStride;
                        util::ui8 *topRight = data +
                            rectangle.origin.y * rowStride +
                            (rectangle.origin.x + rectangle.extents.width - 1) * pixelStride;
                        util::ui32 height = rectangle.extents.height;
                        while (height-- != 0) {
                            memcpy (topLeft, pixelPtr, pixelStride);
                            topLeft += rowStride;
                            memcpy (topRight, pixelPtr, pixelStride);
                            topRight += rowStride;
                        }
                    }
                }
            }
        }

        _LIB_THEKOGANS_UTIL_DECL void _LIB_THEKOGANS_UTIL_API DrawRectangle (
                RGBImage &image,
                const Rectangle &rectangle,
                const Color &color,
                util::ui32 width) {
            assert (width > 0);
            if (width > 0) {
                std::vector<util::ui8> pixel;
                image.ColorToPixel (color, pixel);
                DrawRectangleHelper (image, rectangle, pixel);
                bool grow = true;
                Rectangle outer = rectangle;
                Rectangle inner = rectangle;
                while (--width != 0) {
                    if (grow) {
                        --outer.origin.x;
                        --outer.origin.y;
                        outer.extents.width += 2;
                        outer.extents.height += 2;
                        DrawRectangleHelper (image, outer, pixel);
                        grow = false;
                    }
                    else {
                        ++inner.origin.x;
                        ++inner.origin.y;
                        inner.extents.width -= 2;
                        inner.extents.height -= 2;
                        DrawRectangleHelper (image, inner, pixel);
                        grow = true;
                    }
                }
            }
        }

    } // namespace canvas
} // namespace thekogans

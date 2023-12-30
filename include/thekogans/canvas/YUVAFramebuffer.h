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

#if !defined (__thekogans_canvas_YUVAFramebuffer_h)
#define __thekogans_canvas_YUVAFramebuffer_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Framebuffer.h"
#include "thekogans/canvas/YUVAColor.h"
#include "thekogans/canvas/YUVAPixel.h"

namespace thekogans {
    namespace canvas {

        struct RGBImage;

        // NOTE: The one and only format supported by Framebuffer<YUVAPixel> is I420!

        template<>
        struct _LIB_THEKOGANS_CANVAS_DECL Framebuffer<YUVAPixel> {
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (YUVImage, util::SpinLock)

            enum {
                Y_INDEX,
                U_INDEX,
                V_INDEX,
                A_INDEX
            };

            const util::Rectangle::Extents extents;
            util::Array<util::ui8> buffer;
            struct _LIB_THEKOGANS_CANVAS_DECL Planes {
                util::ui8 *planes[4];

                Planes () {
                    planes[Y_INDEX] =
                    planes[U_INDEX] =
                    planes[V_INDEX] =
                    planes[A_INDEX] = 0;
                }
                Planes (util::ui8 *planes_[4]) {
                    planes[Y_INDEX] = planes_[Y_INDEX];
                    planes[U_INDEX] = planes_[U_INDEX];
                    planes[V_INDEX] = planes_[V_INDEX];
                    planes[A_INDEX] = planes_[A_INDEX];
                }
                Planes (
                        util::ui8 *y,
                        util::ui8 *u,
                        util::ui8 *v,
                        util::ui8 *a) {
                    planes[Y_INDEX] = y;
                    planes[U_INDEX] = u;
                    planes[V_INDEX] = v;
                    planes[A_INDEX] = a;
                }

                inline util::ui8 *operator [] (std::size_t index) {
                    assert (index < 4);
                    return planes[index];
                }
                inline util::ui8 *operator [] (std::size_t index) const {
                    assert (index < 4);
                    return planes[index];
                }
            } planes;

            struct _LIB_THEKOGANS_CANVAS_DECL Strides {
                util::ui32 strides[4];

                Strides () {
                    strides[Y_INDEX] =
                    strides[U_INDEX] =
                    strides[V_INDEX] =
                    strides[A_INDEX] = 0;
                }
                Strides (util::ui32 strides_[4]) {
                    strides[Y_INDEX] = strides_[Y_INDEX];
                    strides[U_INDEX] = strides_[U_INDEX];
                    strides[V_INDEX] = strides_[V_INDEX];
                    strides[A_INDEX] = strides_[A_INDEX];
                }
                Strides (
                        util::ui32 y,
                        util::ui32 u,
                        util::ui32 v,
                        util::ui32 a) {
                    strides[Y_INDEX] = y;
                    strides[U_INDEX] = u;
                    strides[V_INDEX] = v;
                    strides[A_INDEX] = a;
                }

                inline util::ui32 operator [] (std::size_t index) {
                    assert (index < 4);
                    return strides[index];
                }
                inline util::ui32 operator [] (std::size_t index) const {
                    assert (index < 4);
                    return strides[index];
                }
            } strides;

            YUVImage (const util::Rectangle::Extents &extents_);

            void Clear (const YUVAColor &color);

            std::unique_ptr<RGBImage> ToRGBImage (
                util::ui32 componentIndices) const;
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_YUVAFramebuffer_h)

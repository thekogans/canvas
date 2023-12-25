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

#if !defined (__thekogans_canvas_Frame_h)
#define __thekogans_canvas_Frame_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Array.h"
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/RefCounted.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"
#include "thekogans/canvas/Pixel.h"
#include "thekogans/canvas/Framebuffer.h"

namespace thekogans {
    namespace canvas {

        template<typename T = RGBAPixelui8>
        struct Frame : public util::RefCounted {
            THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Frame)
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (Frame, util::SpinLock)

            typedef T PixelType;

            typename Framebuffer<PixelType>::SharedPtr framebuffer;
            /// \brief
            /// Width and height of framebuffer in pixels.
            /// They are unchangable.
            util::Rectangle bounds;

            /// \brief
            /// ctor. \see{Framebuffer::Framebuffer}
            Frame (
                const util::Rectangle::Extents &extents,
                const PixelType *buffer = 0) :
                framebuffer (new Framebuffer<PixelType> (extents, buffer)),
                bounds (util::Point (), extents) {}
            /// \brief
            /// ctor. Wrap a given framebuffer and provide access to a
            /// rectangular region given by bounds. This ctor allows one
            /// to construct multiple independent, lightweight views in
            /// to a single underlying framebuffer. This makes it very
            /// easy to parellalize algorithms by creating independent
            /// sub-frames and letting multiple threads work simultaneously
            /// on different sub-regions.
            Frame (
                typename Framebuffer<PixelType>::SharedPtr framebuffer_,
                const util::Rectangle &bounds_) :
                framebuffer (framebuffer_),
                bounds (bounds_) {}
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Frame_h)

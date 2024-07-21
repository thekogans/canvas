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
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/RefCounted.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Framebuffer.h"

namespace thekogans {
    namespace canvas {

        /// \struct Frame Frame.h thekogans/canvas/Frame.h
        ///
        /// \brief
        /// Frame is a rectangular view in to a \see{Framebuffer}. By creating multiple,
        /// independent (non overlapping rectangles) views in to the same framebuffer you
        /// can take advantage of graphics' embarasigly easy paralellization strategy.
        /// That's the reason almost all image processing algorithms are defined in Frame
        /// and not in \see[Framebuffer}.

        template<typename T>
        struct Frame : public util::RefCounted {
            THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Frame)
            THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

            typedef T PixelType;

            typename Framebuffer<PixelType>::SharedPtr framebuffer;
            /// \brief
            /// The bounding rectangle of the frame.
            util::Rectangle bounds;

            /// \brief
            /// ctor. \see{Framebuffer::Framebuffer}
            Frame (
                const util::Rectangle::Extents &extents,
                PixelType *buffer = 0,
                const typename util::Array<PixelType>::Deleter &deleter =
                    [] (PixelType * /*array*/) {}) :
                framebuffer (new Framebuffer<PixelType> (extents, buffer, deleter)),
                bounds (util::Point (), extents) {}
            /// \brief
            /// ctor. Wrap a given framebuffer and provide access to a
            /// rectangular region given by bounds. This ctor allows one
            /// to construct multiple independent, lightweight views in
            /// to a single underlying framebuffer. This makes it very
            /// easy to parallelize algorithms by creating independent
            /// sub-frames and letting multiple threads work simultaneously
            /// on different regions of the framebuffer.
            Frame (
                typename Framebuffer<PixelType>::SharedPtr framebuffer_,
                const util::Rectangle &bounds_) :
                framebuffer (framebuffer_),
                bounds (bounds_) {}
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Frame_h)

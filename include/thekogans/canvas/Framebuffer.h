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

#if !defined (__thekogans_canvas_Framebuffer_h)
#define __thekogans_canvas_Framebuffer_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Array.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/RefCounted.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"
#include "thekogans/canvas/Pixel.h"

namespace thekogans {
    namespace canvas {

        template<typename T = RGBAPixelui8>
        struct Framebuffer : public util::RefCounted {
            THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Framebuffer)
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (Framebuffer, util::SpinLock)

            typedef T PixelType;

            /// \brief
            /// Width and height of framebuffer in pixels.
            /// They are unchangable.
            const util::Rectangle::Extents extents;
            /// \brief
            /// Framebuffer data.
            util::Array<PixelType> buffer;

            /// \brief
            /// ctor.
            /// Create a framebuffer with given extents.
            /// Optionally copy the contents of the given buffer.
            /// NOTE: If you choose not to provide data to initialize
            /// the framebuffer, it will be uninitialized. This is
            /// intentional as initializing it to some value would
            /// just be a wastse of time as most apps would call Clear
            /// or perform some other initialization.
            Framebuffer (
                    const util::Rectangle::Extents &extents_,
                    const PixelType *buffer_ = 0) :
                    extents (extents_),
                    buffer (extents.height * extents.width) {
                if (buffer_ != 0) {
                    PixelType *dst = buffer.array;
                    for (std::size_t length = buffer.length; length-- != 0;) {
                        *dst++ = *buffer_++;
                    }
                }
            }

            inline PixelType &PixelAt (
                    util::ui32 x,
                    util::ui32 y) const {
                return buffer[y * extents.width + x];
            }

            inline Color<typename PixelType::ComponentType> ColorAt (
                    util::ui32 x,
                    util::ui32 y) const {
                return PixelAt (x, y).ToColor ();
            }

            template<
                typename U,
                typename ComponentConverter>
            typename Framebuffer<U>::SharedPtr Convert () {
                typename Framebuffer<U>::SharedPtr framebuffer (new Framebuffer<U> (extents));
                const T *src = buffer.array;
                U *dst = framebuffer->buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    // This one line contains three seperate conversions.
                    // First the pixel at *src is converted to Color by the call to ToColor.
                    // Then that color is converted to one with different component types by
                    // the call to one of 30 different Color::Convert specializations.
                    // And finally that color is converted to dst pixel by the = operator.
                    // Through the magic of c++ templates this algorithm can be specialized
                    // up to 120 times (4 pixel types * 6 component types * 5 conversion
                    // specializations for every componenet type).
                    *dst++ = (*src++).ToColor ().template Convert<typename U::ComponentType, ComponentConverter> ();
                }
                return framebuffer;
            }

            void Clear (const Color<typename PixelType::ComponentType> &color) {
                const PixelType pixel (color);
                PixelType *dst = buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    *dst++ = pixel;
                }
            }

            void FlipRows () {
                PixelType *topPixel = buffer.array;
                PixelType *bottomRow = topPixel + (extents.height - 1) * extents.width;
                for (std::size_t rows = extents.height / 2; rows-- != 0;) {
                    PixelType *bottomPixel = bottomRow;
                    for (std::size_t pixels = extents.width; pixels-- != 0;) {
                        std::swap (*topPixel++, *bottomPixel++);
                    }
                    bottomRow -= extents.width;
                }
            }

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Framebuffer)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Framebuffer_h)

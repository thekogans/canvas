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
#include <type_traits>
#include "thekogans/util/Types.h"
#include "thekogans/util/Array.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/RefCounted.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Converter.h"

namespace thekogans {
    namespace canvas {

        template<typename T>
        struct Framebuffer : public util::RefCounted {
            THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Framebuffer)
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (Framebuffer, util::SpinLock)

            typedef T PixelType;
            typedef typename PixelType::ColorType ColorType;

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
            /// or perform some other initialization soon after allocation.
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

            inline ColorType ColorAt (
                    util::ui32 x,
                    util::ui32 y) const {
                return PixelAt (x, y).ToColor ();
            }

            /// \brief
            /// Clear the framebuffer using the given color.
            /// \param[in] color Color to set every pixel too.
            void Clear (const ColorType &color) {
                PixelType pixel (color);
                PixelType *dst = buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    *dst++ = pixel;
                }
            }

            /// \brief
            /// We use (0, 0) as the top left hand corner of the framebuffer.
            /// Other systems use bottom left (OpenGL). Use this method to flip
            /// the rows.
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

            void FlipColumns () {
                PixelType *leftColumn = buffer.array;
                for (std::size_t rows = extents.height; rows-- != 0;) {
                    PixelType *leftPixel = leftColumn;
                    PixelType *rightPixel = leftPixel + extents.width - 1;
                    for (std::size_t pixels = extents.width / 2; pixels-- != 0;) {
                        std::swap (*leftPixel++, *rightPixel--);
                    }
                    leftColumn += extents.width;
                }
            }

            /// \brief
            /// Framebuffer pixel color space and component type conversion template.
            /// Depending on the number of pixel color formats and component
            /// types you use, this algorithm can potentially be specialized
            /// many times.
            ///
            /// Ex:
            ///
            /// \code{.cpp}
            /// // create an 8 bpp RGBA framebuffer.
            /// ui8RGBAFramebuffer::SharedPtr fb1 (
            ///     new ui8RGBAFramebuffer (util::Rectangle::Extents (10, 10)));
            /// // clear it to black.
            /// fb1->Clear (ui8RGBAColor::Black);
            /// // convert it to 16 bpp ABGR framebuffer.
            /// ui16ABGRFramebuffer::SharedPtr fb2 = fb1->Convert<ui16RGBAPixel> ();
            /// \endcode
            ///
            /// \tparam[in] OutPixelType Out framebuffer pixel type.
            /// \tparam[in] IntermediateColorConverterType Pixel color type to converter
            /// intermediate color type.
            /// \tparam[in] OutColorConverterType Converter out color type to out pixel color type.
            /// \return Framebuffer<OutPixelType>::SharedPtr.
            template<
                typename OutPixelType,
                typename IntermediateColorConverterType =
                    Converter<typename Converter<ColorType>::IntermediateColorType>,
                typename OutColorConverterType = Converter<typename OutPixelType::ColorType>>
            typename Framebuffer<OutPixelType>::SharedPtr Convert () {
                typename Framebuffer<OutPixelType>::SharedPtr framebuffer (
                    new Framebuffer<OutPixelType> (extents));
                const PixelType *src = buffer.array;
                OutPixelType *dst = framebuffer->buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    typedef typename OutPixelType::ConverterColorType ConverterOutColorType;
                    typedef typename Converter<ColorType>::IntermediateColorType
                        ConverterIntermediateColorType;
                    // This statement contains 6 separate conversions.
                    //
                    // 1 - Swizzle src pixel to color
                    // 2 - same color space component type converter
                    // 3 - converter intermediate color converter
                    // 4 - out color converter
                    // 5 - same out color space component type converter
                    // 6 - Swizzle final color to dst pixel
                    //
                    // The reason for so many is we need to do some intermediate conversions
                    // to keep the combinatorial explosion of color space conversions down to
                    // a minimum. This way we only need to know how to convert all to f32RGBAColor
                    // and f32RGBAColor to all others.
                    //
                    // This design flexibility exists because doing color space conversion
                    // is not trivial and no automatic approach can possibly exist given that
                    // different color spaces have different componenet types and ranges.
                    // Fear not, as that's where the power of this design comes in. Because
                    // this design uses all static typing, known to the compiler, most of the
                    // cost is mitigated by a good compiler optimizing away parts of this
                    // statemnt that are noop for their particular color/component type
                    // combinations.
                    *dst++ =
                        OutColorConverterType::Convert (
                            Converter<ConverterOutColorType>::Convert (
                                Converter<ConverterIntermediateColorType>::Convert (
                                    IntermediateColorConverterType::Convert ((*src++).ToColor ()))));
                }
                return framebuffer;
            }

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Framebuffer)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Framebuffer_h)

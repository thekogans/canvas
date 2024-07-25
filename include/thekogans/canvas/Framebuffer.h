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

        /// \struct Framebuffer Framebuffer.h thekogans/canvas/Framebuffer.h
        ///
        /// \brief
        /// A framebuffer is a rectangular array of pixels. The width and height
        /// of the rectangle are given by extents. The pixels are completely generic
        /// and can be of practically any type (as long as they expose a few simple
        /// types and provide a copy ctor and an assignment operator for their native
        /// color type). See \see{RGBAPixel} for an example. The framebuffer uses
        /// template metaprogamming to parameterize it's algorithms. This design allows
        /// pixel and color type interoperability at the cost of potentially many
        /// instances (specializations). By using a good optimizing compiler run time
        /// cost is kept to an absolute minimum.

        template<typename T>
        struct Framebuffer : public util::RefCounted {
            /// \brief
            /// Declare \see{RefCounted} pointers.
            THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Framebuffer)
            /// \brief
            /// Framebuffer has a private heap to help with performance and memory fragmentation.
            THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

            /// \brief
            /// Framebuffer pixel type.
            typedef T PixelType;
            /// \brief
            /// Pixel color type.
            typedef typename PixelType::ColorType ColorType;

            /// \brief
            /// Width and height of framebuffer in pixels. They are unchangable.
            const util::Rectangle::Extents extents;
            /// \brief
            /// Framebuffer data.
            util::Array<PixelType> buffer;

            /// \brief
            /// ctor.
            /// Create a framebuffer with given extents.
            /// Optionally wrap the contents of the given buffer.
            /// \param[in] extents_ Framebuffer width and height.
            /// \param[in] buffer_ Optional Array of pixels to wrap with the framebuffer.
            /// \param[in] deleter Deleter used to deallocater the buffer_ pointer.
            Framebuffer (
                const util::Rectangle::Extents &extents_,
                PixelType *buffer_ = 0,
                const typename util::Array<PixelType>::Deleter &deleter =
                    [] (PixelType * /*array*/) {}) :
                extents (extents_),
                buffer (extents.height * extents.width, buffer_, deleter) {}

            /// \brief
            /// Return a pixel reference at the given coordinates.
            /// \param[in] x x coordinate of the pixel.
            /// \param[in] y y coordinate of the pixel.
            /// \return Pixel reference at the given coordinates.
            inline PixelType &PixelAt (
                    util::ui32 x,
                    util::ui32 y) const {
                return buffer[y * extents.width + x];
            }

            /// \brief
            /// Return the pixel color at the given coordinates.
            /// \param[in] x x coordinate of the pixel.
            /// \param[in] y y coordinate of the pixel.
            /// \return Pixel color at the given coordinates.
            inline ColorType ColorAt (
                    util::ui32 x,
                    util::ui32 y) const {
                return PixelAt (x, y).ToColor ();
            }

            /// \brief
            /// Return a deep copy of the framebuffer.
            /// \return A deep copy of the framebuffer.
            SharedPtr Copy () const {
                SharedPtr framebuffer (new Framebuffer<PixelType> (extents));
                PixelType *src = buffer.array;
                PixelType *dst = framebuffer->buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    *dst++ = *src++;
                }
                return framebuffer;
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
            /// Mirror the framebuffer across the x-axis.
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

            /// \brief
            /// Mirror the framebuffer across the y-axis.
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
            /// \tparam OutPixelType Out framebuffer pixel type.
            /// \tparam ConverterIntermediateColorConverterType Pixel color type to converter
            /// intermediate color type.
            /// \tparam OutColorConverterType Converter out color type to out pixel color type.
            /// \return Framebuffer<OutPixelType>::SharedPtr.
            template<
                typename OutPixelType,
                typename ConverterIntermediateColorConverterType =
                    Converter<typename Converter<ColorType>::IntermediateColorType>,
                typename OutColorConverterType = Converter<typename OutPixelType::ColorType>>
            typename Framebuffer<OutPixelType>::SharedPtr Convert () const {
                typename Framebuffer<OutPixelType>::SharedPtr framebuffer (
                    new Framebuffer<OutPixelType> (extents));
                const PixelType *src = buffer.array;
                OutPixelType *dst = framebuffer->buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    typedef typename OutPixelType::ColorType::ConverterColorType ConverterOutColorType;
                    typedef typename Converter<ColorType>::IntermediateColorType
                        ConverterIntermediateColorType;
                    // This statement contains 6 separate conversions.
                    //
                    // 1 - Swizzle src pixel to color
                    // 2 - same color space component type converter
                    // 3 - converter intermediate color converter
                    // 4 - out color converter
                    // 5 - same out color space component type converter
                    // 6 - Swizzle out color to dst pixel
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
                    *dst++ = // 6 - Swizzle out color to dst pixel
                        // 5 - same out color space component type converter
                        OutColorConverterType::Convert (
                            // 4 - out color converter
                            Converter<ConverterOutColorType>::Convert (
                                // 3 - converter intermediate color converter
                                Converter<ConverterIntermediateColorType>::Convert (
                                    // 2 - same color space component type converter
                                    ConverterIntermediateColorConverterType::Convert (
                                        // 1 - Swizzle src pixel to color
                                        (*src++).ToColor ()))));
                }
                return framebuffer;
            }

            /// \brief
            /// There's no need to go through all the trouble of duplicating code
            /// for copy ctor and = operator. If you need a deep copy of a framebuffer
            /// just do this;
            ///
            /// \code{.cpp}
            /// ui8RGBAFramebuffer::SharedPtr fb2 (
            ///     new ui8RGBAFramebuffer (fb1->extents, fb1->buffer.array));
            /// \endcode
            ///
            /// This design forces most (all?) framebuffers to be allocated on the heap
            /// and shared among many \see{Frame}s.
            THEKOGANS_UTIL_DISALLOW_COPY_AND_ASSIGN (Framebuffer)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Framebuffer_h)

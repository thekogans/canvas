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
#include "thekogans/canvas/ComponentConverter.h"
#include "thekogans/canvas/Converter.h"
#include "thekogans/canvas/RGBAConverter.h"

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
            /// Framebuffer pixel format and type conversion template.
            /// Depending on the number of pixel formats and component
            /// types you use, this algorithm can potentially be specialized
            /// hundreds of times.
            /// Ex:
            /// \code{.cpp}
            /// // create an 8 bpp RGBA framebuffer.
            /// ui8RGBAFramebuffer::SharedPtr fb1 (new ui8RGBAFramebuffer (util::Rectangle::Extents (10, 10)));
            /// // clear it to black.
            /// fb1->Clear (ui8RGBAColor::Black);
            /// // convert it to 16 bpp ABGR framebuffer using an 8 to 16 bit scaling converter.
            /// ui16ABGRFramebuffer::SharedPtr fb2 = fb1->Convert<ui16RGBAPixel, ui8Toui16ScaleComponentConverter> ();
            /// \endcode
            /// \tparam[in] OutPixelType Out framebuffer pixel type.
            /// \tparam[in] ComponentConverterType Type exposing InComponentType,
            /// OutComponentType and Convert ().
            /// \return Framebuffer<OutPixelType>::SharedPtr.
            template<
                typename OutPixelType,
                typename OutPixelConverterOutColorComponentTypeToOutPixelComponentComponentConverterType =
                    ComponentConverter<
                        typename Converter<OutPixelType>::OutColorType::ComponentType,
                        typename OutPixelType::ComponentType>,
                typename PixelComponentTypeToPixelConverterIntermediateColorComponentComponentConverterType =
                    ComponentConverter<
                        typename PixelType::ComponentType,
                        typename Converter<PixelType>::IntermediateColorType::ComponentType>>
            typename Framebuffer<OutPixelType>::SharedPtr Convert () {
                typedef typename OutPixelType::ColorType OutColorType;
                typedef typename Converter<OutColorType>::OutColorType OutPixelConverterOutColorType;
                typedef typename PixelType::ColorType PixelColorType;
                typedef typename Converter<PixelColorType>::IntermediateColorType PixelConverterIntermediateColorType;
                static_assert (
                    std::is_same<
                        typename PixelType::ComponentType,
                        typename PixelComponentTypeToPixelConverterIntermediateColorComponentComponentConverterType::InComponentType>::value,
                    "Incompatible pixel component and pixel component converter types.");
                static_assert (
                    std::is_same<
                        typename OutPixelConverterOutColorComponentTypeToOutPixelComponentComponentConverterType::OutComponentType,
                        typename OutPixelType::ComponentType>::value,
                    "Incompatible out pixel component and out pixel component converter types.");
                typename Framebuffer<OutPixelType>::SharedPtr framebuffer (new Framebuffer<OutPixelType> (extents));
                const PixelType *src = buffer.array;
                OutPixelType *dst = framebuffer->buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    // This statement contains 6 separate conversions. The reason for so
                    // many is we need to do some intermediate conversions to keep the
                    // combinatorial explosion of color space conversions down to a
                    // minimum. This way we only need to know how to convert all to
                    // f32RGBAColor and f32RGBAColor to all others.
                    //
                    // Ex:
                    //
                    // I purposely pick two 'completely' different (all types different)
                    // pixels to illustrate what each step in the conversion process does.
                    //
                    // ui16ACMYPixel -> ui32AXYZPixel
                    //
                    // ui16ACMYPixel -> ui16CMYAColor -> f32CMYAColor -> f32RGBAColor ->
                    // f32XYZAColor -> ui32XYZAColor -> ui32AXYZPixel
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
                        Converter<OutPixelConverterOutColorType>::Convert (
                            Converter<PixelConverterIntermediateColorType>::Convert (
                                (*src++).ToColor ().
                                template ConvertComponents<PixelComponentTypeToPixelConverterIntermediateColorComponentComponentConverterType> ())).
                        template ConvertComponents<OutPixelConverterOutColorComponentTypeToOutPixelComponentComponentConverterType> ();
                }
                return framebuffer;
            }

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Framebuffer)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Framebuffer_h)

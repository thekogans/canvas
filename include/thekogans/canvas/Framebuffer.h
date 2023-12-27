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
#include "thekogans/canvas/RGBAColor.h"

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
            /// fb1->Clear (ui8RGBAColor (0, 0, 0, 0));
            /// // convert it to 16 bpp ABGR framebuffer using an 8 to 16 bit scaling converter.
            /// ui16ABGRFramebuffer::SharedPtr fb2 = fb1->Convert<ui16RGBAPixel, ui8Toui16ScaleComponentConverter> ();
            /// \endcode
            /// \tparam[in] OutPixelType Out framebuffer pixel type.
            /// \tparam[in] ComponentConverterType Type exposing InComponentType,
            /// OutComponentType and Convert ().
            /// \return Framebuffer<OutPixelType>::SharedPtr.
            template<
                typename OutPixelType,
                typename ComponentConverterType>
            typename Framebuffer<OutPixelType>::SharedPtr Convert () {
                typedef typename PixelType::ComponentType PixelComponentType;
                typedef typename PixelType::ConverterColorType PixelConverterColorType;
                typedef typename OutPixelType::ComponentType OutPixelComponentType;
                typedef typename OutPixelType::ConverterColorType OutPixelConverterColorType;
                typedef typename ComponentConverterType::InComponentType ComponentConverterInComponentType;
                typedef typename ComponentConverterType::OutComponentType ComponentConverterOutComponentType;
                typedef typename Converter<PixelConverterColorType>::ComponentType ConverterColorComponentType;
                typedef typename Converter<PixelConverterColorType>::IntermediateColorType ConverterIntermediateColorType;
                typedef DefaultComponentConverter<
                    PixelComponentType,
                    ConverterColorComponentType> PixelComponentToConverterComponentType;
                typedef DefaultComponentConverter<
                    ConverterColorComponentType,
                    ComponentConverterInComponentType> ConverterComponentTypeToComponentConverterInComponentType;
                static_assert (
                    std::is_same<OutPixelComponentType, ComponentConverterOutComponentType>::value,
                    "Incompatible pixel and converter types.");
                typename Framebuffer<OutPixelType>::SharedPtr framebuffer (
                    new Framebuffer<OutPixelType> (extents));
                const PixelType *src = buffer.array;
                OutPixelType *dst = framebuffer->buffer.array;
                for (std::size_t length = buffer.length; length-- != 0;) {
                    // This statement contains 7 seperate conversions.
                    // 1 - Pixel at *src is converted to it's color type by the call to ToColor ().
                    // 2 - That color's components are converted to ConverterColorComponentType color by
                    // a call to template ConvertComponents<PixelComponentToConverterComponentType> ().
                    // 3 - That color is converted to ConverterIntermediateColorType color by a call to
                    // Converter<ConverterIntermediateColorType>::Convert ().
                    // 4 - That color is then converted to OutPixelConverterColorType color by a call to
                    // Converter<OutPixelConverterColorType>::Convert ().
                    // 5 - That color is then converted to a ComponentConverterInComponentType color by a
                    // call to template ConvertComponents<ConverterComponentTypeToComponentConverterInComponentType> ().
                    // 6 - That color is then converted to OutPixelComponentType color by a call to
                    // template ConvertComponents<ComponentConverterType> ().
                    // 7 - And finally that color is converted to dst pixel by the = operator.
                    *dst++ =
                        Converter<OutPixelConverterColorType>::Convert (
                            Converter<ConverterIntermediateColorType>::Convert (
                                (*src++).ToColor ().
                                template ConvertComponents<PixelComponentToConverterComponentType> ())).
                        template ConvertComponents<ConverterComponentTypeToComponentConverterInComponentType> ().
                        template ConvertComponents<ComponentConverterType> ();
                }
                return framebuffer;
            }

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Framebuffer)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Framebuffer_h)

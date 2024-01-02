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

#if !defined (__thekogans_canvas_Pixel_h)
#define __thekogans_canvas_Pixel_h

#include "thekogans/util/Types.h"
#include "thekogans/canvas/Color.h"

namespace thekogans {
    namespace canvas {

        /// \struct Pixel Pixel.h thekogans/canvas/Pixel.h
        ///
        /// \brief
        /// Pixel is a generic type that represents the interface a concrete
        /// pixel type must expose in order to be used by the \see{Framebuffer}
        /// machinery. As you can see the requirements are minimal. A few typedefs,
        /// a copy ctor and an assignment operator, for the pixel's respective
        /// \see{Color}, and a ToColor method.
        /// NOTE: The difference between a pixel and it's color is component layout.
        /// While a color represents a canonical in memory layout of an instance of
        /// a color space color, a pixel can lay them out any way it wishes. It's one
        /// and only job is to swizzle it's components to and from it's canonical color.
        /// This header is not meant to be included dirctly. It's meant to be used as
        /// a template to define concrete pixel types (see \see{RGBAPixel}).
        /// \tparam T Pixel component type.

        template<typename T>
        struct Pixel {
            /// \brief
            /// Pixel component type.
            typedef T ComponentType;
            /// \brief
            /// \pixel color type.
            typedef Color<ComponentType> ColorType;

            /// \brief
            /// ctor.
            /// NOTE: We don't initialize anything here. We want to be as close as we can to POTs.
            /// Smart compilers will understand that and will elide the ctor when we allocate large
            /// arrays of pixels (see \see{Framebuffer}). It's undestood that initialization will be
            /// performed soon aftor allocation and doing it twice would just be a waste of time.
            Pixel () {}
            /// \brief
            /// ctor. Swizzle color to pixel.
            /// \param[in] color Canonical pixel color.
            Pixel (const ColorType &color);

            /// \brief
            ///
            Pixel &operator = (const ColorType &color);

            /// \brief
            /// Swizzle pixel to color.
            /// \return Canonical pixel color.
            ColorType ToColor () const;
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Pixel_h)

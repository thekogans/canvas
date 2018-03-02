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

#if !defined (__thekogans_canvas_Bitmap_h)
#define __thekogans_canvas_Bitmap_h

#include <memory>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (_WINDOWS_)
        #if !defined (WIN32_LEAN_AND_MEAN)
            #define WIN32_LEAN_AND_MEAN
        #endif // !defined (WIN32_LEAN_AND_MEAN)
        #if !defined (NOMINMAX)
            #define NOMINMAX
        #endif // !defined (NOMINMAX)
        #include <windows.h>
    #endif // !defined (_WINDOWS_)
#elif defined (TOOLCHAIN_OS_Linux)
#if defined (THEKOGANS_CANVAS_USE_XLIB)
    #include <X11/Xlib.h>
#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#endif // defined (TOOLCHAIN_OS_Windows)
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/Exception.h"
#include "thekogans/canvas/Color.h"
#include "thekogans/canvas/Rectangle.h"
#include "thekogans/canvas/RGBImage.h"
#include "thekogans/canvas/Config.h"

namespace thekogans {
    namespace canvas {

        struct Window;

        struct _LIB_THEKOGANS_CANVAS_DECL Bitmap : public RGBImage {
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (Bitmap, util::SpinLock)

        #if defined (TOOLCHAIN_OS_Windows)
            HBITMAP bitmap;
            struct MemoryDC {
                HDC dc;
                HGDIOBJ oldBitmap;
                MemoryDC () :
                    dc (0) {}
                ~MemoryDC () {
                    Destroy ();
                }
                void Create (
                    HDC dc_,
                    HBITMAP bitmap);
                void Destroy ();
            } memoryDC;
        #elif defined (TOOLCHAIN_OS_Linux)
        #if defined (THEKOGANS_CANVAS_USE_XLIB)
            XImage *image;
        #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
        #elif defined (TOOLCHAIN_OS_OSX)
        #endif // defined (TOOLCHAIN_OS_Windows)

        #if defined (TOOLCHAIN_OS_Windows)
            Bitmap () :
                bitmap (0) {}
            explicit Bitmap (const Window &window) :
                    bitmap (0) {
                Create (window);
            }
        #elif defined (TOOLCHAIN_OS_Linux)
        #if defined (THEKOGANS_CANVAS_USE_XLIB)
            Bitmap () :
                image (0) {}
            explicit Bitmap (const Window &window) :
                    image (0) {
                Create (window);
            }
        #else // defined (THEKOGANS_CANVAS_USE_XLIB)
            Bitmap () {}
            explicit Bitmap (const Window &window) {
                Create (window);
            }
        #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
        #elif defined (TOOLCHAIN_OS_OSX)
            Bitmap () {}
            explicit Bitmap (const Window &window) {
                Create (window);
            }
        #endif // defined (TOOLCHAIN_OS_Windows)
            virtual ~Bitmap () {
                Destroy ();
            }

            void Create (
                const Window &window,
                const Rectangle::Extents &extents_ = Rectangle::Extents (),
                bool clear = false);
            void Destroy ();

            /// \brief
            /// Copy a rectangle from a given window.
            /// \param[in] window window from which to copy.
            /// \param[in] rectangle window rectangle to copy.
            /// \param[in] origin offset of top-left corner in
            /// the bitmap where to place the rectangle.
            void DrawWindow (
                const Window &window,
                const Rectangle &rectangle = Rectangle (),
                const Point &origin = Point ());

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Bitmap)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Bitmap_h)

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

#if !defined (__thekogans_canvas_Window_h)
#define __thekogans_canvas_Window_h

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
    #include <X11/Xutil.h>
#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#elif defined (TOOLCHAIN_OS_OSX)
    #include <CoreGraphics/CoreGraphics.h>
#endif // defined (TOOLCHAIN_OS_Windows)
#include "thekogans/util/Types.h"
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/OwnerList.h"
#include "thekogans/canvas/RGBImage.h"
#include "thekogans/canvas/Config.h"

namespace thekogans {
    namespace canvas {

        struct Bitmap;

        struct _LIB_THEKOGANS_CANVAS_DECL Window {
        #if defined (TOOLCHAIN_OS_Windows)
            HWND wnd;
            HDC dc;
        #elif defined (TOOLCHAIN_OS_Linux)
        #if defined (THEKOGANS_CANVAS_USE_XLIB)
            Display *display;
            ::Window window;
            GC gc;
        #else // defined (THEKOGANS_CANVAS_USE_XLIB)
            util::Rectangle rectangle;
            const Window *parent;
            util::OwnerList<Window> children;
            bool visible;
            RGBImage image;
        #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
        #elif defined (TOOLCHAIN_OS_OSX)
            CGWindowID windowID;
            CGContextRef context;
        #endif // defined (TOOLCHAIN_OS_Windows)

        #if defined (TOOLCHAIN_OS_Windows)
            Window () :
                wnd (0),
                dc (0) {}
            Window (const util::Rectangle &rectangle,
                    const Window &parent) :
                    wnd (0),
                    dc (0) {
                Create (rectangle, parent);
            }
        #elif defined (TOOLCHAIN_OS_Linux)
        #if defined (THEKOGANS_CANVAS_USE_XLIB)
            Window () :
                display (0),
                window (0),
                gc (0) {}
            Window (const util::Rectangle &rectangle,
                    const Window &parent) :
                    display (parent.display),
                    window (0),
                    gc (0) {
                Create (rectangle, parent);
            }
        #else // defined (THEKOGANS_CANVAS_USE_XLIB)
            Window () :
                parent (0),
                visible (false) {}
            Window (const util::Rectangle &rectangle,
                    const Window &parent) :
                    parent (0),
                    visible (false) {
                Create (rectangle, parent);
            }
        #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
        #elif defined (TOOLCHAIN_OS_OSX)
            Window () :
                windowID (0),
                context (0) {}
            Window (const util::Rectangle &rectangle,
                    const Window &parent) :
                    windowID (0),
                    context (0) {
                Create (rectangle, parent);
            }
        #endif // defined (TOOLCHAIN_OS_Windows)
            virtual ~Window () {
                Destroy ();
            }

            static util::OwnerVector<Window> monitors;
            static void EnumerateMonitors ();

            static void PumpMessages ();
            static void StopPumpingMessages ();

            virtual void Create (
                const util::Rectangle &rectangle,
                const Window &parent);
            virtual void Destroy ();

            virtual void SetSize () {}
            virtual void Draw () {}

            virtual void Show ();
            virtual void Hide ();

            virtual util::Rectangle GetRectangle () const;

            inline util::ui32 GetComponentIndices () const {
                // FIXME: need to find out dynamically.
                return RGBImage::R2G1B0A3;
            }

            inline util::ui32 GetPixelStride () const {
                // FIXME: need to find out dynamicaly.
            #if defined (TOOLCHAIN_OS_Windows)
                return GetDeviceCaps (dc, BITSPIXEL) >> 3;
            #elif defined (TOOLCHAIN_OS_Linux)
                return 4;
            #elif defined (TOOLCHAIN_OS_OSX)
                return 4;
            #endif // defined (TOOLCHAIN_OS_Windows)
            }

            inline util::ui32 GetRowStride () const {
                util::ui32 rowStride =
                    GetRectangle ().extents.width * GetPixelStride ();
            #if defined (TOOLCHAIN_OS_Windows)
                // Windows requires that each row be DWORD aligned.
                rowStride = (rowStride + 3) & ~3;
            #endif // defined (TOOLCHAIN_OS_Windows)
                return rowStride;
            }

        #if defined (TOOLCHAIN_OS_OSX)
            virtual CGImageRef GetImage () const;
        #endif // defined (TOOLCHAIN_OS_OSX)

            void DrawBitmap (
                const Bitmap &bitmap,
                const util::Rectangle &rectangle = util::Rectangle (),
                const util::Point &origin = util::Point ());

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Window)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Window_h)

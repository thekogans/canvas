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

#include <cassert>
#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_OSX)
    #include <CoreFoundation/CoreFoundation.h>
    #include <CoreGraphics/CoreGraphics.h>
#endif // defined (TOOLCHAIN_OS_OSX)
#include "thekogans/util/Types.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/LoggerMgr.h"
#if defined (TOOLCHAIN_OS_Linux)
#if defined (THEKOGANS_CANVAS_USE_XLIB)
    #include "thekogans/core/Xlib.h"
#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#endif // defined (TOOLCHAIN_OS_Linux)
#include "thekogans/canvas/Window.h"
#include "thekogans/canvas/Bitmap.h"

namespace thekogans {
    namespace canvas {

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK (Bitmap, util::SpinLock)

    #if defined (TOOLCHAIN_OS_Windows)
        void Bitmap::MemoryDC::Create (
                HDC dc_,
                HBITMAP bitmap) {
            Destroy ();
            dc = CreateCompatibleDC (dc_);
            if (dc != 0) {
                oldBitmap = SelectObject (dc, bitmap);
            }
            else {
                THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                    THEKOGANS_UTIL_OS_ERROR_CODE);
            }
        }

        void Bitmap::MemoryDC::Destroy () {
            if (dc != 0) {
                SelectObject (dc, oldBitmap);
                DeleteDC (dc);
                dc = 0;
                oldBitmap = 0;
            }
        }
    #endif // defined (TOOLCHAIN_OS_Windows)

    #if defined (TOOLCHAIN_OS_Linux)
    #if defined (THEKOGANS_CANVAS_USE_XLIB)
        namespace {
            util::ui32 GetWindowPixelDepth (const Window &window) {
                if (window.window != 0) {
                    XWindowAttributes attributes;
                    XGetWindowAttributes (
                        window.display, window.window, &attributes);
                    return attributes.depth;
                }
                return 0;
            }
        }
    #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
    #endif // defined (TOOLCHAIN_OS_Linux)

        void Bitmap::Create (
                const Window &window,
                const util::Rectangle::Extents &extents_,
                bool clear) {
            Destroy ();
            if (extents_ == util::Rectangle::Extents ()) {
                extents = window.GetRectangle ().extents;
            }
            else {
                extents = extents_;
            }
            componentIndices = window.GetComponentIndices ();
            pixelStride = window.GetPixelStride ();
            rowStride = extents.width * pixelStride;
        #if defined (TOOLCHAIN_OS_Windows)
            // Windows requires that each row be DWORD aligned.
            rowStride = (rowStride + 3) & ~3;
            struct BitmapInfo : public BITMAPINFO {
                BitmapInfo (
                        const util::Rectangle::Extents &extents,
                        util::ui32 pixelStride,
                        util::ui32 rowStride) {
                    ZeroMemory (this, sizeof (BITMAPINFO));
                    bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
                    bmiHeader.biWidth = extents.width;
                    bmiHeader.biHeight = -(int)extents.height;
                    bmiHeader.biPlanes = 1;
                    bmiHeader.biBitCount = pixelStride << 3;
                    bmiHeader.biCompression = BI_RGB;
                    bmiHeader.biSizeImage = extents.height * rowStride;
                    bmiHeader.biXPelsPerMeter = 3780;
                    bmiHeader.biYPelsPerMeter = 3780;
                    bmiHeader.biClrUsed = 0;
                    bmiHeader.biClrImportant = 0;
                }
            } bitmapInfo (extents, pixelStride, rowStride);
            bitmap = CreateDIBSection (window.dc,
                &bitmapInfo, DIB_RGB_COLORS, (void **)&data, 0, 0);
            if (bitmap == 0 || data == 0) {
                THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                    THEKOGANS_UTIL_OS_ERROR_CODE);
            }
            memoryDC.Create (window.dc, bitmap);
        #elif defined (TOOLCHAIN_OS_Linux)
        #if defined (THEKOGANS_CANVAS_USE_XLIB)
            data = (util::ui8 *)malloc (
                extents.height * rowStride);
            if (data != 0) {
                image = XCreateImage (
                    window.display,
                    CopyFromParent,
                    GetWindowPixelDepth (window),
                    ZPixmap,
                    0,
                    (char *)data,
                    extents.width,
                    extents.height,
                    pixelStride << 3,
                    rowStride);
                if (image == 0) {
                    free (data);
                    data = 0;
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "%s", "Unable to create image.");
                }
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to allocate image data: %ux%u",
                    extents.width, extents.height);
            }
        #else // defined (THEKOGANS_CANVAS_USE_XLIB)
            data = new util::ui8[extents.height * rowStride];
            if (data != 0) {
                owner = true;
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to allocate image data: %ux%u",
                    extents.width, extents.height);
            }
        #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
        #elif defined (TOOLCHAIN_OS_OSX)
            data = new util::ui8[extents.height * rowStride];
            if (data != 0) {
                owner = true;
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to allocate image data: %ux%u",
                    extents.width, extents.height);
            }
        #endif // defined (TOOLCHAIN_OS_Windows)
            if (clear) {
                memset (data, 0, extents.height * rowStride);
            }
        }

        void Bitmap::Destroy () {
        #if defined (TOOLCHAIN_OS_Windows)
            if (bitmap != 0) {
                DeleteObject (bitmap);
                bitmap = 0;
                data = 0;
            }
        #elif defined (TOOLCHAIN_OS_Linux)
        #if defined (THEKOGANS_CANVAS_USE_XLIB)
            if (image != 0) {
                XDestroyImage (image);
                image = 0;
                data = 0;
            }
        #else // defined (THEKOGANS_CANVAS_USE_XLIB)
            Release ();
        #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
        #elif defined (TOOLCHAIN_OS_OSX)
            Release ();
        #endif // defined (TOOLCHAIN_OS_Windows)
        }

        void Bitmap::DrawWindow (
                const Window &window,
                const util::Rectangle &rectangle,
                const util::Point &origin) {
            util::Rectangle windowRectangle =
                util::Rectangle (util::Point (), window.GetRectangle ().extents);
            util::Rectangle srcRectangle =
                rectangle.IsDegenerate () ?
                windowRectangle : rectangle.Intersection (windowRectangle);
            util::Rectangle dstRectangle =
                util::Rectangle (origin, srcRectangle.extents).Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate () && !dstRectangle.IsDegenerate ()) {
            #if defined (TOOLCHAIN_OS_Windows)
                BitBlt (
                    memoryDC.dc,
                    dstRectangle.origin.x,
                    dstRectangle.origin.y,
                    dstRectangle.extents.width,
                    dstRectangle.extents.height,
                    window.dc,
                    srcRectangle.origin.x,
                    srcRectangle.origin.y,
                    SRCCOPY);
            #elif defined (TOOLCHAIN_OS_Linux)
            #if defined (THEKOGANS_CANVAS_USE_XLIB)
                XImage *tmpImage = XGetImage (
                    window.display,
                    window.window,
                    srcRectangle.origin.x,
                    srcRectangle.origin.y,
                    srcRectangle.extents.width,
                    srcRectangle.extents.height,
                    AllPlanes,
                    ZPixmap);
                if (tmpImage != 0) {
                    if (dstRectangle == GetRectangle ()) {
                        Destroy ();
                        image = tmpImage;
                        data = (util::ui8 *)image->data;
                    }
                    else {
                        Bitmap tmpBitmap;
                        tmpBitmap.image = tmpImage;
                        tmpBitmap.data = (util::ui8 *)image->data;
                        tmpBitmap.extents.width = image->width;
                        tmpBitmap.extents.height = image->height;
                        tmpBitmap.componentIndices = window.GetComponentIndices ();
                        tmpBitmap.pixelStride = image->bitmap_unit;
                        tmpBitmap.rowStride = image->bytes_per_line;
                        tmpBitmap.owner = false;
                        tmpBitmap.Copy (GetRectangle (), dstRectangle.origin, *this);
                    }
                }
                else {
                    THEKOGANS_UTIL_LOG_ERROR ("%s\n", "tmpImage == 0");
                }
            #else // defined (THEKOGANS_CANVAS_USE_XLIB)
                window.image.Copy (rectangle, origin, *this);
            #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
            #elif defined (TOOLCHAIN_OS_OSX)
                struct OSXImageRef {
                    CGImageRef image;
                    OSXImageRef (CGImageRef image_) :
                        image (image_) {}
                    ~OSXImageRef () {
                        if (image != 0) {
                            CGImageRelease (image);
                        }
                    }
                } imageRef (window.GetImage ());
                if (imageRef.image != 0) {
                    struct OSXDataRef {
                        CFDataRef data;
                        OSXDataRef (CFDataRef data_) :
                            data (data_) {}
                        ~OSXDataRef () {
                            if (data != 0) {
                                CFRelease (data);
                            }
                        }
                    } dataRef (CGDataProviderCopyData (CGImageGetDataProvider (imageRef.image)));
                    if (dataRef.data != 0) {
                        util::ui8 *data =
                            (util::ui8 *)CFDataGetBytePtr (dataRef.data);
                        if (data != 0) {
                            RGBImage (
                                data,
                                util::Rectangle::Extents (
                                    CGImageGetWidth (imageRef.image),
                                    CGImageGetHeight (imageRef.image)),
                                window.GetComponentIndices (),
                                CGImageGetBitsPerPixel (imageRef.image) / 8,
                                CGImageGetBytesPerRow (imageRef.image),
                                false).Copy (srcRectangle, dstRectangle.origin, *this);
                        }
                        else {
                            THEKOGANS_UTIL_LOG_ERROR ("%s\n", "data == 0");
                        }
                    }
                    else {
                        THEKOGANS_UTIL_LOG_ERROR ("%s\n", "dataRef == 0");
                    }
                }
                else {
                    THEKOGANS_UTIL_LOG_ERROR ("%s\n", "image == 0");
                }
            #endif // defined (TOOLCHAIN_OS_Windows)
            }
        }

    } // namespace canvas
} // namespace thekogans

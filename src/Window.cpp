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
#if defined (TOOLCHAIN_OS_Linux)
#if defined (THEKOGANS_CANVAS_USE_XLIB)
    #include <X11/Xatom.h>
    #include <X11/extensions/Xinerama.h>
#else // defined (THEKOGANS_CANVAS_USE_XLIB)
    #include <linux/fb.h>
    #include <linux/kd.h>
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <fcntl.h>
#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#endif // defined (TOOLCHAIN_OS_Linux)
#include "thekogans/util/Types.h"
#include "thekogans/util/Path.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/util/Mutex.h"
#include "thekogans/util/Condition.h"
#include "thekogans/util/LockGuard.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/LoggerMgr.h"
#if defined (TOOLCHAIN_OS_Linux)
#if defined (THEKOGANS_CANVAS_USE_XLIB)
    #include "thekogans/canvas/Xlib.h"
#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#endif // defined (TOOLCHAIN_OS_Linux)
#include "thekogans/canvas/Bitmap.h"
#include "thekogans/canvas/Window.h"

namespace thekogans {
    namespace canvas {

        util::OwnerVector<Window> Window::monitors;

    #if defined (TOOLCHAIN_OS_Windows)
        namespace {
            struct Monitor : public Window {
                util::ui32 index;
                Rectangle rectangle;
                Monitor (util::ui32 index_, HMONITOR monitor) :
                        index (index_) {
                    MONITORINFOEX monitorInfo;
                    monitorInfo.cbSize = sizeof (MONITORINFOEX);
                    GetMonitorInfo (monitor, &monitorInfo);
                    dc = CreateDC (0, monitorInfo.szDevice, 0, 0);
                    rectangle.origin.x = monitorInfo.rcMonitor.left;
                    rectangle.origin.y = monitorInfo.rcMonitor.top;
                    rectangle.extents.width =
                        monitorInfo.rcMonitor.right -
                        monitorInfo.rcMonitor.left;
                    rectangle.extents.height =
                        monitorInfo.rcMonitor.bottom -
                        monitorInfo.rcMonitor.top;
                }
                // WARNING: Don't fuck with this! You might think
                // that because this dtor and Window dtor are
                // identical, you can remove this one. WRONG!!!
                // The standard expressly requires that ctors and
                // dtors collapse vtables, and call their own. If
                // you delete this dtor, Monitor::Destroy will
                // never be called. (and that's a very bad thing)
                virtual ~Monitor () {
                    Destroy ();
                }

                virtual void Destroy () {
                    if (dc != 0) {
                        DeleteDC (dc);
                        dc = 0;
                    }
                    assert (wnd == 0);
                }

                virtual Rectangle GetRectangle () const {
                    return rectangle;
                }

                typedef std::pair<util::ui32, util::ui32> DPIInfo;
                inline DPIInfo GetDPIInfo () const {
                    return DPIInfo (
                        util::ROUND (
                            GetDeviceCaps (dc, HORZSIZE) * 25.4f / GetDeviceCaps (dc, HORZRES)),
                        util::ROUND (
                            GetDeviceCaps (dc, VERTSIZE) * 25.4f / GetDeviceCaps (dc, VERTRES)));
                }
            };

            BOOL CALLBACK MonitorEnumProc (
                    HMONITOR monitor,
                    HDC /*dc*/,
                    LPRECT /*rect*/,
                    LPARAM /*data*/) {
                Window::monitors.push_back (
                    new Monitor ((util::ui32)Window::monitors.size (), monitor));
                return TRUE;
            }
        }

        void Window::EnumerateMonitors () {
            monitors.deleteAndClear ();
            EnumDisplayMonitors (0, 0, MonitorEnumProc, 0);
        }

        void Window::PumpMessages () {
            if (!monitors.empty ()) {
                MSG msg;
                while (GetMessage (&msg, 0, 0, 0) == TRUE) {
                    TranslateMessage (&msg);
                    DispatchMessage (&msg);
                }
            }
        }

        namespace {
            const UINT MSG_SHOW =
                RegisterWindowMessage ("thekogans_canvas_Window_MSG_SHOW");
            const UINT MSG_QUIT =
                RegisterWindowMessage ("thekogans_canvas_Window_MSG_QUIT");

            LRESULT CALLBACK WndProc (
                    HWND wnd,
                    UINT message,
                    WPARAM wParam,
                    LPARAM lParam) {
                if (message == MSG_SHOW) {
                    Window *window =
                        (Window *)GetWindowLongPtr (wnd, GWLP_USERDATA);
                    if (window != 0) {
                        ShowWindow (window->wnd, wParam == TRUE ? SW_SHOW : SW_HIDE);
                    }
                }
                else if (message == MSG_QUIT) {
                    PostQuitMessage (0);
                }
                else {
                    switch (message) {
                        case WM_SIZE: {
                            Window *window =
                                (Window *)GetWindowLongPtr (wnd, GWLP_USERDATA);
                            if (window != 0) {
                                window->SetSize ();
                            }
                            break;
                        }
                        case WM_ERASEBKGND: {
                            return TRUE;
                        }
                        case WM_PAINT: {
                            PAINTSTRUCT ps;
                            HDC hdc = BeginPaint (wnd, &ps);
                            Window *window =
                                (Window *)GetWindowLongPtr (wnd, GWLP_USERDATA);
                            if (window != 0) {
                                window->Draw ();
                            }
                            EndPaint (wnd, &ps);
                            break;
                        }
                        case WM_SETCURSOR: {
                            SetCursor (0);
                            return TRUE;
                        }
                        default: {
                            return DefWindowProc (wnd, message, wParam, lParam);
                        }
                    }
                }
                return 0;
            }

            const char *CLASS_NAME = "thekogans_canvas_Window";

            ATOM RegisterChildClass () {
                WNDCLASSEX wcex;
                wcex.cbSize = sizeof (WNDCLASSEX);
                wcex.style = CS_HREDRAW | CS_VREDRAW;
                wcex.lpfnWndProc = WndProc;
                wcex.cbClsExtra = 0;
                wcex.cbWndExtra = 0;
                wcex.hInstance = GetModuleHandle (0);
                wcex.hIcon = 0;
                wcex.hCursor = LoadCursor (0, IDC_ARROW);
                wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
                wcex.lpszMenuName = 0;
                wcex.lpszClassName = CLASS_NAME;
                wcex.hIconSm = 0;
                return RegisterClassEx (&wcex);
            }

            const ATOM atom = RegisterChildClass ();

            HWND quitWnd = CreateWindow (CLASS_NAME, "", WS_POPUP,
                0, 0, 0, 0, 0, 0, GetModuleHandle (0), 0);
        }

        void Window::StopPumpingMessages () {
            if (!monitors.empty ()) {
                PostMessage (quitWnd, MSG_QUIT, 0, 0);
            }
        }

        void Window::Create (
                const Rectangle &rectangle,
                const Window &parent) {
            Destroy ();
            if (atom != 0) {
                wnd = CreateWindow (CLASS_NAME, "",
                    (parent.wnd != 0 ? WS_CHILD : WS_POPUP) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                    rectangle.origin.x, rectangle.origin.y,
                    rectangle.extents.width, rectangle.extents.height,
                    parent.wnd, 0, GetModuleHandle (0), 0);
                if (wnd != 0) {
                    SetWindowLongPtr (wnd, GWLP_USERDATA, (LONG_PTR)this);
                    dc = GetDC (wnd);
                    if (dc == 0) {
                        THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                            THEKOGANS_UTIL_OS_ERROR_CODE);
                    }
                }
                else {
                    THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                        THEKOGANS_UTIL_OS_ERROR_CODE);
                }
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Window class '%s' is not registered.",
                    CLASS_NAME);
            }
        }

        void Window::Destroy () {
            if (dc != 0) {
                ReleaseDC (wnd, dc);
                dc = 0;
            }
            if (wnd != 0) {
                DestroyWindow (wnd);
                wnd = 0;
            }
        }

        void Window::Show () {
            if (wnd != 0) {
                PostMessage (wnd, MSG_SHOW, TRUE, 0);
            }
        }

        void Window::Hide () {
            if (wnd != 0) {
                PostMessage (wnd, MSG_SHOW, FALSE, 0);
            }
        }

        Rectangle Window::GetRectangle () const {
            if (wnd != 0) {
                RECT rect;
                GetWindowRect (wnd, &rect);
                return Rectangle (
                    rect.left, rect.top,
                    rect.right - rect.left, rect.bottom - rect.top);
            }
            return Rectangle ();
        }

        void Window::DrawBitmap (
                const Bitmap &bitmap,
                const Rectangle &rectangle,
                const Point &origin) {
            // No checking for wnd == 0 here. Monitor will have a
            // valid dc, but no wnd.
            Rectangle::Extents extents =
                rectangle.IsDegenerate () ?
                bitmap.GetExtents () : rectangle.extents;
            BitBlt (dc, origin.x, origin.y, extents.width, extents.height,
                bitmap.memoryDC.dc, rectangle.origin.x, rectangle.origin.y, SRCCOPY);
        }
    #elif defined (TOOLCHAIN_OS_Linux)
    #if defined (THEKOGANS_CANVAS_USE_XLIB)
        namespace {
            struct Monitor : public Window {
                util::ui32 index;
                Rectangle rectangle;
                Monitor (Display *display_,
                        util::ui32 index_,
                        util::i32 x,
                        util::i32 y,
                        util::ui32 width,
                        util::ui32 height) :
                        index (index_) {
                    display = display_;
                    window = XRootWindow (display, index);
                    gc = XDefaultGC (display, index);
                    rectangle.origin.x = x;
                    rectangle.origin.y = y;
                    rectangle.extents.width = width;
                    rectangle.extents.height = height;
                }
                Monitor (
                        Display *display_,
                        util::ui32 index_) :
                        index (index_) {
                    display = display_;
                    window = XRootWindow (display, index);
                    gc = XDefaultGC (display, index);
                    XWindowAttributes attributes;
                    XGetWindowAttributes (display, window, &attributes);
                    rectangle.origin.x = attributes.x;
                    rectangle.origin.y = attributes.y;
                    rectangle.extents.width = attributes.width;
                    rectangle.extents.height = attributes.height;
                }
                // WARNING: Don't fuck with this! You might think
                // that, because this dtor, and Window dtor are
                // identical, you can remove this one. WRONG!!!
                // The standard expressly requires that ctors and
                // dtors collapse vtables, and call their own. If
                // you delete this dtor, Monitor::Destroy will
                // never be called. (and that's a very bad thing)
                virtual ~Monitor () {
                    Destroy ();
                }

                virtual void Destroy () {
                    gc = 0;
                    Window::Destroy ();
                }

                virtual Rectangle GetRectangle () const {
                    return rectangle;
                }

                typedef std::pair<util::ui32, util::ui32> DPIInfo;
                inline DPIInfo GetDPIInfo () const {
                    return DPIInfo (
                        util::ROUND (
                            DisplayWidth (display, index) * 25.4f / DisplayWidthMM (display, index)),
                        util::ROUND (
                            DisplayHeight (display, index) * 25.4f / DisplayHeightMM (display, index)));
                }
            };

            bool CheckScreen (
                    Display *display,
                    int screen) {
                int depth = XDefaultDepth (display, screen);
                if (depth != 24 && depth != 32) {
                    THEKOGANS_UTIL_LOG_WARNING (
                        "Only 24 or 32 bpp displays are supported.\n"
                        "Skipping display %d (%d)\n", screen, depth);
                    return false;
                }
                return true;
            }
        }

        void Window::EnumerateMonitors () {
            monitors.deleteAndClear ();
            for (std::list<Display *>::const_iterator
                    it = Xlib::Instance ().displays.begin (),
                    end = Xlib::Instance ().displays.end (); it != end; ++it) {
                if (XineramaIsActive (*it)) {
                    if (CheckScreen (*it, 0)) {
                        int number = 0;
                        XineramaScreenInfo *screenInfo = XineramaQueryScreens (*it, &number);
                        if (screenInfo != 0 && number > 0) {
                            for (int i = 0; i < number; ++i) {
                                monitors.push_back (
                                    new Monitor (*it, 0,
                                        screenInfo[i].x_org, screenInfo[i].y_org,
                                        screenInfo[i].width, screenInfo[i].height));
                                THEKOGANS_UTIL_LOG_INFO ("Screen %d: %d %d %d %d\n",
                                    screenInfo[i].screen_number,
                                    screenInfo[i].x_org, screenInfo[i].y_org,
                                    screenInfo[i].width, screenInfo[i].height);
                            }
                            XFree (screenInfo);
                        }
                        else {
                            THEKOGANS_UTIL_LOG_ERROR ("%s\n",
                                "XineramaQueryScreens (*it, &number); returned NULL");
                        }
                    }
                }
                else {
                    for (int i = 0, count = XScreenCount (*it); i < count; ++i) {
                        if (CheckScreen (*it, i)) {
                            monitors.push_back (new Monitor (*it, i));
                        }
                    }
                }
            }
        }

        void Window::PumpMessages () {
            Xlib::Instance ().PumpMessages ();
        }

        void Window::StopPumpingMessages () {
            Xlib::Instance ().StopPumpingMessages ();
        }

        void Window::Create (
                const Rectangle &rectangle,
                const Window &parent) {
            Destroy ();
            display = parent.display;
            window = XCreateSimpleWindow (
                parent.display, parent.window,
                rectangle.origin.x, rectangle.origin.y,
                rectangle.extents.width, rectangle.extents.height,
                0, 0, 0);
            if (window != 0) {
                gc = XCreateGC (display, window, 0, 0);
                if (gc != 0) {
                    {
                        util::LockGuard<util::SpinLock> guard (
                            Xlib::Instance ().spinLock);
                        Xlib::Instance ().windows.push_back (this);
                    }
                    // Remove mouse cursor.
                    {
                        static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
                        Pixmap noPixmap = XCreateBitmapFromData (
                            display, window, noData, 8, 8);
                        XColor black;
                        black.red = black.green = black.blue = 0;
                        Cursor noCursor = XCreatePixmapCursor (
                            display, noPixmap, noPixmap,
                            &black, &black, 0, 0);
                        XDefineCursor (display, window, noCursor);
                        XFreeCursor (display, noCursor);
                        XFreePixmap (display, noPixmap);
                    }
                    if (dynamic_cast<const Monitor *> (&parent) != 0) {
                        // Remove all decorations (borders, title bar...)
                        {
                            Atom property = XInternAtom (
                                display, "_MOTIF_WM_HINTS", True);
                            struct Hints {
                                unsigned long flags;
                                unsigned long functions;
                                unsigned long decorations;
                                long inputMode;
                                unsigned long status;
                            } hints = {0};
                            // Specify that we're changing the window
                            // decorations.
                            hints.flags = 2;
                            // 0 (false) means that window decorations
                            // should go bye-bye.
                            hints.decorations = 0;
                            XChangeProperty (display, window, property,
                                property, 32, PropModeReplace,
                                (const unsigned char *)&hints, 5);
                        }
                        // Make full screen.
                        {
                            Atom property = XInternAtom (
                                display, "_NET_WM_STATE", False);
                            Atom atoms[2] = {
                                XInternAtom (display, "_NET_WM_STATE_FULLSCREEN", False),
                                None
                            };
                            XChangeProperty (display, window, property,
                                XA_ATOM, 32, PropModeReplace,
                                (const unsigned char *)atoms, 1);
                        }
                    }
                    XSelectInput (display, window,
                        ExposureMask | StructureNotifyMask);
                }
                else {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "Unable to create a gc: (%d, %d) (%u, %u)",
                        rectangle.origin.x, rectangle.origin.y,
                        rectangle.extents.width, rectangle.extents.height);
                }
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to create a window: (%d, %d) (%u, %u)",
                    rectangle.origin.x, rectangle.origin.y,
                    rectangle.extents.width, rectangle.extents.height);
            }
        }

        void Window::Destroy () {
            {
                util::LockGuard<util::SpinLock> guard (
                    Xlib::Instance ().spinLock);
                for (std::list<Window *>::iterator it = Xlib::Instance ().windows.begin (),
                         end = Xlib::Instance ().windows.end (); it != end; ++it) {
                    if (*it == this) {
                        Xlib::Instance ().windows.erase (it);
                        break;
                    }
                }
            }
            if (gc != 0) {
                XFreeGC (display, gc);
                gc = 0;
            }
            if (window != 0) {
                XDestroyWindow (display, window);
                window = 0;
            }
        }

        void Window::Show () {
            if (window != 0) {
                XMapWindow (display, window);
            }
        }

        void Window::Hide () {
            if (window != 0) {
                XUnmapWindow (display, window);
            }
        }

        Rectangle Window::GetRectangle () const {
            if (window != 0) {
                XWindowAttributes attributes;
                XGetWindowAttributes (display, window, &attributes);
                return Rectangle (
                    attributes.x, attributes.y,
                    attributes.width, attributes.height);
            }
            return Rectangle ();
        }

        void Window::DrawBitmap (
                const Bitmap &bitmap,
                const Rectangle &rectangle,
                const Point &origin) {
            if (window != 0) {
                Rectangle::Extents extents = rectangle.IsDegenerate () ?
                    bitmap.GetExtents () : rectangle.extents;
                XPutImage (display, window, gc, bitmap.image,
                    rectangle.origin.x, rectangle.origin.y, origin.x, origin.y,
                    extents.width, extents.height);
            }
        }
    #else // defined (THEKOGANS_CANVAS_USE_XLIB)
        namespace {
            struct Console {
                std::string tty;
                Console () : tty (ttyname (STDIN_FILENO)) {
                    THEKOGANS_UTIL_HANDLE handle = open (tty.c_str (), O_RDWR);
                    if (handle != THEKOGANS_UTIL_INVALID_HANDLE_VALUE) {
                        ioctl (handle, KDSETMODE, KD_GRAPHICS);
                        close (handle);
                    }
                }
                ~Console () {
                    THEKOGANS_UTIL_HANDLE handle = open (tty.c_str (), O_RDWR);
                    if (handle != THEKOGANS_UTIL_INVALID_HANDLE_VALUE) {
                        ioctl (handle, KDSETMODE, KD_TEXT);
                        close (handle);
                    }
                }
            } console;

            struct Monitor : public Window {
                util::ui32 index;
                THEKOGANS_UTIL_HANDLE handle;
                explicit Monitor (util::ui32 index_) :
                    index (index_),
                    handle (
                        open (
                            util::FormatString (
                                "/dev/fb%u", index).c_str (), O_RDWR)) {
                    if (handle == THEKOGANS_UTIL_INVALID_HANDLE_VALUE) {
                        THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                            THEKOGANS_UTIL_OS_ERROR_CODE);
                    }
                    if (ioctl (handle, FBIOBLANK, VESA_NO_BLANKING) < 0) {
                        //close (handle);
                        //THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                        //    THEKOGANS_UTIL_OS_ERROR_CODE);
                    }
                    fb_fix_screeninfo fixInfo;
                    if (ioctl (handle, FBIOGET_FSCREENINFO, &fixInfo) < 0) {
                        close (handle);
                        THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                            THEKOGANS_UTIL_OS_ERROR_CODE);
                    }
                    fb_var_screeninfo varInfo;
                    if (ioctl (handle, FBIOGET_VSCREENINFO, &varInfo) < 0) {
                        close (handle);
                        THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                            THEKOGANS_UTIL_OS_ERROR_CODE);
                    }
                    if (varInfo.bits_per_pixel != 24 && varInfo.bits_per_pixel != 32) {
                        close (handle);
                        THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                            "Only 24 or 32 bpp displays are supported.\n"
                            "Skipping display %d (%d)\n",
                            index, varInfo.bits_per_pixel);
                    }
                    rectangle.origin.x = 0;
                    rectangle.origin.y = 0;
                    rectangle.extents.width = varInfo.xres;
                    rectangle.extents.height = varInfo.yres;
                    visible = true;
                    RGBImage (
                        (util::ui8 *)mmap (0, fixInfo.smem_len,
                            PROT_READ | PROT_WRITE, MAP_SHARED, handle, 0),
                        Rectangle::Extents (varInfo.xres, varInfo.yres),
                        RGBImage::R2G1B0A3,
                        varInfo.bits_per_pixel / 8,
                        fixInfo.line_length, false).Swap (image);
                    if (image.GetData () == 0) {
                        close (handle);
                        THEKOGANS_UTIL_THROW_ERROR_CODE_EXCEPTION (
                            THEKOGANS_UTIL_OS_ERROR_CODE);
                    }
                }
                virtual ~Monitor () {
                    munmap (image.GetData (), 0);
                    close (handle);
                }
            };
        }

        void Window::EnumerateMonitors () {
            monitors.deleteAndClear ();
            for (util::ui32 index = 0;
                 util::Path (
                     util::FormatString ("/dev/fb%u", index)).Exists ();
                 ++index) {
                THEKOGANS_UTIL_TRY {
                    std::auto_ptr<Monitor> monitor (new Monitor (index));
                    monitors.push_back (monitor.get ());
                    monitor.release ();
                }
                THEKOGANS_UTIL_CATCH_AND_LOG
            }
        }

        namespace {
            util::Mutex mutex;
            util::Condition condition (mutex);
        }

        void Window::PumpMessages () {
            util::LockGuard<util::Mutex> guard (mutex);
            condition.Wait ();
        }

        void Window::StopPumpingMessages () {
            util::LockGuard<util::Mutex> guard (mutex);
            condition.Signal ();
        }

        void Window::Create (
                const Rectangle &rectangle_,
                const Window &parent_) {
            Destroy ();
            rectangle = rectangle_;
            parent = &parent_;
            visible = false;
            RGBImage (
                parent->image.GetData () +
                    rectangle.origin.y * parent->image.GetRowStride () +
                    rectangle.origin.x * parent->image.GetPixelStride (),
                rectangle.extents,
                parent->image.GetComponentIndices (),
                parent->image.GetPixelStride (),
                parent->image.GetRowStride (), false).Swap (image);
        }

        void Window::Destroy () {
        }

        void Window::Show () {
            visible = true;
        }

        void Window::Hide () {
            visible = false;
        }

        Rectangle Window::GetRectangle () const {
            return rectangle;
        }

        void Window::DrawBitmap (
                const Bitmap &bitmap,
                const Rectangle &rectangle,
                const Point &origin) {
            bitmap.Copy (rectangle, origin, image);
        }
    #endif // defined (THEKOGANS_CANVAS_USE_XLIB)
    #elif defined (TOOLCHAIN_OS_OSX)
        namespace {
            struct Monitor : public Window {
                util::ui32 index;
                CGDirectDisplayID displayID;
                Rectangle rectangle;
                Monitor (
                        util::ui32 index_,
                        CGDirectDisplayID displayID_) :
                        index (index_),
                        displayID (displayID_) {
                    CGRect rect = CGDisplayBounds (displayID);
                    rectangle.origin.x = rect.origin.x;
                    rectangle.origin.y = rect.origin.y;
                    rectangle.extents.width = rect.size.width;
                    rectangle.extents.height = rect.size.height;
                }
                // WARNING: Don't fuck with this! You might think
                // that, because this dtor, and Window dtor are
                // identical, you can remove this one. WRONG!!!
                // The standard expressly requires that ctors and
                // dtors collapse vtables, and call their own. If
                // you delete this dtor, Monitor::Destroy will
                // never be called. (and that's a very bad thing)
                virtual ~Monitor () {
                    Destroy ();
                }

                virtual void Destroy () {
                }

                virtual Rectangle GetRectangle () const {
                    return rectangle;
                }

                virtual CGImageRef GetImage () const {
                    return CGDisplayCreateImage (displayID);
                }

                typedef std::pair<util::ui32, util::ui32> DPIInfo;
                inline DPIInfo GetDPIInfo () const {
                    CGSize sizeInMillimeters = CGDisplayScreenSize (displayID);
                    return DPIInfo (
                        util::ROUND (
                            CGDisplayPixelsWide (displayID) * 25.4f / sizeInMillimeters.width),
                        util::ROUND (
                            CGDisplayPixelsHigh (displayID) * 25.4f / sizeInMillimeters.height));
                }
            };
        }

        namespace {
            struct CGDisplayModeRefDeleter {
                void operator () (CGDisplayModeRef displayMode) {
                    if (displayMode != 0) {
                        CGDisplayModeRelease (displayMode);
                    }
                }
            };
            typedef std::unique_ptr<CGDisplayMode, CGDisplayModeRefDeleter> CGDisplayModeRefPtr;

            struct CFStringRefDeleter {
                void operator () (CFStringRef stringRef) {
                    if (stringRef != 0) {
                        CFRelease (stringRef);
                    }
                }
            };
            typedef std::unique_ptr<const __CFString, CFStringRefDeleter> CFStringRefPtr;

            size_t CGDisplayBitsPerPixel (CGDirectDisplayID display) {
                CGDisplayModeRefPtr originalMode (CGDisplayCopyDisplayMode (display));
                CFStringRefPtr pixelEncoding (CGDisplayModeCopyPixelEncoding (originalMode.get ()));
                if (CFStringCompare (pixelEncoding.get (), CFSTR (IO32BitDirectPixels), 0) == 0) {
                    return 32;
                }
                else if (CFStringCompare (pixelEncoding.get (), CFSTR (IO16BitDirectPixels), 0) == 0) {
                    return 16;
                }
                return 8;
            }
        }

        void Window::EnumerateMonitors () {
            monitors.deleteAndClear ();
            CGDisplayCount displayCount = 0;
            CGGetActiveDisplayList (0, 0, &displayCount);
            if (displayCount != 0) {
                std::vector<CGDirectDisplayID> activeDisplays (displayCount);
                CGGetActiveDisplayList (displayCount,
                    &activeDisplays[0], &displayCount);
                for (CGDisplayCount i = 0; i < displayCount; ++i) {
                    size_t depth = CGDisplayBitsPerPixel (activeDisplays[i]);
                    if (depth == 24 || depth == 32) {
                        monitors.push_back (new Monitor (i, activeDisplays[i]));
                    }
                    else {
                        THEKOGANS_UTIL_LOG_WARNING (
                            "Only 24 or 32 bpp displays are supported.\n"
                            "Skipping display %d (%d)\n", i, depth);
                    }
                }
            }
        }

        namespace {
            util::Mutex mutex;
            util::Condition condition (mutex);
        }

        void Window::PumpMessages () {
            // FIXME: implement with cocoa
            util::LockGuard<util::Mutex> guard (mutex);
            condition.Wait ();
        }

        void Window::StopPumpingMessages () {
            // FIXME: implement with cocoa
            util::LockGuard<util::Mutex> guard (mutex);
            condition.Signal ();
        }

        void Window::Create (
                const Rectangle &rectangle,
                const Window &parent) {
            // FIXME: implement with cocoa
        }

        void Window::Destroy () {
            // FIXME: implement with cocoa
        }

        void Window::Show () {
            // FIXME: implement with cocoa
        }

        void Window::Hide () {
            // FIXME: implement with cocoa
        }

        Rectangle Window::GetRectangle () const {
            return Rectangle ();
        }

        CGImageRef Window::GetImage () const {
            CGImageRef image = 0;
            CFArrayRef array = CFArrayCreate (
                kCFAllocatorDefault, (const void **)&windowID, 1, 0);
            if (array != 0) {
                image = CGWindowListCreateImageFromArray (CGRectNull,
                    array, kCGWindowImageDefault);
                CFRelease (array);
            }
            return image;
        }

        void Window::DrawBitmap (
                const Bitmap &bitmap,
                const Rectangle &rectangle,
                const Point &origin) {
            Rectangle bitmapRectangle = rectangle.Intersection (bitmap.GetRectangle ());
            if (!bitmapRectangle.IsDegenerate ()) {
                struct ColorSpace {
                    CGColorSpaceRef colorSpace;
                    ColorSpace () : colorSpace (CGColorSpaceCreateDeviceRGB ()) {
                        if (colorSpace == 0) {
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                                "%s", "Unable to create CGColorSpace");
                        }
                    }
                    ~ColorSpace () {
                        CFRelease (colorSpace);
                    }
                    CGColorSpaceRef operator * () const {
                        return colorSpace;
                    }
                } colorSpace;
                struct BitmapContext {
                    CGContextRef context;
                    BitmapContext (
                        const Bitmap &bitmap,
                        const Rectangle &rectangle,
                        const ColorSpace &colorSpace) :
                        context (CGBitmapContextCreate (
                                bitmap.GetData () +
                                    rectangle.origin.y * bitmap.GetRowStride () +
                                    rectangle.origin.x * bitmap.GetPixelStride (),
                                rectangle.extents.width,
                                rectangle.extents.height,
                                util::UI8_SIZE, // bitsPerComponent
                                bitmap.GetRowStride (), // bytesPerRow
                                *colorSpace,
                                kCGImageAlphaNoneSkipLast)) {
                        if (context == 0) {
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                                "%s", "Unable to create CGContext");
                        }
                    }
                    ~BitmapContext () {
                        CFRelease (context);
                    }
                    CGContextRef operator * () const {
                        return context;
                    }
                } bitmapContext (bitmap, bitmapRectangle, colorSpace);
                struct Image {
                    CGImageRef image;
                    Image (const BitmapContext &bitmapContext) :
                        image (CGBitmapContextCreateImage (*bitmapContext)) {
                        if (image == 0) {
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                                "%s", "Unable to create CGImage");
                        }
                    }
                    ~Image () {
                        CFRelease (image);
                    }
                    void Draw (
                            CGContextRef context,
                            const Point &origin) {
                        CGContextDrawImage (
                            context,
                            CGRectMake (
                                origin.x,
                                origin.y,
                                CGImageGetWidth (image),
                                CGImageGetHeight (image)),
                            image);
                    }
                } image (bitmapContext);
                image.Draw (context, origin);
            }
        }
    #endif // defined (TOOLCHAIN_OS_Windows)

    } // namespace canvas
} // namespace thekogans

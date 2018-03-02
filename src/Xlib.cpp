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

#if defined (THEKOGANS_OS_Linux)
#if defined (THEKOGANS_CANVAS_USE_XLIB)

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/extensions/xf86vmode.h>
#include "thekogans/util/Types.h"
#include "thekogans/util/LockGuard.h"
#include "thekogans/util/Directory.h"
#include "thekogans/util/LoggerMgr.h"
#include "thekogans/stream/Socket.h"
#include "thekogans/util/RGBImage.h"
#include "thekogans/canvas/Window.h"
#include "thekogans/canvas/Bitmap.h"
#include "thekogans/canvas/Xlib.h"

namespace thekogans {
    namespace canvas {

        namespace {
            int ErrorHandler (
                    Display *display,
                    XErrorEvent *errorEvent) {
                char buffer[1024];
                XGetErrorText (display, errorEvent->error_code, buffer, 1024);
                THEKOGANS_UTIL_LOG_ERROR ("%s\n", buffer);
                return 0;
            }

            int IOErrorHandler (Display *display) {
                THEKOGANS_UTIL_LOG_ERROR ("%s\n", "Fatal IO error.");
                return 0;
            }
        }

        Xlib::Xlib () :
                done (true) {
            XInitThreads ();
            XSetErrorHandler (ErrorHandler);
            XSetIOErrorHandler (IOErrorHandler);
            util::Directory directory ("/tmp/.X11-unix");
            util::Directory::Entry entry;
            for (bool gotEntry = directory.GetFirstEntry (entry);
                    gotEntry; gotEntry = directory.GetNextEntry (entry)) {
                if (!entry.name.empty () && entry.name[0] == 'X') {
                    entry.name[0] = ':';
                    Display *display = XOpenDisplay (entry.name.c_str ());
                    if (display != 0) {
                        XF86VidModeSetViewPort (display, 0, 0, 0);
                        displays.push_back (display);
                    }
                    else {
                        THEKOGANS_UTIL_LOG_WARNING (
                            "Unable to open display: %s\n",
                            entry.name.c_str ());
                    }
                }
            }
        }

        namespace {
            struct DisplaySocket {
                Display *display;
                stream::Socket socket;
                explicit DisplaySocket (Display *display_) :
                    display (display_),
                    socket (ConnectionNumber (display)) {}
            };
        }

        void Xlib::PumpMessages () {
            if (!displays.empty () && done) {
                done = false;
                util::OwnerList<DisplaySocket> displaySockets;
                for (std::list<Display *>::const_iterator it = displays.begin (),
                         end = displays.end (); it != end; ++it) {
                    displaySockets.push_back (new DisplaySocket (*it));
                }
                while (!done) {
                    streamSelector.Clear ();
                    for (util::OwnerList<DisplaySocket>::const_iterator
                            it = displaySockets.begin (),
                            end = displaySockets.end (); it != end; ++it) {
                        streamSelector.AddStreamForReading ((*it)->socket);
                    }
                    if (streamSelector.Select ()) {
                        for (util::OwnerList<DisplaySocket>::const_iterator
                                it = displaySockets.begin (),
                                end = displaySockets.end (); it != end; ++it) {
                            if (streamSelector.IsStreamReadyForReading ((*it)->socket)) {
                                while (XPending ((*it)->display) > 0) {
                                    XEvent event;
                                    XNextEvent ((*it)->display, &event);
                                    if (event.type == Expose && event.xexpose.count == 0) {
                                        util::LockGuard<util::SpinLock> guard (spinLock);
                                        for (std::list<Window *>::const_iterator
                                                it = windows.begin (),
                                                end = windows.end (); it != end; ++it) {
                                            if ((*it)->window == event.xany.window) {
                                                (*it)->Draw ();
                                                break;
                                            }
                                        }
                                    }
                                    else if (event.type == ConfigureNotify) {
                                        util::LockGuard<util::SpinLock> guard (spinLock);
                                        for (std::list<Window *>::const_iterator
                                                it = windows.begin (),
                                                end = windows.end (); it != end; ++it) {
                                            if ((*it)->window == event.xany.window) {
                                                (*it)->SetSize ();
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void Xlib::StopPumpingMessages () {
            if (!displays.empty () && !done) {
                done = true;
                streamSelector.Break ();
            }
        }

    } // namespace core
} // namespace thekogans

#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#endif // defined (THEKOGANS_OS_Linux)

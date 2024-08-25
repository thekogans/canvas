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

#if !defined (__thekogans_canvas_Xlib_h)
#define __thekogans_canvas_Xlib_h

#include "thekogans/util/Environment.h"

#if defined (TOOLCHAIN_OS_Linux)
#if defined (THEKOGANS_CANVAS_USE_XLIB)

#include <memory>
#include <list>
#include <X11/Xlib.h>
#include "thekogans/util/Types.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/util/Singleton.h"
#include "thekogans/stream/StreamSelector.h"
#include "thekogans/CloudDisplay/canvas/Config.h"

namespace thekogans {
    namespace canvas {

        struct Window;

        struct Xlib : util::Singleton<Xlib> {
            volatile bool done;
            std::list<Display *> displays;
            std::list<Window *> windows;
            util::SpinLock spinLock;
            stream::StreamSelector streamSelector;

            Xlib ();

            void PumpMessages ();
            void StopPumpingMessages ();

            THEKOGANS_UTIL_DISALLOW_COPY_AND_ASSIGN (Xlib)
        };

    } // namespace canvas
} // namespace thekogans

#endif // defined (THEKOGANS_CANVAS_USE_XLIB)
#endif // defined (TOOLCHAIN_OS_Linux)

#endif // !defined (__thekogans_canvas_Xlib_h)

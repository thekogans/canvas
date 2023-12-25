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

#if !defined (__thekogans_canvas_Canvas_h)
#define __thekogans_canvas_Canvas_h

#include <memory>
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/TimeSpec.h"
#include "thekogans/util/Thread.h"
#include "thekogans/util/SpinRWLock.h"
#include "thekogans/util/LoggerMgr.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/canvas/RGBImage.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Bitmap.h"
#include "thekogans/canvas/Window.h"

namespace thekogans {
    namespace canvas {

        struct _LIB_THEKOGANS_CANVAS_DECL Canvas :
                public util::Thread,
                public Window {
        protected:
            Bitmap bitmap;
            util::Rectangle dirtyRectangle;
            util::TimeSpec updatePeriod;
            util::i32 priority;
            volatile bool done;
            util::SpinRWLock lock;

        public:
            explicit Canvas (util::ui32 monitorIndex) :
                Window (
                    Window::monitors[monitorIndex]->GetRectangle (),
                    *Window::monitors[monitorIndex]),
                    bitmap (*this),
                    done (true) {}
            explicit Canvas (Window &parent) :
                Window (parent.GetRectangle (), parent),
                bitmap (*this),
                done (true) {}
            virtual ~Canvas () {}

            struct _LIB_THEKOGANS_CANVAS_DECL LockGuard {
                Canvas &canvas;
                util::Rectangle rectangle;
                RGBImage bitmap;
                LockGuard (
                    Canvas &canvas_,
                    const util::Rectangle &rectangle_) :
                    canvas (canvas_),
                    rectangle (rectangle_),
                    bitmap (canvas.Lock (rectangle)) {}
                ~LockGuard () {
                    canvas.Unlock (rectangle);
                }
            };

            void Start (
                const util::TimeSpec &updatePeriod_ =
                    util::TimeSpec::FromMilliseconds (33),
                util::i32 priority_ =
                    THEKOGANS_UTIL_NORMAL_THREAD_PRIORITY);
            void Stop ();

            RGBImage GetSubImage (const util::Rectangle &rectangle);

            void Invalidate (const util::Rectangle &rectangle);

            RGBImage Lock (const util::Rectangle &rectangle);
            void Unlock (const util::Rectangle &rectangle);

            inline util::ui8 *GetData () const {
                return bitmap.GetData ();
            }
            inline util::Rectangle::Extents GetExtents () const {
                return bitmap.GetExtents ();
            }
            inline util::ui32 GetComponentIndices () const {
                return bitmap.GetComponentIndices ();
            }
            inline util::ui32 GetPixelStride () const {
                return bitmap.GetPixelStride ();
            }
            inline util::ui32 GetRowStride () const {
                return bitmap.GetRowStride ();
            }

        protected:
            // util::Thread
            virtual void Run () throw ();

            // Window
            virtual void SetSize ();
            virtual void Draw ();
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Canvas_h)

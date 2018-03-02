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

#include "thekogans/util/RWLockGuard.h"
#include "thekogans/util/LoggerMgr.h"
#include "thekogans/canvas/Canvas.h"

namespace thekogans {
    namespace canvas {

        void Canvas::Start (
                const util::TimeSpec &updatePeriod_,
                util::i32 priority_) {
            if (done) {
                updatePeriod = updatePeriod_;
                priority = priority_;
                done = false;
                util::Thread::Create (priority);
            }
        }

        void Canvas::Stop () {
            if (!done) {
                done = true;
                Wait ();
            }
        }

        RGBImage Canvas::GetSubImage (const Rectangle &rectangle) {
            Rectangle rectangle_ =
                bitmap.GetRectangle ().Intersection (rectangle);
            return RGBImage (
                bitmap.GetData () +
                    rectangle_.origin.y * bitmap.GetRowStride () +
                    rectangle_.origin.x * bitmap.GetPixelStride (),
                rectangle_.extents,
                bitmap.GetComponentIndices (),
                bitmap.GetPixelStride (),
                bitmap.GetRowStride (), false);
        }

        void Canvas::Invalidate (const Rectangle &rectangle) {
            if (!rectangle.IsDegenerate ()) {
                util::RWLockGuard<util::SpinRWLock> guard (lock, false);
                dirtyRectangle = dirtyRectangle.Union (rectangle);
            }
        }

        RGBImage Canvas::Lock (const Rectangle &rectangle) {
            lock.Acquire (true);
            return GetSubImage (rectangle);
        }

        void Canvas::Unlock (const Rectangle &rectangle) {
            if (!rectangle.IsDegenerate ()) {
                dirtyRectangle = dirtyRectangle.Union (rectangle);
            }
            lock.Release (true);
        }

        void Canvas::Run () throw () {
            while (!done) {
                {
                    util::RWLockGuard<util::SpinRWLock> guard (lock, false);
                    if (!dirtyRectangle.IsDegenerate ()) {
                        DrawBitmap (bitmap, dirtyRectangle, dirtyRectangle.origin);
                        dirtyRectangle = Rectangle ();
                    }
                }
                util::Sleep (updatePeriod);
            }
        }

        void Canvas::SetSize () {
            util::RWLockGuard<util::SpinRWLock> guard (lock, false);
            bitmap.Create (*this);
            bitmap.Clear (bitmap.GetRectangle ());
            dirtyRectangle = bitmap.GetRectangle ();
        }

        void Canvas::Draw () {
            util::RWLockGuard<util::SpinRWLock> guard (lock, false);
            dirtyRectangle = bitmap.GetRectangle ();
        }

    } // namespace canvas
} // namespace thekogans

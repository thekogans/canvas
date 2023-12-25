// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_util.
//
// libthekogans_util is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_util is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_util. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_canvas_os_osx_OSXUtils_h)
#define __thekogans_canvas_os_osx_OSXUtils_h

#include "thekogans/util/Environment.h"

#if defined (TOOLCHAIN_OS_OSX)

#include <string>
#include <vector>
#include <CoreGraphics/CoreGraphics.h>

namespace thekogans {
    namespace canvas {
        namespace os {
            namespace osx {

                struct Screen {
                    /// \brief
                    /// Display number to be used with CGDisplay* routines.
                    CGDirectDisplayID display;
                    /// \brief
                    ///
                    int depth;
                    std::string colorSpaceName;
                    /// \brief
                    /// Screen frame relative to global coordinate system.
                    /// In a multi-screen environment frame can be used to
                    /// figure out the position of this screen.
                    CGRect frame;
                    CGFloat backingScaleFactor;
                    std::size_t bitsPerSample;
                    /// \brief
                    /// Screen size in dpi.
                    CGSize resolution;
                    /// \brief
                    /// Screen size in pixels.
                    CGSize size;

                    Screen () {
                        display = 0;
                        depth = 0;
                        frame.origin.x = 0.0f;
                        frame.origin.y = 0.0f;
                        frame.size.width = 0.0f;
                        frame.size.height = 0.0f;
                        backingScaleFactor = 0.0f;
                        bitsPerSample = 0;
                        resolution.width = 0.0f;
                        resolution.height = 0.0f;
                        size.width = 0.0f;
                        size.height = 0.0f;
                    }
                    Screen (
                        CGDirectDisplayID display_,
                        int depth_,
                        const std::string &colorSpaceName_,
                        const CGRect &frame_,
                        CGFloat backingScaleFactor_,
                        std::size_t bitsPerSample_,
                        const CGSize &resolution_,
                        const CGSize &size_) :
                        display (display_),
                        depth (depth_),
                        colorSpaceName (colorSpaceName_),
                        frame (frame_),
                        backingScaleFactor (backingScaleFactor_),
                        bitsPerSample (bitsPerSample_),
                        resolution (resolution_),
                        size (size_) {}
                };

                /// \brief
                /// Return the scale factor used for displays.
                /// \return Scale factor used for displays.
                std::vector<Screen> GetScreens ();

            } // namespace osx
        } // namespace os
    } // namespace util
} // namespace thekogans

#endif // defined (TOOLCHAIN_OS_OSX)

#endif // !defined (__thekogans_canvas_os_osx_OSXUtils_h)

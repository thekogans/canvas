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

#include <AppKit/AppKit.h>
#include "thekogans/canvas/os/osx/OSXUtils.h"

namespace thekogans {
    namespace canvas {
        namespace os {
            namespace osx {

                std::vector<Screen> GetScreens () {
                    std::vector<Screen> screens;
                    NSArray<NSScreen *> *nsScreens = [NSScreen screens];
                    NSUInteger count = [nsScreens count];
                    for (NSUInteger i = 0; i < count; ++i) {
                        NSDictionary<NSDeviceDescriptionKey, id> *deviceDescription = [nsScreens[i] deviceDescription];
                        screens.push_back (
                            Screen (
                                [deviceDescription[@"NSScreenNumber"] unsignedIntValue],
                                NSBitsPerPixelFromDepth (nsScreens[i].depth),
                                [NSColorSpaceFromDepth (nsScreens[i].depth) UTF8String],
                                [nsScreens[i] frame],
                                [nsScreens[i] backingScaleFactor],
                                [deviceDescription[NSDeviceBitsPerSample] unsignedIntValue],
                                [deviceDescription[NSDeviceResolution] sizeValue],
                                [deviceDescription[NSDeviceSize] sizeValue]));
                    }
                    return screens;
                }

            } // namespace osx
        } // namespace os
    } // namespace util
} // namespace thekogans

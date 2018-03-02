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

#if !defined (__thekogans_canvas_Point_h)
#define __thekogans_canvas_Point_h

#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/canvas/Config.h"

namespace thekogans {
    namespace canvas {

        struct Rectangle;

        struct _LIB_THEKOGANS_CANVAS_DECL Point {
            util::i32 x;
            util::i32 y;

            Point (
                util::i32 x_ = 0,
                util::i32 y_ = 0) :
                x (x_), y (y_) {}

            enum {
                SIZE = util::I32_SIZE + util::I32_SIZE
            };

            inline Point &operator += (const Point &pt) {
                x += pt.x;
                y += pt.y;
                return *this;
            }

            inline Point &operator -= (const Point &pt) {
                x -= pt.x;
                y -= pt.y;
                return *this;
            }

            inline Point &operator *= (util::f32 scale) {
                x = (util::i32)util::ROUND ((util::f32)x * scale);
                y = (util::i32)util::ROUND ((util::f32)y * scale);
                return *this;
            }

            bool InRectangle (const Rectangle &rectangle) const;
        };

        inline bool operator == (
                const Point &p1,
                const Point &p2) {
            return p1.x == p2.x && p1.y == p2.y;
        }

        inline bool operator != (
                const Point &p1,
                const Point &p2) {
            return p1.x != p2.x || p1.y != p2.y;
        }

        inline Point operator + (
                const Point &p1,
                const Point &p2) {
            return Point (p1.x + p2.x, p1.y + p2.y);
        }

        inline Point operator - (
                const Point &p1,
                const Point &p2) {
            return Point (p1.x - p2.x, p1.y - p2.y);
        }

        inline Point operator * (
                const Point &pt,
                util::f32 scale) {
            return Point (
                (util::i32)util::ROUND ((util::f32)pt.x * scale),
                (util::i32)util::ROUND ((util::f32)pt.y * scale));
        }

        inline util::Serializer &operator << (
                util::Serializer &serializer,
                const Point &point) {
            serializer << point.x << point.y;
            return serializer;
        }
        inline util::Serializer &operator >> (
                util::Serializer &serializer,
                Point &point) {
            serializer >> point.x >> point.y;
            return serializer;
        }

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Point_h)

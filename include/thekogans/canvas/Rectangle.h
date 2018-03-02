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

#if !defined (__thekogans_canvas_Rectangle_h)
#define __thekogans_canvas_Rectangle_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Point.h"

namespace thekogans {
    namespace canvas {

        struct _LIB_THEKOGANS_CANVAS_DECL Rectangle {
            Point origin;
            struct _LIB_THEKOGANS_CANVAS_DECL Extents {
                util::ui32 width;
                util::ui32 height;

                Extents (
                    util::ui32 width_ = 0,
                    util::ui32 height_ = 0) :
                    width (width_),
                    height (height_) {}

                enum {
                    SIZE = util::UI32_SIZE + util::UI32_SIZE
                };

                // Scale self to aspect fit in to given extents.
                // Return the scale factor used to acomplish the feat.
                util::f32 AspectFit (const Extents &extents);

                inline bool IsDegenerate () const {
                    return width == 0 || height == 0;
                }

                inline util::ui32 GetArea () const {
                    return width * height;
                }
            } extents;

            Rectangle (
                util::i32 x = 0,
                util::i32 y = 0,
                util::ui32 width = 0,
                util::ui32 height = 0) :
                origin (x, y),
                extents (width, height) {}
            Rectangle (
                const Point &origin_,
                const Extents &extents_) :
                origin (origin_),
                extents (extents_) {}

            enum {
                SIZE = Point::SIZE + Extents::SIZE
            };

            inline bool IsDegenerate () const {
                return extents.IsDegenerate ();
            }

            inline bool IsInside (const Rectangle &rectangle) const {
                return
                    origin.x >= rectangle.origin.x &&
                    origin.x + extents.width <=
                        rectangle.origin.x + rectangle.extents.width &&
                    origin.y >= rectangle.origin.y &&
                    origin.y + extents.height <=
                        rectangle.origin.y + rectangle.extents.height;
            }

            enum {
                SplitVertical,
                SplitHorizontal
            };
            void Split (
                util::ui32 splitDirection,
                Rectangle &half0,
                Rectangle &half1,
                util::f32 t = 0.5f) const;

            Rectangle Intersection (const Rectangle &rectangle) const;
            Rectangle Union (const Rectangle &rectangle) const;

            bool CanMergeWith (const Rectangle &rectangle) const;
            void MergeWith (const Rectangle &rectangle);

            inline util::ui32 GetArea () const {
                return extents.GetArea ();
            }

            // Return a rectangle (and optionaly the scale) which will
            // aspect fit, and letterbox the frame in the window.
            static Rectangle AspectFitAndLetterbox (
                const Extents &frame,
                const Extents &window,
                util::f32 *scale = 0);
        };

        inline bool operator == (
                const Rectangle::Extents &extents1,
                const Rectangle::Extents &extents2) {
            return extents1.width == extents2.width &&
                extents1.height == extents2.height;
        }
        inline bool operator != (
                const Rectangle::Extents &extents1,
                const Rectangle::Extents &extents2) {
            return extents1.width != extents2.width ||
                extents1.height != extents2.height;
        }
        inline Rectangle::Extents operator * (
                const Rectangle::Extents &extents,
                util::f32 scale) {
            return Rectangle::Extents (
                (util::ui32)util::ROUND (((util::f32)extents.width * scale)),
                (util::ui32)util::ROUND (((util::f32)extents.height * scale)));
        }

        inline bool operator == (
                const Rectangle &rectangle1,
                const Rectangle &rectangle2) {
            return rectangle1.origin == rectangle2.origin &&
                rectangle1.extents == rectangle2.extents;
        }

        inline bool operator != (
                const Rectangle &rectangle1,
                const Rectangle &rectangle2) {
            return rectangle1.origin != rectangle2.origin ||
                rectangle1.extents != rectangle2.extents;
        }

        inline Rectangle operator * (
                const Rectangle &rectangle,
                util::f32 scale) {
            return Rectangle (rectangle.origin * scale, rectangle.extents * scale);
        }

        inline Rectangle operator + (
                const Rectangle &rectangle,
                const Point &offset) {
            return Rectangle (rectangle.origin + offset, rectangle.extents);
        }

        inline Rectangle operator - (
                const Rectangle &rectangle,
                const Point &offset) {
            return Rectangle (rectangle.origin - offset, rectangle.extents);
        }

        inline Rectangle::Extents operator + (
                const Rectangle::Extents &extents,
                const Point &offset) {
            return Rectangle::Extents (extents.width + offset.x,
                extents.height + offset.y);
        }

        inline Rectangle::Extents operator - (
                const Rectangle::Extents &extents,
                const Point &offset) {
            return Rectangle::Extents (extents.width - offset.x,
                extents.height - offset.y);
        }

        inline util::Serializer &operator << (
                util::Serializer &serializer,
                const Rectangle::Extents &extents) {
            serializer << extents.width << extents.height;
            return serializer;
        }
        inline util::Serializer &operator >> (
                util::Serializer &serializer,
                Rectangle::Extents &extents) {
            serializer >> extents.width >> extents.height;
            return serializer;
        }

        inline util::Serializer &operator << (
                util::Serializer &serializer,
                const Rectangle &rectangle) {
            serializer << rectangle.origin << rectangle.extents;
            return serializer;
        }
        inline util::Serializer &operator >> (
                util::Serializer &serializer,
                Rectangle &rectangle) {
            serializer >> rectangle.origin >> rectangle.extents;
            return serializer;
        }

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Rectangle_h)

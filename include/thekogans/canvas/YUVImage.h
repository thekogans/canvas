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

#if !defined (__thekogans_canvas_YUVImage_h)
#define __thekogans_canvas_YUVImage_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"

namespace thekogans {
    namespace canvas {

        struct RGBImage;

        // NOTE: The one and only format supported by YUVImage is I420!

        struct _LIB_THEKOGANS_CANVAS_DECL YUVImage {
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (YUVImage, util::SpinLock)

            typedef std::unique_ptr<YUVImage> UniquePtr;

            enum {
                Y_INDEX,
                U_INDEX,
                V_INDEX,
                A_INDEX
            };

            struct _LIB_THEKOGANS_CANVAS_DECL Planes {
                util::ui8 *planes[4];

                Planes () {
                    planes[Y_INDEX] =
                    planes[U_INDEX] =
                    planes[V_INDEX] =
                    planes[A_INDEX] = 0;
                }
                Planes (util::ui8 *planes_[4]) {
                    planes[Y_INDEX] = planes_[Y_INDEX];
                    planes[U_INDEX] = planes_[U_INDEX];
                    planes[V_INDEX] = planes_[V_INDEX];
                    planes[A_INDEX] = planes_[A_INDEX];
                }
                Planes (
                        util::ui8 *y,
                        util::ui8 *u,
                        util::ui8 *v,
                        util::ui8 *a) {
                    planes[Y_INDEX] = y;
                    planes[U_INDEX] = u;
                    planes[V_INDEX] = v;
                    planes[A_INDEX] = a;
                }

                inline util::ui8 *operator [] (std::size_t index) {
                    assert (index < 4);
                    return planes[index];
                }
                inline util::ui8 *operator [] (std::size_t index) const {
                    assert (index < 4);
                    return planes[index];
                }
            };

            struct _LIB_THEKOGANS_CANVAS_DECL Strides {
                util::ui32 strides[4];

                Strides () {
                    strides[Y_INDEX] =
                    strides[U_INDEX] =
                    strides[V_INDEX] =
                    strides[A_INDEX] = 0;
                }
                Strides (util::ui32 strides_[4]) {
                    strides[Y_INDEX] = strides_[Y_INDEX];
                    strides[U_INDEX] = strides_[U_INDEX];
                    strides[V_INDEX] = strides_[V_INDEX];
                    strides[A_INDEX] = strides_[A_INDEX];
                }
                Strides (
                        util::ui32 y,
                        util::ui32 u,
                        util::ui32 v,
                        util::ui32 a) {
                    strides[Y_INDEX] = y;
                    strides[U_INDEX] = u;
                    strides[V_INDEX] = v;
                    strides[A_INDEX] = a;
                }

                inline util::ui32 operator [] (std::size_t index) {
                    assert (index < 4);
                    return strides[index];
                }
                inline util::ui32 operator [] (std::size_t index) const {
                    assert (index < 4);
                    return strides[index];
                }
            };

        protected:
            // VERY, VERY IMPORTANT: YUVImage implements a shallow
            // copy without reference counting. Semantically, what
            // this means is, the 'owner' image must be the last one
            // to go out of scope or the 'copies' will be left with a
            // dangling pointer. To aid with that, use Swap liberally
            // for very inexpensive ownership transfer.
            util::ui8 *data;
            util::Rectangle::Extents extents;
            Planes planes;
            Strides strides;
            bool owner;

        public:
            YUVImage () :
                data (0),
                owner (false) {}
            YUVImage (const util::Rectangle::Extents &extents_,
                bool hasAlpha = false,
                bool clear = false);
            YUVImage (
                util::ui8 *data_,
                const util::Rectangle::Extents &extents_,
                const Planes &planes_,
                const Strides &strides_,
                bool owner_ = true) :
                data (data_),
                extents (extents_),
                planes (planes_),
                strides (strides_),
                owner (owner_) {}
            YUVImage (const YUVImage &image) :
                data (image.data),
                extents (image.extents),
                planes (image.planes),
                strides (image.strides),
                owner (false) {}
            ~YUVImage () {
                Release ();
            }

            inline util::ui8 *GetData () const {
                return data;
            }
            inline const util::Rectangle::Extents &GetExtents () const {
                return extents;
            }
            inline util::Rectangle GetRectangle () const {
                return util::Rectangle (util::Point (), extents);
            }
            inline util::ui8 *GetYPlane () const {
                return planes[Y_INDEX];
            }
            inline util::ui8 *GetUPlane () const {
                return planes[U_INDEX];
            }
            inline util::ui8 *GetVPlane () const {
                return planes[V_INDEX];
            }
            inline util::ui8 *GetAPlane () const {
                return planes[A_INDEX];
            }
            inline util::ui32 GetYStride () const {
                return strides[Y_INDEX];
            }
            inline util::ui32 GetUStride () const {
                return strides[U_INDEX];
            }
            inline util::ui32 GetVStride () const {
                return strides[V_INDEX];
            }
            inline util::ui32 GetAStride () const {
                return strides[A_INDEX];
            }

            inline void Swap (YUVImage &image) {
                std::swap (data, image.data);
                std::swap (extents, image.extents);
                std::swap (planes, image.planes);
                std::swap (strides, image.strides);
                std::swap (owner, image.owner);
            }

            void Clear (
                const util::Rectangle &rectangle,
                const Color &color = Color ());

            enum Filter {
                UnknownFilter,
                Nearest,
                Box,
                Bilinear
            };
            static std::string FilterTostring (Filter filter);
            static Filter stringToFilter (const std::string &filter);

            UniquePtr Scale (
                const util::Rectangle::Extents &dstExtents,
                Filter filter) const;
            void Scale (YUVImage &dst, Filter filter) const;

            enum Angle {
                UnknownAngle,
                Angle90,
                Angle180,
                Angle270,
            };
            static std::string AngleTostring (Angle angle);
            static Angle stringToAngle (const std::string &angle);

            UniquePtr Rotate (Angle angle) const;
            void Rotate (
                YUVImage &dst,
                Angle angle) const;
            // NOTE: angle is in degrees.
            UniquePtr Rotate (
                util::f32 angle,
                const util::Point &centerOfRotation,
                const Color &fillColor = Color (0, 0, 0)) const;

            enum Axis {
                UnknownAxis,
                X,
                Y
            };
            static std::string AxisTostring (Axis axis);
            static Axis stringToAxis (const std::string &axis);

            UniquePtr Mirror (Axis axis) const;

            UniquePtr Copy (const util::Rectangle &rectangle) const;
            void Copy (
                const util::Rectangle &rectangle,
                const util::Point &origin,
                YUVImage &dst,
                bool hasAlpha = false) const;

            std::unique_ptr<RGBImage> ToRGBImage (
                util::ui32 componentIndices) const;

            YUVImage &operator = (const YUVImage &image);

        private:
            void Release ();

            UniquePtr MirrorX () const;
            UniquePtr MirrorY () const;
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_YUVImage_h)

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
#include <algorithm>
#include <libyuv.h>
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/RGBImage.h"
#include "thekogans/canvas/YUVImage.h"

namespace thekogans {
    namespace canvas {

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK (YUVImage, util::SpinLock)

        YUVImage::YUVImage (
                const Rectangle::Extents &extents_,
                bool hasAlpha,
                bool clear) :
                data (
                    new util::ui8[
                        extents_.GetArea () + // Y
                        extents_.GetArea () / 2 + // UV
                        (hasAlpha ? extents_.GetArea () : 0)]), // A
                extents (extents_),
                owner (true) {
            planes.planes[Y_INDEX] = data;
            planes.planes[U_INDEX] = data + extents.GetArea ();
            planes.planes[V_INDEX] = data + 5 * extents.GetArea () / 4;
            planes.planes[A_INDEX] = hasAlpha ? data + 3 * extents.GetArea () / 2 : 0;
            strides.strides[Y_INDEX] = extents.width;
            strides.strides[U_INDEX] =
            strides.strides[V_INDEX] = extents.width / 2;
            strides.strides[A_INDEX] = hasAlpha ? extents.width : 0;
        }

        namespace {
            #define RGB2YUV_SHIFT 15

            #define RY ( (int)(0.299 * 219 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
            #define GY ( (int)(0.587 * 219 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
            #define BY ( (int)(0.114 * 219 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))

            #define RU (-(int)(0.169 * 224 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
            #define GU (-(int)(0.331 * 224 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
            #define BU ( (int)(0.500 * 224 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))

            #define RV ( (int)(0.500 * 224 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
            #define GV (-(int)(0.419 * 224 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
            #define BV (-(int)(0.081 * 224 / 255 * (1 << RGB2YUV_SHIFT) + 0.5))
        }

        void YUVImage::Clear (
                const Rectangle &rectangle,
                const Color &color) {
            Rectangle srcRectangle = rectangle.Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate ()) {
                // Y
                {
                    util::ui8 y =
                        ((RY * color.r + GY * color.g + BY * color.b) >> RGB2YUV_SHIFT) + 16;
                    util::ui32 yStride = strides[Y_INDEX];
                    util::ui8 *yPlane = planes[Y_INDEX] +
                        srcRectangle.origin.y * yStride + srcRectangle.origin.x;
                    util::ui32 width = srcRectangle.extents.width;
                    util::ui32 height = srcRectangle.extents.height;
                    while (height-- != 0) {
                        memset (yPlane, y, width);
                        yPlane += yStride;
                    }
                }
                // UV
                {
                    util::ui8 u =
                        ((RU * color.r + GU * color.g + BU * color.b) >> RGB2YUV_SHIFT) + 128;
                    util::ui8 v =
                        ((RV * color.r + GV * color.g + BV * color.b) >> RGB2YUV_SHIFT) + 128;
                    util::ui32 uStride = strides[U_INDEX];
                    util::ui32 vStride = strides[V_INDEX];
                    util::ui8 *uPlane = planes[U_INDEX] +
                        (srcRectangle.origin.y * uStride + srcRectangle.origin.x) / 2;
                    util::ui8 *vPlane = planes[V_INDEX] +
                        (srcRectangle.origin.y * vStride + srcRectangle.origin.x) / 2;
                    util::ui32 width = srcRectangle.extents.width / 2;
                    util::ui32 height = srcRectangle.extents.height / 2;
                    while (height-- != 0) {
                        memset (uPlane, u, width);
                        uPlane += uStride;
                        memset (vPlane, v, width);
                        vPlane += vStride;
                    }
                }
                // A
                if (planes[A_INDEX] != 0) {
                    util::ui8 a = color.a;
                    util::ui32 aStride = strides[A_INDEX];
                    util::ui8 *aPlane = planes[A_INDEX] +
                        srcRectangle.origin.y * aStride + srcRectangle.origin.x;
                    util::ui32 width = srcRectangle.extents.width;
                    util::ui32 height = srcRectangle.extents.height;
                    while (height-- != 0) {
                        memset (aPlane, a, width);
                        aPlane += aStride;
                    }
                }
            }
        }

        std::string YUVImage::FilterTostring (Filter filter) {
            return filter == Nearest ? "Nearest" :
                filter == Box ? "Box" :
                filter == Bilinear ? "Bilinear" : "UnknownFilter";
        }

        YUVImage::Filter YUVImage::stringToFilter (const std::string &filter) {
            return filter == "Nearest" ? Nearest :
                filter == "Box" ? Box :
                filter == "Bilinear" ? Bilinear : UnknownFilter;
        }

        YUVImage::UniquePtr YUVImage::Scale (
                const Rectangle::Extents &dstExtents, Filter filter) const {
            UniquePtr dst (new YUVImage (dstExtents));
            Scale (*dst, filter);
            return dst;
        }

        void YUVImage::Scale (
                YUVImage &dst,
                Filter filter) const {
            libyuv::I420Scale (
                planes[Y_INDEX], strides[Y_INDEX],
                planes[U_INDEX], strides[U_INDEX],
                planes[V_INDEX], strides[V_INDEX],
                extents.width, extents.height,
                dst.planes[Y_INDEX], dst.strides[Y_INDEX],
                dst.planes[U_INDEX], dst.strides[U_INDEX],
                dst.planes[V_INDEX], dst.strides[V_INDEX],
                dst.extents.width, dst.extents.height,
                filter == Bilinear ? libyuv::kFilterBilinear :
                filter == Box ? libyuv::kFilterBox : libyuv::kFilterNone);
        }

        std::string YUVImage::AngleTostring (Angle angle) {
            return angle == Angle90 ? "Angle90" :
                angle == Angle180 ? "Angle180" :
                angle == Angle270 ? "Angle270" : "UnknownAngle";
        }

        YUVImage::Angle YUVImage::stringToAngle (const std::string &angle) {
            return angle == "Angle90" ? Angle90 :
                angle == "Angle180" ? Angle180 :
                angle == "Angle270" ? Angle270 : UnknownAngle;
        }

        YUVImage::UniquePtr YUVImage::Rotate (Angle angle) const {
            canvas::Rectangle::Extents dstExtents;
            switch (angle) {
                case Angle90:
                case Angle270:
                    dstExtents.width = extents.height;
                    dstExtents.height = extents.width;
                    break;
                default:
                    dstExtents.width = extents.width;
                    dstExtents.height = extents.height;
                    break;
            }
            UniquePtr dst (new YUVImage (dstExtents));
            Rotate (*dst, angle);
            return dst;
        }

        void YUVImage::Rotate (YUVImage &dst, Angle angle) const {
            libyuv::I420Rotate (
                planes[Y_INDEX], strides[Y_INDEX],
                planes[U_INDEX], strides[U_INDEX],
                planes[V_INDEX], strides[V_INDEX],
                dst.planes[Y_INDEX], dst.strides[Y_INDEX],
                dst.planes[U_INDEX], dst.strides[U_INDEX],
                dst.planes[V_INDEX], dst.strides[V_INDEX],
                extents.width, extents.height,
                angle == Angle90 ? libyuv::kRotate90 :
                angle == Angle180 ? libyuv::kRotate180 : libyuv::kRotate270);
        }

        YUVImage::UniquePtr YUVImage::Rotate (util::f32 angle,
                const Point &centerOfRotation, const Color &fillColor) const {
            // FIXME: implement
            assert (0);
            return YUVImage::UniquePtr ();
        }

        std::string YUVImage::AxisTostring (Axis axis) {
            return axis == X ? "X" :
                axis == Y ? "Y" : "UnknownAxis";
        }

        YUVImage::Axis YUVImage::stringToAxis (const std::string &axis) {
            return axis == "X" ? X :
                axis == "Y" ? Y : UnknownAxis;
        }

        YUVImage::UniquePtr YUVImage::Mirror (Axis axis) const {
            switch (axis) {
                case UnknownAxis:
                    break;
                case X:
                    return MirrorX ();
                case Y:
                    return MirrorY ();
            }
            return UniquePtr ();
        }

        YUVImage::UniquePtr YUVImage::Copy (const Rectangle &rectangle) const {
            UniquePtr dst;
            Rectangle srcRectangle = rectangle.Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate ()) {
                dst.reset (
                    new YUVImage (srcRectangle.extents, planes[A_INDEX] != 0));
                if (dst.get () != 0) {
                    // Y
                    {
                        const util::ui8 *srcYPlane = planes[Y_INDEX] +
                            srcRectangle.origin.y * strides[Y_INDEX] + srcRectangle.origin.x;
                        util::ui32 srcYStride = strides[Y_INDEX];
                        util::ui8 *dstYPlane = dst->planes[Y_INDEX];
                        util::ui32 dstYStride = dst->strides[Y_INDEX];
                        util::ui32 width = dst->extents.width;
                        util::ui32 height = dst->extents.height;
                        while (height-- != 0) {
                            memcpy (dstYPlane, srcYPlane, width);
                            srcYPlane += srcYStride;
                            dstYPlane += dstYStride;
                        }
                    }
                    // UV
                    {
                        const util::ui8 *srcUPlane = planes[U_INDEX] +
                            (srcRectangle.origin.y * strides[U_INDEX] + srcRectangle.origin.x) / 2;
                        util::ui32 srcUStride = strides[U_INDEX];
                        const util::ui8 *srcVPlane = planes[V_INDEX] +
                            (srcRectangle.origin.y * strides[V_INDEX] + srcRectangle.origin.x) / 2;
                        util::ui32 srcVStride = strides[V_INDEX];
                        util::ui8 *dstUPlane = dst->planes[U_INDEX];
                        util::ui32 dstUStride = dst->strides[U_INDEX];
                        util::ui8 *dstVPlane = dst->planes[V_INDEX];
                        util::ui32 dstVStride = dst->strides[V_INDEX];
                        util::ui32 width = dst->extents.width / 2;
                        util::ui32 height = dst->extents.height / 2;
                        while (height-- != 0) {
                            memcpy (dstUPlane, srcUPlane, width);
                            srcUPlane += srcUStride;
                            dstUPlane += dstUStride;
                            memcpy (dstVPlane, srcVPlane, width);
                            srcVPlane += srcVStride;
                            dstVPlane += dstVStride;
                        }
                    }
                    // A
                    if (planes[A_INDEX] != 0) {
                        const util::ui8 *srcAPlane = planes[A_INDEX] +
                            srcRectangle.origin.y * strides[A_INDEX] + srcRectangle.origin.x;
                        util::ui32 srcAStride = strides[A_INDEX];
                        util::ui8 *dstAPlane = dst->planes[A_INDEX];
                        util::ui32 dstAStride = dst->strides[A_INDEX];
                        util::ui32 width = dst->extents.width;
                        util::ui32 height = dst->extents.height;
                        while (height-- != 0) {
                            memcpy (dstAPlane, srcAPlane, width);
                            srcAPlane += srcAStride;
                            dstAPlane += dstAStride;
                        }
                    }
                }
            }
            return dst;
        }

        void YUVImage::Copy (
                const Rectangle &rectangle,
                const Point &origin,
                YUVImage &dst,
                bool hasAlpha) const {
            //assert (IsValid ());
            Rectangle srcRectangle = rectangle.Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate ()) {
                Point offset = srcRectangle.origin - rectangle.origin;
                Rectangle dstRectangle =
                    Rectangle (origin + offset, rectangle.extents - offset).Intersection (
                        dst.GetRectangle ());
                if (!dstRectangle.IsDegenerate ()) {
                    if (!hasAlpha || planes[A_INDEX] == 0 || dst.planes[Y_INDEX] == 0) {
                        // Y
                        {
                            const util::ui8 *srcYPlane = planes[Y_INDEX] +
                                srcRectangle.origin.y * strides[Y_INDEX] + srcRectangle.origin.x;
                            util::ui32 srcYStride = strides[Y_INDEX];
                            util::ui8 *dstYPlane = dst.planes[Y_INDEX] +
                                dstRectangle.origin.y * dst.strides[Y_INDEX] + dstRectangle.origin.x;
                            util::ui32 dstYStride = dst.strides[Y_INDEX];
                            util::ui32 width = std::min (
                                srcRectangle.extents.width, dstRectangle.extents.width);
                            util::ui32 height = std::min (
                                srcRectangle.extents.height, dstRectangle.extents.height);
                            while (height-- != 0) {
                                memcpy (dstYPlane, srcYPlane, width);
                                srcYPlane += srcYStride;
                                dstYPlane += dstYStride;
                            }
                        }
                        // UV
                        {
                            const util::ui8 *srcUPlane = planes[U_INDEX] +
                                (srcRectangle.origin.y * strides[U_INDEX] + srcRectangle.origin.x) / 2;
                            util::ui32 srcUStride = strides[U_INDEX];
                            const util::ui8 *srcVPlane = planes[V_INDEX] +
                                (srcRectangle.origin.y * strides[V_INDEX] + srcRectangle.origin.x) / 2;
                            util::ui32 srcVStride = strides[V_INDEX];
                            util::ui8 *dstUPlane = dst.planes[U_INDEX] +
                                (dstRectangle.origin.y * dst.strides[U_INDEX] + dstRectangle.origin.x) / 2;
                            util::ui32 dstUStride = dst.strides[U_INDEX];
                            util::ui8 *dstVPlane = dst.planes[V_INDEX] +
                                (dstRectangle.origin.y * dst.strides[V_INDEX] + dstRectangle.origin.x) / 2;
                            util::ui32 dstVStride = dst.strides[V_INDEX];
                            util::ui32 width = std::min (
                                srcRectangle.extents.width / 2, dstRectangle.extents.width / 2);
                            util::ui32 height = std::min (
                                srcRectangle.extents.height / 2, dstRectangle.extents.height / 2);
                            while (height-- != 0) {
                                memcpy (dstUPlane, srcUPlane, width);
                                srcUPlane += srcUStride;
                                dstUPlane += dstUStride;
                                memcpy (dstVPlane, srcVPlane, width);
                                srcVPlane += srcVStride;
                                dstVPlane += dstVStride;
                            }
                        }
                        // A
                        if (planes[A_INDEX] != 0 && dst.planes[A_INDEX] != 0) {
                            const util::ui8 *srcAPlane = planes[A_INDEX] +
                                srcRectangle.origin.y * strides[A_INDEX] + srcRectangle.origin.x;
                            util::ui32 srcAStride = strides[A_INDEX];
                            util::ui8 *dstAPlane = dst.planes[A_INDEX] +
                                dstRectangle.origin.y * dst.strides[A_INDEX] + dstRectangle.origin.x;
                            util::ui32 dstAStride = dst.strides[A_INDEX];
                            util::ui32 width = std::min (
                                srcRectangle.extents.width, dstRectangle.extents.width);
                            util::ui32 height = std::min (
                                srcRectangle.extents.height, dstRectangle.extents.height);
                            while (height-- != 0) {
                                memcpy (dstAPlane, srcAPlane, width);
                                srcAPlane += srcAStride;
                                dstAPlane += dstAStride;
                            }
                        }
                    }
                    else {
                        // Y
                        {
                            const util::ui8 *srcYPlane = planes[Y_INDEX] +
                                srcRectangle.origin.y * strides[Y_INDEX] + srcRectangle.origin.x;
                            util::ui32 srcYStride = strides[Y_INDEX];
                            const util::ui8 *srcAPlane = planes[A_INDEX] +
                                srcRectangle.origin.y * strides[A_INDEX] + srcRectangle.origin.x;
                            util::ui32 srcAStride = strides[A_INDEX];
                            util::ui8 *dstYPlane = dst.planes[Y_INDEX] +
                                dstRectangle.origin.y * dst.strides[Y_INDEX] + dstRectangle.origin.x;
                            util::ui32 dstYStride = dst.strides[Y_INDEX];
                            util::ui8 *dstAPlane = dst.planes[A_INDEX] +
                                dstRectangle.origin.y * dst.strides[A_INDEX] + dstRectangle.origin.x;
                            util::ui32 dstAStride = dst.strides[A_INDEX];
                            util::ui32 width = std::min (
                                srcRectangle.extents.width, dstRectangle.extents.width);
                            util::ui32 height = std::min (
                                srcRectangle.extents.height, dstRectangle.extents.height);
                            while (height-- != 0) {
                                const util::ui8 *srcYRow = srcYPlane;
                                srcYPlane += srcYStride;
                                const util::ui8 *srcARow = srcAPlane;
                                srcAPlane += srcAStride;
                                util::ui8 *dstYRow = dstYPlane;
                                dstYPlane += dstYStride;
                                util::ui8 *dstARow = dstAPlane;
                                dstAPlane += dstAStride;
                                for (util::ui32 i = 0; i < width; ++i) {
                                    util::ui32 srcY = *srcYRow++;
                                    util::ui32 srcA = *srcARow++;
                                    util::ui32 dstY = *dstYRow;
                                    *dstYRow++ =
                                        (util::ui8)((srcA * srcY + dstY * (255 - srcA)) / 255);
                                }
                            }
                        }
                        // UV
                        {
                            const util::ui8 *srcUPlane = planes[U_INDEX] +
                                (srcRectangle.origin.y * strides[U_INDEX] + srcRectangle.origin.x) / 2;
                            util::ui32 srcUStride = strides[U_INDEX];
                            const util::ui8 *srcVPlane = planes[V_INDEX] +
                                (srcRectangle.origin.y * strides[V_INDEX] + srcRectangle.origin.x) / 2;
                            util::ui32 srcVStride = strides[V_INDEX];
                            const util::ui8 *srcAPlane = planes[A_INDEX] +
                                srcRectangle.origin.y * strides[A_INDEX] + srcRectangle.origin.x;
                            util::ui32 srcAStride = strides[A_INDEX];
                            util::ui8 *dstUPlane = dst.planes[U_INDEX] +
                                (dstRectangle.origin.y * dst.strides[U_INDEX] + dstRectangle.origin.x) / 2;
                            util::ui32 dstUStride = dst.strides[U_INDEX];
                            util::ui8 *dstVPlane = dst.planes[V_INDEX] +
                                (dstRectangle.origin.y * dst.strides[V_INDEX] + dstRectangle.origin.x) / 2;
                            util::ui32 dstVStride = dst.strides[V_INDEX];
                            util::ui32 width = std::min (
                                srcRectangle.extents.width / 2, dstRectangle.extents.width / 2);
                            util::ui32 height = std::min (
                                srcRectangle.extents.height / 2, dstRectangle.extents.height / 2);
                            while (height-- != 0) {
                                const util::ui8 *srcURow = srcUPlane;
                                srcUPlane += srcUStride;
                                const util::ui8 *srcVRow = srcVPlane;
                                srcVPlane += srcVStride;
                                const util::ui8 *srcARow0 = srcAPlane;
                                srcAPlane += srcAStride;
                                const util::ui8 *srcARow1 = srcAPlane;
                                srcAPlane += srcAStride;
                                util::ui8 *dstURow = dstUPlane;
                                dstUPlane += dstUStride;
                                util::ui8 *dstVRow = dstVPlane;
                                dstVPlane += dstVStride;
                                for (util::ui32 i = 0; i < width; ++i) {
                                    util::ui32 srcU = *srcURow++;
                                    util::ui32 srcV = *srcVRow++;
                                    util::ui32 srcA00 = *srcARow0++;
                                    util::ui32 srcA01 = *srcARow0++;
                                    util::ui32 srcA10 = *srcARow1++;
                                    util::ui32 srcA11 = *srcARow1++;
                                    util::ui32 srcA = (srcA00 + srcA01 + srcA10 + srcA11) / 4;
                                    util::ui32 dstU = *dstURow;
                                    util::ui32 dstV = *dstVRow;
                                    *dstURow++ =
                                        (util::ui8)((srcA * srcU + dstU * (255 - srcA)) / 255);
                                    *dstVRow++ =
                                        (util::ui8)((srcA * srcV + dstV * (255 - srcA)) / 255);
                                }
                            }
                        }
                        // A
                        {
                            const util::ui8 *srcAPlane = planes[A_INDEX] +
                                srcRectangle.origin.y * strides[A_INDEX] + srcRectangle.origin.x;
                            util::ui32 srcAStride = strides[A_INDEX];
                            util::ui8 *dstAPlane = dst.planes[A_INDEX] +
                                dstRectangle.origin.y * dst.strides[A_INDEX] + dstRectangle.origin.x;
                            util::ui32 dstAStride = dst.strides[A_INDEX];
                            util::ui32 width = std::min (
                                srcRectangle.extents.width, dstRectangle.extents.width);
                            util::ui32 height = std::min (
                                srcRectangle.extents.height, dstRectangle.extents.height);
                            while (height-- != 0) {
                                const util::ui8 *srcARow = srcAPlane;
                                srcAPlane += srcAStride;
                                util::ui8 *dstARow = dstAPlane;
                                dstAPlane += dstAStride;
                                for (util::ui32 i = 0; i < width; ++i) {
                                    util::ui32 srcA = *srcARow++;
                                    util::ui32 dstA = *dstARow;
                                    *dstARow++ =
                                        (util::ui8)((srcA * srcA + dstA * (255 - srcA)) / 255);
                                }
                            }
                        }
                    }
                }
            }
        }

        RGBImage::UniquePtr YUVImage::ToRGBImage (util::ui32 componentIndices) const {
            RGBImage::UniquePtr dst;
            if (RGBImage::IsValidComponentIndices (componentIndices)) {
                dst.reset (new RGBImage (extents, componentIndices, 4, 0));
                switch (componentIndices) {
                    case RGBImage::R0G1B2A3:
                        libyuv::I420ToRGBA (
                            planes[Y_INDEX], strides[Y_INDEX],
                            planes[U_INDEX], strides[U_INDEX],
                            planes[V_INDEX], strides[V_INDEX],
                            dst->GetData (), dst->GetRowStride (),
                            extents.width, extents.height);
                        break;
                    case RGBImage::R2G1B0A3:
                        libyuv::I420ToBGRA (
                            planes[Y_INDEX], strides[Y_INDEX],
                            planes[U_INDEX], strides[U_INDEX],
                            planes[V_INDEX], strides[V_INDEX],
                            dst->GetData (), dst->GetRowStride (),
                            extents.width, extents.height);
                        break;
                    case RGBImage::R1G2B3A0:
                        libyuv::I420ToARGB (
                            planes[Y_INDEX], strides[Y_INDEX],
                            planes[U_INDEX], strides[U_INDEX],
                            planes[V_INDEX], strides[V_INDEX],
                            dst->GetData (), dst->GetRowStride (),
                            extents.width, extents.height);
                        break;
                    case RGBImage::R3G2B1A0:
                        libyuv::I420ToABGR (
                            planes[Y_INDEX], strides[Y_INDEX],
                            planes[U_INDEX], strides[U_INDEX],
                            planes[V_INDEX], strides[V_INDEX],
                            dst->GetData (), dst->GetRowStride (),
                            extents.width, extents.height);
                        break;
                }
            }
            return dst;
        }

        YUVImage &YUVImage::operator = (const YUVImage &image) {
            if (&image != this) {
                Release ();
                data = image.data;
                extents = image.extents;
                planes = image.planes;
                strides = image.strides;
                owner = false;
            }
            return *this;
        }

        void YUVImage::Release () {
            if (owner) {
                if (data != 0) {
                    delete [] data;
                    data = 0;
                }
                owner = false;
            }
        }

        YUVImage::UniquePtr YUVImage::MirrorX () const {
            // FIXME: implement
            assert (0);
            return YUVImage::UniquePtr ();
        }

        YUVImage::UniquePtr YUVImage::MirrorY () const {
            // FIXME: implement
            assert (0);
            return YUVImage::UniquePtr ();
        }

    } // namespace canvas
} // namespace thekogans

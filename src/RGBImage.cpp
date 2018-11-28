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
#include <cmath>
#include <algorithm>
#include <libyuv.h>
#include "thekogans/util/Types.h"
#include "thekogans/util/Array.h"
#include "thekogans/util/Buffer.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/File.h"
#include "thekogans/canvas/TJUtils.h"
#include "thekogans/canvas/lodepng.h"
#include "thekogans/canvas/YUVImage.h"
#include "thekogans/canvas/RGBImage.h"

namespace thekogans {
    namespace canvas {

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK (RGBImage, util::SpinLock)

        std::string RGBImage::ComponentIndicesTostring (
                util::ui32 componentIndices) {
            return
                componentIndices == R0G1B2A3 ? "R0G1B2A3" :
                componentIndices == R2G1B0A3 ? "R2G1B0A3" :
                componentIndices == R1G2B3A0 ? "R1G2B3A0" :
                componentIndices == R3G2B1A0 ? "R3G2B1A0" : "";
        }

        util::ui32 RGBImage::stringToComponentIndices (
                const std::string &componentIndices) {
            return
                componentIndices == "R0G1B2A3" ? R0G1B2A3 :
                componentIndices == "R2G1B0A3" ? R2G1B0A3 :
                componentIndices == "R1G2B3A0" ? R1G2B3A0 :
                componentIndices == "R3G2B1A0" ? R3G2B1A0 : 0;
        }

        RGBImage::RGBImage (const Rectangle::Extents &extents_,
                util::ui32 componentIndices_,
                util::ui32 pixelStride_,
                util::ui32 rowStride_,
                bool clear) :
                extents (extents_), componentIndices (componentIndices_),
                pixelStride (pixelStride_), rowStride (rowStride_), owner (true) {
            assert (pixelStride >= 3);
            if (rowStride == 0) {
                rowStride = extents.width * pixelStride;
            }
            util::ui32 size = extents.height * rowStride;
            data = new util::ui8[size];
            if (clear) {
                memset (data, 0, size);
            }
        }

        RGBImage::UniquePtr RGBImage::FromPNGBuffer (
                const util::ui8 *buffer,
                util::ui32 size,
                util::ui32 componentIndices) {
            UniquePtr image;
            std::vector<util::ui8> data;
            util::ui32 width = 0;
            util::ui32 height = 0;
            util::ui32 error = lodepng::decode (data, width, height, buffer, size);
            if (error == 0) {
                image.reset (
                    new RGBImage (
                        Rectangle::Extents (width, height),
                        componentIndices, 4, 0));
                util::ui8 *dstData = image->GetData ();
                const util::ui8 *srcData = &data[0];
                util::ui32 rIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, R_INDEX);
                util::ui32 gIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, G_INDEX);
                util::ui32 bIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, B_INDEX);
                util::ui32 aIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, A_INDEX);
                for (util::ui32 y = 0; y < height; ++y) {
                    for (util::ui32 x = 0; x < width; ++x) {
                        dstData[rIndex] = srcData[0];
                        dstData[gIndex] = srcData[1];
                        dstData[bIndex] = srcData[2];
                        dstData[aIndex] = srcData[3];
                        dstData += 4;
                        srcData += 4;
                    }
                }
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to load a PNG image from buffer (%s)",
                    lodepng_error_text (error));
            }
            return image;
        }

        RGBImage::UniquePtr RGBImage::FromPNGFile (
                const std::string &path,
                util::ui32 componentIndices) {
            UniquePtr image;
            std::vector<util::ui8> data;
            util::ui32 width = 0;
            util::ui32 height = 0;
            util::ui32 error = lodepng::decode (data, width, height, path.c_str ());
            if (error == 0) {
                image.reset (
                    new RGBImage (
                        Rectangle::Extents (width, height),
                        componentIndices, 4, 0));
                util::ui8 *dstData = image->GetData ();
                const util::ui8 *srcData = &data[0];
                util::ui32 rIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, R_INDEX);
                util::ui32 gIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, G_INDEX);
                util::ui32 bIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, B_INDEX);
                util::ui32 aIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, A_INDEX);
                for (util::ui32 y = 0; y < height; ++y) {
                    for (util::ui32 x = 0; x < width; ++x) {
                        dstData[rIndex] = srcData[0];
                        dstData[gIndex] = srcData[1];
                        dstData[bIndex] = srcData[2];
                        dstData[aIndex] = srcData[3];
                        dstData += 4;
                        srcData += 4;
                    }
                }
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to load a PNG image from %s (%s)",
                    path.c_str (), lodepng_error_text (error));
            }
            return image;
        }

        RGBImage::UniquePtr RGBImage::FromJPGBuffer (
                const util::ui8 *buffer,
                util::ui32 size,
                util::ui32 componentIndices) {
            UniquePtr image;
            {
                TJDecompressHandle handle;
                int width;
                int height;
                int jpegSubsamp;
                if (tjDecompressHeader2 (handle.handle, (util::ui8 *)buffer,
                        size, &width, &height, &jpegSubsamp) == 0) {
                    util::ui32 pixelStride = 4;
                    util::ui32 rowStride = width * pixelStride;
                    util::ui8 *data = new util::ui8[height * rowStride];
                    if (tjDecompress2 (handle.handle, (util::ui8 *)buffer,
                            size, data, width, rowStride, height,
                            ComponentIndicesToTJPixelFormat (
                                componentIndices, pixelStride), 0) == 0) {
                        image.reset (
                            new RGBImage (data, Rectangle::Extents (width, height),
                                componentIndices, pixelStride, rowStride, true));
                    }
                    else {
                        delete [] data;
                        THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                            "%s", tjGetErrorStr ());
                    }
                }
                else {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "%s", tjGetErrorStr ());
                }
            }
            return image;
        }

        RGBImage::UniquePtr RGBImage::FromJPGFile (
                const std::string &path,
                util::ui32 componentIndices) {
            UniquePtr image;
            {
                util::ReadOnlyFile file (util::HostEndian, path);
                util::ui32 size = (util::ui32)file.GetSize ();
                if (size > 0) {
                    std::vector<util::ui8> buffer (size);
                    file.Read (&buffer[0], size);
                    image = FromJPGBuffer (&buffer[0], size, componentIndices);
                }
                else {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "Empty jpg file: %s", path.c_str ());
                }
            }
            return image;
        }

        namespace {
            const util::i16 BMP_TYPE = 0x4d42;

            struct FileHeader {
                util::i16 bfType;
                util::i32 bfSize;
                util::i16 bfReserved1;
                util::i16 bfReserved2;
                util::i32 bfOffBits;

                FileHeader () :
                    bfType (0),
                    bfSize (0),
                    bfReserved1 (0),
                    bfReserved2 (0),
                    bfOffBits (0) {}
            };

            struct InfoHeader {
                util::i32 biSize;
                util::i32 biWidth;
                util::i32 biHeight;
                util::i16 biPlanes;
                util::i16 biBitCount;
                util::i32 biCompression;
                util::i32 biSizeImage;
                util::i32 biXPelsPerMeter;
                util::i32 biYPelsPerMeter;
                util::i32 biClrUsed;
                util::i32 biClrImportant;

                InfoHeader () :
                    biSize (0),
                    biWidth (0),
                    biHeight (0),
                    biPlanes (0),
                    biBitCount (0),
                    biCompression (0),
                    biSizeImage (0),
                    biXPelsPerMeter (0),
                    biYPelsPerMeter (0),
                    biClrUsed (0),
                    biClrImportant (0) {}
            };

            inline util::Buffer &operator >> (
                    util::Buffer &buffer,
                    FileHeader &fileHeader) {
                buffer >> fileHeader.bfType >> fileHeader.bfSize >>
                    fileHeader.bfReserved1 >> fileHeader.bfReserved2 >>
                    fileHeader.bfOffBits;
                return buffer;
            }

            inline util::Buffer &operator >> (
                    util::Buffer &buffer,
                    InfoHeader &infoHeader) {
                buffer >> infoHeader.biSize >> infoHeader.biWidth >>
                    infoHeader.biHeight >> infoHeader.biPlanes >>
                    infoHeader.biBitCount >> infoHeader.biCompression >>
                    infoHeader.biSizeImage >> infoHeader.biXPelsPerMeter >>
                    infoHeader.biYPelsPerMeter >> infoHeader.biClrUsed >>
                    infoHeader.biClrImportant;
                return buffer;
            }
        }

        RGBImage::UniquePtr RGBImage::FromBMPBuffer (
                const util::ui8 *buffer,
                util::ui32 size,
                util::ui32 componentIndices) {
            UniquePtr image;
            util::Buffer buffer_ (
                util::LittleEndian,
                const_cast<util::ui8 *> (buffer),
                size,
                false);
            FileHeader fileHeader;
            InfoHeader infoHeader;
            buffer_ >> fileHeader >> infoHeader;
            // We read a fairly limited (but common) subset of formats.
            // - one plane
            // - 24 or 32 bpp
            // - no compression
            if (fileHeader.bfType == BMP_TYPE &&
                    infoHeader.biSize == sizeof (InfoHeader) &&
                    infoHeader.biWidth > 0 &&
                    infoHeader.biHeight > 0 &&
                    infoHeader.biPlanes == 1 &&
                    (infoHeader.biBitCount == 24 || infoHeader.biBitCount == 32) &&
                    infoHeader.biCompression == 0) {
                util::ui32 pixelStride = 4;
                util::ui32 rowStride = infoHeader.biWidth * pixelStride;
                util::ui8 *data =
                    new util::ui8[infoHeader.biHeight * rowStride];
                {
                    util::ui8 *dataPtr = data;
                    util::ui32 rIndex =
                        THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, R_INDEX);
                    util::ui32 gIndex =
                        THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, G_INDEX);
                    util::ui32 bIndex =
                        THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, B_INDEX);
                    util::ui32 aIndex =
                        THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, A_INDEX);
                    buffer_.writeOffset = fileHeader.bfOffBits;
                    for (util::i32 y = 0; y < infoHeader.biHeight; ++y) {
                        for (util::i32 x = 0; x < infoHeader.biWidth; ++x) {
                            buffer_ >> dataPtr[rIndex] >> dataPtr[gIndex] >> dataPtr[bIndex];
                            if (infoHeader.biBitCount == 24) {
                                dataPtr[aIndex] = 255;
                            }
                            else {
                                buffer_ >> dataPtr[aIndex];
                            }
                            dataPtr += pixelStride;
                        }
                    }
                }
                image.reset (
                    new RGBImage (data,
                        Rectangle::Extents (infoHeader.biWidth, infoHeader.biHeight),
                        componentIndices, pixelStride, rowStride, true));
            }
            else {
                // FIXME: throw error
                assert (0);
                //THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                //    "Unsupported bmp file: %s", path.c_str ());
            }
            return image;
        }

        RGBImage::UniquePtr RGBImage::FromBMPFile (
                const std::string &path,
                util::ui32 componentIndices) {
            UniquePtr image;
            {
                util::ReadOnlyFile file (util::HostEndian, path);
                util::ui32 size = (util::ui32)file.GetSize ();
                if (size > 0) {
                    std::vector<util::ui8> buffer (size);
                    file.Read (&buffer[0], size);
                    image = FromBMPBuffer (&buffer[0], size, componentIndices);
                }
                else {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "Empty bmp file: %s", path.c_str ());
                }
            }
            return image;
        }

        void RGBImage::Clear (
                const Rectangle &rectangle,
                const Color &color) {
            Rectangle srcRectangle = rectangle.Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate ()) {
                util::ui8 *srcData = data +
                    srcRectangle.origin.y * rowStride +
                    srcRectangle.origin.x * pixelStride;
                util::ui32 height =
                    srcRectangle.extents.height;
                util::ui32 width =
                    srcRectangle.extents.width;
                std::vector<util::ui8> pixel;
                ColorToPixel (color, pixel);
                const util::ui8 *pixelData = &pixel[0];
                while (height-- != 0) {
                    util::ui8 *row = srcData;
                    for (util::ui32 i = 0; i < width; ++i) {
                        memcpy (row, pixelData, pixelStride);
                        row += pixelStride;
                    }
                    srcData += rowStride;
                }
            }
        }

        std::string RGBImage::FilterTostring (Filter filter) {
            return
                filter == Nearest ? "Nearest" :
                filter == Box ? "Box" :
                filter == Bilinear ? "Bilinear" : "UnknownFilter";
        }

        RGBImage::Filter RGBImage::stringToFilter (const std::string &filter) {
            return
                filter == "Nearest" ? Nearest :
                filter == "Box" ? Box :
                filter == "Bilinear" ? Bilinear : UnknownFilter;
        }

        RGBImage::UniquePtr RGBImage::Scale (
                const Rectangle::Extents &dstExtents,
                Filter filter) const {
            UniquePtr dst (new RGBImage (dstExtents, componentIndices, pixelStride));
            Scale (*dst, filter);
            return dst;
        }

        void RGBImage::Scale (
                RGBImage &dst,
                Filter filter) const {
            libyuv::ARGBScale (
                data, rowStride,
                extents.width, extents.height,
                dst.data, dst.rowStride,
                dst.extents.width, dst.extents.height,
                filter == Bilinear ? libyuv::kFilterBilinear :
                filter == Box ? libyuv::kFilterBox : libyuv::kFilterNone);
        }

        std::string RGBImage::AngleTostring (Angle angle) {
            return angle == Angle90 ? "Angle90" :
                angle == Angle180 ? "Angle180" :
                angle == Angle270 ? "Angle270" : "UnknownAngle";
        }

        RGBImage::Angle RGBImage::stringToAngle (const std::string &angle) {
            return angle == "Angle90" ? Angle90 :
                angle == "Angle180" ? Angle180 :
                angle == "Angle270" ? Angle270 : UnknownAngle;
        }

        RGBImage::UniquePtr RGBImage::Rotate (Angle angle) const {
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
            UniquePtr dst (new RGBImage (dstExtents, componentIndices, pixelStride));
            Rotate (*dst, angle);
            return dst;
        }

        void RGBImage::Rotate (
                RGBImage &dst,
                Angle angle) const {
            libyuv::ARGBRotate (
                data, rowStride,
                dst.data, dst.rowStride,
                extents.width, extents.height,
                angle == Angle90 ? libyuv::kRotate90 :
                angle == Angle180 ? libyuv::kRotate180 : libyuv::kRotate270);
        }

        namespace {
            struct f32Point {
                util::f32 x;
                util::f32 y;

                f32Point (
                    util::f32 x_ = 0.0f,
                    util::f32 y_ = 0.0f) :
                    x (x_),
                    y (y_) {}

                inline f32Point Rotate (
                        util::f32 cosAngle,
                        util::f32 sinAngle,
                        const f32Point &p0) {
                    return f32Point (p0.x + (x - p0.x) * cosAngle - (y - p0.y) * sinAngle,
                        p0.y + (y - p0.y) * cosAngle + (x - p0.x) * sinAngle);
                }
            };
        }

        // This algorithm was borrowed from wxWindows. I cleaned it up
        // a bit to suit my needs. Thanks goes to the wxWindows team.
        RGBImage::UniquePtr RGBImage::Rotate (
                util::f32 angle,
                const Point &centerOfRotation,
                const Color &fillColor) const {
            const util::ui32 srcHeight = extents.height;
            const util::ui32 srcWidth = extents.width;
            const util::f32 cosAngle = cos (util::RAD (angle));
            const util::f32 sinAngle = sin (util::RAD (angle));
            const f32Point p0 (centerOfRotation.x, centerOfRotation.y);
            f32Point p1 = f32Point (0, 0).Rotate (cosAngle, sinAngle, p0);
            f32Point p2 = f32Point (0, srcHeight).Rotate (cosAngle, sinAngle, p0);
            f32Point p3 = f32Point (srcWidth, 0).Rotate (cosAngle, sinAngle, p0);
            f32Point p4 = f32Point (srcWidth, srcHeight).Rotate (cosAngle, sinAngle, p0);
            util::i32 x1a =
                (util::i32)floor (
                    std::min (std::min (p1.x, p2.x), std::min (p3.x, p4.x)));
            util::i32 y1a =
                (util::i32)floor (
                    std::min (std::min (p1.y, p2.y), std::min (p3.y, p4.y)));
            util::i32 x2a =
                (util::i32)ceil (
                    std::max (std::max (p1.x, p2.x), std::max (p3.x, p4.x)));
            util::i32 y2a =
                (util::i32)ceil (
                    std::max (std::max (p1.y, p2.y), std::max (p3.y, p4.y)));
            UniquePtr dst (
                new RGBImage (
                    Rectangle::Extents (x2a - x1a + 1, y2a - y1a + 1),
                    componentIndices, pixelStride));
            util::ui8 *dstData = dst->GetData ();
            const util::ui32 dstHeight = dst->GetExtents ().height;
            const util::ui32 dstWidth = dst->GetExtents ().width;
            const util::ui32 dstRowStride = dst->GetRowStride ();
            std::vector<util::ui8> fillPixel;
            ColorToPixel (fillColor, fillPixel);
            for (util::i32 y = 0; y < dstHeight; ++y) {
                util::ui8 *dstRow = dstData;
                for (util::i32 x = 0; x < dstWidth; ++x) {
                    f32Point src = f32Point (x + x1a, y + y1a).Rotate (cosAngle, -sinAngle, p0);
                    if (-0.25f < src.x && src.x < srcWidth - 0.75f &&
                           -0.25f < src.y && src.y < srcHeight - 0.75f) {
                        // Interpolate using the 4 enclosing
                        // grid-points. Those points can be
                        // obtained using floor and ceiling of the
                        // exact coordinates of the point.
                        util::i32 x1;
                        util::i32 x2;
                        if (0 < src.x && src.x < srcWidth - 1) {
                            x1 = util::ROUND (floor (src.x));
                            x2 = util::ROUND (ceil (src.x));
                        }
                        else {
                            // x is near one of the borders (0 or width - 1)
                            x1 =
                            x2 = util::ROUND (src.x);
                        }
                        util::i32 y1;
                        util::i32 y2;
                        if (0 < src.y && src.y < srcHeight - 1) {
                            y1 = util::ROUND (floor (src.y));
                            y2 = util::ROUND (ceil (src.y));
                        }
                        else {
                            y1 =
                            y2 = util::ROUND (src.y);
                        }
                        // Get four points and the distances (square
                        // of the distance, for efficiency reasons)
                        // for the interpolation formula.
                        const util::f32 d1 =
                            (src.x - x1) * (src.x - x1) + (src.y - y1) * (src.y - y1);
                        const util::f32 d2 =
                            (src.x - x2) * (src.x - x2) + (src.y - y1) * (src.y - y1);
                        const util::f32 d3 =
                            (src.x - x2) * (src.x - x2) + (src.y - y2) * (src.y - y2);
                        const util::f32 d4 =
                            (src.x - x1) * (src.x - x1) + (src.y - y2) * (src.y - y2);
                        // Now interpolate as a weighted average
                        // of the four surrounding points, where
                        // the weights are the distances to each
                        // of those points If the point is exactly
                        // at one point of the grid of the source
                        // image, then don't interpolate -- just
                        // assign the pixel d1, d2, d3, d4 are
                        // positive -- no need for abs ()
                        if (d1 < util::EPSILON) {
                            memcpy (dstRow, data + y1 * rowStride + x1 * pixelStride, pixelStride);
                            dstRow += pixelStride;
                        }
                        else if (d2 < util::EPSILON) {
                            memcpy (dstRow, data + y1 * rowStride + x2 * pixelStride, pixelStride);
                            dstRow += pixelStride;
                        }
                        else if (d3 < util::EPSILON) {
                            memcpy (dstRow, data + y2 * rowStride + x2 * pixelStride, pixelStride);
                            dstRow += pixelStride;
                        }
                        else if (d4 < util::EPSILON) {
                            memcpy (dstRow, data + y2 * rowStride + x1 * pixelStride, pixelStride);
                            dstRow += pixelStride;
                        }
                        else {
                            util::ui8 *v1 = data + y1 * rowStride + x1 * pixelStride;
                            util::ui8 *v2 = data + y1 * rowStride + x2 * pixelStride;
                            util::ui8 *v3 = data + y2 * rowStride + x2 * pixelStride;
                            util::ui8 *v4 = data + y2 * rowStride + x1 * pixelStride;
                            // Weights for the weighted average
                            // are proportional to the inverse of
                            // the distance.
                            const util::f32 w1 = 1.0f / d1;
                            const util::f32 w2 = 1.0f / d2;
                            const util::f32 w3 = 1.0f / d3;
                            const util::f32 w4 = 1.0f / d4;
                            const util::f32 w = w1 + w2 + w3 + w4;
                            for (util::ui32 i = 0; i < pixelStride; ++i) {
                                *dstRow++ = (util::ui8)
                                    ((w1 * *v1++ + w2 * *v2++ + w3 * *v3++ + w4 * *v4++) / w);
                            }
                        }
                    }
                    else {
                        memcpy (dstRow, &fillPixel[0], pixelStride);
                        dstRow += pixelStride;
                    }
                }
                dstData += dstRowStride;
            }
            return dst;
        }

        std::string RGBImage::AxisTostring (Axis axis) {
            return axis == X ? "X" :
                axis == Y ? "Y" : "UnknownAxis";
        }

        RGBImage::Axis RGBImage::stringToAxis (const std::string &axis) {
            return axis == "X" ? X :
                axis == "Y" ? Y : UnknownAxis;
        }

        RGBImage::UniquePtr RGBImage::Mirror (Axis axis) const {
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

        RGBImage::UniquePtr RGBImage::Copy (const Rectangle &rectangle) const {
            assert (IsValid ());
            UniquePtr dst;
            Rectangle srcRectangle = rectangle.Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate ()) {
                dst.reset (
                    new RGBImage (srcRectangle.extents, componentIndices, pixelStride));
                if (dst.get () != 0) {
                    const util::ui8 *srcData = data +
                        srcRectangle.origin.y * rowStride +
                        srcRectangle.origin.x * pixelStride;
                    util::ui8 *dstData = dst->data;
                    util::ui32 dstRowStride = dst->rowStride;
                    util::ui32 width = dst->extents.width * dst->pixelStride;
                    util::ui32 height = dst->extents.height;
                    while (height-- != 0) {
                        memcpy (dstData, srcData, width);
                        srcData += rowStride;
                        dstData += dstRowStride;
                    }
                }
            }
            return dst;
        }

        void RGBImage::Copy (
                const Rectangle &rectangle,
                const Point &origin,
                RGBImage &dst,
                bool hasAlpha) const {
            assert (IsValid ());
            Rectangle srcRectangle = rectangle.Intersection (GetRectangle ());
            if (!srcRectangle.IsDegenerate ()) {
                Point offset = srcRectangle.origin - rectangle.origin;
                Rectangle dstRectangle =
                    Rectangle (origin + offset, rectangle.extents - offset).Intersection (
                        dst.GetRectangle ());
                if (!dstRectangle.IsDegenerate ()) {
                    const util::ui8 *srcData = data +
                        srcRectangle.origin.y * rowStride +
                        srcRectangle.origin.x * pixelStride;
                    util::ui8 *dstData = dst.data +
                        dstRectangle.origin.y * dst.rowStride +
                        dstRectangle.origin.x * dst.pixelStride;
                    util::ui32 height = std::min (
                        srcRectangle.extents.height, dstRectangle.extents.height);
                    if (!hasAlpha && componentIndices == dst.componentIndices &&
                            pixelStride == dst.pixelStride) {
                        const util::ui32 width =
                            std::min (srcRectangle.extents.width,
                                dstRectangle.extents.width) * dst.pixelStride;
                        const util::ui32 dstRowStride = dst.rowStride;
                        while (height-- != 0) {
                            memcpy (dstData, srcData, width);
                            srcData += rowStride;
                            dstData += dstRowStride;
                        }
                    }
                    else {
                        util::ui32 width = std::min (
                            srcRectangle.extents.width, dstRectangle.extents.width);
                        if (hasAlpha) {
                            util::ui32 srcRIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, R_INDEX);
                            util::ui32 srcGIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, G_INDEX);
                            util::ui32 srcBIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, B_INDEX);
                            util::ui32 srcAIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, A_INDEX);
                            util::ui32 dstRIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, R_INDEX);
                            util::ui32 dstGIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, G_INDEX);
                            util::ui32 dstBIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, B_INDEX);
                            util::ui32 dstAIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, A_INDEX);
                            const util::ui32 dstPixelStride = dst.pixelStride;
                            const util::ui32 dstRowStride = dst.rowStride;
                            while (height-- != 0) {
                                const util::ui8 *srcRow = srcData;
                                util::ui8 *dstRow = dstData;
                                for (util::ui32 i = 0; i < width; ++i) {
                                    util::ui32 srcR = srcRow[srcRIndex];
                                    util::ui32 srcG = srcRow[srcGIndex];
                                    util::ui32 srcB = srcRow[srcBIndex];
                                    util::ui32 srcA = srcRow[srcAIndex];
                                    util::ui32 dstR = dstRow[dstRIndex];
                                    util::ui32 dstG = dstRow[dstGIndex];
                                    util::ui32 dstB = dstRow[dstBIndex];
                                    util::ui32 dstA = dstRow[dstAIndex];
                                    util::ui32 inverseSrcA = 255 - srcA;
                                    dstRow[dstRIndex] =
                                        (util::ui8)((srcA * srcR + dstR * inverseSrcA) / 255);
                                    dstRow[dstGIndex] =
                                        (util::ui8)((srcA * srcG + dstG * inverseSrcA) / 255);
                                    dstRow[dstBIndex] =
                                        (util::ui8)((srcA * srcB + dstB * inverseSrcA) / 255);
                                    dstRow[dstAIndex] =
                                        (util::ui8)((srcA * srcA + dstA * inverseSrcA) / 255);
                                    srcRow += pixelStride;
                                    dstRow += dstPixelStride;
                                }
                                srcData += rowStride;
                                dstData += dstRowStride;
                            }
                        }
                        else {
                            util::ui32 srcRIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, R_INDEX);
                            util::ui32 srcGIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, G_INDEX);
                            util::ui32 srcBIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    componentIndices, B_INDEX);
                            util::ui32 dstRIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, R_INDEX);
                            util::ui32 dstGIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, G_INDEX);
                            util::ui32 dstBIndex =
                                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                                    dst.componentIndices, B_INDEX);
                            const util::ui32 dstPixelStride = dst.pixelStride;
                            const util::ui32 dstRowStride = dst.rowStride;
                            while (height-- != 0) {
                                const util::ui8 *srcRow = srcData;
                                util::ui8 *dstRow = dstData;
                                for (util::ui32 i = 0; i < width; ++i) {
                                    dstRow[dstRIndex] = srcRow[srcRIndex];
                                    dstRow[dstGIndex] = srcRow[srcGIndex];
                                    dstRow[dstBIndex] = srcRow[srcBIndex];
                                    srcRow += pixelStride;
                                    dstRow += dstPixelStride;
                                }
                                srcData += rowStride;
                                dstData += dstRowStride;
                            }
                        }
                    }
                }
            }
        }

        void RGBImage::ToJPG (
                std::vector<util::ui8> &buffer,
                util::ui32 quality,
                Chroma chroma) const {
            buffer.resize (1024 + // min jpeg
                extents.height * extents.width * pixelStride);
            TJCompressHandle  handle;
            util::ui8 *ptr = &buffer[0];
            unsigned long size = buffer.size ();
            if (tjCompress2 (handle.handle, data,
                    extents.width, rowStride, extents.height,
                    ComponentIndicesToTJPixelFormat (
                        componentIndices, pixelStride),
                    &ptr, &size, ChromaToTJChroma (chroma),
                    quality, TJFLAG_NOREALLOC) == 0) {
                buffer.resize (size);
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "%s", tjGetErrorStr ());
            }
        }

        void RGBImage::SaveJPG (
                const std::string &path,
                util::ui32 quality,
                Chroma chroma) const {
            std::vector<util::ui8> buffer;
            ToJPG (buffer, quality, chroma);
            util::File file (util::HostEndian, path);
            file.Write (&buffer[0], buffer.size ());
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

        YUVImage::UniquePtr RGBImage::ToYUVImage (bool hasAlpha) const {
            assert (IsValid ());
            YUVImage::UniquePtr image (new YUVImage (extents, hasAlpha));
            if (image.get () != 0) {
                util::ui32 rIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, R_INDEX);
                util::ui32 gIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, G_INDEX);
                util::ui32 bIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, B_INDEX);
                util::ui32 aIndex =
                    THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, A_INDEX);
                util::ui32 width = extents.width;
                util::ui32 height = extents.height;
                util::ui32 planeSize = width * height;
                const util::ui8 *srcData = data;
                util::ui8 *yPlane = image->GetYPlane ();
                util::ui8 *uPlane = image->GetUPlane ();
                util::ui8 *vPlane = image->GetVPlane ();
                util::ui8 *aPlane = image->GetAPlane ();
                util::Array<util::ui8> u (planeSize);
                util::Array<util::ui8> v (planeSize);
                // YA pass.
                for (util::ui32 y = 0, i = 0; y < height; ++y) {
                    for (util::ui32 x = 0; x < width; ++x, ++i) {
                        util::ui32 pixelIndex = x * pixelStride;
                        util::ui32 r = srcData[pixelIndex + rIndex];
                        util::ui32 g = srcData[pixelIndex + gIndex];
                        util::ui32 b = srcData[pixelIndex + bIndex];
                        yPlane[i] = ((RY * r + GY * g + BY * b) >> RGB2YUV_SHIFT) + 16;
                        if (hasAlpha) {
                            aPlane[i] = srcData[pixelIndex + aIndex];
                        }
                        u[i] = ((RU * r + GU * g + BU * b) >> RGB2YUV_SHIFT) + 128;
                        v[i] = ((RV * r + GV * g + BV * b) >> RGB2YUV_SHIFT) + 128;
                    }
                    srcData += rowStride;
                }
                // UV pass, 4 x 4 downsampling.
                for (util::ui32 y = 0, i = 0; y < height; y += 2) {
                    for (util::ui32 x = 0; x < width; x += 2, ++i) {
                        util::ui32 pixelIndex0 = y * width + x;
                        util::ui32 pixelIndex1 = pixelIndex0 + 1;
                        util::ui32 pixelIndex2 = pixelIndex0 + width;
                        util::ui32 pixelIndex3 = pixelIndex1 + width;
                        uPlane[i] =
                            (u[pixelIndex0] + u[pixelIndex1] +
                            u[pixelIndex2] + u[pixelIndex3]) >> 2;
                        vPlane[i] =
                            (v[pixelIndex0] + v[pixelIndex1] +
                            v[pixelIndex2] + v[pixelIndex3]) >> 2;
                    }
                }
            }
            return image;
        }

        RGBImage &RGBImage::operator = (const RGBImage &image) {
            if (&image != this) {
                Release ();
                data = image.data;
                extents = image.extents;
                componentIndices = image.componentIndices;
                pixelStride = image.pixelStride;
                rowStride = image. rowStride;
                owner = false;
            }
            return *this;
        }

        void RGBImage::ColorToPixel (const Color &color,
                std::vector<util::ui8> &pixel) const {
            pixel.resize (pixelStride);
            memset (&pixel[0], 0, pixelStride);
            util::ui32 rIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, R_INDEX);
            if (rIndex < pixelStride) {
                pixel[rIndex] = color.r;
            }
            util::ui32 gIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, G_INDEX);
            if (gIndex < pixelStride) {
                pixel[gIndex] = color.g;
            }
            util::ui32 bIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, B_INDEX);
            if (bIndex < pixelStride) {
                pixel[bIndex] = color.b;
            }
            util::ui32 aIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (componentIndices, A_INDEX);
            if (aIndex < pixelStride) {
                pixel[aIndex] = color.a;
            }
        }

        void RGBImage::Release () {
            if (owner) {
                if (data != 0) {
                    delete [] data;
                    data = 0;
                }
                owner = false;
            }
        }

        RGBImage::UniquePtr RGBImage::MirrorX () const {
            assert (IsValid ());
            UniquePtr dst (
                new RGBImage (extents, componentIndices, pixelStride));
            if (dst.get () != 0) {
                util::ui32 width = extents.width;
                util::ui32 height = extents.height;
                const util::ui8 *srcData = data;
                util::ui32 dstRowStride = dst->rowStride;
                for (util::ui32 i = 0; i < height; ++i) {
                    util::ui8 *dstData =
                        dst->data + (height - i - 1) * dstRowStride;
                    memcpy (dstData, srcData, dstRowStride);
                    srcData += rowStride;
                }
            }
            return dst;
        }

        RGBImage::UniquePtr RGBImage::MirrorY () const {
            assert (IsValid ());
            UniquePtr dst (
                new RGBImage (extents, componentIndices, pixelStride));
            if (dst.get () != 0) {
                util::ui32 width = extents.width;
                util::ui32 height = extents.height;
                const util::ui8 *srcData = data;
                for (util::ui32 j = 0; j < height; ++j) {
                    util::ui8 *dstData =
                        dst->data + (j + 1) * dst->rowStride - pixelStride;
                    for (util::ui32 i = 0; i < width; ++i) {
                        memcpy (dstData, srcData, pixelStride);
                        srcData += pixelStride;
                        dstData -= pixelStride;
                    }
                }
            }
            return dst;
        }

    } // namespace canvas
} // namespace thekogans

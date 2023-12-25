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

#if !defined (__thekogans_canvas_RGBImage_h)
#define __thekogans_canvas_RGBImage_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"

namespace thekogans {
    namespace canvas {

        struct YUVImage;

        struct _LIB_THEKOGANS_CANVAS_DECL RGBImage {
            THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (RGBImage, util::SpinLock)

            typedef std::unique_ptr<RGBImage> UniquePtr;

            enum {
                R_INDEX = 0,
                G_INDEX = 1,
                B_INDEX = 2,
                A_INDEX = 3
            };
            enum {
                R0G1B2A3 = 0x00010203,
                R2G1B0A3 = 0x02010003,
                R1G2B3A0 = 0x01020300,
                R3G2B1A0 = 0x03020100
            };
            static bool IsValidComponentIndices (util::ui32 componentIndices) {
                return
                    componentIndices == R0G1B2A3 ||
                    componentIndices == R2G1B0A3 ||
                    componentIndices == R1G2B3A0 ||
                    componentIndices == R3G2B1A0;
            }
            static std::string ComponentIndicesTostring (util::ui32 componentIndices);
            static util::ui32 stringToComponentIndices (const std::string &componentIndices);

        protected:
            // VERY, VERY IMPORTANT: RGBImage implements a shallow
            // copy without reference counting. Semantically, what
            // this means is, the 'owner' image must be the last one
            // to go out of scope or the 'copies' will be left with a
            // dangling pointer. To aid with that, use Swap liberally
            // for very inexpensive ownership transfer.
            util::ui8 *data;
            util::Rectangle::Extents extents;
            util::ui32 componentIndices;
            util::ui32 pixelStride;
            util::ui32 rowStride;
            bool owner;

        public:
            RGBImage () :
                data (0),
                componentIndices (R0G1B2A3),
                pixelStride (0),
                rowStride (0),
                owner (false) {}
            RGBImage (
                const util::Rectangle::Extents &extents_,
                util::ui32 componentIndices_ = R0G1B2A3,
                util::ui32 pixelStride_ = 4,
                util::ui32 rowStride_ = 0,
                bool clear = false);
            RGBImage (
                    util::ui8 *data_,
                    const util::Rectangle::Extents &extents_,
                    util::ui32 componentIndices_,
                    util::ui32 pixelStride_,
                    util::ui32 rowStride_,
                    bool owner_ = true) :
                    data (data_),
                    extents (extents_),
                    componentIndices (componentIndices_),
                    pixelStride (pixelStride_),
                    rowStride (rowStride_ > 0 ?
                        rowStride_ : extents_.width * pixelStride_),
                    owner (owner_) {
                assert (rowStride >= extents.width * pixelStride);
            }
            RGBImage (const RGBImage &image) :
                data (image.data),
                extents (image.extents),
                componentIndices (image.componentIndices),
                pixelStride (image.pixelStride),
                rowStride (image.rowStride),
                owner (false) {}
            virtual ~RGBImage () {
                Release ();
            }

            inline util::ui8 *GetData () const {
                return data;
            }
            inline util::ui32 GetSize () const {
                // IMPORTANT: It's important that we use rowStride
                // here because this image might be a sub-image of
                // another (larger) image. If you are using GetSize
                // to stream the image data keep in mind that it
                // will work just fine though you may also be
                // streaming part of the parent image.
                return extents.height * rowStride;
            }
            inline const util::Rectangle::Extents &GetExtents () const {
                return extents;
            }
            inline util::Rectangle GetRectangle () const {
                return util::Rectangle (util::Point (), extents);
            }
            inline util::ui32 GetComponentIndices () const {
                return componentIndices;
            }
            inline util::ui32 GetPixelStride () const {
                return pixelStride;
            }
            inline util::ui32 GetRowStride () const {
                return rowStride;
            }

            inline void Swap (RGBImage &image) {
                std::swap (data, image.data);
                std::swap (extents, image.extents);
                std::swap (componentIndices, image.componentIndices);
                std::swap (pixelStride, image.pixelStride);
                std::swap (rowStride, image.rowStride);
                std::swap (owner, image.owner);
            }

            static UniquePtr FromPNGBuffer (
                const util::ui8 *buffer,
                util::ui32 size,
                util::ui32 componentIndices);
            static UniquePtr FromPNGFile (
                const std::string &path,
                util::ui32 componentIndices);
            static UniquePtr FromJPGBuffer (
                const util::ui8 *buffer,
                util::ui32 size,
                util::ui32 componentIndices);
            static UniquePtr FromJPGFile (
                const std::string &path,
                util::ui32 componentIndices);
            static UniquePtr FromBMPBuffer (
                const util::ui8 *buffer,
                util::ui32 size,
                util::ui32 componentIndices);
            static UniquePtr FromBMPFile (
                const std::string &path,
                util::ui32 componentIndices);

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
            void Scale (
                RGBImage &dst,
                Filter filter) const;

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
                RGBImage &dst,
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
                RGBImage &dst,
                bool hasAlpha = false) const;

            enum Chroma {
                CHROMA_NONE,
                CHROMA_2x1,
                CHROMA_1x2,
                CHROMA_2x2
            };
            void ToJPG (
                std::vector<util::ui8> &buffer,
                util::ui32 quality = 85,
                Chroma chroma = CHROMA_2x2) const;
            void SaveJPG (
                const std::string &path,
                util::ui32 quality = 85,
                Chroma chroma = CHROMA_2x2) const;

            std::unique_ptr<YUVImage> ToYUVImage (bool hasAlpha = false) const;

            RGBImage &operator = (const RGBImage &image);

            inline bool IsValid () const {
                return data != 0 &&
                    !extents.IsDegenerate () &&
                    IsValidComponentIndices (componentIndices) &&
                    pixelStride >= 3 &&
                    rowStride >= extents.width * pixelStride;
            }
            inline bool CompareGeometry (const RGBImage &image) const {
                return extents == image.extents &&
                    componentIndices == image.componentIndices &&
                    pixelStride == image.pixelStride &&
                    rowStride == image.rowStride;
            }

            void ColorToPixel (
                const Color &color,
                std::vector<util::ui8> &pixel) const;

        protected:
            void Release ();

        private:
            UniquePtr MirrorX () const;
            UniquePtr MirrorY () const;
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_RGBImage_h)

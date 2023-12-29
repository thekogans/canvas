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

#include "thekogans/util/Heap.h"
#include "thekogans/util/File.h"
#include "thekogans/canvas/RGBAFramebuffer.h"
#include "thekogans/canvas/XYZAFramebuffer.h"
#include "thekogans/canvas/ComponentConverter.h"
#include "thekogans/canvas/lodepng.h"
#include "thekogans/canvas/TJUtils.h"

namespace thekogans {
    namespace canvas {

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32RGBAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64RGBAFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32BGRAFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64BGRAFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ARGBFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ARGBFramebuffer, util::SpinLock)

        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui8ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui16ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui32ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (ui64ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f32ABGRFramebuffer, util::SpinLock)
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK_T (f64ABGRFramebuffer, util::SpinLock)

        void foo () {
            ui8RGBAFramebuffer::SharedPtr fb1 (new ui8RGBAFramebuffer (util::Rectangle::Extents (10, 10)));
            f32XYZAFramebuffer::SharedPtr fb2 = fb1->Convert<f32XYZAPixel> ();
            f32RGBAFramebuffer::SharedPtr fb3 = fb2->Convert<f32RGBAPixel> ();
            ui8RGBAFramebuffer::SharedPtr fb4 = fb3->Convert<
                ui8RGBAPixel,
                ComponentConverter<util::f32, util::ui8>> ();
        }

        ui8RGBAFramebuffer::SharedPtr FromPNGBuffer (
                const util::ui8 *buffer,
                std::size_t size) {
            std::vector<util::ui8> data;
            util::ui32 width = 0;
            util::ui32 height = 0;
            util::ui32 error = lodepng::decode (data, width, height, buffer, size);
            if (error == 0) {
                ui8RGBAFramebuffer::SharedPtr framebuffer (
                    new ui8RGBAFramebuffer (util::Rectangle::Extents (width, height)));
                ui8RGBAPixel *dst = framebuffer->buffer.array;
                const util::ui8 *src = &data[0];
                for (std::size_t length = framebuffer->buffer.length; length-- != 0;) {
                    util::ui8 r = *src++;
                    util::ui8 g = *src++;
                    util::ui8 b = *src++;
                    util::ui8 a = *src++;
                    *dst++ = ui8RGBAPixel (ui8RGBAColor (r, g, b, a));
                }
                return framebuffer;
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to load a PNG image from buffer (%s)",
                    lodepng_error_text (error));
            }
        }

        ui8RGBAFramebuffer::SharedPtr FromPNGFile (const std::string &path) {
            std::vector<util::ui8> data;
            util::ui32 width = 0;
            util::ui32 height = 0;
            util::ui32 error = lodepng::decode (data, width, height, path.c_str ());
            if (error == 0) {
                ui8RGBAFramebuffer::SharedPtr framebuffer (
                    new ui8RGBAFramebuffer (util::Rectangle::Extents (width, height)));
                ui8RGBAPixel *dst = framebuffer->buffer.array;
                const util::ui8 *src = &data[0];
                for (std::size_t length = framebuffer->buffer.length; length-- != 0;) {
                    util::ui8 r = *src++;
                    util::ui8 g = *src++;
                    util::ui8 b = *src++;
                    util::ui8 a = *src++;
                    *dst++ = ui8RGBAPixel (ui8RGBAColor (r, g, b, a));
                }
                return framebuffer;
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unable to load a PNG image from %s (%s)",
                    path.c_str (), lodepng_error_text (error));
            }
        }

        ui8RGBAFramebuffer::SharedPtr FromJPGBuffer (
                const util::ui8 *buffer,
                std::size_t size) {
            TJDecompressHandle handle;
            int width;
            int height;
            int jpegSubsamp;
            if (tjDecompressHeader2 (handle.handle, (util::ui8 *)buffer,
                    size, &width, &height, &jpegSubsamp) == 0) {
                ui8RGBAFramebuffer::SharedPtr framebuffer (
                    new ui8RGBAFramebuffer (util::Rectangle::Extents (width, height)));
                ui8RGBAPixel *dst = framebuffer->buffer.array;
                util::Array<util::ui8> data (width * height * 4);
                if (tjDecompress2 (handle.handle, (util::ui8 *)buffer,
                        size, data, width, width * 4, height, TJPF_RGBX, 0) == 0) {
                    const util::ui8 *src = data.array;
                    for (std::size_t length = framebuffer->buffer.length; length-- != 0;) {
                        util::ui8 r = *src++;
                        util::ui8 g = *src++;
                        util::ui8 b = *src++;
                        util::ui8 a = *src++;
                        *dst++ = ui8RGBAPixel (ui8RGBAColor (r, g, b, a));
                    }
                return framebuffer;
                }
                else {
                    THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                        "%s", tjGetErrorStr ());
                }
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "%s", tjGetErrorStr ());
            }
        }

        ui8RGBAFramebuffer::SharedPtr FromJPGFile (const std::string &path) {
            util::ReadOnlyFile file (util::HostEndian, path);
            util::ui64 size = file.GetSize ();
            if (size > 0) {
                std::vector<util::ui8> buffer (size);
                file.Read (buffer.data (), size);
                return FromJPGBuffer (buffer.data (), size);
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Empty jpg file: %s", path.c_str ());
            }
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

        ui8RGBAFramebuffer::SharedPtr FromBMPBuffer (
                const util::ui8 *buffer,
                std::size_t size) {
            util::TenantReadBuffer buffer_ (util::LittleEndian, buffer, size);
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
                ui8RGBAFramebuffer::SharedPtr framebuffer (
                    new ui8RGBAFramebuffer (
                        util::Rectangle::Extents (infoHeader.biHeight, infoHeader.biWidth)));
                ui8RGBAPixel *dst = framebuffer->buffer.array;
                buffer_.readOffset = fileHeader.bfOffBits;
                for (std::size_t length = framebuffer->buffer.length; length-- != 0;) {
                    util::ui8 r;
                    util::ui8 g;
                    util::ui8 b;
                    util::ui8 a;
                    buffer_ >> r >> g >> b;
                    if (infoHeader.biBitCount == 32) {
                        buffer_ >> a;
                    }
                    else {
                        a = 255;
                    }
                    *dst++ = ui8RGBAPixel (ui8RGBAColor (r, g, b, a));
                }
                return framebuffer;
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Unsupported bmp file format: %d", fileHeader.bfType);
            }
        }

        ui8RGBAFramebuffer::SharedPtr FromBMPFile (const std::string &path) {
            util::ReadOnlyFile file (util::HostEndian, path);
            util::ui64 size = file.GetSize ();
            if (size > 0) {
                std::vector<util::ui8> buffer (size);
                file.Read (buffer.data (), size);
                return FromBMPBuffer (buffer.data (), size);
            }
            else {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "Empty bmp file: %s", path.c_str ());
            }
        }

    } // namespace canvas
} // namespace thekogans

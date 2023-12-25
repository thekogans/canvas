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

#if !defined (__thekogans_canvas_Font_h)
#define __thekogans_canvas_Font_h

#include <string>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "thekogans/util/Types.h"
#include "thekogans/util/Point.h"
#include "thekogans/util/Rectangle.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/util/LockGuard.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/LoggerMgr.h"
#include "thekogans/canvas/Config.h"
#include "thekogans/canvas/Color.h"
//#include "thekogans/canvas/RGBImage.h"

namespace thekogans {
    namespace canvas {

        const char *GetErrorMessage (FT_Error error);

        struct _LIB_THEKOGANS_CANVAS_DECL Font {
            static util::SpinLock freetypeLock;


            struct LibraryPtr {
                FT_Library library;
                LibraryPtr () :
                        library (0) {
                    util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                    FT_Error error = FT_Init_FreeType (&library);
                    if (error != 0) {
                        THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                            "FT_Init_FreeType: %s", GetErrorMessage (error));
                    }
                }
                ~LibraryPtr () {
                    util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                    FT_Error error = FT_Done_FreeType (library);
                    if (error != 0) {
                        THEKOGANS_UTIL_LOG_ERROR (
                            "FT_Done_FreeType: %s\n", GetErrorMessage (error));
                    }
                }
            } library;
            struct FacePtr {
                FT_Face face;
                FacePtr (
                        FT_Library library,
                        const std::string &path) :
                        face (0) {
                    util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                    FT_Error error = FT_New_Face (library, path.c_str (), 0, &face);
                    if (error != 0) {
                        THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                            "FT_New_Face (%s): %s", path.c_str (), GetErrorMessage (error));
                    }
                }
                ~FacePtr () {
                    util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                    FT_Error error = FT_Done_Face (face);
                    if (error != 0) {
                        THEKOGANS_UTIL_LOG_ERROR (
                            "FT_Done_Face: %s\n", GetErrorMessage (error));
                    }
                }
            } face;
            struct Glyph {
                THEKOGANS_UTIL_DECLARE_HEAP_WITH_LOCK (Glyph, util::SpinLock)

                FT_UInt index;
                struct GlyphPtr {
                    FT_Glyph glyph;
                    GlyphPtr (
                            FT_Face face,
                            FT_UInt index) :
                            glyph (0) {
                        util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                        FT_Error error = FT_Load_Glyph (face, index, FT_LOAD_DEFAULT);
                        if (error != 0) {
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                                "FT_Load_Glyph: %s", GetErrorMessage (error));
                        }
                        error = FT_Get_Glyph (face->glyph, &glyph);
                        if (error != 0) {
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                                "FT_Get_Glyph: %s", GetErrorMessage (error));
                        }
                    }
                    ~GlyphPtr () {
                        util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                        FT_Done_Glyph (glyph);
                    }

                    inline const FT_BitmapGlyph operator -> () const {
                        return (FT_BitmapGlyph)glyph;
                    }
                } glyph;
                util::Rectangle::Extents size;
                util::Point bearing;
                util::Point advance;

                Glyph (
                        FT_Face face,
                        char ch,
                        bool flipRows) :
                        index (FT_Get_Char_Index (face, ch)),
                        glyph (face, index) {
                    {
                        util::LockGuard<util::SpinLock> guard (Font::freetypeLock);
                        FT_Error error = FT_Glyph_To_Bitmap (&glyph.glyph, FT_RENDER_MODE_NORMAL, 0, 1);
                        if (error != 0) {
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                                "FT_Glyph_To_Bitmap: %s", GetErrorMessage (error));
                        }
                    }
                    size.width = glyph->bitmap.width;
                    size.height = glyph->bitmap.rows;
                    bearing.x = glyph->left;
                    bearing.y = glyph->top;
                    // advance.x = glyph->advance.x;
                    // advance.y = glyph->advance.y;
                    if (flipRows) {
                        util::ui8 *topRow = glyph->bitmap.buffer;
                        util::ui8 *bottomRow = topRow + (glyph->bitmap.rows - 1) * glyph->bitmap.pitch;
                        std::vector<util::ui8> temp (glyph->bitmap.width);
                        for (std::size_t count = glyph->bitmap.rows / 2; count-- != 0;) {
                            memcpy (&temp[0], topRow, glyph->bitmap.width);
                            memcpy (topRow, bottomRow, glyph->bitmap.width);
                            memcpy (bottomRow, &temp[0], glyph->bitmap.width);
                            topRow += glyph->bitmap.pitch;
                            bottomRow -= glyph->bitmap.pitch;
                        }
                    }
                }

                inline const util::ui8 *GetData () const {
                    return glyph->bitmap.buffer;
                }
                inline util::Rectangle::Extents GetExtents () const {
                    return util::Rectangle::Extents (glyph->bitmap.width, glyph->bitmap.rows);
                }
                inline util::i32 GetPixelStride () const {
                    return 1;
                }
                inline util::i32 GetRowStride () const {
                    return glyph->bitmap.pitch;
                }
                inline util::i32 GetLeftSideBearing () const {
                    return glyph->left;
                }
                inline util::i32 GetTopSideBearing () const {
                    return glyph->top;
                }

                THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Glyph)
            };
            util::OwnerVector<Glyph> glyphCache;
            bool flipGliphRows;

            // NOTE: width and height are in points.
            // horizontalResolution and verticalResolution are in dpi.
            Font (
                const std::string &path,
                util::ui32 width,
                util::ui32 height,
                util::ui32 horizontalResolution = 96,
                util::ui32 verticalResolution = 96,
                bool flipGliphRows_ = false);

            inline util::ui32 GetHeight () const {
                return (util::ui32)(face.face->size->metrics.height >> 6);
            }
            inline util::ui32 GetAveCharWidth () const {
                return (util::ui32)(face.face->size->metrics.max_advance >> 6);
            }

            enum Alignment {
                UnknownAlignment,
                TL, // TopLeft
                TC, // TopCenter
                TR, // TopRight
                CL, // CenterLeft
                CC, // CenterCenter
                CR, // CenterRight
                BL, // BottomLeft
                BC, // BottomCenter
                BR  // BottomRight
            };
            static std::string AlignmentTostring (Alignment alignment);
            static Alignment stringToAlignment (const std::string &alignment);

            util::Point AlignText (
                const std::string &text,
                const util::Rectangle &rectangle,
                Alignment alignment);
            util::Rectangle::Extents GetTextExtents (
                const std::string &text);
#if 0
            void DrawText (
                RGBImage &image,
                const std::string &text,
                const util::Rectangle &rectangle,
                const Color &textColor = Color (255, 255, 255),
                const Color &backgroundColor = Color (0, 0, 0, 255));
            void DrawText (
                const std::string &text,
                const util::Rectangle &rectangle,
                const Color &textColor = Color (255, 255, 255),
                const Color &backgroundColor = Color (0, 0, 0, 255));
#endif

            void GetGlyphs (
                const std::string &text,
                std::vector<Glyph *> &glyphs,
                std::vector<FT_Vector> &positions);

            THEKOGANS_CANVAS_DISALLOW_COPY_AND_ASSIGN (Font)
        };

    } // namespace canvas
} // namespace thekogans

#endif // !defined (__thekogans_canvas_Font_h)

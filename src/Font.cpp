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
#include "thekogans/util/SpinLock.h"
#include "thekogans/canvas/Font.h"

namespace thekogans {
    namespace canvas {

        const char *GetErrorMessage (FT_Error error) {
            #undef __FTERRORS_H__
            #define FT_ERRORDEF(e, v, s) case e: return s;
            #define FT_ERROR_START_LIST switch (error) {
            #define FT_ERROR_END_LIST }
            #include FT_ERRORS_H
            return "(Unknown error)";
        }

        util::SpinLock Font::freetypeLock;

        typedef Font::Glyph FontGlyph;
        THEKOGANS_UTIL_IMPLEMENT_HEAP_WITH_LOCK (FontGlyph, util::SpinLock)

        Font::Font (
                const std::string &path,
                util::ui32 width,
                util::ui32 height,
                util::ui32 horizontalResolution,
                util::ui32 verticalResolution) :
                face (library.library, path),
                glyphCache (256) {
            FT_Error error = FT_Set_Char_Size (face.face, width << 6, height << 6,
                horizontalResolution, verticalResolution);
            if (error != 0) {
                THEKOGANS_UTIL_THROW_STRING_EXCEPTION (
                    "FT_Set_Char_Size (%s): %s", path.c_str (), GetErrorMessage (error));
            }
        }

        std::string Font::AlignmentTostring (Alignment alignment) {
            return alignment == TL ? "TL" :
                alignment == TC ? "TC" :
                alignment == TR ? "TR" :
                alignment == CL ? "CL" :
                alignment == CC ? "CC" :
                alignment == CR ? "CR" :
                alignment == BL ? "BL" :
                alignment == BC ? "BC" :
                alignment == BR ? "BR" : "UnknownAlignment";
        }

        Font::Alignment Font::stringToAlignment (const std::string &alignment) {
            return alignment == "TL" ? TL :
                alignment == "TC" ? TC :
                alignment == "TR" ? TR :
                alignment == "CL" ? CL :
                alignment == "CC" ? CC :
                alignment == "CR" ? CR :
                alignment == "BL" ? BL :
                alignment == "BC" ? BC :
                alignment == "BR" ? BR : UnknownAlignment;
        }

        Point Font::AlignText (
                const std::string &text,
                const Rectangle &rectangle,
                Alignment alignment) {
            switch (alignment) {
                case UnknownAlignment:
                    break;
                case TL:
                    return rectangle.origin;
                case TC: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (
                        (rectangle.extents.width - textExtents.width) / 2, 0);
                }
                case TR: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (
                        rectangle.extents.width - textExtents.width, 0);
                }
                case CL: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (0,
                        (rectangle.extents.height - textExtents.height) / 2);
                }
                case CC: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (
                        (rectangle.extents.width - textExtents.width) / 2,
                        (rectangle.extents.height - textExtents.height) / 2);
                }
                case CR: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (
                        rectangle.extents.width - textExtents.width,
                        (rectangle.extents.height - textExtents.height) / 2);
                }
                case BL: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (0,
                        rectangle.extents.height - textExtents.height);
                }
                case BC: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (
                        (rectangle.extents.width - textExtents.width) / 2,
                        rectangle.extents.height - textExtents.height);
                }
                case BR: {
                    Rectangle::Extents textExtents = GetTextExtents (text);
                    return rectangle.origin + Point (
                        rectangle.extents.width - textExtents.width,
                        rectangle.extents.height - textExtents.height);
                }
            }
            return Point ();
        }

        Rectangle::Extents Font::GetTextExtents (const std::string &text) {
            std::vector<Glyph *> glyphs;
            std::vector<FT_Vector> positions;
            GetGlyphs (text, glyphs, positions);
            Rectangle::Extents extents;
            for (std::size_t i = 0, count = glyphs.size (); i < count; ++i) {
                Rectangle::Extents glyphExtents = glyphs[i]->GetExtents ();
                glyphExtents.width += positions[i].x + glyphs[i]->GetLeftSideBearing ();
                glyphExtents.height += positions[i].y - glyphs[i]->GetTopSideBearing ();
                if (extents.width < glyphExtents.width) {
                    extents.width = glyphExtents.width;
                }
                if (extents.height < glyphExtents.height) {
                    extents.height = glyphExtents.height;
                }
            }
            return extents;
        }

        void Font::DrawText (
                RGBImage &image,
                const std::string &text,
                const Rectangle &rectangle,
                const Color &textColor,
                const Color &backgroundColor) {
            std::vector<Glyph *> glyphs;
            std::vector<FT_Vector> positions;
            GetGlyphs (text, glyphs, positions);
            FT_Int topMax = 0;
            for (std::size_t i = 0, count = glyphs.size (); i < count && positions[i].y == 0; ++i) {
                if (topMax < glyphs[i]->GetTopSideBearing ()) {
                    topMax = glyphs[i]->GetTopSideBearing ();
                }
            }
            if (backgroundColor.a < 255) {
                image.Clear (rectangle, backgroundColor);
            }
            util::ui32 imagePixelStride = image.GetPixelStride ();
            util::ui32 imageRowStride = image.GetRowStride ();
            util::ui32 imageComponentIndices = image.GetComponentIndices ();
            util::ui32 imageRIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                    imageComponentIndices, RGBImage::R_INDEX);
            util::ui32 imageGIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                    imageComponentIndices, RGBImage::G_INDEX);
            util::ui32 imageBIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                    imageComponentIndices, RGBImage::B_INDEX);
            util::ui32 imageAIndex =
                THEKOGANS_UTIL_UI32_GET_UI8_AT_INDEX (
                    imageComponentIndices, RGBImage::A_INDEX);
            for (std::size_t i = 0, count = glyphs.size (); i < count; ++i) {
                Point origin (
                    rectangle.origin.x + positions[i].x + glyphs[i]->GetLeftSideBearing (),
                    rectangle.origin.y + positions[i].y + topMax - glyphs[i]->GetTopSideBearing ());
                Rectangle rectangle =
                    Rectangle (origin, glyphs[i]->GetExtents ()).Intersection (image.GetRectangle ());
                if (!rectangle.IsDegenerate ()) {
                    util::ui8 *imageData = image.GetData () +
                        rectangle.origin.y * imageRowStride +
                        rectangle.origin.x * imagePixelStride;
                    util::ui32 glyphPixelStride = glyphs[i]->GetPixelStride ();
                    util::i32 glyphRowStride = glyphs[i]->GetRowStride ();
                    const util::ui8 *glyphData = glyphs[i]->GetData () +
                        (rectangle.origin.y - origin.y) * glyphRowStride +
                        (rectangle.origin.x - origin.x) * glyphPixelStride;
                    util::ui32 glyphHeight = rectangle.extents.height;
                    util::ui32 glyphWidth = rectangle.extents.width;
                    while (glyphHeight-- != 0) {
                        util::ui8 *imageRow = imageData;
                        const util::ui8 *glyphRow = glyphData;
                        for (util::ui32 i = 0; i < glyphWidth; ++i) {
                            if (*glyphRow == 255) {
                                imageRow[imageRIndex] = textColor.r;
                                imageRow[imageGIndex] = textColor.g;
                                imageRow[imageBIndex] = textColor.b;
                                imageRow[imageAIndex] = 255;
                            }
                            else {
                                util::ui32 glyphR = textColor.r;
                                util::ui32 glyphG = textColor.g;
                                util::ui32 glyphB = textColor.b;
                                util::ui32 glyphA = *glyphRow;
                                util::ui32 imageR = imageRow[imageRIndex];
                                util::ui32 imageG = imageRow[imageGIndex];
                                util::ui32 imageB = imageRow[imageBIndex];
                                util::ui32 imageA = imageRow[imageAIndex];
                                util::ui32 inverseGlyphA = 255 - glyphA;
                                imageRow[imageRIndex] =
                                    (util::ui8)((glyphA * glyphR + imageR * inverseGlyphA) / 255);
                                imageRow[imageGIndex] =
                                    (util::ui8)((glyphA * glyphG + imageG * inverseGlyphA) / 255);
                                imageRow[imageBIndex] =
                                    (util::ui8)((glyphA * glyphB + imageB * inverseGlyphA) / 255);
                                imageRow[imageAIndex] =
                                    (util::ui8)((glyphA * glyphA + imageA * inverseGlyphA) / 255);
                            }
                            imageRow += imagePixelStride;
                            glyphRow += glyphPixelStride;
                        }
                        imageData += imageRowStride;
                        glyphData += glyphRowStride;
                    }
                }
            }
        }

#if 0
        void Font::DrawText (
                const std::string &text,
                const Rectangle &rectangle,
                const Color &textColor,
                const Color &backgroundColor) {
            std::vector<Glyph *> glyphs;
            std::vector<FT_Vector> positions;
            GetGlyphs (text, glyphs, positions);
            FT_Int topMax = 0;
            for (std::size_t i = 0, count = glyphs.size (); i < count && positions[i].y == 0; ++i) {
                if (topMax < glyphs[i]->GetTopSideBearing ()) {
                    topMax = glyphs[i]->GetTopSideBearing ();
                }
            }
            opengl::Scissor scissor (
                blas::Size (
                    rectangle.origin.x,
                    rectangle.origin.y,
                    rectangle.extents.width,
                    rectangle.extents.height));
            if (backgroundColor.a < 255) {
                opengl::ClearColor clearColor (
                    opengl::ui8Color (
                        backgroundColor.r,
                        backgroundColor.g,
                        backgroundColor.b));
                glClear (GL_COLOR_BUFFER_BIT);
            }
            opengl::Enable blend (GL_BLEND, true);
            opengl::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            opengl::UnpackAlignemnt unpackAlignment (1);
            opengl::Color color (
                opengl::ui8Color (
                    textColor.r,
                    textColor.g,
                    textColor.b));
            for (std::size_t i = 0, count = glyphs.size (); i < count; ++i) {
                glRasterPos2i (
                    rectangle.origin.x + positions[i].x + glyphs[i]->GetLeftSideBearing (),
                    rectangle.origin.y + positions[i].y + topMax - glyphs[i]->GetTopSideBearing ());
                const Rectangle::Extents &extents = glyphs[i]->GetExtents ();
                glDrawPixels (extents.width, extents.height, GL_ALPHA,
                    GL_UNSIGNED_BYTE, (const GLubyte *)glyphs[i]->GetData ());
            }
        }
#endif

        void Font::GetGlyphs (
                const std::string &text,
                std::vector<Glyph *> &glyphs,
                std::vector<FT_Vector> &positions) {
            glyphs.reserve (text.size ());
            positions.reserve (text.size ());
            int x = 0;
            int y = 0;
            FT_Bool hasKerning = FT_HAS_KERNING (face.face);
            FT_UInt prevGlyphIndex = 0;
            for (std::size_t i = 0, count = text.size (); i < count; ++i) {
                if (text[i] == '\n') {
                    x = 0;
                    y += GetHeight ();
                    prevGlyphIndex = 0;
                }
                else {
                    Glyph *glyph = glyphCache[text[i]];
                    if (glyph == 0) {
                        glyph = glyphCache[text[i]] = new Glyph (face.face, text[i]);
                    }
                    assert (glyph != 0);
                    glyphs.push_back (glyph);
                    if (hasKerning && prevGlyphIndex != 0 && glyph->index != 0) {
                        FT_Vector kerning;
                        FT_Get_Kerning (face.face, prevGlyphIndex,
                            glyph->index, FT_KERNING_DEFAULT, &kerning);
                        x += kerning.x >> 6;
                    }
                    FT_Vector position = {x, y};
                    positions.push_back (position);
                    x += glyph->glyph.glyph->advance.x >> 16;
                    prevGlyphIndex = glyph->index;
                }
            }
        }

    } // namespace canvas
} // namespace thekogans

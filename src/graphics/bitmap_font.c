/*
#   clove
#
#   Copyright (C) 2017-2018 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "bitmap_font.h"
#include "shader.h"
#include "batch.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../tools/utf8.h"

/* TODO: A better implementation would be by using hashmaps instead of arrays. */

void graphics_BitmapFont_new(graphics_BitmapFont* dst, char const* filename,
        int max_size, char const* glyphs, float glyph_width, float glyph_height)
{
    size_t glyphs_len = strlen(glyphs);

    dst->glyphs_t = malloc(sizeof(glyph_t) * glyphs_len);
    dst->image = malloc(sizeof(graphics_Image));
    dst->data = malloc(sizeof(image_ImageData));
    dst->batches = malloc(sizeof(graphics_Batch));

    int err = image_ImageData_new_with_filename(dst->data, filename);
    if (err == 0)
        printf("%s \n", dst->data->error_msg);

    graphics_Image_new_with_ImageData(dst->image, dst->data);

    int img_width = image_ImageData_getWidth(dst->data);
    int img_height = image_ImageData_getHeight(dst->data);

    // In case we did not specify these two we calculate them manually
    dst->glyph_width = glyph_width = glyph_width == 0 ? (int)(img_width / glyphs_len) : glyph_width;
    dst->glyph_height = glyph_height = glyph_height == 0 ? img_height : glyph_height;

    dst->indexer_map_size = glyphs_len;

    // Get all the glyphs from the font
    uint16_t i = 0;
    uint32_t get_w = 0;

    graphics_Batch_new(dst->batches, dst->image, max_size, graphics_BatchUsage_static);
    while(get_w = utf8_scan(&glyphs))
    {
        dst->glyphs_t[i].quad = malloc(sizeof(graphics_Quad));
        graphics_Quad_newWithRef(dst->glyphs_t[i].quad, glyph_width * i, 0,
                glyph_width, glyph_height, img_width, img_height);
        dst->glyphs_t[i].word = get_w;
        dst->glyphs_t[i].index = i;
        dst->glyphs_t[i].offsetx = 0;
        dst->glyphs_t[i].offsety = 0;
        i++;
    }
    dst->allocated_glyphs = i;
}

void graphics_BitmapFont_setGlyphOffsetX(graphics_BitmapFont* font, float off, char const* glyph)
{
    uint32_t convert_glyph = utf8_scan(&glyph);
    for (uint16_t i = 0; i < font->indexer_map_size; i++)
    {
        if (font->glyphs_t[i].word == convert_glyph)
        {
            font->glyphs_t[i].offsetx = off;
        }
    }
}

void graphics_BitmapFont_setGlyphOffsetY(graphics_BitmapFont* font, float off, char const* glyph)
{
    uint32_t convert_glyph = utf8_scan(&glyph);
    for (uint16_t i = 0; i < font->indexer_map_size; i++)
    {
        if (font->glyphs_t[i].word == convert_glyph)
        {
            font->glyphs_t[i].offsety = off;
        }
    }
}

static glyph_t* find_glyph(graphics_BitmapFont* font, uint32_t what)
{
    for (uint16_t i = 0; i < font->indexer_map_size; i++)
    {
        uint32_t get_word = font->glyphs_t[i].word;
        if (get_word == what) {
            return &font->glyphs_t[i];
        }
    }
    return NULL;
}

void graphics_BitmapFont_render(graphics_BitmapFont* dst, char const* text,
        int x, int y, float r, float sx, float sy, float ox, float oy, float kx,
        float ky)
{
    graphics_Shader* shader = graphics_getShader();
    graphics_setDefaultShader();

    int g = 0;
    int posx = 0;
    int posy = 0;

    graphics_Batch_clear(dst->batches);
    while(g = utf8_scan(&text))
    {
        glyph_t* map = find_glyph(dst, g);
        if (map != NULL && map->word == g) {
            if (g == '\n') {
                posx = 0;
                posy += floor(dst->glyph_height + 1 + 0.5f);
                continue;
            }
            graphics_Batch_add(dst->batches, map->quad, posx + map->offsetx,
                    posy + map->offsety, 0, 1, 1, 0, 0, 0, 0);
            posx += dst->glyph_width;
        }
    }


    graphics_Batch_bind(dst->batches);
    graphics_Batch_draw(dst->batches, x, y, r, sx, sy, ox, oy, kx, ky);
    graphics_Batch_unbind(dst->batches);

    graphics_setShader(shader);
}

void graphics_BitmapFont_free(graphics_BitmapFont* dst)
{
   graphics_Batch_free(dst->batches);
    for (size_t i = 0; i < dst->allocated_glyphs; i++)
        free(dst->glyphs_t[i].quad);
    free(dst->glyphs_t);
    image_ImageData_free(dst->data);
    graphics_Image_free(dst->image);
}


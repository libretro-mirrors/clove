/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "../include/ui.h"

#include "atlas.inl"

#include "../include/image.h"
#include "../include/imagedata.h"
#include "../include/geometry.h"
#include "../include/graphics.h"
#include "../include/font.h"

static struct {
    mu_Context *ctx;
    graphics_Font *font;

    graphics_Image *close_img;
    graphics_Image *check_img;
    graphics_Image *expand_img;
    graphics_Image *collapsed_img;
} moduleData;


static void draw_rect(mu_Rect rect, mu_Color color) {
  graphics_setColor(color.r / 255.0f, color.g / 255.0f,
                    color.b / 255.0f, color.a / 255.0f);
  graphics_geometry_rectangle(true, rect.x, rect.y,
                              rect.w, rect.h,
                              0, 1, 1, 0, 0);
  graphics_setColor(1, 1, 1, 1);
}


static void draw_icon(int id, mu_Rect rect, mu_Color color) {
    mu_Rect src = atlas[id];

  int x = rect.x + (rect.w - src.w) / 2;
  int y = rect.y + (rect.h - src.h) / 2;
  graphics_setColor(color.r / 255.0f, color.g / 255.0f,
                    color.b / 255.0f, color.a / 255.0f);
  graphics_Quad quad;
  graphics_Quad_newWithRef(&quad, src.x, src.y, src.w, src.h, ATLAS_WIDTH, ATLAS_HEIGHT);

  if (id == MU_ICON_CLOSE) {
      graphics_Image_draw(moduleData.close_img, &quad, x, y, 0, 1, 1, 0, 0, 0, 0);
  }
  graphics_setColor(1, 1, 1, 1);
}

static void draw_font(mu_TextCommand text, mu_Color color) {
    graphics_setColor(color.r / 255.0f, color.g / 255.0f,
                      color.b / 255.0f, color.a / 255.0f);
    graphics_Font_render(moduleData.font, text.str,
                         text.pos.x, text.pos.y,
                         0, 1, 1, 0, 0, 0, 0);
    graphics_setColor(1, 1, 1, 1);
}

static int text_width(mu_Font font, const char *text, int len) {
    return graphics_Font_getWidth(moduleData.font, text);
}

static int text_height(mu_Font font) {
  return graphics_Font_getHeight(moduleData.font);
}

void ui_init(void) {
    moduleData.ctx = malloc(sizeof (mu_Context));
    mu_init(moduleData.ctx);

    moduleData.font = malloc(sizeof (graphics_Font));
    graphics_Font_new(moduleData.font, NULL, 12);

    // For font alignment and clipping to work correctly
    moduleData.ctx->text_width = text_width;
    moduleData.ctx->text_height = text_height;

    image_ImageData *data = malloc(sizeof(image_ImageData));
    image_ImageData_new_with_surface(data, atlas_texture, ATLAS_WIDTH, ATLAS_HEIGHT, 1);

    graphics_Filter filter;
    filter.magMode = graphics_FilterMode_nearest;
    filter.minMode = graphics_FilterMode_nearest;
    filter.maxAnisotropy = 1;

    moduleData.close_img = malloc(sizeof(graphics_Image));
    graphics_Image_new_with_ImageData(moduleData.close_img, data);
    graphics_Image_setFilter(moduleData.close_img, &filter);

    graphics_Font_setFilter(moduleData.font, &filter);

}

void ui_deinit(void) {
    free(moduleData.ctx);
    free(moduleData.font);
    graphics_Image_free(moduleData.close_img);
    free(moduleData.close_img);
}

mu_Context *ui_get_context(void) {
    return moduleData.ctx;
}

void ui_layout_row(int no_items, int widths[], int height) {
    mu_layout_row(moduleData.ctx, no_items, widths, height);
}

void ui_layout_begin_column(void) {
    mu_layout_begin_column(moduleData.ctx);
}

void ui_layout_end_column(void) {
    mu_layout_end_column(moduleData.ctx);
}

void ui_layout_set_next(int x, int y,
                        int w, int h,
                        int relative) {
    mu_layout_set_next(moduleData.ctx,
                       mu_rect(x, y, w, h), relative);
}

void ui_layout_width(int width) {
    mu_layout_width(moduleData.ctx, width);
}

mu_Container* ui_get_container() {
    return mu_get_container(moduleData.ctx);
}

mu_Container* ui_init_window(int x, int y, int w, int h, int opt) {
    mu_Container *window = malloc(sizeof(mu_Container));
    window->inited = false;
    if (!window->inited) {
        mu_init_window(moduleData.ctx, window, opt);
        window->rect = mu_rect(x, y, w, h);

        /* limit window to minimum size */
        window->rect.w = mu_max(window->rect.w, 240);
        window->rect.h = mu_max(window->rect.h, 300);
    }
    return window;
}

void ui_deinit_window(mu_Container* window) {
    if (window) {
        free(window);
        window = NULL;
    }
}

int ui_begin_window(const char* title, mu_Container *window) {
    return mu_begin_window(moduleData.ctx, window, title);
}

int ui_button(const char* label) {
    return mu_button(moduleData.ctx, label);
}

int ui_textbox(char* label) {
    return mu_textbox(moduleData.ctx, label, sizeof(label));
}

int ui_header(int *state, const char *label) {
    return mu_header(moduleData.ctx, state, label);
}

int ui_begin_tree(int *state, const char *label) {
    return mu_begin_treenode(moduleData.ctx, state, label);
}

void ui_end_tree(void) {
    mu_end_treenode(moduleData.ctx);
}

void ui_label(const char *label) {
    mu_label(moduleData.ctx, label);
}

void ui_draw_text(const char *text) {
    mu_text(moduleData.ctx, text);
}

void ui_draw_rect(int x, int y, int w, int h,
                  int r, int g, int b, int a) {
    mu_draw_rect(moduleData.ctx, mu_rect(x, y, w, h),
                 mu_color(r, g, b, a));
}

void ui_begin_panel(mu_Container *cnt) {
    mu_begin_panel(moduleData.ctx, cnt);
}

void ui_end_panel(void) {
    mu_end_panel(moduleData.ctx);
}

int ui_begin_popup(mu_Container *cnt) {
    return mu_begin_popup(moduleData.ctx, cnt);
}

void ui_end_popup() {
    mu_end_popup(moduleData.ctx);
}

int ui_slider(mu_Real value, int low, int high, int opt) {
  static mu_Real tmp;
  mu_push_id(moduleData.ctx, &value, sizeof(value));
  tmp = value;
  int res = mu_slider_ex(moduleData.ctx, &tmp, low, high,
                         0, "%.0f", opt);
  value = tmp;
  mu_pop_id(moduleData.ctx);
  return res;
}

void ui_end_window(void) {
    mu_end_window(moduleData.ctx);
}

void ui_begin(void) {
    mu_begin(moduleData.ctx);
}

void ui_end(void) {
    mu_end(moduleData.ctx);
}

void ui_draw(void) {
    mu_Command *cmd = NULL;
    while (mu_next_command(moduleData.ctx, &cmd)) {
      if (cmd->type == MU_COMMAND_TEXT) {
          draw_font(cmd->text, cmd->text.color);
      } if (cmd->type == MU_COMMAND_RECT) {
          draw_rect(cmd->rect.rect, cmd->rect.color);
      } else if (cmd->type == MU_COMMAND_ICON) {
          draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
      } else if (cmd->type == MU_COMMAND_CLIP) {
          glScissor(cmd->clip.rect.x, graphics_getHeight() - (cmd->clip.rect.y + cmd->clip.rect.h),
                    cmd->clip.rect.w, cmd->clip.rect.h);
      }
    }
}

void ui_input_mouse_move(int x, int y) {
    mu_input_mousemove(moduleData.ctx, x, y);
}

void ui_input_mouse_down(int btn, int x, int y) {
    mu_input_mousedown(moduleData.ctx, x, y, btn);
}

void ui_input_mouse_up(int btn, int x, int y) {
    mu_input_mouseup(moduleData.ctx, x, y, btn);
}

void ui_input_text(const char *txt) {
    mu_input_text(moduleData.ctx, txt);
}

void ui_input_scroll(int x, int y) {
    mu_input_scroll(moduleData.ctx, x, y);
}

void ui_input_keydown(int key) {
    mu_input_keydown(moduleData.ctx, key);
}

void ui_input_keyup(int key) {
    mu_input_keyup(moduleData.ctx, key);
}

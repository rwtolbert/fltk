//
// Unit tests for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2022 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

#include "unittests.h"

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/fl_draw.H>

#include "unittest_unicode_utf8.h"      // defines utf8_box_test[]
static const char *helptext =
  "In this test, ideally the box's lines should all be touching "
  "without white space between. Underbar and overbars should both "
  "be visible and not touching. All the above should be unaffected "
  "by different font sizes and font settings.";


class UnicodeBoxTest : public Fl_Group {
  Fl_Text_Buffer      *textbuffer;
  Fl_Text_Display     *textdisplay;
  Fl_Multiline_Input  *multilineinput;
  Fl_Choice           *font_choice;
  Fl_Hor_Value_Slider *fontsize_slider;

  // Font choice callback
  void FontChoice_CB2() {
    switch ( font_choice->value() ) {
      case 0: textdisplay->textfont(FL_COURIER); break;
      case 1: textdisplay->textfont(FL_SCREEN); break;
    }
    parent()->redraw();
  }
  static void FontChoice_CB(Fl_Widget*, void *userdata) {
    UnicodeBoxTest *o = (UnicodeBoxTest*)userdata;
    o->FontChoice_CB2();
  }

  // Slider callback - apply new font size to widgets
  void FontSizeSlider_CB2() {
    // Get font size from slider value, apply to widgets
    int fontsize = (int)fontsize_slider->value();
    textdisplay->textsize(fontsize);
    multilineinput->textsize(fontsize);
    multilineinput->position(0);                // keep scrolled to top
    parent()->redraw();
  }
  static void FontSizeSlider_CB(Fl_Widget*, void *userdata) {
    UnicodeBoxTest *o = (UnicodeBoxTest*)userdata;
    o->FontSizeSlider_CB2();
  }

public:
  static Fl_Widget *create() {
    return new UnicodeBoxTest(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H);
  }

  UnicodeBoxTest(int x, int y, int w, int h) : Fl_Group(x, y, w, h) {
    // Fl_Text_Display
    textbuffer = new Fl_Text_Buffer();
    textbuffer->text(utf8_box_test);
    textdisplay = new Fl_Text_Display(x+5, y+20, 250, 250, "Fl_Text_Display");
    textdisplay->textfont(FL_COURIER);
    textdisplay->buffer(textbuffer);
    textdisplay->tooltip(helptext);
    // Fl_Multiline_Input
    multilineinput = new Fl_Multiline_Input(x+250+15, y+20, 250, 250, "Fl_Multiline_Input");
    multilineinput->align(FL_ALIGN_CENTER|FL_ALIGN_TOP);
    multilineinput->textfont(FL_COURIER);
    multilineinput->value(utf8_box_test);
    multilineinput->tooltip(helptext);
    // Font choice
    //    Fonts must be fixed width to work correctly..
    font_choice = new Fl_Choice(x+150, y+h-80, 200, 25, "Font face");
    font_choice->add("FL_COURIER");
    font_choice->add("FL_SCREEN");
    font_choice->value(0);
    font_choice->callback(FontChoice_CB, (Fl_Widget*)this);
    // Font size slider
    fontsize_slider = new Fl_Hor_Value_Slider(x+150, y+h-50, 200, 25, "Font size");
    fontsize_slider->align(FL_ALIGN_LEFT);
    fontsize_slider->range(1.0, 50.0);
    fontsize_slider->step(1.0);
    fontsize_slider->value(14.0);
    fontsize_slider->callback(FontSizeSlider_CB, (Fl_Widget*)this);
    end();
  }
};

UnitTest unicode_font_test(kUnicodeBoxTest, "Unicode Boxes", UnicodeBoxTest::create);

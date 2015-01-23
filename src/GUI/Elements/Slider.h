/*
 * Slider.h
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 FilipeCN
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
*/

#pragma once

#ifndef SLIDER_H
#define SLIDER_H

#include <GUI.h>
using aergia::gui::GUI;
#include <Config.h>
using aergia::common::Config;
#include <GUIManager.h>
using aergia::gui::GUIManager;
#include <Button.h>
using aergia::gui::Button;

#include <vector>
using std::vector;

namespace aergia {

    namespace gui {

        class Slider : public GUI {
        public:
            Slider(string text, float x, float y);

            void render(unsigned int mode = 0);
            bool processButton(int button, int action, vec2 p);
            bool processMouse(vec2 p);

            vector<Button> buttons;

        protected:
        };

    }
}
#endif

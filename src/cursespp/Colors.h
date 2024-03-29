//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2023 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cursespp/curses_config.h>
#include <string>
#include <vector>

namespace cursespp {
    class Color {
        public:
            enum Type {
                Default = -1,

                ListItemSelected = 1,
                ListItemHighlighted = 2,
                ListItemError = 3,
                ListItemHighlightedSelected = 4,
                ListItemHeader = 5,
                ListItemHeaderHighlighted = 6,

                ContentColorDefault = 7,
                FrameDefault = 8,
                FrameFocused = 9,
                FrameImportant = 10,

                TextDefault = 11,
                TextDisabled = 12,
                TextFocused = 13,
                TextActive = 14,
                TextWarning = 15,
                TextError = 16,
                TextHidden = 17,

                ButtonDefault = 18,
                ButtonHighlighted = 19,

                ShortcutRowDefault = 20,
                ShortcutRowFocused = 21,

                OverlayFrame = 22,
                OverlayContent = 23,
                OverlayTextInputFrame = 24,
                OverlayTextFocused = 25,
                OverlayListFrame = 26,
                OverlayListFrameFocused = 27,

                Header = 28,
                Footer = 29,
                Banner = 30
            };

            Color() {
                this->value = -1LL;
            }

            Color(Type type) {
                this->value = (type == Default) ? -1LL : COLOR_PAIR(type);
            }

            Color(const Color& color) {
                this->value = color;
            }

            operator int64_t() const {
                return this->value;
            }

        private:
            int64_t value;
    };

    class Colors {
        private:
            Colors();

        public:
            enum Mode {
                RGB,
                Palette,
                Basic
            };

            enum BgType {
                Theme,
                Inherit,
            };

            static void Init(Mode mode = Mode::Basic, BgType bgType = BgType::Theme);
            static void SetTheme(const std::string& name);
            static std::vector<std::string> ListThemes();
    };
}

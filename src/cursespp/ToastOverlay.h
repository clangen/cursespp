//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2020 musikcube team
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

#include <cursespp/OverlayBase.h>
#include <f8n/runtime/IMessage.h>
#include <vector>

namespace cursespp {
    class ToastOverlay:
        public OverlayBase,
        public sigslot::has_slots<>
    {
        public:
            static void Show(const std::string& text, int durationMs = 3000);

            virtual ~ToastOverlay();

            ToastOverlay(const ToastOverlay& other) = delete;
            ToastOverlay& operator=(const ToastOverlay& other) = delete;

            /* IWindow */
            void Layout() override;
            bool KeyPress(const std::string& key) override;
            void ProcessMessage(f8n::runtime::IMessage& message) override;
            void OnRedraw() override;

        protected:
            void OnVisibilityChanged(bool visible) override;

        private:
            ToastOverlay(const std::string& text, long durationMs);

            void RecalculateSize();

            bool ticking;
            std::string title;
            std::vector<std::string> titleLines;
            int durationMs;
            int x, y;
            int width, height;
    };
}
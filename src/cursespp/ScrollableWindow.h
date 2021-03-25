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

#include <cursespp/curses_config.h>
#include <cursespp/Window.h>
#include <cursespp/IScrollAdapter.h>
#include <cursespp/IScrollable.h>
#include <cursespp/IKeyHandler.h>

namespace cursespp {
    class ScrollableWindow:
        public Window,
        public IScrollable,
        public IKeyHandler
    {
        public:
            ScrollableWindow(
                std::shared_ptr<IScrollAdapter> adapter,
                IWindow *parent = nullptr);

            ScrollableWindow(IWindow *parent = nullptr);

            virtual ~ScrollableWindow();

            /* IWindow */
            void Show() override;
            void OnDimensionsChanged() override;
            bool KeyPress(const std::string& key) override;
            void Focus() override;
            void Blur() override;
            void OnRedraw() override;

            /* IMouseHandler */
            bool MouseEvent(const IMouseHandler::Event& event) override;

            /* IScrollable */
            void ScrollToTop() override;
            void ScrollToBottom() override;
            void ScrollUp(int delta = 1) override;
            void ScrollDown(int delta = 1) override;
            void PageUp() override;
            void PageDown() override;

            void SetAllowArrowKeyPropagation(bool allow = true);

            /* virtual methods we define */
            virtual void SetAdapter(std::shared_ptr<IScrollAdapter> adapter);
            virtual void OnAdapterChanged();
            virtual const IScrollAdapter::ScrollPosition& GetScrollPosition();

        protected:
            friend class Scrollbar;

            virtual IScrollAdapter& GetScrollAdapter();
            virtual IScrollAdapter::ScrollPosition& GetMutableScrollPosition();

            size_t GetPreviousPageEntryIndex();
            bool IsLastItemVisible();

        private:
            std::shared_ptr<IScrollAdapter> adapter;
            IScrollAdapter::ScrollPosition scrollPosition;
            bool allowArrowKeyPropagation;
    };
}

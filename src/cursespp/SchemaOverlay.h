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

#include <f8n/sdk/ISchema.h>
#include <f8n/preferences/Preferences.h>

namespace cursespp {
    class SchemaOverlay {
        public:
            static void Show(
                const std::string& title,
                std::shared_ptr<f8n::prefs::Preferences> prefs,
                std::shared_ptr<f8n::sdk::ISchema> schema,
                std::function<void(bool)> callback);

            static void ShowListOverlay(
                const std::string& title,
                std::vector<std::string>& items,
                const std::string defaultValue,
                std::function<void(std::string)> cb);

            static void ShowBoolOverlay(
                const f8n::sdk::ISchema::BoolEntry* entry,
                std::shared_ptr<f8n::prefs::Preferences> prefs,
                std::function<void(std::string)> callback);

            static void ShowIntOverlay(
                const f8n::sdk::ISchema::IntEntry* entry,
                std::shared_ptr<f8n::prefs::Preferences> prefs,
                std::function<void(std::string)> callback);

            static void ShowDoubleOverlay(
                const f8n::sdk::ISchema::DoubleEntry* entry,
                std::shared_ptr<f8n::prefs::Preferences> prefs,
                std::function<void(std::string)> callback);

            static void ShowStringOverlay(
                const f8n::sdk::ISchema::StringEntry* entry,
                std::shared_ptr<f8n::prefs::Preferences> prefs,
                std::function<void(std::string)> callback);

            static void ShowEnumOverlay(
                const f8n::sdk::ISchema::EnumEntry* entry,
                std::shared_ptr<f8n::prefs::Preferences> prefs,
                std::function<void(std::string)> callback);

        private:
            SchemaOverlay();
    };
}

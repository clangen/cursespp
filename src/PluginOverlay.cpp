//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007-2017 musikcube team
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

#include <cursespp/PluginOverlay.h>

#include <f8n/environment/Environment.h>
#include <f8n/environment/Filesystem.h>
#include <f8n/plugins/Plugins.h>
#include <f8n/sdk/ISchema.h>
#include <f8n/i18n/Locale.h>
#include <f8n/str/utf.h>
#include <f8n/str/util.h>

#include <cursespp/App.h>
#include <cursespp/Colors.h>
#include <cursespp/DialogOverlay.h>
#include <cursespp/InputOverlay.h>
#include <cursespp/ListOverlay.h>
#include <cursespp/SchemaOverlay.h>
#include <cursespp/Screen.h>
#include <cursespp/ScrollAdapterBase.h>
#include <cursespp/SingleLineEntry.h>
#include <cursespp/Text.h>

#include <algorithm>
#include <ostream>
#include <iomanip>
#include <limits>

using namespace f8n;
using namespace f8n::env;
using namespace f8n::sdk;
using namespace f8n::prefs;
using namespace f8n::plugin;
using namespace f8n::i18n;
using namespace f8n::utf;
using namespace cursespp;

static const std::string unchecked = "[ ]";
static const std::string checked = "[x]";

struct PluginInfo {
    IPlugin* plugin;
    std::string fn;
    bool enabled;
};

using PluginInfoPtr = std::shared_ptr<PluginInfo>;
using PluginList = std::vector<PluginInfoPtr>;
using PrefsPtr = std::shared_ptr<Preferences>;
using SinglePtr = std::shared_ptr<SingleLineEntry>;
using SchemaPtr = std::shared_ptr<ISchema>;

static size_t DEFAULT_INPUT_WIDTH = 26;
static size_t MINIMUM_OVERLAY_WIDTH = 16;

static int overlayWidth() {
    return (int)(0.8f * (float) Screen::GetWidth());
}

static void showConfigureOverlay(IPlugin* plugin, SchemaPtr schema) {
    auto prefs = Preferences::ForPlugin(plugin->Name());
    std::string title = _TSTR("settings_configure_plugin_title");
    str::replace(title, "{{name}}", plugin->Name());
    SchemaOverlay::Show(title, prefs, schema, [plugin](bool changed) {
        if (changed) {
            plugin->Reload();
        }
    });
}

static void showNoSchemaDialog(const std::string& name) {
    std::shared_ptr<DialogOverlay> dialog(new DialogOverlay());

    std::string message = _TSTR("settings_no_plugin_config_message");
    str::replace(message, "{{name}}", name);

    (*dialog)
        .SetTitle(_TSTR("settings_no_plugin_config_title"))
        .SetMessage(message)
        .AddButton("KEY_ENTER", "ENTER", _TSTR("button_ok"));

    App::Overlays().Push(dialog);
}

class PluginListAdapter : public ScrollAdapterBase {
    public:
        PluginListAdapter() {
            this->prefs = Preferences::ForComponent("plugins");
            this->Refresh();
        }

        virtual ~PluginListAdapter() {
            this->prefs->Save();
        }

        void configure(size_t index) {
            SchemaPtr schema;

            using Deleter = Plugins::ReleaseDeleter<ISchema>;
            std::string guid = plugins[index]->plugin->Guid();
            Plugins::Instance().QueryInterface<ISchema, Deleter>(
                "GetSchema",
                [guid, &schema](IPlugin* raw, SchemaPtr plugin, const std::string& fn) {
                    if (raw->Guid() == guid) {
                        schema = plugin;
                    }
                });

            if (schema) {
                showConfigureOverlay(plugins[index]->plugin, schema);
            }
            else {
                showNoSchemaDialog(plugins[index]->plugin->Name());
            }
        }

        void toggleEnabled(size_t index) {
            PluginInfoPtr plugin = this->plugins.at(index);
            plugin->enabled = !plugin->enabled;
            this->prefs->SetBool(plugin->fn, plugin->enabled);
        }

        virtual size_t GetEntryCount() override {
            return plugins.size();
        }

        virtual EntryPtr GetEntry(cursespp::ScrollableWindow* window, size_t index) override {
            PluginInfoPtr info = plugins.at(index);

            std::string display =
                " " +
                (info->enabled ? checked : unchecked) + " " +
                info->plugin->Name() + " (" + info->fn + ")";

            SinglePtr result = SinglePtr(new SingleLineEntry(text::Ellipsize(display, this->GetWidth())));

            result->SetAttrs(Color(Color::Default));
            if (index == window->GetScrollPosition().logicalIndex) {
                result->SetAttrs(Color(Color::ListItemHighlighted));
            }

            return result;
        }

    private:
        static std::string SortKey(const std::string& input) {
            std::string name = input;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            return name;
        }

        void Refresh() {
            plugins.clear();

            PluginList& plugins = this->plugins;
            auto prefs = this->prefs;

            using Deleter = Plugins::NullDeleter<IPlugin>;
            using Plugin = std::shared_ptr<IPlugin>;

            Plugins::Instance().QueryInterface<IPlugin, Deleter>(
                "GetPlugin",
                [&plugins, prefs](IPlugin* raw, Plugin plugin, const std::string& fn) {
                    PluginInfoPtr info(new PluginInfo());
                    info->plugin = raw;
                    info->fn = fs::Filename(fn);
                    info->enabled = prefs->GetBool(info->fn, true);
                    plugins.push_back(info);
                });

            std::sort(plugins.begin(), plugins.end(), [](PluginInfoPtr p1, PluginInfoPtr p2) -> bool {
                return SortKey(p1->plugin->Name()) < SortKey(p2->plugin->Name());
            });
        }

        std::shared_ptr<Preferences> prefs;
        PluginList plugins;
};

PluginOverlay::PluginOverlay() {

}

void PluginOverlay::Show() {
    std::shared_ptr<PluginListAdapter> pluginAdapter(new PluginListAdapter());
    std::shared_ptr<ListOverlay> dialog(new ListOverlay());

    dialog->SetAdapter(pluginAdapter)
        .SetTitle(_TSTR("plugin_overlay_title"))
        .SetWidthPercent(80)
        .SetAutoDismiss(false)
        .SetKeyInterceptorCallback(
            [pluginAdapter](ListOverlay* overlay, std::string key) -> bool {
                if (key == " ") {
                    size_t index = overlay->GetSelectedIndex();
                    if (index != ListWindow::NO_SELECTION) {
                        pluginAdapter->toggleEnabled(index);
                        overlay->RefreshAdapter();
                        return true;
                    }
                }
                return false;
            })
        .SetItemSelectedCallback(
            [pluginAdapter](ListOverlay* overlay, IScrollAdapterPtr adapter, size_t index) {
                pluginAdapter->configure(index);
            });

    cursespp::App::Overlays().Push(dialog);
}

/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - David Pinedo <david@lunarg.com>
 */

#include "main_doc.h"
#include "../vkconfig_core/layer_manager.h"
#include "../vkconfig_core/doc.h"
#include "../vkconfig_core/configuration_manager.h"

#include <cassert>

int run_doc_html(const CommandLine& commandLine) {

    PathManager paths;
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);

    LayerManager layers(environment);
    layers.LoadAllInstalledLayers();

    for (std::size_t i = 0, n = layers.available_layers.size(); i < n; ++i) {
        const Layer& layer = layers.available_layers[i];
        if (layer.key == commandLine.doc_layer_name) {
            const std::string path = format("%s/%s.html", GetPath(BUILTIN_PATH_APPDATA).c_str(), layer.key.c_str());
            ExportHtmlDoc(layer, path);
            return 0;
        }
    }
    fprintf(stderr, "vkconfig: layer %s not found\n", commandLine.doc_layer_name.c_str());
    return -1;
}

int run_doc_settings(const CommandLine& commandLine) {

    bool rval;
    PathManager paths;
    Environment environment(paths);
    environment.Reset(Environment::DEFAULT);
    ConfigurationManager configuration_manager(paths, environment);
    Configuration config;
    LayerManager layers(environment);
    const std::string settings_path = GetPath(BUILTIN_PATH_OVERRIDE_SETTINGS);
    Layer *layer;

    layers.LoadLayer(commandLine.doc_layer_name);
    layer = FindByKey(layers.available_layers, commandLine.doc_layer_name.c_str());
    if (!layer) {
       printf("vkconfig: could not find layer %s\n", commandLine.doc_layer_name.c_str());
       return -1;
    }
    config = configuration_manager.CreateConfiguration(layers.available_layers, "Config");
    config.parameters = GatherParameters(config.parameters, layers.available_layers);
    config.parameters[0].state = LAYER_STATE_OVERRIDDEN;
    ExportSettingsDoc(environment, layers.available_layers, config, settings_path);

    return rval;
}

int run_doc(const CommandLine& command_line) {
    assert(command_line.command == COMMAND_DOC);
    assert(command_line.error == ERROR_NONE);

    switch (command_line.command_doc_arg) {
        case COMMAND_DOC_HTML: {
            return run_doc_html(command_line);
        }
        case COMMAND_DOC_SETTINGS: {
            return run_doc_settings(command_line);
        }
        default: {
            assert(0);
            return -1;
        }
    }
}
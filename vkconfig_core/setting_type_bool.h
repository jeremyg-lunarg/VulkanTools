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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "setting_type.h"

#include <memory>

struct SettingMetaBool : public SettingMeta {
    static const SettingType TYPE;

    SettingData* Instantiate() override;
    bool Load(const QJsonObject& json_setting) override;
    std::string Export(ExportMode export_mode) const override;

    bool default_value;

   protected:
    SettingMetaBool(Layer& layer, const std::string& key, const SettingType type);

    bool Equal(const SettingMeta& other) const override;

   private:
    SettingMetaBool(Layer& layer, const std::string& key);

    friend class Layer;
};

struct SettingDataBool : public SettingData {
    SettingDataBool(const SettingMetaBool* meta);

    void Reset() override;
    bool Parse(const std::string& value, const ParseSource parse = PARSE_SETTING) override;
    bool Load(const QJsonObject& json_setting) override;
    bool Save(QJsonObject& json_setting) const override;
    std::string Export(ExportMode export_mode) const override;

    bool value;

   protected:
    bool Equal(const SettingData& other) const override;

    const SettingMetaBool* meta;
};

struct SettingMetaBoolNumeric : public SettingMetaBool {
    static const SettingType TYPE;

    SettingMetaBoolNumeric(Layer& layer, const std::string& key);

    SettingData* Instantiate() override;

    std::string Export(ExportMode export_mode) const override;
};

struct SettingDataBoolNumeric : public SettingDataBool {
    SettingDataBoolNumeric(const SettingMetaBoolNumeric* meta);

    std::string Export(ExportMode export_mode) const override;
};

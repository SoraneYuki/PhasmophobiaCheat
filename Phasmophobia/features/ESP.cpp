﻿#include "ESP.h"
#include "GhostList.h"
#include "../library/magic_enum/magic_enum.hpp"

ESP::ESP() : Feature{} {
   
}

auto ESP::GetInstance() -> ESP& {
    static ESP Instance;
    return Instance;
}

auto ESP::GetInfo() const -> const GuiInfo& {
    static GuiInfo info{ reinterpret_cast<const char*>(u8"透视"), reinterpret_cast<const char*>(u8"透视设置"), true };
    return info;
}

auto ESP::Render() -> void {
    ImGui::Checkbox(reinterpret_cast<const char*>(u8"鬼魂透视"), &ghostEsp);
}

auto ESP::Update() -> void {
    
}

auto ESP::DrawStatus() -> void {
    try {
        if (ghostEsp) {
            const auto ghosts = GhostList::GetGhosts();
            for (auto& ghost : ghosts) {
                auto vector3 = unity::CSharper::IL2cpp::Camera::GetMain()->WorldToScreenPoint(ghost->GetTransform()->GetPosition(), unity::CSharper::IL2cpp::Camera::m_eCameraEye_Center);
                vector3.y = initSpace::guiInfo.h - vector3.y;
                if ((vector3.x > 0 && vector3.y > 0) && (vector3.x < initSpace::guiInfo.w && vector3.y < initSpace::guiInfo.h) && vector3.z > 0) {
                    ImGui::GetBackgroundDrawList()->AddText(ImVec2(vector3.x, vector3.y), 0xFF0000FF, std::format("{}.{}\n[{}] M", static_cast<int>(ghost->GetGhostType()), magic_enum::enum_name<GhostAPI::GhostType>(ghost->GetGhostType()), vector3.z).c_str());
                }
            }
        }
    } catch (...) {}
}

auto ESP::Save(nlohmann::json& json) -> void {
    json["ghostEsp"] = ghostEsp;
}

auto ESP::Load(nlohmann::json& json) -> void {
    if (json.contains("ghostEsp")) {
        ghostEsp = json["ghostEsp"];
    }
}
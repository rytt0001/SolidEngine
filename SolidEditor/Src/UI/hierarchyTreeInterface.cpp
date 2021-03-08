#include "UI/hierarchyTreeInterface.hpp"
#include "UI/editorInterface.hpp"
#include <imgui.h>



namespace Solid
{
    void HierarchyTreeInterface::Draw(Engine* _engine)
    {
        if(!p_open)
            return;

        UI::SetNextWindowSize(ImVec2(250,250));

        UI::Begin("Hierarchy", &p_open,
                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        UI::TextWrapped("Welcome on the Solid hierarchy tree!");

        for(GameObject* g : _engine->ecsManager.GetWorld()->childs)
        {
            DrawEntities(g,0);
        }
        DrawCreateObject();

        UI::End();
    }
}

void Solid::HierarchyTreeInterface::DrawCreateObject()
{
    if(UI::BeginPopupContextWindow("createObject"))
    {
        if(UI::BeginMenu("New"))
        {
            if(UI::MenuItem("Cube"))
            {

            }
            if(UI::MenuItem("Sphere"))
            {

            }
            if(UI::MenuItem("Solid"))
            {

            }
            UI::EndMenu();
        }

        UI::EndPopup();
    }
}

void Solid::HierarchyTreeInterface::DrawEntities(GameObject* child, unsigned int it)
{


    ImVec2 pos = UI::GetCursorPos();
    pos.x += 20.f * (float)it;
    UI::SetCursorPos(pos);

    DrawEntity(child);


    for(GameObject* child : child->childs)
    {
        DrawEntities(child, it+1);
    }

}

void Solid::HierarchyTreeInterface::DrawEntity(GameObject* child)
{
    ImVec4 colButton = UI::GetStyleColorVec4(ImGuiCol_Button);
    if (child == EditorInterface::selectedGO)
        colButton.w = 0.5f;
    else
        colButton.w = 0.0f;

    UI::PushStyleColor(ImGuiCol_Button, colButton);
    UI::PushStyleColor(ImGuiCol_ButtonHovered, colButton);

    if(UI::SmallButton(child->name.c_str()))
        EditorInterface::selectedGO = child;

    UI::PopStyleColor(2);
}

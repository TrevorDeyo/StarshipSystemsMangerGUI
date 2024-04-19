#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "string"
#include "colors.h"

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

class StarshipSystemManagerLayer : public Walnut::Layer {
public:

    virtual void OnUIRender() override
    {

        ImGui::Begin("Starship Systems Manger");

        // Solar Pannels
        static bool solarPanelsDeploy{ false };
        ImGui::Checkbox("Solar Pannels", &solarPanelsDeploy);

        static float solarPanelsStatus = 0.0f;
        static float startCapacitorsCharge{ 0.1f };
        static std::string solarPanelState = "";

        static ImColor panelStateColor(1, 1, 1, 1);

        // solar panel deploying
        if (solarPanelsDeploy == true && solarPanelsStatus < 1.0f) {
            solarPanelsStatus += 0.1f * ImGui::GetIO().DeltaTime;
            solarPanelState = " Deploying";
            panelStateColor = ImGui::Colors::Yellow;
        }

        // solar panels retracting
        if (solarPanelsDeploy == false && solarPanelsStatus > 0.0f) {
            solarPanelsStatus -= 0.1f * ImGui::GetIO().DeltaTime;
            solarPanelState = " Retracting";
            panelStateColor = ImGui::Colors::Yellow;
        }

        // Solar panels drawing power from start capactiors 
        if (solarPanelsDeploy == true && solarPanelsStatus < 1 || solarPanelsDeploy == false && solarPanelsStatus > 0) {
            startCapacitorsCharge -= 0.001f * ImGui::GetIO().DeltaTime;
        }
        
        // Solar panels deployed
        if (solarPanelsStatus >= 1.0f) {
            solarPanelState = " Deployed";
            panelStateColor = ImGui::Colors::LawnGreen;
        }

        // Solar panels retracted
        if (solarPanelsStatus <= 0.0f) {
            solarPanelState = " Stowed";
            panelStateColor = ImGui::Colors::Aqua;
        }

        // Solar panel progress bar
        ImGui::ProgressBar(solarPanelsStatus , ImVec2(0.0f, 0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Solar Panel Status:", solarPanelState.c_str());
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::TextColored(panelStateColor, solarPanelState.c_str());

        // Solar panels charging rate
        static float solarPanelsRate{ 0.01f * ImGui::GetIO().DeltaTime };
        // Start Capacitors getting power from solar panels
        if (solarPanelsStatus >=1 && startCapacitorsCharge < 1.0f ) { startCapacitorsCharge += solarPanelsRate; }
        if (startCapacitorsCharge > 1.0f) { startCapacitorsCharge = 1.0f; }
        
        // Start Capacitors Progress Bar
        static float startCapacitorsMaxCapacity{ 10000.0f };
        float progress_saturated = IM_CLAMP(startCapacitorsCharge, 0.0f, startCapacitorsMaxCapacity);
        char buf[32];
        sprintf_s(buf, "%d/%d", (int)(progress_saturated * startCapacitorsMaxCapacity), (int)startCapacitorsMaxCapacity);
        ImGui::ProgressBar(startCapacitorsCharge, ImVec2(0.f, 0.f), buf);
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Start Capacitors Charge");

        // Gas Harvesters Check Boxes
        ImGui::Text("Gas Harvester Systems");

        static bool heliumHarvester{ false };
        ImGui::Checkbox("Helium", &heliumHarvester);

        ImGui::SameLine();
        static bool Helium3Harvester{ false };
        ImGui::Checkbox("Helium3", &Helium3Harvester);

        ImGui::SameLine();
        static bool DeuteriumHarvester{ false };
        ImGui::Checkbox("Deuterium 1 & 2", &DeuteriumHarvester);

        // Helium Tank
        static float heliumTankCollectionRate{ 1.0f * ImGui::GetIO().DeltaTime };
        static float heliumTank{ 0 };
        static float heliumTankMaxCapacity{ 100000 };

        // Helium3 Tank
        static float helium3TankCollectionRate{ 1.0f * ImGui::GetIO().DeltaTime };
        static float helium3Tank{ 0 };
        static float helium3TankMaxCapacity{ 100000 };

        // Deuterium Tank
        static float deuteriumTankCollectionRate{ 1.0f * ImGui::GetIO().DeltaTime };
        static float deuteriumTank{ 0 };
        static float deuteriumTankMaxCapacity{ 200000 };

        static bool heliumTankChiller{ false };

        static bool helium3TankChiller{ false };

        static bool deuteriumTank1Chiller{ false };



        // Vertical Slider Code
        const float spacing = 4;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

        static int int_value = 0;
        ImGui::VSliderInt("##int", ImVec2(18, 160), &int_value, 0, 5);
        ImGui::SameLine();

        static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
        ImGui::PushID("set1");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
            ImGui::VSliderFloat("##v", ImVec2(18, 160), &values[i], 0.0f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.3f", values[i]);
            ImGui::PopStyleColor(4);
            ImGui::PopID();
        }
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID("set2");
        static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
        const int rows = 3;
        const ImVec2 small_slider_size(18, (float)(int)((160.0f - (rows - 1) * spacing) / rows));
        for (int nx = 0; nx < 4; nx++)
        {
            if (nx > 0) ImGui::SameLine();
            ImGui::BeginGroup();
            for (int ny = 0; ny < rows; ny++)
            {
                ImGui::PushID(nx * rows + ny);
                ImGui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
                if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                    ImGui::SetTooltip("%.3f", values2[nx]);
                ImGui::PopID();
            }
            ImGui::EndGroup();
        }
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID("set3");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
            ImGui::VSliderFloat("##v", ImVec2(40, 160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
            ImGui::PopStyleVar();
            ImGui::PopID();
        }
        ImGui::PopID();
        ImGui::PopStyleVar();




        // Fusion Reactor
        static bool magneticCoils{ false };
        static bool reactorState{ false };
        static bool reactorBusSupply{ false };

        // Batteries
        // Life Support
        // Shields
        // FTL Drive
        // Weapons

        // text color for different systems?

        // variance in power generation and gas collection
        ImGui::End();

        //ImGui::ShowDemoWindow();
    }
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    Walnut::ApplicationSpecification spec;
    spec.Name = "Starship Systems Manager";

    Walnut::Application* app = new Walnut::Application(spec);
    app->PushLayer<StarshipSystemManagerLayer>();
    app->SetMenubarCallback([app]()
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Quit", "Alt+F4"))
                {
                    app->Close();
                }
                ImGui::EndMenu();
            }
        });
    return app;
}

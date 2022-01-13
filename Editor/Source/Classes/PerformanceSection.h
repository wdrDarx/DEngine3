#pragma once
#include "EditorSection.h"
#include "DEngine.h"
#include "EditorApp.h"

class PerformanceSection : public EditorSection
{
public:
	using EditorSection::EditorSection;

	void Init() override
	{

	}

	void Render() override
	{
		m_FrameTimes.push_back(GetEditorApp()->GetLastTick().DeltaTime * 1000.f);
		if (m_FrameTimes.size() > m_MaxFramesToRecord)
			m_FrameTimes.erase(m_FrameTimes.begin());

		ImGui::Begin("Performance");

		std::string msTetx = "DeltaTime : " + STRING(GetEditorApp()->GetLastTick().DeltaTime * 1000.f) + "ms";
		std::string FPSText = "FPS : " + STRING(1.f / GetEditorApp()->GetLastTick().DeltaTime);
		ImGui::Text(msTetx.c_str());
		ImGui::Text(FPSText.c_str());

		ImGui::PlotLines("", m_FrameTimes.data(), m_FrameTimes.size(), 0, (const char*)0, 0.f, 20.f, { 200,200 }, 4);

		ImGui::End();
	}

	float m_FrameRecordTime = 0.f;
	uint m_MaxFramesToRecord = 600;
	std::vector<float> m_FrameTimes;

};


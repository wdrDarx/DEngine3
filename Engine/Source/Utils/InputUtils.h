#pragma once
#include "Rendering/Window.h"

class DENGINE_API InputUtils
{
	public:
		static void SetShowCursor(Ref<RenderAPI> context, bool show);
		static void SetCursorPos(Ref<RenderAPI> context, const vec2d& windowPos);
		static void SetUseRawMouseInput(Ref<RenderAPI> context, bool use);
};
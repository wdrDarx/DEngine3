#include "InputUtils.h"
#include "DEngine.h"

void InputUtils::SetShowCursor(Ref<RenderAPI> context, bool show)
{
	glfwSetInputMode(context->GetCurrentContext(), GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void InputUtils::SetCursorPos(Ref<RenderAPI> context, const vec2d& windowPos)
{
	//invert for glfw
	glfwSetCursorPos(context->GetCurrentContext(), windowPos.x, context->GetWindowSize().y - windowPos.y);
}

void InputUtils::SetUseRawMouseInput(Ref<RenderAPI> context, bool use)
{
	glfwSetInputMode(context->GetCurrentContext(), GLFW_RAW_MOUSE_MOTION, use);
}

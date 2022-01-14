#pragma once
#include "DEngine.h"
#include "ImGuizmo.h"
#include "EditorApp.h"

class Viewport
{
public:
	Viewport(Ref<Scene> scene, Ref<Window> WindowContext, const std::string& viewportName) : m_Scene(scene), m_Window(WindowContext), m_ViewportName(viewportName)
	{
		FrameBufferSpec spec;
		spec.Width = 1;
		spec.Height = 1;
		spec.Samples = 1;
		spec.ColorFormat = GL_RGB16F;
		m_Framebuffer = MakeRef<FrameBuffer>(spec);

		m_Camera = MakeRef<Camera>(WindowContext->GetRenderAPI());

		//key callback
		m_KeyEvent.Assign([&](EventKey& event)
		{
			if (event.keyState == KeyState::DOWN)
			{

			}
		});
		m_InputManager.BindOnKeyDown(m_KeyEvent);

		//mouse callback
		m_MouseCallback.Assign([&](EventMouseMove& event)
		{			
			m_LastMouseVector = event.MoveVector;
		});

		m_InputManager.BindOnMouseMove(m_MouseCallback);
	}

	~Viewport()
	{

	}

	//Render the scene onto our framebuffer
	void Render(bool overrideFocus = false, bool DrawImGui = true)
	{
		m_Framebuffer->Bind();

		RenderUtils::SetViewport(m_LastViewportSize);
		RenderUtils::Clear();
		HandleCameraMovement(overrideFocus);

		if (m_Scene && m_Scene->GetPipeline())
		{
			if (m_Scene->GetActiveCamera() && m_ViewportMode == AppState::GAME)
				m_Scene->GetPipeline()->RenderFrame(m_Scene->GetActiveCamera());
			else
				m_Scene->GetPipeline()->RenderFrame(m_Camera);
		}

		m_Framebuffer->Unbind();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		bool open = true;
		if (DrawImGui)
		{
			ImGui::Begin(m_ViewportName.c_str(), &open);
			if (!open)
				m_Close = true;
		}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImVec2 viewportPanelPos = ImGui::GetWindowPos();
		vec2d ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		ViewportSize.x = MAX(ViewportSize.x, 1);
		ViewportSize.y = MAX(ViewportSize.y, 1);

		if (m_LastViewportSize.x != ViewportSize.x || m_LastViewportSize.y != ViewportSize.y)
		{
			m_LastViewportSize = ViewportSize;
			m_Framebuffer->Resize(ViewportSize.x, ViewportSize.y);
			RenderUtils::SetViewport(m_LastViewportSize);
			m_Camera->RecalculateViewProjectionMatrix();

			if (m_Scene && m_Scene->GetActiveCamera())
				m_Scene->GetActiveCamera()->RecalculateViewProjectionMatrix();
		}
		m_ViewportPos = vec2d(viewportPanelPos.x, viewportPanelPos.y);

		uint64_t textureID = m_Framebuffer->GetColorAttachement();
		ImVec2 PreViewportCursor = ImGui::GetCursorPos();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_LastViewportSize.x, m_LastViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		bool isFocused = ImGui::IsWindowFocused();
		m_WasFocusedOnviewport = m_FocusedOnviewport;
		m_FocusedOnviewport = isFocused;

		//clear input on unfocus
		if (!m_FocusedOnviewport && m_WasFocusedOnviewport)
		{
			ExitFocus();
		}

		//draw gizmo
		if (m_FocusedOnviewport)
		{
			if (DrawImGui)
				RenderGizmo();
			else
			{
				Bound bounds;
				bounds.min.x = ImGui::GetItemRectMin().x;
				bounds.min.y = ImGui::GetItemRectMin().y;
				bounds.max.x = ImGui::GetItemRectMax().x;
				bounds.max.y = ImGui::GetItemRectMax().y;
				RenderGizmo(bounds);
			}
		}

		if (DrawImGui)
		{
// 			if (ImGui::BeginDragDropTarget())
// 			{
// 				std::string PrefabassetTag = "Asset_PrefabAsset";
// 				std::string SceneassetTag = "Asset_SceneAsset";
// 				const ImGuiPayload* Prefabpayload = ImGui::AcceptDragDropPayload(PrefabassetTag.c_str());
// 				const ImGuiPayload* SceneAssetpayload = ImGui::AcceptDragDropPayload(SceneassetTag.c_str());
// 
// 				if (Prefabpayload)
// 				{
// 					std::string assetPath((char*)Prefabpayload->Data);
// 
// 					if (File::DoesFileExist(assetPath))
// 					{
// 						//AssetRef<PrefabAsset> assetRef(assetPath);
// 						if (m_Scene)
// 						{
// 							Transform trans;
// 							trans.pos += m_Camera->GetTransform().pos;
// 							trans.pos += World::GetForwardVector(m_Camera->GetTransform().rot) * 10.f;
// 
// 							//SceneUtils::SpawnPrefabInScene(assetRef, m_Scene.get(), trans, ObjectInitializer(), true);
// 						}
// 					}
// 				}
// 
// 				if (SceneAssetpayload)
// 				{
// 					std::string assetPath((char*)SceneAssetpayload->Data);
// 
// 					if (File::DoesFileExist(assetPath))
// 					{
// 						Ref<AssetHandle> handle = MakeRef<AssetHandle>(assetPath);
// 
// 						if (m_Scene)
// 						{
// 							Ref<SceneAsset> prefabAsset = m_Scene->GetApplication()->GetAssetManager().LoadAsset<SceneAsset>(handle);
// 							Ref<Scene> loaded = SceneUtils::CreateAndLoadSceneFromAsset(m_Scene->GetApplication(), prefabAsset);
// 							auto app = Cast<EditorApp>(m_Scene->GetApplication());
// 							app->m_EditorScene = loaded;
// 							m_Scene->GetApplication()->DestroyAppObject(m_Scene);
// 							m_Scene = loaded;
// 							m_SelectedComponent = nullptr;
// 							app->m_SceneObjectPannel.m_SelectedComponent = nullptr;
// 							app->m_SceneObjectPannel.m_SelectedObject = nullptr;
// 						}
// 					}
// 				}
// 
// 				ImGui::EndDragDropTarget();
// 			}
		}

		DrawMenu(PreViewportCursor);

		if (DrawImGui)
		{
			ImGui::End();
		}

		ImGui::PopStyleVar();
	}

	void HandleCameraMovement(bool overrideFocus = false)
	{
		if (m_Scene && (m_FocusedOnviewport || overrideFocus) && m_InputManager.IsKeyDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			EnterFocus();

			if ((m_ViewportMode == AppState::GAME && m_Scene->GetLastTick().GetTickGroup() == TickGroup::GAME && m_Scene->GetActiveCamera())) return;

			auto trans = m_Camera->GetTransform();

			//movement
			if (m_InputManager.IsKeyDown(GLFW_KEY_W))
			{
				trans.pos += World::GetForwardVector(trans.rot) * m_CameraMovementSpeed * m_Scene->GetLastTick().DeltaTime;
			}
			if (m_InputManager.IsKeyDown(GLFW_KEY_S))
			{
				trans.pos -= World::GetForwardVector(trans.rot) * m_CameraMovementSpeed * m_Scene->GetLastTick().DeltaTime;
			}
			if (m_InputManager.IsKeyDown(GLFW_KEY_A))
			{
				trans.pos -= World::GetRightVector(trans.rot) * m_CameraMovementSpeed * m_Scene->GetLastTick().DeltaTime;
			}
			if (m_InputManager.IsKeyDown(GLFW_KEY_D))
			{
				trans.pos += World::GetRightVector(trans.rot) * m_CameraMovementSpeed * m_Scene->GetLastTick().DeltaTime;
			}

			//Rotation 
			if (m_InputManager.IsKeyDown(GLFW_KEY_UP))
			{
				trans.rot.x += m_CameraRotationSpeed * m_Scene->GetLastTick().DeltaTime;
			}
			if (m_InputManager.IsKeyDown(GLFW_KEY_DOWN))
			{
				trans.rot.x -= m_CameraRotationSpeed * m_Scene->GetLastTick().DeltaTime;
			}
			if (m_InputManager.IsKeyDown(GLFW_KEY_LEFT))
			{
				trans.rot.y += m_CameraRotationSpeed * m_Scene->GetLastTick().DeltaTime;
			}
			if (m_InputManager.IsKeyDown(GLFW_KEY_RIGHT))
			{
				trans.rot.y -= m_CameraRotationSpeed * m_Scene->GetLastTick().DeltaTime;
			}

			if (!World::IsNearlyZero(m_LastMouseVector))
			{
				m_TargetCameraRotation.x += m_LastMouseVector.y * m_CameraRotationSpeed * m_Scene->GetLastTick().DeltaTime;
				m_TargetCameraRotation.y += -m_LastMouseVector.x * m_CameraRotationSpeed * m_Scene->GetLastTick().DeltaTime;
			}

			//60 fps timing
			trans.rot.y = World::Lerp(trans.rot.y, m_TargetCameraRotation.y, 60.f * m_Scene->GetLastTick().DeltaTime);
			trans.rot.x = World::Lerp(trans.rot.x, m_TargetCameraRotation.x, 60.f * m_Scene->GetLastTick().DeltaTime);

			m_LastMouseVector = { 0,0 };
			m_Camera->SetTransform(trans);
		}
		else
		{
			if (m_WasFocusedOnviewport)
			{
				ExitFocus();
			}
		}
	}

	void EnterFocus()
	{
		
	}

	void ExitFocus()
	{
		m_InputManager.ClearInput();
	}

	void RenderGizmo(const Bound& viewportBounds = Bound{})
	{
		if (m_ViewportMode == AppState::GAME) return;

		if (m_SelectedComponent.Get() == nullptr && m_SelectedObject.Get())
		{
			if (auto root = m_SelectedObject.Get()->GetRootComponent())
				m_SelectedComponent = root.get();
		}

		if (m_SelectedComponent.Get())
		{
			TransformComponent* selectedTransform = Cast<TransformComponent>(m_SelectedComponent.Get());
			if (selectedTransform)
			{
				ImGuizmo::SetOrthographic(m_Camera->GetProjectionType() == ProjectionType::ORTHO);
				ImGuizmo::SetDrawlist();
				if (viewportBounds.min == Bound{}.min && viewportBounds.max == Bound{}.max)
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, (float)ImGui::GetWindowWidth(), (float)ImGui::GetWindowHeight());
				else
				{
					ImGuizmo::SetRect(viewportBounds.min.x, viewportBounds.min.y, viewportBounds.max.x - viewportBounds.min.x, viewportBounds.max.y - viewportBounds.min.y);
				}
				const glm::mat4& proj = m_Camera->GetProjectionMatrix();
				const glm::mat4& view = m_Camera->GetViewMatrix();

				glm::mat4 transform = selectedTransform->GetWorldMatrix();
				glm::mat4 Delta = glm::mat4(1.0);
				float snapValues[3] = { 0.5f, 0.5f, 0.5f };
				ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), m_TransformMode, m_TransformSpace, glm::value_ptr(transform), glm::value_ptr(Delta));
				if (ImGuizmo::IsUsing())
				{
					if (selectedTransform->GetParent())
					{
						glm::mat4 WorldTrans = selectedTransform->GetWorldMatrix();
						transform = selectedTransform->GetMatrix() * glm::inverse(WorldTrans) * transform;

						Transform LocalTransform = selectedTransform->GetLocalTransform();
						Transform Result = World::MakeTransform(transform);
						vec3d deltaRotation = Result.rot - LocalTransform.rot;
						LocalTransform.pos = Result.pos;
						LocalTransform.rot += deltaRotation;
						LocalTransform.scale = Result.scale;
						selectedTransform->SetLocalTransform(LocalTransform);
					}
					else
					{
						Transform LocalTransform = selectedTransform->GetLocalTransform();
						Transform Result = World::MakeTransform(transform);
						vec3d deltaRotation = Result.rot - LocalTransform.rot;
						LocalTransform.pos = Result.pos;
						LocalTransform.rot += deltaRotation;
						LocalTransform.scale = Result.scale;
						selectedTransform->SetLocalTransform(LocalTransform);
					}
				}
			}
		}
	}


	void DrawMenu(const ImVec2& startPos)
	{
		ImVec2 start = startPos;
		ImGui::SetCursorPos({ m_MenuOffset.x + start.x, m_MenuOffset.y + start.y });
		std::string modeStr = "Mode : " + std::string(m_ViewportMode == AppState::GAME ? "Game" : "Editor");

		if (ImGui::Button(modeStr.c_str()))
		{
			m_ViewportMode = m_ViewportMode == AppState::GAME ? AppState::EDITOR : AppState::GAME;
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(100.f);
		ImGui::SliderFloat("Camera Speed", &m_CameraMovementSpeed, 10.f, 1000.f, "%.0f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushItemWidth(100.f);
		std::string spacestr = "Transform Space : " + std::string(m_TransformSpace == ImGuizmo::WORLD ? "World" : "Local");
		if (ImGui::Button(spacestr.c_str()))
		{
			m_TransformSpace = m_TransformSpace == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
		}
		ImGui::PopItemWidth();
	}


public:
	std::string m_ViewportName;
	vec2d m_LastViewportSize = { 1,1 };
	vec2d m_ViewportPos;
	bool m_FocusedOnviewport = false;
	bool m_WasFocusedOnviewport = false;
	bool m_RecieveInput = false;
	Ref<FrameBuffer> m_Framebuffer;
	Ref<Scene> m_Scene;
	Ref<Window> m_Window;
	Ref<Camera> m_Camera;

	float m_CameraMovementSpeed = 700.f;
	float m_CameraRotationSpeed = 100.f;
	vec3d m_TargetCameraRotation = { 0,0,0 };

	bool m_Close = false;

	InputManager m_InputManager;
	Callback<EventMouseMove> m_MouseCallback;
	Callback<EventKey> m_KeyEvent;
	vec2d m_LastMouseVector = { 0,0 };

	//Viewport menu offset
	vec2d m_MenuOffset = { 10.f, 10.f };

	AppState m_ViewportMode;

	ImGuizmo::OPERATION m_TransformMode = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE m_TransformSpace = ImGuizmo::LOCAL;

	//gizmo stuff
	bool m_DrawGizmo = true;
	SafeObjectPtr<ObjectComponent> m_SelectedComponent;
	SafeObjectPtr<SceneObject> m_SelectedObject;
};



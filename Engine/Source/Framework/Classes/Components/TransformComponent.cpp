#include "TransformComponent.h"
#include "DEngine.h"

void TransformComponent::OnPostConstruct()
{
	Super::OnPostConstruct();

// 	if (m_ParentRef)
// 	{
// 		for (auto& comp : GetOwner()->GetComponents())
// 		{
// 			if (comp->GetName() == m_ParentRef.ComponentName)
// 			{
// 				AttachTo(Cast<TransformComponent>(comp));
// 			}
// 		}	
// 	}
}

void TransformComponent::OnUpdate(const Tick& tick)
{
	Super::OnUpdate(tick);

	if (m_LastTransform != m_Transform)
	{
		ClearCache();
		m_LastTransform = m_Transform;
	}
}

glm::mat4 TransformComponent::GetWorldMatrix()
{
	if (m_Parent)
	{ 
		glm::mat4 parent = m_Parent->GetWorldMatrix();
		return parent * GetMatrix();
	}
	else
	{
		return  GetMatrix();
	}
}

bool TransformComponent::IsRootComponent() const
{
	return GetOwner()->GetRootComponent().get() == this;
}

std::vector<Ref<TransformComponent>> TransformComponent::GetChildren() const
{
	std::vector<Ref<TransformComponent>> out;

	for (auto& comp : GetOwner()->GetComponents())
	{
		if (auto& TransComp = Cast<TransformComponent>(comp))
		{
			if(TransComp->GetParent().get() == this && TransComp.get() != this)
				out.push_back(TransComp);			
		}
	}

	return out;
}

size_t TransformComponent::Serialize(Buffer& buffer) const
{
	BufferWritter writter(buffer, Super::Serialize(buffer));

	//if we have a parent then store a ref to them
	LocalComponentRef ParentRef;
	if(m_Parent)
		ParentRef.ComponentName = m_Parent->GetName();
	Buffer ParentBuffer;
	ParentRef.Serialize(ParentBuffer);
	writter.WriteBuffer(ParentBuffer);
	return writter.GetCurrentOffset();
}

size_t TransformComponent::Deserialize(const Buffer& buffer)
{
	BufferReader reader(buffer, Super::Deserialize(buffer));

	//read the buffer into the member var
	Buffer ParentBuffer;
	reader.ReadBuffer(ParentBuffer);
	m_ParentRef.Deserialize(ParentBuffer);
	return reader.GetCurrentOffset();
}


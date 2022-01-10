#include "ObjectBase.h"
#include "Utils/Rand.h"

void ObjectBase::Initialize(const ObjectInitializer& initializer)
{
	//assign the member initializer for later use
	m_ObjectInitializer = initializer;

	//flag initialized as true
	m_Initialized = true;

	//assign name, class name by default
	if (initializer.Name.empty())
		m_Name = GetClassType().Name;
	else
		m_Name = initializer.Name;

	//override AssociatedModuleName 
	if(!initializer.AssociatedModuleName.empty())
		SetAssociatedModuleName(initializer.AssociatedModuleName);

	//assign random ID
	if (initializer.Flags & ConstructFlags::RANDOMID)
	{
		m_ID = { Rand().Int64() };
	}

	//start invalid
	if (initializer.Flags & ConstructFlags::INVALIDATE)
	{
		m_IsValid = false;
	}

	//Define Properties
	if ((initializer.Flags & ConstructFlags::NOPROPS) == 0) //if the flag is not set
		DefineProperties();

	//call the virtual constructor
	if((initializer.Flags & ConstructFlags::NOCONSTRUCT) == 0) //if the flag is not set
		OnConstruct();

	//call the virtual post construct by default (this will usually disabled and called manually after deserialzation)
	if ((initializer.Flags & ConstructFlags::NOPOSTCONSTRUCT) == 0) //if the flag is not set
		OnPostConstruct();
}

uint ObjectBase::Serialize(Buffer& buffer) const
{
	return 0;
}

uint ObjectBase::Deserialize(const Buffer& buffer)
{
	return 0;
}

Buffer ObjectBase::GeneratePropBuffer() const
{
	return Buffer{};
}

void ObjectBase::LoadPropsFromBuffer(const Buffer& buffer)
{
	
}


ObjectInitializer ObjectInitializer::Module(ObjectBase* ExistantModuleObject)
{
	ASSERT(ExistantModuleObject);

	ObjectInitializer init;
	init.AssociatedModuleName = ExistantModuleObject->GetAssociatedModuleName();
	return init;
}

ObjectInitializer ObjectInitializer::Module(const std::string& AssociatedModuleName)
{
	ObjectInitializer init;
	init.AssociatedModuleName = AssociatedModuleName;
	return init;
}

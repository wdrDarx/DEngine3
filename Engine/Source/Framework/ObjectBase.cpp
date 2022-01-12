#include "ObjectBase.h"
#include "Utils/Rand.h"
#include "Utils/ModuleUtils.h"

void ObjectBase::Initialize(const ObjectInitializer& initializer)
{
	PROFILE_FUNC()

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

size_t ObjectBase::Serialize(Buffer& buffer) const
{
	PROFILE_FUNC()

	BufferWritter writter(buffer, 0);
	writter.Write(&GetID(), sizeof(UID));
	writter.WriteString(GetName());
	writter.Write(&GetObjectFlags(), sizeof(ObjectFlags));
	writter.WriteBuffer(GeneratePropBuffer());

	return writter.GetCurrentOffset();
}

size_t ObjectBase::Deserialize(const Buffer& buffer)
{
	PROFILE_FUNC()

	BufferReader reader(buffer, 0);
	reader.Read(&m_ID, sizeof(UID));
	reader.ReadString(m_Name);
	reader.Read(&m_ObjectFlags, sizeof(ObjectFlags));

	Buffer PropBuffer;
	reader.ReadBuffer(PropBuffer);
	LoadPropsFromBuffer(PropBuffer);

	return reader.GetCurrentOffset();
}

Buffer ObjectBase::GeneratePropBuffer() const
{
	PROFILE_FUNC()

	BufferArray PropsArray;

	for (auto& prop : GetProperties())
	{
		PropsArray.AddPiece(prop->MakeBuffer());
	}

	return PropsArray.MakeBuffer();
}

void ObjectBase::LoadPropsFromBuffer(const Buffer& buffer)
{
	PROFILE_FUNC()

	BufferArray PropsArray(buffer);

	for (auto& piece : PropsArray.GetDataPieces())
	{
		Property prop;
		prop.LoadAllMetadata(buffer); //only need to load the metadata for checking

		for (auto& p : GetProperties()) //search all current props for corresponding name and type
		{	
			if (p->GetName() == prop.GetName() && p->GetType() == prop.GetType()) 
			{
				//Found the matching prop, now deserialize it 
				p->FromBuffer(piece);
				break;
			}
		}
	}
}

ObjectInitializer::ObjectInitializer()
{
	AssociatedModuleName = GetCurrentModuleName();
}

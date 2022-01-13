#include "StructBase.h"
#include "DEngine.h"

Buffer StructBase::MakeBuffer() const
{
	PROFILE_FUNC()

	BufferArray PropsArray;

	for (auto& prop : GetProperties())
	{
		PropsArray.AddPiece(prop->MakeBuffer());
	}

	return PropsArray.MakeBuffer();
}

void StructBase::FromBuffer(const Buffer& buffer)
{
	PROFILE_FUNC()

	BufferArray PropsArray(buffer);

	for (auto& piece : PropsArray.GetDataPieces())
	{
		Property prop;
		prop.LoadAllMetadata(piece); //only need to load the metadata for checking

		for (auto& p : GetProperties()) //search all current props for corresponding name
		{
			if (p->GetName() == prop.GetName())
			{
				//Found the matching prop, now deserialize it 
				p->FromBuffer(piece);
				break;
			}
		}
	}
}

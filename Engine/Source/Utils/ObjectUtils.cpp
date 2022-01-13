#include "ObjectUtils.h"
#include "DEngine.h"

bool ObjectUtils::HasPropertyChanged(const Property& CheckAgainst, const ObjectBase& DefaultObject)
{
	Property* test = nullptr;
	for (auto& p : DefaultObject.GetProperties())
	{
		if (p->GetName() == CheckAgainst.GetName() && p->GetType() == CheckAgainst.GetType())
		{
			test = p.get();
			break;
		}
	}

	if (!test)
	{
		LOG_ERROR("Could not find property to check against in the base object!");
		return false;
	}

	return !(*test == CheckAgainst);
}

void ObjectUtils::ResetProperty(const std::string& PropName, ObjectBase& TargetObject)
{
	ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
	Ref<ObjectBase> BaseObj = ToRef<ObjectBase>(reg.MakeObjectFromClassName(TargetObject.GetClassType().Name));
	BaseObj->DefineProperties();

	for (auto& p : TargetObject.GetProperties())
	{
		if (p->GetName() == PropName)
		{
			for (auto& BaseProp : BaseObj->GetProperties())
			{
				if (BaseProp->GetName() == p->GetName())
				{
					p->ValueFromBuffer(p->m_Value, BaseProp->MakeValueBuffer(BaseProp->GetValue()));
					return;
				}
			}
		}
	}
}

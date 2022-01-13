#pragma once
#include "Core/Core.h"
#include "Framework/Tick.h"
#include "Serialization/Buffer.h"
#include "Framework/Property.h"
#include "Framework/StaticClass.h"

enum ConstructFlags
{
	//assign a random ID on construct
	RANDOMID = BIT(0),

	//start invalid
	INVALIDATE = BIT(1),

	//dont call OnConstruct()
	NOCONSTRUCT = BIT(2),

	//dont call OnPostConstruct()
	NOPOSTCONSTRUCT = BIT(3),

	//dont define properties
	NOPROPS = BIT(4)
};

enum ObjectFlags
{
	//the object is a prefab
	PREFAB = BIT(0)
};

//contains initializer values for an object
class ObjectBase;
struct ObjectInitializer
{
	//TODO make name non-copy
	std::string Name;

	/*  
		optional name of associated module of this object (used mainly for auto deleting objects when a module gets unloaded)
		WILL OVERRIDE m_AssociatedModuleName WHEN AN OBJECT IS INITIALIZED WITH THIS
    */
	std::string AssociatedModuleName;

	//Construct flags
	int Flags = 0;

	ObjectInitializer(const std::string& name, ConstructFlags flags) : ObjectInitializer()
	{
		Name = name;
		Flags = flags;
	}

	ObjectInitializer(const std::string& name) : ObjectInitializer()
	{
		Name = name;
	}

	ObjectInitializer(ConstructFlags flags) : ObjectInitializer((int)flags)
	{

	}

	ObjectInitializer(int flags) : ObjectInitializer()
	{
		Flags = flags;
	}

	ObjectInitializer();
};


class DENGINE_API _placeholder
{
protected:
	virtual size_t Serialize(Buffer& buffer)
	{
		return 0;
	}

	virtual size_t Deserialize(const Buffer& buffer)
	{
		return 0;
	}
};


/*
	Simple class that has a property system, serialization interface,
	a unique assignable id and an Event interface.

	has an empty constructor and an explicit initializer constructor
	that needs to be called manually - Initialize(const ObjectInitializer& initializer)

	also has a constructor that will call initializer - ObjectBase(const ObjectInitializer& initializer)
*/
class DENGINE_API ObjectBase : public _placeholder
{
public:
	using Super = _placeholder;
	using ThisClass = ObjectBase;

	//empty constructor
	ObjectBase()
	{
	
	}

	virtual ~ObjectBase()
	{

	}

	//basically the actual constructor
	void Initialize(const ObjectInitializer& initializer);

	/*
		used to create the object itself (components, etc)
		Called before Deserialization so properties wont have their values - Use OnPostConstruct()
	*/
	virtual void OnConstruct()
	{

	}

	/*
		called after any data has been deserialized and the object is complete 
	*/
	virtual void OnPostConstruct()
	{

	}

	/*
		called per frame
	*/
	virtual void OnUpdate(const Tick& tick)
	{

	}

	/*
		Called manually by parent containers of this object, and only when this object is a real real object not just a temporary
	*/
	virtual void OnDestroy()
	{

	}

	//use to define props here with PROPDEF(x)
	virtual void DefineProperties()
	{
		
	}

	const UID& GetID() const
	{
		return m_ID;
	}

	void SetID(const UID& id)
	{
		m_ID = id;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	std::string& GetNameMutable()
	{
		return m_Name;
	}
	
	void SetName(const std::string& name)
	{
		m_Name = name;
	}

	//automatically serializes props, can be customized and overriden
	//return value is the final offet of the buffer (use this offset for chain serialization)
	virtual size_t Serialize(Buffer& buffer) const;

	//automatically Deserializes props, can be customized and overriden
	//return value is the final offet of the buffer (use this offset for chain serialization)
	virtual size_t Deserialize(const Buffer& buffer);

	//Saves all current properties as a buffer array outputed as a single buffer
	Buffer GeneratePropBuffer() const;

	//Loads all found props in a buffer 
	void LoadPropsFromBuffer(const Buffer& buffer);

	//this is manually overriden on all object classes with a macro
	virtual ClassType GetClassType() const
	{
		return typeid(this);
	}

	//this is manually overriden on all object classes with a macro
	virtual ClassType GetParentClassType() const
	{
		return typeid(Super);
	}

	//get static class - use OBJECT_STATIC_CLASS()
	template<class T>
	static StaticClass _GetStaticClass()
	{
		StaticClass out;
		out.FromTemlate<T>();
		return out;
	}

	const ObjectInitializer& GetObjectInitializer() const
	{
		return m_ObjectInitializer;
	}

	void SetAssociatedModuleName(const std::string& name)
	{
		m_AssociatedModuleName = name;
	}

	const std::string& GetAssociatedModuleName() const
	{
		return m_AssociatedModuleName;
	}

	//true if Initialize() has been called
	bool IsInitialized() const
	{
		return m_Initialized;
	}

	//misc flags
	const ObjectFlags& GetObjectFlags() const
	{
		return m_ObjectFlags;
	}

	//misc flags
	ObjectFlags& GetObjectFlagsMutable()
	{
		return m_ObjectFlags;
	}

	const std::vector<Ref<Property>>& GetProperties() const
	{
		return m_Properties;
	}

protected:

	//Array of properties that point to members of this class
	std::vector<Ref<Property>> m_Properties;

private:

	//Misc flags
	ObjectFlags m_ObjectFlags;

	//set true after initialize is called (needed for checking things like is this a static class or an actual object?)
	bool m_Initialized = false;

	//copy of the initializer that has been used when initializing
	ObjectInitializer m_ObjectInitializer;

	//WILL GET OVERRIDEN BY THE OBJECT INITIALIZER
	std::string m_AssociatedModuleName;

	//unique assignable ID
	UID m_ID;

	//name of the object (could be empty)
	std::string m_Name = "";
};
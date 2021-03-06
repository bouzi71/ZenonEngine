#pragma once

#include "Guid.h"


// FORWARD BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE IBaseManager;
ZN_INTERFACE IPropertiesGroup;
ZN_INTERFACE IByteBuffer;
ZN_INTERFACE IXMLReader;
ZN_INTERFACE IXMLWriter;

ZN_INTERFACE IObject;
ZN_INTERFACE IObjectLoadSave;
// FORWARD END


ZN_INTERFACE ZN_API	IObjectLoadSave
{
	virtual ~IObjectLoadSave() {}

	virtual std::shared_ptr<IObject> Copy() const = 0;
	virtual void CopyTo(std::shared_ptr<IObject> Destination) const = 0;
	virtual void Load(const std::shared_ptr<IByteBuffer>& Buffer) = 0;
	virtual void Save(const std::shared_ptr<IByteBuffer>& Buffer) const = 0;
	virtual void Load(const std::shared_ptr<IXMLReader>& Reader) = 0;
	virtual void Save(const std::shared_ptr<IXMLWriter>& Writer) const = 0;

	// Templates

	template <typename T>
	inline std::shared_ptr<T> CopyCast() const
	{
		return std::dynamic_pointer_cast<T>(Copy());
	}
};

ZN_INTERFACE ZN_API IObject
	: public virtual IObjectLoadSave
{
	virtual ~IObject() {}

	virtual Guid GetGUID() const = 0;
	virtual std::string GetObjectTypeName() const = 0;
	virtual std::string GetObjectClassName() const = 0;

	virtual std::string GetName() const = 0;
	virtual void SetName(const std::string& Name) = 0;

	virtual const IBaseManager& GetBaseManager() const = 0;
	virtual std::shared_ptr<IPropertiesGroup> GetProperties() const = 0;

	inline bool IsType(ObjectType Type) const { return GetType() == Type; }
	inline bool IsClass(ObjectClass Class) const { return GetClass() == Class; }
	inline ObjectClass GetType() const { return GetGUID().GetObjectClass(); }
	inline ObjectClass GetClass() const { return GetGUID().GetObjectClass(); }
	inline ObjectCounterType GetID() const { return GetGUID().GetCounter(); }

	inline bool operator==(const IObject& rhs) const { return GetGUID() == rhs.GetGUID(); }
	inline bool operator==(const IObject* rhs) const { return operator==(*rhs); }

	inline bool operator!=(const IObject& rhs) const { return GetGUID() != rhs.GetGUID(); }
	inline bool operator!=(const IObject* rhs) const { return operator!=(*rhs); }
};


ZN_INTERFACE ZN_API IObjectPrivate
{
	virtual ~IObjectPrivate() {}

	virtual void SetGUID(const Guid& NewGuid) = 0;
};




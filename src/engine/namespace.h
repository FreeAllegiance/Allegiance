#pragma once

#include <tref.h>

#include "font.h"
#include "model.h"

//////////////////////////////////////////////////////////////////////////////
//
// NameSpace
//
//////////////////////////////////////////////////////////////////////////////

class IEngineFont;
class Geo;
class MDLType;

class INameSpace : public IObject {
public:
    virtual const ZString&   GetName()                                           = 0;

    virtual void             AddMember(const ZString& strName, IObject* pobject) = 0;
    virtual IObject*         FindMember(const ZString& strName)                  = 0;
    virtual IObject*         FindMemberLocal(const ZString& strName)             = 0;
    virtual IObject*         FindMemberAndNameSpace(const ZString& str, INameSpace*& pns) = 0;

    virtual void             AddType(const ZString& strName, MDLType* ptype)     = 0;
    virtual MDLType*         FindType(const ZString& strName)                    = 0;
    virtual MDLType*         FindTypeLocal(const ZString& strName)               = 0;

    virtual void             WriteTypeHeader(ZFile* pfile)                       = 0;
    virtual void             WriteToTextFile(int indent, ZFile* pfile)           = 0;
    virtual void             WriteToBinaryFile(ZFile* pfile)                     = 0;

	float FindNumber(const ZString& strName, float valueDefault = 0)
	{
		TRef<Number> pnumber; CastTo(pnumber, FindMember(strName));
		return pnumber->GetValue();
		//return pnumber ? pnumber->GetValue() : valueDefault;
	}

	bool FindBoolean(const ZString& strName, bool valueDefault = false)
	{
		TRef<Boolean> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue->GetValue();
		//return pvalue ? pvalue->GetValue() : valueDefault;
	}

	IEngineFont* FindFont(const ZString& strName, IEngineFont* valueDefault = NULL)
	{
		TRef<FontValue> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue->GetValue();
		//return pvalue ? pvalue->GetValue() : valueDefault;
	}

	IObjectList* FindList(const ZString& strName)
	{
		TRef<IObjectList> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue;
	}

	const ZString& FindString(const ZString& strName, const ZString& valueDefault = ZString())
	{
		TRef<StringValue> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue->GetValue();
		//return pvalue ? pvalue->GetValue() : valueDefault;
	}

	const Color& FindColor(const ZString& strName, const Color& valueDefault = Color::White())
	{
		TRef<ColorValue> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue->GetValue();
		//return pvalue ? pvalue->GetValue() : valueDefault;
	}

	const Point& FindPoint(const ZString& strName, const Point& valueDefault = Point(0, 0))
	{
		TRef<PointValue> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue->GetValue();
		//return pvalue ? pvalue->GetValue() : valueDefault;
	}

	WinPoint FindWinPoint(const ZString& strName, const WinPoint& valueDefault = WinPoint(0, 0))
	{
		TRef<PointValue> pvalue; CastTo(pvalue, FindMember(strName));
		return WinPoint::Cast(pvalue->GetValue());
		//return pvalue ? WinPoint::Cast(pvalue->GetValue()) : valueDefault;
	}

	const Vector& FindVector(const ZString& strName, const Vector& valueDefault = Vector(0, 0, 0))
	{
		TRef<VectorValue> pvalue; CastTo(pvalue, FindMember(strName));
		return pvalue->GetValue();
		//return pvalue ? pvalue->GetValue() : valueDefault;
	}

	TRef<Image> FindImage(const ZString& strName)
	{
		TRef<Image> pvalue; CastTo(pvalue, (Value*)FindMember(strName));
		return pvalue;
	}

	TRef<Geo> FindGeo(const ZString& strName)
	{
		TRef<Geo> pvalue; CastTo(pvalue, (Value*)FindMember(strName));
		return pvalue;
	}

};

typedef TList<TRef<INameSpace> > INameSpaceList;
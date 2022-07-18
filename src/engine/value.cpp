#include "value.h"

#include <base.h>
#include <matrix.h>

#include "LogFile.h"

//////////////////////////////////////////////////////////////////////////////
//
// Constructors
//
//////////////////////////////////////////////////////////////////////////////

#ifdef EnableDebugString
    void Value::UpdateDebugString()
    {
        m_strDebug = GetString(1);
    }
#endif

Value::Value() :
    m_bChanged(true),
    m_pchildren(0, 0)
{
}

Value::Value(Value* pvalue0) :
    m_bChanged(true),
    m_pchildren(1, 1)
{
    m_pchildren.Set(0, pvalue0);

    pvalue0->AddParent(this);
}

Value::Value(Value* pvalue0, Value* pvalue1) :
    m_bChanged(true),
    m_pchildren(2, 2)
{
    m_pchildren.Set(0, pvalue0);
    m_pchildren.Set(1, pvalue1);

    pvalue0->AddParent(this);
    pvalue1->AddParent(this);
}

Value::Value(Value* pvalue0, Value* pvalue1, Value* pvalue2) :
    m_bChanged(true),
    m_pchildren(3, 3)
{
    m_pchildren.Set(0, pvalue0);
    m_pchildren.Set(1, pvalue1);
    m_pchildren.Set(2, pvalue2);

    pvalue0->AddParent(this);
    pvalue1->AddParent(this);
    pvalue2->AddParent(this);
}

Value::Value(Value* pvalue0, Value* pvalue1, Value* pvalue2, Value* pvalue3) :
    m_bChanged(true),
    m_pchildren(4, 4)
{
    m_pchildren.Set(0, pvalue0);
    m_pchildren.Set(1, pvalue1);
    m_pchildren.Set(2, pvalue2);
    m_pchildren.Set(3, pvalue3);

    pvalue0->AddParent(this);
    pvalue1->AddParent(this);
    pvalue2->AddParent(this);
    pvalue3->AddParent(this);
}

//////////////////////////////////////////////////////////////////////////////
//
// Destructor
//
//////////////////////////////////////////////////////////////////////////////

Value::~Value()
{
    int count = m_pchildren.GetCount();
    for (int index = 0; index < count; index++) {
        m_pchildren[index]->RemoveParent(this);
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Private functions
//
//////////////////////////////////////////////////////////////////////////////

void Value::Evaluate()
{
}

TRef<Value> Value::Fold()
{
    return NULL;
}

bool Value::DoFold()
{
    bool bAnyFold = false;

    //
    // Fold all of the children first
    //

    int count = m_pchildren.GetCount();
    for (int index = 0; index < count; index++) {
        if (m_pchildren[index]->DoFold()) {
            bAnyFold = true;
        }
    }

    //
    // Now fold this node
    //

    TRef<Value> pvalueFold = Fold();

    if (pvalueFold) {
        if (pvalueFold != this) {
            ChangeTo(pvalueFold);
        }
        return true;
    }

    return bAnyFold;
}

void Value::ChildChanged(Value* pvalue, Value* pvalueNew)
{
    if (pvalueNew != NULL) {
        int count = m_pchildren.GetCount();
        for (int index = 0; index < count; index++) {
            if (pvalue == m_pchildren[index]) {
                SetChild(index, pvalueNew);
            }
        }

        // !!! DoFold();
    }

    Changed();
}

void Value::InternalUpdate()
{
    m_bChanged = false;

    int count = m_pchildren.GetCount();
    for (int index = 0; index < count; index++) {
        if (m_pchildren[index]->HasChanged()) {
            m_pchildren[index]->InternalUpdate();
        }
    }

    Evaluate();

    #ifdef EnableDebugString
        UpdateDebugString();
    #endif
}

void Value::Update()
{
    if (HasChanged()) {
        InternalUpdate();
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Protected functions
//
//////////////////////////////////////////////////////////////////////////////

void Value::Changed()
{
    #ifdef EnableDebugString
        UpdateDebugString();
    #endif

    if (!m_bChanged) {
        m_bChanged = true;

        for (auto iter = m_listParents.begin(); iter != m_listParents.end(); ++iter) {
            Value* pvalue = *iter;
            pvalue->ChildChanged(this, NULL);
        }
    }
}

void Value::ChangeTo(Value* pvalue)
{
    ZAssert(pvalue != this);
    TRef<Value> pvalueSave = this;

    while (m_listParents.begin() != m_listParents.end()) {
        (*m_listParents.begin())->ChildChanged(this, pvalue);
    };
}

bool Value::IsConstant()
{
    return false;
}

void Value::AddParent(Value* pvalue)
{
    m_listParents.insert(m_listParents.begin(), pvalue);
}

void Value::RemoveParent(Value* pvalue)
{
    auto iter = m_listParents.begin();
    while (iter != m_listParents.end()) {
        if (*iter == pvalue) {
            iter = m_listParents.erase(iter);
        }
        else {
            ++iter;
        }
    }

    if (m_listParents.size() == 0) {
        OnNoParents();
    }
}

void Value::SetChild(int index, Value* pvalueChild)
{
	// BT - 10/17 - If there are no elements in the list, then just add this element.
	// Fixing crash: 8975122	211205	allegiance.exe	allegiance.exe	tvector.h	215	4	0	Win32 StructuredException at 0044750A : UNKNOWN	2017-10-05 11:54:01	0x0004750A	10	UNKNOWN
	if (m_pchildren.GetCount() == 0)
	{
		AddChild(pvalueChild);
	}
	else if (m_pchildren[index] != pvalueChild) {
        m_pchildren[index]->RemoveParent(this);
        pvalueChild->AddParent(this);
        m_pchildren.Set(index, pvalueChild);
        Changed();
    }
}

void Value::SetChildSilently(int index, Value* pvalueChild)
{
    //this does not call Changed(), the caller is responsible for making sure the updates are handled correctly
    m_pchildren[index]->RemoveParent(this);
    pvalueChild->AddParent(this);
    m_pchildren.Set(index, pvalueChild);
}

void Value::AddChild(Value* pvalueChild)
{
    m_pchildren.PushEnd(pvalueChild);
    pvalueChild->AddParent(this);
}

//////////////////////////////////////////////////////////////////////////////
//
// Binary Files
//
//////////////////////////////////////////////////////////////////////////////

void Value::WriteInternal(IMDLBinaryFile* pfile)
{
    if (m_pnsInfo != NULL) {
        //
        // This value is top level in some NameSpace
        //

        pfile->WriteReference(m_pnsInfo->GetName());
    } else {
        Write(pfile);
    }
}

void Value::WriteChildren(IMDLBinaryFile* pfile)
{
    int count = m_pchildren.GetCount();
    for (int index = count - 1; index >= 0; index--) {
        m_pchildren[index]->WriteInternal(pfile);
    }
}

void Value::Write(IMDLBinaryFile* pfile)
{
    WriteChildren(pfile);
    pfile->WriteReference(GetFunctionName());
    pfile->WriteApply();
}

void Value::FillImportTable(IMDLBinaryFile* pfile)
{
    if (m_pnsInfo) {
        if (pfile->AddImport(m_pnsInfo)) {
            return;
        }
    }

    ZString str = GetFunctionName();

    if (!str.IsEmpty()) {
        pfile->AddImport(str);
    }

    int count = m_pchildren.GetCount();
    for (int index = count - 1; index >= 0; index--) {
        m_pchildren[index]->FillImportTable(pfile);
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Public functions
//
//////////////////////////////////////////////////////////////////////////////

void Value::SetNameSpaceInfo(INameSpaceInfo* pnsInfo)
{
    m_pnsInfo = pnsInfo;
}

int Value::GetNodeCount()
{
    int nodes = 1;

    int count = m_pchildren.GetCount();
    for (int index = 0; index < count; index++) {
        nodes += m_pchildren[index]->GetNodeCount();
    }

    return nodes;
}

ZString Value::GetFunctionName()
{
    ZUnimplemented();

    return ZString();
}

ZString Value::GetChildString(int indent)
{
    if (m_pnsInfo != NULL) {
        return m_pnsInfo->GetName();
    } else {
        return GetString(indent);
    }
}

ZString Value::Indent(int indent)
{
    static const char* pchSpaces = "                                                                ";

    ZAssert(indent * 2 < 64);

    return ZString(pchSpaces, indent * 2);
}

ZString Value::GetString(int indent)
{
    // name ( args )

    ZString str = GetFunctionName() + "(\n";

    int count = m_pchildren.GetCount();
    for (int index = 0; index < count; index++) {
        str += Indent(indent + 1) + m_pchildren[index]->GetChildString(indent + 1);

        if (index != count - 1) {
            str += ",\n";
        } else {
            str += "\n";
        }
    }

    return str + Indent(indent) + ")";
}

//////////////////////////////////////////////////////////////////////////////
//
// Value List
//
//////////////////////////////////////////////////////////////////////////////

ValueList::ValueList(Site* psite) :
    m_psite(psite),
    m_list(),
    m_iter(m_list.begin())
{
    ZAssert(psite == NULL || (((DWORD)psite) > 0x10));
}

ValueList::~ValueList()
{
    Value* pvalue = GetFirst();
    while (pvalue) {
        pvalue->RemoveParent(this);
        pvalue = GetNext();
    }
}

void ValueList::PushFront(Value* pvalue)
{
    pvalue->AddParent(this);
    m_list.insert(m_list.begin(), pvalue);
    m_iter = m_list.begin();
    Changed();
}

void ValueList::PushEnd(Value* pvalue)
{
    pvalue->AddParent(this);
    m_list.push_back(pvalue);
    m_iter = m_list.begin();
    Changed();
}

void ValueList::Remove(Value* pvalueArg)
{
    TRef<Value> pvalue = pvalueArg;

    auto iter = m_list.begin();
    while (iter != m_list.end()) {
        if ((Value*)*iter == pvalueArg) {
            iter = m_list.erase(iter);

            pvalue->RemoveParent(this);
            Changed();
        }
        else {
            ++iter;
        }
    }
    m_iter = m_list.begin();
}

bool ValueList::DoFold()
{
    //
    // Fold all of the children
    //

    bool bAnyFold = false;

    TRef<Value> pvalue = GetFirst();
    while (pvalue) {
        if (pvalue->DoFold()) {
            bAnyFold = true;
        }

        pvalue = GetNext();
    }

    return bAnyFold;
}

void ValueList::ChildChanged(Value* pvalue, Value* pvalueNew)
{
    auto found_iter = std::find_if(m_list.begin(), m_list.end(), [pvalue](const TRef<Value>& entry) {
        return pvalue == (Value*)entry;
        });
    ZAssert(found_iter != m_list.end());
    ZAssert(pvalue != pvalueNew); //would be a waste if it was called with the same argument

    if (pvalueNew) {
        pvalue->RemoveParent(this);
        pvalueNew->AddParent(this);
        if (m_psite != NULL && m_psite->RemoveValue(pvalueNew)) {
            m_list.erase(found_iter);
            m_iter = m_list.begin();
        } else {
            *found_iter = pvalueNew;
        }
    }

    Changed();
}

void ValueList::InternalUpdate()
{
    m_bChanged = false;

    Value* pvalue = GetFirst();
    while (pvalue) {
        if (pvalue->HasChanged()) {
            pvalue->InternalUpdate();
        }
        pvalue = GetNext();
    }
}

int ValueList::GetNodeCount()
{
    int nodes = 1;

    Value* pvalue = GetFirst();
    while (pvalue) {
        nodes += pvalue->GetNodeCount();
        pvalue = GetNext();
    }

    return nodes;
}

int ValueList::GetCount()
{
    return m_list.size();
}

Value* ValueList::GetFirst()
{
    m_iter = m_list.begin();
    if (m_list.begin() != m_list.end()) {
        return *m_list.begin();
    }

    return NULL;
}

Value* ValueList::GetLast()
{
    if (m_list.size() > 0) {
        m_iter = --m_list.end();
        return *m_iter;
    }

    return NULL;
}

Value* ValueList::GetNext()
{
    if (m_iter == m_list.end()) {
        return NULL;
    }
    if (++m_iter != m_list.end()) {
        return *m_iter;
    }

    return NULL;
}

Value* ValueList::GetPrevious()
{
    if (m_iter != m_list.begin()) {
        m_iter--;
        return *m_iter;
    }

    return NULL;
}

Value* ValueList::GetCurrent()
{
    if (m_iter != m_list.end()) {
        return *m_iter;
    } else {
        return NULL;
    }
}

Value* ValueList::RemoveCurrent()
{
    m_iter = m_list.erase(m_iter);
    return GetCurrent();
}

bool ValueList::IsFirst()
{
    return m_iter == m_list.begin();
}

bool ValueList::IsLast()
{
    return m_iter == --m_list.end();
}

ZString ValueList::GetString(int indent)
{
    // [ args ]

    ZString str = "[\n";

    return str + Indent(indent) + "]";
}

void ValueList::Write(IMDLBinaryFile* pmdlFile)
{
}

void ValueList::FillImportTable(IMDLBinaryFile* pfile)
{
}

class OrNumber : public Number {
public:
    OrNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() =
            (float)(
            ((DWORD)Get0()->GetValue())
                | ((DWORD)Get1()->GetValue())
                );
    }
};

class AndNumber : public Number {
public:
    AndNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() =
            (float)(
            ((DWORD)Get0()->GetValue())
                & ((DWORD)Get1()->GetValue())
                );
    }
};

class XOrNumber : public Number {
public:
    XOrNumber(Number* pvalue0, Number* pvalue1) :
        Number(pvalue0, pvalue1)
    {
    }

    Number* Get0() { return Number::Cast(GetChild(0)); }
    Number* Get1() { return Number::Cast(GetChild(1)); }

    void Evaluate()
    {
        GetValueInternal() =
            (float)(
            ((DWORD)Get0()->GetValue())
                ^ ((DWORD)Get1()->GetValue())
                );
    }
};

TRef<Number> Or(Number* pvalue1, Number* pvalue2)
{
    return new OrNumber(pvalue1, pvalue2);
}

TRef<Number> And(Number* pvalue1, Number* pvalue2)
{
    return new AndNumber(pvalue1, pvalue2);
}

TRef<Number> XOr(Number* pvalue1, Number* pvalue2)
{
    return new XOrNumber(pvalue1, pvalue2);
}

//////////////////////////////////////////////////////////////////////////////
//
// GetString
//
//////////////////////////////////////////////////////////////////////////////

ZString GetString(int indent, const ZString& str)
{
    //  , handle escape charaters within the string
    return "\"" + str + "\"";
}

ZString GetString(int indent, const Color& color)
{
    return
          "Color("
        + ZString(color.GetRed()  ) + ", "
        + ZString(color.GetGreen()) + ", "
        + ZString(color.GetBlue() ) + ")";
}

ZString GetString(int indent, const Point& point)
{
    return
          "Point("
        + ZString(point.X()) + ", "
        + ZString(point.Y()) + ")";
}

ZString GetString(int indent, const WinPoint& point)
{
	return
		"WinPoint("
		+ ZString(point.X()) + ", "
		+ ZString(point.Y()) + ")";
}

ZString GetString(int indent, const Rect& Rect)
{
    return
          "Rect("
        + ZString(Rect.XMin()) + ", "
        + ZString(Rect.YMin()) + ", "
        + ZString(Rect.XMax()) + ", "
        + ZString(Rect.YMax()) + ")";
}

ZString GetString(int indent, const Orientation& orient)
{
    return
          "Orientation("
        + GetString(indent, orient.GetUp()) + ", "
        + GetString(indent, orient.GetForward()) + ")";
}

ZString GetString(int indent, const Vector& vec)
{
    return
          "Vector("
        + ZString(vec.X()) + ", "
        + ZString(vec.Y()) + ", "
        + ZString(vec.Z()) + ")";
}

ZString GetString(int indent, float value)
{
    return ZString(value);
}

ZString GetString(int indent, bool value)
{
    return value ? "true" : "false";
}

ZString GetString(int indent, const Matrix& mat)
{
    ZString str;

    str = "Matrix(\n";
    str += Value::Indent(indent + 1) + ZString(mat[0][0]) + ", "+ ZString(mat[0][1]) + ", "+ ZString(mat[0][2]) + ", "+ ZString(mat[0][3]) + ",\n";
    str += Value::Indent(indent + 1) + ZString(mat[1][0]) + ", "+ ZString(mat[1][1]) + ", "+ ZString(mat[1][2]) + ", "+ ZString(mat[1][3]) + ",\n";
    str += Value::Indent(indent + 1) + ZString(mat[2][0]) + ", "+ ZString(mat[2][1]) + ", "+ ZString(mat[2][2]) + ", "+ ZString(mat[2][3]) + ",\n";
    str += Value::Indent(indent + 1) + ZString(mat[3][0]) + ", "+ ZString(mat[3][1]) + ", "+ ZString(mat[3][2]) + ", "+ ZString(mat[3][3]) + "\n";
    str += Value::Indent(indent) + ")";

    return str;
}

ZString GetString(int indent, const Matrix2& mat)
{
    return
          "Matrix2(\n"
        + Value::Indent(indent + 1) + ZString(mat[0][0]) + ", "+ ZString(mat[0][1]) + ", "+ ZString(mat[0][2]) + ",\n"
        + Value::Indent(indent + 1) + ZString(mat[1][0]) + ", "+ ZString(mat[1][1]) + ", "+ ZString(mat[1][2]) + ",\n"
        + Value::Indent(indent + 1) + ZString(mat[2][0]) + ", "+ ZString(mat[2][1]) + ", "+ ZString(mat[2][2]) + "\n"
        + Value::Indent(indent) + ")";
}

//////////////////////////////////////////////////////////////////////////////
//
// Write to binary file
//
//////////////////////////////////////////////////////////////////////////////

ZString GetFunctionName(const ZString& value)
{
    return ZString();
}

void Write(IMDLBinaryFile* pmdlFile, const ZString& value)
{
    pmdlFile->WriteString(value);
}

ZString GetFunctionName(float value)
{
    return ZString();
}

void Write(IMDLBinaryFile* pmdlFile, float value)
{
    pmdlFile->WriteNumber(value);
}

ZString GetFunctionName(bool value)
{
    return ZString();
}

void Write(IMDLBinaryFile* pmdlFile, bool value)
{
    pmdlFile->WriteBoolean(value);
}

ZString GetFunctionName(const Orientation& value)
{
    return "Orientation";
}

void Write(IMDLBinaryFile* pmdlFile, const Orientation& value)
{
    pmdlFile->WriteReference("Orientation");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    pfile->Write((void*)&value, sizeof(value));
}

ZString GetFunctionName(const Rect& value)
{
    return "Rect";
}

void Write(IMDLBinaryFile* pmdlFile, const Rect& value)
{
    pmdlFile->WriteReference("Rect");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    pfile->Write((void*)&value, sizeof(value));
}

ZString GetFunctionName(const Color& value)
{
    return "Color";
}

void Write(IMDLBinaryFile* pmdlFile, const Color& value)
{
    pmdlFile->WriteReference("Color");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    pfile->Write((void*)&value, sizeof(value));
}

ZString GetFunctionName(const Point& value)
{
    return "Point";
}

ZString GetFunctionName(const WinPoint& value)
{
	return "WinPoint";
}

void Write(IMDLBinaryFile* pmdlFile, const Point& value)
{
    pmdlFile->WriteReference("Point");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    pfile->Write((void*)&value, sizeof(value));
}

void Write(IMDLBinaryFile* pmdlFile, const WinPoint& value)
{
	pmdlFile->WriteReference("WinPoint");
	TRef<ZFile> pfile = pmdlFile->WriteBinary();
	pfile->Write((void*)&value, sizeof(value));
}

ZString GetFunctionName(const Vector& value)
{
    return "Vector";
}

void Write(IMDLBinaryFile* pmdlFile, const Vector& value)
{
    pmdlFile->WriteReference("Vector");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    pfile->Write((void*)&value, sizeof(value));
}

ZString GetFunctionName(const Matrix& value)
{
    return "Matrix";
}

void Write(IMDLBinaryFile* pmdlFile, const Matrix& value)
{
    pmdlFile->WriteReference("Matrix");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();
    pfile->Write((void*)&value, sizeof(value));
}

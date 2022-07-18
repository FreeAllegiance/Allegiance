#include "mdl.h"

#include <base.h>
#include <token.h>

#include "enginep.h"
#include "model.h"
#include "namespace.h"
#include "value.h"

//////////////////////////////////////////////////////////////////////////////
//
// MDL Types
//
//
// bits, integer, short, char, etc
// String
// TRef<Type>
// TRef<IObjectList> [Type]
// struct
//
//////////////////////////////////////////////////////////////////////////////

class StringMDLType : public MDLType {
private:
public:
    int GetSize()
    {
        return sizeof(ZString);
    }

    void Destruct(BYTE*& pbyte)
    {
        ((ZString*)pbyte)->m_pdata = NULL;
        pbyte += sizeof(ZString);
    }

    void StackRead(BYTE*& pbyte, ObjectStack& stack)
    {
        TRef<StringValue> pstring; CastTo(pstring, (IObject*)stack.Pop());

        *(ZString*)pbyte = pstring->GetValue();
        pbyte += sizeof(ZString);
    }

    void Read(BYTE*& pbyte, IBinaryReaderSite* psite, ObjectStack& stack)
    {
        *(ZString*)pbyte = psite->GetString();
        pbyte += sizeof(ZString);
    }

    ZString GetCPPType()
    {
        return "ZString";
    }

    ZString GetHungarianPrefix()
    {
        return "str";
    }
};

TRef<MDLType> CreateStringMDLType()
{
    return new StringMDLType();
}

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class PointerMDLType : public MDLType {
private:
    TRef<MDLType> m_ptype;

public:
    PointerMDLType(MDLType* ptype) :
        m_ptype(ptype)
    {
    }

    int GetSize()
    {
        return 4;
    }

    void Destruct(BYTE*& pbyte)
    {
        *(TRef<IObject>*)pbyte = NULL;
        pbyte += 4;
    }

    void StackRead(BYTE*& pbyte, ObjectStack& stack)
    {
        TRef<IObject> pobject = stack.Pop();

        *(TRef<IObject>*)pbyte = pobject;
        pbyte += 4;
    }

    void Read(BYTE*& pbyte, IBinaryReaderSite* psite, ObjectStack& stack)
    {
        TRef<IObject> pobject = stack.Pop();

        *(TRef<IObject>*)pbyte = pobject;
        pbyte += 4;
    }

    ZString GetCPPType()
    {
        return "TRef< " + m_ptype->GetCPPType() + " >";
    }

    ZString GetHungarianPrefix()
    {
        return "p";
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class ListMDLType : public MDLType {
private:
    TRef<MDLType> m_ptype;

public:
    ListMDLType(MDLType* ptype) :
        m_ptype(ptype)
    {
    }

    int GetSize()
    {
        return 4;
    }

    void Destruct(BYTE*& pbyte)
    {
        *(TRef<IObjectList>*)pbyte = NULL;
        pbyte += 4;
    }

    void StackRead(BYTE*& pbyte, ObjectStack& stack)
    {
        TRef<IObjectList> plist; CastTo(plist, (IObject*)stack.Pop());

        *(TRef<IObjectList>*)pbyte = plist;
        pbyte += 4;
    }

    void Read(BYTE*& pbyte, IBinaryReaderSite* psite, ObjectStack& stack)
    {
        TRef<IObjectList> plist; CastTo(plist, (IObject*)stack.Pop());

        *(TRef<IObjectList>*)pbyte = plist;
        pbyte += 4;
    }

    ZString GetCPPType()
    {
        return "TRef<IObjectList>";
    }

    ZString GetHungarianPrefix()
    {
        return "list";
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class IObjectMDLType : public MDLType {
private:
    ZString m_strType;
    ZString m_strPrefix;

public:
    IObjectMDLType(const ZString& strType, const ZString& strPrefix) :
        m_strType(strType),
        m_strPrefix(strPrefix)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // MDLType methods
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetSize()
    {
        ZUnimplemented();
        return 0;
    }

    virtual bool IsStruct()
    {
        return true;
    }

    void Destruct(BYTE*& pbyte)
    {
        ZUnimplemented();
    }

    void StackRead(BYTE*& pbyte, ObjectStack& stack)
    {
        ZUnimplemented();
    }

    void Read(BYTE*& pbyte, IBinaryReaderSite* psite, ObjectStack& stack)
    {
        ZUnimplemented();
    }

    ZString GetCPPType()
    {
        return m_strType;
    }

    ZString GetHungarianPrefix()
    {
        return m_strPrefix;
    }
};

TRef<MDLType> CreateIObjectMDLType(const ZString& strType, const ZString& strPrefix)
{
    return new IObjectMDLType(strType, strPrefix);
}

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class StructMDLType : public MDLType {
private:
    class MemberData {
    public:
        ZString       m_str;
        TRef<MDLType> m_ptype;
    };
    typedef TList<MemberData> MemberDataList;

    MemberDataList m_list;
    ZString        m_strName;
    ZString        m_strParent;

public:
    StructMDLType(const ZString& strName, const ZString& strParent) :
        m_strName(strName),
        m_strParent(strParent)
    {
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // StructMDLType methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddMember(const ZString& str, MDLType* ptype)
    {
        m_list.PushEnd();
        m_list.GetEnd().m_str   = str;
        m_list.GetEnd().m_ptype = ptype;
    }

    ZString GetCPPString(const ZString& strIdentifier)
    {
        ZString str =
            "class " + strIdentifier + " : public " + m_strParent + " {\n"
            "public:\n";

        int offset = 0;
        MemberDataList::Iterator iter(m_list);

        while (!iter.End()) {
            MemberData& data = iter.Value();

            // float& GetChar() { return *(float*)(GetDataPointer() + 0); } 

            ZString strType = data.m_ptype->GetCPPType();
            ZString strName = data.m_str;
            strName = strName.Left(1).ToUpper() + strName.RightOf(1);

            str +=
                  "    "
                + strType
                + "& Get"
                + strName
                + "() { return *("
                + strType
                + "*)(GetDataPointer() + "
                + ZString(offset)
                + "); }\n";

            offset += data.m_ptype->GetSize();

            /*
            str +=
                  strType
                + "m_"
                + data.m_ptype->GetHungarianPrefix()
                + data.m_str
                + ";\n";
            */

            iter.Next();
        }

        str += "};\n";

        return str;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // MDLType methods
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetSize()
    {
        int size = 0;

        MemberDataList::Iterator iter(m_list);

        while (!iter.End()) {
            MemberData& data = iter.Value();

            size += data.m_ptype->GetSize();

            iter.Next();
        }

        return size;
    }

    virtual bool IsStruct()
    {
        return true;
    }

    void Destruct(BYTE*& pbyte)
    {
        MemberDataList::Iterator iter(m_list);

        while (!iter.End()) {
            MemberData& data = iter.Value();

            data.m_ptype->Destruct(pbyte);

            iter.Next();
        }
    }

    void StackRead(BYTE*& pbyte, ObjectStack& stack)
    {
        MemberDataList::Iterator iter(m_list);

        while (!iter.End()) {
            MemberData& data = iter.Value();

            data.m_ptype->StackRead(pbyte, stack);

            iter.Next();
        }
    }

    void Read(BYTE*& pbyte, IBinaryReaderSite* psite, ObjectStack& stack)
    {
        MemberDataList::Iterator iter(m_list);

        while (!iter.End()) {
            MemberData& data = iter.Value();

            data.m_ptype->Read(pbyte, psite, stack);

            iter.Next();
        }
    }

    ZString GetCPPType()
    {
        return m_strName;
    }

    ZString GetHungarianPrefix()
    {
        ZUnimplemented();
        return ZString();
    }

    /*
    void Write(BYTE*& pbyte, IMDLBinaryFile* pmdlFile)
    {
        m_node->Write(pmdlFile);
        pmdlFile->WriteReference("QuickChatMenuItem");
        TRef<ZFile> pfile = pmdlFile->WriteBinary();

        pfile->WriteStructure(pobject->m_char);
        pfile->Write(m_string);
    }
    */
};

ZString GetStructHeader(MDLType* ptype, const ZString& strIdentifier)
{
    StructMDLType* pstructType; CastTo(pstructType, ptype);

    return pstructType->GetCPPString(strIdentifier);
}

//////////////////////////////////////////////////////////////////////////////
//
// Generic Factory
//
//////////////////////////////////////////////////////////////////////////////

class MDLObjectFactory : public IFunction {
private:
    TRef<MDLType> m_ptype;

public:
    MDLObjectFactory(MDLType* ptype) :
        m_ptype(ptype)
    {
    }

	TRef<IObject> Apply(ObjectStack& stack)
	{
        TRef<IMDLObject> pobject = IMDLObject::Allocate(m_ptype->GetSize(), m_ptype);
        BYTE*            pbyte   = pobject->GetDataPointer();

        m_ptype->StackRead(pbyte, stack);

        return pobject;
	}

    TRef<IObject> Read(IBinaryReaderSite* psite, ObjectStack& stack)
    {
        TRef<IMDLObject> pobject = IMDLObject::Allocate(m_ptype->GetSize(), m_ptype);
        BYTE*            pbyte   = pobject->GetDataPointer();

        m_ptype->Read(pbyte, psite, stack);

        return pobject;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Default IFunction::Read function causes an ZAssert
//
//////////////////////////////////////////////////////////////////////////////

TRef<IObject> IFunction::Read(IBinaryReaderSite* psite, ObjectStack& stack)
{
    ZUnimplemented();
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//
// ObjectPair
//
//////////////////////////////////////////////////////////////////////////////

IObject* IObjectPair::GetNth(int index)
{
    ZAssert(index >= 0);

    if (index == 0) {
        return m_pfirst;
    } else {
        IObjectPair* ppair; CastTo(ppair, m_psecond);

        return ppair->GetNth(index - 1);
    }
}

IObject* IObjectPair::GetLastNth(int index)
{
    ZAssert(index >= 0);

    if (index == 0) {
        return m_pfirst;
    } else if (index == 1) {
        return m_psecond;
    } else {
        IObjectPair* ppair; CastTo(ppair, m_psecond);

        return ppair->GetLastNth(index - 1);
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Tokenizer for text MDL Files
//
//////////////////////////////////////////////////////////////////////////////

class MDLTextToken : public TextTokenImpl {
public:
    int Comma       ;
    int SemiColon   ;
    int Colon       ;
    int LeftParen   ;
    int RightParen  ;
    int LeftBracket ;
    int RightBracket;
    int LeftCurly   ;
    int RightCurly  ;
    int Equals      ;
    int OrToken     ;
    int Let         ;
    int In          ;
    int True        ;
    int False       ;
    int Use         ;
    int Struct      ;
    int Extends     ;

    MDLTextToken(PCC pcc, int length) :
        TextTokenImpl(pcc, length)
    {
        Comma        = AddToken("','");
        SemiColon    = AddToken("';'");
        Colon        = AddToken("':'");
        LeftParen    = AddToken("'('");
        RightParen   = AddToken("')'");
        LeftBracket  = AddToken("'['");
        RightBracket = AddToken("']'");
        LeftCurly    = AddToken("'{'");
        RightCurly   = AddToken("'}'");
        Equals       = AddToken("'='");
        OrToken      = AddToken("'|'");
        Let          = AddSymbol("let");
        In           = AddSymbol("in");
        True         = AddSymbol("true");
        False        = AddSymbol("false");
        Use          = AddSymbol("use");
        Struct       = AddSymbol("struct");
        Extends      = AddSymbol("extends");

        Next();
    }

    int ParseToken(PCC& pcc)
    {
        switch (pcc[0]) {
            case '[': pcc++; return LeftBracket;
            case ']': pcc++; return RightBracket;
            case '{': pcc++; return LeftCurly;
            case '}': pcc++; return RightCurly;
            case '(': pcc++; return LeftParen;
            case ')': pcc++; return RightParen;
            case ',': pcc++; return Comma;
            case ';': pcc++; return SemiColon;
            case ':': pcc++; return Colon;
            case '=': pcc++; return Equals;
            case '|': pcc++; return OrToken;
        }

        return 0;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Parser for MDL Files
//
////////////////
// MDL grammer
////////////////
//
// ArgList        : [Expression [, Expression]]
//
// Term           : ( Expression )
//                  String
//                  Number
//                  Symbol
//                  Symbol ( ArgList )
//                  '[' LISTOF Expression , ']'
//                  let DefinitionList in Expression
//
// Apply          : [Apply] Term
//
// Or             : [Or |] Apply
//
// Pair           : Apply [, Pair]
//
// Expression     : Pair
//
// Definition     : Struct ;
//                  Symbol = Expression ;
//
// DefinitionList : Definition [ DefinitionList ]
//
/////////////////
// Type Grammer
/////////////////
//
// Type       :
//              Symbol
//              [ Type ]
//
// Member     : Symbol ':' Type ;
//
// MemberList : Member MemberList
// 
// Struct     : struct Symbol [extends Symbol] { MemberList }
//
//////////////////////////////////////////////////////////////////////////////

class MDLParser : public IObject {
    //////////////////////////////////////////////////////////////////////////////
    //
    // Data members
    //
    //////////////////////////////////////////////////////////////////////////////

    MDLTextToken* m_ptoken;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    MDLParser(PCC pcc, int length)
    {
        m_ptoken = new MDLTextToken(pcc, length);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    ~MDLParser()
    {
        delete m_ptoken;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool Error()
    {
        return m_ptoken->Error();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadType(INameSpace* pns, TRef<MDLType>& ptype, bool bError)
    {
        ZString strType;

        if (m_ptoken->Is(m_ptoken->LeftBracket, false)) {
            TRef<MDLType> m_ptypeItem;

            if (ReadType(pns, m_ptypeItem, true)) {
                if (m_ptoken->Is(m_ptoken->RightBracket, true)) {
                    ptype = new ListMDLType(m_ptypeItem);
                    return true;
                }
            }
        } else if (m_ptoken->IsSymbol(strType, bError)) {
            ptype = pns->FindType(strType);
            if (ptype != NULL) {
                return true;
            }
            ZError("Undefined indentifier " + strType);
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadMember(INameSpace* pns, StructMDLType* pstructType, bool bError)
    {
        ZString strMember;

        if (m_ptoken->IsSymbol(strMember, bError)) {
            if (m_ptoken->Is(m_ptoken->Colon, true)) {
                TRef<MDLType> ptypeMember;

                if (ReadType(pns, ptypeMember, true)) {
                    if (m_ptoken->Is(m_ptoken->SemiColon, true)) {
                        if (ptypeMember->IsStruct()) {
                            pstructType->AddMember(
                                strMember,
                                new PointerMDLType(
                                    ptypeMember
                                )
                            );
                        } else {
                            pstructType->AddMember(strMember, ptypeMember);
                        }
                        return true;
                    }
                }                
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadMemberList(INameSpace* pns, StructMDLType* pstructType, bool bError)
    {
        ZString strMember;

        if (ReadMember(pns, pstructType, bError)) {
            return ReadMemberList(pns, pstructType, false);
        }

        return !bError;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadStruct(INameSpace* pns, bool bError)
    {
        ZString strType;

        if (m_ptoken->IsSymbol(strType, bError)) {
            if (g_bMDLLog) {
                ZDebugOutput("Reading struct: " + strType + "\n");
            }
            TRef<StructMDLType> pstructType;

            if (m_ptoken->Is(m_ptoken->Extends, false)) {
                ZString strParent;
                if (m_ptoken->IsSymbol(strParent, true)) {
                    pstructType = new StructMDLType(strType, strParent);
                } else {
                    return false;
                }
            } else {
                pstructType = new StructMDLType(strType, "IMDLObject");
            }

            if (m_ptoken->Is(m_ptoken->LeftCurly, true)) {
                if (ReadMemberList(pns, pstructType, true)) {
                    if (m_ptoken->Is(m_ptoken->RightCurly, true)) {
                        if (m_ptoken->Is(m_ptoken->SemiColon, true)) {
                            pns->AddType(strType, pstructType);
                            pns->AddMember(strType, new MDLObjectFactory(pstructType));
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadList(INameSpace* pns, IObjectList* plist, bool bError)
    {
        TRef<IObject> pobject;
        if (ReadApply(pns, pobject, bError)) {
            if (m_ptoken->Is(m_ptoken->Comma, false)) {
                if (ReadList(pns, plist, true)) {
                    plist->PushFront(pobject);
                }
            } else {
                plist->PushFront(pobject);
            }
        }

        if (bError) {
            return !m_ptoken->Error();
        } else {
            return true;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadTerminal(INameSpace* pns, TRef<IObject>& pobject, bool bError)
    {
        ZString str;
        float   value;

        if (m_ptoken->IsString(str, false)) {
            pobject = new StringValue(str);
            return true;
        } else if (m_ptoken->IsNumber(value, false)) {
            pobject = new Number(value);
            return true;
        } else if (m_ptoken->Is(m_ptoken->True, false)) {
            pobject = new Boolean(true);
            return true;
        } else if (m_ptoken->Is(m_ptoken->False, false)) {
            pobject = new Boolean(false);
            return true;
        } else if (m_ptoken->IsSymbol(str, false)) {
            //
            // an Identifier
            //

            pobject = pns->FindMember(str);
            if (pobject != NULL) {
                return true;
            }

            m_ptoken->SetError("Undefined identifier " + str);
        } else if (m_ptoken->Is(m_ptoken->LeftParen, false)) {
            if (ReadExpression(pns, pobject, true)) {
                if (m_ptoken->Is(m_ptoken->RightParen, true)) {
                    return true;
                }
            }
        } else if (m_ptoken->Is(m_ptoken->LeftBracket, false)) {
            //
            // a list
            //

            TRef<IObjectList> plist = new IObjectList();

            if (ReadList(pns, plist, false)) {
                if (m_ptoken->Is(m_ptoken->RightBracket, true)) {
                    pobject = plist;
                    return true;
                }
            }
        } if (m_ptoken->Is(m_ptoken->Let, false)) {
            //
            // let defs in expr
            //

            TRef<INameSpace> pnsLet = CreateNameSpace(ZString(), pns);

            if (ReadDefinitionList(pnsLet, true)) {
                if (m_ptoken->Is(m_ptoken->In, true)) {
                    if (ReadExpression(pnsLet, pobject, true)) {
                        return true;
                    }
                }
            }
        }

        if (bError) {
            m_ptoken->SetErrorUnexpected();
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadOr(INameSpace* pns, TRef<IObject>& pobject, bool bError)
    {
        if (ReadTerminal(pns, pobject, bError)) {
            while (m_ptoken->Is(m_ptoken->OrToken, false)) {
                TRef<IObject> pobject2;

                if (ReadTerminal(pns, pobject2, true)) {
                    pobject = 
						Or(
							Number::Cast((IObject*)pobject), 
							Number::Cast((IObject*)pobject2)
						);

                    continue;
                }

                return false;
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadArgumentPair(INameSpace* pns, ObjectStack& stack)
    {
        TRef<IObject> pobject;

        if (ReadApply(pns, pobject, true)) {
            if (m_ptoken->Is(m_ptoken->Comma, false)) {
                if (!ReadArgumentPair(pns, stack)) {
                    return false;
                }
            }

            stack.Push(pobject);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadApply(INameSpace* pns, TRef<IObject>& pobject, bool bError)
    {
        if (ReadOr(pns, pobject, bError)) {
            while (true) {
                if (m_ptoken->Is(m_ptoken->LeftParen, false)) {
                    IFunction* pfunction; CastTo(pfunction, pobject);

                    ObjectStack stack;

                    if (m_ptoken->Is(m_ptoken->RightParen, false)) {
                        pobject = pfunction->Apply(stack);
                        continue;
                    } else if (ReadArgumentPair(pns, stack)) {
                        if (m_ptoken->Is(m_ptoken->RightParen, true)) {
                            pobject = pfunction->Apply(stack);
                            continue;
                        }
                    }

                    return false;
                }

                return true;
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadPair(INameSpace* pns, TRef<IObject>& pobject, bool bError)
    {
        if (ReadApply(pns, pobject, bError)) {
            if (m_ptoken->Is(m_ptoken->Comma, false)) {
                TRef<IObject> pobject2;

                if (ReadPair(pns, pobject2, true)) {
                    pobject = new IObjectPair(pobject, pobject2);

                    return true;
                }

                return false;
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadExpression(INameSpace* pns, TRef<IObject>& pobject, bool bError)
    {
        return ReadPair(pns, pobject, bError);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadDefinition(INameSpace* pns, bool bError)
    {
        ZString strIdentifier;

        if (m_ptoken->Is(m_ptoken->Struct, false)) {
            return ReadStruct(pns, true);
        } else if (m_ptoken->IsSymbol(strIdentifier, bError)) {
            if (g_bMDLLog) {
			    ZDebugOutput("Reading identifier: " + strIdentifier + "\n");
            }
            if (m_ptoken->Is(m_ptoken->Equals, true)) {
                TRef<IObject> pobject;

                if (ReadExpression(pns, pobject, true)) {
                    if (m_ptoken->Is(m_ptoken->SemiColon, true)) {
                        pns->AddMember(strIdentifier, pobject);
                        return true;
                    }
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadDefinitionList(INameSpace* pns, bool bError)
    {
        while (ReadDefinition(pns, false)) {
        }

        return !m_ptoken->Error();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadImports(Modeler* pmodeler, INameSpaceList& list)
    {
        while (m_ptoken->Is(m_ptoken->Use, false)) {
            ZString str;

            if (m_ptoken->IsString(str, true)) {
                if (m_ptoken->Is(m_ptoken->SemiColon, true)) {
                    TRef<INameSpace> pns = pmodeler->GetNameSpace(str);

                    if (pns) {
                        list.PushEnd(pns);
                        continue;
                    }
                }
            }

            return false;
        }

        return true;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Read an MDL File
//
//////////////////////////////////////////////////////////////////////////////

TRef<INameSpace> CreateNameSpace(const ZString& str, Modeler* pmodeler, ZFile* pfile)
{
    TRef<MDLParser> pparser = new MDLParser((PCC)pfile->GetPointer(), pfile->GetLength());

    INameSpaceList list;

    if (pparser->ReadImports(pmodeler, list)) {
        TRef<INameSpace> pns = ::CreateNameSpace(str, list);

        if (pparser->ReadDefinitionList(pns, true)) {
            return pns;
        }
    }

    return NULL;
}

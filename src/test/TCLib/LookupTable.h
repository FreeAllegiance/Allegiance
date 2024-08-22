#pragma once

/////////////////////////////////////////////////////////////////////////////
// LookupTable.h | Declaration of a simple lookup table.

#pragma warning(disable: 4786)

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Declares a template structure that holds a pair of values of types
// /key/ and /Value/.  This is used by the TCLookupTable macros.
//
// Template Parameters:
//   Key - The type of the m_key public data member.
//   Value - The type of the m_value public data member;
//
// See Also: TCLookupTable Macros, TCLookupTable_DECLARE
template <class Key, class Value, class KeyRef = Key const &>
struct TCLookupTablePair
{
  typedef Key       KeyT;  // {secret}
  typedef Value   ValueT;  // {secret}
  typedef KeyRef KeyRefT;  // {secret}

  KeyT   m_key;    // {secret}
  ValueT m_value;  // {secret}
};

/////////////////////////////////////////////////////////////////////////////
// Iterates through the entries of the specified lookup table until either
// the specified /key/ is found or the number of table entries specified by
// /nSize/ have been searched.
//
// This is used by the TCLookupTable_FIND macro and probably has limited use
// outside of that macro.
//
// Template Parameters:
//   Key - The type of the table's keys.
//   Value - The type of the table's associated values.
// Parameters:
//   pTable - The address of the first entry in the table.
//   nSize - The number of table entries in the table.
//   key - The key to be located in the table.
//   value - A reference to where the located key's associated value will be
//   copied. If the key is not found, this parameter is not used.
//
// Return Value: true if the specified key was located in the specified
// table, otherwise false;
//
// See Also: TCLookupTable Macros, TCLookupTable_FIND
template <class Key, class Value, class KeyRef>
bool TCLookupTable_Find(TCLookupTablePair<Key, Value, KeyRef>* ptable,
  UINT nSize, KeyRef key, Value& value)
{
  for (UINT i = 0; i < nSize; i++)
  {
    if (key == ptable[i].m_key)
    {
      value = ptable[i].m_value;
      return true;
    }
  }
  return false;
}

#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // Macro Group: TCLookupTable Macros
  //
  // There is often the need during development to define lookup tables for a
  // variety of uses.  Regardless of the use, this involves mapping a
  // /key/ to one or more /values/.  For dynamic lookup tables, those that
  // will be built and modified during run-time, an obvious choice might be
  // the STL std::map class. However, many lookup tables are static in nature
  // and, as such, can be defined at compile time and do not change during
  // the course of the program. The TCLookupTable macros provide a reusable
  // mechanism to assist in the declaration and initialization of such static
  // lookup tables.
  //
  // Note: These macros are not designed to be used to declare static tables
  // in template classes. This is mostly due to limitations of the
  // C++ preprocessor which, for example, make it difficult (or impossible)
  // to specify a template class name with multiple template arguments as a
  // macro parameter; the comma's get in the way. However, for efficiency
  // reasons, using static tables in a template class is *not* recommended,
  // anyway.
  // It could be inefficient since multiple template instances would each
  // have their own copy of the static data. If you need a static table in a
  // template class, derive the template class from a non-template base class
  // that declares (and initializes) the table. Be sure to make the tables
  // *public* or *protected* in the non-template base class so that the
  // derived template class has access to them.
  //
  // To declare a static lookup table in your class, add the
  // TCLookupTable_DECLARE macro inside your class declaration:
  //
  //      class CMyClass
  //      {
  //        // ...(Other members)...
  //      …
  //      // Data Members
  //      protected:
  //        TCLookupTable_DECLARE(fruits, int, LPCTSTR)
  //      };
  //
  // The above example declares a static lookup table named "fruits" as a
  // protected data member of CMyClass. The table will map integers to
  // strings. For the sake of the example, forget that all localizable
  // strings should be put into String Table resources.
  //
  // A class can declare as many tables as needed. If the CMyClass also
  // needed a table of vegetables, it could simply specify the macro again,
  // with a different table name:
  //
  //      class CMyClass
  //      {
  //        // ...(Other members)...
  //      …
  //      // Data Members
  //      protected:
  //        TCLookupTable_DECLARE(fruits, int, LPCTSTR)
  //        TCLookupTable_DECLARE(vegetables, int, LPCTSTR)
  //      };
  //
  // To initialize each table, use the TCLookupTable_ENTRY macros, enclosed
  // within the TCLookupTable_BEGIN and TCLookupTable_END macros. Keep in
  // mind that the table is a static data member of the class. So the best
  // place to initialize the static data members is in your class
  // implementation (CPP) file:
  //
  //      // (In MyClass.cpp)
  //      // Static Initialization
  //      TCLookupTable_BEGIN(CMyClass, fruits)
  //        TCLookupTable_ENTRY(10, TEXT("Apple"))
  //        TCLookupTable_ENTRY(20, TEXT("Banana"))
  //        TCLookupTable_ENTRY(30, TEXT("Orange"))
  //      TCLookupTable_END()
  //
  // The above example only shows the TCLookupTable_ENTRY macro, but several
  // other macros are provided to assist in the declaration of other common
  // scenarios, such as when the associated value type is a structure.
  //
  // Now that your lookup table is initialized, the TCLookupTable_FIND macro
  // allows a key and its associated value to easily be found in the table:
  //
  //      LPCTSTR CMyClass::FruitToString(int idFruit) const
  //      {
  //        // Lookup the specified fruit ID
  //        LPCTSTR pszName;
  //        bool bFound = TCLookupTable_FIND(fruit, idFruit, pszName);
  //        return bFound ? pszName : NULL;
  //      }
  //
  // While the above example demonstrates the most common use for a lookup
  // table, it is sometimes useful to manually iterate through the table
  // entries. To assist with this, the TCLookupTable_SIZE macro is provided.
  // As its name implies, it provides the number of entries in the table. The
  // table consists of a regular array of TCLookupTablePair structures. Since
  // this structure is a template, the TCLookupTable_DECLARE macro declares
  // a type definition consisting of the table name suffixed with _ValueType.
  // In the following example, the /entry/ variable is a reference of type
  // /fruit_ValueType/, which is the type definition declared by the
  // TCLookupTable_DECLARE macro:
  //
  //      void CMyClass::DumpFruitTable() const
  //      {
  //        for (int i = 0; i < TCLookupTable_SIZE(fruit); ++i)
  //        {
  //          fruit_ValueType& entry = fruit[i];
  //          _TRACE2("Fruit ID %d = %s\n", entry.m_key, entry.m_value);
  //        }
  //      }
  //
  // See Also: TCLookupTablePair, TCLookupTable_DECLARE,
  // TCLookupTable_DECLARE, TCLookupTable_BEGIN, TCLookupTable_END,
  // TCLookupTable_ENTRY Macros, TCLookupTable_SIZE, TCLookupTable_FIND,
  // TCLookupTable_Find
  #define TCLookupTable
#endif // _DOCJET_ONLY

/////////////////////////////////////////////////////////////////////////////
// Declares a static lookup table as a static member variable. Also declares
// a type definition for the entries of the table by suffixing the
// /table/ parameter with _ValueType.
//
// Use this macro within a class declaration. More than one table can be
// declared in a class by specifying a different /table/ name in each macro
// statement.
//
// Parameters:
//   table - The table name, which doubles as the name of the static member
//   variable.
//   Key - The type of the table's keys.
//   Value - The type of the table's associated values.
//
// See Also: TCLookupTable Macros, TCLookupTable_BEGIN, TCLookupTablePair
#define TCLookupTable_DECLARE(table, Key, Value)                            \
  typedef TCLookupTablePair<Key, Value> table##_ValueType;                  \
  static table##_ValueType table [];

#define TCLookupTable_DECLARE_REF(table, Key, Value, KeyRef)                \
  typedef TCLookupTablePair<Key, Value, KeyRef> table##_ValueType;          \
  static table##_ValueType table [];

/////////////////////////////////////////////////////////////////////////////
// Begins the initialization scope of a static lookup table. This scope
// should contain TCLookupTable_ENTRY macros and should end with the
// TCLookupTable_END macro.
//
// Since this initializes a static member variable of the specified class, it
// needs to be specified *outside* of the class declaration and positioned so
// that it is included exactly *once* by the linker. The best place to
// initialize the static data members is in the class implementation (CPP)
// file.
//
// Parameters:
//   theClass - the name of the class which declares the static lookup table.
//   table - the name of the table in /theClass/.
//
// See Also: TCLookupTable Macros, TCLookupTable_ENTRY Macros,
// TCLookupTable_END
#define TCLookupTable_BEGIN(theClass, table) \
  theClass::table##_ValueType theClass::table [] = {

/////////////////////////////////////////////////////////////////////////////
// {alias: TCLookupTable_ENTRY2, TCLookupTable_ENTRY3, TCLookupTable_ENTRY4}
// {alias: TCLookupTable_ENTRY_PREFIX, TCLookupTable_ENTRY_SUFFIX}
// Initializes an entry in a static lookup table.  These macros should only
// be used within the initialization scope of a static lookup table. This
// scope should begin with the TCLookupTable_BEGIN macro and should end with
// the TCLookupTable_END macro.
//
// TCLookupTable_ENTRY is the simplest of these macros, taking just a key
// and a value with which to initialize the table entry.
//
// TCLookupTable_ENTRY2 through TCLookupTable_ENTRY4 require the key and from
// 2 to 4 value parameters, respectively. The multiple value parameters are
// used to initialize the members of a structure when a structure is used as
// the table's value type.
//
// TCLookupTable_ENTRY_PREFIX and TCLookupTable_ENTRY_SUFFIX are used when
// the key and/or value are symbolic and the symbols contain some element of
// identical text. For example, if a lookup table was being used to map error
// code identifiers to string table identifiers, the following sets of
// entries would be equivalent:
//
// This way is tedious and the repetition may introduce typing errors:
//
//        TCLookupTable_ENTRY(E_FILENOTFOUND, IDS_E_FILENOTFOUND)
//        TCLookupTable_ENTRY(E_INPUTIS2UGLY, IDS_E_INPUTIS2UGLY)
//
// This way results in the exact same table initialization:
//
//        TCLookupTable_ENTRY_PREFIX(E_FILENOTFOUND, IDS_)
//        TCLookupTable_ENTRY_PREFIX(E_INPUTIS2UGLY, IDS_)
//
// TODO: It may also be useful to provide a macro to "stringize" the key
// symbol to derive the value:
//
//        #define TCLookupTable_ENTRY_STRING(key) \
//          TCLookupTable_Entry(key, TEXT(#key))
// 
// Parameters:
//   key - The value with which to initialize the table entry's key.
//   value - The value to which the table entry's key is associated.
//   value1_thru_value4 - The values with which to initialize the structure
//   associated with the table entry's key.
//   prefix - Used to create a value name based on the key parameter prefixed
//   with the specified parameter.
//   suffix - Used to create a value name based on the key parameter suffixed
//   with the specified parameter.
//
// Macro Group: TCLookupTable_ENTRY Macros
//
// Declaration:
// #define TCLookupTable_ENTRY(key, value)
// #define TCLookupTable_ENTRY2(key, value1, value2)
// #define TCLookupTable_ENTRY3(key, value1, value2, value3)
// #define TCLookupTable_ENTRY4(key, value1, value2, value3, value4)
// #define TCLookupTable_ENTRY_PREFIX(key, prefix)
// #define TCLookupTable_ENTRY_SUFFIX(key, suffix)
//
// See Also: TCLookupTable Macros, TCLookupTable_BEGIN, TCLookupTable_END
#define TCLookupTable_ENTRY(key, value) \
  { key, value },

/////////////////////////////////////////////////////////////////////////////
// {partof:TCLookupTable_ENTRY}
#define TCLookupTable_ENTRY2(key, value1, value2) \
  { key, {value1, value2} },

/////////////////////////////////////////////////////////////////////////////
// {partof:TCLookupTable_ENTRY}
#define TCLookupTable_ENTRY3(key, value1, value2, value3) \
  { key, {value1, value2, value3} },

/////////////////////////////////////////////////////////////////////////////
// {partof:TCLookupTable_ENTRY}
#define TCLookupTable_ENTRY4(key, value1, value2, value3, value4) \
  { key, {value1, value2, value3, value4} },

/////////////////////////////////////////////////////////////////////////////
// {partof:TCLookupTable_ENTRY}
#define TCLookupTable_ENTRY_PREFIX(key, prefix) \
  TCLookupTable_ENTRY(key, prefix##key)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCLookupTable_ENTRY}
#define TCLookupTable_ENTRY_SUFFIX(key, suffix) \
  TCLookupTable_ENTRY(key, key##suffix)

/////////////////////////////////////////////////////////////////////////////
// Ends the initialization scope of a static lookup table. This scope should
// begin with the TCLookupTable_BEGIN macro and should contain
// TCLookupTable_ENTRY macros.
//
// See Also: TCLookupTable Macros, TCLookupTable_BEGIN, TCLookupTable_ENTRY
// Macros
#define TCLookupTable_END() };

/////////////////////////////////////////////////////////////////////////////
// Computes the number of entries in a static lookup table. Since the size of
// the table is known at compile time, this macro will compile out to a
// constant value.
//
// Parameters:
//   table - The name of the table.
//
// Return Value: The number of elements in the table.
//
// See Also: TCLookupTable Macros
#define TCLookupTable_SIZE(table) (sizeof(table) / sizeof(table[0]))

/////////////////////////////////////////////////////////////////////////////
// Iterates through the entries of the specified lookup table until either
// the specified /key/ is found or the end of the table is reached.
//
// Parameters:
//   table - The name of the table.
//   key - The key to be located in the table.
//   value - A reference to where the located key's associated value will be
//   copied. If the key is not found, this parameter is not used.
//   
// Return Value: true if the specified key was located in the specified
// table, otherwise false;
//
// See Also: TCLookupTable Macros, TCLookupTable_Find
#define TCLookupTable_FIND(table, key, value)                               \
  TCLookupTable_Find<table##_ValueType::KeyT,                               \
    table##_ValueType::ValueT, table##_ValueType::KeyRefT>(table,           \
      TCLookupTable_SIZE(table), key, value)


/////////////////////////////////////////////////////////////////////////////
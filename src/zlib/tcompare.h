#pragma once

#include "zassert.h"

//////////////////////////////////////////////////////////////////////////////
//
// Compare Object
//
//////////////////////////////////////////////////////////////////////////////

class DefaultEquals {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        return value1 == value2;
    }
};

class DefaultCompare {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        return value1 > value2;
    }
};

class DefaultNoEquals {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        ZError("DefaultNoEquals(...) called");
        return false;
    }
};


class DefaultNoCompare {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        ZError("DefaultNoCompare(...) called");
        return false;
    }
};
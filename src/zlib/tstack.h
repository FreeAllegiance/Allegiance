#pragma once

#include <zassert.h>

//////////////////////////////////////////////////////////////////////////////
//
// Stack Template
//
//////////////////////////////////////////////////////////////////////////////

template<class ValueType>
class TStack {
protected:
    ValueType* m_pvalue;
    int     m_count;
    int     m_countAlloc;

public:
    TStack(int countAlloc = 0) :
        m_count(0),
        m_countAlloc(countAlloc)
    {
        ZAssert(m_countAlloc >= 0);

        if (m_countAlloc == 0) {
            m_pvalue = NULL;
        } else {
            m_pvalue = new ValueType[m_countAlloc];
        }
    }

    ~TStack()
    {
        if (m_pvalue) {
            delete[] m_pvalue;
        }
    }

    ValueType& operator[](int index) const
    {
        ZAssert(index >= 0 && index < m_count);

        return m_pvalue[m_count - index - 1];
    }

    void Push(const ValueType& value)
    {
        if (m_count == m_countAlloc) {
            if (m_countAlloc == 0) {
                m_countAlloc = 1;
            } else {
                m_countAlloc *= 2;
            }

            ValueType* m_pvalueNew = new ValueType[m_countAlloc];

            for(int index = 0; index < m_count; index++) {
                m_pvalueNew[index] = m_pvalue[index];
            }

            delete[] m_pvalue;
            m_pvalue = m_pvalueNew;
        }

        m_pvalue[m_count] = value;
        m_count++;
    }

    ValueType Pop()
    {
        ZAssert(m_count > 0);

        m_count--;

        return m_pvalue[m_count];
    }

    int GetCount() const
    {
        return m_count;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Stack Object
//
//////////////////////////////////////////////////////////////////////////////

template<class ValueType>
class TStackObject : public IObject, public TStack<ValueType> {
};
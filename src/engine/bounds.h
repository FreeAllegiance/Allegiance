#pragma once

#include <rect.h>

class Matrix2;

//////////////////////////////////////////////////////////////////////////////
//
// Bounds2
//
//////////////////////////////////////////////////////////////////////////////

class Bounds2 {
private:
    Rect m_rect;

public:
    //
    // Constructors
    //

    Bounds2() {}
    Bounds2(const Rect& m_rect);
    Bounds2(const Bounds2& Bounds2);

    static const Bounds2& GetEmpty();
    Bounds2 Transform(const Matrix2& mat) const;
    friend Bounds2 Union(const Bounds2& Bounds21, const Bounds2& Bounds22);
    friend Bounds2 Intersect(const Bounds2& Bounds21, const Bounds2& Bounds22);

    //
    // Accessors
    //

    const Rect& GetRect() const { return m_rect; }

    //
    // Methods
    //

    void SetRect(const Rect& rect);
};

//////////////////////////////////////////////////////////////////////////////
//
// Bounds
//
//////////////////////////////////////////////////////////////////////////////

/*
class Bounds {
private:
    Vector m_vecMin;
    Vector m_vecMax;
    float m_diagonal;

    void CalcDiagonal();

public:
    //
    // Constructors
    //

    Bounds(const Vector& vecMin, const Vector& vecMax);
    Bounds(const Bounds& bounds);
    Bounds() {}

    static const Bounds& GetEmpty();
    Bounds Transform(const Matrix& mat);
    friend Bounds Union(const Bounds& bounds1, const Bounds& bounds2);

    //
    // Accessors
    //

    const Vector& GetMin() const { return m_vecMin; }
    const Vector& GetMax() const { return m_vecMax; }
    Vector GetCenter() const { return (m_vecMin + m_vecMax) * 0.5; }
    float GetDiagonal() const { return m_diagonal; }
    float GetRadius() const { return m_diagonal / 2; }

    //
    // Methods
    //

    void SetVectors(const Vector& vecMin, const Vector& vecMax);
};
*/
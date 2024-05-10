#pragma once

#include "value.h"


//////////////////////////////////////////////////////////////////////////////
//
// Material
//
//////////////////////////////////////////////////////////////////////////////

class Material : public Value {
protected:
    Material(
        ColorValue* pcolorDiffuse,
        ColorValue* pcolorSpecular,
        ColorValue* pcolorEmissive,
        Number*     ppower
    ) : Value(pcolorDiffuse, pcolorSpecular, pcolorEmissive, ppower)
    {
    }

public:
    DefineCastMember(Material);

    virtual Color GetDiffuse()  const = 0;
    virtual Color GetEmissive() const = 0;
    virtual Color GetSpecular() const = 0;
	virtual float GetPower() const = 0;

    virtual void  SetDiffuse(const Color& color) = 0;
    virtual void SetEmissive(const Color& color) = 0;
    virtual void SetSpecular(const Color& color) = 0;
};

TRef<Material> CreateMaterial(
    const Color& colorDiffuse,
    const Color& colorSpecular = Color::Black(),
    const Color& colorEmissive = Color::Black(),
    float power = 1
);

TRef<Material> CreateMaterial(
    ColorValue* pcolorDiffuse,
    ColorValue* pcolorSpecular,
    ColorValue* pcolorEmissive,
    Number*     ppower
);
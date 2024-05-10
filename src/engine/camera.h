#pragma once

#include <matrix.h>

#include "value.h"

class IKeyboardInput;
class Image;

//////////////////////////////////////////////////////////////////////////////
//
// Camera header
//
//////////////////////////////////////////////////////////////////////////////

class Camera : public Value {
private:
    float       m_fov;
    float       m_znear;
    float       m_zfar;
    float       m_focus;
    Matrix      m_matModel;
    Matrix      m_matPerspective;
    Matrix      m_matInverseModel;
    bool        m_bInverseValid;

protected:
    void InternalSetPosition(const Vector& vec);
    void InternalSetOrientation(const Orientation& orient);
    void UpdateInverse();

public:
    DefineSubclassMembers(Camera);
    DefineCastMember(Camera);
    Camera();

    VectorValue* GetPositionValue() { return VectorValue::Cast(GetChild(0)); }
    void SetPositionValue(VectorValue* pvalue) { SetChild(0, pvalue); }

    OrientationValue* GetOrientationValue() { return OrientationValue::Cast(GetChild(1)); }
    void SetOrientationValue(OrientationValue* pvalue) { SetChild(1, pvalue); }

          float        GetFocus()             { return m_focus;          }
          float        GetFOV()               { return m_fov;            }
          float        GetNear()              { return m_znear;          }
          float        GetFar()               { return m_zfar;           }
    const Matrix&      GetModelMatrix()       { return m_matModel;       }
    const Matrix&      GetPerspectiveMatrix() { return m_matPerspective; }
    const Vector&      GetPosition()   ;
    const Orientation& GetOrientation();

    void SetPosition(const Vector& vec)                ;
    void SetOrientation(const Orientation& orientation);
    void SetZClip(float znear, float zfar)             ;
    void SetOrthogonal()                               ;
    void SetPerspective(float focal)                   ;
    void SetFOV(float fov)                             ;

    Vector       TransformLocalToEye(const Vector& vec)              ;
    bool         TransformEyeToImage(const Vector& vec, Point& point);
    bool       TransformLocalToImage(const Vector& vec, Point& point);
    bool   TransformDirectionToImage(const Vector& vec, Point& point);

    Vector TransformImageToDirection(const Point& point);

    //
    // Value members
    //

    void Evaluate();
    ZString GetFunctionName() { return "Camera"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// MotionCamera
//
//////////////////////////////////////////////////////////////////////////////

TRef<Camera> CreateMotionCamera(
    const Vector& vec,
    Number* ptime,
    TRef<Image>& pimage,
    TRef<IKeyboardInput>& pkeyboardInput
);

//////////////////////////////////////////////////////////////////////////////
//
// SmoothPosition
//
//////////////////////////////////////////////////////////////////////////////

class ISmoothPosition : public VectorValue {
protected:
    ISmoothPosition(Number* ptime) :
        VectorValue(ptime)
    {
    }

public:
    virtual void Goto(const Vector& vector, float dtime) = 0;
};

TRef<ISmoothPosition> CreateSmoothPosition(Number* ptime, const Vector& vec);

//////////////////////////////////////////////////////////////////////////////
//
// SmoothOrientation
//
//////////////////////////////////////////////////////////////////////////////

class ISmoothOrientation : public OrientationValue {
protected:
    ISmoothOrientation(Number* ptime) :
        OrientationValue(ptime)
    {
    }

public:
    virtual void Goto(const Orientation& o, float dtime) = 0;
};

TRef<ISmoothOrientation> CreateSmoothOrientation(Number* ptime, const Orientation& o);

//////////////////////////////////////////////////////////////////////////////
//
// LookAtOrientation
//
//////////////////////////////////////////////////////////////////////////////

TRef<OrientationValue> CreateLookAtOrientation(
    VectorValue* pvectorAt,
    VectorValue* pvectorFrom,
    VectorValue* pvectorUp
);
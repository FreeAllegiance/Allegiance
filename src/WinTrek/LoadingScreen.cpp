#include "pch.h"
#include "LoadingScreen.h"

#include "image.h"
#include "imagetransform.h"

LoadingScreen::~LoadingScreen()
{
    Terminate();
}

void LoadingScreen::Terminate() {
    m_papp->GetEngineWindow()->SetImage(Image::GetEmpty());
}

TRef<Image> GetLoadingImage(TrekApp* papp) {
    auto configuration = UiScreenConfiguration::Create("global/loading_icon.lua", {
        { "time", NumberExposer::Create(papp->GetEngineWindow()->GetTime()) }
    });

    configuration->SetErrorImage(ImageTransform::Rotate(
        CreateExtentImage(new RectValue(Rect(0, 0, 200, 200)), new ColorValue(Color(1, 0, 0))),
        NumberTransform::Multiply(new Number(1.0f), papp->GetEngineWindow()->GetTime())
    ));

    return papp->GetUiEngine()->LoadImageFromLua(configuration);
}

TRef<Image> GetScreenImage(TrekApp* papp) {
    return ImageTransform::Justify(GetLoadingImage(papp), papp->GetEngine()->GetResolution(), JustifyCenter());
}

void LoadingScreen::Start() {
    m_cleanableOnClose = std::move(m_papp->GetEngineWindow()->GetOnCloseEventSource()->AddSinkManaged(new CallbackSink([this]() {
        this->Terminate();
        return true;
    })));

    m_cleanableOnFrame = std::move(m_papp->GetEngineWindow()->GetEvaluateFrameEventSource()->AddSinkManaged(new CallbackValueSink<Time>([this](Time time) {
        this->EvaluateFrame(time);
        return true;
    })));

    m_papp->GetEngineWindow()->SetImage(GetScreenImage(m_papp));
}

void LoadingScreen::EvaluateFrame(Time time) {

}
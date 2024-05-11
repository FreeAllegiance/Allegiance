#pragma once

#include <controls.h>

/*
**
**  File:	mappreview.h
**
**  Author: KGJV
**
**  Description: interface of MapPreviewImage
**  This Image render a game map preview
**  Call CreateMapPreviewImage to create an instance
**  Call ComputePreview passing game parameters to change the displayed preview.
**  History: 
**   april 2007 - created
*/
class MapPreviewImage : public Image
{
public:
	virtual void CalcBounds() = 0;
	virtual void ComputePreview(const MissionParams& p) = 0;
	virtual void Render(Context* pcontext) = 0;
	virtual void ShowDetails(bool bDetails) = 0;
	virtual void ShowSide(bool bSide) = 0;
};
TRef<MapPreviewImage> CreateMapPreviewImage(
    Modeler*         pmodeler,
	Image *pimage);

/*
**  Description: MapPreviewPane
**  This Pane renders a map preview
**  Call ComputePreview passing game parameters to change the displayed preview.
*/
class MapPreviewPane : public AnimatedImagePane {
private:
	TRef<MapPreviewImage> m_pMapPreviewImage;
	bool				  m_bShowDetails;
public:
	MapPreviewPane(Modeler *pmodeler, Image *pimage, bool bShowDetails, bool bShowSide) :
	  AnimatedImagePane(Image::GetEmpty(),pimage->GetBounds().GetRect()),
	  m_bShowDetails(bShowDetails)
	{
		m_pMapPreviewImage=CreateMapPreviewImage(pmodeler,pimage);
		m_pMapPreviewImage->ShowSide(bShowSide);
		SetImage(m_pMapPreviewImage);
	}
	void        MouseEnter(IInputProvider* pprovider, const Point& point);
    void        MouseLeave(IInputProvider* pprovider);
	void ComputePreview(const MissionParams& p) {m_pMapPreviewImage->ComputePreview(p);}
};
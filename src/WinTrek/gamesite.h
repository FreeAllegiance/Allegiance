#pragma once

int GameSite::MessageBox(const ZString& strText, const ZString& strCaption, UINT nType)
{
    return GetWindow()->MessageBox(strText, strCaption, nType);
}
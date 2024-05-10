#pragma once

//------------------------------------------------------------------------------
// functions to handle bad words
//------------------------------------------------------------------------------
void    LoadBadWords (void);
ZString CensorBadWords (const ZString& string);
void    ToggleCensorDisplay (void);
bool    CensorDisplay (void);

//------------------------------------------------------------------------------
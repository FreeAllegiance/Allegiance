echo Building message resources for AGCEvents...
set targetDir=..\%1
cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsCPP.xsl %targetDir%AGCEventsCPP.h
cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsRC2.xsl %targetDir%AGCEventsRC2.rc2
cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsRCH.xsl %targetDir%AGCEventsRCH.h
cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsMC.xsl %targetDir%AGCEventsMC.mc

echo Compiling AGC messages
echo mc.exe -r %targetDir% -h %targetDir% %targetDir%AGCEventsMC.mc
mc.exe -r %targetDir% -h %targetDir% %targetDir%AGCEventsMC.mc

If AllegianceInterop.dll fails to load, then chances are the MSVCRT runtime is not up to date. Install the latest run time from: 

	https://aka.ms/vs/17/release/vc_redist.x86.exe
	https://aka.ms/vs/17/release/vc_redist.x64.exe

(WOPR is usually x86, but you can install both.)

If AllegianceInterop.dll still fails to load, use dependency walker to see what it's missing, then put those .DLLs into the same directory as WOPR. 

If WOPR hangs when starting, check that BotAuthenticationGuid is set on both the machine running WOPR and on the game server in 
SOFTWARE\WOW3264Node\Microsoft\Microsoft Games\Allegiance\1.4\Server\BotAuthenticationGuid

If you are trying to run WOPR on a debug build, put the MSVCRTD* dlls into the same directory as WOPR.




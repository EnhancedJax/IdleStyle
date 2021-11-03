SetTitleMatchMode, 3
DetectHiddenWindows, On

IniRead, VidPath, ..\..\Launch\Vars\CustomVideo.inc, Variables, VideoPath
IniRead, LoopBool, ..\..\Launch\Vars\CustomVideo.inc, Variables, Loop
IniRead, SoundBool, ..\..\Launch\Vars\CustomVideo.inc, Variables, Sound

FadeIn(Window, Speed="1")
{
	DetectHiddenWindows, On
	WinSet, Transparent, 0, %Window%
	WinShow, %Window%
	Loop 15
	{
		Sleep %Speed%
		WinSet, Transparent, % A_Index * 17, %Window%
	}
	Return
}

FadeOut(Window, Speed="1")
{
	WinGet, Trans, Transparent, %Window%
	If Not Trans {
		WinHide, %Window%
		WinSet, Transparent, 0, %Window%
		Return
	}
	Loop 15
	{
		WinSet, Transparent, % 255 - A_Index * 17, %Window%
		Sleep %Speed%
	}
	WinHide, %Window%
	Return
}

If (SoundBool = 1) {
	Run, ffplay "%VidPath%" -left 0 -top 0 -loop %LoopBool% -window_title "IdleStyle" -hide_banner,, Hide
}
else
{
	Run, ffplay "%VidPath%" -left 0 -top 0 -loop %LoopBool% -window_title "IdleStyle" -hide_banner -an,, Hide
}
WinWait, IdleStyle
WinSet, AlwaysOnTop, On, IdleStyle
WinSet, ExStyle, +0x20, IdleStyle
FadeIn(IdleStyle, 1)
ExitApp
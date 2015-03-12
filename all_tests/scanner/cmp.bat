@echo off
for %%i in (*.c) do (
	..\..\Debug\mcc.exe -l %%i
	fc %%~ni.txt lex.out
	if ERRORLEVEL 1 ( 
		echo %%i - Fail 
	) else ( 
		echo %%i - Success 
	)
)
del lex.out

PAUSE
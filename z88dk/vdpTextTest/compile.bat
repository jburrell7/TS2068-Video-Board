
rem standard 256x192 screen
zcc  +ts2068 -subtype=nohrg -create-app textMode.c

rem 512x192 screen
rem zcc  +ts2068 -create-app hiresTest.c
zcc  +ts2068 -a -create-app textMode.c


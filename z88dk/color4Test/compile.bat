
rem standard 256x192 screen
rem zcc  +ts2068 -subtype=nohrg -create-app color4Test.c

rem 512x192 screen
zcc  +ts2068 -create-app color4Test.c

rem assembler listing
rem zcc  +ts2068 -a -create-app color4Test.c


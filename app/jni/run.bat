ndk-build -B NDK_DEBUG=0 -j20 
pause
cd ..
xcopy /y /c /h /r /s libs\*.* jniLibs\
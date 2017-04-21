set DST=%~dp0app\src\main\cpp\
if not exist %DST% md %DST%

::不拷贝子目录，避免拷贝多余的工程文件
set SRC=%~dp0..\Zealens\IPCamera\
set DST=%~dp0app\src\main\cpp\ipcamera\
rmdir /Q/S %DST%
::if not exist %DST% md %DST%
::for /r %SRC% %%a in (*.cpp) do (echo "%%a" | (findstr "main.cpp" || copy "%%a" %DST%))
::for /r %SRC% %%a in (*.c) do (copy "%%a" %DST%)
::for /r %SRC% %%a in (*.h) do (copy "%%a" %DST%)

set SRC=%~dp0..\Zealens\TennisBase\
set DST=%~dp0app\src\main\cpp\tennisbase\
rmdir /Q/S %DST%
if not exist %DST% md %DST%
for /r %SRC% %%a in (*.cpp) do (echo "%%a" | (findstr "main.cpp" || copy "%%a" %DST%))
for /r %SRC% %%a in (*.c) do (copy "%%a" %DST%)
for /r %SRC% %%a in (*.h) do (copy "%%a" %DST%)

set SRC=%~dp0..\Zealens\Bout\
set DST=%~dp0app\src\main\cpp\tennisbout\
rmdir /Q/S %DST%
if not exist %DST% md %DST%
for /r %SRC% %%a in (*.cpp) do (echo "%%a" | findstr "main.cpp" || copy "%%a" %DST%)
for /r %SRC% %%a in (*.c) do (copy "%%a" %DST%)
for /r %SRC% %%a in (*.h) do (copy "%%a" %DST%)

set SRC=%~dp0..\Zealens\Drill\
set DST=%~dp0app\src\main\cpp\drill\
rmdir /Q/S %DST%
::if not exist %DST% md %DST%
::for /r %SRC% %%a in (*.cpp) do (echo "%%a" | (findstr "main.cpp" || copy "%%a" %DST%))
::for /r %SRC% %%a in (*.c) do (copy "%%a" %DST%)
::for /r %SRC% %%a in (*.h) do (copy "%%a" %DST%)

set SRC=%~dp0..\Zealens\Util\
set DST=%~dp0app\src\main\cpp\util\
rmdir /Q/S %DST%
if not exist %DST% md %DST%
for /r %SRC% %%a in (*.h) do (echo "%%a" | (findstr "src inih" || copy "%%a" %DST%))
for /r %SRC% %%a in (*.cpp) do (echo "%%a" | (findstr "src inih" || copy "%%a" %DST%))
for /r %SRC% %%a in (*.c) do (echo "%%a" | (findstr "src inih" || copy "%%a" %DST%))

::需要拷贝子目录，所以用这个命令行
set SRC=%~dp0..\Zealens\Util\ALox\src\src.cpp
set DST=%~dp0app\src\main\cpp\util\alox
rmdir /Q/S %DST%
if not exist %DST% md %DST%
XCOPY %SRC% %DST% /S /Q 

set SRC=%~dp0..\Zealens\Util\inih
set DST=%~dp0app\src\main\cpp\util\inih
rmdir /Q/S %DST%
if not exist %DST% md %DST%
XCOPY %SRC% %DST% /S /Q 

set SRC=%~dp0..\Zealens\interface
set DST=%~dp0app\src\main\cpp\interface
rmdir /Q/S %DST%
if not exist %DST% md %DST%
XCOPY %SRC% %DST% /S /Q 
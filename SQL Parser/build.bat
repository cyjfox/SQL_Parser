set EnvBat="C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat"
:%EnvBat%
set CompilerPath=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\
set Compiler=cl.exe
set BuildCommand="%CompilerPath%%Compiler%"
set WorkingFolder=e:\git\SQL Parser\
set Path=%Path%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\;
set Include=%Include%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\;
set Lib=%Lib%;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\;
cd %WorkingFolder%
e:
%BuildCommand% "%WorkingFolder%GetToken_test.cpp"
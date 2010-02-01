In order to succesfully run the icmp eunit tests one has to recreate
EUnitExeEnvironment as it lacks capability NetworkControl. Here's how
to do it:

1: cd \epoc32\release\winscw\udeb (or similar)
2: setcap EUnitExeEnvironment.exe bfc30 EUnitExeEnvironment2.exe
3: move \epoc32\winscw\c\sys\bin\EunitExeEnvironment2.exe .
4: del EUnitExeEnvironment.exe
5: move EunitExeEnvironment2.exe EUnitExeEnvironment.exe	

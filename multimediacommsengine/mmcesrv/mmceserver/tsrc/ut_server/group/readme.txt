Instructions for running Platform Security tests

Before you start you should have CppUnit installed
on the substed drive.

Step 1: 

  run CreatePlatSecExecutables.pl

this will create executables with different capabilities
needed for profile agent IPC testing. the following files
are copied to your \epoc32\release\winscw\udeb\ directory

  cppunit_WDD_CAPS.exe
  cppunit_NWS_CAPS.exe
  cppunit_ALLMCE_CAPS.exe

Step 2:

   go to W:\MMComms\MMComms\MMCE\group

   open mceserver.mmp

   uncomment MACRO PLAT_SEC_TEST 

   build a clean version of sip server

Step 3:

  go to \MMComms\MMComms\MMCE\server\internal\test\group

  edit MCECSServerCapabilityTest.mmp file so that you uncomment 
  MACRO 	MCE_WDD_CAPS       

  build MCECSServerCapabilityTest.mmp file

  include MCECSServerCapabilityTest.dll in your cppunit.ini file

  go to \epoc32\release\winscw\udeb\ direcotry
  run cppunit_WDD_CAPS.exe

  all the tests should pass

Step 4:

  repeat Step 3 in the same way with, one MACRO at a time

  MACRO MCE_"MACRO"_CAPS

  run cppunit_"MACRO"_CAPS.exe



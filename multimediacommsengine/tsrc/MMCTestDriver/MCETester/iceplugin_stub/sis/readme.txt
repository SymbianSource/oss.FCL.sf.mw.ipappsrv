------------------------------------------
HOW TO USE ICEPLUGIN SIS-STUBS
------------------------------------------

Create ROM with SIS-stubs
-------------------------

* Run createstubs.cmd. This script will create SIS-stub (iceplugin_stub.sis), 
  copy it into the \epoc32\data\Z\system\install -folder and copy 
  iceplugin_stubs.iby into the \epoc32\rom\include folder 

* Include sip_stubs.iby into the end of \epoc32\rom\include\s60.iby file
		#include <iceplugin_stubs.iby>

* Flash the ROM. Stub is installed into z:\system\install\iceplugin_stub.sis
  and makes update via SIS-package possible.

Update files via SIS-package
----------------------------

* Some sources suggests that it may be necessary to update your DLL's 
  version number higher than the original one in the ROM image. This 
  could be done with the command elftran -version 10.5 <myupdateddll.dll> 
  where 10.5 is the new version number (default version is 10.0 so any 
  number between 10.1 - 10.9 are okay). I have noticed this isn't necessary,
  but if eclipsing DLL files isn't working, worth trying.

* Edit iceplugin.pkg if necessary

* Create SIS-package with the following command:
		makesis iceplugin.pkg iceplugin.sis

* Sign SIS-package:
		signsis iceplugin.sis iceplugin_signed.sis <certificate_file> <key_file>

* Uninstall the your update with AppMgr and install iceplugin_signed.sis into the device

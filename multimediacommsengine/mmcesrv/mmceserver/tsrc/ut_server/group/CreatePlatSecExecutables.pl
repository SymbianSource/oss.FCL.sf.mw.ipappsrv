#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  
#

use File::Path;
use File::Copy;

#NetworkServices
my $ret=`\\epoc32\\release\\winscw\\udeb\\setcap.exe cppunit.exe 2000 -SID 0 cppunit_NWS_CAPS.exe`;

    
#WriteDeviceData
my $ret=`\\epoc32\\release\\winscw\\udeb\\setcap.exe cppunit.exe 20 -SID 0 cppunit_WDD_CAPS.exe`;


#All MCE caps: NetworkControl + NetworkServices + Location + ReadDeviceData + WriteDeviceData
my $ret=`\\epoc32\\release\\winscw\\udeb\\setcap.exe cppunit.exe 22430 -SID 0 cppunit_ALLMCE_CAPS.exe`;


# Copy the created executables
system("copy \\epoc32\\winscw\\c\\sys\\bin\\cppunit*.exe \\epoc32\\release\\winscw\\udeb\\");
 

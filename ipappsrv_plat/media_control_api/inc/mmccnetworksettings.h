/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    Class is used for configuring network access.
*
*/




#ifndef MMCCNETWORKSETTINGS_H
#define MMCCNETWORKSETTINGS_H

// INCLUDES
#include <e32def.h>
#include <in_sock.h>
#include <e32cmn.h>

// CONSTANTS
const TInt KDefaultMediaQos = 184;
const TInt KDefaultRtpPort = 17000;
const TInt KDefaultRtcpPort = 17001;

// MACROS

// DATA TYPES
    
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* The TMccNetSettings captures the MCC network settings
* into one class. MCC keeps local public addresses uptodate. 
* If NAT traversal is supported by the system, local and local
* public addresses may be different.
*
*  @lib mmccinterface.dll
*/
class TMccNetSettings
{
public: // Constructors and destructor
    
    inline TMccNetSettings() : 
        iRemoteAddress( KInetAddrAny ), 
        iRemoteRtcpPort( KDefaultRtcpPort ), 
        iLocalAddress( KInetAddrAny ), 
        iLocalPublicAddress( KInetAddrAny ), 
        iLocalRtcpPort( KDefaultRtcpPort ),
        iLocalPublicRtcpPort( KDefaultRtcpPort ), 
        iMediaQosValue( KDefaultMediaQos ), 
        iIapId( -1 ),
        iReserved1( 0 ),
        iReserved2( 0 )
        {};

public: // Data
    
    TInetAddr iRemoteAddress;
    TUint iRemoteRtcpPort;

    TInetAddr iLocalAddress;
    TInetAddr iLocalPublicAddress;
    TUint iLocalRtcpPort;
    TUint iLocalPublicRtcpPort;

    TInt iMediaQosValue;
    TInt iIapId;
    
    TUint32 iReserved1;
    TUint32 iReserved2;
    
};

typedef TPckgBuf<TMccNetSettings> TMccNetSettingsPackage;

#endif /* MMCCNETWORKSETTINGS_H */

// End of File

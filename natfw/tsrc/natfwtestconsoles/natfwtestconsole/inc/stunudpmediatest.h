/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef STUNUDPMEDIATEST_H
#define STUNUDPMEDIATEST_H

// INCLUDES
#include <e32base.h>
#include <e32cons.h>
#include <in_sock.h>
#include <mmccnetworksettings.h>
#include "natfwconnectivityapidefs.h"


// FORWARD DECLARATIONS
class CNATFWConnectivityApi;
class CMediaTestStubs;
class CNATFWCandidate;
class CNATFWCandidatePair;
class CMccInterface;


enum TNATTESTTransmission
    {
    EReceiving = 1,
    ESending = 2
    };

// CLASS
class CStunUdpMediaTest :
    public CBase
    {
    public:
        static CStunUdpMediaTest* NewL(
            CConsoleBase& aConsole,
            CNATFWConnectivityApi& aNat,
            CMediaTestStubs& aIFStub,
            const TDesC8& aDomain,
            TUint aIapId,
            TUint32 aDestAddr,
            TUint aDestPort );
        
        virtual ~CStunUdpMediaTest();
    
    public:

        void TestUdpAudioStreamingL();
        
        void PrintLocalAddressL();
        
        TInt GetLocalAddress( TInetAddr& localAddr );
                
        void PrepareAudioStreamingL();
  
    private:
    
        CStunUdpMediaTest(  CConsoleBase& aConsole,
                            CNATFWConnectivityApi& aNat,
                            CMediaTestStubs& aIFStub,
                            TUint aIapId,
                            TUint32 aDestAddr,
                            TUint aDestPort );
        
        void ConstructL( const TDesC8& aDomain );
        
    private:
    
        CConsoleBase& iConsole;
        CNATFWConnectivityApi& iNat;
        CMediaTestStubs& iIfStub;
        TUint iConnection;
      	CMccInterface* iMccInterface;
      	TMccNetSettings iNetsettings;
      	TUint iSessionId;
      	TUint iStreamId;
      	TUint iIapId;
      	HBufC8* iDomain;
      	TInetAddr iReceiveMediaAddress;
      	TUint32 iDestAddr;
      	TUint iDestPort;
      	CNATFWCandidate* iLocalCandidate;
    };
    
#endif //STUNUDPMEDIATEST_H

// End of File

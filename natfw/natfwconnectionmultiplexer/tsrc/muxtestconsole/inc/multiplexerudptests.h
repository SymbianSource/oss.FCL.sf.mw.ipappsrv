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
* Description:    
*
*/




#ifndef MULTIPLEXERUDPTESTS_H
#define MULTIPLEXERUDPTESTS_H

// INCLUDES
#include <e32base.h>
#include <e32cons.h>
#include <mmccnetworksettings.h>


// FORWARD DECLARATIONS
class CNcmConnectionMultiplexer;
class CMuxTestConsoleStubs;
class CMccInterface;


// CLASS
class CMultiplexerUdpTests : public CBase
    {

    public:
        static CMultiplexerUdpTests* NewL( CConsoleBase& aConsole,      
            CNcmConnectionMultiplexer& aMux, CMuxTestConsoleStubs& aIFStub );
        
        virtual ~CMultiplexerUdpTests();
    
    public:

        void DoRunUdpMuxtTests();
        
        void DoRunTcpMuxTestsL();
    
    
    
        void TestUdpAudioStreamingL();
        
    private:

        // UDP
        void TestCreateSessionL();
        
        void TestCreateStreamL();
        
        void TestCreateConnectionL();
        
        void TestActivateUdpConnectionsL();
        
        void TestActivateStunUdpMediaConnectionsL();
     
    private:
    
        CMultiplexerUdpTests( CConsoleBase& aConsole,
                        CNcmConnectionMultiplexer& aMux,
                        CMuxTestConsoleStubs& aIFStub );
        
        void ConstructL();
        
    private:
    
        CConsoleBase& iConsole;

        CNcmConnectionMultiplexer& iMux;
        CMuxTestConsoleStubs& iIfStub;
    
        TUint iConnection;
        
      	CMccInterface* iMccInterface;
      	
      	TMccNetSettings iNetsettings;
      	TUint iSessionId;
    };
    
#endif //MULTIPLEXERUDPTESTS_H

// End of File

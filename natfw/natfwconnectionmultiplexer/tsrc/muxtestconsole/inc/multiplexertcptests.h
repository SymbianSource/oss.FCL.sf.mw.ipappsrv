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




#ifndef MULTIPLEXERTCPTESTS_H
#define MULTIPLEXERTCPTESTS_H
#include <e32base.h>
#include <e32cons.h>


// FORWARD DECLARATIONS
class CNcmConnectionMultiplexer;
class CMuxTestConsoleStubs;


// CLASS
class CMultiplexerTcpTests : public CBase
    {
    public:
        static CMultiplexerTcpTests* NewL( CConsoleBase& aConsole,      
            CNcmConnectionMultiplexer& aMux, CMuxTestConsoleStubs& aIFStub );
        
        virtual ~CMultiplexerTcpTests();
    
    public:

        
        void DoRunTcpMuxTestsL();
    
    private:

        
        void TestCreateSessionL();
        
        void TestCreateStreamL();
        
        void TestCreateConnectionL();

        void TestConnectTcpConnectionsL();
        
        void TestActivateStunTcpMediaConnectionsL();


   
        
    private:
    
        CMultiplexerTcpTests( CConsoleBase& aConsole,
                        CNcmConnectionMultiplexer& aMux,
                        CMuxTestConsoleStubs& aIFStub );
        
        void ConstructL();

        
    private:
    
        CConsoleBase& iConsole;

        CNcmConnectionMultiplexer& iMux;
        CMuxTestConsoleStubs& iIfStub;
    
        TUint iConnection;
    };
    
#endif //MULTIPLEXERTCPTESTS_H

// End of File

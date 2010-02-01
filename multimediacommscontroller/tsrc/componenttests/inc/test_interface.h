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




#ifndef CTEST_INTERFACE_H
#define CTEST_INTERFACE_H

// INCLUDES
#include <e32base.h>
#include <e32cons.h>
#include "mmccnetworksettings.h"

// FORWARD DECLARATIONS
class CMccInterface;
class CMccInterfaceStub;
class CMccCodecInformation;

// CLASS
class CTestInterface : public CBase
    {
    public:
        static CTestInterface* NewL( CConsoleBase& aConsole,
            const TMccNetSettings& aSettings, 
            CMccInterface& aIF, CMccInterfaceStub& aIFStub );
        
        virtual ~CTestInterface();
    
    public:

        void DoRunInterfaceTestsL();
    
    private:

        // BASIC TESTS WITH SINGLE STREAM AT A TIME
        void TestGetCapabilitiesL() const;
        
        void TestSingleSessionCreationL();
        
        void TestSingleLinkCreationL();
        
        void TestSingleStreamCreationL();
        
        void TestSingleStreamPreparingL( TInt aLinkType, TInt aStreamType,
            TUid aDataSource, TUid aDataSink, const TDesC8& aCodec );
        
        void TestSingleStreamPlayingL( TInt aLinkType, TInt aStreamType,
            TUid aDataSource, TUid aDataSink, const TDesC8& aCodec );
        
        void TestSingleStreamPausingL(  TInt aLinkType, TInt aStreamType,
            TUid aDataSource, TUid aDataSink, const TDesC8& aCodec );
            
        void TestAudioMuteL();
        
        void TestSetCodecL();
        
        void TestGetCodecL();
        
        void TestGetSSRCL();
        
        void TestInactivityTimerL();
        
        // MULTI STREAM TESTS
        void TestMultiStreamLinkSessionL();
        
        void TestFastShutdownsL();
        
        void TestResourceReuseL();
        
        void TestMultiStreamPreparingL();
        
        // CAN BE TESTED PROPERLY ONLY IN HW
        void TestAudioStreamingL( const TDesC8& aCodec );
        
        void TestMultiStreamPausingL();
        
        void TestAmrFecL();
        
        void TestRedundancyL( const TDesC8& aCodec );
        
        void TestAudioRoutingL();
        
    private:
    
        CTestInterface( CConsoleBase& aConsole,
                        const TMccNetSettings& aSettings,
                        CMccInterface& aIF,
                        CMccInterfaceStub& aIFStub );
        
        void ConstructL();
    
        CMccCodecInformation* FetchCodecByMimeSubtypeNameL( 
            const CMccInterface& aInterface, const TDesC8& aName ) const;
        
    private:
    
        CConsoleBase& iConsole;
        TMccNetSettings iNetsettings;
        CMccInterface& iInterface;
        CMccInterfaceStub& iIfStub;
    };
    
#endif //CTEST_INTERFACE_H

// End of File

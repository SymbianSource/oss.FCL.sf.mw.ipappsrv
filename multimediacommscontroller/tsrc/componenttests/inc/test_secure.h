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




#ifndef CTEST_SECURE_H
#define CTEST_SECURE_H

// INCLUDES
#include <e32cons.h>
#include "mmccnetworksettings.h"
#include <mmccsrtpmasterkey.h>
#include <mmccsrtpmastersalt.h>
#include <mmccsrtpcryptoparams.h>

// FORWARD DECLARATIONS
class CMccInterface;
class CMccInterfaceStub;
class CMccCodecInformation;
class CMccSecureInterface;

// CLASS
class CTestSecure : public CBase
    {
    public:
        static CTestSecure* NewL( CConsoleBase& aConsole,
            const TMccNetSettings& aSettings, 
            CMccInterface& aIF, CMccInterfaceStub& aIFStub );
        
        virtual ~CTestSecure();
    
    public:

        void DoRunSecureTestsL();
    
    private:

        void DummyMethodL();
        void TestCreateSecureInterfaceL();
        void TestCreateCryptoContextL();
        void TestCreateInvalidCryptoContextL();
        void TestRemoveCryptoContextL();
        void TestUpdateCryptoContextL();
        void TestBindCryptoContextL();
        void TestBindCryptoContextAndRefreshContextL();
        void TestBindCryptoContextAndPrepareStreamL();
        void TestBindCryptoContextAndPrepareMultipleStreamsL();

        
    private:
    
        CTestSecure( CConsoleBase& aConsole,
                        const TMccNetSettings& aSettings,
                        CMccInterface& aIF,
                        CMccInterfaceStub& aIFStub );
        
        void ConstructL();
        
        void CreateSecureSessioLinkAddDlUlSinkSourceCreateStreamsL();
        void CloseSecureSessioL();
        
    

    private:
    
        CConsoleBase& iConsole;
        TMccNetSettings iNetsettings;
        CMccInterface& iInterface;
        CMccInterfaceStub& iInterfaceStub;
        
        CMccSecureInterface* iSecureInterface;
        TUint32 iContextId;
        TUint32 iContextId2;
        
        TUint32 iSessionId;
        TUint32 iLinkId;
        
        // DL
        TUint32 iRtpSourceId;
        TUint32 iAudioSinkId;
        TUint32 iDlStreamId;
        
        // UL
        TUint32 iRtpSinkId;
        TUint32 iMicSourceId;
        TUint32 iUlStreamId;
        
    };
    
#endif //CTEST_SECURE_H

// End of File

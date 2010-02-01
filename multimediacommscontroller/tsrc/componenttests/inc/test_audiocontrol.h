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




#ifndef CTEST_AUDIO_H
#define CTEST_AUDIO_H

// INCLUDES
#include <e32cons.h>
#include <e32base.h>
#include <e32cmn.h>
#include "mmccnetworksettings.h"

// FORWARD DECLARATIONS
class CMccCodecInformation;
class CMccInterface;
class CMccInterfaceStub;

// CLASS
class CTestAudioControl : public CBase
    {
    public:
        static CTestAudioControl* NewL( CConsoleBase& aConsole,
            const TMccNetSettings& aSettings, CMccInterface& aIF,
            CMccInterfaceStub& aIFStub );
    
        virtual ~CTestAudioControl();
    
    public:
        // Mcc Audio control tests
        void DoRunAudioControlTestsL();
               
    private:

        void TestGetVolumeL();
        void TestGetGainL();
        void TestGetBalanceL();
        void TestGetMaxVolumeL();
        void TestGetMaxGainL();
        void TestSetVolumeL();
        void TestSetGainL();
        void TestSetBalanceL();
        
    private:
        CTestAudioControl( CConsoleBase& aConsole,
            const TMccNetSettings& aSettings, CMccInterface& aIF,
            CMccInterfaceStub& aIFStub );
            
        void ConstructL();
        
        void CreateStreamsL();
        
        void DeleteStreamsL();

        CMccCodecInformation* FetchCodecByMimeSubtypeNameL( 
            const CMccInterface& aInterface, const TDesC8& aName ) const;
                
    private:
        CConsoleBase& iConsole;
        TMccNetSettings iNetsettings;
        CMccInterface& iInterface;
        CMccInterfaceStub& iIfStub;
        TUint32 iSessionId;
        TUint32 iLinkId;
        TUint32 iUlStreamId;
        TUint32 iDlStreamId;
        TUint32 iRtpSinkId;
        TUint32 iRtpSourceId;
        TUint32 iAudioSourceId;
        TUint32 iAudioSinkId;

    };
    
#endif //CTEST_AUDIO_H

// End of File

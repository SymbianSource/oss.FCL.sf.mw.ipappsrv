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



#ifndef CTEST_ASYNCFEATURES_H
#define CTEST_ASYNCFEATURES_H

// INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <e32cons.h>
#include "mmccinterfacedef.h"
#include "mmccnetworksettings.h"
#include "mccteststubs.h"


// FORWARD DECLARATIONS
class CMccInterface;
class CMccCodecInformation;

// CLASS
class CTestAsyncFeat
    {
    public:
        static CTestAsyncFeat* NewL( CConsoleBase& aConsole,
            const TMccNetSettings& aNetsettings, CMccInterface& aIF,
            CMccInterfaceStub& aIFStub );
        virtual ~CTestAsyncFeat();
    
    public:
        void DoRunAsyncFeatureTestsL();
    
    private:
        CTestAsyncFeat( CConsoleBase& aConsole,
            const TMccNetSettings& aNetsettings, CMccInterface& aIF,
            CMccInterfaceStub& aIFStub );
    
        void ConstructL();
        
    private:
        void TestMediaQualityRequestL();
        CMccCodecInformation* FetchCodecByMimeSubtypeNameL( 
            const CMccInterface& aInterface, const TDesC8& aName ) const;
    
    private:
        CMccInterface& iInterface;
        CConsoleBase& iConsole;
        TMccNetSettings iNetsettings;
        CMccInterfaceStub& iIfStub;
        TUint32 iSessionId;
        TUint32 iStreamId1;
        TUint32 iStreamId2;
        TUint32 iLinkId;
        TUint32 iAudioSinkId;
        TUint32 iRtpSourceId;
        TUint32 iRtpSinkId;
        TUint32 iAudioSourceId;
        CMccCodecInformation* iCodec;
        TMccMediaQualityConfig iConfig;
    };


#endif //CTEST_ASYNCFEATURES_H

// End of File


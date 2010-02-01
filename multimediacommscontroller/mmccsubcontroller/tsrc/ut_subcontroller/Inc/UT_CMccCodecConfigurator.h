/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CMccCODECCONFIGURATOR_H__
#define __UT_CMccCODECCONFIGURATOR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <mmf/common/mmfcontrollerframework.h>


//  FORWARD DECLARATIONS
class CMccRtpMediaClock;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccCodecConfigurator )
     : public CEUnitTestSuiteClass, public MAsyncEventHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccCodecConfigurator* NewL();
        static UT_CMccCodecConfigurator* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccCodecConfigurator();
        
        /**
         * From MAsyncEventHandler
         */
        TInt SendEventToClient( const TMMFEvent& /*aEvent*/ )
            {
            return KErrNone;
            }
        
    private:    // Constructors and destructors

        UT_CMccCodecConfigurator();
        void ConstructL();

    private:    // New methods

        void SetupL();
        
        void Teardown();

        void UT_CMccCodecConfigurator_ConfigureCodecLL();
        void UT_CMccCodecConfigurator_DoConfigureAmrNbLL( );
        void UT_CMccCodecConfigurator_DoConfigureAmrNbL2L();
        void UT_CMccCodecConfigurator_DoConfigureUplinkStreamLL();
        void UT_CMccCodecConfigurator_DoConfigureDownlinkStreamLL();
        void UT_CMccCodecConfigurator_DoGetClipConfigurationLL();
        void UT_MccCodecConfigurator_IlbcNeedsStopResumeL();
        void UT_MccCodecConfigurator_VadWithoutComfortNoiseL();
        void UT_CMccCodecConfigurator_PassPrerollFromSinkToSourceLL();
        void UT_MccCodecConfigurator_DoConfigureFileSinkL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        CMccRtpMediaClock* iRtpMediaClock;
    };

#endif      //  __UT_CMccCODECCONFIGURATOR_H__

// End of file

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




#ifndef CTEST_RTCP_H
#define CTEST_RTCP_H

// INCLUDES
#include <e32cons.h>
#include <e32base.h>
#include "mmccnetworksettings.h"

// FORWARD DECLARATIONS

// CLASS
class CTestRtcp : public CBase
    {
    public:
        static CTestRtcp* NewL( CConsoleBase& aConsole, 
                            const TMccNetSettings& aNetsettings );
        virtual ~CTestRtcp();
    
    public:
        void DoRunRtcpTestsL();
    private:
        // RTCP related tests
        void TestSendRTCPReceiverReportL();
        void TestSendRTCPSenderReportL();
        void TestSendRTCPDataL();
        
        void TestSetSdesL();
        void TestSendAppL();
        void TestSendByeL();
        
    private:
        CTestRtcp( CConsoleBase& aConsole, 
                         const TMccNetSettings& aNetsettings );
        void ConstructL();
    
    private:

        // Not own
        CConsoleBase& iConsole;

        TMccNetSettings iNetsettings;
    };
    
#endif //CTEST_RTCP_H

// End of File

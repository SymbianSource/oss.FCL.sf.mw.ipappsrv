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




#ifndef CTEST_DTMF_H
#define CTEST_DTMF_H

// INCLUDES
#include <e32cons.h>
#include <e32base.h>
#include "mmccnetworksettings.h"

// FORWARD DECLARATIONS
class CMccCodecInformation;
class CMccInterface;

// CLASS
class CTestDtmf : public CBase
    {

    enum TDtmfSignalType
        {
        EInbandSignal   = 0x01,
        EOutbandSignal  = 0x10
        };
        
public:
    static CTestDtmf* NewL( CConsoleBase& aConsole, 
        const TMccNetSettings& aNetsettings );
    
    virtual ~CTestDtmf();
        
    void DoRunDtmfTestsL();
        
private:
        
    /**
     * Test different combinations of DTMF-stream creation.
     *
     * @since   S60 4.0
     * @param   aSigType    Signaling type
     * @return  void
     */
    void TestStreamCreationL( TDtmfSignalType aSigType );
    
    /**
     * Verifies that stream control operations works for DTMF-stream.
     *
     * @since   S60 4.0
     * @param   aSigType    Signaling type
     * @return  void
     */
    void TestStreamControlL( TDtmfSignalType aSigType );
    
    /**
     * Verifies that DTMF string sending works.
     *
     * @since   S60 4.0
     * @param   aSigType    Signaling type
     * @return  void
     */
    void TestAutomaticDtmfSendingL( TDtmfSignalType aSigType );
    
    /**
     * Verifies that manual DTMF sending works.
     *
     * @since   S60 4.0
     * @param   aSigType    Signaling type
     * @return  void
     */
    void TestManualDtmfSendingL( TDtmfSignalType aSigType );
        
private:

    CTestDtmf( CConsoleBase& aConsole, const TMccNetSettings& aNetsettings );
    
    void ConstructL();
    
    CMccCodecInformation* FetchCodecByMimeSubtypeNameL( 
        const CMccInterface& aInterface, const TDesC8& aName ) const;
        
private:

    // Not own
    CConsoleBase& iConsole;
    
    TMccNetSettings iNetsettings;

    };
    
#endif //CTEST_DTMF_H

// End of File

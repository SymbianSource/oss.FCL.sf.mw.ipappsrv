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




//  CLASS HEADER
#include "UT_CMccInterface.h"
#include "UT_CMccCodecInformation.h"
#include "UT_CMccCodecAMR.h"
#include "UT_CMccCodecAmrWb.h"
#include "UT_CMccCodecG711.h"
#include "UT_CMccCodecG729.h"
#include "UT_CMccCodecILBC.h"
#include "UT_CMccCodecAVC.h"
#include "UT_CMccCodecAAC.h"
#include "UT_CMccCodecDTMF.h"
#include "UT_CMccCodecH263.h"
#include "UT_CMccCodecRed.h"
#include "UT_CMccCodecCn.h"
#include "UT_TMccCryptoContext.h"
#include "UT_CMccCryptoContextContainer.h"
#include "UT_CMccSecureInterface.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/CEUnitTestSuite.h>


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Wizard generated"));
    
    rootSuite->AddL( UT_CMccInterface::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccCodecInformation::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccCodecAMR::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccCodecAmrWb::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMCCCodecG711::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMCCCodecG729::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMCCCodecILBC::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccCodecAVC::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccCodecAAC::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccCodecDTMF::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccCodecH263::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccCodecRed::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccCodecCn::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_TMccCryptoContext::NewLC() );
    CleanupStack::Pop();   
    
    rootSuite->AddL( UT_CMccCryptoContextContainer::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMccSecureInterface::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }


//  END OF FILE

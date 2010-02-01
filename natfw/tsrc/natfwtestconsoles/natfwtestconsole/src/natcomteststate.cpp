/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    ICE com test states
*
*/




#include "natcomteststate.h"

// ---------------------------------------------------------------------------
// TNatComTestState::TNatComTestState
// ---------------------------------------------------------------------------
//  
TNatComTestState::TNatComTestState() :
     iCurrentIndex( 0 )
    {
    }

// ---------------------------------------------------------------------------
// TNatComTestState::~TNatComTestState
// ---------------------------------------------------------------------------
//      
TNatComTestState::~TNatComTestState()
    {
    iStateSequence.Close(); 
    }
 
// ---------------------------------------------------------------------------
// TNatComTestState::SetStateSequence
// ---------------------------------------------------------------------------
//   
void TNatComTestState::SetSequence(
    const RArray<TNatComState> aStateSequence )
    {
    iStateSequence.Reset();
    TInt count = aStateSequence.Count();

    for ( TInt i( 0 ); i < count; i++ )
        {
        iStateSequence.Append( aStateSequence[ i ] );
        }
    }
    
// ---------------------------------------------------------------------------
// TNatComTestState::Current
// ---------------------------------------------------------------------------
//  
TNatComState TNatComTestState::Current()
    {
    return iStateSequence[ iCurrentIndex ];
    }    
    
// ---------------------------------------------------------------------------
// TNatComTestState::Next
// ---------------------------------------------------------------------------
//      
TNatComState TNatComTestState::Next()
    {
    if ( iCurrentIndex < iStateSequence.Count() )
        {
        iCurrentIndex++;
        }
    return iStateSequence[ iCurrentIndex ];
    }

// ---------------------------------------------------------------------------
// TNatComTestState::SetTestType
// ---------------------------------------------------------------------------
//
void TNatComTestState::SetTestType( TInt aTestType )
    {
    iComTestType = aTestType;
    }

// ---------------------------------------------------------------------------
// TNatComTestState::TestType
// ---------------------------------------------------------------------------
//
TInt TNatComTestState::TestType()
    {
    return iComTestType;
    }

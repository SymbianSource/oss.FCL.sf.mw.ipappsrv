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




#ifndef MCCDATASINK_INL
#define MCCDATASINK_INL

// INCLUDE FILES
#include <e32std.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccDataSink::~CMccDataSink
// -----------------------------------------------------------------------------
//       
inline CMccDataSink::~CMccDataSink()
    {
    }

// -----------------------------------------------------------------------------
// CMccDataSink::AddUserL
// -----------------------------------------------------------------------------
//
inline TInt CMccDataSink::AddUser( MAsyncEventHandler* /*aUser*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccDataSink::RemoveUser
// -----------------------------------------------------------------------------
//        
inline void CMccDataSink::RemoveUser( MAsyncEventHandler* /*aUser*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccDataSink::SetCurrentUser
// -----------------------------------------------------------------------------
//
inline void CMccDataSink::SetCurrentUser( MAsyncEventHandler* /*aUser*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccDataSink::GetParameterL
// -----------------------------------------------------------------------------
//         
inline void CMccDataSink::GetParameterL( TUint32 /*aParam*/, TDes8& /*aVal*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataSink::SetParameterL
// -----------------------------------------------------------------------------
//         
inline void CMccDataSink::SetParameterL( TUint32 /*aParam*/, const TDesC8& /*aVal*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataSink::UpdateL
// -----------------------------------------------------------------------------
//   
inline void CMccDataSink::UpdateL( const TDesC8& /*aVal*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataSink::SetResources
// -----------------------------------------------------------------------------
//  
inline void CMccDataSink::SetResources( MMccResources* aResources )
    {
    iMccResources = aResources;
    }

// -----------------------------------------------------------------------------
// CMccDataSink::RateAdaptationRequest
// -----------------------------------------------------------------------------
//     
inline TInt CMccDataSink::RateAdaptationRequest( 
    const TMccEvent& /*aInputData*/, 
    TMccEvent& /*aOutputData*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccDataSink::RateAdaptationAlert
// -----------------------------------------------------------------------------
//                                          
inline TInt CMccDataSink::RateAdaptationAlert( 
    const TMccEvent& /*aAdaptationData*/,
    TAlertType /*aAlertType*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccDataSink::CMccDataSink
// -----------------------------------------------------------------------------
//      
inline CMccDataSink::CMccDataSink( TUid aType ) :
    MDataSink( aType )
    {
    }
    
#endif      // MCCDATASINK_INL   
            
// End of File

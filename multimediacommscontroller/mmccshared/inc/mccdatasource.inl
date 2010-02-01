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




#ifndef MCCDATASOURCE_INL
#define MCCDATASOURCE_INL

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
// CMccDataSource::~CMccDataSource
// -----------------------------------------------------------------------------
//       
inline CMccDataSource::~CMccDataSource()
    {
    }

// -----------------------------------------------------------------------------
// CMccDataSource::AddUserL
// -----------------------------------------------------------------------------
//
inline TInt CMccDataSource::AddUser( MAsyncEventHandler* /*aUser*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccDataSource::RemoveUser
// -----------------------------------------------------------------------------
//        
inline void CMccDataSource::RemoveUser( MAsyncEventHandler* /*aUser*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccDataSource::SetCurrentUser
// -----------------------------------------------------------------------------
//
inline void CMccDataSource::SetCurrentUser( MAsyncEventHandler* /*aUser*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccDataSource::GetParameterL
// -----------------------------------------------------------------------------
//         
inline void CMccDataSource::GetParameterL( TUint32 /*aParam*/, TDes8& /*aVal*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataSource::SetParameterL
// -----------------------------------------------------------------------------
//         
inline void CMccDataSource::SetParameterL( TUint32 /*aParam*/, const TDesC8& /*aVal*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataSource::UpdateL
// -----------------------------------------------------------------------------
//   
inline void CMccDataSource::UpdateL( const TDesC8& /*aVal*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataSource::SetResources
// -----------------------------------------------------------------------------
//  
inline void CMccDataSource::SetResources( MMccResources* aResources )
    {
    iMccResources = aResources;
    }

// -----------------------------------------------------------------------------
// CMccDataSource::RateAdaptationRequest
// -----------------------------------------------------------------------------
//     
inline TInt CMccDataSource::RateAdaptationRequest( 
    const TMccEvent& /*aInputData*/, 
    TMccEvent& /*aOutputData*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccDataSource::RateAdaptationAlert
// -----------------------------------------------------------------------------
//                                          
inline TInt CMccDataSource::RateAdaptationAlert( 
    const TMccEvent& /*aAdaptationData*/,
    TAlertType /*aAlertType*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// CMccDataSource::CMccDataSource
// -----------------------------------------------------------------------------
//      
inline CMccDataSource::CMccDataSource( TUid aType ) :
    MDataSource( aType )
    {
    }

#endif      // MCCDATASOURCE_INL 
            
// End of File

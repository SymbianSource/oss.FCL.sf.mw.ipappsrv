/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/



// INCLUDE FILES
#include "SmlDmCallback_stub.h"
#include "UT_StateQuery.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSmlDmCallback_stub::CSmlDmCallback_stub
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSmlDmCallback_stub::CSmlDmCallback_stub()
    {
    iLUID2 = NULL;
    }

// -----------------------------------------------------------------------------
// CSmlDmCallback_stub::~CSmlDmCallback_stub
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSmlDmCallback_stub::~CSmlDmCallback_stub()
    {
    delete iLUID;
    delete iLUID2;
    }

CSmlDmCallback_stub* CSmlDmCallback_stub::NewL()
    {
    CSmlDmCallback_stub* self = new(ELeave) CSmlDmCallback_stub();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
        
void CSmlDmCallback_stub::SetResultsL( TInt /*aResultsRef*/, 
                                       CBufBase& aObject,
                                       const TDesC8& /*aType*/ )
    {
    TPtr8 ptr = aObject.Ptr(0);
    iResult = ptr;
    }

void CSmlDmCallback_stub::SetResultsL( TInt /*aResultsRef*/, 
                                       RReadStream*& /*aStream*/,
                                       const TDesC8& /*aType*/ )
    {
    }

void CSmlDmCallback_stub::SetStatusL( TInt /*aStatusRef*/,
                                      MSmlDmAdapter::TError aErrorCode )
    {
    iStatus = aErrorCode;
    }

void CSmlDmCallback_stub::SetMappingL( const TDesC8& /*aURI*/, 
                                       const TDesC8& aLUID )
    {
    delete iLUID;
    iLUID = NULL;
    iLUID = aLUID.AllocL();
    }

_LIT( KChar, "2" );
void CSmlDmCallback_stub::FetchLinkL( const TDesC8& /*aURI*/, 
                                      CBufBase& aData,
                                      MSmlDmAdapter::TError& aStatus )
    {
    aData.ResizeL( 2 );
    aData.Ptr( 0 ).Copy( KChar );
    aStatus = iStatus;
    }

HBufC8* CSmlDmCallback_stub::GetLuidAllocL( const TDesC8& /*aURI*/ )
    {
    if ( !iLUID2 )
        {
        iLUID2 = HBufC8::NewL( 2 );
        TPtr8 ptr( iLUID2->Des() );
        ptr.Copy( _L( "2" ) );
        }
    TStates* states = static_cast< TStates* > ( Dll::Tls() );
    TInt err( states->Err() );
    if ( err )
        {
        TPtr8 ptr( iLUID2->Des() );
        ptr.Copy( _L( "0" ) );
        }
    return iLUID2;
    }
    
 TInt CSmlDmCallback_stub::RemoveMappingL( TUint32 /*aAdapterId*/, const TDesC8& /*aURI*/, TBool /*aChildAlso*/ )
    {
    	return 0;
    }   
    
    
void CSmlDmCallback_stub::ConstructL()
    {
    iLUID = HBufC8::NewL( 0 );
    }

// End of File

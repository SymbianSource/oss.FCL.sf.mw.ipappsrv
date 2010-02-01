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
* Description:    Class is used for configuring secure RTP.
*
*/




#include "mmccsecureinterface_stub.h"
#include "mmccinterfacelogs.h"
const TUid KMccControllerUidInterface = {0x101F8CA6};

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CMccEvent::CMccEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccSecureInterface::CMccSecureInterface( CMccInterface& aInterface )
    : iInterface( aInterface )/*, iMessageDest( KMccControllerUidInterface )*/
    {  
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::ConstructL
// ---------------------------------------------------------------------------
//
void CMccSecureInterface::ConstructL( )
    {
    __INTERFACE( "CMccSecureInterface::ConstructL" )
    //iInterface.GetController( iController );
	iContextCounter = 0;
	iContextId1 = 0;
	iContextId2 = 0;
	iContextId1Binded = EFalse;
	iContextId2Binded = EFalse;
    //iContextContainer = CMccCryptoContextContainer::NewL();
    }
    
// ---------------------------------------------------------------------------
// CMccSecureInterface::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CMccSecureInterface* CMccSecureInterface::NewL( CMccInterface& aInterface )
    {
    CMccSecureInterface* self = new(ELeave) CMccSecureInterface( aInterface );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccSecureInterface::~CMccSecureInterface
// ---------------------------------------------------------------------------
//
CMccSecureInterface::~CMccSecureInterface()
    {      
    //delete iContextContainer;
    }

// ---------------------------------------------------------------------------
// CMccSecureInterface::CreateContext
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::CreateContext( const TDesC8& /*aMasterKey*/,
    							                  const TDesC8& /*aSaltKey*/,
    							                  TUint32& aContextId,
    							                  const TDesC8& /*aParams*/ )
    {
    
    TInt err = KErrNone;
	aContextId = iContextCounter ++;   
    if ( iContextId1 == 0 )
    	iContextId1 = aContextId ;
    else if ( iContextId2 == 0 )
    		iContextId2 = aContextId;	
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccSecureInterface::UpdateContext
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::UpdateContext( const TDesC8& /*aMasterKey*/,
    					                          const TDesC8& /*aSaltKey*/,
    					                          TUint32 aContextId,
    					                          const TDesC8& /*aParams*/ )
    {
    
    TInt err( KErrNone );
            
    if ( aContextId != iContextId1 && iContextId2 != aContextId )
    	{
    	err = KErrNotFound;
    	}
    return err;   
    }
    
// ---------------------------------------------------------------------------
// CMccSecureInterface::RemoveContext
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::RemoveContext( TUint32 aContextId )
    {
    
    TInt err( KErrNone );
    
            
    if ( aContextId != iContextId1 && iContextId2 != aContextId )
    	{
    	err = KErrNotFound;
    	}
    
    
    
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccSecureInterface::Bind
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::Bind( TUint32 /*aSessionId*/, TUint32 /*aLinkId*/,
    			                         TUint32 /*aStreamId*/, TUint32 /*aEndPointId*/,
    			                         TUint32 aContextId )
    {
    
    TInt err = KErrNone;
    
   
    if ( iContextId1Binded && iContextId2Binded )
    	{
    	err = KErrArgument;
    	}
    if ( aContextId == iContextId1 && iContextId1Binded )
    	{
    	err = KErrArgument;
    	}
    if ( aContextId == iContextId2 && iContextId2Binded )
    	{
    	err = KErrArgument;
    	}	
    if ( aContextId != iContextId1 && aContextId != iContextId2 )
    	{
    	err = KErrNotFound;
    	}
    if ( aContextId == iContextId1 && !iContextId1Binded )
    	{
    	iContextId1Binded = ETrue;
    	}
    	
    if ( aContextId == iContextId2 && !iContextId2Binded )
    	{
    	iContextId2Binded = ETrue;
    	}
    return err;
    }

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




#include "mmccsecureinterface.h"
#include "mmccinterfacelogs.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "mmcccryptocontextcontainer.h"
#include "mmcccryptocontext.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CMccEvent::CMccEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccSecureInterface::CMccSecureInterface( CMccInterface& aInterface )
    : iInterface( aInterface ), iMessageDest( KMccControllerUidInterface )
    {  
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::ConstructL
// ---------------------------------------------------------------------------
//
void CMccSecureInterface::ConstructL( )
    {
    __INTERFACE( "CMccSecureInterface::ConstructL" )
    iInterface.GetController( iController );

    iContextContainer = CMccCryptoContextContainer::NewL();
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
    delete iContextContainer;
    }

// ---------------------------------------------------------------------------
// CMccSecureInterface::CreateContext
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::CreateContext( const TDesC8& aMasterKey,
    							                  const TDesC8& aSaltKey,
    							                  TUint32& aContextId,
    							                  const TDesC8& aParams )
    {
    __INTERFACE( "MccSecureInterface::CreateContext" )
    
    TInt err = KErrNone;

    TMccSrtpMasterKeyPckg masterKeyPckg =
                    static_cast<const TMccSrtpMasterKeyPckg&>( aMasterKey );    
    TMccSrtpMasterSaltPckg masterSaltPckg = 
                    static_cast<const TMccSrtpMasterSaltPckg&>( aSaltKey );       
    TMccSrtpCryptoParamsPckg cryptoParamsPckg =
                    static_cast<const TMccSrtpCryptoParamsPckg&>( aParams );
                     
    err = iContextContainer->CreateContext( masterKeyPckg(),
                                            masterSaltPckg(),
                                            aContextId,
    							            cryptoParamsPckg() );

    __INTERFACE_INT1( "MccSecureInterface::CreateContext, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccSecureInterface::UpdateContext
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::UpdateContext( const TDesC8& aMasterKey,
    					                          const TDesC8& aSaltKey,
    					                          TUint32 aContextId,
    					                          const TDesC8& aParams )
    {
    __INTERFACE( "MccSecureInterface::UpdateContext" )
    
    TInt err( KErrNone );
            
    TMccCryptoContext* contextToUpdate;
        
    err = iContextContainer->GetContext( aContextId, contextToUpdate );
    
    if ( KErrNone != err )
        {
        return err;     
        }
    
    TMccSrtpMasterKeyPckg masterKeyPckg =
                    static_cast<const TMccSrtpMasterKeyPckg&>( aMasterKey );    
    TMccSrtpMasterSaltPckg masterSaltPckg = 
                    static_cast<const TMccSrtpMasterSaltPckg&>( aSaltKey );       
    TMccSrtpCryptoParamsPckg cryptoParamsPckg =
                    static_cast<const TMccSrtpCryptoParamsPckg&>( aParams );  
    
    TMccCryptoContext cryptoContextCandidate( 0,
                                              masterKeyPckg(),
                                              masterSaltPckg(),
                                              cryptoParamsPckg() ); 

    // Validate candidates params                                           
    err = cryptoContextCandidate.ValidateContext();                                         
       
    if ( KErrNone != err )
        {
        return err;     
        }
   
    // Get crypto context binding destination
    // if returns KErrNotFound context isn't binded yet
    TMccSecurityDestination securityDest;
          
    err = contextToUpdate->GetDestinationInfo( securityDest.iSessionID, securityDest.iLinkID, 
                                               securityDest.iStreamID, securityDest.iEndpointID );
    
    if ( KErrNotFound != err )
        {        
        cryptoContextCandidate.SetDestinationInfo( securityDest.iSessionID,
                                                   securityDest.iLinkID, 
                                                   securityDest.iStreamID,
                                                   securityDest.iEndpointID );
            
        TMccCryptoContextPckg cryptoContextPckg( cryptoContextCandidate );
        
        TMccSecurityDestinationBuffer securityDestPckg( securityDest );
        
        err = iController.CustomCommandSync( iMessageDest,
                                             EMccBindContextIntoStream,
                                             cryptoContextPckg,
                                             securityDestPckg );
        if ( KErrNone != err )
            {
            return err;
            }
        }
     
    err = iContextContainer->UpdateContext( aContextId, cryptoContextCandidate );
   
    __INTERFACE_INT1( "MccSecureInterface::UpdateContext, exit with ", err )   
    return err;   
    }
    
// ---------------------------------------------------------------------------
// CMccSecureInterface::RemoveContext
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::RemoveContext( TUint32 aContextId )
    {
    __INTERFACE( "MccSecureInterface::RemoveContext" )
    
    TInt err( KErrNone );
    
              
    TMccCryptoContext* contextToRemove;       
    err = iContextContainer->GetContext( aContextId, contextToRemove );
    
    if ( KErrNotFound == err )
        {
        return err;     
        }

    // Get crypto context binding destination
    // if returns KErrNotFound context isn't binded yet
    TMccSecurityDestination securityDest;
          
    err = contextToRemove->GetDestinationInfo( securityDest.iSessionID, securityDest.iLinkID, 
                                               securityDest.iStreamID, securityDest.iEndpointID );
    
    if ( KErrNotFound != err )
        {            
        TMccSecurityDestinationBuffer securityDestPckg( securityDest );
        
        err = iController.CustomCommandSync( iMessageDest,
                                             EMccRemoveContext,
                                             KNullDesC8,
                                             securityDestPckg );
        }
    
    
    
    err = iContextContainer->RemoveContext( aContextId );

    __INTERFACE_INT1( "MccSecureInterface::RemoveContext, exit with ", err )
    
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccSecureInterface::Bind
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccSecureInterface::Bind( TUint32 aSessionId, TUint32 aLinkId,
    			                         TUint32 aStreamId, TUint32 aEndPointId,
    			                         TUint32 aContextId )
    {
    __INTERFACE( "MccSecureInterface::Bind IN" )
    
    TInt err = KErrNone;
    
    TMccCryptoContext* context;
      
    err = iContextContainer->GetContext( aContextId, context );
    
    if ( KErrNotFound == err )
        {
        return err;     
        }

    TMccSecurityDestination securityDest; 
    
    err = context->GetDestinationInfo( securityDest.iSessionID, securityDest.iLinkID, 
                                       securityDest.iStreamID, securityDest.iEndpointID );
    
    // Checks if crypto context is already binded to destination. Can't bind twice
    if ( KErrNotFound != err )
        {
        return KErrArgument;
        }
            
    TMccCryptoContextPckg cryptoContextPckg( *context );   

    securityDest.iSessionID = aSessionId;
    securityDest.iLinkID = aLinkId;
    securityDest.iStreamID = aStreamId;
    securityDest.iEndpointID = aEndPointId;
    
    TMccSecurityDestinationBuffer securityDestPckg( securityDest );
                                              
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccBindContextIntoStream,
                                         cryptoContextPckg,
                                         securityDestPckg
                                         );

    __INTERFACE_INT1( "MccSecureInterface::Bind, exit with ", err )
    
    if ( KErrNone == err )
        {
        context->SetDestinationInfo( aSessionId, aLinkId, aStreamId, aEndPointId );
        }
    return err;
    }

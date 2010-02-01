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
* Description:  
*
*/



#include <badesca.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipmessageelements.h>
#include <siprequireheader.h>
#include "mcesipextensions.h"
#include "mcedictionary.h"
#include "mcesip.h"
#include "mcecomsession.h"

// -----------------------------------------------------------------------------
// CMceSipExtensions::CMceSipExtensions
// -----------------------------------------------------------------------------
//
CMceSipExtensions* CMceSipExtensions::NewL()
    {
    CMceSipExtensions* self = new( ELeave ) CMceSipExtensions();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::CMceSipExtensions
// -----------------------------------------------------------------------------
//
CMceSipExtensions::CMceSipExtensions()
    {
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::ConstructL()
    {
    iLocal = new( ELeave ) CMceDictionary();
    AddDefaultsL( *iLocal );
    iRemote = new( ELeave ) CMceDictionary();
    AddDefaultsL( *iRemote );
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::~CMceSipExtensions
// -----------------------------------------------------------------------------
//
CMceSipExtensions::~CMceSipExtensions()
 	{
 	delete iLocal;
 	delete iRemote;
 	}

// -----------------------------------------------------------------------------
// CMceSipExtensions::Local
// -----------------------------------------------------------------------------
//
CMceSipExtensions::TLevel CMceSipExtensions::Local( TExtension aExtension )
    {
    return Level( *iLocal, aExtension );
    }
    
// -----------------------------------------------------------------------------
// CMceSipExtensions::Remote
// -----------------------------------------------------------------------------
//
CMceSipExtensions::TLevel CMceSipExtensions::Remote( TExtension aExtension )
    {
    return Level( *iRemote, aExtension );
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::SetLocal
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::SetLocal( TExtension aExtension, TLevel aLevel )
    {
    SetLevel( *iLocal, aExtension, aLevel );
    }
    
// -----------------------------------------------------------------------------
// CMceSipExtensions::SetRemote
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::SetRemote( TExtension aExtension, TLevel aLevel )
    {
    SetLevel( *iRemote, aExtension, aLevel );
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::AddDefaultsL
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::AddDefaultsL( CMceDictionary& aDictionary )
    {
    // Addin all the MCE extension headers Update, Subscribe, Prack, Refer, Notify
    RArray< TExtension > extMethods;
    CleanupClosePushL( extMethods );
    LocalSipExtensionsL( extMethods );
    
    for (TInt index=0; index < extMethods.Count(); index++ )
    	{
    	CMceDictionaryItem* extObj = 
	        new( ELeave ) CMceKeyTValuePair<TLevel>( 
	        	static_cast<TInt>(extMethods[ index ]), ESupported );
	    CleanupStack::PushL( extObj );
	    aDictionary.AddL( extObj );
	    CleanupStack::Pop( extObj );
    	}
    CleanupStack::PopAndDestroy( &extMethods );
    
    // 100rel
    CMceDictionaryItem* item100rel = 
        new( ELeave ) CMceKeyTValuePair<TLevel>( E100rel, ESupported );
    CleanupStack::PushL( item100rel );
    aDictionary.AddL( item100rel );
    CleanupStack::Pop( item100rel );
    
    // QOS preconditions
    CMceDictionaryItem* itemQosPreconds = 
        new( ELeave ) CMceKeyTValuePair<TLevel>( EQosPreconds, ESupported );
    CleanupStack::PushL( itemQosPreconds );
    aDictionary.AddL( itemQosPreconds );
    CleanupStack::Pop( itemQosPreconds );
    
    //SEC precondition
    CMceDictionaryItem* itemSecPreconds = 
        new( ELeave ) CMceKeyTValuePair<TLevel>( ESecPreconds, ESupported );
    CleanupStack::PushL( itemSecPreconds );
    aDictionary.AddL( itemSecPreconds );
    CleanupStack::Pop( itemSecPreconds );
    
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::Level
// -----------------------------------------------------------------------------
//
CMceSipExtensions::TLevel CMceSipExtensions::Level( 
                                        const CMceDictionary& aDictionary,
                                        TExtension aExtension ) const
    {
    TLevel ret = ENotSupported;
    CMceKeyTValuePair<TLevel>* pair = static_cast<CMceKeyTValuePair<TLevel>*>
        ( aDictionary.Pair( aExtension ) );
    if ( pair )
        {
        ret = pair->Value();
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::SetLevel
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::SetLevel( CMceDictionary& aDictionary, 
                                  TExtension aExtension, 
                                  TLevel aLevel )
    {
    CMceKeyTValuePair<TLevel>* pair = static_cast<CMceKeyTValuePair<TLevel>*>
        ( aDictionary.Pair( aExtension ) );
    if ( pair )
        {
        pair->SetValue( aLevel );
        }
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::LevelL
// -----------------------------------------------------------------------------
//
CMceSipExtensions::TLevel CMceSipExtensions::LevelL( 
                                    CSIPTransactionBase& aTransaction,
                                    CMceSipExtensions::TExtension aExtension )
    {
    CMceSipExtensions::TLevel level = CMceSipExtensions::ENotSupported;
    TInt reqInd = SipStrConsts::ERequireHeader;
    TInt suppInd = SipStrConsts::ESupportedHeader;
    TInt allowInd = SipStrConsts::EAllowHeader;
    
    const CSIPMessageElements* elements = MceSip::ToMessageElements( aTransaction );
    if ( elements )
        {
        switch ( aExtension )
            {
            case CMceSipExtensions::E100rel:
                {
                if ( MceSip::HasHeaderValueL( *elements, reqInd, KMceSip100Rel ) )
                    {
                    level = CMceSipExtensions::ERequired;
                    }
                else if ( MceSip::HasHeaderValueL( *elements, 
                                                  suppInd, 
                                                  KMceSip100Rel ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                else
                    {
                    // NOP
                    }
                break;
                }
            case CMceSipExtensions::EQosPreconds:
            case CMceSipExtensions::ESecPreconds:
                {
                if ( MceSip::HasHeaderValueL( *elements, reqInd, KMceSipPrecondition ) )
                    {
                    level = CMceSipExtensions::ERequired;
                    }
                else if ( MceSip::HasHeaderValueL( *elements, 
                                                  suppInd, 
                                                  KMceSipPrecondition ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                else
                    {
                    // NOP
                    }
                break;
                }
            case CMceSipExtensions::EUpdate:
                {
                if ( MceSip::HasHeaderValueL( *elements, allowInd,
                    SIPStrings::StringF( SipStrConsts::EUpdate ).DesC() ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                break;
                }
            case CMceSipExtensions::ESubscribe:
                {
                if ( MceSip::HasHeaderValueL( *elements, allowInd,
                    SIPStrings::StringF( SipStrConsts::ESubscribe ).DesC() ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                break;
                } 
             case CMceSipExtensions::ERefer:
                {
                if ( MceSip::HasHeaderValueL( *elements, allowInd,
                    SIPStrings::StringF( SipStrConsts::ERefer ).DesC() ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                break;
                }  
             case CMceSipExtensions::ENotify:
                {
                if ( MceSip::HasHeaderValueL( *elements, allowInd,
                    SIPStrings::StringF( SipStrConsts::ENotify ).DesC() ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                break;
                }
             case CMceSipExtensions::EPrack:
                {
                if ( MceSip::HasHeaderValueL( *elements, allowInd,
                    SIPStrings::StringF( SipStrConsts::EPrack ).DesC() ) )
                    {
                    level = CMceSipExtensions::ESupported;
                    }
                break;
                }                  
            default:
                {
                // NOP
                break;
                }
            }
        }
    return level;
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::UpdateL
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::UpdateL( CSIPServerTransaction& aTransaction )
    {
    const CSIPMessageElements* msg = MceSip::ToMessageElements( aTransaction );
    User::LeaveIfError( msg ? KErrNone : KErrArgument );
    
    // Get the values before updating anything. If a leave happens, the
    // values remain not modified.
    TLevel remote100rel = LevelL( aTransaction, CMceSipExtensions::E100rel );
    
    TLevel remoteQosPrecond = LevelL( aTransaction, 
                                     CMceSipExtensions::EQosPreconds );
    TLevel remoteSecPrecond = LevelL( aTransaction, 
                                     CMceSipExtensions::ESecPreconds );                                 
    
    // Check if the local valus will be modified
    TLevel local100rel = Local( CMceSipExtensions::E100rel );
    if ( remote100rel == CMceSipExtensions::ERequired )
        {
        if ( local100rel == CMceSipExtensions::ENotSupported )
            {
            User::Leave( KErrNotSupported );
            }
        else
            {
            local100rel = remote100rel;
            }
        }
    
    TLevel localQosPrecond = Local( CMceSipExtensions::EQosPreconds );
    if ( remoteQosPrecond == CMceSipExtensions::ERequired )
        {
        if ( localQosPrecond == CMceSipExtensions::ENotSupported )
            {
            User::Leave( KErrNotSupported );
            }
        else
            {
            localQosPrecond = remoteQosPrecond;
            }
        }
    
    TLevel localSecPrecond = Local( CMceSipExtensions::ESecPreconds );
    if ( remoteSecPrecond == CMceSipExtensions::ERequired )
        {
        if ( localSecPrecond == CMceSipExtensions::ENotSupported &&
        	localQosPrecond == CMceSipExtensions::ENotSupported )
            {
            User::Leave( KErrNotSupported );
            }
        else
            {
            localSecPrecond = remoteSecPrecond;
            }
        }
        
    // Finally set the values, first local
    // 100rel
    SetLocal( CMceSipExtensions::E100rel, local100rel );
            
    // precond
    SetLocal( CMceSipExtensions::EQosPreconds, localQosPrecond );
    
    // sec precond
    SetLocal( CMceSipExtensions::ESecPreconds, localSecPrecond );
    
    // then remote
    // 100rel
    SetRemote( CMceSipExtensions::E100rel, remote100rel );
            
    // precond
    SetRemote( CMceSipExtensions::EQosPreconds, remoteQosPrecond );
    
    //sec precond
    SetRemote( CMceSipExtensions::ESecPreconds, remoteSecPrecond );
        
    // MCE sipextension allow header : UPDATE, PRACk, Subscribe, REfer, Notify
    
    SetExtensionAllowHeadersL( msg );
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::UpdateL
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::UpdateL( CSIPClientTransaction& aTransaction )
    {
    const CSIPMessageElements* msg = MceSip::ToMessageElements( aTransaction );
    User::LeaveIfError( msg ? KErrNone : KErrArgument );
    
    if ( MceSip::ResponseCode( aTransaction ) == KMceSipExtensionRequired )
        {
        TBool reqPreconds = MceSip::HasHeaderValueL( 
                                        *msg, 
                                        SipStrConsts::ERequireHeader,
                                        KMceSipPrecondition );
        if ( reqPreconds )
            {
            // needs to require preconditions locally, if possible
            if ( Local( CMceSipExtensions::EQosPreconds ) != 
                CMceSipExtensions::ENotSupported )
                {
                SetLocal( CMceSipExtensions::EQosPreconds, 
                CMceSipExtensions::ERequired );
                }
            else if (Local( CMceSipExtensions::ESecPreconds ) != 
                CMceSipExtensions::ENotSupported && 
                Local( CMceSipExtensions::EQosPreconds ) == 
                CMceSipExtensions::ENotSupported ) 
            	{
            	SetLocal( CMceSipExtensions::ESecPreconds, 
                CMceSipExtensions::ERequired );
            	}
            else
                {
                User::Leave( KErrNotSupported );
                }        
            }
        else
        	{
        	// "Require: precondition" is the only extension required in 421 response 
        	// that may lead to a retried INVITE. 
        	// All the other required extensions to the initial INVITE terminate the 
        	// session.
        	User::Leave( KErrNotSupported );
        	}
        }
    else if ( MceSip::ResponseType( aTransaction ) == E1XX ||
              MceSip::ResponseType( aTransaction ) == E2XX ) 
        {
        if ( MceSip::ResponseType( aTransaction ) == E1XX  &&
             Local( CMceSipExtensions::E100rel ) == 
                    CMceSipExtensions::ERequired )
            {
            // use of 100rel extension is required locally,
            // check, that provisional response is sent reliably
            TBool req100Rel = MceSip::HasHeaderValueL( 
                                        *msg, 
                                        SipStrConsts::ERequireHeader,
                                        KMceSip100Rel );
            User::LeaveIfError( req100Rel ? KErrNone : KErrNotSupported );
            }
       
        SetExtensionAllowHeadersL( msg );
        }
    else
        {
        // NOP
        }
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::Update
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::Update( CMceComSession& aBody )
    {
    for ( TInt i = 0; i< aBody.iSessionModifiers.Count(); i++ )
        {
        Update( i, aBody.Modifier( i ) );
        }
    }

// -----------------------------------------------------------------------------
// CMceSipExtensions::Update
// -----------------------------------------------------------------------------
//
void CMceSipExtensions::Update( TMceSessionModifier aModifier, TUint aValue )
    {
    TInt extension = KErrNotFound;
    TInt level = KErrNotFound;    
     
    switch ( aModifier )
        {
        case KMcePreconditions:
            {
            extension = CMceSipExtensions::EQosPreconds;
            if ( aValue == KMcePreconditionsNotUsed )
                {
                level = CMceSipExtensions::ENotSupported;
                }
            else if ( aValue == KMcePreconditionsSupported )
                {
                level = CMceSipExtensions::ESupported;
                }
            else
                {
                // NOP
                }
            break;
            }
        case KMceSecPreconditions:
            {
            extension = CMceSipExtensions::ESecPreconds;
            if ( aValue == KMcePreconditionsNotUsed )
                {
                level = CMceSipExtensions::ENotSupported;
                }
            else if ( aValue == KMcePreconditionsE2ESupported )
                {
                level = CMceSipExtensions::ESupported;
                }
            else
                {
                // NOP
                }
            break;
            }    
        case KMce100Rel:
            {
            extension = CMceSipExtensions::E100rel;
            if ( aValue == KMce100RelNotUsed )
                {
                level = CMceSipExtensions::ENotSupported;
                }
            else if ( aValue == KMce100RelSupported )
                {
                level = CMceSipExtensions::ESupported;
                }
            else if ( aValue == KMce100RelRequired )
                {
                level = CMceSipExtensions::ERequired;
                }
            else
                {
                // NOP
                }
            break;
            }
        default:
            {
            // NOP
            break;
            }
        }
    if ( extension != KErrNotFound && level != KErrNotFound )
        {
        SetLocal( ( CMceSipExtensions::TExtension ) ( extension ),
                  ( CMceSipExtensions::TLevel ) ( level ) );
        }
    }
// -----------------------------------------------------------------------------
// CMceSipExtensions::RemoteIMSRel5
// -----------------------------------------------------------------------------
//
TBool CMceSipExtensions::RemoteIMSRel5()
    {
    CMceSipExtensions::TLevel localQos = 
        Local( CMceSipExtensions::EQosPreconds );
    
    return  ( localQos == CMceSipExtensions::ERequired );
    }

 // -----------------------------------------------------------------------------
// CMceSipExtensions::ExtensionAllowMethods
// -----------------------------------------------------------------------------
//    
void CMceSipExtensions::ExtensionAllowMethodsL( RArray<TInt>& aArray )
	{
	User::LeaveIfError( aArray.Append( SipStrConsts::EUpdate ));
	User::LeaveIfError( aArray.Append( SipStrConsts::EPrack ));
	User::LeaveIfError( aArray.Append( SipStrConsts::ESubscribe ));
	User::LeaveIfError( aArray.Append( SipStrConsts::ERefer ));
	User::LeaveIfError( aArray.Append( SipStrConsts::ENotify ));	
	}	
	
 // -----------------------------------------------------------------------------
// CMceSipExtensions::LocalSipExtensions
// -----------------------------------------------------------------------------
//    
void CMceSipExtensions::LocalSipExtensionsL( RArray<TExtension>& aArray )
	{
	User::LeaveIfError( aArray.Append( CMceSipExtensions::EUpdate ));
	User::LeaveIfError( aArray.Append( CMceSipExtensions::EPrack ));
	User::LeaveIfError( aArray.Append( CMceSipExtensions::ESubscribe ));
	User::LeaveIfError( aArray.Append( CMceSipExtensions::ERefer ));
	User::LeaveIfError( aArray.Append( CMceSipExtensions::ENotify ));	
	}
	
 // -----------------------------------------------------------------------------
// CMceSipExtensions::SetExtensionAllowHeaders
// -----------------------------------------------------------------------------
//    
void CMceSipExtensions::SetExtensionAllowHeadersL( const CSIPMessageElements* aMessage )
	{
	RArray< TExtension > localExts;
	CleanupClosePushL( localExts );
    RArray< TInt>  extMethods; 
    CleanupClosePushL( extMethods );
    TInt index = 0;
    LocalSipExtensionsL( localExts );
    ExtensionAllowMethodsL( extMethods );
    
    for ( index=0; index < extMethods.Count(); index++ )
    	{
    	
    	TBool extMethod = MceSip::HasHeaderValueL( 
	                        *aMessage, 
	                        SipStrConsts::EAllowHeader,
	                        SIPStrings::StringF( extMethods[index] ).DesC() );
    	
    	SetRemote( localExts[ index ], extMethod ? 
    						ESupported : ENotSupported );
    	}
    CleanupStack::PopAndDestroy( &extMethods );
    CleanupStack::PopAndDestroy( &localExts );	
	}
// End of file

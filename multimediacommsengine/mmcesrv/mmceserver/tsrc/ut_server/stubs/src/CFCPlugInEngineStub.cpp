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




#include "fcpluginengine.h"
#include "fcplugininfo.h"
#include "fcinterfaceinitparams.h"
#include "fcpluginiter.h"
#include "fcsession.h"

_LIT8( KFCTBCP, "TBCP" );

// ----------------------------------------------------------------------------
// CFCPlugInEngine::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CFCPlugInEngine* CFCPlugInEngine::NewL(MFCObserver& aFCObserver)
	{
	CFCPlugInEngine* self = CFCPlugInEngine::NewLC(aFCObserver);
	CleanupStack::Pop(self);
	return self;
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::NewLC
// ----------------------------------------------------------------------------
//
EXPORT_C CFCPlugInEngine* CFCPlugInEngine::NewLC(MFCObserver& aFCObserver)
	{
	CFCPlugInEngine* self = new (ELeave) CFCPlugInEngine(aFCObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CFCPlugInEngine::~CFCPlugInEngine()
	{
	iFCSessions.ResetAndDestroy();	
	iFCPlugIns.ResetAndDestroy();	
	//REComSession::FinalClose();
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::ConstructL
// ----------------------------------------------------------------------------
//
void CFCPlugInEngine::ConstructL()
	{
	ListMechanismImplementationsL();
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::ConstructL
// ----------------------------------------------------------------------------
//
CFCPlugInEngine::CFCPlugInEngine(MFCObserver& aFCObserver)
: iFCObserver(aFCObserver),
  iFCPlugInIter(iFCPlugIns)
	{
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine::FCPlugInIter
// ----------------------------------------------------------------------------
//	
TFCPlugInIter& CFCPlugInEngine::FCPlugInIter()
	{
	return iFCPlugInIter;
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine:: SupportedProtocolL
// ----------------------------------------------------------------------------
//	
EXPORT_C CDesC8Array* CFCPlugInEngine::SupportedProtocolL()
	{
	CDesC8ArrayFlat* protocols = new (ELeave) CDesC8ArrayFlat(1);
	CleanupStack::PushL(protocols);
	TFCPlugInIter plugInIter(iFCPlugIns);
	CFCPlugInInfo* plugin = plugInIter.First();	

	while (plugin)
		{
		HBufC8* name = HBufC8::NewLC(plugin->Name().Length());
		name->Des().Copy(plugin->Name());
		protocols->AppendL(*name);
		plugin = plugInIter.Next();
		CleanupStack::PopAndDestroy(name);
		}
	CleanupStack::Pop(protocols);
	return protocols;
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine:: PlugInByNameL
// ----------------------------------------------------------------------------
//	

EXPORT_C MFCPlugIn* CFCPlugInEngine::PlugInByNameL(const TDesC8& aProtocolName) 
	{
	TFCPlugInIter plugInIter(iFCPlugIns);
	CFCPlugInInfo* plugin = NULL;
	plugin = plugInIter.First();
	while (plugin)
		{
		if (aProtocolName.CompareF(plugin->Name()) == KErrNone)
			{
			// Instantiate plug-in with UID
			return NULL;//CFCInterface::NewL( plugin->Uid());
			}
		plugin = plugInIter.Next();
		}
	return NULL;
	}
	
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine:: CreateNewSessionL
// ----------------------------------------------------------------------------
//	

EXPORT_C CFCSession& CFCPlugInEngine::CreateNewSessionL(
                                                     const TDesC8& /*aType*/, 
		                                             TUint32 aIapId,
		                                             TUint32 aPort)
	{
	MFCPlugIn* plugIn = NULL;
	//plugIn = PlugInByNameL( aType );
	//User::LeaveIfNull(plugIn);
	
	CFCSession* fcSession = CFCSession::NewLC( plugIn, aIapId, aPort );
	//fcSession->SetNegotiatedFCPlugInL(aType);
	//fcSession->SetPlugInTypeL(plugIn);
	fcSession->SetPort( aPort );
	iFCSessions.AppendL( fcSession );
	CleanupStack::Pop( fcSession );
	return *fcSession;
	
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine:: ReleaseSession
// ----------------------------------------------------------------------------
//	

EXPORT_C void CFCPlugInEngine::ReleaseSession(const CFCSession& aSession)
	{
	TInt ind = iFCSessions.Find( &aSession );
	if ( ind != KErrNotFound )
	    {
	    CFCSession* fcSession = iFCSessions[ ind ];
	    fcSession->ReleaseSession();
	    iFCSessions.Remove( ind );
	    delete fcSession;
	    fcSession = NULL;
	    }
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine::FormatAttributeFieldsL
// Attribute must set first
// ----------------------------------------------------------------------------
//	
EXPORT_C RPointerArray<CSdpFmtAttributeField>& 
	CFCPlugInEngine::FormatAttributeFieldsL(const CFCSession& aSession)
	{
	TInt ind = iFCSessions.Find( &aSession );
    User::LeaveIfError( ind );
	return iFCSessions[ ind ]->FormatAttributeFields();
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine:: SetFormatAttributeFieldsL
// Sets the formateAttributeFields
// ----------------------------------------------------------------------------
//	
EXPORT_C void CFCPlugInEngine::SetFormatAttributeFieldsL(
                            const CFCSession& aSession, 
					        RPointerArray<CSdpFmtAttributeField>* aAttributes )
	{
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
    return iFCSessions[ ind ]->SetFormatAttributeFieldsL(aAttributes);
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::UpdateOfferL
// Modifies local parameters of FC to offer document
// ----------------------------------------------------------------------------
//
EXPORT_C void CFCPlugInEngine::UpdateOfferL( const CFCSession& aSession, 
											 CSdpDocument& aSdpDocument )
	{
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	iFCSessions[ ind ]->UpdateSDPL( aSdpDocument );		
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::SetSessionPortL()
// ----------------------------------------------------------------------------
//
EXPORT_C void CFCPlugInEngine::SetSessionPortL( CFCSession& aFCSession, 
                                                TUint32 aNewPort )
    {
    TInt ind = iFCSessions.Find( &aFCSession );
    User::LeaveIfError( ind );
    iFCSessions[ ind ]->SetPort( aNewPort );
    }

// ----------------------------------------------------------------------------
// CFCPlugInEngine::UpdateSession
// Modifies local parameters of FC to offer document
// ----------------------------------------------------------------------------
//
EXPORT_C void CFCPlugInEngine::UpdateSessionL( CFCSession& aFCSession, 
                                               TUint32 aNewPort )
    {
    TInt ind = iFCSessions.Find( &aFCSession );
    User::LeaveIfError( ind );
    iFCSessions[ ind ]->Update( aNewPort );
    }
			
// ----------------------------------------------------------------------------
// CFCPlugInEngine::UpdateAnswerL
// Modifies local parameters of FC to anser document
// ----------------------------------------------------------------------------
//
EXPORT_C void CFCPlugInEngine::UpdateAnswerL( const CFCSession& aSession, 
											  CSdpDocument& aSdpDocument )
	{
	// Updating the remote port and address using the original 
	// port and own address into the SDP doc
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	iFCSessions[ ind ]->UpdateSDPL( aSdpDocument );
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine::PeekDocumentL
// Reads remote parameters of FC from offer/answer document.
// ----------------------------------------------------------------------------
//		
EXPORT_C void CFCPlugInEngine::PeekDocumentL( 
                                        const CFCSession& aSession, 
									    const CSdpDocument& aSdpDocument )
	{
	//Store the value inside of the SDP remote address and port value
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	iFCSessions[ ind ]->StoreInfoL(aSdpDocument);
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::StartConnectionL
// ----------------------------------------------------------------------------
//		
EXPORT_C void CFCPlugInEngine::StartConnectionL( const CFCSession& aSession )
	{
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	iFCSessions[ ind ]->ConnectionReadyL();
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::SendToNetL
// ----------------------------------------------------------------------------
//			
EXPORT_C void CFCPlugInEngine::SendToNetL( const CFCSession& aSession, 
                                           HBufC8* aData)
	{
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	iFCSessions[ ind ]->SendToNetL(aData);
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::IsReceiverL 
// ----------------------------------------------------------------------------
//				
EXPORT_C TBool CFCPlugInEngine::IsReceiverL( const CFCSession& aSession )
    {
    TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	return iFCSessions[ ind ]->IsReceiver();
    }
   	
// ----------------------------------------------------------------------------
// CFCPlugInEngine:: UpdateNeeded
// ----------------------------------------------------------------------------
//			
EXPORT_C TBool CFCPlugInEngine::UpdateNeededL( const CFCSession& aSession ) 
	{
	TInt ind = iFCSessions.Find( &aSession );
	User::LeaveIfError( ind );
	return iFCSessions[ ind ]->UpdateNeeded();
	}    

// ----------------------------------------------------------------------------
// CFCPlugInEngine::ReceivedData 
// ----------------------------------------------------------------------------
//			
void CFCPlugInEngine::ReceivedData( HBufC8* aData, MFCPlugIn* aPlugIn ) 
	{
	CFCSession* fcSession = FindFCSession( aPlugIn );
	if ( fcSession )
	    {
	    iFCObserver.FCReceivedData( aData, fcSession );   
	    }
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine:: ReceivedData
// ----------------------------------------------------------------------------
//			
void CFCPlugInEngine::ErrorNotify( TInt aErrCode, MFCPlugIn* aPlugIn ) 
	{
	CFCSession* fcSession = FindFCSession( aPlugIn );
	if ( fcSession )
	    {
	    iFCObserver.FCErrorNotify( aErrCode, fcSession );    
	    }
	}
	
// ----------------------------------------------------------------------------
// CFCPlugInEngine::ListMechanismImplementationsL
// ----------------------------------------------------------------------------
//
void CFCPlugInEngine::ListMechanismImplementationsL()
	{
	TUid implUid = TUid::Uid( 10 );
    CFCPlugInInfo* plugin = 
		    CFCPlugInInfo::NewL( implUid, KFCTBCP );
	//add to the Plugin Iter
	User::LeaveIfError( iFCPlugIns.Append( plugin ) );

	}
	

// -----------------------------------------------------------------------------
// CFCPlugInEngine::ArrayCleanup
// -----------------------------------------------------------------------------
//
void CFCPlugInEngine::ArrayCleanup(TAny* aArray)
	{
    RImplInfoPtrArray* array = reinterpret_cast<RImplInfoPtrArray*>(aArray);
    if (array)
        {
        array->ResetAndDestroy();
        }
	}
// -----------------------------------------------------------------------------
// CFCPlugInEngine::FindFCSession
// -----------------------------------------------------------------------------
//
CFCSession* CFCPlugInEngine::FindFCSession( MFCPlugIn* aPlugIn )
    {
    CFCSession* fcSession = NULL;
    TBool found = EFalse;
    for ( TInt ind = 0; !found && ind < iFCSessions.Count(); ind++ )
        {
        fcSession = iFCSessions[ ind ];
        if ( aPlugIn == fcSession->PlugIn() )
            {
            found = ETrue;
            }
        }
    return fcSession;
    }
    
//  End of File




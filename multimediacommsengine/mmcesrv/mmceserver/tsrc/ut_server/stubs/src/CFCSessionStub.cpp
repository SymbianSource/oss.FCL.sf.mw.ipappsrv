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
* Description:    The class provides the FC session information
*
*/




//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <s32strm.h>

#include "fcsession.h"
#include "FCInterface.h"
#include "SdpOriginField.h"
#include "SdpConnectionField.h"
#include "SdpMediaField.h"
#include "SdpFmtAttributeField.h"
#include "SdpAttributeField.h"
#include "sdpdocument.h"

_LIT8( KPluginName, "TBCP");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFCSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFCSession* CFCSession::NewL( MFCPlugIn* aPlugIn, 
		                      TUint32 aIapId, 
		                      TUint32 aPort )
	{
	CFCSession* self = CFCSession::NewLC( aPlugIn, aIapId, aPort );
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CFCSession::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFCSession* CFCSession::NewLC( MFCPlugIn* aPlugIn, 
		                       TUint32 aIapId, 
		                       TUint32 aPort )
	{
	CFCSession* self = new (ELeave) CFCSession( aPort );
	CleanupStack::PushL(self);
	self->ConstructL( aPlugIn, aIapId );
	return self;
	}

// -----------------------------------------------------------------------------
// CFCSession::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CFCSession::ConstructL( MFCPlugIn* aPlugIn, TUint32 /*aIapId*/ )
	{
	//User::LeaveIfNull( aPlugIn );
	iPlugIn = ( CFCInterface* ) aPlugIn;
	//iPlugIn->SetIapId( aIapId );
	//iPlugIn->SetLocalPort( iPort );
	}
	
	
// -----------------------------------------------------------------------------
// CFCSession::~CFCSession
// destructor.
// -----------------------------------------------------------------------------
//
CFCSession::~CFCSession()
	{		
	if (iSdpFmtAttributeFields)
		{
		iSdpFmtAttributeFields->ResetAndDestroy();
		iSdpFmtAttributeFields->Close();
		delete iSdpFmtAttributeFields;
		}
    delete iPlugIn;
	}

// -----------------------------------------------------------------------------
// CFCSession::Port
// 
// -----------------------------------------------------------------------------
//
TUint CFCSession::Port() const
	{
	return iPort;
	}

// -----------------------------------------------------------------------------
// CFCSession::SetPort
// 
// -----------------------------------------------------------------------------
//
void CFCSession::SetPort( const TUint aPort )
    {
    iPort = aPort;
	//iPlugIn->SetLocalPort(aPort);
    }

void CFCSession::Update( TUint32 aPort )
    {
    SetPort( aPort );
    iIsReceiver = ( iPort != KFCDefaultPort );
    }
// -----------------------------------------------------------------------------
// CFCSession::DecodeConnectionAddrL
// Get Port from the Sdpdoc
// -----------------------------------------------------------------------------
//
HBufC* CFCSession::DecodeConnectionAddrL( 
                            const CSdpConnectionField* aConnectionField ) const 
	{
    HBufC* addr = NULL;
    // a connection field might not exist in session level
    // so do not leave here, if there is no c= in session level, return NULL
    if ( aConnectionField )
        {
        const TInetAddr* remoteAddr = aConnectionField->InetAddress();
        TBuf< KMaxAddressLength > addr16;
        remoteAddr->Output( addr16 );
        addr = addr16.AllocL();
        }
    return addr;
    
    }

// -----------------------------------------------------------------------------
// CFCSession::FormatAttributeFields()
// Gets all SipMediaFmtAttribute params defined for the FCSession
// -----------------------------------------------------------------------------
//
RPointerArray<CSdpFmtAttributeField>& CFCSession::FormatAttributeFields()
	{
	return *iSdpFmtAttributeFields;
	}

// -----------------------------------------------------------------------------
// CFCSession::SetFormatAttributeFields()
// Setting all SipMediaFmtAttribute params defined for the FCSession
// -----------------------------------------------------------------------------
//	
void CFCSession::SetFormatAttributeFieldsL(
    RPointerArray<CSdpFmtAttributeField>* aAttributes )
	{
	if (iSdpFmtAttributeFields)
		{
		iSdpFmtAttributeFields->ResetAndDestroy();
		iSdpFmtAttributeFields->Close();
		delete iSdpFmtAttributeFields;	
		}
	iSdpFmtAttributeFields = aAttributes;
	}

// -----------------------------------------------------------------------------
// CFCSession::IsFmtAttributeValid()
// validats all SipMediaFmtAttribute params defined for the FCSession
// -----------------------------------------------------------------------------
//	
TBool CFCSession::IsFmtAttributeValid()
	{
	TBool validate= EFalse;
	if (iSdpFmtAttributeFields)
		{
		TInt len=0;
		CSdpFmtAttributeField* fmtatt = NULL;
		for(TInt count=0; count < iSdpFmtAttributeFields->Count(); count++)
	        {
	        fmtatt = (*iSdpFmtAttributeFields)[count];
	        len = fmtatt->Value().Length();
	        if (len!=0)
	        	{
	            validate= ETrue;
	        	}
	        else
	        	{
        		validate = EFalse;
        		return validate;	
        		}
        	return validate;
        	}
		}
	return validate;
	}

// -----------------------------------------------------------------------------
// CFCSession::UpdateSDPL 
// Adds/modifies the FC media line in the offer.
// RStringPool should open before using this function
// -----------------------------------------------------------------------------
//
void CFCSession::UpdateSDPL( CSdpDocument& aSdpDocument )
    {
    //m= application iPort udp NegotiatedPlugin()
    
    // Find the media field, and remove, if exists.
    RPointerArray< CSdpMediaField >& mediaFields = aSdpDocument.MediaFields();
    TInt ind = FindMediaField( aSdpDocument.MediaFields() );
    if ( ind != KErrNotFound )
        {
        delete mediaFields[ ind ];
        mediaFields.Remove( ind );
        }
    
    // create  and add new media field. 
	RStringF mediaType = SdpCodecStringPool::StringPoolL().StringF(
                            SdpCodecStringConstants::EMediaApplication, 
                            SdpCodecStringPool::StringTableL() );
    CleanupClosePushL( mediaType );
    
    RStringF protocol = SdpCodecStringPool::StringPoolL().StringF(
                    SdpCodecStringConstants::EProtocolUdp, 
                    SdpCodecStringPool::StringTableL() );
    CleanupClosePushL( protocol );
    
	CSdpMediaField* mediaField = CSdpMediaField::NewL( mediaType,
                                                       iPort,
                                                       protocol,
                                                       NegotiatedFCPlugIn() );
	CleanupStack::PushL( mediaField );            
    	
	if (IsFmtAttributeValid() )
    	{
    	for( TInt i = 0; i < iSdpFmtAttributeFields->Count(); i++ )
    		{
    		CSdpFmtAttributeField* newField = 
    		    ( ( *iSdpFmtAttributeFields )[i] )->CloneL();
    		CleanupStack::PushL( newField );
    		mediaField->FormatAttributeFields().AppendL( newField );
    		CleanupStack::Pop( newField );
    		}
    	}
    
    mediaFields.AppendL( mediaField);	
    
    CleanupStack::Pop( mediaField );  

    CleanupStack::PopAndDestroy( 2 ); // protocol, mediaType	
	}

// -----------------------------------------------------------------------------
// CFCSession::DecodeSDPMediaPort
// Get Port from the Sdpdoc
// -----------------------------------------------------------------------------
//
TUint CFCSession::DecodeSDPMediaPort( 
        const RPointerArray< CSdpMediaField >& aMediaFields ) const
	{
	TUint port = 0;
    
    TInt ind = FindMediaField( aMediaFields );
    
    if ( ind != KErrNotFound )
        {
        port = aMediaFields[ ind ]->Port();
        }
    
    return port;
    }


// -----------------------------------------------------------------------------
// CFCSession::NegotiatedFCPlugIn()
// Gets the negotiated floor control plugIn
// -----------------------------------------------------------------------------
//
const TDesC8& CFCSession::NegotiatedFCPlugIn() const
	{
	return KPluginName;//iPlugIn->Name();
	}

// ----------------------------------------------------------------------------
// CFCPlugInEngine::StoreInfoL
// Store the information
// ----------------------------------------------------------------------------
//
void CFCSession::StoreInfoL( const CSdpDocument& aSdpDocument )
	{
//Resolve the remote port and addr	   
	HBufC* addr= DecodeConnectionAddrL( aSdpDocument.ConnectionField() );

	RPointerArray<CSdpMediaField>& mediaDescriptions = 
	    ( const_cast< CSdpDocument* >( &aSdpDocument ) )->MediaFields();
	       
	if ( addr )
    	{
    	CleanupStack::PushL( addr );
    
	    TUint port = DecodeSDPMediaPort( mediaDescriptions );
	
    	//Set the remote address for the specific plugin ex TBCP
    	//iPlugIn->SetAddressL( *addr, port );
    	CleanupStack::PopAndDestroy( addr );    
    	}
	
	//Store the fmt attributes
	RPointerArray<CSdpFmtAttributeField>* fmtFields = 
		new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
    CleanupStack::PushL( TCleanupItem( ArrayCleanup, fmtFields ) );
		
	TInt ind = FindMediaField( mediaDescriptions );
	if ( ind != KErrNotFound )
	    {
	    CSdpMediaField* mediaAnnouncement = mediaDescriptions[ ind ];
    	RPointerArray<CSdpFmtAttributeField>& attributes = 
            mediaAnnouncement->FormatAttributeFields();
        		    
        for( TInt fmtCount = 0; fmtCount < attributes.Count(); fmtCount++ )
            {
            CSdpFmtAttributeField* fmtatt = attributes[ fmtCount ];
            const TDesC8& plugInformat = fmtatt->Format();
			if ( iPlugIn->Name().Compare( fmtatt->Format() ) == 0)
		        {
		        CSdpFmtAttributeField* newField = fmtatt->CloneL();
				CleanupStack::PushL( newField );
				fmtFields->AppendL( newField );
				CleanupStack::Pop( newField );
				}
			}
		}
	DoSetFormatAttributeFieldsL( fmtFields );
	CleanupStack::Pop(); // cleanup item
	}

// -----------------------------------------------------------------------------
// CFCSession::CFCSession
// -----------------------------------------------------------------------------
//
void CFCSession::ReleaseSession()
	{
	iPort=0;
	//Stop the sending and reciving and socketserver. Socket is not close yet
	//TRAP_IGNORE(iPlugIn->StopL());
	}
	
// -----------------------------------------------------------------------------
// CFCSession::ConnectionReadyL
// -----------------------------------------------------------------------------
//
void CFCSession::ConnectionReadyL()
	{
	//iPlugIn->ConnectL( iIsReceiver );
	}

// -----------------------------------------------------------------------------
// CFCSession::SendToNetL
// -----------------------------------------------------------------------------
//
void CFCSession::SendToNetL(HBufC8* /*aData*/)
	{
	//iPlugIn->SendToNetL(aData);
	}

// -----------------------------------------------------------------------------
// CFCSession::CFCSession
// -----------------------------------------------------------------------------
//
CFCSession::CFCSession( TUint32 aPort ) : iPort( aPort )
    {
	iIsReceiver = ( iPort != KFCDefaultPort );
	}

// -----------------------------------------------------------------------------
// CFCSession::PlugIn
// -----------------------------------------------------------------------------
//
MFCPlugIn* CFCSession::PlugIn() const
    {
    return iPlugIn;
    }

// -----------------------------------------------------------------------------
// CFCSession::IsReceiver
// -----------------------------------------------------------------------------
//
TBool CFCSession::IsReceiver()
    {
    return iIsReceiver;
    }

// -----------------------------------------------------------------------------
// CFCSession::FindMediaField
// Search is done comparing media format to plugin name
// -----------------------------------------------------------------------------
//
TInt CFCSession::FindMediaField( 
                    const RPointerArray<CSdpMediaField>& aMediaFields ) const   
    {
	TInt ind = KErrNotFound;
	for ( TInt i = 0; ind == KErrNotFound && i < aMediaFields.Count(); i++ )
	    {
	    if ( aMediaFields[ i ]->FormatList().Compare( NegotiatedFCPlugIn() ) == 0 )
	        {
	        ind = i;
	        }
	    }
	return ind;	        
    }
		    
// -----------------------------------------------------------------------------
// CFCSession::ArrayCleanup
// -----------------------------------------------------------------------------
//
void CFCSession::ArrayCleanup(TAny* aArray)
	{
    RPointerArray< CSdpFmtAttributeField >*  array = 
        reinterpret_cast< RPointerArray< CSdpFmtAttributeField >* >( aArray );
    if (array)
        {
        array->ResetAndDestroy();
        }
	}
	
// -----------------------------------------------------------------------------
// CFCSession::DoSetFormatAttributeFieldsL
// Setting all SipMediaFmtAttribute params defined for the FCSession
// -----------------------------------------------------------------------------
//	
void CFCSession::DoSetFormatAttributeFieldsL(
    RPointerArray<CSdpFmtAttributeField>* aAttributes )
	{
	if (iSdpFmtAttributeFields)
		{
		iSdpFmtAttributeFields->ResetAndDestroy();
		iSdpFmtAttributeFields->Close();
		delete iSdpFmtAttributeFields;	
		}
		
	if ( aAttributes )
	    {
	    iSdpFmtAttributeFields = aAttributes;    
	    }
	else
	    {
	    iSdpFmtAttributeFields = 
	        new (ELeave) RPointerArray<CSdpFmtAttributeField>(1);
	    }
	}
	
// -----------------------------------------------------------------------------
// CFCSession::UpdateNeeded
// -----------------------------------------------------------------------------
//
TBool CFCSession::UpdateNeeded()
    {
    return iUpdateNeeded;
    }
	
//  End of File

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




#include "mcepreconditions.h"
#include "sdpdocument.h"
#include "sdpmediafield.h"
#include "sdpattributefield.h"
#include "sdpfmtattributefield.h"
#include "mcesip.h"
#include "sdpcodecstringpool.h"
#include "sdpcodecstringconstants.h"
#include "mcemmlogs.h"
#include "mcemediastate.h"


#define SDP_STRING( stringIndex )\
    iStringPool.StringF( stringIndex, *iStringTable )

        
const TInt KCurrPrecType = 0;
const TInt KCurrPrecStatusType = 1;
const TInt KCurrPrecDirection = 2;

const TInt KDesPrecType = 0;
const TInt KDesPrecStrength = 1;
const TInt KDesPrecStatusType = 2;
const TInt KDesPrecDirection = 3;



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// TMcePreconditions::TMcePreconditions
// -----------------------------------------------------------------------------
//
TMcePreconditions::TMcePreconditions( CMceComMediaStream& aStream )
    : iStream( aStream ),
      iState( ECreated ),
      iStringTable ( 0 ),
      iType ( EQosPreconds )
    {
    }
   
// -----------------------------------------------------------------------------
// TMcePreconditions::~TMcePreconditions
// -----------------------------------------------------------------------------
// 
TMcePreconditions::~TMcePreconditions()
    {
    }
    
// -----------------------------------------------------------------------------
// TMcePreconditions::ConstrucL
// -----------------------------------------------------------------------------
//
void TMcePreconditions::ConstructL()
    {
    iStringPool = SdpCodecStringPool::StringPoolL();
    iStringTable = &SdpCodecStringPool::StringTableL();

    iKAttributeCurr = SDP_STRING( SdpCodecStringConstants::EAttributeCurr );
    iKAttributeDes  = SDP_STRING( SdpCodecStringConstants::EAttributeDes );
    iKAttributeConf = SDP_STRING( SdpCodecStringConstants::EAttributeConf );
    iKAttributeNone = SDP_STRING( SdpCodecStringConstants::EAttributeStatusDirectionTagNone );

    iKAttributeSend = SDP_STRING( SdpCodecStringConstants::EAttributeStatusDirectionTagSend );
    iKAttributeRecv = SDP_STRING( SdpCodecStringConstants::EAttributeStatusDirectionTagRecv );
    iKAttributeSendRecv = SDP_STRING( SdpCodecStringConstants::EAttributeStatusDirectionTagSendrecv );
    
    iKAttributeLocal = SDP_STRING( SdpCodecStringConstants::EAttributeStatusTypeLocal );
    iKAttributeRemote = SDP_STRING( SdpCodecStringConstants::EAttributeStatusTypeRemote );

    iKAttributeMandatory = SDP_STRING( SdpCodecStringConstants::EAttributeStatusStrengthTagMandatory );
    iKAttributeOptional = SDP_STRING( SdpCodecStringConstants::EAttributeStatusStrengthTagOptional );

    iKAttributeFailure = SDP_STRING( SdpCodecStringConstants::EAttributeStatusStrengthTagFailure );
    iKAttributeUnknown = SDP_STRING( SdpCodecStringConstants::EAttributeStatusStrengthTagUnknown );
	
	iKAttributeEnd2End = SDP_STRING( SdpCodecStringConstants::EAttributeStatusTypeE2e );

    DoConstructL();
    }
    
// -----------------------------------------------------------------------------
// TMcePreconditions::IsMet
// -----------------------------------------------------------------------------
//
TBool TMcePreconditions::IsMet() const
    {
    return iState == EReserved;
    }

// -----------------------------------------------------------------------------
// TMcePreconditions::IsMet
// -----------------------------------------------------------------------------
//
TMcePreconditions::TPreconditionType TMcePreconditions::Type()
	{
	return iType;
	}
	
// -----------------------------------------------------------------------------
// TMcePreconditions::ConstrucL
// -----------------------------------------------------------------------------
//
void TMcePreconditions::ConstrucL( const TMcePreconditions& aPreconditions )
    {
    __ASSERT_ALWAYS( aPreconditions.iStringTable != NULL, 
                     User::Leave( KErrArgument ) );

    iState = aPreconditions.iState;
    iStringPool = aPreconditions.iStringPool;
    iStringTable = aPreconditions.iStringTable;

    iKAttributeCurr = aPreconditions.iKAttributeCurr;
    iKAttributeDes = aPreconditions.iKAttributeDes;
    iKAttributeConf = aPreconditions.iKAttributeConf;
    iKAttributeNone = aPreconditions.iKAttributeNone;

    iKAttributeSend = aPreconditions.iKAttributeSend;
    iKAttributeRecv = aPreconditions.iKAttributeRecv;
    iKAttributeSendRecv = aPreconditions.iKAttributeSendRecv;
   
    iKAttributeLocal = aPreconditions.iKAttributeLocal;
    iKAttributeRemote = aPreconditions.iKAttributeRemote;

    iKAttributeMandatory = aPreconditions.iKAttributeMandatory;
    iKAttributeOptional = aPreconditions.iKAttributeOptional;

    iKAttributeFailure = aPreconditions.iKAttributeFailure;
    iKAttributeUnknown = aPreconditions.iKAttributeUnknown;
    
    iKAttributeEnd2End = aPreconditions.iKAttributeEnd2End;   
    }
    
// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::TMceSegmentedPreconditions
// -----------------------------------------------------------------------------
//
TMceSegmentedPreconditions::TMceSegmentedPreconditions( 
    CMceComMediaStream& aStream )
    : TMcePreconditions( aStream )
    {
    }  

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::CloneL
// -----------------------------------------------------------------------------
//
TMcePreconditions* TMceSegmentedPreconditions::CloneL( 
    CMceComMediaStream& aStream )
    {
    TMceSegmentedPreconditions* copy = 
        new (ELeave) TMceSegmentedPreconditions( aStream );
    CleanupStack::PushL( copy );
    copy->ConstrucL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }
    
// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::ConstrucL
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::ConstrucL( 
    const TMceSegmentedPreconditions& aPreconditions )
    {
    TMcePreconditions::ConstrucL( aPreconditions );

    iCurrentLocalStatus = aPreconditions.iCurrentLocalStatus;
    iCurrentRemoteStatus = aPreconditions.iCurrentRemoteStatus;
    iDesiredLocalSend = aPreconditions.iDesiredLocalSend;
    iDesiredLocalRecv = aPreconditions.iDesiredLocalRecv;
    
    iDesiredRemoteSend = aPreconditions.iDesiredRemoteSend;
    iDesiredRemoteRecv = aPreconditions.iDesiredRemoteRecv;

    iConfirmation = aPreconditions.iConfirmation;
    iType  = aPreconditions.iType;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DoConstructL
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::DoConstructL()
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::DoConstructL(), Entry ")
    
    iCurrentLocalStatus = iKAttributeNone;
    iCurrentRemoteStatus = iKAttributeNone;
    iDesiredLocalSend = iKAttributeNone;
    iDesiredLocalRecv = iKAttributeNone;
    iDesiredRemoteSend = iKAttributeNone;
    iDesiredRemoteRecv = iKAttributeNone;
    iConfirmation = iKAttributeNone;
    iType = TMcePreconditions::EQosPreconds;
    
    TInt direction = iStream.Direction();
    direction = direction == SdpCodecStringConstants::EAttributeInactive ? 
                iStream.SdpStreamType() : direction;
    
    switch( direction )
        {
        case SdpCodecStringConstants::EAttributeSendrecv:
            {
            SetDesiredStatus( iKAttributeLocal, iKAttributeSendRecv, iKAttributeMandatory );
            MCEMM_DEBUG_SVALUE("desired direction ", iKAttributeSendRecv.DesC() )
            break;
            }
        case SdpCodecStringConstants::EAttributeSendonly:
            {
            SetDesiredStatus( iKAttributeLocal, iKAttributeSend, iKAttributeMandatory );
            MCEMM_DEBUG_SVALUE("desired direction ", iKAttributeSend.DesC() )
            break;
            }
        case SdpCodecStringConstants::EAttributeRecvonly:
            {
            SetDesiredStatus( iKAttributeLocal, iKAttributeRecv, iKAttributeMandatory );
            MCEMM_DEBUG_SVALUE("desired direction ", iKAttributeRecv.DesC() )
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
        }
        
    MCEMM_DEBUG("TMceSegmentedPreconditions::DoConstructL(), Exit ")        
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::EncodeL
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::EncodeL( CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeL(), Entry ")
    
    TMceNegotiationRole role = iStream.Session()->NegotiationState().Role();
    //if it is MT but does not decoded anything then we should not add in 183
	if ( role == EMceRoleAnswerer && iState == ECreated && 
		iStream.PreconditionsL( KMceSecPreconditions ) )
		{
		return;
		}
    //currentLocalStatus
    EncodeCurrentStatusL( aMedia, iKAttributeLocal );

    //currentRemoteStatus
    EncodeCurrentStatusL( aMedia, iKAttributeRemote );
    
    //desiredLocalStatus
    EncodeDesiredStatusL( aMedia, iKAttributeLocal );
    
    //desiredRemoteStatus
    EncodeDesiredStatusL( aMedia, iKAttributeRemote );

    //if answerer and confirmation is needed based on offerer's 
    //reservation status
    if ( role == EMceRoleAnswerer && RemoteReservationNeeded() )
        {
        iConfirmation = DesiredDirection( iKAttributeRemote );
        EncodeConfirmationL( aMedia );
        }
        
    if ( role == EMceRoleOfferer )
        {
        if ( iConfirmation != iKAttributeNone )
            {
            iState = EReserved;
            iConfirmation = iKAttributeNone;
            }
        else
            {
            iState = ENegotiating;
            }
        }
    else
        {
        //reset confirmation
    	iConfirmation = iKAttributeNone;
    
    	//if not allready e2e reserved
    	if ( !IsMet() )
        	{
        	SetState();
        	}
        }   
	    
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeL(), Exit ") 
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceSegmentedPreconditions::DecodeL( CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeL(), Entry ")

    TMceNegotiationRole role = iStream.Session()->NegotiationState().Role();
    RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
    
    TInt decoded = 0;
    TInt index = 0;
    
    for( index = 0; index < attributes.Count(); index++ )
        {
        CSdpAttributeField* attribute = attributes[ index ];
        //if offerer and there is confirmation request
        if ( attribute->Attribute() == iKAttributeConf && 
             role == EMceRoleOfferer )
            {
            decoded += DecodeConfStatusL( *attribute );
            }
        //current and desired statuses are only decoded when reservation
        //is still ongoing
        else if ( !IsMet() )
            {
            //decode current status
            if ( attribute->Attribute() == iKAttributeCurr )
                {
                decoded += DecodeCurrentStatusL( *attribute );
                }
            //decode desired status
            else if ( attribute->Attribute() == iKAttributeDes )
                {
                decoded += DecodeDesiredStatusL( *attribute );
                }
            else
                {
                //NOP
                }
            }
        else
            {
            //NOP
            }
        }

    //if something was decoeded
    if ( decoded )
        {
        SetState();
        }
    else	
        {
        //if nothing was decoded. For example preconditions were sent
        //but nothing received. This is interpretet as resources are reserved
        iState = EReserved;
        iCurrentRemoteStatus = DesiredDirection( iKAttributeRemote );
        iCurrentRemoteStatus = ( iCurrentRemoteStatus == iKAttributeNone ) ?
            DecodeMediaDirectionL( DesiredDirection( iKAttributeLocal ).DesC() ) : 
            iCurrentRemoteStatus;
        }
        
    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeL(), Exit ")
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::EncodeCurrentStatusL
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::EncodeCurrentStatusL( 
    CSdpMediaField& aMedia,
    RStringF aAccessNetwork ) const
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeCurrentStatusL(), Entry ")
    MCEMM_DEBUG_SVALUE("access network", aAccessNetwork.DesC() )
    
    HBufC8* currentStatus = NULL;
    CSdpAttributeField* qosLine = NULL;
    RStringF current = aAccessNetwork == iKAttributeLocal ? 
                       iCurrentLocalStatus : iCurrentRemoteStatus;
    RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
                       
                           
    currentStatus = CurrentStausTextLC( aAccessNetwork.DesC(),
                                        current.DesC() );
                                        
    qosLine = CSdpAttributeField::NewLC( iKAttributeCurr, *currentStatus );
    MCEMM_DEBUG_SVALUE("encoded current status", qosLine->Value() )
    attributes.AppendL( qosLine );
    CleanupStack::Pop( qosLine );
    CleanupStack::PopAndDestroy( currentStatus );
    
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeCurrentStatusL(), Exit ")
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::EncodeDesiredStatusL
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::EncodeDesiredStatusL( 
    CSdpMediaField& aMedia,
    RStringF aAccessNetwork ) const
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeDesiredStatusL(), Entry ")
    MCEMM_DEBUG_SVALUE("access network", aAccessNetwork.DesC() )
    
    HBufC8* desiredStatus = NULL;
    CSdpAttributeField* qosLine = NULL;
    RStringF desiredSend = aAccessNetwork == iKAttributeLocal ? 
                           iDesiredLocalSend : iDesiredRemoteSend;
    RStringF desiredRecv = aAccessNetwork == iKAttributeLocal ? 
                           iDesiredLocalRecv : iDesiredRemoteRecv;
    RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
                           
    if ( desiredSend == desiredRecv )
        {
        desiredStatus = DesiredStausTextLC( desiredSend.DesC(),
                                            aAccessNetwork.DesC(),
                                            iKAttributeSendRecv.DesC() );
        qosLine = CSdpAttributeField::NewLC( iKAttributeDes, *desiredStatus );
        MCEMM_DEBUG_SVALUE("encoded desired send & recv status", qosLine->Value() )
        attributes.AppendL( qosLine );
        CleanupStack::Pop( qosLine );
        CleanupStack::PopAndDestroy( desiredStatus );
        }
    else
        {
        desiredStatus = DesiredStausTextLC( desiredSend.DesC(),
                                            aAccessNetwork.DesC(),
                                            iKAttributeSend.DesC() );
        qosLine = CSdpAttributeField::NewLC( iKAttributeDes, *desiredStatus );
        MCEMM_DEBUG_SVALUE("encoded desired send status", qosLine->Value() )
        attributes.AppendL( qosLine );
        CleanupStack::Pop( qosLine );
        CleanupStack::PopAndDestroy( desiredStatus );

        desiredStatus = DesiredStausTextLC( desiredRecv.DesC(),
                                            aAccessNetwork.DesC(),
                                            iKAttributeRecv.DesC() );
        qosLine = CSdpAttributeField::NewLC( iKAttributeDes, *desiredStatus );
        MCEMM_DEBUG_SVALUE("encoded desired recv status", qosLine->Value() )
        attributes.AppendL( qosLine );
        CleanupStack::Pop( qosLine );
        CleanupStack::PopAndDestroy( desiredStatus );   
        }
    
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeDesiredStatusL(), Exit ")
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::EncodeConfirmationL
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::EncodeConfirmationL( CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeConfirmationL(), Entry ")
    
    if ( iConfirmation != iKAttributeNone )
        {
        HBufC8* confirmation = NULL;
        CSdpAttributeField* qosLine = NULL;
        RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
                               
        confirmation = CurrentStausTextLC( iKAttributeRemote.DesC(),
                                           iConfirmation.DesC() );
                                            
        qosLine = CSdpAttributeField::NewLC( iKAttributeConf, *confirmation );
        MCEMM_DEBUG_SVALUE("encoded confirmation status", qosLine->Value() )
        attributes.AppendL( qosLine );
        CleanupStack::Pop( qosLine );
        CleanupStack::PopAndDestroy( confirmation );
        }
        
    MCEMM_DEBUG("TMceSegmentedPreconditions::EncodeConfirmationL(), Exit ")   
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DecodeCurrentStatusL
// -----------------------------------------------------------------------------
//
TUint TMceSegmentedPreconditions::DecodeCurrentStatusL( 
    CSdpAttributeField& aAttribute )
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeCurrentStatusL(), Entry ")
    
    TUint decoded = (TUint)ETrue;
    TPtrC8 accessNetworkDesc;
    TPtrC8 directionDesc;
    
    if ( ParseCurrentStatus( aAttribute.Value(), accessNetworkDesc, directionDesc ) == KErrNone )
        {
        if ( accessNetworkDesc == iKAttributeLocal.DesC() )
            {
            SetRemoteStatus( DecodeMediaDirectionL( directionDesc ) );
            MCEMM_DEBUG_SVALUE("decoded current status", aAttribute.Value() )
            }
        }
    else
        {
        decoded = (TUint)EFalse;
        }

    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeCurrentStatusL(), Exit ")
    return decoded;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DecodeDesiredStatusL
// -----------------------------------------------------------------------------
//
TUint TMceSegmentedPreconditions::DecodeDesiredStatusL( 
    CSdpAttributeField& aAttribute )
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeCurrentStatusL(), Entry ")

    TUint decoded = (TUint)ETrue;

    TPtrC8 strengthDesc; 
    TPtrC8 accessNetworkDesc;
    TPtrC8 directionDesc;
    
    if ( ParseDesiredStatus( aAttribute.Value(), 
                             strengthDesc, accessNetworkDesc, directionDesc ) == KErrNone )
        {
        if ( accessNetworkDesc == iKAttributeLocal.DesC() )
            {
            RStringF strength = DecodeStrengthL( strengthDesc );
            RStringF direction = DecodeMediaDirectionL( directionDesc );
            SetDesiredStatus( iKAttributeRemote, direction, strength );
            MCEMM_DEBUG_SVALUE("decoded desired status", aAttribute.Value() )
            }
        }
    else
        {
        decoded = (TUint)EFalse;
        }

    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeCurrentStatusL(), Exit ")
    return decoded;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::SetRemoteStatus
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::SetRemoteStatus( RStringF aDirection )
    {
    if ( iCurrentRemoteStatus == iKAttributeNone ||
         aDirection == iKAttributeSendRecv )
        {
        iCurrentRemoteStatus = aDirection;
        }
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DecodeConfStatusL
// -----------------------------------------------------------------------------
//
TUint TMceSegmentedPreconditions::DecodeConfStatusL( 
    CSdpAttributeField& aAttribute )
    {
    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeConfStatusL(), Entry ")

    TUint decoded = (TUint)ETrue;

    TPtrC8 accessNetworkDesc;
    TPtrC8 directionDesc;
    
    if ( ParseCurrentStatus( aAttribute.Value(), accessNetworkDesc, directionDesc ) == KErrNone )
        {
        if ( accessNetworkDesc == iKAttributeRemote.DesC() )
            {
            iConfirmation = DecodeMediaDirectionL( directionDesc );
            MCEMM_DEBUG_SVALUE("decoded confirmation", aAttribute.Value() )
            }
        else
            {
            decoded = (TUint)EFalse;
            }
        }
    else
        {
        decoded = (TUint)EFalse;
        }
    
    MCEMM_DEBUG("TMceSegmentedPreconditions::DecodeConfStatusL(), Exit ")
    return decoded;
        
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::SetState
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::SetState()
    {
    iState = ENegotiating;
    
    if ( !ReservationNeeded() &&
         !RemoteReservationNeeded() &&
         iConfirmation == iKAttributeNone )
        {
        iState = EReserved;
        }
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::SetDesiredStatus
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::SetDesiredStatus( RStringF aAccessNetwork,
                                                   RStringF aDirection, 
                                                   RStringF aStrength )
    {
    RStringF& desiredStatusSend = aAccessNetwork == iKAttributeLocal ? 
                                 iDesiredLocalSend : iDesiredRemoteSend;
    RStringF& desiredStatusRecv = aAccessNetwork == iKAttributeLocal ? 
                                 iDesiredLocalRecv : iDesiredRemoteRecv;
    
    if ( aDirection == iKAttributeSend )
        {
        desiredStatusSend = aStrength;
        }
    else if ( aDirection == iKAttributeRecv )
        {
        desiredStatusRecv = aStrength;
        }
    else 
        {
        desiredStatusSend = aStrength;
        desiredStatusRecv = aStrength;
        }
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DesiredDirection
// -----------------------------------------------------------------------------
//
RStringF TMceSegmentedPreconditions::DesiredDirection( RStringF aAccessNetwork )
    {
    RStringF desiredStatus = iKAttributeNone;
    
    RStringF desiredStatusSend = aAccessNetwork == iKAttributeLocal ? 
                                 iDesiredLocalSend : iDesiredRemoteSend;
    RStringF desiredStatusRecv = aAccessNetwork == iKAttributeLocal ? 
                                 iDesiredLocalRecv : iDesiredRemoteRecv;

    if ( desiredStatusSend == iKAttributeMandatory &&
         desiredStatusRecv == iKAttributeMandatory )
        {
        desiredStatus = iKAttributeSendRecv;
        }
    else if ( desiredStatusSend == iKAttributeMandatory &&
              desiredStatusRecv != iKAttributeMandatory )
        {
        desiredStatus = iKAttributeSend;
        }
    else if ( desiredStatusSend != iKAttributeMandatory &&
              desiredStatusRecv == iKAttributeMandatory )
        {
        desiredStatus = iKAttributeRecv;
        }
    else
        {
        //none
        }
        
    return desiredStatus;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DesiredStausTextLC
// -----------------------------------------------------------------------------
//
HBufC8* TMceSegmentedPreconditions::DesiredStausTextLC( const TDesC8& aStrength, 
                                                        const TDesC8& aAccessNetwork, 
                                                        const TDesC8& aMediaDirection ) const
    {
    TInt desStatusTextLength = KMceSipSdpSegPrecDesiredStatusPattern().Length() +
                               aStrength.Length() +
                               aAccessNetwork.Length() +
                               aMediaDirection.Length();
    
    HBufC8* desiredStatusText = HBufC8::NewLC( desStatusTextLength );
    desiredStatusText->Des().AppendFormat( KMceSipSdpSegPrecDesiredStatusPattern,
                                           &aStrength, &aAccessNetwork,
                                           &aMediaDirection );
    return desiredStatusText;
    } 

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::CurrentStausTextLC
// -----------------------------------------------------------------------------
//
HBufC8* TMceSegmentedPreconditions::CurrentStausTextLC( const TDesC8& aAccessNetwork, 
                                                        const TDesC8& aMediaDirection ) const
    {
    TInt curStatusTextLength = KMceSipSdpSegPrecCurrentStatusPattern().Length() + 
                                  aAccessNetwork.Length() +
                                  aMediaDirection.Length();
    
    HBufC8* currentStatusText = HBufC8::NewLC( curStatusTextLength );
    currentStatusText->Des().AppendFormat( KMceSipSdpSegPrecCurrentStatusPattern,
                                             &aAccessNetwork,
                                             &aMediaDirection );

    return currentStatusText;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::ParseCurrentStatus
// -----------------------------------------------------------------------------
//
TInt TMceSegmentedPreconditions::ParseCurrentStatus( const TDesC8& aLine,
                                                     TPtrC8& aAccessNetwork,
                                                     TPtrC8& aMediaDirection )
    {
    TLex8 attribute( aLine );
    TInt tokenNbr = 0;
    
    while( !attribute.Eos() )
        {
        TPtrC8 token = attribute.NextToken();
        if ( tokenNbr == KCurrPrecType )
            {
            if ( token != KMceSipSdpQos )
                {
                return KErrNotFound;
                }
            }
        else if ( tokenNbr == KCurrPrecStatusType )
            {
            aAccessNetwork.Set( token );
            }
        else if ( tokenNbr == KCurrPrecDirection )
            {
            aMediaDirection.Set( token );
            }
        else
            {
            //ignore trailing tokens
            }
        tokenNbr++;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::ParseDesiredStatus
// -----------------------------------------------------------------------------
//
TInt TMceSegmentedPreconditions::ParseDesiredStatus( const TDesC8& aLine,
                                                     TPtrC8& aStrength,
                                                     TPtrC8& aAccessNetwork,
                                                     TPtrC8& aMediaDirection )
    {
    TLex8 attribute( aLine );
    TInt tokenNbr = 0;
    
    while( !attribute.Eos() )
        {
        TPtrC8 token = attribute.NextToken();

        if ( tokenNbr == KDesPrecType )
            {
            if ( token != KMceSipSdpQos )
                {
                return KErrNotFound;
                }
            }
        else if ( tokenNbr == KDesPrecStrength )
            {
            aStrength.Set( token );
            }
        else if ( tokenNbr == KDesPrecStatusType )
            {
            aAccessNetwork.Set( token );
            }
        else if ( tokenNbr == KDesPrecDirection )
            {
            aMediaDirection.Set( token );
            }
        else
            {
            //ignore trailing tokens
            }
        tokenNbr++;
        }
    
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DecodeMediaDirectionL
// -----------------------------------------------------------------------------
//
RStringF TMceSegmentedPreconditions::DecodeMediaDirectionL( const TDesC8& aMediaDirection )
    {
    RStringF mediaDirection = iKAttributeNone;
    
    RStringF direction = iStringPool.OpenFStringL( aMediaDirection );
    CleanupClosePushL( direction );

    if ( direction == iKAttributeSend )
        {
        mediaDirection = iKAttributeRecv;
        }
    else if ( direction == iKAttributeRecv )
        {
        mediaDirection = iKAttributeSend;
        }
    else if ( direction == iKAttributeSendRecv )
        {
        mediaDirection = iKAttributeSendRecv;
        }
    else if ( direction == iKAttributeNone )
        {
        mediaDirection = iKAttributeNone;
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    CleanupStack::PopAndDestroy();//direction
    
    return mediaDirection;
    
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::DecodeStrengthL
// -----------------------------------------------------------------------------
//
RStringF TMceSegmentedPreconditions::DecodeStrengthL( const TDesC8& aStrength )
    {
    RStringF strength = iKAttributeNone;
    
    RStringF tmpStrength = iStringPool.OpenFStringL( aStrength );
    CleanupClosePushL( tmpStrength );

    if ( tmpStrength == iKAttributeMandatory )
        {
        strength = iKAttributeMandatory;
        }
    else if ( tmpStrength == iKAttributeOptional )
        {
        strength = iKAttributeOptional;
        }
    else if ( tmpStrength == iKAttributeNone )
        {
        strength = iKAttributeNone;
        }
    else if ( tmpStrength == iKAttributeFailure )
        {
        strength = iKAttributeFailure;
        }
    else if ( tmpStrength == iKAttributeUnknown )
        {
        strength = iKAttributeUnknown;
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    CleanupStack::PopAndDestroy();//tmpStrength
    
    return strength;
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::Reserved
// -----------------------------------------------------------------------------
//
void TMceSegmentedPreconditions::Reserved()
    {
    //Mark local resources as reserved
    iCurrentLocalStatus = DesiredDirection( iKAttributeLocal );
    }

// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::ReservationNeeded
// -----------------------------------------------------------------------------
//
TBool TMceSegmentedPreconditions::RemoteReservationNeeded() const
    {
    return iCurrentRemoteStatus == iKAttributeNone;  
    }
    
// -----------------------------------------------------------------------------
// TMceSegmentedPreconditions::ReservationNeeded
// -----------------------------------------------------------------------------
//
TBool TMceSegmentedPreconditions::ReservationNeeded() const
    {
    return iCurrentLocalStatus == iKAttributeNone;
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::TMceSecurePreconditions
// -----------------------------------------------------------------------------
//
TMceSecurePreconditions::TMceSecurePreconditions( CMceComMediaStream& aStream )                                                    
    : TMcePreconditions( aStream )
    {
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::CloneL
// -----------------------------------------------------------------------------
//
TMcePreconditions* TMceSecurePreconditions::CloneL( 
    CMceComMediaStream& aStream )
    {
    TMceSecurePreconditions* copy = 
        new (ELeave) TMceSecurePreconditions( aStream );
    CleanupStack::PushL( copy );
    copy->ConstrucL( *this );
    CleanupStack::Pop( copy );
    return copy;
    }
    
// -----------------------------------------------------------------------------
// TMceSecurePreconditions::ConstrucL
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::ConstrucL( 
    const TMceSecurePreconditions& aPreconditions )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::ConstructL(), Entry ")
    TMcePreconditions::ConstrucL( aPreconditions );

    iCurrentLocalStatus = aPreconditions.iCurrentLocalStatus;
    iCurrentRemoteStatus = aPreconditions.iCurrentRemoteStatus;
    MCEMM_DEBUG_SVALUE("iCurrentRemoteStatus", iCurrentRemoteStatus.DesC() )
    iDesiredLocalSendRecv = aPreconditions.iDesiredLocalSendRecv;
    
    iDesiredRemoteSendRecv = aPreconditions.iDesiredRemoteSendRecv;

    iConfirmation = aPreconditions.iConfirmation;
    
    iType = aPreconditions.iType;
    MCEMM_DEBUG("TMceSecurePreconditions::ConstructL(), Exit ")     
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DoConstructL
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::DoConstructL()
    {
    MCEMM_DEBUG("TMceSecurePreconditions::DoConstructL(), Entry ")
    
    iCurrentLocalStatus = iKAttributeNone;
    iCurrentRemoteStatus = iKAttributeNone;
    iDesiredLocalSendRecv = iKAttributeNone;
    iDesiredRemoteSendRecv = iKAttributeNone;
    iConfirmation = iKAttributeNone;
    iType = TMcePreconditions::ESecPreconds;
    
    TInt direction = iStream.Direction();
    direction = direction == SdpCodecStringConstants::EAttributeInactive ? 
        iStream.SdpStreamType() : direction;
    
    switch( direction )
        {
        case SdpCodecStringConstants::EAttributeSendrecv:
        case SdpCodecStringConstants::EAttributeSendonly:
        case SdpCodecStringConstants::EAttributeRecvonly:
            {
            SetDesiredStatus( iKAttributeLocal, iKAttributeMandatory );
            MCEMM_DEBUG_SVALUE("desired direction ", iKAttributeSendRecv.DesC() )
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
        }    
    
    MCEMM_DEBUG("TMceSecurePreconditions::DoConstructL(), Exit ")
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::EncodeL
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::EncodeL( CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeL(), Entry ")
    
    TMceNegotiationRole role = iStream.Session()->NegotiationState().Role();

   //current e2e Status
    EncodeCurrentStatusL( aMedia, role );
    
    //desired e2e Status
    EncodeDesiredStatusL( aMedia );
    if ( iDesiredRemoteSendRecv == iKAttributeFailure )
    	{
    	iDesiredLocalSendRecv = iDesiredRemoteSendRecv;  
    	}
    //if answerer and confirmation is needed based on offerer's 
    //reservation status
    if ( role == EMceRoleAnswerer && ReservationNeeded() )
        {
        iConfirmation = iKAttributeSendRecv;
        EncodeConfirmationL( aMedia );
        }
        
    //reset confirmation
    iConfirmation = iKAttributeNone;
    
    //if not allready e2e reserved
    if ( !IsMet() )
        {
        SetState();
        }
    
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeL(), Exit ")
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DecodeL
// -----------------------------------------------------------------------------
//
TMceSipWarningCode TMceSecurePreconditions::DecodeL( CSdpMediaField& aMedia )
    {		
    MCEMM_DEBUG("TMceSecurePreconditions::DecodeL(), Entry ")
	
    TMceNegotiationRole role = iStream.Session()->NegotiationState().Role();
    RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
    TMceSipWarningCode warning = KErrNone;
    TInt decoded = 0;
    TInt index = 0;
    TBool anyPreconds = EFalse;
    if ( FindSecPreconds( aMedia ) )
    	{
		for ( index = 0; index < attributes.Count(); index++ )
		    {
		    CSdpAttributeField* attribute = attributes[ index ];
		    //if offerer and there is confirmation request
		    if ( attribute->Attribute() == iKAttributeConf && 
		         role == EMceRoleOfferer && 
		         attribute->Value().Find( KMceSipSdpSec ) != KErrNotFound )
		        {
		        decoded += DecodeConfStatusL( *attribute );
		        anyPreconds = ETrue;
		        }
		    //current and desired statuses are only decoded when reservation
		    //is still ongoing
		    else if ( !IsMet() || 
		              iDesiredRemoteSendRecv == iKAttributeFailure ||
		    		  iDesiredLocalSendRecv == iKAttributeFailure )
		        {
		        //decode current status
		        if ( attribute->Attribute() == iKAttributeCurr && 
		        	attribute->Value().Find( KMceSipSdpSec ) != KErrNotFound )
		            {
		            decoded += DecodeCurrentStatusL( *attribute );
		            }
		        //decode desired status
		        else if ( attribute->Attribute() == iKAttributeDes &&
		        		attribute->Value().Find( KMceSipSdpSec ) != KErrNotFound )
		            {
		            decoded += DecodeDesiredStatusL( *attribute );
		            anyPreconds = ETrue;
		            }
		        else
		            {
		            //NOP
		            }
		        }
		    else
		        {
		        //NOP
		        }
		    }

		//if something was decoded
		if ( decoded >= KMceSipPreconditionFailure )
			{
			iState = ECreated;
			return warning = KMceSipPreconditionFailure;
			}
		else if ( decoded )
		    {
		    SetState();
		    if ( EMceRoleOfferer == role &&
		         iKAttributeNone == iCurrentLocalStatus )
		        {
		        //decoding provisional response
		        iCurrentLocalStatus = iKAttributeRecv;
		        }
		    }
		else
		    {
		    //NOP
		    }
    	}
    
    // If nothing was decoded or preconditions were not sent in response
    // This is interpreted as resources are reserved  	
    if ( !decoded || !FindSecPreconds( aMedia ) )   	
    	{
    	iState = EReserved;
    	if ( iCurrentRemoteStatus == iKAttributeNone )
    	    {
    	    iCurrentRemoteStatus = iKAttributeSendRecv;
    	    }

	    //if there is required header but not any precondition present    
	    if ( iStream.Session()->iRemoteSecPreconditionsRequired && !anyPreconds )
	    	{
	    	iDesiredRemoteSendRecv = iKAttributeFailure;
	    	}
    	}
    	
    MCEMM_DEBUG("TMceSecurePreconditions::DecodeL(), Exit ")
    return warning;
    }
    
// -----------------------------------------------------------------------------
// TMceSecurePreconditions::EncodeCurrentStatusL
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::EncodeCurrentStatusL( 
    CSdpMediaField& aMedia,
    TMceNegotiationRole aRole )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeCurrentStatusL(), Entry ")
	
    HBufC8* currentStatus = NULL;
    CSdpAttributeField* secLine = NULL;
	RStringF current = iKAttributeNone;
    
    if ( aRole == EMceRoleAnswerer && 
         ReservationNeeded() )
    	{
    	current = iKAttributeRecv;
    	iCurrentLocalStatus = current;
    	}
    else
    	{
    	if ( aRole == EMceRoleOfferer  )
    	    {
    	    if ( iCurrentLocalStatus != iKAttributeNone )
    	        {
    	        current = iKAttributeSendRecv;
    	        }
    	    }
    	else // EMceRoleAnswerer
    	    {
    	    if ( iCurrentRemoteStatus != iKAttributeNone )
    	        {
    	        current = iKAttributeSendRecv;
    	        iState = EReserved;
    	        }		
    	    }		
    	}
    
    RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();                   
                           
    currentStatus = CurrentStatusTextLC( current.DesC() );
                                        
    secLine = CSdpAttributeField::NewLC( iKAttributeCurr, *currentStatus );
    MCEMM_DEBUG_SVALUE("encoded current status", secLine->Value() )
    attributes.AppendL( secLine );
    CleanupStack::Pop( secLine );
    CleanupStack::PopAndDestroy( currentStatus );
    
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeCurrentStatusL(), Exit ")
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::EncodeDesiredStatusL
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::EncodeDesiredStatusL( 
    CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeDesiredStatusL(), Entry ")
    
    TMceNegotiationRole role = iStream.Session()->NegotiationState().Role();
    HBufC8* desiredStatus = NULL;
    CSdpAttributeField* secLine = NULL;
   
    RStringF desired = iDesiredRemoteSendRecv == iKAttributeNone ? 
    			iKAttributeOptional : iKAttributeMandatory;  
    // for 183 always upgraded to mandatory			                                 
    desired = role ==  EMceRoleAnswerer ? iKAttributeMandatory : desired;	                                   
    iDesiredLocalSendRecv = desired;
    MCEMM_DEBUG_SVALUE("desired", desired.DesC() )    
    
    RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
    
    if ( iDesiredRemoteSendRecv == iKAttributeFailure )
    	{
    	desiredStatus = DesiredStatusTextLC( iKAttributeFailure.DesC(),
                                             iKAttributeSendRecv.DesC() );
                                      
    	}
    else
    	{
    	desiredStatus = DesiredStatusTextLC( desired.DesC(),
                                             iKAttributeSendRecv.DesC() );
    	}
    secLine = CSdpAttributeField::NewLC( iKAttributeDes, *desiredStatus );
    MCEMM_DEBUG_SVALUE("encoded desired send & recv status", secLine->Value() )
    attributes.AppendL( secLine );
    CleanupStack::Pop( secLine );
    CleanupStack::PopAndDestroy( desiredStatus );

    MCEMM_DEBUG("TMceSecurePreconditions::EncodeDesiredStatusL(), Exit ")
    }
	
// -----------------------------------------------------------------------------
// TMceSecurePreconditions::EncodeConfirmationL
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::EncodeConfirmationL( CSdpMediaField& aMedia )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeConfirmationL(), Entry ")
    
    if ( iConfirmation != iKAttributeNone )
        {
        HBufC8* confirmation = NULL;
        CSdpAttributeField* secLine = NULL;
        RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
                               
        confirmation = CurrentStatusTextLC( iConfirmation.DesC() );
                                            
        secLine = CSdpAttributeField::NewLC( iKAttributeConf, *confirmation );
        MCEMM_DEBUG_SVALUE("encoded confirmation status", secLine->Value() )
        attributes.AppendL( secLine );
        CleanupStack::Pop( secLine );
        CleanupStack::PopAndDestroy( confirmation );
        }
        
    MCEMM_DEBUG("TMceSecurePreconditions::EncodeConfirmationL(), Exit ")    
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DecodeCurrentStatusL
// -----------------------------------------------------------------------------
//
TUint TMceSecurePreconditions::DecodeCurrentStatusL( 
    CSdpAttributeField& aAttribute )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::DecodeCurrentStatusL(), Entry ")
    
    TUint decoded = (TUint)EFalse;
    TPtrC8 directionDesc;
    
    if ( ParseCurrentStatus( aAttribute.Value(), directionDesc ) )
        {
        decoded = (TUint)ETrue;
        TBool decodePrackCurrentStatus =
            ( iCurrentLocalStatus == iKAttributeRecv &&
              iCurrentRemoteStatus == iKAttributeNone ) ? ETrue : EFalse;
        TBool decode200OkCurrentStatus =
            ( iCurrentLocalStatus == iKAttributeRecv &&
              iCurrentRemoteStatus == iKAttributeSend ) ? ETrue : EFalse;
        if ( decodePrackCurrentStatus && 
             DecodeMediaDirectionL( directionDesc ) == iKAttributeNone )
            {
            //return 580 when decoding PRACK
            decoded = (TUint ) KMceSipPreconditionFailure; 
            iCurrentRemoteStatus == iKAttributeNone;
            }
        else if ( decode200OkCurrentStatus &&
                  DecodeMediaDirectionL( directionDesc ) != iKAttributeSendRecv )
            {
            //return 580 when decoding 200OK
            decoded = ( TUint ) KMceSipPreconditionFailure; 
            iCurrentRemoteStatus == iKAttributeNone;
            }
        else
            {
            SetRemoteStatus( DecodeMediaDirectionL( directionDesc ) );
            if ( decodePrackCurrentStatus || decode200OkCurrentStatus )
                {
                Reserved();
                }     		
            }
        MCEMM_DEBUG_SVALUE("decoded current status", aAttribute.Value() )
        }

    MCEMM_DEBUG("TMceSecurePreconditions::DecodeCurrentStatusL(), Exit ")
    return decoded;  
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DecodeDesiredStatusL
// -----------------------------------------------------------------------------
//
TUint TMceSecurePreconditions::DecodeDesiredStatusL( 
    CSdpAttributeField& aAttribute )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::DecodeCurrentStatusL(), Entry ")

    TUint decoded = ( TUint )EFalse;

    TPtrC8 strengthDesc; 
    TPtrC8 directionDesc;
    TMceNegotiationRole role = iStream.Session()->NegotiationState().Role();
    
    if ( ParseDesiredStatus( aAttribute.Value(), strengthDesc, directionDesc ) )
        {
        decoded = ( TUint )ETrue;
        RStringF strength = DecodeStrengthL( strengthDesc );
        SetDesiredStatus( iKAttributeRemote, strength );
        MCEMM_DEBUG_SVALUE("decoded desired status", aAttribute.Value() )
            
        if ( ( strength == iKAttributeMandatory && 
               !iStream.Session()->iClientCryptoSuites.Count() ) ||
             ( iCurrentLocalStatus == iKAttributeRecv && 
               iCurrentRemoteStatus == iKAttributeNone  && 
               role == EMceRoleAnswerer ) )
            {
            //there is no require header but desired status == mandatory
            decoded = (TUint ) KMceSipPreconditionFailure;  
            iDesiredRemoteSendRecv = iKAttributeFailure;
            HBufC8* desiredValue = DesiredStatusTextLC( 
                iKAttributeFailure.DesC(), iKAttributeSendRecv.DesC() );
            aAttribute.SetL( iKAttributeDes, *desiredValue);
            CleanupStack::PopAndDestroy( desiredValue );
            }
        if ( StrengthDowngraded( strength ) &&
             iCurrentLocalStatus != iKAttributeNone )
            {
            //downgrading strength not allowed after initiating negotiation
            MCEMM_DEBUG("Error: Strength downgraded")
            return (TUint) KMceSipPreconditionFailure; 
            }
        if ( strength == iKAttributeFailure || 
             iDesiredLocalSendRecv == iKAttributeFailure )	
            {
            decoded = (TUint ) KMceSipPreconditionFailure; 
            }
        if ( strength == iKAttributeOptional && 
             !iStream.Session()->iClientCryptoSuites.Count() )
            {
            iStream.Session()->Modifier( KMceSecPreconditions ) = 
                KMcePreconditionsNotUsed;
            }
        if ( strength == iKAttributeOptional && 
             iStream.Session()->iClientCryptoSuites.Count() )
            {
            iStream.Session()->Modifier( KMceSecPreconditions ) = 
                KMcePreconditionsE2ESupported;
            iStream.Session()->Modifier( KMcePreconditions ) = 
                KMcePreconditionsNotUsed;
            iStream.Session()->iRemoteSecPreconditionsRequired = ETrue;
            }
        }

    MCEMM_DEBUG("TMceSecurePreconditions::DecodeCurrentStatusL(), Exit ")
	return decoded;
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::SetCurrentStatus
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::SetRemoteStatus( RStringF aDirection )
	{   
   	if ( iCurrentRemoteStatus == iKAttributeNone ||
         aDirection == iKAttributeSendRecv )
        {
        iCurrentRemoteStatus = aDirection;
        }
    } 
    
// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DecodeConfStatusL
// -----------------------------------------------------------------------------
//
TUint TMceSecurePreconditions::DecodeConfStatusL( 
    CSdpAttributeField& aAttribute )
    {
    MCEMM_DEBUG("TMceSecurePreconditions::DecodeConfStatusL(), Entry ")

    TUint decoded = ( TUint )EFalse;
    TPtrC8 directionDesc;
    
    if ( ParseCurrentStatus( aAttribute.Value(), directionDesc ) )
        {
        decoded = (TUint)ETrue;
        iConfirmation = DecodeMediaDirectionL( directionDesc );
        MCEMM_DEBUG_SVALUE("decoded confirmation", aAttribute.Value() )
        }
    
    MCEMM_DEBUG("TMceSecurePreconditions::DecodeConfStatusL(), Exit ")
    return decoded;       
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::SetState
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::SetState()
    {
    MCEMM_DEBUG("TMceSecurePreconditions::SetState Entry ")
    iState = ENegotiating;
    
    if ( iCurrentRemoteStatus == iKAttributeSendRecv && 
         ( iDesiredRemoteSendRecv == iKAttributeMandatory ||
     	   iDesiredRemoteSendRecv == iKAttributeOptional ) )
     	{
     	Reserved();
     	}
    if ( !ReservationNeeded() &&
         !RemoteReservationNeeded() &&
         iConfirmation == iKAttributeNone )
        {
        MCEMM_DEBUG("SetState as Reserved ")
        iState = EReserved;
        }
    MCEMM_DEBUG_DVALUE ("Current Secure precondition STate ", iState )   
    MCEMM_DEBUG("TMceSecurePreconditions::SetState Exit ")
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::SetDesiredStatus
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::SetDesiredStatus( 
    RStringF aEntity,
    RStringF aStrength )
    {
    if ( aEntity == iKAttributeLocal )
        {
        iDesiredLocalSendRecv = aStrength;
        }
    else
        {
        iDesiredRemoteSendRecv = aStrength;
        }
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DesiredStatusTextLC
// -----------------------------------------------------------------------------
//
HBufC8* TMceSecurePreconditions::DesiredStatusTextLC( 
    const TDesC8& aStrength,  
    const TDesC8& aMediaDirection ) const
    {
    TInt desStatusTextLength = 
        KMceSipSdpSecPrecDesiredStatusPattern().Length() +
        aStrength.Length() +
        iKAttributeEnd2End.DesC().Length() +
        aMediaDirection.Length();
    
    HBufC8* desiredStatusText = HBufC8::NewLC( desStatusTextLength );
    desiredStatusText->Des().AppendFormat( 
        KMceSipSdpSecPrecDesiredStatusPattern,
        &aStrength, 
        &( iKAttributeEnd2End.DesC() ),
        &aMediaDirection );

    return desiredStatusText;
    } 

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::CurrentStatusTextLC
// -----------------------------------------------------------------------------
//
HBufC8* TMceSecurePreconditions::CurrentStatusTextLC( 
    const TDesC8& aMediaDirection ) const
    {
    TInt curStatusTextLength = 
        KMceSipSdpSecPrecCurrentStatusPattern().Length() + 
        iKAttributeEnd2End.DesC().Length() +
        aMediaDirection.Length();
    
    HBufC8* currentStatusText = HBufC8::NewLC( curStatusTextLength );
    
    currentStatusText->Des().AppendFormat( 
        KMceSipSdpSecPrecCurrentStatusPattern,
        &( iKAttributeEnd2End.DesC() ),
        &aMediaDirection );

    return currentStatusText;  
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::ParseCurrentStatus
// -----------------------------------------------------------------------------
//
TBool TMceSecurePreconditions::ParseCurrentStatus( 
    const TDesC8& aLine,
    TPtrC8& aMediaDirection )
    {
    TLex8 attribute( aLine );
    TInt tokenNbr = 0;
    
    while( !attribute.Eos() )
        {
        TPtrC8 token = attribute.NextToken();
        if ( tokenNbr == KCurrPrecType )
            {
            if ( token != KMceSipSdpSec )
                {
                return EFalse;
                }
            }
        else if ( tokenNbr == KCurrPrecStatusType )
            {
            if ( token != iKAttributeEnd2End.DesC() )
                {
                return EFalse;
                }
            }
        else if ( tokenNbr == KCurrPrecDirection )
            {
            aMediaDirection.Set( token );
            }
        else
            {
            //ignore trailing tokens
            }
        tokenNbr++;
        }

    return ETrue;   
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::ParseDesiredStatus
// -----------------------------------------------------------------------------
//
TBool TMceSecurePreconditions::ParseDesiredStatus( 
    const TDesC8& aLine,
    TPtrC8& aStrength,
    TPtrC8& aMediaDirection )
    {
    TLex8 attribute( aLine );
    TInt tokenNbr = 0;
    
    while( !attribute.Eos() )
        {
        TPtrC8 token = attribute.NextToken();

        if ( tokenNbr == KDesPrecType )
            {
            if ( token != KMceSipSdpSec )
                {
                return EFalse;
                }
            }
        else if ( tokenNbr == KDesPrecStrength )
            {
            aStrength.Set( token );
            }
        else if ( tokenNbr == KDesPrecStatusType )
            {
            if ( token != iKAttributeEnd2End.DesC() )
                {
                return EFalse;
                }
            }
        else if ( tokenNbr == KDesPrecDirection )
            {
            aMediaDirection.Set( token );
            }
        else
            {
            //ignore trailing tokens
            }
        tokenNbr++;
        }
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DecodeMediaDirectionL
// -----------------------------------------------------------------------------
//
RStringF TMceSecurePreconditions::DecodeMediaDirectionL( 
    const TDesC8& aMediaDirection )
    {
    RStringF mediaDirection = iKAttributeNone;
    
    RStringF direction = iStringPool.OpenFStringL( aMediaDirection );
    CleanupClosePushL( direction );

    if ( direction == iKAttributeSend )
        {
        mediaDirection = iKAttributeRecv;
        }
    else if ( direction == iKAttributeRecv )
        {
        mediaDirection = iKAttributeSend;
        }
    else if ( direction == iKAttributeSendRecv )
        {
        mediaDirection = iKAttributeSendRecv;
        }
    else if ( direction == iKAttributeNone )
        {
        mediaDirection = iKAttributeNone;
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    CleanupStack::PopAndDestroy();//direction
    
    return mediaDirection;
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::DecodeStrengthL
// -----------------------------------------------------------------------------
//
RStringF TMceSecurePreconditions::DecodeStrengthL( const TDesC8& aStrength )
    {
    RStringF strength = iKAttributeNone;
    
    RStringF tmpStrength = iStringPool.OpenFStringL( aStrength );
    CleanupClosePushL( tmpStrength );

    if ( tmpStrength == iKAttributeMandatory )
        {
        strength = iKAttributeMandatory;
        }
    else if ( tmpStrength == iKAttributeOptional )
        {
        strength = iKAttributeOptional;
        }
    else if ( tmpStrength == iKAttributeNone )
        {
        strength = iKAttributeNone;
        }
    else if ( tmpStrength == iKAttributeFailure )
        {
        strength = iKAttributeFailure;
        }
    else if ( tmpStrength == iKAttributeUnknown )
        {
        strength = iKAttributeUnknown;
        }
    else
        {
        User::Leave( KErrArgument );
        }
        
    CleanupStack::PopAndDestroy();//tmpStrength
    
    return strength;
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::Reserved
// -----------------------------------------------------------------------------
//
void TMceSecurePreconditions::Reserved()
    {
    iCurrentLocalStatus = iKAttributeSendRecv;
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::ReservationNeeded
// -----------------------------------------------------------------------------
//
TBool TMceSecurePreconditions::RemoteReservationNeeded() const
    {
    return iCurrentRemoteStatus == iKAttributeNone;     
    }
    
// -----------------------------------------------------------------------------
// TMceSecurePreconditions::ReservationNeeded
// -----------------------------------------------------------------------------
//
TBool TMceSecurePreconditions::ReservationNeeded() const
    {
    return ( iCurrentLocalStatus == iKAttributeNone || 
    		 iCurrentLocalStatus == iKAttributeRecv );
    }

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::FindSecPreconds
// -----------------------------------------------------------------------------
//
TBool TMceSecurePreconditions::FindSecPreconds( CSdpMediaField& aMedia )
	{
	TBool decodedSec = EFalse;
	if ( &aMedia )
		{
		RPointerArray<CSdpAttributeField>& attributes = aMedia.AttributeFields();
	    
	    TInt index = 0;

	    for( index = 0; index < attributes.Count(); index++ )
		    {
		    CSdpAttributeField* attribute = attributes[ index ];
		    //decode current status
		    
		    if ( attribute->Attribute().DesC().Match( KMceSipSdpPreconditionCurr) != KErrNotFound )
		        {
		        TLex8 attributeValue( attribute->Value() );
		    	TPtrC8 token = attributeValue.NextToken();
			    if ( token == KMceSipSdpSec )
			    	{
			    	return decodedSec = ETrue;
			       	}
		        }
	  		}
		}
	return decodedSec; 	
	}	

// -----------------------------------------------------------------------------
// TMceSecurePreconditions::StrengthDowngraded
// -----------------------------------------------------------------------------
//
TBool TMceSecurePreconditions::StrengthDowngraded( RStringF aStrength )
    {
    TBool decision( ETrue ); 
    if ( aStrength == iDesiredLocalSendRecv )
        {
        decision = EFalse;
        }
    else if ( iDesiredLocalSendRecv == iKAttributeMandatory )
        {
        decision = ETrue;
        }
    else if ( iDesiredLocalSendRecv == iKAttributeOptional )
        {
        decision = !( aStrength == iKAttributeMandatory );
        }
    else
        {
        decision = EFalse;
        }
    return decision;
    }	

// -----------------------------------------------------------------------------
// TMcePreconditionsFactory::CreateL
// -----------------------------------------------------------------------------
//
TMcePreconditions* TMcePreconditionsFactory::CreateL( 
    CMceComMediaStream& aMediaStream,
    CSdpMediaField& aMediaLine )
	{
	//for decoding
	//check if there is a=curr:sec
	MCEMM_DEBUG("TMcePreconditionsFactory::CreateL EntryL")
	if ( &aMediaLine )
        {	
		RPointerArray<CSdpAttributeField>& attributes = aMediaLine.AttributeFields();
	    
	    TBool decodedSec = EFalse;
	    TInt index = 0;

	    for( index = 0; index < attributes.Count(); index++ )
	        {
	        CSdpAttributeField* attribute = attributes[ index ];
	        //decode current status	    
	        if ( attribute->Attribute().DesC().Match( KMceSipSdpPreconditionCurr) != KErrNotFound )
	            {
	            TLex8 attributeValue( attribute->Value() );
	    	    TPtrC8 token = attributeValue.NextToken();
		        if ( token == KMceSipSdpSec )
		    	    {
		    	    decodedSec = ETrue;
		       	    }      
	            }
	        }
	        
	    if ( decodedSec )    
	    	{
	    	aMediaStream.Session()->iIsSecureSession = ETrue;
	    	TMceSecurePreconditions* secPreconditions =
	        new (ELeave) TMceSecurePreconditions( aMediaStream );
	    	CleanupStack::PushL( secPreconditions );
	    	secPreconditions->ConstructL();
	        CleanupStack::Pop( secPreconditions );
	        aMediaStream.Session()->Modifier( KMceSecPreconditions ) 
	        	= KMcePreconditionsE2ESupported;
	        aMediaStream.Session()->iRemoteSecPreconditionsRequired = ETrue;
	        MCEMM_DEBUG("return Sec Preconds")
	        MCEMM_DEBUG("TMcePreconditionsFactory::CreateL ExitL")
		
			return secPreconditions;
	    	}
		}
    else if ( aMediaStream.Session()->iClientCryptoSuites.Count() &&
    		  aMediaStream.Session()->Modifier( KMceSecPreconditions ) 
    		    == KMcePreconditionsE2ESupported)
    		
    	{
    	//for encoded
    	TMceSecurePreconditions* secPreconditions =
        new (ELeave) TMceSecurePreconditions( aMediaStream );
    	CleanupStack::PushL( secPreconditions );
    	secPreconditions->ConstructL();
        CleanupStack::Pop( secPreconditions );
        MCEMM_DEBUG("for encoding preconds return Sec Preconds")
	
	    MCEMM_DEBUG("TMcePreconditionsFactory::CreateL ExitL")
       	return secPreconditions;
    	}
    else
    	{
    	//nop
    	}
	MCEMM_DEBUG("TMcePreconditionsFactory::CreateL ExitL")
       
	return NULL;
	}
	
// -----------------------------------------------------------------------------
// TMcePreconditionsFactory::CreateL
// -----------------------------------------------------------------------------
//
TMcePreconditions* TMcePreconditionsFactory::CreateL( 
    CMceComMediaStream& aMediaStream )
	{
	//only for segmented precondition	 
  	TMceSegmentedPreconditions*  preconditions =
        new (ELeave) TMceSegmentedPreconditions( aMediaStream );
    CleanupStack::PushL( preconditions );
    preconditions->ConstructL();
    preconditions->Reserved();
    CleanupStack::Pop( preconditions );
    return preconditions;
	}

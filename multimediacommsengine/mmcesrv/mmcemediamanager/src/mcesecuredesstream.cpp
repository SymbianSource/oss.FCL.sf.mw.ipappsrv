/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides services for SDP Security Descriptions.
*
*/




// INCLUDE FILES
#include    "mcesecuredesstream.h"
#include	"mcemmlogs.h"
#include    <tconvbase64.h> // TImCodecB64
#include    <random.h> 
#include    <e32math.h>
#include 	<sdpdocument.h>
#include    <sdpcodecstringconstants.h>
#include    <sdpcodecstringpool.h>
#include    <sdpmediafield.h>
#include    <sdpfmtattributefield.h>
#include    <sdpattributefield.h>
#include    <mmccsrtpmasterkey.h>
#include    <mmccsrtpmastersalt.h>
#include 	<mmccsecureinterface.h>
#include 	"mcesecuresession.h"
#include 	"mcecomsession.h"
#include 	"mcecommediastream.h"
#include 	"mcesip.h"
#define MSG_IGNORE_RETURN( )\
    if( iGnoreSdpMsg ){ iSecureSession.RemoveSecureCrypto(); return; }
        
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceSecureDesStream::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::ConstructL(CSdpMediaField& aMediaField)
    {
   	iCryptoOuts = new ( ELeave ) CArrayFixFlat< TMceSecureCryptoInfo >( KCryptoGranularity );
    iCryptoIns = new ( ELeave ) CArrayFixFlat< TMceSecureCryptoInfo >( KCryptoGranularity );

    iMediaField=aMediaField.CloneL();
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMceSecureDesStream* CMceSecureDesStream::NewL( CMceSecureMediaSession& aSecureSession,
												CSdpMediaField& aMediaField,
											CMccSecureInterface& aSecureInterface,
											CMceComMediaStream& aMediaStream)
    {
    CMceSecureDesStream* self = new (ELeave) CMceSecureDesStream(aSecureSession,
    															aSecureInterface,
    															aMediaStream);
    CleanupStack::PushL( self );
    self->ConstructL(aMediaField);
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::CMceSecureDesStream
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMceSecureDesStream::CMceSecureDesStream(	CMceSecureMediaSession& aSecureSession,
											CMccSecureInterface& aSecureInterface,
        									CMceComMediaStream& aMediaStream
        									):
	
	iSecureSession(aSecureSession),
	iSecInf(aSecureInterface),
	iMediaStream(aMediaStream),
	iIsSAVP(ETrue),
	iWaitingBinding(EFalse),
	iCryptoContextOutId(0),
	iCryptoContextInId(0),
	iOldLocalMediaPort(0)
	{
	
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::CMceSecureDesStream
// Destructor
// -----------------------------------------------------------------------------
//
CMceSecureDesStream::~CMceSecureDesStream()
    {
    if(iCryptoOuts!=NULL)
        {
    	if ( iCryptoOuts->Count() )
    		{
    		iCryptoOuts->Reset();
    		}
        }
    if(iCryptoIns!=NULL)
        {
	    if ( iCryptoIns->Count() )
    		{
    		iCryptoIns->Reset();
    		}
        }
    delete iCryptoOuts;
    delete iCryptoIns;
    iCryptoOut.iMasterKeys.Close();
    iCryptoIn.iMasterKeys.Close();
    iCryptoOut.iSaltKeys.Close();
    iCryptoIn.iSaltKeys.Close();
    iCryptoOut.iEncodedKeys.Close();
    iCryptoIn.iEncodedKeys.Close();
   	iCryptoOut.CryptoDestruct();
   	iCryptoIn.CryptoDestruct();
   	delete iMediaField;
   	delete iMKIBuf;
    }
    

// -----------------------------------------------------------------------------
// CMceSecureDesStream::GenerateRandomKeys
// Generates security keys
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::GenerateRandomKeys( TMceSecureCryptoInfo& aCrypto )
    {
    MCEMM_DEBUG("CMceSecureDesStream::GenerateRandomKeys, Entry ")
    
    MCEMM_DEBUG("Generate Keys")
	TBuf8< KMasterKeyAndSaltLength > random;
	RandomString( random );

	// Encode random string
	TBuf8< KEncodedStringLength > encoded;
	Base64Encode( random, encoded );
	aCrypto.ResetKeys();
	aCrypto.iEncodedKey = encoded;
	//added generated masterkey
	aCrypto.iSetMasterKey = random.Mid( 0, KMasterKeyLength );

	//added generated salt key
	aCrypto.iSetSaltKey = random.Mid( KMasterKeyLength );
	aCrypto.iKeysCreated = ETrue;
	random.Zero();
	
    MCEMM_DEBUG("CMceSecureDesStream::GenerateRandomKeys, Exit ")   
    }
    
// -----------------------------------------------------------------------------
// CMceSecureDesStream::EncodeSecureSdpL()
// Generates security key to SDP offer/Answer.
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::EncodeSecureSdpL( 
    CSdpMediaField& aMediaField,
    TBool aIsAnswer )
    {
   	MCEMM_DEBUG("CMceSecureDesStream::EncodeSecureSdpL(), Entry ")
    
    iGnoreSdpMsg = EFalse;
    
    const TUint8 KCryptoTag = 1;
    
   	TInt authCount = ClientCrytoCount();
   	authCount = authCount ? authCount : KAuthTagLengthTypeCount;
   	MCEMM_DEBUG_DVALUE("iKeyNeedUpdated", iSecureSession.iKeyNeedUpdated )
  	TBuf8< KCryptoLineMaxLength > cryptoLine;
  	
    if ( aIsAnswer ) 
    	{
    	MCEMM_DEBUG("Encode Secure Answer")
		if ( iSecureSession.iCryptoContextUpdate )
		    {
		    iCryptoOuts->Reset();
		    iSecureSession.iCryptoContextUpdate = EFalse;	
		    }

    	iCryptoOut.iTag = KCryptoTag;
       	GenerateCryptoLineL( cryptoLine, iCryptoOut.iTag, aIsAnswer );

    	if ( !iGnoreSdpMsg && 
    	     KCryptoAttributeValueMinLength <= cryptoLine.Length() &&
             iCryptoOut.iEncodedKey.Length() > 0 )
            {
    		AppendCryptoAttributeL(	cryptoLine, aMediaField );    
    		}
        cryptoLine.Zero();                        
    	}
    else if ( iSecureSession.iKeyNeedUpdated && 
              iSecureSession.iCryptoContextUpdate )
    	{
    	MCEMM_DEBUG("Encode Secure Offer; Crypto Update Required")		
    	iSecureSession.iCryptoContextUpdate = EFalse;
    	TInt tagIndex = 0;
    	TInt tag;
    	iCryptoOuts->Reset();
   		for ( tagIndex = 0; tagIndex < authCount; tagIndex++ )
			{
			tag = tagIndex+1;
			GenerateCryptoLineL( cryptoLine, tag, aIsAnswer );	

		    if ( !iGnoreSdpMsg && 
		         KCryptoAttributeValueMinLength <= cryptoLine.Length() )
		        {
		        AppendCryptoAttributeL(	cryptoLine, aMediaField );  
		        }
			cryptoLine.Zero();
			}
    	}     	
   	else 
   		{ 
   		MCEMM_DEBUG("Encode Updated Offer; No Crypto Update Required")	
   		iCryptoOut.iTag = KCryptoTag;
       	GenerateCryptoLineL( cryptoLine, iCryptoOut.iTag, aIsAnswer );	
	
		if ( !iGnoreSdpMsg && 
		      KCryptoAttributeValueMinLength <= cryptoLine.Length() &&
              iCryptoOut.iEncodedKey.Length() > 0 )
            {
		    AppendCryptoAttributeL(	cryptoLine, aMediaField );    
		    }	
       	cryptoLine.Zero();                        
   		}       	    
    	
    if ( !iGnoreSdpMsg )
    	{
    	SetSecureProtocolL( aMediaField );
    	iCryptoUpdateNeeded = 
    	    iCryptoUpdateNeeded ? EFalse : iCryptoUpdateNeeded;
    	MCEMM_DEBUG("Encode Secure Crypto Succeeded")
    	}
   	
   	if ( !aMediaField.IsValid() )
   	    {
   	    User::Leave( KErrCorrupt );
   	    }
    
    MCEMM_DEBUG("CMceSecureDesStream::EncodeSecureSdpL(), Exit")	
    }


// -----------------------------------------------------------------------------
// CMceSecureDesStream::StoreKeys()
// Store security keys
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::StoreKeys( TDesC8& aData )
    {
    //DecodeAnswer or Offer
    //multiple tag
    	/*a=crypto:1 AES_CM_128_HMAC_SHA1_80
       inline:WVNfX19zZW1jdGwgKCkgewkyMjA7fQp9CnVubGVz|2^20|1:4
       FEC_ORDER=FEC_SRTP
      a=crypto:2 F8_128_HMAC_SHA1_80
       inline:MTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5QUJjZGVm|2^20|1:4;
       inline:QUJjZGVmMTIzNDU2Nzg5QUJDREUwMTIzNDU2Nzg5|2^20|2:4
       FEC_ORDER=FEC_SRTP
	*/
    
    TBuf8 < KMasterKeyAndSaltLength > masterKeyAndSalt;
    Base64Decode( aData, masterKeyAndSalt );
    
	//it should be validate already so it is our prefered key
	iCryptoIn.iEncodedKey = aData;
	iCryptoIn.iSetMasterKey = masterKeyAndSalt.Mid( 0, KMasterKeyLength );
	iCryptoIn.iSetSaltKey = masterKeyAndSalt.Mid( KMasterKeyLength );
	iCryptoIn.iKeysCreated = ETrue;
    }
        

// -----------------------------------------------------------------------------
// CMceSecureDesStream::DecodeSecureSdpL()
// Decode Secure Sdp Offer / Update
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::DecodeSecureSdpL( CSdpMediaField& aMediaField )
    {
    MCEMM_DEBUG("CMceSecureDesStream::DecodeSecureSdpL(), Entry")
    MCEMM_DEBUG_DVALUE("iKeyNeedUpdated", iSecureSession.iKeyNeedUpdated )
    iGnoreSdpMsg = EFalse;
    CSdpAttributeField* attributeField = NULL;    
    
	RPointerArray< CSdpAttributeField > attrList =  
                aMediaField.AttributeFields();
    TInt attrCount = attrList.Count();
    TInt cryptoAttrCount =
        iSecureSession.SdpCryptoAttributeCount( aMediaField );
    iCryptoIns->Reset();
    RemoveClientCrypto();

    for (TInt j = 0; j < attrCount; j++ )
        {
       	attributeField = attrList[ j ];

        RStringF attribute = attributeField->Attribute();

        // check if attribute is 'crypto'
        if ( attribute.DesC().Match(KCrypto)!= KErrNotFound )
            {
           	--cryptoAttrCount;
           	//check how many inline
           	ValidateOfferByAnswerL( attributeField->Value() );
           	if ( !cryptoAttrCount && iGnoreSdpMsg && !iCryptoIns->Count() )
           		{
           		//Last attribute ignored and no valid attributes found
           		User::Leave ( KErrArgument );
           		}
		    if ( !cryptoAttrCount )
		        {
		        break;
		        }
		    }
        }
	if ( iCryptoIns->Count() )
	    {
	    //acceptable crypto suite found, not to be ignored
	    iGnoreSdpMsg = EFalse;
	    }
    if ( !iGnoreSdpMsg && SetMediaProfile ( aMediaField ) == KErrNone )
    	{
    	SetMultipleClientCryptoL( *iCryptoIns );
    	MCEMM_DEBUG(" Set Multiple Client Cryptoto")
    	User::LeaveIfError ( CountCryptoInOffer( aMediaField ) );
    	Session().iIsSecureSession =  !iGnoreSdpMsg ? ETrue : EFalse;
    	}
    MCEMM_DEBUG("CMceSecureDesStream::DecodeSecureSdpL(), Exit")      
    MSG_IGNORE_RETURN()
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::DecodeSecureSdpAnswerL()
// Decode Secure Sdp Offer/Answer
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::DecodeSecureSdpAnswerL( CSdpMediaField& aMediaField)
    {
    MCEMM_DEBUG("CMceSecureDesStream::DecodeSecureSdpAnswerL(), Entry")  
    MCEMM_DEBUG_DVALUE("iKeyNeedUpdated", iSecureSession.iKeyNeedUpdated)
    TInt cryptoCount = iSecureSession.SdpCryptoAttributeCount(aMediaField);
    
    if ( cryptoCount != KCryptoAnswerCount )
    	{
    	User::Leave( KErrArgument );
    	}
	
	CSdpAttributeField* attr = MediaFieldAttrMatch( aMediaField, KCrypto);    
    
    // check if attribute is 'crypto' 
    if ( attr )
        {
        ValidateAnswerByOfferL( attr->Value() );
        
    	if ( !iGnoreSdpMsg  && SetMediaProfile ( aMediaField ) == KErrNone )
			{
			if ( iSecureSession.iKeyNeedUpdated )
				{
				Session().iIsSecureSession = ETrue;
				
				Session().iClientCryptoSuites.Reset();
				
				SetClientCryptoL( iCryptoIn );
				}
			if ( iSecureSession.iLSReadyToBind )
				{
				SetCryptoContextL();
				}
			MSG_IGNORE_RETURN()
			if ( !iCryptoIn.iCryptoSuite.Length() )
				{
				User::Leave( KErrArgument );
				}
			MCEMM_DEBUG("Set Crypto Context succeed")  	
			
			}
	
		else
			{
			MCEMM_DEBUG("No suitable crypto or malformate crypto session reject")      
    
			User::Leave( KErrArgument );
			}
		}
    MCEMM_DEBUG("CMceSecureDesStream::DecodeSecureSdpAnswerL(), Exit")    
  	MSG_IGNORE_RETURN()
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::RemvoeSecureSdpL
// Remove Secure Sdp Offer/Answer
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::RemvoeSecureSdp(CSdpMediaField& aMediaField)
	{
	MCEMM_DEBUG("CMceSecureDesStream::RemvoeSecureSdpL(), Entry") 
    
    RStringF mediaType = aMediaField.Media();
    
    RPointerArray< CSdpAttributeField> attrList =  
                    aMediaField.AttributeFields();
	
	TInt attrCount = attrList.Count();
    
    for (TInt index = attrCount; index >0 ; index-- )
        {
        CSdpAttributeField* attributeField = attrList[index-1];
        RStringF attribute = attributeField->Attribute();
                 // check if attribute is 'crypto' 
        if ( KErrNotFound != attribute.DesC().Find( KCrypto ) )
            {   
            TInt removing =aMediaField.AttributeFields().Find(attributeField);
            aMediaField.AttributeFields().Remove(removing);
            delete attributeField;
            attributeField=NULL;
            }
        }
	mediaType.Close();
	MCEMM_DEBUG("CMceSecureDesStream::RemvoeSecureSdpL(), Exit")     
    
	}

// -----------------------------------------------------------------------------
// CMceSecureDesStream::SetCryptoContext( )
// Sets crypto context to MCC
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetCryptoContextL( )
    {
    MCEMM_DEBUG("SetCryptoContext(), Entry")   
    TBool storedIgnoreSdpMsg = EFalse;
    //Check state first if the crypto has been set
    if ( !iCryptoIn.iIfCryptoContextIdSet && 
    	!iCryptoOut.iIfCryptoContextIdSet &&
    	iCryptoIn.iSetMasterKey.Length() && 
    	iCryptoIn.iSetSaltKey.Length() &&
    	iCryptoOut.iSetMasterKey.Length() &&
    	iCryptoOut.iSetSaltKey.Length() )
    	{
    	MCEMM_DEBUG("First Time Bind")
    	CreateCryptoContextL( iCryptoIn );
    	CreateCryptoContextL( iCryptoOut ); 
    	MCEMM_DEBUG_DVALUE( "iCryptoContextOutId", iCryptoIn.iCryptoContextId )
		MCEMM_DEBUG_DVALUE( "iCryptoContextInId", iCryptoOut.iCryptoContextId )

    	iCryptoOuts->Reset();
    	
    
    	}
    else if ( iCryptoIn.iSetMasterKey.Length() && 
    		iCryptoIn.iSetSaltKey.Length() &&
    	  	iCryptoOut.iSetMasterKey.Length() &&
    	 	iCryptoOut.iSetSaltKey.Length() )
		{
		MCEMM_DEBUG("Bind has done before so this is for updated")  
		if ( iCryptoContextInId != 0  && iCryptoContextOutId != 0 )
			{
			iCryptoIn.iCryptoContextId = iCryptoContextInId;
			iCryptoOut.iCryptoContextId =  iCryptoContextOutId;
			}
		MCEMM_DEBUG_DVALUE( "Updated iCryptoContextOutId", iCryptoIn.iCryptoContextId )
		MCEMM_DEBUG_DVALUE( "Updated iCryptoContextInId", iCryptoOut.iCryptoContextId )
    	if ( iSecureSession.iKeyNeedUpdated )
    		{
    		UpdateCryptoContextL( iCryptoIn ); 
    		storedIgnoreSdpMsg = iGnoreSdpMsg;
    		UpdateCryptoContextL( iCryptoOut ); 
    		iGnoreSdpMsg = iGnoreSdpMsg && storedIgnoreSdpMsg;
    		}
    	else
    		{
    		iWaitingBinding = ETrue;	
    		}
    	iCryptoOuts->Reset(); 
    	}
    iGnoreSdpMsg = (iCryptoIn.iIfCryptoContextIdSet && 
    				iCryptoOut.iIfCryptoContextIdSet ) &&
    				!iGnoreSdpMsg ? EFalse : ETrue;		
    if ( iWaitingBinding )
    	{
    	iSecureSession.BindStreamCrypto();
    	}
    	
    MCEMM_DEBUG("SetCryptoContext(), Exit")
    }


// -----------------------------------------------------------------------------
// CMceSecureDesStream::CreateCryptoContextL()
// 
// -----------------------------------------------------------------------------
//     
void CMceSecureDesStream::CreateCryptoContextL( TMceSecureCryptoInfo& aCrypto )
    {
    MCEMM_DEBUG("CMceSecureDesStream::CreateCryptoContextL Entry")
    // if  ESrtpNullAlg == iEncAlgms Fallback to RTP, set NULL policy
    // Create phony master and salt from local values 
    // since remote keys not available
    iWaitingBinding = EFalse;
    TInt err(KErrNone);
    FormMKIL(aCrypto);
    TBuf8<KMKILength> mki;
    mki.Copy( KNullDesC8 );
   	TMccSrtpMasterKey masterKey(aCrypto.iSetMasterKey, mki); 
   
    if (aCrypto.iMKIUsed && iMKIBuf)
    	{
    	masterKey.iMKI.Copy(iMKIBuf->Des()); 
    	masterKey.iMKI.SetLength(aCrypto.iMKILength);
    	}
    TMccSrtpMasterSalt saltKey( aCrypto.iSetSaltKey);
	TMccSrtpCryptoParams params;
    
    params.iSrtpEncAlg = aCrypto.iEncAlgms; 
    params.iSrtcpEncAlg = aCrypto.iEncAlgms; 
    params.iSrtpAuthAlg = aCrypto.iAuthAlgms; 
    params.iSrtcpAuthAlg = aCrypto.iAuthAlgms; 
    params.iSrtpAuthTagLen = aCrypto.iTagLen;
    params.iSrtcpAuthTagLen = aCrypto.iTagLen;
    if ( aCrypto.iMKLifeTime )
    	{
    	params.iMasterKeysLifeTime = aCrypto.iMKLifeTime;
    	}
    
    
    TMccSrtpMasterKeyPckg masterKeyPckg(masterKey); 
    const TDesC8& masterKeyDes = masterKeyPckg;
    
    TMccSrtpMasterSaltPckg masterSaltPckg( saltKey );  
    const TDesC8& saltKeyDes = masterSaltPckg;                      
   	
   	TMccSrtpCryptoParamsPckg cryptoParamsPckg ( params );
   	const TDesC8& cryptoParamsDes = cryptoParamsPckg;   
   	//remember to handle and pass the keys
   	err = iSecInf.CreateContext(masterKeyDes, saltKeyDes,
	                                 aCrypto.iCryptoContextId, cryptoParamsDes );  
	if ( err == KErrNone )
		{
		MCEMM_DEBUG("Create Crypto Succeed")
		aCrypto.iIfCryptoContextIdSet = ETrue;
		iWaitingBinding = ETrue;
		}
	
	MCEMM_DEBUG_DVALUE( "createContext error", err )		
	MCEMM_DEBUG("CMceSecureDesStream::CreateCryptoContextL Exit")
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::UpdateCryptoContext()
// 
// -----------------------------------------------------------------------------
//     
void CMceSecureDesStream::UpdateCryptoContextL( TMceSecureCryptoInfo& aCrypto )
    {
    MCEMM_DEBUG("CMceSecureDesStream::UpdateCryptoContextL Entry")	
    TInt err(KErrNone);
    iWaitingBinding = EFalse;
   
    aCrypto.iIfBinded = EFalse;
    FormMKIL(aCrypto);
    TBuf8<KMKILength> mki;
    mki.Copy( KNullDesC8 );
   	TMccSrtpMasterKey masterKey( aCrypto.iSetMasterKey, mki ); 
   
    if ( aCrypto.iMKIUsed && iMKIBuf )
    	{
    	masterKey.iMKI.Copy(iMKIBuf->Des()); 
    	}
    TMccSrtpMasterSalt saltKey( aCrypto.iSetSaltKey);
	TMccSrtpCryptoParams params;
    
    params.iSrtpEncAlg = aCrypto.iEncAlgms; 
    params.iSrtcpEncAlg = aCrypto.iEncAlgms; 
    params.iSrtpAuthAlg = aCrypto.iAuthAlgms; 
    params.iSrtcpAuthAlg = aCrypto.iAuthAlgms; 
    params.iSrtpAuthTagLen = aCrypto.iTagLen;
    params.iSrtcpAuthTagLen = aCrypto.iTagLen;
    if ( aCrypto.iMKLifeTime )
    	{
    	params.iMasterKeysLifeTime = aCrypto.iMKLifeTime;
    	}
    
    
    TMccSrtpMasterKeyPckg masterKeyPckg(masterKey); 
    const TDesC8& masterKeyDes = masterKeyPckg;
    
    TMccSrtpMasterSaltPckg masterSaltPckg( saltKey );  
    const TDesC8& saltKeyDes = masterSaltPckg;                      
   	
   	TMccSrtpCryptoParamsPckg cryptoParamsPckg ( params );
   	const TDesC8& cryptoParamsDes = cryptoParamsPckg;  

	err = iSecInf.UpdateContext( masterKeyDes, saltKeyDes,
                             aCrypto.iCryptoContextId, cryptoParamsDes );

	if ( err==KErrNone)
		{
		MCEMM_DEBUG("Updated Crypto Succeed")
		aCrypto.iIfCryptoContextIdSet = ETrue;
		iWaitingBinding = ETrue;
		}
	//This is to check if the crypto Id after update can be found
	// if not found, context Id maybe wrong	
	iGnoreSdpMsg = err == KErrNotFound ? ETrue : EFalse;	
	MCEMM_DEBUG_DVALUE( "UpdateContext error", err )
	MCEMM_DEBUG("CMceSecureDesStream::UpdateCryptoContextL Exit")	
	}

// -----------------------------------------------------------------------------
// CMceSecureDesStream::FormMKIL()
// 
// -----------------------------------------------------------------------------
//     
void CMceSecureDesStream::FormMKIL( TMceSecureCryptoInfo& aCrypto )
    {
    delete iMKIBuf;
    iMKIBuf=NULL;
    if ( aCrypto.iMKILength > 0 && 
         aCrypto.iMKILength%4 == 0 &&
         aCrypto.iMKIUsed )
    	{
    	__ASSERT_ALWAYS( aCrypto.iMKILength <= KMKIMaxLength,
    	                 User::Leave( KErrOverflow ) );
    	
    	iMKIBuf = HBufC8::NewL( aCrypto.iMKILength );
    	TUint8* mkiData = const_cast<TUint8*>( iMKIBuf->Des().Ptr() );
    	Mem::FillZ( mkiData, aCrypto.iMKILength );
    	if (aCrypto.iMKILength > KMKILength)
    		{
    		mkiData+= ( aCrypto.iMKILength - KMKILength );
    		}
		TUint32 mkiValue=aCrypto.iMKIValue;
		BigEndian::Put32(mkiData, mkiValue);
    	mkiData += KMKILength;
    	iMKIBuf->Des().SetLength( aCrypto.iMKILength );
    	}
    
    }
// -----------------------------------------------------------------------------
// CMceSecureDesStream::BindCrypto( )
// -----------------------------------------------------------------------------
//

TInt CMceSecureDesStream::BindCrypto( CMceSrvStream& aStream )
    					
	{
	MCEMM_DEBUG("CMceSecureDesStream::BindCrypto(), Entry")				
	
	MCEMM_DEBUG_DVALUE("	BindStreamId", aStream.Id() )	
	MCEMM_DEBUG_DVALUE("	BindLinkId", aStream.LinkId() )	
	MCEMM_DEBUG_DVALUE("	iCryptoIn.iIfBinded", iCryptoIn.iIfBinded )
	MCEMM_DEBUG_DVALUE("	iCryptoOut.iIfBinded", iCryptoOut.iIfBinded )
	
	TInt err(KErrNotFound);
	if (iCryptoIn.iEncodedKey.Length() && 
		iCryptoIn.iIfCryptoContextIdSet &&
		IS_RECEIVESTREAM(&aStream.Data()) )
		{
		MCEMM_DEBUG_DVALUE("	BindContext In Id", iCryptoIn.iCryptoContextId )	
		MCEMM_DEBUG_DVALUE("	BindSourceId", aStream.Source().Id() )
	
		err = iSecInf.Bind( aStream.SessionId(), 
								aStream.LinkId(),
								aStream.Id(), 
								aStream.Source().Id(), 
								iCryptoIn.iCryptoContextId );	
		
		MCEMM_DEBUG_DVALUE("	BindSource result=", err)
		
		if ( err == KErrNone )
			{
			iCryptoIn.iIfBinded = ETrue;
			MCEMM_DEBUG("Bind Downlink Source Successful")
			}
					
		}


	if (iCryptoOut.iEncodedKey.Length() && 
		iCryptoOut.iIfCryptoContextIdSet &&
		IS_SENDSTREAM(&aStream.Data())  )
		{
	
		MCEMM_DEBUG_DVALUE("	BindContext Out Id", iCryptoOut.iCryptoContextId )	
	    MCEMM_DEBUG_DVALUE("	BindSinkId", aStream.Sink().Id() )
	
		err =iSecInf.Bind( aStream.SessionId(), 
								aStream.LinkId(),
								aStream.Id(), 
								aStream.Sink().Id(), 
								iCryptoOut.iCryptoContextId );	
		
		MCEMM_DEBUG_DVALUE("	BindSink result=", err)				

		if (err == KErrNone)
			{
			iCryptoOut.iIfBinded=ETrue;
			MCEMM_DEBUG("	Bind Uplink Sink Successful")
			}
		
		}							

	if (iCryptoIn.iIfBinded && iCryptoOut.iIfBinded)
		{
		MCEMM_DEBUG("	Bind Successful for Downlink and Uplink")
		iWaitingBinding=EFalse;	
		}
	err = iWaitingBinding? KErrNotFound : KErrNone;
		
	MCEMM_DEBUG("CMceSecureDesStream::BindCrypto(), Exit")			
	
	return err;	
   }
			

// -----------------------------------------------------------------------------
// CMceSecureDesStream::Base64Encode
// Base64 encoding.
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::Base64Encode( const TDesC8& aData, TDes8& aEncoded )
    {
    TBase64 encode;
    encode.Encode( aData, aEncoded );
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::Base64Decode
// Base64 decode.
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::Base64Decode( const TDesC8& aData, TDes8& aDecoded )
    {
    TBase64 decode;
    decode.Decode( aData, aDecoded );
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::ValidateSecurityDescriptions
// Check if security description is valid
// -----------------------------------------------------------------------------
//    
TBool CMceSecureDesStream::ValidateSecurityDescriptions( TDesC8& aData )
    {
    TBool valid( ETrue );
    
    TBuf8< KCryptoLineMaxLength > masterKeyAndSalt;
    Base64Decode( aData, masterKeyAndSalt );
    TInt countChar=0;
    // Check that encoded string is right sized
    // Validate length of master key and salt 
    if ( KEncodedStringLength != aData.Length() || 
         KMasterKeyAndSaltLength != masterKeyAndSalt.Length() )
        {
        return !valid ;
        }
          
    // Validate Base64 Encoding characters
    TBool charFound( EFalse );
    for ( TInt i = 0; i < aData.Length(); i++ )
        {
        
        for (TInt j = 0; j < KRandomStringCount; j++ )
            {
            if ( aData[i] == KBase64Chars[j] )
                {
                charFound = ETrue;
                countChar++;
                }
            }
        
        if ( !charFound )
            {
            // invalid character has been found
            valid = EFalse;
            }
            
        charFound = EFalse;
        }
    if (countChar!=aData.Length())
    	{
    	valid=EFalse; 
    	}
    return valid;
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::RandomString
// Generates random string
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::RandomString( TDes8& aRandom )
    {
    TTime time;
    time.UniversalTime();
    TInt64 seed = time.Int64();
    TUint32 random= Math::Random();
    seed += random;
    TInt randomNumber(0);
        
    for( TInt i = 0; i < aRandom.MaxSize(); i++ )
        {
        randomNumber = Math::Rand( seed ) % KRandomStringCount;
        aRandom.Append( KRandomChars[ randomNumber ] );
        }

    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::GenerateCryptoLineL()
// Cryptoline creation
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::GenerateCryptoLineL( 
    TDes8& aResult,
    TInt aCryptoCount,
    TBool aAnswer )
    {
    MCEMM_DEBUG("CMceSecureDesStream::GenerateCryptoLineL Entry")
    SetClientCryptoL( aCryptoCount );
    if ( aAnswer ) 
        {
        //Compare with crypto In and create cryptoOut
        MCEMM_DEBUG("GenerateCryptoLineL for Answer")
        if ( iSecureSession.iKeyNeedUpdated )
        	{
       		User::LeaveIfError ( CompareCryptosForAnswer() );
        	GenerateRandomKeys( iCryptoOut );
        	MCEMM_DEBUG("GenerateRandomKeys Key is updated")
        	iOldLocalMediaPort = iMediaStream.iLocalMediaPort;
        	}
        else
        	{
        	if(iOldLocalMediaPort != iMediaStream.iLocalMediaPort)
        		{
        	    GenerateRandomKeys( iCryptoOut );
        	    MCEMM_DEBUG("GenerateRandomKeys Key is updated when oldport is different from newport")
        	    iOldLocalMediaPort = iMediaStream.iLocalMediaPort;
        	    }
        	}
        aResult.AppendNum( iCryptoOut.iTag );
		aResult.Append( KSpace );
        GenerateCryptoSuiteLineL( aResult, iCryptoOut );
        MSG_IGNORE_RETURN()
      	if ( iSecureSession.iLSReadyToBind )
      		{
      		SetCryptoContextL();
      		}
        MSG_IGNORE_RETURN()
        }
	 else
    	{
    	MCEMM_DEBUG("GenerateCryptoLineL for Offer")
    	
    	if ( iSecureSession.iKeyNeedUpdated )
    		{
    		iGnoreSdpMsg = !iCryptoOuts->Count() ? ETrue : EFalse;
    		iGnoreSdpMsg = aCryptoCount - 1 >= iCryptoOuts->Count() ? ETrue : EFalse;
    		MSG_IGNORE_RETURN()
    		TMceSecureCryptoInfo crypto = iCryptoOuts->At( aCryptoCount-1 );
    		GenerateRandomKeys( crypto );
    		crypto.iTag = aCryptoCount;
    		aResult.AppendNum( aCryptoCount );
			aResult.Append( KSpace );
			GenerateCryptoSuiteLineL( aResult, crypto );
			MSG_IGNORE_RETURN()
			iCryptoOuts->At( aCryptoCount-1 ).Copy( crypto );
            iOldLocalMediaPort = iMediaStream.iLocalMediaPort;			
    		}
		else
			{
		    aResult.AppendNum( iCryptoOut.iTag );
			aResult.Append( KSpace );
        	GenerateCryptoSuiteLineL( aResult, iCryptoOut );
			}    		
    	
		}
	MCEMM_DEBUG("CMceSecureDesStream::GenerateCryptoLineL Exit")	
   	MSG_IGNORE_RETURN()	
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::GenerateCryptoSuiteLine
// Cryptoline creation
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::GenerateCryptoSuiteLineL( TDes8& aResult, 
										TMceSecureCryptoInfo& aCrypto )
    {
    switch ( aCrypto.iEncAlgms )
        {
        case ESrtpEncAES_CM:
            {
            if ( aCrypto.iTagLen == KAuthTagLength32 )
                {
                aResult.Append( KAES_SHA1_32 );
                }
            else
                {
                aResult.Append( KAES_SHA1_80 );
                }
            break;
            }
        default:
            {
            iGnoreSdpMsg = ETrue;
            break;
            }
        }
   
    MSG_IGNORE_RETURN()
    aResult.Append( KSpace );
    aResult.Append( KInline );
    if (aCrypto.iEncodedKey.Length() > 0)
    	{
    	aResult.Append( aCrypto.iEncodedKey );
    // Append MKI and lifetime if needed, MKI as default
    	if ( aCrypto.iMKIUsed && aCrypto.iMKI.Length() > 0 )
        	{
        	aResult.Append( KSeparator );
       	 	aResult.Append( aCrypto.iMKI );
        	}
        //set default masterkeyLT
        aCrypto.iMKLifeTime = KDefalutMaterKeysLifeTime;
    	}
    else
    	{
    	User::Leave( KErrArgument ); 	
    	}
    }


// -----------------------------------------------------------------------------
// CMceSecureDesStream::MediaField()
// Return this MediaField
// -----------------------------------------------------------------------------
//	
CSdpMediaField& CMceSecureDesStream::MediaField()
	{
	return *iMediaField;
	}

// -----------------------------------------------------------------------------
// CMceSecureDesStream::Codec()
// Return this related codec
// -----------------------------------------------------------------------------
//
CMceComMediaStream& CMceSecureDesStream::MediaStream() const
    {
    return iMediaStream;
    }
    

// -----------------------------------------------------------------------------
// CMceSecureDesStream::CompareContextId
// Return TBool
// -----------------------------------------------------------------------------
//    
TBool CMceSecureDesStream::CompareContextId( TUint32 aContextId )
	{
	TBool equal = EFalse;
	if (aContextId == iCryptoIn.iCryptoContextId ||
			aContextId == iCryptoOut.iCryptoContextId)
		{
		equal = ETrue;
		}
	return equal;	
	}	
	
// -----------------------------------------------------------------------------
// CMceSecureDesStream::DecodeMKLifeTime
// 
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::DecodeMKLifeTimeL(const TDesC8& aSecDec,
											TMceSecureCryptoInfo& aCrypto )
	{
	/*
	RFC 4568 6.1         
	*/
	// aSecDec is CSdpAttributeField value
	//By Type we know it is offer or answer so we know crypto In/Out should be
	// assign
	//Note that no matter how many inline the MKI will be same in the one cryptosute
	_LIT8( KMatchMKLTPowerTwo,"*|*^*|*:*" );
	_LIT8( KMatchMKLTDigit,"*|*|*:*" );
    _LIT8 ( KMatchMKLT, "*|2^*");
    _LIT8 ( KOnlyMKI, "*|*:*");
   
    TChar separator = '|';
    const TUint8 KMatchMKLToffset = 3;
    const TReal mkiPowerBase = 2;
    TBool found=EFalse;
	TInt totalLen = aSecDec.Length();
	TUint32 unitMKLT = 0;
	TPtrC8 mkLT( NULL,0 );
	TPtrC8 sec( NULL,0 );
	TReal powerResult = 0;
	TInt posSemi= aSecDec.Find( KSemiColon );
	sec.Set( aSecDec.Ptr(), aSecDec.Length() );
	if ( posSemi != KErrNotFound )
		{
		sec.Set(aSecDec.Left(posSemi).Ptr(), posSemi);
		}
	//sec is now only before semicolon
	// make sure if there is MKLT
	if ( sec.Match ( KMatchMKLTPowerTwo ) != KErrNotFound ) 
    	{
        
        TInt firstSep = SearchChar( sec, separator, 0 );
        TInt sndSep = SearchChar( sec, separator, firstSep );
    	mkLT.Set( sec.Mid( firstSep + 1, sndSep - firstSep -1 ) );
    	TInt posInvol = mkLT.Find( KInvolute );
		TLex8 lexMKL( mkLT.Mid( posInvol+1 ) );
		User::LeaveIfError( lexMKL.Val(unitMKLT, EDecimal ));
		Math::Pow( powerResult, mkiPowerBase, (TReal)unitMKLT );
		unitMKLT = (TUint32)powerResult;

		found = ETrue;	
        }
    else if (sec.Match ( KMatchMKLTDigit ) != KErrNotFound )
    	{
    	found= ETrue;
		//only Decimal case
		TInt first = sec.Find( KSeparator );
		TInt second = sec.LocateReverse( separator );
		mkLT.Set( aSecDec.Mid( first+1, second - first - 1  ));
		TLex8 lexMKL(mkLT);
		
		User::LeaveIfError( lexMKL.Val(unitMKLT, EDecimal ));
    	}
	else if( sec.Match( KMatchMKLT ) != KErrNotFound )	
		{
		found = ETrue;
		//onlyMKLT with power 2 no MasterKeyIdentifier
		TInt posSeparator = sec.Match( KMatchMKLT );
		TInt remainLen = sec.Length() - ( posSeparator + KMatchMKLToffset );
		//only number after power
		mkLT.Set( sec.Mid( posSeparator + KMatchMKLToffset, remainLen ) );
	
		TLex8 lexMKL( mkLT );
		
		User::LeaveIfError( lexMKL.Val(unitMKLT, EDecimal ));
		Math::Pow( powerResult, mkiPowerBase, (TReal)unitMKLT );
		mkLT.Set( sec.Mid( posSeparator + 1 ));// ex 2^32
		
		unitMKLT = (TUint32)powerResult;
		}
     else if ( sec.Match(KOnlyMKI) == KErrNotFound  && totalLen > 0
			&& sec.Find(KSeparator) != KErrNotFound )
		{
		found= ETrue;
		//only Decimal case
		TInt posfirst = sec.Find( KSeparator );
		TInt mkltpos = posfirst +1;
		TInt remainLen = totalLen - ( mkltpos );
		mkLT.Set( aSecDec.Mid( mkltpos, remainLen ));
		TLex8 lexMKL(mkLT);
		User::LeaveIfError( lexMKL.Val(unitMKLT, EDecimal ));
		}  
    
	if (found )
		{
		//It is decode so only for cryptoIn
		aCrypto.iMKLifeTime = unitMKLT;
		}
    }

// -----------------------------------------------------------------------------
// CMceSecureDesStream::DecodeMKIValue
// 
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::DecodeMKIValueL( 	const TDesC8& aSecDec,
											TBool aIsAnswer,
    										TMceSecureCryptoInfo& aCrypto )
	{
	/*RFC 4568 6.1
	*/
	// aSecDec is CSdpAttributeField value
	const TInt KMKIMaxLen = 3;
    const TInt KMKIColonLen = 1;
    const TInt KMKIMaxBytes = 128;
    _LIT8 (KOnlyMKI, "*|*:*");

	TPtrC8 sec(NULL,0);
	TChar separator='|';
	TChar colon=':';
	TChar space = ' ';
	TInt mkifound = aSecDec.Match( KOnlyMKI );
	TInt posSemi= aSecDec.Find( KSemiColon );
	sec.Set( aSecDec.Ptr(),aSecDec.Length() );
	if ( posSemi != KErrNotFound )
		{
		//with secon inline
		sec.Set( aSecDec.Left(posSemi).Ptr(), posSemi );
		}
		
	if ( aIsAnswer )
		{
		//compare with cryptoOut
		if ( !iCryptoOut.iMKIUsed && mkifound != KErrNotFound )
			{
			//should not have MKI field , reject this 
			User::Leave ( KErrArgument );
			}
		}
	if ( mkifound != KErrNotFound )
		{
		//there is MKI
		/*If the MKI length is not given or
   		its value exceeds 128 (bytes), then the entire crypto attribute MUST
   		be considered invalid.*/
		TInt separator2ndPos = sec.LocateReverse( separator );
		TInt mkiPos = separator2ndPos+1;
		TInt colonPos = sec.LocateReverse( colon );
		TInt mKIoffset = colonPos - mkiPos;
		//mki value
		if ( mKIoffset < 0 )
			{
			//malformate MKI
			User::Leave( KErrArgument );
			}
	
		TPtrC8 mkiValue = sec.Mid(mkiPos, mKIoffset);
		TLex8 lexMKI(mkiValue);
		User::LeaveIfError(lexMKI.Val(aCrypto.iMKIValue, EDecimal) );
		
		//mki length
		TInt mkilenPos = colonPos+1;
		TInt mkilenOffset = sec.Length() - mkilenPos;
		if ( mkilenOffset> KMKIMaxLen )
			{
			//there is extra session parameter behind
			//looking for white space
			TInt spacePos = sec.LocateReverse( space );
			mkilenOffset = spacePos-mkilenPos;
			}
		TPtrC8 mkilen = sec.Mid( mkilenPos, mkilenOffset );
		TLex8 lexMkiLen(mkilen);
		User::LeaveIfError(lexMkiLen.Val(aCrypto.iMKILength, EDecimal, KMKIMaxBytes));
    	__ASSERT_ALWAYS( aCrypto.iMKILength <= KMKIMaxLength,
    	                 User::Leave( KErrOverflow ) );		
		
		//get whole MKI ( value + length)-> This is for adding SDP only
		TInt total = mKIoffset +  mkilenOffset + KMKIColonLen;
		aCrypto.iMKI = sec.Mid( mkiPos, total ) ;		
		aCrypto.iMKIUsed = ETrue;
		}
	
	return;	
	}

// -----------------------------------------------------------------------------
// CMceSecureDesStream::ValidateAnswerByOffer
// Validate and Set cryptoIn information
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::ValidateAnswerByOfferL(const TDesC8& aSecDec )
	{
	/*
	Vaidata the incoming answer by the original offer
	7.1.3.  Processing of the Initial Answer - Unicast Streams
	*/
	//remember to check how many inlinle later
	MCEMM_DEBUG("CMceSecureDesStream::ValidateAnswerByOfferL, Entry")
    if (aSecDec.Length() < KCryptoAttributeValueMinLength)
 		{
 		User::Leave( KErrArgument );
 		}
    // Check SRTP crypto-suites
    if ( iSecureSession.iKeyNeedUpdated )
    	{
    	IfMatchLocalCryptoL(aSecDec);
    	}
    MSG_IGNORE_RETURN()
   	//check key param
   
	TInt foundInline = aSecDec.Find( KInline );
 	TInt foundSeparator = aSecDec.Find( KSeparator);
 	TInt foundColon = aSecDec.Find(KColon);
 	if ( KErrNotFound != foundInline)
 		{
    //check key info
	    TInt keyInfoPos = foundColon+1;
	    TInt keyInfoLen = 0;
	    if (foundSeparator != KErrNotFound)
	        	{
	        	keyInfoLen = foundSeparator-keyInfoPos;
	        	}
	        else
	        	{
	        	keyInfoLen = aSecDec.Length()-keyInfoPos;
	        	}
    
	    TPtrC8 keyInfo = aSecDec.Mid(keyInfoPos, keyInfoLen);
	    if ( iSecureSession.iKeyNeedUpdated )
	    	{
	    	
		    TBool valid = ValidateSecurityDescriptions( keyInfo );
	   		if ( valid )
	   			{
	   			
		    	// check keyInfo 
			    if (iCryptoOut.iEncodedKey.Compare( keyInfo ) == 0)
			    	{
			    	User::Leave( KErrArgument );
			    	}
		    	//check keyinfo mki 
				DecodeMKIValueL(aSecDec, ETrue, iCryptoIn );     
				MSG_IGNORE_RETURN()
				DecodeMKLifeTimeL( aSecDec, iCryptoIn );		
					//check sessionparam later
				iCryptoIn.iEncodedKey = keyInfo;
				StoreKeys(iCryptoIn.iEncodedKey);
	   			}
	   		else
	   			{
	   			User::Leave( KErrArgument );
	   			}
	    	}
	   	}
 	else
 		{
 		User::Leave( KErrArgument );
 		}
    MCEMM_DEBUG("CMceSecureDesStream::ValidateAnswerByOfferL, Exit")
    }
	
// -----------------------------------------------------------------------------
// CMceSecureDesStream::ValidateOfferByAnswer
// aSecDec is one attribute
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::ValidateOfferByAnswerL( const TDesC8& aSecDec )
	{
	/*
	RFC 4568               SDP Security Descriptions               
	7.1.2.  Generating the Initial Answer - Unicast Streams
	*/
	//check client abilities and same time set the Answer crypto but not keys
	MCEMM_DEBUG("CMceSecureDesStream::ValidateOfferByAnswerL, Entry")

 	if ( aSecDec.Length() < KCryptoAttributeValueMinLength )
 		{
 		User::Leave ( KErrArgument);
 		}
 	SetPreferedCryptoL();
 	TMceSecureCryptoInfo crypto;
    StoreCryptoInFromOfferL(aSecDec, crypto);
    MSG_IGNORE_RETURN()
	//SetClientCryptoL(crypto);	        
	   //check key param
     
    TInt foundInline = aSecDec.Find( KInline );
    TInt foundSeparator = aSecDec.Find( KSeparator);
    TInt foundColon = aSecDec.Find(KColon);
    if ( KErrNotFound != foundInline)
     	{
        //check key info
        TInt keyInfoPos=foundColon+1;
        TInt keyInfoLen=0;
        if (foundSeparator!= KErrNotFound)
        	{
        	keyInfoLen= foundSeparator-keyInfoPos;
        	}
        else
        	{
        	keyInfoLen = aSecDec.Length()-keyInfoPos;
        	}
        TPtrC8 keyInfo = aSecDec.Mid(keyInfoPos, keyInfoLen);
        TBool valid = ValidateSecurityDescriptions( keyInfo );
        
        if ( valid )
        	{
        	
            //check keyinfo mki 
		     DecodeMKIValueL( aSecDec, EFalse, crypto );   
		     MSG_IGNORE_RETURN()
		     //set MKI value for our local crypto because all MKI in one crypto suite
		     // should be the same
	     
	     	DecodeMKLifeTimeL( aSecDec, crypto );
	     	//StorKey
	     	crypto.iEncodedKey = keyInfo;
	     	TBuf8 < KMasterKeyAndSaltLength > masterKeyAndSalt;
			Base64Decode( crypto.iEncodedKey, masterKeyAndSalt );

			//it should be validate already so it is our prefered key
		
			crypto.iSetMasterKey = masterKeyAndSalt.Mid( 0, KMasterKeyLength );
			crypto.iSetSaltKey = masterKeyAndSalt.Mid( KMasterKeyLength );
			crypto.iKeysCreated = ETrue;
			iCryptoIns->AppendL( crypto );
	     	//check sessionparam later	
	     		
        	}
        else
        	{
        	User::Leave( KErrArgument);
        	}
     	}
   	else
   		{
 		User::Leave( KErrArgument);
 		}

    MCEMM_DEBUG("CMceSecureDesStream::ValidateOfferByAnswerL, Exit")
 	}
	
// -----------------------------------------------------------------------------
// CMceSecureDesStream::SetSecureProtocol()
// 
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::SetSecureProtocolL(CSdpMediaField& aMediaField)
	{
	MCEMM_DEBUG("CMceSecureDesStream::SetSecureProtocol(), Entry")   
	TPtrC8 protocolName( KProtocolSAVP );
	
	if ( Session().Modifier( KMceSecureSession ) == KMceSecurePlainAVP )
		{
		protocolName.Set ( KProtocolAVP );
		}
	if ( !iIsSAVP )
		{
		protocolName.Set( KProtocolSAVPF );
		Session().Modifier( KMceSecureSession ) = KMceSecureNormal;
		}
		
	RStringF protocol = SdpCodecStringPool::StringPoolL().OpenFStringL( protocolName);		
	CleanupClosePushL( protocol );                            
    
	aMediaField.SetProtocolL( protocol );	
	CleanupStack::PopAndDestroy( &protocol );

	MCEMM_DEBUG("CMceSecureDesStream::SetSecureProtocol(), Exit")  
	}
	

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetPreferedCrypto()
// 
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::SetPreferedCryptoL()
	{
	MCEMM_DEBUG( "CMceSecureDesStream::SetPreferedCryptoL, entry" )	
	RArray <TMceCryptoContext> clientCryptoSuite = 
					Session().iClientCryptoSuites;
	TInt clientCryptoCount = clientCryptoSuite.Count();
	if ( !iCryptoOuts->Count() )
		{
		AppendEmptyCryptoL( *iCryptoOuts, KTotalCryptoAnswerCount );
		}
	if (!clientCryptoCount )
		{
		MCEMM_DEBUG( "Client has no preference on crypto" )	
		iCryptoOuts->Reset();
		// tag 80 prefered
		SetDefaultCryptoL( *iCryptoOuts );
		}
	else
		{
		iCryptoOuts->Reset();
		SetCryptoByClientL( *iCryptoOuts );
		}
	MCEMM_DEBUG( "CMceSecureDesStream::SetPreferedCryptoL, exit" )		
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetClientCrypto()
// 
// -----------------------------------------------------------------------------
//    
void CMceSecureDesStream::SetClientCryptoL( TInt aCryptoCount )
	{
	MCEMM_DEBUG( "CMceSecureDesStream::SetClientCryptoL, entry" )	
	RArray< TMceCryptoContext > clientCryptoSuite = 
	    Session().iClientCryptoSuites;
	TInt clientCryptoSuiteCount = clientCryptoSuite.Count();
	
	if ( iCryptoOuts->Count() == 0 )
		{
		AppendEmptyCryptoL( *iCryptoOuts, KTotalCryptoAnswerCount );
		}
		
	if ( clientCryptoSuiteCount > 0 )
		{	
		for ( TInt index = 0; index < clientCryptoSuiteCount; index++ )
			{
			if ( aCryptoCount-1 == index )
				{
				MCEMM_DEBUG_DVALUE(" index = ", index )
				MCEMM_DEBUG_DVALUE(" iCryptoOuts->Count() ", iCryptoOuts->Count() )	
				iGnoreSdpMsg = index >= iCryptoOuts->Count() ? ETrue : EFalse;
				MSG_IGNORE_RETURN()
				TMceSecureCryptoInfo cryptoOut = iCryptoOuts->At( index );
				
				if ( clientCryptoSuite[index]== EAES_CM_128_HMAC_SHA1_80 )
					{
					SetSHA180( cryptoOut, aCryptoCount );
					}
				if ( clientCryptoSuite[ index ] == EAES_CM_128_HMAC_SHA1_32 )
					{
					//iCrytoOuts has been created in GenerateKey
					SetSHA132( cryptoOut, aCryptoCount );
					}	
				iCryptoOuts->At( index ).Copy( cryptoOut );	
				}
			}
		}
	MCEMM_DEBUG( "CMceSecureDesStream::SetClientCryptoL, exit" )
	}	


//-----------------------------------------------------------------------------
// CMceSecureDesStream::ClientCrytoCount()
// 
// -----------------------------------------------------------------------------
//    
TInt CMceSecureDesStream::ClientCrytoCount()
	{
	TInt count = Session().iClientCryptoSuites.Count();
	return count;
	}		


//-----------------------------------------------------------------------------
// CMceSecureDesStream::IfMatchLocalCryptoL()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::IfMatchLocalCryptoL( const TDesC8& aSecDec )
	{
	MCEMM_DEBUG( "CMceSecureDesStream::IfMatchLocalCryptoL, entry" )
	TMceSecureCryptoInfo crypto;
	TInt foundAes80 = aSecDec.Find( KAES_SHA1_80 );
	TInt foundAes32 = aSecDec.Find( KAES_SHA1_32 );
	if (foundAes80 != KErrNotFound )
		{
		TPtrC8 tagValue = aSecDec.Mid( foundAes80-2, 1);
		TLex8 lexTag(tagValue);
		TUint tag=0;
		User::LeaveIfError( lexTag.Val( tag, EDecimal));
		SetSHA180( crypto, tag );
		}
	else if (foundAes32!= KErrNotFound )
		{
		TPtrC8 tagValue = aSecDec.Mid( foundAes32-2, 1 );
		TLex8 lexTag( tagValue );
		TUint tag = 0;
		User::LeaveIfError( lexTag.Val( tag, EDecimal ));
		SetSHA132( crypto, tag );
		}	
	TInt find = SearchAndSetCrypto( crypto );
	iGnoreSdpMsg = find == KErrNone ? EFalse : ETrue;
	MCEMM_DEBUG( "CMceSecureDesStream::IfMatchLocalCryptoL, exit" )
	return ;
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SearchAndSetCrypto()
// 
// -----------------------------------------------------------------------------
//
TInt CMceSecureDesStream::SearchAndSetCrypto(TMceSecureCryptoInfo& aCrypto)
	{
	MCEMM_DEBUG( "CMceSecureDesStream::SearchAndSetCrypto, entry" )
	for ( TInt i=0; i < iCryptoOuts->Count(); i++ )
		{
		MCEMM_DEBUG_DVALUE(" iCryptoOuts->Count() ", iCryptoOuts->Count() )	
				
		TMceSecureCryptoInfo cryptoOut = iCryptoOuts->At( i );
		if ( cryptoOut.iTagLen == aCrypto.iTagLen &&
			 cryptoOut.iEncAlgms == aCrypto.iEncAlgms &&
			 cryptoOut.iAuthAlgms == aCrypto.iAuthAlgms &&
			 cryptoOut.iCryptoSuite.Compare(aCrypto.iCryptoSuite) == 0)
			{
			//SEtCrypto
			iCryptoOut.Copy( cryptoOut );
			iCryptoIn.iTag = cryptoOut.iTag;
			iCryptoIn.iEncAlgms = cryptoOut.iEncAlgms;
			iCryptoIn.iAuthAlgms = cryptoOut.iAuthAlgms;
			iCryptoIn.iTagLen = cryptoOut.iTagLen;
			iCryptoIn.iCryptoSuite = cryptoOut.iCryptoSuite ;
        	return KErrNone;
			}
		}
	MCEMM_DEBUG( "SearchAndSetCrypto NotFound" )	
	MCEMM_DEBUG( "CMceSecureDesStream::SearchAndSetCrypto, exit" )	
	return KErrNotFound;	
	}	
	
	
//-----------------------------------------------------------------------------
// CMceSecureDesStream::StoreCryptoInFromOffer()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::StoreCryptoInFromOfferL( const TDesC8& aSecDec, 
                                                   TMceSecureCryptoInfo& aCrypto)
	{
	MCEMM_DEBUG( "CMceSecureDesStream::StoreCryptoInFromOfferL entry" )
	TInt foundAes80 = aSecDec.Find( KAES_SHA1_80 );
	TInt foundAes32 = aSecDec.Find( KAES_SHA1_32 );
	if ( foundAes80 != KErrNotFound )
		{
		TPtrC8 tagValue = aSecDec.Mid( foundAes80-2, 1 );
		TLex8 lexTag( tagValue );
		TUint tag = 0;
		User::LeaveIfError( lexTag.Val( tag, EDecimal ));
		SetSHA180( aCrypto, tag );
		}
	else if (foundAes32!= KErrNotFound )
		{
		TPtrC8 tagValue = aSecDec.Mid( foundAes32-2, 1 );
		TLex8 lexTag( tagValue );
		TUint tag = 0;
		User::LeaveIfError( lexTag.Val( tag, EDecimal ));
		SetSHA132( aCrypto, tag );
		}
			
	if (foundAes32 == KErrNotFound && foundAes80 == KErrNotFound)
		{
		iGnoreSdpMsg = ETrue;
		MCEMM_DEBUG( " Not Found any acceptible Crypto Value in SecureDes" )
		return;
		} 
	
	for ( TInt i=0; i < iCryptoOuts->Count(); i++ )
		{
		MCEMM_DEBUG_DVALUE(" iCryptoOuts->Count() ", iCryptoOuts->Count() )	
				
		TMceSecureCryptoInfo cryptoOut = iCryptoOuts->At( i );
		if ( cryptoOut.iTagLen == aCrypto.iTagLen &&
			 cryptoOut.iEncAlgms == aCrypto.iEncAlgms &&
			 cryptoOut.iAuthAlgms == aCrypto.iAuthAlgms &&
			 cryptoOut.iCryptoSuite.Compare(aCrypto.iCryptoSuite) == 0 )
			{
			MCEMM_DEBUG( " Compare with own crypto OK" )
			return;
			}
		}
	iGnoreSdpMsg = ETrue;
	MCEMM_DEBUG( "CMceSecureDesStream::StoreCryptoInFromOfferL exit" )	
	}	

	
//-----------------------------------------------------------------------------
// CMceSecureDesStream::StoreCryptoInFromOffer()
// 
// -----------------------------------------------------------------------------
//
TInt CMceSecureDesStream::CompareCryptosForAnswer()
	{
	//check how client wants
	MCEMM_DEBUG( "CMceSecureDesStream::CompareCryptosForAnswer entry" )
	if (iCryptoOuts->Count())
		{
		MCEMM_DEBUG_DVALUE(" iCryptoOuts->Count() ", iCryptoOuts->Count() )	
				
		TInt count = iCryptoOuts->Count();
		for ( TInt i = 0; i< count; i++ )
			{
			for ( TInt j = 0; j < iCryptoIns->Count(); j++ )
				{	
				MCEMM_DEBUG_DVALUE(" iCryptoIns->Count() ", iCryptoIns->Count() )
				if (iCryptoOuts->At( i ).iTagLen == iCryptoIns->At( j ).iTagLen &&
					iCryptoOuts->At( i ).iEncAlgms == iCryptoIns->At( j ).iEncAlgms &&
					iCryptoOuts->At( i ).iAuthAlgms == iCryptoIns->At( j ).iAuthAlgms &&
					iCryptoOuts->At( i ).iCryptoSuite.Compare( iCryptoIns->At( j ).iCryptoSuite) == 0 )
					{
					iCryptoIn.Copy( iCryptoIns->At( j ) );
					//So can set the MKI and MKT
					iCryptoOut.Copy( iCryptoIns->At( j ) );
					MCEMM_DEBUG( "CMceSecureDesStream::CompareCryptosForAnswer exit" )
					return KErrNone;
					}
				}
			}
		}
	MCEMM_DEBUG( "No CryptoOuts Set" )
	MCEMM_DEBUG( "CMceSecureDesStream::CompareCryptosForAnswer exit" )
	return KErrArgument;	
	}
	
//-----------------------------------------------------------------------------
// CMceSecureDesStream::RemoveClientCrypto()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::RemoveClientCrypto()
	{
	TInt count = ClientCrytoCount();
    for (TInt i = count; i > 0; i--)
    	{
    	Session().iClientCryptoSuites.Remove( i-1 );	
   		}	
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetClientCrypto()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetClientCryptoL( TMceSecureCryptoInfo& aCrypto )
	{
    if ( !aCrypto.iCryptoSuite.Compare( KAES_SHA1_32 ))
    	{
    	User::LeaveIfError( 
    		Session().iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_32 ));		
    	}
    if ( !aCrypto.iCryptoSuite.Compare(KAES_SHA1_80) )
    	{
    	User::LeaveIfError( 
    	    Session().iClientCryptoSuites.Append( EAES_CM_128_HMAC_SHA1_80 ));		
    	}
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetMultipleClientCrypto()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetMultipleClientCryptoL( 
    CArrayFixFlat< TMceSecureCryptoInfo>& aArray )
	{
	TInt index =0;
	TInt count = aArray.Count();
	for ( index = 0; index < count; index ++ )
		{
		SetClientCryptoL( aArray[index] );
		}
    
	}
//-----------------------------------------------------------------------------
// CMceSecureDesStream::CountCrypto()
// 
// -----------------------------------------------------------------------------
//
TInt CMceSecureDesStream::CountCryptoInOffer( CSdpMediaField& aMediaField )
	{
	
    if ( !iCryptoIns->Count() && !ClientCrytoCount()  && 
    		iSecureSession.SdpCryptoAttributeCount(aMediaField) )
    	{
    	return KErrArgument;
    	}
    return KErrNone;	
    }				

//-----------------------------------------------------------------------------
// CMceSecureDesStream::Copy()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::CopyStreamCryptoL( CMceSecureDesStream& aCopyFrom ) 
	{
	MCEMM_DEBUG( "CMceSecureDesStream::CopyStreamCryptoL Entry" )
	iCryptoUpdateNeeded = ETrue;
    iCryptoOut.Copy(aCopyFrom.iCryptoOut);
    MCEMM_DEBUG_DVALUE( "iCryptoContextOutId", aCopyFrom.iCryptoOut.iCryptoContextId )
    iCryptoContextOutId = aCopyFrom.iCryptoOut.iCryptoContextId;
    iCryptoIn.Copy(aCopyFrom.iCryptoIn);
    iCryptoContextInId = aCopyFrom.iCryptoIn.iCryptoContextId;
    MCEMM_DEBUG_DVALUE( "iCryptoContextInId", aCopyFrom.iCryptoIn.iCryptoContextId )

	for (TInt i = 0; i < aCopyFrom.iCryptoOuts->Count(); i++)
		{
		iCryptoOuts->AppendL( aCopyFrom.iCryptoOuts->At( i ) );
		}
		iOldLocalMediaPort = aCopyFrom.iOldLocalMediaPort;
 	MCEMM_DEBUG( "CMceSecureDesStream::CopyStreamCryptoL Exit" )
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::Session()
// 
// -----------------------------------------------------------------------------
//
CMceComSession& CMceSecureDesStream::Session()
	{
	return iSecureSession.iSession;
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::MediaFieldAttrMatch()
// 
// -----------------------------------------------------------------------------
//
CSdpAttributeField* CMceSecureDesStream::MediaFieldAttrMatch( 	CSdpMediaField& aMediaField,
																const TDesC8& aMatchString )
	{
	RPointerArray< CSdpAttributeField > attrList =  
                aMediaField.AttributeFields();
    
    TInt attrCount = attrList.Count();
    
    for (TInt j = 0; j < attrCount; j++ )
        {
        RStringF attribute = attrList[ j ]->Attribute();
     
     	 if ( attribute.DesC().Match( aMatchString )!= KErrNotFound )
        	{
        	return attrList[ j ];
        	}
        }
    return NULL;    
	}
	
//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetMediaProfile ()
// 
// -----------------------------------------------------------------------------
//
TInt CMceSecureDesStream::SetMediaProfile( CSdpMediaField& aMediaField )
	{
	//answer or offer/update or refresh
	TInt findSAVP = aMediaField.Protocol().DesC().Find( KProtocolSAVP );
	TInt findSAVPF = aMediaField.Protocol().DesC().Find( KProtocolSAVPF );	
    TInt findAVP = aMediaField.Protocol().DesC().Find( KProtocolAVP );	
    
    if ( findSAVP == KErrNotFound && findSAVPF == KErrNotFound &&
		findAVP == KErrNotFound)
		{
		return KErrArgument;
		}
	if ( findAVP != KErrNotFound )
		{
        Session().Modifier( KMceSecureSession ) = KMceSecurePlainAVP;
		}
	if ( findSAVP != KErrNotFound || findSAVPF != KErrNotFound )
		{
		Session().Modifier( KMceSecureSession ) = KMceSecureNormal;
		iIsSAVP = findSAVPF != KErrNotFound ? !iIsSAVP : iIsSAVP;	
		}
	
	
	return KErrNone;
	}


//-----------------------------------------------------------------------------
// CMceSecureDesStream::AppendCryptoAttributeL ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::AppendCryptoAttributeL( 	TDesC8& aCryptoLine,
													CSdpMediaField& aMediaField )
	{
	RStringF crypto = SdpCodecStringPool::StringPoolL().OpenFStringL( KCrypto );
    CleanupClosePushL( crypto );               
	CSdpAttributeField* attribute = NULL;
	attribute = CSdpAttributeField::NewLC( crypto, aCryptoLine );
   	User::LeaveIfError ( aMediaField.AttributeFields().Append( attribute ) );
    CleanupStack::Pop(attribute);	   
    CleanupStack::PopAndDestroy( &crypto ); 
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::AppendCryptoAttributeL ()
// 
// -----------------------------------------------------------------------------
//
TInt CMceSecureDesStream::SearchChar( 	TDesC8& aSearchLine, 
										TChar& aChar, 
										TInt aSearchFrom )
	{
	TInt len = aSearchLine.Length();
	TLex8 search( aSearchLine );

	for ( TInt offset = 0; offset < len; offset++ )
		{
		if ( search.Peek() == aChar )
			{
			if ( !aSearchFrom )
				return offset;
			if (aSearchFrom && offset > aSearchFrom)
				return offset;
			}
		search.Inc();	
		}
	return KErrNone;
	}	

//-----------------------------------------------------------------------------
// CMceSecureDesStream::AppendEmptyCryptoL ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::AppendEmptyCryptoL( 
							CArrayFixFlat<TMceSecureCryptoInfo>& aCryptos,
							TInt aCount )
	{
	for ( TInt i = 0; i < aCount; i++)
		{
		TMceSecureCryptoInfo crypto;
		aCryptos.AppendL( crypto );
		}
	}

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetSHA132 ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetSHA132( TMceSecureCryptoInfo& aCrypto, TInt aTag )
	{
	aCrypto.iTag = aTag;
	aCrypto.iEncAlgms = ESrtpEncAES_CM;
    aCrypto.iTagLen = KAuthTagLength32;
    aCrypto.iAuthAlgms = ESrtpAuthHMAC_SHA1;
	aCrypto.iCryptoSuite = KAES_SHA1_32;
	}	

//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetSHA180 ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetSHA180( TMceSecureCryptoInfo& aCrypto, TInt aTag )
	{
	aCrypto.iTag = aTag;
	aCrypto.iEncAlgms = ESrtpEncAES_CM;
    aCrypto.iTagLen = KAuthTagLength80;
    aCrypto.iAuthAlgms = ESrtpAuthHMAC_SHA1;
	aCrypto.iCryptoSuite = KAES_SHA1_80;
   	}
   	
//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetDefaultCryptoL ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetDefaultCryptoL( 
							CArrayFixFlat<TMceSecureCryptoInfo>& aCryptos )
	{
	MCEMM_DEBUG( "CMceSecureDesStream::SetDefaultCryptoL, entry " )
	const TInt tag1 = 1;
	const TInt tag2 = 2;
	const TUint index1 = 0;
	const TInt index2 = 1;
	
	AppendEmptyCryptoL( aCryptos, KTotalCryptoAnswerCount );
	
	MCEMM_DEBUG_DVALUE(" CryptoCount = ", aCryptos.Count() )
	iGnoreSdpMsg = !aCryptos.Count() ? ETrue : EFalse;
	MSG_IGNORE_RETURN()
    SetSHA180( aCryptos.At( index1 ), tag1 );
    
    iGnoreSdpMsg = index2 >= iCryptoOuts->Count() ? ETrue : EFalse;
    MSG_IGNORE_RETURN()
    SetSHA132( aCryptos.At( index2 ), tag2 );
    
	MCEMM_DEBUG( "CMceSecureDesStream::SetDefaultCryptoL, exit " )
	}   
	
//-----------------------------------------------------------------------------
// CMceSecureDesStream::SetCryptoByClientL ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::SetCryptoByClientL( 
							CArrayFixFlat<TMceSecureCryptoInfo>& aCryptos )
	{
	MCEMM_DEBUG( "CMceSecureDesStream::SetCryptoByClientL, entry " )
	RArray <TMceCryptoContext> clientCryptoSuite =  
											Session().iClientCryptoSuites;
	TInt clientCryptoCount = clientCryptoSuite.Count();
	MCEMM_DEBUG_DVALUE(" ClientCryptoCount = ", aCryptos.Count() )											
	for ( TInt i = 0; i < clientCryptoCount; i++ )
			{
			TMceSecureCryptoInfo crypto;
			
			if ( clientCryptoSuite[i] == EAES_CM_128_HMAC_SHA1_32 )
				{
				SetSHA132( crypto, i+1 );
				}
			if ( clientCryptoSuite[i] == EAES_CM_128_HMAC_SHA1_80 )
				{
				SetSHA180( crypto, i+1 );
				}
			aCryptos.AppendL( crypto );		
			}
	MCEMM_DEBUG( "CMceSecureDesStream::SetCryptoByClientL, exit " )
	}   							
//-----------------------------------------------------------------------------
// CMceSecureDesStream::ForceUpdateStreamL ()
// 
// -----------------------------------------------------------------------------
//
void CMceSecureDesStream::ForceUpdateStreamL()
	{
	MCEMM_DEBUG( "CMceSecureDesStream::ForceUpdateStream, entry " )
	UpdateCryptoContextL( iCryptoIn ); 
    UpdateCryptoContextL( iCryptoOut ); 			
    if ( iWaitingBinding )
    	{
    	iSecureSession.BindStreamCrypto();
    	}
    MCEMM_DEBUG( "CMceSecureDesStream::ForceUpdateStream, exit " )
	}
//  End of File  

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
* Description:    Class is used for encapsulating crypto context and
*                destination information in one class.
*
*/



#ifndef MMCCCRYPTOCONTEXT_H
#define MMCCCRYPTOCONTEXT_H

#include <mmf/common/mmfcontrollerframework.h>
#include <e32base.h>

#include "mmccsrtpmasterkey.h"
#include "mmccsrtpmastersalt.h"
#include "mmccsrtpcryptoparams.h"

const TUint32 KTUintNumValue0 = 0;

/**.
 * This class encapsulates crypto context and destiantion
 * related information.
 *
 *
 * @lib mmccinterface.dll
 * @since S60 3.2
 */

class TMccCryptoContext
    { 
public:

    /**
     * Default constructor
     */
    inline TMccCryptoContext() :
        iContextId( KTUintNumValue0 ),
        iSessionId( KTUintNumValue0 ),
        iLinkId( KTUintNumValue0 ),
        iStreamId( KTUintNumValue0 ),
        iEndPointId( KTUintNumValue0 ),
        iSrtpMasterKey( TMccSrtpMasterKey( KNullDesC8, KNullDesC8 ) ),
        iSrtpMasterSalt( TMccSrtpMasterSalt( KNullDesC8 ) ),
        iSrtpCryptoParams( TMccSrtpCryptoParams() ),
        iBindingDone( EFalse )
        {        
        };
            
    /**
     * Parametric constructor
     */
    inline TMccCryptoContext( TUint32 aContextId,
                              const TMccSrtpMasterKey& aSrtpMasterKey,
                              const TMccSrtpMasterSalt& aSrtpMasterSalt,
                              const TMccSrtpCryptoParams& aSrtpCryptoParams ) :
        iContextId( aContextId ),
        iSessionId( KTUintNumValue0 ),
        iLinkId( KTUintNumValue0 ),
        iStreamId( KTUintNumValue0 ),
        iEndPointId( KTUintNumValue0 ),
        iSrtpMasterKey( aSrtpMasterKey ),
        iSrtpMasterSalt( aSrtpMasterSalt ),
        iSrtpCryptoParams( aSrtpCryptoParams ),
        iBindingDone( EFalse )
        {        
        };
     
    /**
     * Checks the validity of the whole crypto context.
     * @since Series 60 3.2
     * @return Error code
     */
    TInt ValidateContext() const;
    
    /**
     * Updates the crypto context by the given crypto context.
     * @since Series 60 3.2
     * @return Error code
     */
    TInt UpdateContext( TMccCryptoContext& aCandidate );
    
    /**
     * Gets the destination associated with this crypto context
     * @since Series 60 3.2
     * @param aSessionId session where context belongs to
     * @param aLinkId link ID
     * @param aStreamId ID identifying stream
     * @param aEndpointId ID identifying endpoint of the stream
     * @return Error code
     */
    TInt GetDestinationInfo( TUint32& aSessionId, TUint32& aLinkId,
					         TUint32& aStreamId, TUint32& aEndPointId ) const;
    
    /**
     * Sets the destination associated with this crypto context
     * @since Series 60 3.2
     * @param aSessionId session ID where context belongs to
     * @param aLinkId link ID
     * @param aStreamId ID identifying stream
     * @param aEndpointId ID identifying endpoint of the stream
     * @return Error code
     */
    TInt SetDestinationInfo( TUint32 aSessionId, TUint32 aLinkId,
					         TUint32 aStreamId, TUint32 aEndPoint );
	
	/**
     * Sets the master key.
     * @since Series 60 3.2
     * @param TMccSrtpMasterKey Master key.
     * @return void.
     */				         
    void SetMasterKey( const TMccSrtpMasterKey& aSrtpMasterKey );	
	
	/**
     * Sets the master salt.
     * @since Series 60 3.2
     * @param TMccSrtpMasterSalt Master salt.
     * @return void.
     */				         
    void SetMasterSalt( const TMccSrtpMasterSalt& aSrtpMasterSalt );
    
    /**
     * Sets the crypto params.
     * @since Series 60 3.2
     * @param TMccSrtpCryptoParams Crypto params.
     * @return void.
     */				         
    void SetCryptoParams( const TMccSrtpCryptoParams& aSrtpCryptoParams );
    
	/**
     * Gets the master key.
     * @since Series 60 3.2
     * @return Master key.
     */				         
    const TMccSrtpMasterKey& MasterKey() const;					         

    /**
     * Gets the master salt.
     * @since Series 60 3.2
     * @return Master salt.
     */	
    const TMccSrtpMasterSalt& MasterSalt() const;
    
    /**
     * Gets the crypto params.
     * @since Series 60 3.2
     * @return Crypto params.
     */	  
    const TMccSrtpCryptoParams& CryptoParams() const;

    /**
     * Gets the context id associated with this crypto context
     * @since Series 60 3.2
     * @return Context id
     */
    TUint32 ContextId() const;    

private:

    /**
     * MCC SRTP master key
     */
    TMccSrtpMasterKey iSrtpMasterKey;
  
    /**
     * MCC SRTP master salt
     */
    TMccSrtpMasterSalt iSrtpMasterSalt;
            
    /**
     * MCC SRTP crypto params
     */
    TMccSrtpCryptoParams iSrtpCryptoParams;
            
    /**
     * context ID
     */
    TUint32 iContextId;
    
    /**
     * ID of session.
     */
    TUint32 iSessionId;
    
    /**
     * ID of link.
     */
    TUint32 iLinkId;
    
    /**
     * ID of stream.
     */
    TUint32 iStreamId;
    
    /**
     * ID identifying endpoint of the stream
     */
    TUint32 iEndPointId;
            
    /**
     * Indicates is crypto context binded into the stream
     */
    TBool iBindingDone;
        
    };
    
typedef TPckgBuf<TMccCryptoContext> TMccCryptoContextPckg;


#endif // MMCCCRYPTOCONTEXT_H

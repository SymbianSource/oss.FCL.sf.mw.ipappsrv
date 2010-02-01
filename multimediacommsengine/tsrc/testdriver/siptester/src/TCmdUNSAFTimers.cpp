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
* Description:  Implementation
*
*/


#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include "CTcSIPContext.h"
#include "TCmdUNSAFTimers.h"
#include "SIPConstants.h"

//NOTE: This value must be same as in CUNSAFCRSettings.h
const TInt KMaxServerNameLen = 256;
	       		   
TInt TCmdUNSAFTimers::ReadTimerValuesL( CRepository& aRepository,
									    TUint32& aKey,
									    TInt& aBindingRequestInterval,
									    TInt& aUdpKeepAliveInterval,
									    TInt& aTcpKeepAliveInterval ) const
	{
	TPtrC8 iapName = ExtractTextL( KParamIAPName, EFalse );
	if( iapName != KNullDesC8 )
		{
		return ReadIapTimersL( aRepository,
							   // Leaves if not found
							   iContext.IAPIdL( iapName ),
							   aKey,
							   aBindingRequestInterval,
							   aUdpKeepAliveInterval,
							   aTcpKeepAliveInterval );
		}

	// Since IAP was not given, domain is mandatory	
	TPtrC8 domainName = ExtractTextL( KParamDomainName, ETrue );
	return ReadDomainTimersL( aRepository,									 
							  domainName,
							  aKey,
							  aUdpKeepAliveInterval,
							  aTcpKeepAliveInterval );	
	}

TInt TCmdUNSAFTimers::ReadIapTimersL( CRepository& aRepository,
									  TInt aIapId,
									  TUint32& aKey,
									  TInt& aBindingRequestInterval,
									  TInt& aUdpKeepAliveInterval,
									  TInt& aTcpKeepAliveInterval ) const
    {
    TInt status = FindIapKeyL( aRepository, aIapId, aKey );
    if ( status == KErrNone )
    	{
    	// It doesn't matter if some parameter doesn't exist
        Read( aRepository,
        	  KUNSAFProtocolsIntervalUDPMask | aKey,
              aUdpKeepAliveInterval );
        Read( aRepository,
        	  KUNSAFProtocolsIntervalTCPMask | aKey,
              aTcpKeepAliveInterval );
        Read( aRepository,
        	  KUNSAFProtocolsSTUNRetransmitTimerMask | aKey,
              aBindingRequestInterval );
    	}
    return status;
    }

TInt TCmdUNSAFTimers::FindIapKeyL( CRepository& aRepository,
								   TInt aIapId,
								   TUint32& aKey ) const
    {
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = aRepository.FindL( KUNSAFProtocolsIAPIdMask,
                                  KUNSAFProtocolsFieldTypeMask,
                                  keys );
	TInt ret = KErrNotFound;
    if ( err == KErrNone && keys.Count() > 0 )
        {
        for ( TInt i = 0; i < keys.Count() && ret == KErrNotFound; i++ )
            {
            TUint32 key = ( KUNSAFProtocolsIAPIdMask ^ ( keys[ i ] ) );
            key |= KUNSAFProtocolsIAPTableMask;
 
  			TInt iapId( 0 );
            Read( aRepository, KUNSAFProtocolsIAPIdMask | key, iapId );

            if ( iapId == aIapId )
            	{
            	aKey = key;	            
	            ret = KErrNone;
            	}
            }
        }

    CleanupStack::PopAndDestroy( &keys );
    return ret;
    }

TInt TCmdUNSAFTimers::ReadDomainTimersL( CRepository& aRepository,							
										 TPtrC8 aDomain,
										 TUint32& aKey,
										 TInt& aUdpKeepAliveInterval,
										 TInt& aTcpKeepAliveInterval ) const
    {
    TInt status = FindDomainKeyL( aRepository, aDomain, aKey );
    if ( status == KErrNone )
    	{
    	Read( aRepository,
    	  	  KUNSAFProtocolsDomainIntervalUDPMask | aKey,
          	  aUdpKeepAliveInterval );
		Read( aRepository,
    	  	  KUNSAFProtocolsDomainIntervalTCPMask | aKey,
          	  aTcpKeepAliveInterval );
    	}
    return status;
    }

TInt TCmdUNSAFTimers::FindDomainKeyL( CRepository& aRepository,							
									  TPtrC8 aDomain,
									  TUint32& aKey ) const
    {
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = aRepository.FindL( KUNSAFProtocolsDomainMask,
                                  KUNSAFProtocolsFieldTypeMask,
                                  keys );
	TInt ret = KErrNotFound;
    if ( err == KErrNone && keys.Count() > 0 )
        {
        for ( TInt i = 0; i < keys.Count() && ret == KErrNotFound; i++ )
            {
            TUint32 key = ( KUNSAFProtocolsDomainMask ^ ( keys[ i ] ) );
            key |= KUNSAFProtocolsDomainTableMask;

            TBuf8<KMaxServerNameLen> domain;
            if ( ( Read( aRepository,
            		     KUNSAFProtocolsDomainMask | key,
                         domain ) == KErrNone ) &&
                 ( domain == aDomain ) )
        		{
        		aKey = key;     
                ret = KErrNone;
        		}
            }
        }

    CleanupStack::PopAndDestroy( &keys );
    return ret;
    }

void TCmdUNSAFTimers::Read( CRepository& aRepository,
						    const TUint32& aKey,
							TInt& aKeyValue ) const
    {
    TInt tmp( 0 );
    if ( aRepository.Get( aKey, tmp ) == KErrNone )
        {
        aKeyValue = tmp;     
        }    
    }

TInt TCmdUNSAFTimers::Read( CRepository& aRepository,
							const TUint32& aKey,
							TDes8& aKeyValue ) const
    {
    TBuf8<1> tmp;
    TInt actualLength = 0;
    // This Get call returns KErrOverflow, but it doesn't matter as we are only
    // interested in actualLength.
    aRepository.Get( aKey, tmp, actualLength );

	if ( actualLength <= 0 || actualLength > aKeyValue.MaxLength() )
		{
		return KErrOverflow;
		}

    return aRepository.Get( aKey, aKeyValue );
    }

// Allow writing value zero, so that STUN client's behaviour can be tested also
// in such case. If aKeyValue is KErrNotFound, then application didn't provide
// it, and it is not written.
void TCmdUNSAFTimers::WriteL( CRepository& aRepository,
						      const TUint32& aKey,
						      TInt aKeyValue ) const
    {
    if ( aKeyValue != KErrNotFound )
    	{
    	User::LeaveIfError( aRepository.Set( aKey, aKeyValue ) );
    	}    
    }

void TCmdUNSAFTimers::WriteL( CRepository& aRepository,
						      const TUint32& aKey,
						      const TDesC8& aKeyValue ) const
    {
    if ( aKeyValue != KNullDesC8 )
    	{
    	User::LeaveIfError( aRepository.Set( aKey, aKeyValue ) );
    	}    
    }

TUint32 TCmdUNSAFTimers::CreateNewKeyL( CRepository& aRepository,
										TUint32 aKey,
										TUint32 aTable ) const
    {
    const TUint32 KInitialKey = 1;
	TUint32 newKey = KInitialKey;
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = aRepository.FindL( aKey,
                                  KUNSAFProtocolsFieldTypeMask,
                                  keys );
    if ( err != KErrNotFound )
        {
        User::LeaveIfError( err );
        TInt keyCount = keys.Count();
        if ( keyCount != 0 )
            {
            // Find the biggest key and increment it by one
            keys.SortUnsigned();
            TUint32 maxKey = aKey ^ keys[ keyCount - 1 ];
            newKey = maxKey + 1;
            }
        }

    newKey |= aTable;
    CleanupStack::PopAndDestroy( &keys );
    return newKey;
    }

void TCmdUNSAFTimers::RemoveParameterL( CRepository& aRepository,
										TUint32 aKey ) const
	{
	const TUint32 KSingleKeyMask = 0xffffffff;
	TUint32 dummy( 0 );
	TInt status = aRepository.Delete( aKey, KSingleKeyMask, dummy );
	__ASSERT_ALWAYS( status == KErrNone || status == KErrNotFound,
					 User::Leave( status ) );	
	}

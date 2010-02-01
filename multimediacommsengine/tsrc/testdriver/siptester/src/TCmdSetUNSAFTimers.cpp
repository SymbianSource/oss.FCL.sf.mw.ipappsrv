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
#include "TCmdSetUNSAFTimers.h"
#include "SIPConstants.h"

//This value must be same as declared in CUNSAFCRSettings.h
const TInt KMaxServerNameLen = 256;

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	KParamIAPName,
 *	 				KParamDomainName,
 *					KParamUNSAFBindingRequestInterval,
 *					KParamUNSAFUDPKeepaliveInterval
 *					KParamUNSAFTCPKeepaliveInterval
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetUNSAFTimers::ExecuteL()
	{
	TInt requestInterval( 0 );
	TInt udpKeepAlive( 0 );
	TInt tcpKeepAlive( 0 );
	TBool writeToRepository = ReadInputParametersL( requestInterval,
													udpKeepAlive,
													tcpKeepAlive );	

	CRepository* repository = CRepository::NewLC( KCRUidUNSAFProtocols );
	TUint32 key( 0 );
	// Existing values in repository are not needed
	TInt dummyRequestInterval( 0 );
	TInt dummyUdpKeepAlive( 0 );
	TInt dummyTcpKeepAlive( 0 );
	TInt status = ReadTimerValuesL( *repository,
								    key,
								    dummyRequestInterval,
								    dummyUdpKeepAlive,
								    dummyTcpKeepAlive );
	__ASSERT_ALWAYS( status == KErrNone || status == KErrNotFound,
					 User::Leave( status ) );


	if ( writeToRepository )
		{		
		SetValuesToRepositoryL( *repository,
								status,
								key,
								requestInterval,
								udpKeepAlive,
								tcpKeepAlive );
		}
	else
		{
		if ( status == KErrNone )
			{
			RemoveParameterL( *repository,
							  KUNSAFProtocolsIntervalUDPMask | key );
			RemoveParameterL( *repository,
							  KUNSAFProtocolsIntervalTCPMask | key );
			RemoveParameterL( *repository,
							  KUNSAFProtocolsSTUNRetransmitTimerMask | key );
			RemoveParameterL( *repository,
							  KUNSAFProtocolsDomainIntervalUDPMask | key );
			RemoveParameterL( *repository,
							  KUNSAFProtocolsDomainIntervalTCPMask | key );
			}
		}

	CleanupStack::PopAndDestroy( repository );
	}

TBool TCmdSetUNSAFTimers::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetUNSAFTimers") );
	}

TTcCommandBase* TCmdSetUNSAFTimers::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetUNSAFTimers( aContext );
	}

TBool
TCmdSetUNSAFTimers::ReadInputParametersL( TInt& aBindingRequestInterval,
									  	  TInt& aUdpKeepAliveInterval,
									  	  TInt& aTcpKeepAliveInterval ) const
	{
	aBindingRequestInterval =
		ExtractIntegerL( KParamUNSAFBindingRequestInterval,
						 KErrNotFound,
						 EFalse );
	aUdpKeepAliveInterval =
		ExtractIntegerL( KParamUNSAFUDPKeepaliveInterval,
						 KErrNotFound,
						 EFalse );
	aTcpKeepAliveInterval =
		ExtractIntegerL( KParamUNSAFTCPKeepaliveInterval,
						 KErrNotFound,
						 EFalse );
	return ( aBindingRequestInterval >= 0 ||
			 aUdpKeepAliveInterval >= 0 ||
			 aTcpKeepAliveInterval >= 0 );
	}

void TCmdSetUNSAFTimers::SetValuesToRepositoryL(
	CRepository& aRepository,
	TInt aStatus,
	TUint32 aKey,
	TInt aBindingRequestInterval,
	TInt aUdpKeepAliveInterval,
	TInt aTcpKeepAliveInterval )
	{
	User::LeaveIfError(
		aRepository.StartTransaction(
			CRepository::EConcurrentReadWriteTransaction ) );
    aRepository.CleanupCancelTransactionPushL();

	TPtrC8 iapName = ExtractTextL( KParamIAPName, EFalse );
	if( iapName != KNullDesC8 )
		{
		TInt iapId = iContext.IAPIdL( iapName );
		if ( aStatus == KErrNotFound )
			{
			aKey = CreateNewKeyL( aRepository,
                                  KUNSAFProtocolsIAPIdMask,
                                  KUNSAFProtocolsIAPTableMask );
			WriteL( aRepository, KUNSAFProtocolsIAPIdMask | aKey, iapId );
			}
		WriteIapParamsL( aRepository,
					 	 aKey,
					 	 aBindingRequestInterval,
					 	 aUdpKeepAliveInterval,
					 	 aTcpKeepAliveInterval );
		}
	else
		{
		// Since IAP was not given, domain is now mandatory
		TPtrC8 domainName = ExtractTextL( KParamDomainName, ETrue );
		if ( aStatus == KErrNotFound )
			{
		    aKey = CreateNewKeyL( aRepository,
                                  KUNSAFProtocolsDomainMask,
                                  KUNSAFProtocolsDomainTableMask );
		    WriteL( aRepository, KUNSAFProtocolsDomainMask | aKey, domainName );
			}
	    WriteDomainParamsL( aRepository, 
							aKey,
							aUdpKeepAliveInterval,
							aTcpKeepAliveInterval );
		}

	TUint32 dummy( 0 );
    User::LeaveIfError( aRepository.CommitTransaction( dummy ) );
    CleanupStack::Pop( 1 ); // transaction
	}

void TCmdSetUNSAFTimers::WriteIapParamsL( CRepository& aRepository,
										  TUint32 aKey,
										  TInt aBindingRequestInterval,
										  TInt aUdpKeepAliveInterval,
										  TInt aTcpKeepAliveInterval )
    {    
    WriteL( aRepository, 
            KUNSAFProtocolsIntervalUDPMask | aKey,
            aUdpKeepAliveInterval );
	WriteL( aRepository,
            KUNSAFProtocolsIntervalTCPMask | aKey,
            aTcpKeepAliveInterval );
    WriteL( aRepository,
            KUNSAFProtocolsSTUNRetransmitTimerMask | aKey,
            aBindingRequestInterval );
    }

void TCmdSetUNSAFTimers::WriteDomainParamsL( CRepository& aRepository, 
										     TUint32 aKey,
										     TInt aUdpKeepAliveInterval,
										     TInt aTcpKeepAliveInterval )
    {
    WriteL( aRepository,
            KUNSAFProtocolsDomainIntervalUDPMask | aKey,
            aUdpKeepAliveInterval );
	WriteL( aRepository,
            KUNSAFProtocolsDomainIntervalTCPMask | aKey,
            aTcpKeepAliveInterval );
    }    

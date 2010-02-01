/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  See class definition below.
*
*/

#include "CTcSIPIMSAuthenticator.h"
#include "SipConstants.h"
#include "ErrorHandling.h"

#include <commsdat.h>
#include <CommsDatTypesV1_1.h>
#include <MetaDatabase.h>
#include <cdblen.h>
#include <tconvbase64.h>  


#ifdef RD_SIP_TESTER
const TInt KAuthenticationTimeout = 30000000; // 30 seconds
#endif

CTcSIPIMSAuthenticationParams* CTcSIPIMSAuthenticationParams::NewL()
    {
    CTcSIPIMSAuthenticationParams* self = 
        new ( ELeave ) CTcSIPIMSAuthenticationParams();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CTcSIPIMSAuthenticationParams::~CTcSIPIMSAuthenticationParams()
    {
    delete iRes;
    delete iIk;
    delete iCk;
    delete iAuts;
    }
	
void CTcSIPIMSAuthenticationParams::SetResL( const TDesC8& aRes )
    {
    delete iRes;
    iRes = 0;
    
    const TInt KBase64EncodedResLength = 20;
    iRes = EncodeL( aRes, KBase64EncodedResLength );
    }
	    
void CTcSIPIMSAuthenticationParams::SetIkL( const TDesC8& aIk )
    {
    delete iIk;
    iIk = 0;
    
    const TInt KBase64EncodedIkLength = 20;
    iIk = EncodeL( aIk, KBase64EncodedIkLength );
    }
    
void CTcSIPIMSAuthenticationParams::SetCkL( const TDesC8& aCk )
    {
    delete iCk;
    iCk = 0;
    
    const TInt KBase64EncodedCkLength = 20;
	iCk = EncodeL( aCk, KBase64EncodedCkLength );
    }
    
void CTcSIPIMSAuthenticationParams::SetAutsL( const TDesC8& aAuts )
    {
    delete iAuts;
    iAuts = 0;
    
    const TInt KBase64EncodedAutsLength = 20;
	iAuts = EncodeL( aAuts, KBase64EncodedAutsLength );
    }
	    
const TDesC8& CTcSIPIMSAuthenticationParams::Res()
    {
    return *iRes;
    }
	    
const TDesC8& CTcSIPIMSAuthenticationParams::Ik()
    {
    return *iIk;
    }
	    
const TDesC8& CTcSIPIMSAuthenticationParams::Ck()
    {
    return *iCk;
    }
	    
const TDesC8& CTcSIPIMSAuthenticationParams::Auts()
    {
    return *iAuts;
    }

void CTcSIPIMSAuthenticationParams::ConstructL()
    {
    iRes = KNullDesC8().AllocL();
    iIk = KNullDesC8().AllocL();
    iCk = KNullDesC8().AllocL();
    iAuts = KNullDesC8().AllocL();
    }
    
HBufC8* CTcSIPIMSAuthenticationParams::EncodeL( 
    const TDesC8& aInput, 
    TInt aEncodedMaxLen )
    {
    if ( aInput.Length() > 0 )
        {
    	HBufC8* encoded = HBufC8::NewLC( aEncodedMaxLen );
    	TPtr8 encodedPtr( encoded->Des() );	
        TBase64 encoder;
    	User::LeaveIfError( encoder.Encode( aInput, encodedPtr ) );
    	CleanupStack::Pop( encoded );
    	return encoded;
        }
    return KNullDesC8().AllocL();
    }
        

CTcSIPIMSAuthenticator* CTcSIPIMSAuthenticator::NewL()
    {
    CTcSIPIMSAuthenticator* self = 
        new ( ELeave ) CTcSIPIMSAuthenticator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CTcSIPIMSAuthenticator::~CTcSIPIMSAuthenticator()
    {
    Cancel();
    delete iTimer;
    delete iAuthenticationResult;
    #ifdef RD_SIP_TESTER
        {
        iPhone.Close();
        }
    #endif // RD_SIP_TESTER
    iEtelServer.Close();
    }


void CTcSIPIMSAuthenticator::RunL()
    {
    #ifdef RD_SIP_TESTER
        {
        TInt error( iStatus.Int() );
        switch ( error )
    		{
    		case KErrNone:
    		    {
    		    iAuthenticationResult->SetIkL( iData.iIK );
                iAuthenticationResult->SetCkL( iData.iCK );
                iAuthenticationResult->SetResL( iData.iRES );
    		    break;
    		    }
    		case -3214: 
    		    {
    		    // Using harcoded value for KErrMMEtelSqnVerificationFailed
    		    // to enable building SIP Tester on public SDKs
    		    // which do not have file etelmmerr.h.
    		    iAuthenticationResult->SetResL( iData.iRES );
                iAuthenticationResult->SetAutsL( iData.iAUTS );
    			break;
    		    }
    		default:
    		    {
    		    // KErrMMEtelAuthenticateFailed and KErrMMEtelMacVerificationFailed
    		    // are handled over here	
    			iError = error;
    			break;
    		    }
    		}
        }
    #endif // RD_SIP_TESTER   
        
    if ( iActiveWait.IsStarted() )
		{
		iTimer->Remove( iReceiveTimeout );
		iActiveWait.AsyncStop();
		}
    }
        
void CTcSIPIMSAuthenticator::DoCancel()
    {
    #ifdef RD_SIP_TESTER
        {
        iPhone.CancelAsyncRequest( EMobilePhoneIMSAuthenticate );
        }
    #endif // RD_SIP_TESTER
    }

CTcSIPIMSAuthenticationParams& CTcSIPIMSAuthenticator::AuthenticateL( 
    const TDesC8& aNonce )
    {
    #ifdef RD_SIP_TESTER
        {
        __ASSERT_ALWAYS( !IsActive(), User::Leave( KErrInUse ) );
        
        CTcSIPIMSAuthenticationParams* tmp = 
            CTcSIPIMSAuthenticationParams::NewL();
        delete iAuthenticationResult;
        iAuthenticationResult = tmp;
        
        SetAuthenticationDataL( aNonce );
        
        RMobilePhone::TImsAuthenticateDataV5Pckg message =
            RMobilePhone::TImsAuthenticateDataV5Pckg( iData );

    	iPhone.ImsAuthenticate( iStatus, message );	
        SetActive();
        
        iTimer->Queue( KAuthenticationTimeout, iReceiveTimeout );
    	iActiveWait.Start();

    	if ( !iAuthenticationResult )
    		{
    		User::Leave( KErrTimedOut );
    		}
    		
        if ( iError != KErrNone )
            {
            TInt err( iError );
            iError = KErrNone;
            User::Leave( err );
            }
        }
    #else
        aNonce.Length(); // Avoid compiler warning   
        User::Leave( KErrNotSupported );
    #endif // RD_SIP_TESTER
    
    return *iAuthenticationResult;
    }

CTcSIPIMSAuthenticator::CTcSIPIMSAuthenticator() : 
    CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }
    
void CTcSIPIMSAuthenticator::ConstructL()
    {
   	iTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( ReceiveTimeout, this );
	iReceiveTimeout.Set( cb );
	
	TBuf<KCommsDbSvrMaxFieldLength> tsyModuleName;
	GetTsyModuleNameL( tsyModuleName );
	PreparePhoneL( tsyModuleName );
    }
        
TInt CTcSIPIMSAuthenticator::ReceiveTimeout( TAny* aSelf )
	{
	CTcSIPIMSAuthenticator& self =
				*reinterpret_cast< CTcSIPIMSAuthenticator* >( aSelf );
	if( self.iActiveWait.IsStarted() )
		{
		self.iActiveWait.AsyncStop();
		}

	return KErrNone;
	}
	
void CTcSIPIMSAuthenticator::GetTsyModuleNameL( TDes& aTsyModuleName ) const
    {
    __ASSERT_ALWAYS( aTsyModuleName.MaxSize() >= KCommsDbSvrMaxFieldLength,
		User::Leave( KErrArgument ) );

    using namespace CommsDat;

    CMDBSession* db = CMDBSession::NewLC( KCDLatestVersion ); 

    CMDBField<TUint32>* globalSettingField =
    	new ( ELeave ) CMDBField<TUint32>( KCDTIdModemPhoneServicesSMS );
    CleanupStack::PushL( globalSettingField );

    globalSettingField->SetRecordId( 1 );
    globalSettingField->LoadL( *db );
    TUint32 modemId = *globalSettingField; 

    CMDBField<TDesC>* tsyField =
    	new ( ELeave ) CMDBField<TDesC>( KCDTIdTsyName );
    CleanupStack::PushL( tsyField );

    tsyField->SetRecordId( modemId ); 
    tsyField->LoadL( *db );
    aTsyModuleName = *tsyField;

    CleanupStack::PopAndDestroy( 3, db ); // db, tsyField & globalSettingField 
    }

void CTcSIPIMSAuthenticator::PreparePhoneL( const TDes& aTsyModuleName )
	{
	#ifdef RD_SIP_TESTER
	    {
        User::LeaveIfError( iEtelServer.Connect() );
        User::LeaveIfError( iEtelServer.LoadPhoneModule( aTsyModuleName ) );

        //Get total number of phones supported by all currently loaded TSY modules    
        TInt phoneCount( 0 );
        User::LeaveIfError( iEtelServer.EnumeratePhones( phoneCount ) );

        RTelServer::TPhoneInfo phoneInfo;
    	TName searchTsyName;
    	TInt phoneIndex( KErrNotFound );

        while ( phoneIndex == KErrNotFound && phoneCount-- > 0 )
            {
            //Check whether this phone belongs to the loaded TSY
            if ( ( iEtelServer.GetTsyName( phoneCount, searchTsyName ) == KErrNone )
                 &&
                 ( searchTsyName.CompareF( aTsyModuleName ) == 0 ) )
            	{
            	phoneIndex = phoneCount;        	
            	}
            }
    	if ( phoneIndex == KErrNotFound )
    		{
    		User::Leave( KErrNotFound );
    		}

        User::LeaveIfError( iEtelServer.GetPhoneInfo( phoneIndex, phoneInfo ) );
        User::LeaveIfError( iPhone.Open( iEtelServer, phoneInfo.iName ) );	

        RPhone::TStatus phoneStatus;
        User::LeaveIfError( iPhone.GetStatus( phoneStatus ) );

        //Check whether phone has already been initialised
        if ( phoneStatus.iMode == RPhone::EModeUnknown )
            {
            TInt err = iPhone.Initialise();

            if ( err != KErrNone )
                {
                iPhone.Close();
                User::Leave( err );
                }
            }
	    }
    #else
        {
        aTsyModuleName.Length(); // Avoid compiler warning
        }
    #endif // RD_SIP_TESTER
    }

void CTcSIPIMSAuthenticator::SetAuthenticationDataL( const TDesC8& aNonce )
    {
    #ifdef RD_SIP_TESTER
        {
        iData.iRAND.Zero();
        iData.iAUTN.Zero();
        iData.iRES.Zero();
    	iData.iIK.Zero();
        iData.iCK.Zero();
        iData.iAUTS.Zero();

        HBufC8* decoded = HBufC8::NewLC( aNonce.Length() );
        TPtr8 decodedPtr( decoded->Des() );
        
        TBase64 encoder;
    	User::LeaveIfError( encoder.Decode( aNonce, decodedPtr ) );
    		       
        __ASSERT_ALWAYS( decodedPtr.Length() >= 
                         ( RMobilePhone::KRandLength + RMobilePhone::KAutnLength ),
                         User::Leave( KErrCorrupt ) );

        TPtrC8 rand = decodedPtr.Left( RMobilePhone::KRandLength );
        TPtrC8 autn = decodedPtr.Mid( RMobilePhone::KRandLength,
                                      RMobilePhone::KAutnLength );

    	iData.iRAND.Copy( rand );
    	iData.iAUTN.Copy( autn );
    	
    	CleanupStack::PopAndDestroy( decoded );
        }
    #else
        {
        aNonce.Length(); // Avoid compiler warning
        }  
	#endif // RD_SIP_TESTER
    }
    

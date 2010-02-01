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

#ifndef CTCSIPIMSAUTHENTICATOR_H
#define CTCSIPIMSAUTHENTICATOR_H

// INCLUDES
#include <e32base.h>
#include <etelmm.h>

// FORWARD DECLARATIONS

// HELPER CLASS
class CTcSIPIMSAuthenticationParams : public CBase
    {
    public:
    
        static CTcSIPIMSAuthenticationParams* NewL();

		~CTcSIPIMSAuthenticationParams();
	
	public:
		
	    void SetResL( const TDesC8& aRes );
	    void SetIkL( const TDesC8& aIk );
	    void SetCkL( const TDesC8& aCk );
	    void SetAutsL( const TDesC8& aAuts );
	    
	    const TDesC8& Res();
	    const TDesC8& Ik();
	    const TDesC8& Ck();
	    const TDesC8& Auts();
	    
    private:
    
        CTcSIPIMSAuthenticationParams() {}
        void ConstructL();  
        
        HBufC8* EncodeL( const TDesC8& aInput, TInt aEncodedMaxLen ) ;
        
    private:
    
        HBufC8* iRes;
        HBufC8* iIk;
        HBufC8* iCk;
        HBufC8* iAuts; 
        
    };

// CLASS DEFINITION

class CTcSIPIMSAuthenticator: public CActive
	{
	public:	// Constructors and destructor

		static CTcSIPIMSAuthenticator* NewL();

		~CTcSIPIMSAuthenticator();	

    public: // From CActive
    
        void RunL();
        
        void DoCancel();
        
    public:
    
		CTcSIPIMSAuthenticationParams& AuthenticateL( const TDesC8& aNonce );
	
	private:

		CTcSIPIMSAuthenticator();

        void ConstructL();
        
	private:	

		static TInt ReceiveTimeout( TAny* aSelf );
		
		void GetTsyModuleNameL( TDes& aTsyModuleName ) const;
		
		void PreparePhoneL( const TDes& aTsyModuleName );
		
		void SetAuthenticationDataL( const TDesC8& aNonce );

	private: // data
	
	    /// Active scheduler wrapper for async waiting. Owned.
		CActiveSchedulerWait iActiveWait;
		
		/// Timeout timer. Owned.
		CDeltaTimer* iTimer;
		
		/// Timeout entry. Owned.
		TDeltaTimerEntry iReceiveTimeout;
		
		CTcSIPIMSAuthenticationParams* iAuthenticationResult;
		
        RTelServer iEtelServer;

        #ifdef RD_SIP_TESTER
            RMobilePhone iPhone;
            //Authentication data    
            RMobilePhone::TImsAuthenticateDataV5 iData;
        #endif // RD_SIP_TESTER
        
        TInt iError;
	
	};

#endif // CTCSIPIMSAUTHENTICATOR_H

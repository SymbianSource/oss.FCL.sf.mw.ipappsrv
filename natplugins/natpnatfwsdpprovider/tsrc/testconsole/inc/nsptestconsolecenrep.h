/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef NSPTESTCONSOLECENREP_H
#define NSPTESTCONSOLECENREP_H

#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>

class TCenrepKeys
	{
public: // Methods
	
	inline TUint32 DomainKey()
		{
		return KUNSAFProtocolsDomainMask;
		}
	
	inline TUint32 IapKey()
		{
		return KUNSAFProtocolsIAPIdMask;
		}
	
	inline TUint32 DomainSpecificAprotocol()
		{
	    return KUNSAFProtocolsUsedNATProtocolMask;
		}
	
	inline TUint32 DomainSpecificStunSrvAddr()
		{
		return KUNSAFProtocolsSTUNServerMask;
		}

	inline TUint32 DomainSpecificStunSrvPort()
		{
		return KUNSAFProtocolsSTUNServerPortMask;
		}
	};

class TCenrepReader : private TCenrepKeys
{
public: // Methods

	inline TCenrepReader( CRepository& aCenrep )
		: iCenrep( aCenrep )
		{
		}
	
	inline CRepository& Cenrep()
		{
		return iCenrep;
		}
	
	inline void ReadDomainL( TDes& aBuffer )
		{
		TBuf8<255> buffer;
		ReadDomainL( buffer );
		aBuffer.Copy( buffer );
		}
	
	inline void ReadDomainL( TDes8& aBuffer )
		{
		if ( KErrNotFound == iCenrep.Get( DomainKey(), aBuffer ) )
			{
			aBuffer.Zero();
			User::LeaveIfError( iCenrep.Create( DomainKey(), aBuffer ) );
			}
		}
	
	inline void ReadIapL( TUint32& aIap )
		{
		TInt value( 0 );
		
		if ( KErrNotFound == iCenrep.Get( IapKey(), value ) )
			{
			aIap = 0;
			User::LeaveIfError( iCenrep.Create( IapKey(), value ) );
			}
		
		aIap = (TUint32) value;
		}
	
	inline void ReadAProtocolL( TDes& aBuffer )
		{
		TBuf8<255> buffer;
		ReadAProtocolL( buffer );
		aBuffer.Copy( buffer );
		}
	
	inline void ReadAProtocolL( TDes8& aBuffer )
		{
		const TUint32 key = DomainSpecificAprotocol();
		
		if ( KErrNotFound == iCenrep.Get( key, aBuffer ) )
			{
			aBuffer.Zero();
			User::LeaveIfError( iCenrep.Create( key, aBuffer ) );
			}
		}
	
	inline void ReadStunSrvAddrL( TDes& aBuffer )
		{
		TBuf8<255> buffer;
		ReadStunSrvAddrL( buffer );
		aBuffer.Copy( buffer );
		}
	
	inline void ReadStunSrvAddrL( TDes8& aBuffer )
		{
		const TUint32 key = DomainSpecificStunSrvAddr();
		
		if ( KErrNotFound == iCenrep.Get( key, aBuffer ) )
			{
			aBuffer.Zero();
			User::LeaveIfError( iCenrep.Create( key, aBuffer ) );
			}
		}
	
	inline void ReadStunSrvPortL( TUint& aPort )
		{
		const TUint32 key = DomainSpecificStunSrvPort();
		TInt value( 0 );
		
		if ( KErrNotFound == iCenrep.Get( key, value ) )
			{
			aPort = 0;
			User::LeaveIfError( iCenrep.Create( key, value ) );
			}
		
		aPort = (TUint) value;
		}
	
private: // data

	CRepository& iCenrep;
};

class TCenrepWriter : private TCenrepKeys
{
public: // Methods

	inline TCenrepWriter( CRepository& aCenrep )
		: iCenrep( aCenrep )
		{
		}
	
	inline CRepository& Cenrep()
		{
		return iCenrep;
		}
	
	inline void WriteDomainL( const TDesC& aBuffer )
		{
		TBuf8<255> buffer;
		buffer.Copy( aBuffer );
		WriteDomainL( buffer );
		}
	
	inline void WriteDomainL( const TDesC8& aBuffer )
		{
		if ( KErrNotFound == iCenrep.Set( DomainKey(), aBuffer ) )
			{
			User::LeaveIfError( iCenrep.Create( DomainKey(), aBuffer ) );
			}
		}
	
	inline void WriteIapL( TUint32 aIap )
		{
		TInt value = (TInt) aIap;
		
		if ( KErrNotFound == iCenrep.Set( IapKey(), value ) )
			{
			User::LeaveIfError( iCenrep.Create( IapKey(), value ) );
			}
		}
	
	inline void WriteAProtocolL( const TDesC& aBuffer )
		{
		TBuf8<255> buffer;
		buffer.Copy( aBuffer );
		WriteAProtocolL( buffer );
		}
	
	inline void WriteAProtocolL( const TDesC8& aBuffer )
		{
		const TUint32 key = DomainSpecificAprotocol();
		
		if ( KErrNotFound == iCenrep.Set( key, aBuffer ) )
			{
			User::LeaveIfError( iCenrep.Create( key, aBuffer ) );
			}
		}
	
	inline void WriteStunSrvAddrL( const TDesC& aBuffer )
		{
		TBuf8<255> buffer;
		buffer.Copy( aBuffer );
		WriteStunSrvAddrL( buffer );
		}
	
	inline void WriteStunSrvAddrL( const TDesC8& aBuffer )
		{
		const TUint32 key = DomainSpecificStunSrvAddr();
		
		if ( KErrNotFound == iCenrep.Set( key, aBuffer ) )
			{
			User::LeaveIfError( iCenrep.Create( key, aBuffer ) );
			}
		}
	
	inline void WriteStunSrvPortL( TUint aPort )
		{
		const TUint32 key = DomainSpecificStunSrvPort();
		TInt value = (TInt) aPort;
		
		if ( KErrNotFound == iCenrep.Set( key, value ) )
			{
			User::LeaveIfError( iCenrep.Create( key, value ) );
			}
		}
	
private: // data

	CRepository& iCenrep;
};

#endif // NSPTESTCONSOLECENREP_H

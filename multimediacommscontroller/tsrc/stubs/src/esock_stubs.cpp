/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies).
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




#include <comms-infras/nifprvar.h>
#include <ES_SOCK.H>
#include <es_sock_partner.h>
//#include "SOCKMES.H"
#include "ConnPref.h"
//#include "ss_log.h"
#include <ecom/ecom.h>
#include <networking/qos3gpp_subconparams.h>

using Meta::STypeId;

////////////////////////////////////////////////////////////////////////////////
// Socket Address
////////////////////////////////////////////////////////////////////////////////

EXPORT_C TSockAddr::TSockAddr()
	: TBuf8<KMaxSockAddrSize>()
/** Default constructor initialises the address to be zero-filled. */
	{
	SetLength(KMaxSockAddrSize);
	SetFamily(KAFUnspec);
	}

EXPORT_C TSockAddr::TSockAddr(TUint aFamily)
/**
Constructs the address with the family member data specified.

@released.
@param aFamily, Address family of address object.
*/
	: TBuf8<KMaxSockAddrSize>()
/** Constructs the address with the family member data specified.

@param aFamily Address family Id. */
	{
	SetLength(KMaxSockAddrSize);
	SetFamily(aFamily);
	}

EXPORT_C TUint TSockAddr::Family() const
/** Gets the address family and thereby the type of address. The family may be 
used to effect 'down-casting'.

@return Address family Id. */
	{
	return BasePtr()->iFamily;
	}

EXPORT_C void TSockAddr::SetFamily(TUint aFamily)
/** Sets the address family of an address.

@param aFamily Address family Id. */
	{
	BasePtr()->iFamily = aFamily;
	}

EXPORT_C void TSockAddr::SetPort(TUint aPort)
//
// Set Generic Port
//
/** Sets the address port of an address.

@param aPort New port value for address */
	{
	BasePtr()->iPort = aPort;
	}

EXPORT_C TUint TSockAddr::Port() const
//
// Get Generic Port
//
/** Gets the port value.

@return Port value */
	{
	return BasePtr()->iPort;
	}

EXPORT_C TBool TSockAddr::CmpPort(const TSockAddr &anAddr) const
/**
Compare two ports.

Compares ports in two addresses

@param anAddr, socket address.
@return ETrue if the ports match and EFalse if they do not 
*/
	{
	return (BasePtr()->iFamily==anAddr.BasePtr()->iFamily) 
		&& (BasePtr()->iPort==anAddr.BasePtr()->iPort);
	}

EXPORT_C void TSockAddr::SetUserLen(TInt aLen)
//
// Set descriptor length to actual length of
// subclass.
//
/** Sets the length of data it appends after the base class' data. This function 
is mainly intended for use in derived classes.

@param aLen New length for appended data. */
	{
	SetLength(aLen+Align4(sizeof(SSockAddr)));
	}

EXPORT_C TInt TSockAddr::GetUserLen()
//
// Get length of user part of TSockAddr
//
/** Gets the number of bytes a derived class has set beyond the SSockAddr part 
of the address. This function is mainly intended for use in derived classes.

@return User length */
	{
	return Length()-Align4(sizeof(SSockAddr));
	}

////////////////////////////////////////////////////////////////////////////////
// RConnection
////////////////////////////////////////////////////////////////////////////////

EXPORT_C RConnection::RConnection()
	: iNewISPId(0)	//lint -esym(1401, RConnection::iReserved)	// no BC issue as ctor not inlined
/** Empty constructor. */
	{
	}

EXPORT_C RConnection::~RConnection()
/** Empty destructor. */
	{
	}

////////////////////////////////////////////////////////////////////////////////
// RSocketServ
////////////////////////////////////////////////////////////////////////////////

EXPORT_C RSocketServ::RSocketServ()
/**
Default Constructor
*/
    {
    }

////////////////////////////////////////////////////////////////////////////////
// RSocket
////////////////////////////////////////////////////////////////////////////////

EXPORT_C RSocket::RSocket()
/**
Default Constructor
*/
	{
	}

EXPORT_C TInt RSocket::SetOpt(TUint /*anOptionName*/,TUint /*anOptionLevel*/,const TDesC8& /*anOption*/ /*=TPtrC(NULL,0)*/)
/** Sets a socket option. The socket server has options which are generic to all 
sockets and protocols may add specific options.

Options available for all protocols can be set with anOptionLevel set to KSOLSocket. 
See individual protocol notes for other socket options. 

@param anOptionName An integer constant which identifies an option.
@param anOptionLevel An integer constant which identifies level of an option: 
i.e. an option level groups related options together.
@param anOption Option value packaged in a descriptor.
@return KErrNone if successful, otherwise another of the system-wide error 
codes. 

@capability Dependent on the type of operation so deferred to PRT.  See documentation
of constant values used in aOptionName and aOptionLevel for more information */
	{
//	return SendReceive(ESoSetOpt,TIpcArgs(anOptionName,&anOption,anOptionLevel));
    return KErrNone;
	}

EXPORT_C TInt RSocket::SetOpt(TUint anOptionName,TUint anOptionLevel,TInt anOption)
/** Sets a socket option. The socket server has options which are generic to all 
sockets and protocols may add specific options.

Options available for all protocols can be set with anOptionLevel set to KSOLSocket. 
See individual protocol notes for other socket options. 

@param anOptionName An integer constant which identifies an option.
@param anOptionLevel An integer constant which identifies level of an option: 
i.e. an option level groups related options together.
@param anOption Option value as an integer.
@return KErrNone if successful, otherwise another of the system-wide error 
codes. 

@capability Dependent on the type of operation so deferred to PRT.  See documentation
of constant values used in aOptionName and aOptionLevel for more information */
	{

	TPtr8 optionDes((TUint8*)&anOption,sizeof(TInt),sizeof(TInt));
	return SetOpt(anOptionName,anOptionLevel,optionDes);
	}

EXPORT_C void RSocket::LocalName(TSockAddr &/*anAddr*/)
/** Gets the local address of a bound socket. 

The local address is set either by calling Bind(), or is automatically set 
when Connect() is called. 

If a socket is created through Accept() then a socket will inherit the port 
of its parent unless otherwise specified by a protocol's behaviour. 

Depending on a protocol implementation, additional information may be gained 
through this call.

@param anAddr Local address which is filled in on return. */
	{
//	SendReceive(ESoGetLocalName,TIpcArgs(&anAddr));
	}

EXPORT_C TUint RSocket::LocalPort()
/** Gets the local port number of a bound socket. 

Getting the local port is similar to getting the local name.

@see LocalName() for a description.
@return The local port of a socket. */
	{
	TSockAddr addr;
	LocalName(addr);
	return addr.Port();
	}

////////////////////////////////////////////////////////////////////////////////
// TNotificationEventBuf
////////////////////////////////////////////////////////////////////////////////

EXPORT_C TNotificationEventBuf::TNotificationEventBuf()
	{
	SetLength(this->MaxLength());
	FillZ();
	}

EXPORT_C TNotificationEventBuf::~TNotificationEventBuf()
	{
	}
	
EXPORT_C TUint32 TNotificationEventBuf::Id() const
/** Provides the sub-type Id of the event

@return sub-type id
*/
	{
	return *((TUint32*)(Ptr() + sizeof(TUint32)));
	}

////////////////////////////////////////////////////////////////////////////////
// RSubConParameterBundle
////////////////////////////////////////////////////////////////////////////////

EXPORT_C RSubConParameterBundle::RSubConParameterBundle()
	: iBundle(NULL)
/** Empty sub-connection parameter bundle constructor
*/
	{
	}

EXPORT_C void RSubConParameterBundle::Close()
/** Close the parameter bundle handle
*/
	{
	if (iBundle)
		{
		iBundle->Close();
		iBundle = NULL;
		}
	}

EXPORT_C void RSubConParameterBundle::AddFamilyL(CSubConParameterFamily* aFamily)
/** Add a sub-connection parameter family to the bundle.

@param aFamily Family to be added (bundle takes ownership)
@exception leaves with KErrNoMemory in out of memory conditions
*/
	{
	User::LeaveIfError(CheckBundle());
	iBundle->AddFamilyL(aFamily);
	}

TInt RSubConParameterBundle::CheckBundle() const
/** Check to see if the handle has a heap object. If it hasn't, it creates one

@return KErrNone if successful, otherwise a system wide error
*/
	{
	if (!iBundle)
		{
		TRAPD(ret, iBundle = CSubConParameterBundle::NewL());
		return ret;
		}
	return KErrNone;
	}

////////////////////////////////////////////////////////////////////////////////
// CSubConParameterBundle
////////////////////////////////////////////////////////////////////////////////

CSubConParameterBundle::CSubConParameterBundle()
/** Empty sub-connection generic parameter bundle constructor
*/
	{
	}

EXPORT_C CSubConParameterBundle::~CSubConParameterBundle()
/** Sub-connection parameter bundle destructor, clear up the families
*/
	{
	iFamilies.ResetAndDestroy();
	}

EXPORT_C CSubConParameterBundle* CSubConParameterBundle::NewL()
/** Creates a new instance of a sub-connection parameter bundle (heap object)
    Can be used directly as a CObject or via the RSubConParameterBundle handle.

@return newly created instance of a sub-connection parameter bundle
@exception leaves with KErrNoMemory in out of memory conditions
*/
	{
	return new (ELeave) CSubConParameterBundle();
	}

EXPORT_C void CSubConParameterBundle::AddFamilyL(CSubConParameterFamily* aFamily)
/** Add a sub-connection parameter family to the bundle.

@param aFamily Family to be added (bundle takes ownership)
@exception leaves with KErrNoMemory in out of memory conditions
*/
	{
	iFamilies.AppendL(aFamily);
	}

////////////////////////////////////////////////////////////////////////////////
// CSubConParameterFamily
////////////////////////////////////////////////////////////////////////////////

EXPORT_C CSubConParameterFamily* CSubConParameterFamily::NewL(RSubConParameterBundle& aBundle, TUint32 aFamilyId)
/** Creates a new sub-connection parameter set family. This class is used as a container for a number of parameter
    sets (generic and extension) that make up a family. It is a specific instantiatable class and therefore creates
	an instance of CSubConParameterFamily without using ECOM. 

@param aBundle Family container (bundle) that this family is to be added to
@param aFamilyId identifier for the specific family type, currently only 2, QoS (KSubConQoSFamily) and Authorisation (KSubConAuthorisationFamily), are defined.
@return a pointer to a sub-connection parameter family if successful, otherwise leaves with system error code.
*/
	{
	CSubConParameterFamily* family = new (ELeave) CSubConParameterFamily(aFamilyId);
	CleanupStack::PushL(family);
	family->ConstructL(aBundle);
	CleanupStack::Pop(family);
	return family;
	}

CSubConParameterFamily::CSubConParameterFamily(TUint32 aFamilyId)
	: iFamilyId(aFamilyId)
/** Empty sub-connection parameter family constructor

@param aFamilyId Identity of the family
*/
	{
	}

void CSubConParameterFamily::ConstructL(RSubConParameterBundle& aBundle)
/** Set up the sub-connection family with empty sets

@param aBundle Bundle (container) the family is to be added to
@exception leaves with KErrNoMemory in out of memory conditions
*/
	{
	iGenericSets.AppendL(NULL);
	iGenericSets.AppendL(NULL);
	iGenericSets.AppendL(NULL);
	aBundle.AddFamilyL(this);
	}

EXPORT_C CSubConParameterFamily::~CSubConParameterFamily()
/** Sub-connection parameter family destructor. It cleans up the arrays deleting all
    the paarmeter sets it ownes
*/
	{
	iGenericSets.ResetAndDestroy();
	iExtensionSets[ERequested].ResetAndDestroy();
	iExtensionSets[EAcceptable].ResetAndDestroy();
	iExtensionSets[EGranted].ResetAndDestroy();
	}

EXPORT_C void CSubConParameterFamily::AddExtensionSetL(CSubConExtensionParameterSet& aExtensionSet, TParameterSetType aType)
/**
Adds an extension parameter set to a sub-connection parameter set family.

@param aExtensionSet Extension Parameter Set to be added to the family (family takes ownership)
@param aType The type of the set (requested, acceptable or granted)
@exception leaves with KErrNoMemory in out of memeory conditions.
*/
	{
	iExtensionSets[aType].AppendL(&aExtensionSet);
	}


EXPORT_C CSubConParameterSet* CSubConParameterSet::NewL(const STypeId& aTypeId)
/** Creates a new sub-connection parameter set using ECOM to load the DLL plugin
    that implements the set.

@param aTypeId Id of the class (Composed of Uid of the implementation and an integer sub-type
@return a generic pointer to a specific parameter set if successful, otherwise leaves with system error code.
*/
	{
	return static_cast<CSubConParameterSet*>(Meta::SMetaDataECom::NewInstanceL(aTypeId));
	}

CSubConParameterSet::~CSubConParameterSet()
/** Empty (virtual) sub-connection parameter set destructor
*/
	{
	}

CSubConParameterSet::CSubConParameterSet()
/** Empty sub-connection parameter set constructor
*/
	{
	}


////////////////////////////////////////////////////////////////////////////////
// Meta::RMetaDataContainerBase
////////////////////////////////////////////////////////////////////////////////


EXPORT_C Meta::RMetaDataContainerBase::RMetaDataContainerBase()
/**
 * Constructor
 */
    {	
    }


////////////////////////////////////////////////////////////////////////////////
// Meta::SMetaDataECom
////////////////////////////////////////////////////////////////////////////////

EXPORT_C Meta::SMetaDataECom::SMetaDataECom()
/**
 * Protected and explicit constructor
 */
	{
	iDestroyUid.iUid = 0;
	}

EXPORT_C Meta::SMetaDataECom::~SMetaDataECom()
/**
 * Destructor
 */
	{
	if ( iDestroyUid.iUid )
      {
      REComSession::DestroyedImplementation(iDestroyUid);
      }
   }

EXPORT_C SMetaDataECom* Meta::SMetaDataECom::NewInstanceL(const STypeId& aTypeId)
/**
 * Static, instantiates a meta object based on its Type ID.
 */
    {
/*    
	TUid destroyUid;
	SMetaDataECom* obj = reinterpret_cast<SMetaDataECom*>(REComSession::CreateImplementationL(aTypeId.iUid, destroyUid, (TAny*)aTypeId.iType));
	obj->iDestroyUid = destroyUid;
*/	
	SMetaDataECom* obj = reinterpret_cast<SMetaDataECom*>(REComSession::CreateImplementationL(aTypeId.iUid, (TInt32)0 ));
	return obj;
	}

EXPORT_C SMetaDataECom* Meta::SMetaDataECom::LoadL(TPtrC8& aDes)
/**
 * Static, encapsulates instantiation and loading of a content of a meta object
 * from data contained in a descriptor.
 * This function must be provided by all meta objects that will be used as a parameter
 * to TMetaPtr<> template.
 */
	{
	STypeId typeId;
	Mem::Copy(&typeId.iUid, aDes.Ptr(), sizeof(TUid));
	aDes.Set(aDes.Ptr()+sizeof(TUint32),aDes.Length()-sizeof(TUint32)); //update pointer (uid)
	Mem::Copy(&typeId.iType, aDes.Ptr(), sizeof(TInt32));
	aDes.Set(aDes.Ptr()+sizeof(TUint32),aDes.Length()-sizeof(TUint32)); //update pointer (type)

	SMetaDataECom* mtd = NewInstanceL(typeId);
    return mtd;
	}

////////////////////////////////////////////////////////////////////////////////
// Meta::SMetaData
////////////////////////////////////////////////////////////////////////////////

EXPORT_C Meta::SMetaData::SMetaData()
/**
 * Protected and explicit constructor
 */
    {
    }

EXPORT_C Meta::SMetaData::~SMetaData()
/**
 * Virtual destructor
 */
	{
	}

CSubConExtensionParameterSet::~CSubConExtensionParameterSet()
    {
        
    }

CSubConExtensionParameterSet::CSubConExtensionParameterSet()
    {
        
    }

Meta::SVDataTableEntry const* CSubConQosR99ParamSet::GetVDataTable() const
    {
        return NULL;
    }

TUint8* CSubConQosR99ParamSet::GetAttribPtr(const TInt /*aOffset*/) const
    {
        return NULL;
    }


/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device Management NAT-FW Traversal adapter
*
*/


#ifndef __NSMLDMNATFWTRAVERSALADAPTER_H__
#define __NSMLDMNATFWTRAVERSALADAPTER_H__

// INCLUDES
#include <smldmadapter.h>

// CLASS DECLARATION
class CNATFWDomainEntry;
class CNATFWIAPEntry;
class CRepository;

/**
 *  TDMNATFWStatusInfo
 *  For buffering DM commands for NAT-FW
 *  @since Series 60_3.1
 */
class TDMNATFWStatusInfo
    {

public:

    enum TCommandType
        {
        EDMAddDomain,
        EDMUpdateDomain,
        EDMDeleteDomain,
        EDMAddIAP,
        EDMUpdateIAP,
        EDMDeleteIAP,
        EDMAddSTUN,
        EDMUpdateSTUN,
        EDMDeleteSTUN,
        EDMAddTURN,
        EDMUpdateTURN,
        EDMDeleteTURN
        };

public:  // data members

    // Key id for this command 
    TUint32 iKeyId;
    
    // Status reference for this command 
    TInt iStatusRef;
    
    // command type 
    TCommandType iCommandType;

    // Settings location for interior nodes.
    TInt iLocation;
    };

/**
 *  CNSmlDmNATFWTraversalAdapter
 *  Main implementation of NAT-FW Traversal Adapter
 *  @since Series 60_3.1
 */
class CNSmlDmNATFWTraversalAdapter : public CSmlDmAdapter
    {
public: // enumerations

    enum TNatProtocol
        {
        EProtocolNone = 0,
        EStunProtocol,
        ETurnProtocol,
        EIceProtocol
        };

public:
    
    static CNSmlDmNATFWTraversalAdapter* NewL( MSmlDmCallback* aDmCallback );
    static CNSmlDmNATFWTraversalAdapter* NewLC( MSmlDmCallback* aDmCallback );
    virtual ~CNSmlDmNATFWTraversalAdapter();

private:

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aDDFVersion for returning DDF version of adapter
     */
    void DDFVersionL( CBufBase& aDDFVersion );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aDDF for returning DDF structure of adapter
     */
    void DDFStructureL( MSmlDmDDFObject& aDDF );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI of the object
     * @param aLUID LUID of the object
     * @param aObject Data of the object.
     * @param aType MIME type of the object
     * @param aStatusRef Reference to correct command
     */
    void UpdateLeafObjectL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        const TDesC8& aObject,
        const TDesC8& aType, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI of the object
     * @param aLUID LUID of the object 
     * @param aStream Data of the object.
     * @param aType MIME type of the object
     * @param aStatusRef Reference to correct command
     */
    void UpdateLeafObjectL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        RWriteStream*& aStream,
        const TDesC8& aType, 
        TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI of the object
     * @param aLUID LUID of the object
     * @param aStatusRef Reference to correct command
     */
    void DeleteObjectL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI of the object
     * @param aLUID LUID of the object
     * @param aType MIME type of the object
     * @param aResultsRef Reference to correct results
     * @param aStatusRef Reference to correct command
     */
    void FetchLeafObjectL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        const TDesC8& aType, 
        const TInt aResultsRef, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI of the object
     * @param aLUID LUID of the object
     * @param aType MIME type of the object
     * @param aResultsRef Reference to correct results
     * @param aStatusRef Reference to correct command
     */
    void FetchLeafObjectSizeL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        const TDesC8& aType, 
        TInt aResultsRef, 
        TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI from dm-module
     * @param aLUID LUID of the parent object
     * @param aPreviousURISegmentList from dm-module
     * @param aResultsRef results reference from dm-module
     * @param aStatusRef Reference to correct command
     */
    void ChildURIListL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList, 
        const TInt aResultsRef, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI of the object
     * @param aParentLUID LUID of the parent object
     * @param aStatusRef Reference to correct command
     */
    void AddNodeObjectL( 
        const TDesC8& aURI, 
        const TDesC8& aParentLUID, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI from dm-module
     * @param aLUID LUID of the object
     * @param aArgument Argument for the command
     * @param aType MIME type of the object 
     * @param aStatusRef Reference to correct command 
     */
    void ExecuteCommandL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        const TDesC8& aArgument, 
        const TDesC8& aType, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aURI URI from dm-module
     * @param aLUID LUID of the object
     * @param aStream Argument for the command
     * @param aType MIME type of the object 
     * @param aStatusRef Reference to correct command 
     */
    void ExecuteCommandL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        RWriteStream*& aStream, 
        const TDesC8& aType, 
        const TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aTargetURI Target URI for the command
     * @param aTargetLUID LUID of the target object
     * @param aSourceURI Source URI for the command
     * @param aSourceLUID LUID of the source object
     * @param aType MIME type of the objects
     * @param aStatusRef Reference to correct command
     */
    void CopyCommandL( 
        const TDesC8& aTargetURI, 
        const TDesC8& aTargetLUID, 
        const TDesC8& aSourceURI, 
        const TDesC8& aSourceLUID, 
        const TDesC8& aType, 
        TInt aStatusRef );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * 
     */
    void StartAtomicL();

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * 
     */
    void CommitAtomicL();

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * 
     */
    void RollbackAtomicL();

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * @param aItemSize size limit for stream usage
     * @return TBool ETrue for streaming support
     */
    TBool StreamingSupport( TInt& aItemSize );

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * 
     */
    void StreamCommittedL();

    /**
     * From MSmlDmAdapter
     * @since Series 60_3.1
     * 
     */
    void CompleteOutstandingCmdsL();

private: 

    CNSmlDmNATFWTraversalAdapter( TAny* aEcomArguments );

    /**
     * Fetches leaf object values
     * @since Series 60_3.1
     * @param aURI, URI of the object
     * @param aLUID, LUID of the object
     * @param aObject, data of the object.
     * @return TError Status code of operation
     */
    CSmlDmAdapter::TError FetchObjectL( 
        const TDesC8& aURI, 
        const TDesC8& aLUID, 
        CBufBase& aObject );
        
    /**
     * Fetches Domain specific leaf object values
     * @since Series 60_3.1
     * @param aURI, URI of the object
     * @param aLUID, LUID of the object
     * @param aObject, data of the object.
     * @return TError Status code of operation
     */
    CSmlDmAdapter::TError FetchDomainObjectL(
        const TDesC8& aURI, 
        const TDesC8& aLUID,
        TDes8& aSegmentResult );
        
    /**
     * Fetches IAP specific leaf object values
     * @since Series 60_3.1
     * @param aURI, URI of the object
     * @param aLUID, LUID of the object
     * @param aObject, data of the object.
     * @return TError Status code of operation
     */
    CSmlDmAdapter::TError FetchIAPObjectL(
        const TDesC8& aURI, 
        const TDesC8& aLUID,
        TDes8& aSegmentResult,
        CBufBase& aObject );

    /**
     * Fetches AdditionalSTUNServer/<x>/ leaf object values
     * @since Series 60_3.2
     * @param aURI, URI of the object
     * @param aLUID, LUID of the object
     * @param aObject, data of the object.
     * @return TError Status code of operation
     */
    CSmlDmAdapter::TError FetchAdditionalSTUNServerObjectL(
        const TDesC8& aURI, 
        const TDesC8& aLUID,
        TDes8& aSegmentResult );

    /**
     * Fetches STUNSpecific/STUNServer/<x>/ leaf object values
     * @since Series 60_3.2
     * @param aURI, URI of the object
     * @param aLUID, LUID of the object
     * @param aObject, data of the object.
     * @return TError Status code of operation
     */
    CSmlDmAdapter::TError FetchTURNServerObjectL(
        const TDesC8& aURI, 
        const TDesC8& aLUID,
        TDes8& aSegmentResult );

    /**
     * Updates DomainSpecific node
     * @since Series 60_3.2
     */   
    CSmlDmAdapter::TError UpdateDomainSpecificObjectL(
        const TDesC8& aURI,
        const TDesC8& aLUID, 
        const TDesC8& aObject, 
        const TInt aStatusRef );

    /**
     * Updates IAPSpecific node
     * @since Series 60_3.2
     */   
    CSmlDmAdapter::TError UpdateIAPSpecificObjectL(
        const TDesC8& aURI,
        const TDesC8& aLUID, 
        const TDesC8& aObject, 
        const TInt aStatusRef );

    /**
     * Updates DomainSpecific/<x>/AdditionalSTUNServer node
     * @since Series 60_3.2
     */   
    CSmlDmAdapter::TError UpdateAdditionalSTUNServerObjectL(
        const TDesC8& aURI,
        const TDesC8& aLUID, 
        const TDesC8& aObject, 
        const TInt aStatusRef );

    /**
     * Updates DomainSpecific/<x>/TURNSpecific/TURNServer node
     * @since Series 60_3.2
     */   
    CSmlDmAdapter::TError UpdateTURNServerObjectL(
        const TDesC8& aURI,
        const TDesC8& aLUID, 
        const TDesC8& aObject, 
        const TInt aStatusRef );

    /**
     * Lists the Domain specific children of the node
     * @since Series 60_3.2
     */
    CSmlDmAdapter::TError DomainChildURIListL(
        const TDesC8& aURI,
        const TDesC8& aLUID, 
        CBufBase* const aURISegmentList,
        const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList );

    /**
     * Lists the IAP specific children of the node
     * @since Series 60_3.2
     */
    CSmlDmAdapter::TError IAPChildURIListL(
        const TDesC8& aURI,
        const TDesC8& aLUID, 
        CBufBase* const aURISegmentList,
        const CArrayFix<TSmlDmMappingInfo>& aPreviousURISegmentList );

    /**
     * Loads NAT-FW profile information
     * @since Series 60_3.1
     */
    void LoadProfilesL();

    /**
     * Saves NAT-FW profile information
     * @since Series 60_3.1
     */
    void SaveProfilesL();
    
    /**
     * Adds NAT-FW settings
     * @param aRep Central Repository for NAT-FW Traversal settings.
     * @since Series 60_3.2
     */
    void AddSettingsL( CRepository* aRep );

    /**
     * Updates NAT-FW settings
     * @param aRep Central Repository for NAT-FW Traversal settings.
     * @since Series 60_3.2
     */
    void UpdateSettingsL( CRepository* aRep );

    /**
     * Deletes NAT-FW settings
     * @param aRep Central Repository for NAT-FW Traversal settings.
     * @since Series 60_3.2
     */
    void DeleteSettingsL( CRepository* aRep );

    /**
     * Gets domain specific settings from CenRep.
     * @since Series 60_3.1
     * @param aDomainKeyId CenRep domain key to get.
     * @param aEntry, domain entry to store values.
     */
    void GetDomainSettingsL( TUint32 aDomainKeyId, 
        CNATFWDomainEntry& aEntry );

    /**
     * Gets IAP specific settings from CenRep.
     * @since Series 60_3.1
     * @param aIAPKey CenRep IAP key to get.
     * @param aEntry, IAP entry to store values.
     */
    void GetIAPSettingsL( TUint32 aIAPKey, 
        CNATFWIAPEntry& aEntry );

    /**
     * Finds domain location in buffered entries.
     * @since Series 60_3.1
     * @param aLUID, LUID value to look.
     * @return TInt, location in entries.
     */
    TInt FindDomainLocation( const TDesC8& aLUID );

    /**
     * Finds domain location in buffered entries.
     * @since Series 60_3.1
     * @param aDomainKey, CenRep key value to look.
     * @return TInt, location in entries.
     */
    TInt FindDomainLocation( TUint32 aDomainKey );

    /**
     * Finds IAP location in buffered entries.
     * @since Series 60_3.1
     * @param aLUID, LUID value to look.
     * @return TInt, location in entries.
     */
    TInt FindIAPLocation( const TDesC8& aLUID );

    /**
     * Finds IAP location in buffered entries.
     * @since Series 60_3.1
     * @param aLUID, CenRep key value to look.
     * @return TInt, location in entries.
     */
    TInt FindIAPLocation( TUint32 aIAPKey );

    /**
     * Converts descriptor to TUint.
     * @since Series 60_3.1
     * @param aDes descriptor to convert.
     * @return TUint, converted value.
     */
    TUint DesToTUint( const TDesC8& aDes );

    /**
     * Find accesspoint with AP adapter and sets it to IAP specific entry.
     * @since Series 60_3.1
     * @param aObject, PrefConRef value to be set.
     * @param aProfileLoc, entry location to set IAP ID.
     * @return TInt, IAP Id found.
     */
    TInt SetConRefL( const TDesC8& aObject, TInt aProfileLoc );

    /**
     * Find PrefConRef value (AP/APIdxxx) with AP adapter.
     * @since Series 60_3.1
     * @param aObject, PrefConRef value to be set.
     * @param aProfileLoc, entry location to get IAP ID.
     * @return TInt, IAP Id found.
     */
    TBool GetConRefL( CBufBase& aObject, TInt aProfileLoc );

    /**
     * Converts descriptor to int.
     * @since Series 60_3.0
     * @param aDes descriptor to convert.
     * @return TInt. Converted value.
     */
    TInt DesToInt( const TDesC8& aDes );

    /**
     * Removes last uri segment from aURI
     * @since Series 60_3.0
     * @param aDes Descriptor to handle
     * @return pointer to uri
     */
    TPtrC8 RemoveLastURISeg( const TDesC8& aURI );

    /**
     * Gets number of uri segments.
     * @since Series 60_3.0
     * @param aURI Descriptor to handle
     * @return TInt. Number of URI segments.
     */
    TInt NumOfURISegs( const TDesC8& aURI );

    /**
     * Gets last uri segment.
     * @since Series 60_3.0
     * @param aURI Descriptor to handle
     * @return TPtrC8. Pointer to beginning of last uri segment.
     */
    TPtrC8 LastURISeg( const TDesC8& aURI );

    /**
     * Removes ./ from uri
     * @since Series 60_3.0
     * @param aURI Descriptor to handle
     * @return pointer to uri
     */
    TPtrC8 RemoveDotSlash( const TDesC8& aURI );

private: // Data

    /**
     * For returning data to DM module.
     * Not own.
     */
    MSmlDmCallback* iDmCallback;
    
    /**
     * Profile ID currently being modified.
     */
    TInt iProfileID;

    /**
     * Array of pointers to all Domain entries.
     * Own.
     */
    CArrayPtrFlat<CNATFWDomainEntry>* iNATFWDomainEntries;
    
    /**
     * Array of pointers to all IAP entries.
     * Own.
     */
    CArrayPtrFlat<CNATFWIAPEntry>* iNATFWIAPEntries;
    
    /**
     * For buffering DM commands.
     */
    RArray< TDMNATFWStatusInfo > iNATFWCommandBuffer;
    
    /**
     * For knowing status of dm-module.
     */
    TInt iStatus;
    
    /**
     * For returning results to dm-module.
     * Not own.
     */
    CBufBase *iResults;

private: // Friend class for testing

   	#if defined( TEST_EUNIT )
    friend class UT_CNSmlDmNATFWTraversalAdapter;
    #endif
    };

#endif // __NSMLDMNATFWTRAVERSALADAPTER_H__

// End of File

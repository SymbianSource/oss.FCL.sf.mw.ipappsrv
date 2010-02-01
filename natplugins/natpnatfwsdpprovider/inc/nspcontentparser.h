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
* Description:  Description for Sdp content parser.
*
*/

#ifndef NSPCONTENTPARSER_H
#define NSPCONTENTPARSER_H

#include <e32base.h>
#include <stringpool.h>
#include "natfwconnectivityapidefs.h"
#include "natfwcandidate.h"
#include "nsputdefinitions.h"

class TInetAddr;
class CSdpDocument;
class CSdpMediaField;
class CSdpAttributeField;
class CNATFWCredentials;

/**
 *  Class methods are used to insert/remove/modify SDP content.
 *
 *  SDP document must be modified in both directions to point NAT FW ports,
 *  to ensure perfect audio routing. Modification of these documents are 
 *  done by means introduced in this class.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPContentParser : public CBase
    {
public: // Constructors and destructor

    /**
     * A two-phase constructor.
     */
    static CNSPContentParser* NewL();
    
    /**
     * A two-phase constructor.
     */
    static CNSPContentParser* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNSPContentParser();


private: // Constructors and destructor

    CNSPContentParser();
    
    void ConstructL();


public: // New functions

    /**
     * Used to check whether sdp document contains any candidate - attributes.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, that is to be
     *                                  checked for candidate - attributes.
     * @return      Boolean value indicating whether Ice grammar is supported.
     */
    TBool IsIceSupported( CSdpDocument& aDocument ) const;
    
    /**
     * Used to check whether sdp document has a mismatch situation.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object which m/c - lines
     *                                  are compared against candidate - lines.
     * @return      Boolean value indicating a mismatch in Sdp document.
     */
    TBool IsMismatchL( CSdpDocument& aDocument ) const;
    
    /**
     * Used to check whether media field has a mismatch situation.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object which primary port,
     *                                  and given Inet address, is compared
     *                                  against candidate - line trans. address.
     * @param       aAddress            Address descriptor from the session level.
     * @return      Boolean value indicating a mismatch in Sdp document.
     */
    TBool IsMismatchL( CSdpMediaField& aMediaField, const TDesC8& aAddress ) const;
    
    /**
     * Used to check whether media field contains ice-mismatch - attribute.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object which attributes
     *                                  are searched for ice-mismatch attribute.
     * @return      Boolean value indicating whether ice-mismatch was present.
     */
    TBool IsMismatchAttribute( CSdpMediaField& aMediaField ) const;
    
    /**
     * Used to check whether sdp document contains ice-lite - attribute.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object which attributes
     *                                  are searched for ice-lite attribute.
     * @return      Boolean value indicating whether ice-lite was present.
     */
    TBool IsLiteAttribute( CSdpDocument& aDocument ) const;
    
    /**
     * Used to check whether media field has rtcp - attribute. If rtcp was 
     * present then transport address values are given 
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object which attributes
     *                                  are searched for rtcp attribute.
     * @param       aAddress            A reference to a TDes8 descriptor, used
     *                                  to convey transport address strings.
     * @param       aPort               Reference to TUint, which is used to convey
     *                                  port information from the rctp-attribute.
     * @return      Boolean value indicating whether rtcp was present.
     */
    TBool FindRTCP( CSdpMediaField& aMediaField, TDes8& aAddress, TUint& aPort ) const;
    
    /**
     * Used to modify rctp information in rtcp - attribute.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, which 
     *                                  rtcp-attribute is modified to contain
     *                                  a new transport address.
     * @param       aAddress            A reference to a TDesC8 descriptor, used
     *                                  to convey transport address values.
     * @param       aPort               Port for rtcp-attribute
     */
    void ModifyRTCPL( CSdpMediaField& aMediaField, const TDesC8& aAddress, TUint aPort ) const;
    
    /**
     * Used to insert ice-lite - attribute.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object where ice-lite 
     *                                  is added.
     */
    void AddLiteL( CSdpDocument& aDocument ) const;
    
    /**
     * Used to insert ice-mismatch - attribute to answer, from corresponding
     * offer media line.
     *
     * @since       S60 3.2
     * @param       aMismatch           Sdp document object, which has mismatch attribute.
     * @param       aMismatched         Sdp document object, which is edited to
     *                                  have mismatch attribute in m-line,
     *                                  from corresponding offer m-line.
     */
    void AddMismatchL( CSdpDocument& aMismatch, CSdpDocument& aMismatched ) const;
    
    /**
     * Used to insert the actual ice-mismatch - attribute.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, in which the
     *                                  ice-mismatch attribute is added.
     */
    void AddMismatchL( CSdpMediaField& aMediaField ) const;
    
    /**
     * Used to insert candidates from the array to the media line.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, in which the
     *                                  candidate - attributes are added.
     * @param       aCandidates         Array containing NAT FW candidate objects.
     */
    void AddCandidatesL( CSdpMediaField& aMediaField,
            const RPointerArray<CNATFWCandidate>& aCandidates ) const;
    
    /**
     * Used to insert remote-candidates attribute to media line.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, where remote
     *                                  candidates attribute is inserted.
     * @param       aCandidates         Array containing remote candidates.
     */
    void AddRemoteCandidatesL( CSdpMediaField& aMediaField,
            const RPointerArray<CNATFWCandidate>& aCandidates ) const;
    
    /**
     * Used to insert ice-ufrag - & ice-pwd - attributes to media line.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, in which the
     *                                  ice-ufrag/ice-pwd attributes are added.
     * @param       aCredentials        Object containing NAT FW credentials.
     */
    void AddCredentialsL( CSdpMediaField& aMediaField,
            const CNATFWCredentials& aCredentials ) const;
    
    /**
     * Used to insert ice-ufrag - & ice-pwd - attributes to session level.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, in which the
     *                                  ice-ufrag/ice-pwd attributes are added.
     * @param       aCredentials        Object containing NAT FW credentials.
     */
    void AddCredentialsL( CSdpDocument& aDocument,
            const CNATFWCredentials& aCredentials ) const;
    
    /**
     * Used to get candidate - attributes from media field.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, where the
     *                                  candidate attributes are acquired from.
     * @param       aCandidates         Array where candidates are inserted.
     * @return		Warning code if failure(s) happened in candidate decoding.
     */
    TInt GetCandidatesL( CSdpMediaField& aMediaField,
            RPointerArray<CNATFWCandidate>& aCandidates ) const;
    
    /**
     * Used to get remote-candidates - attributes from sdp document.
     * 
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, from where 
     *                                  remote-candidates attributes are retrieved.
     * @param       aRemoteCands        Array where remote candidates are inserted.
     * @return		Warning code if failure(s) happened in candidate decoding.
     */
    TInt GetRemoteCandidatesL( CSdpDocument& aDocument,
            RPointerArray<CNATFWCandidate>& aRemoteCands ) const;
    
    /**
     * Used to get remote-candidates - attributes from media field.
     * 
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, where the
     *                                  remote-candidates attributes are acquired from.
     * @param       aRemoteCands        Array where remote candidates are inserted.
     * @return		Warning code if failure(s) happened in candidate decoding.
     */
    TInt GetRemoteCandidatesL( CSdpMediaField& aMediaField,
            RPointerArray<CNATFWCandidate>& aRemoteCands ) const;
    
    /**
     * Used to get ice-ufrag - & ice-pwd - attributes from media field.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, from which the
     *                                  candidate attributes are acquired.
     * @param       aCredentials        Object containing NAT FW credentials.
     * @return		Warning code if failure(s) happened in candidate decoding.
     */
    TInt GetCredentialsL( CSdpMediaField& aMediaField,
            CNATFWCredentials& aCredentials ) const;
    
    /**
     * Used to get ice-ufrag - & ice-pwd - attributes from session level.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp media field object, where the
     *                                  candidate attributes are acquired from.
     * @param       aCredentials        Object containing NAT FW credentials.
     * @return		Warning code if failure(s) happened in candidate decoding.
     */
    TInt GetCredentialsL( CSdpDocument& aDocument,
            CNATFWCredentials& aCredentials ) const;
    
    /**
     * Used to remove all ice specific attributes from sdp document.
     *
     * @since       S60 3.2
     * @param       aDocument           Sdp document object, from which all
     *                                  Ice specific content is removed.
     */
    void RemoveIceContent( CSdpDocument& aDocument ) const;
    
    /**
     * Used to remove all ice specific attributes from media field.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object, from which all
     *                                  Ice specific content is removed.
     */
    void RemoveIceContent( CSdpMediaField& aMediaField ) const;


private: // New functions

    CSdpAttributeField* EncodeCandidateLineL(
            const CNATFWCandidate& aCandidate ) const;
    
    void DecodeCandidateLineL(
            const CSdpAttributeField& aAttribute,
            CNATFWCandidate& aCandidate ) const;
    
    CSdpAttributeField* EncodeRemoteCandidatesL(
            const RPointerArray<CNATFWCandidate>& aCandidates ) const;
    
    void EncodeRemoteCandidateL( TDes8& aBuffer,
            const CNATFWCandidate& aCandidate ) const;
    
    void DecodeRemoteCandidatesL( const TDesC8& aValue,
            RPointerArray<CNATFWCandidate>& aCandidates ) const;
    
    CSdpAttributeField* EncodeUfragL(
            const CNATFWCredentials& aCredentials ) const;
    
    void DecodeUfragL( const CSdpAttributeField& aAttribute,
            CNATFWCredentials& aCredentials ) const;
    
    CSdpAttributeField* EncodePwdL(
            const CNATFWCredentials& aCredentials ) const;
    
    void DecodePwdL( const CSdpAttributeField& aAttribute,
            CNATFWCredentials& aCredentials ) const;
    
    void LexTDesC8ToTUintL( const TDesC8& aDesC8, TUint& aUint ) const;
    
    void SolveTransportProtocolL( const TDesC8& aDesC8, TUint& aProto ) const;
    
    void InputTDesC8ToTInetAddrL( const TDesC8& aDesC8, TInetAddr& aAddress ) const;
    
    void SolveCandidateTypeL( const TDesC8& aDesC8Typ,
            const TDesC8& aDesC8Value, CNATFWCandidate::TCandidateType& aType ) const;
    
    TInt FindCandidateType( const TDesC8& aDesC8Value ) const;
    
    TInt FindCandidateTypeString( const CNATFWCandidate::TCandidateType& aType ) const;
    
    void AppendTDesC8ToTDes8L( TDes8& aDes8,
            const TDesC8& aDes, TBool aAddFieldSeparator = ETrue ) const;
    
    void AppendNumToTDes8L( TDes8& aDes8,
            TUint aNumber, TBool aAddFieldSeparator = ETrue ) const;
    
    void AppendProtocolToTDes8L( TDes8& aDes8,
            const TUint& aProtocol, TBool aAddFieldSeparator = ETrue ) const;
    
    void AppendTInetAddrToTDes8L( TDes8& aDes8,
            const TInetAddr& aAddress, TBool aAddFieldSeparator = ETrue ) const;
    
    void AppendTypeToTDes8L( TDes8& aDes8,
            const CNATFWCandidate::TCandidateType& aType,
            TBool aAddFieldSeparator = ETrue ) const;
    
    void LeaveIfTooLongL( const TDes8& aDes8, const TInt aLength ) const;
    
    CSdpMediaField* FindMediaField( const CSdpMediaField& aMediaField,
            const RPointerArray<CSdpMediaField>& aMediaFields ) const;
    
    static void CleanupArrayItem( TAny* aArray );


private: // data

    /**
     * Handle to a string that resides in string pool, to "candidate" string.
     */
    RStringF iCandidate;
    
    /**
     * Handle to a string that resides in string pool, to "remote-candidates"
     * string.
     */
    RStringF iRemoteCandidates;
    
    /**
     * Handle to a string that resides in string pool, to "ice-lite" string.
     */
    RStringF iLite;
    
    /**
     * Handle to a string that resides in string pool, to "ice-passive" string.
     */    
    RStringF iPassive;
    
    /**
     * Handle to a string that resides in string pool, to "ice-ufrag" string.
     */
    RStringF iUfrag;
    
    /**
     * Handle to a string that resides in string pool, to "ice-pwd" string.
     */
    RStringF iPwd;
    
    /**
     * Handle to a string that resides in string pool, to "ice-mismatch" string.
     */
    RStringF iMismatch;
    
    /**
     * Handle to a string that resides in string pool, to "rtcp" string.
     */
    RStringF iRTCP;
    
    /**
     * Handle to a string that resides in string pool, to "UDP" string.
     */
    RStringF iUdp;
    
    /**
     * Handle to a string that resides in string pool, to "TCP" string.
     */
    RStringF iTcp;
    
    /**
     * Handle to a string that resides in string pool, to "IN" string.
     */
    RStringF iIN;
    
    /**
     * Handle to a string that resides in string pool, to "IP4" string.
     */
    RStringF iIPv4;
    
    /**
     * Handle to a string that resides in string pool, to "IP6" string.
     */
    RStringF iIPv6;
    
    /**
     * Definitions for unit testing.
     */
    NSP_UT_DEFINITIONS
    
    };

#endif // NSPCONTENTPARSER_H

// end of file

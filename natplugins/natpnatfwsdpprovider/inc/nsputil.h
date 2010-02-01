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
* Description:  Utility class description
*
*/

#ifndef NSPUTIL_H
#define NSPUTIL_H

#include "nsppluginreturndef.h"
#include <e32des8.h>
#include <e32cmn.h>
#include "nsputdefinitions.h"

class CSdpDocument;
class CSdpConnectionField;
class CSdpOriginField;
class CSdpMediaField;
class CNATFWCandidate;
class CNSPMediaStream;

/**
 *  Static utility class, that contains common sdp processing methods.
 *
 *  This class has static methods that are needed by multiple states in
 *  state machine. To avoid duplicate code, static methods are preferred.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class NSPUtil
    {
public: // New functions

    /**
     * This method is used to map a single Sdp media field to correct
     * media stream. Mapping is done by comparing Media() - return values.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aMediaStreams       Array containing media streams.
     * @return      Pointer to media stream object, ownership is not changed.
     *              NULL returned if not found.
     */
    static CNSPMediaStream* FindMediaStream( const CSdpMediaField& aMediaField,
            RPointerArray<CNSPMediaStream>& aMediaStreams );
    
    /**
     * This method is used to map a single Sdp media field to correct
     * media stream. Mapping is done by comparing Media() - return values.
     *
     * @since       S60 3.2
     * @param       aMediaStream        media stream object.
     * @param       aMediaStreams       Array containing media streams.
     * @return      Pointer to media stream object, ownership is not changed.
     *              NULL returned if not found.
     */
    static CNSPMediaStream* FindMediaStream( const CNSPMediaStream& aMediaStream,
            RPointerArray<CNSPMediaStream>& aMediaStreams );
    
    /**
     * This method is used to map a single Sdp media stream to correct
     * media field. Mapping is done by comparing Media() - return values.
     *
     * @since       S60 3.2
     * @param       aMediaStream        media stream object.
     * @param       aMediaFields        Array containing Sdp media field
     *                                  objects.
     * @return      Pointer to media field object, ownership is not changed.
     *              NULL returned if not found.
     */
    static CSdpMediaField* FindMediaField( const CNSPMediaStream& aMediaStream,
            RPointerArray<CSdpMediaField>& aMediaFields );
    
    /**
     * This method is used to map a single Sdp media field to another
     * media field. Mapping is done by comparing Media() - return values.
     *
     * @since       S60 3.2
     * @param       aMediaField         Sdp media field object.
     * @param       aMediaFields        Array containing Sdp media field
     *                                  objects.
     * @return      Pointer to media field object, ownership is not changed.
     *              NULL returned if not found.
     */
    static CSdpMediaField* FindMediaField( const CSdpMediaField& aMediaField,
            RPointerArray<CSdpMediaField>& aMediaFields );
    
    /**
     * This method is used to update transport address to connection field.
     * 
     * @since       S60 3.2
     * @param       aField              Sdp connection field object.
     * @param       aAddress            address as string.
     */
    static void UpdateConnectionFieldL( CSdpConnectionField& aField,
            const TDesC8& aAddress );
    
    /**
     * This method is used to update transport address to origin field.
     * 
     * @since       S60 3.2
     * @param       aField              Sdp origin field object.
     * @param       aAddress            address as string.
     */
    static void UpdateOriginFieldL( CSdpOriginField& aField,
            const TDesC8& aAddress );
    
    /**
     * This method is used to insert media field rejection info to media field.
     *
     * @since       S60 3.2
     * @param       aField              Sdp media field object.
     */
    static void RejectL( CSdpMediaField& aField );
    
    /**
     * This method is used to check if the given media field is rejected,
     * i.e. reject port.
     * 
     * @since       S60 3.2
     * @param       aField              Sdp media field object.
     * @return      ETrue if media field is rejected, EFalse otherwise.
     */
    static TBool IsReject( const CSdpMediaField& aField );
    
    /**
     * This method is used to check if given candidate object is duplicate for some
     * other object in the given array.
     *
     * @since       S60 3.2
     * @param       aCand               Candidate object
     * @param       aCandidates         Candidates array.
     * @return      ETrue if candidate is a duplicate, EFalse otherwise.
     */
    static TBool IsDuplicate( const CNATFWCandidate& aCand,
            const RPointerArray<CNATFWCandidate>& aCandidates );
    
    /**
     * This method is used to check if given address is 'Unspeficied', i.e.
     * either 0.0.0.0(IPv4) or ::(IPv6).
     * Leave will occur if heap runs out.
     * 
     * @since		S60 3.2
     * @param		aAddress			Address as descriptor.
     * @param		aPort				Port as TUint.
     * @return		ETrue if given address is unspecified, EFalse otherwise.
     */
    static TBool IsUnspecifiedL( const TDesC8& aAddress, TUint aPort );
    
    /**
     * This method is used to sort objects as priority order.
     *
     * @since       S60 3.2
     * @param       aLocalCandidates    Array containing NAT FW candidate objects.
     */
    static void SortCandidatesL( RPointerArray<CNATFWCandidate>& aLocalCandidates );
    
    /**
     * This method is used jointly with TCleanupItem - class to clear objects from 
     * RPointerArray if leave occurs.
     *
     * @since       S60 3.2
     * @param       anArray             Array containing objects.
     */
    static void CleanupArrayItem( TAny* anArray );


private: // data
        
    NSP_UT_DEFINITIONS
    
    };

#endif // NSPUTILITY_H

// end of file

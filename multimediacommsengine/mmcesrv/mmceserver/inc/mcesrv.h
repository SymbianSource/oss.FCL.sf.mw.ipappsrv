/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCESRV_H
#define MCESRV_H

#ifndef MCE_COMMON_SERVER_SIDE
#define MCE_COMMON_SERVER_SIDE
#endif

#ifdef EUNIT_UNIT_TEST
#pragma warn_illtokenpasting off
#endif

#ifdef MCESRV_UNIT_TESTING
#define MCESRV_UT_DEFINITIONS\
    friend class UT_CMceCsSession;\
    friend class UT_CMceCsSubSession;\
    friend class UT_CMceSipSession;\
    friend class UT_CMceSipEvent;\
    friend class UT_TMceActionSet;\
    friend class UT_CMceStateIdle;\
    friend class UT_CMceStateClientEstablishing;\
    friend class UT_CMceStateOffering;\
    friend class UT_CMceStateEstablished;\
    friend class UT_CMceStateServerEstablishing;\
    friend class UT_CMceStateServerOffering;\
    friend class UT_CMceStateUpdating;\
    friend class UT_CMceStateUpdated;\
    friend class UT_CMceStateAnswering;\
    friend class UT_CMceStateTerminating;\
    friend class UT_CMceStateCanceled;\
    friend class UT_CMceStateError;\
    friend class UT_CMceStateConfirming;\
    friend class UT_CMceStateClientReserving;\
    friend class UT_CMceStateAcknowledgementRequired;\
    friend class UT_CMceStateServerReserving;\
    friend class UT_CMceStateConfirmationRequired;\
    friend class UT_CMceStateServerInitializing;\
    friend class UT_CMceSipConnection;\
    friend class UT_CMceClientResolver;\
    friend class UT_CMceSipManager;\
    friend class UT_MCEUsesCases;\
    friend class UT_CMceCsReceiverBase;\
    friend class UT_CMceSipDefaultData;\
    friend class UT_CMceSipDefaults;\
    friend class UT_CMceCsReceiveQueue;\
    friend class UT_MCESIP;\
    friend class UT_CMceDictionary;\
    friend class UT_CMceSipExtensions;\
    friend class UT_CMceReliableSender;\
    friend class UT_CMceLocalAddrResolver;\
    friend class UT_CMceNatSipSession;\
    friend class UT_CMCENatUseCases;\
    friend class UT_CMceNatStateCreateOfferRequested;\
    friend class UT_CMceNatStateCreateOffer;\
    friend class UT_CMceNatStateCreateAnswer;\
    friend class UT_CMceNatStateDecodeAnswer;\
    friend class UT_CMceNatStateDecodeOffer;\
    friend class UT_CMceNatStateWait;\
    friend class UT_CMceNatStateConnected;\
    friend class UT_CMceNatStateIdle;\
    friend class MCETestHelper;\
    friend class UT_CMceState;
#else

#define MCESRV_UT_DEFINITIONS

#endif

#ifdef MCEMM_UNIT_TESTING
#define MCEMM_UT_DEFINITIONS\
    friend class UT_CMceMediaManager;\
    friend class UT_CMceSdpSession;\
    friend class UT_TMceSrvStreamIterator;\
    friend class UT_TMceMediaState;\
    friend class UT_TMceMediaIdle;\
    friend class UT_TMceOfferingMedia;\
    friend class UT_TMceAnsweringMedia;\
    friend class UT_TMceAnsweringMediaUpdate;\
    friend class UT_TMceMediaNegotiated;\
    friend class UT_TMcePreparingOffererStreams;\
    friend class UT_TMceStartingOffererReceiveStreams;\
    friend class UT_TMceStartingOffererSendStreams;\
    friend class UT_TMcePreparingAnswererStreams;\
    friend class UT_TMceStartingAnswererStreams;\
    friend class UT_TMceSegmentedPreconditions;\
    friend class UT_TMceSecurePreconditions;\
    friend class UT_CMceSrvStream;\
    friend class UT_CMceSrvSink;\
    friend class UT_CMceSrvSource;\
    friend class UT_CMceAdoptedSrvStream;\
    friend class UT_CMceSdpCodec;\
    friend class UT_CMceMediaSdpCodec;\
    friend class UT_CMceAudioSdpCodec;\
    friend class UT_CMceVideoSdpCodec;\
    friend class UT_CMceComCameraSource;\
    friend class UT_CMceComFileSource;\
    friend class MceMediaManagerTestHelper;
    
    
#else

#define MCEMM_UT_DEFINITIONS

#endif
    
	
#endif

// End of File

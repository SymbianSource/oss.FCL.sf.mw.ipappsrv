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
* Description:    This file lists all supported commands of SIPTester
*
*/



#ifndef __COMMANDINDEX_H__
#define __COMMANDINDEX_H__

// INCLUDES

#include "TCmdAddAudioStream.h"
#include "TCmdAddVideoStream.h"
#include "TCmdAddSink.h"
#include "TCmdCreateEvent.h"
#include "TCmdCreateRefer.h"
#include "TCmdCreateSession.h"
#include "TCmdEstablish.h"
#include "TCmdFCGetMediaAttrLines.h"
#include "TCmdFCSend.h"
#include "TCmdFCUpdate.h"
#include "TCmdDeleteProperty.h"
#include "TCmdGetAudioCodecInfo.h"
#include "TCmdGetVideoCodecInfo.h"
#include "TCmdGetControlPathSecurityLevel.h"
#include "TCmdGetCurrentState.h"
#include "TCmdGetEventId.h"
#include "TCmdGetEventState.h"
#include "TCmdGetFCMessage.h"
#include "TCmdGetMessage.h"
#include "TCmdGetNotify.h"
#include "TCmdGetProperty.h"
#include "TCmdGetReceiverReport.h"
#include "TCmdGetReferId.h"
#include "TCmdGetReferState.h"
#include "TCmdGetRtpEvent.h"
#include "TCmdGetSenderReport.h"
#include "TCmdGetSessionId.h"
#include "TCmdGetState.h"
#include "TCmdGetStreamInfo.h"
#include "TCmdGetStreamState.h"
#include "TCmdGetSupportedCodecs.h"
#include "TCmdGetSinkStatus.h"
#include "TCmdGetSourceStatus.h"
#include "TCmdModifyAudioCodec.h"
#include "TCmdModifyVideoCodec.h"
#include "TCmdRemoveCodec.h"
#include "TCmdRemoveStream.h"
#include "TCmdSendAcceptEvent.h"
#include "TCmdSendAcceptRefer.h"
#include "TCmdSendAcceptSession.h"
#include "TCmdSendCancel.h"
#include "TCmdSendDeRegister.h"
#include "TCmdSendNotify.h"
#include "TCmdSendRefer.h"
#include "TCmdSendRegister.h"
#include "TCmdSendRejectEvent.h"
#include "TCmdSendRejectRefer.h"
#include "TCmdSendRejectSession.h"
#include "TCmdSendRequest.h"
#include "TCmdSendResponse.h"
#include "TCmdSendRinging.h"
#include "TCmdSendRespond.h"
#include "TCmdSendRR.h"
#include "TCmdSendSR.h"
#include "TCmdSendSubscribe.h"
#include "TCmdSendTerminate.h"
#include "TCmdSendTerminateEvent.h"
#include "TCmdSetObserver.h"
#include "TCmdSetProperty.h"
#include "TCmdSetSinkActivity.h"
#include "TCmdSetSource.h"
#include "TCmdSetSourceActivity.h"
#include "TCmdSetStreamActivity.h"
#include "TCmdSetStreamLocalPort.h"
#include "TCmdTerminateProcess.h"
#include "TCmdUpdateSDPLines.h"
#include "TCmdUpdateSession.h"
#include "TCmdUpdateSubscription.h"
#include "TCmdUpdateRtpSink.h"
#include "TCmdCreateSecureSession.h"
#include "TCmdSendStreamDtmf.h"
#include "TCmdGetProfileId.h"
#include "TCmdFileHandler.h"


/// MCE Command index
/// List all commands in this table
const TcCommand KTcSscCommands[] =
	{
	AddCmd( TCmdAddAudioStream ),
    AddCmd( TCmdAddVideoStream ),
	AddCmd( TCmdAddSink ),
	AddCmd( TCmdCreateEvent ),
	AddCmd( TCmdCreateRefer ),
	AddCmd( TCmdCreateSession ),
	AddCmd( TCmdDeleteProperty ),
	AddCmd( TCmdEstablish ),
	AddCmd( TCmdFCGetMediaAttrLines),
	AddCmd( TCmdFCSend ),
	AddCmd( TCmdFCUpdate ),
	AddCmd( TCmdGetAudioCodecInfo ),
	AddCmd( TCmdGetVideoCodecInfo ),
	AddCmd( TCmdGetControlPathSecurityLevel ),
	AddCmd( TCmdGetCurrentState ),
	AddCmd( TCmdGetEventId ),
	AddCmd( TCmdGetEventState ),
	AddCmd( TCmdGetFCMessage ),
	AddCmd( TCmdGetMessage ),
	AddCmd( TCmdGetNotify ),
	AddCmd( TCmdGetProperty ),
	AddCmd( TCmdGetReceiverReport ),
	AddCmd( TCmdGetReferId ),
	AddCmd( TCmdGetReferState ),
	AddCmd( TCmdGetRtpEvent ),
	AddCmd( TCmdGetSenderReport ),
	AddCmd( TCmdGetSessionId ),
	AddCmd( TCmdGetState ),
	AddCmd( TCmdGetStreamInfo ),
	AddCmd( TCmdGetStreamState ),
	AddCmd( TCmdGetSupportedCodecs ),
	AddCmd( TCmdGetSinkStatus ),
	AddCmd( TCmdGetSourceStatus ),
	AddCmd( TCmdModifyAudioCodec ),
	AddCmd( TCmdModifyVideoCodec ),
	AddCmd( TCmdRemoveCodec ),
	AddCmd( TCmdRemoveStream ),
	AddCmd( TCmdSendAcceptEvent ),
	AddCmd( TCmdSendAcceptRefer ),
	AddCmd( TCmdSendAcceptSession ),
	AddCmd( TCmdSendCancel ),
	AddCmd( TCmdSendDeRegister ),	
	AddCmd( TCmdSendNotify ),
	AddCmd( TCmdSendRefer ),
	AddCmd( TCmdSendRegister ),
	AddCmd( TCmdSendRejectEvent ),
	AddCmd( TCmdSendRejectRefer ),
	AddCmd( TCmdSendRejectSession ),
	AddCmd( TCmdSendRequest ),
	AddCmd( TCmdSendResponse ),
	AddCmd( TCmdSendRinging ),
	AddCmd( TCmdSendRespond ),
	AddCmd( TCmdSendRR ),
	AddCmd( TCmdSendSR ),
	AddCmd( TCmdSendSubscribe ),
	AddCmd( TCmdSendTerminate ),
	AddCmd( TCmdSendTerminateEvent ),
	AddCmd( TCmdSetObserver ),
	AddCmd( TCmdSetProperty ),
	AddCmd( TCmdSetSinkActivity ),
	AddCmd( TCmdSetSource ),
	AddCmd( TCmdSetSourceActivity ),
	AddCmd( TCmdSetStreamActivity ),
	AddCmd( TCmdSetStreamLocalPort ),
	AddCmd( TCmdTerminateProcess ),
	AddCmd( TCmdUpdateSDPLines ),
	AddCmd( TCmdUpdateSession ),
	AddCmd( TCmdUpdateSubscription ),
	AddCmd( TCmdUpdateRtpSink ),
	AddCmd( TCmdCreateSecureSession ),
	AddCmd( TCmdSendStreamDtmf ),
	AddCmd( TCmdGetProfileId ),
	AddCmd( TCmdFileHandler )
	};

#endif // __COMMANDINDEX_H__

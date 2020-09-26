#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "CnSMSDBThread.h"
#include "CnServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "CnSimLayer.h"

#define CLASS_TYPE KCnSMSDBThread
ImplPfID( KCnSMSDBThread, PI_CN_SMS_DB );

KCnSMSDBThread::~KCnSMSDBThread(void)
{
}

void KCnSMSDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{    
		CASE( DBE_SEND_PHONE_MSG_NOT );
		//{{ 2010. 11. 2	������	������ ���� ����͸�
#ifdef SERV_MORNITORING
		_CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
		//}}

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

void KCnSMSDBThread::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KCnSMSDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KCnSMSDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSUser( nGS, nTo, usEventID, char() );
}

IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_NOT )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrPhoneNum, kPacket_.m_vecPhoneNum )
	{
        std::wstring wstrProject = L"ES";
        std::wstring wstrNation = KSimLayer::GetKObj()->GetAuthTypeStr( KSimLayer::GetKObj()->GetAuthFlag() );
        DO_QUERY_NO_PROFILE( L"exec SMS.dbo.SMS_Send", L"N\'%s\', N\'%s\',N\'%s\',N\'%s\',N\'%s\',%d, %d", 
            % wstrPhoneNum 
            % wstrPhoneNum
            % kPacket_.m_wstrSMSMessage
            % wstrProject
            % wstrNation
            % 0
            % 0 
            );
	}

	START_LOG( cout2, L"�޴��� �޼��� ������ ����!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
	return;

end_proc:
	START_LOG( cerr, L"�޴��� �޼��� ������ ����!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
}

//{{ 2010. 11. 2	������	������ ���� ����͸�
#ifdef SERV_MORNITORING

_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, KDBE_SEND_PHONE_MSG_NOT )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrPhoneNum, kPacket_.m_vecPhoneNum )
	{
        std::wstring wstrProject = L"ES";
        std::wstring wstrNation = KSimLayer::GetKObj()->GetAuthTypeStr( KSimLayer::GetKObj()->GetAuthFlag() );
        DO_QUERY_NO_PROFILE( L"exec SMS.dbo.SMS_Send", L"N\'%s\', N\'%s\',N\'%s\',N\'%s\',N\'%s\',%d, %d", 
            % wstrPhoneNum 
            % wstrPhoneNum
            % kPacket_.m_wstrSMSMessage
            % wstrProject
            % wstrNation
            % 0
            % 0 
            );
	}

	START_LOG( cout2, L"�޴��� �޼��� ������ ����!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );
	return;

end_proc:
	START_LOG( cerr, L"�޴��� �޼��� ������ ����!" )
		<< BUILD_LOG( kPacket_.m_wstrSMSMessage );

	SendToServer( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, kPacket_ );
}

#endif SERV_MORNITORING
//}}


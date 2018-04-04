#ifndef _LISTENEVENT_H
#define _LISTENEVENT_H

#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/thread.h"
#include "event2/buffer.h"

#include "ClientManager.h"

#ifdef _MSC_VER
#pragma comment(lib,"ws2_32.lib")
#endif

#include <signal.h>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <string.h>
#endif

struct _Callback_Info
{
	event_base*      m_pBase;          //eventָ��
	CClientManager*  m_pClientManager; //�ͻ�������
	 
	_Callback_Info()
	{
		m_pBase          = NULL;
		m_pClientManager = NULL;
	}
};

//����Sig�¼�
void signal_cb(evutil_socket_t sig, short events, void *user_data);

//����ͻ��˽����¼�
void listener_cb(evconnlistener* listener, evutil_socket_t fd,
                 struct sockaddr* sock, int socklen, void* arg);

//�������ݵ����¼�
void socket_read_cb(bufferevent* bev, void* arg);

//���������쳣����Ϣ
void socket_event_cb(bufferevent* bev, short events, void* arg);

//����windows����˿�
int Listen_Windows(int nPort, _Callback_Info* pCallbackInfo);

//����Linux����˿�
int Listen_Linux(int nPort, _Callback_Info* pCallbackInfo);

#endif


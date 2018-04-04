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
	event_base*      m_pBase;          //event指针
	CClientManager*  m_pClientManager; //客户端连接
	 
	_Callback_Info()
	{
		m_pBase          = NULL;
		m_pClientManager = NULL;
	}
};

//处理Sig事件
void signal_cb(evutil_socket_t sig, short events, void *user_data);

//处理客户端接入事件
void listener_cb(evconnlistener* listener, evutil_socket_t fd,
                 struct sockaddr* sock, int socklen, void* arg);

//处理数据到达事件
void socket_read_cb(bufferevent* bev, void* arg);

//处理连接异常的消息
void socket_event_cb(bufferevent* bev, short events, void* arg);

//监听windows服务端口
int Listen_Windows(int nPort, _Callback_Info* pCallbackInfo);

//监听Linux服务端口
int Listen_Linux(int nPort, _Callback_Info* pCallbackInfo);

#endif


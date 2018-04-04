#include "ListenEvent.h"

int Listen_Windows(int nPort, _Callback_Info* pCallbackInfo)
{
#if WIN32
		struct event *signal_event;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);
    //这里必须初始化网络，不然会创建Socket失败
    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    //监听端口
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(nPort);

    evthread_use_windows_threads();

    struct event_config* cfg = event_config_new();
    event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP);
    //根据CPU实际数量配置libEvent的CPU数
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    event_config_set_num_cpus_hint(cfg, si.dwNumberOfProcessors);

    event_base* base;
    base = event_base_new_with_config(cfg);
    event_config_free(cfg);
    
    //配置参数
    pCallbackInfo->m_pBase  = base;
    printf("[Listen_Windows]base=0x%08x.\n", base);

    // 绑定并监听IPV4端口
    evconnlistener* listener = evconnlistener_new_bind(base, listener_cb, (void*)pClientManager,
                               LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                               10, (struct sockaddr*)&sin,
                               sizeof(sin));

		//注册监听sig事件
		if (!listener) 
		{
			printf("[Listen_Windows]Could not create a listener(%d)!\n", nPort);
			return 1;
		}

		signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

		if (!signal_event || event_add(signal_event, NULL)<0) 
		{
			printf("[Listen_Windows]Could not create/add a signal event!\n");
			return 1;
		}		
		
    //事件分发处理
    printf("[Listen_Windows]Listen port(%d) success.\n", nPort);
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);
    WSACleanup();

#endif
    return 0;
}

int Listen_Linux(int nPort, _Callback_Info* pCallbackInfo)
{
#ifdef _LINUX
		struct event *signal_event;

    //监听端口
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(nPort);

    struct event_config* cfg = event_config_new();
    event_config_set_flag(cfg, EVENT_BASE_FLAG_NOLOCK);

    event_base* base;
    base = event_base_new_with_config(cfg);
    event_config_free(cfg);
    
    pCallbackInfo->m_pBase  = base;
    printf("[Listen_Linux]base=0x%08x.\n", base);

    // 绑定并监听IPV4端口
    evconnlistener* listener = evconnlistener_new_bind(base, listener_cb, (void*)pCallbackInfo,
                               LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
                               10, (struct sockaddr*)&sin,
                               sizeof(sin));
		
		//注册监听sig事件
		if (!listener) 
		{
			printf("[Listen_Linux]Could not create a listener(%d)!\n", nPort);
			return 1;
		}

		signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

		if (!signal_event || event_add(signal_event, NULL)<0) 
		{
			printf("[Listen_Linux]Could not create/add a signal event!\n");
			return 1;
		}	

    //事件分发处理
    printf("[Listen_Linux]Listen port(%d) success.\n", nPort);
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

#endif
    return 0;
}

void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
	event_base *base = (event_base* )user_data;
	struct timeval delay = { 2, 0 };

	printf("[signal_cb]Caught an interrupt signal; exiting cleanly in two seconds.\n");

	event_base_loopexit(base, &delay);
}

void listener_cb(evconnlistener* listener, evutil_socket_t fd,
                 struct sockaddr* sock, int socklen, void* arg)
{
    char Buffer[256];
    sockaddr_in* addr = (sockaddr_in*)sock;
    evutil_inet_ntop(addr->sin_family, &addr->sin_addr, Buffer, sizeof(Buffer));
    //printf("accept a client %d,IP:%s\n", fd, Buffer);

    _Callback_Info* pCallbackInfo = (_Callback_Info* )arg;
    CClientManager* pClientManager = pCallbackInfo->m_pClientManager;
    
    pClientManager->Add(Buffer, addr->sin_port, fd);
    pClientManager->Display();

		printf("[listener_cb]pCallbackInfo->m_pBase=0x%08x.\n", pCallbackInfo->m_pBase);
    //为这个客户端分配一个bufferevent
    bufferevent* bev = bufferevent_socket_new(pCallbackInfo->m_pBase, fd,
                       BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, pCallbackInfo->m_pClientManager);
    bufferevent_enable(bev, EV_READ);

    printf("[listener_cb]OK.\n");
}

void socket_read_cb(bufferevent* bev, void* arg)
{   
    char szMsg[4096] = {'\0'};  
    
    //获得Client管理对象
    CClientManager* pClientManager = (CClientManager* )arg;
    
    int nCllientfd = (int)bufferevent_getfd(bev);
    
 		size_t input_len = 4096;
  	int nLen = bufferevent_read(bev, szMsg, input_len);
  	
  	if(nLen <= 0)
  	{
  		printf("[socket_read_cb](fd=%d,len=%d)Error.\n", nCllientfd, nLen);
  	}
  	else
  	{
  		printf("[socket_read_cb](fd=%d,len=%d))szMsg=%s.\n", nCllientfd, nLen, szMsg);
  		
  		
  	}
    
}

void socket_event_cb(bufferevent* bev, short events, void* arg)
{
    //获得Client管理对象
    CClientManager* pClientManager = (CClientManager* )arg;	
    
    int nCllientfd = (int)bufferevent_getfd(bev);
    pClientManager->Del(nCllientfd);
    pClientManager->Display();
	
    if (events & BEV_EVENT_EOF)
    {
        printf("[socket_event_cb](%d)connection closed.\n", nCllientfd);
    }
    else if (events & BEV_EVENT_ERROR)
    {
        printf("[socket_event_cb](%d)some other error.\n", nCllientfd);
    }

    //这将自动close套接字和free读写缓冲区
    bufferevent_free(bev);
}

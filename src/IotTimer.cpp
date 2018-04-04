// LibEventServer.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "ListenEvent.h"

#define LISTEN_PORT 20000   //�����˿�



int main()
{
		CClientManager objClientManager;
		_Callback_Info objCallbackInfo;
		
		objCallbackInfo.m_pClientManager = &objClientManager;
		
#ifdef WIN32
    Listen_Windows(LISTEN_PORT, &objCallbackInfo);
#else
    Listen_Linux(LISTEN_PORT, &objCallbackInfo);
#endif

		printf("Server run done.\n");		

    return 0;
}
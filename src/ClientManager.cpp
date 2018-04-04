#include "ClientManager.h"

CClientManager::CClientManager()
{
	pthread_mutex_init(&m_mutex, NULL);  
}

CClientManager::~CClientManager()
{
	Close();	
}

void CClientManager::Close()
{
	m_vec_Client_Socket.clear();	
	pthread_mutex_destroy(&m_mutex);  
}

int CClientManager::Check_socket(int nSocket)
{
	int nCount = (int)m_vec_Client_Socket.size();
	for(int i = 0; i < nCount; i++)
	{
		if(m_vec_Client_Socket[i].m_nSocket == nSocket)
		{
			return i;
		}
	}
	
	return -1;
}

int CClientManager::Del_Socket(int nSocket)
{
	for(vec_Client_Socket::iterator Iter = m_vec_Client_Socket.begin(); Iter != m_vec_Client_Socket.end(); Iter++) 
	{ 
	  if((*Iter).m_nSocket == nSocket) 
	  { 
	     m_vec_Client_Socket.erase(Iter);
	     return 0;
	  }
	}
	
	return -1;
}

void CClientManager::Add(const char* pIp, int nPort, int nSocket)
{
	pthread_mutex_lock(&m_mutex);  
	
	int nIndex  =Check_socket(nSocket);
	if(nIndex == -1)
	{
		//添加新的客户端信息
		_Client_Socket obj_Client_Socket;
		obj_Client_Socket.m_nSocket = nSocket;
		sprintf(obj_Client_Socket.m_szIp, "%s", pIp);
		obj_Client_Socket.m_nPort   = nPort;
		
		m_vec_Client_Socket.push_back(obj_Client_Socket);
	}
	else
	{
		sprintf(m_vec_Client_Socket[nIndex].m_szIp, "%s", pIp);
		m_vec_Client_Socket[nIndex].m_nPort = nPort;
	}
	
	pthread_mutex_unlock(&m_mutex);  
	return;
}	

void CClientManager::Del(int nSocket)
{
	pthread_mutex_lock(&m_mutex);  
	Del_Socket(nSocket);
	pthread_mutex_unlock(&m_mutex); 
}

_Client_Socket* CClientManager::Get(int nSocket)
{
	pthread_mutex_lock(&m_mutex);  
	
	int nIndex = Check_socket(nSocket);
	if(nIndex == -1)
	{
		pthread_mutex_unlock(&m_mutex);  
		return NULL;
	}
	else
	{
		pthread_mutex_unlock(&m_mutex);  
		return &m_vec_Client_Socket[nIndex];
	}
}	

void CClientManager::Display()
{
	pthread_mutex_lock(&m_mutex);  
	
	printf("[Display]>>>>>>>>>>>>>>>>.\n");
	int nCount = (int)m_vec_Client_Socket.size();
	for(int i = 0; i < nCount; i++)
	{
		printf("[Display]Socket=%d,Ip=%s.Port=%d.\n", 
						m_vec_Client_Socket[i].m_nSocket,
						m_vec_Client_Socket[i].m_szIp,
						m_vec_Client_Socket[i].m_nPort);
	}
	printf("[Display]>>>>>>>>>>>>>>>>.\n");
	
	pthread_mutex_unlock(&m_mutex);  
	return;
}
	
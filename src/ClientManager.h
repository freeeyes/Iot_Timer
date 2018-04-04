#include "Common.h"
#include "pthread.h"

//客户端管理类
class CClientManager
{
public:	
	CClientManager();
	~CClientManager();
	
	void Close();
	
	void Add(const char* pIp, int nPort, int nSocket);
	void Del(int nSocket);
	_Client_Socket* Get(int nSocket);
	
	void Display();
	
private:
	int Check_socket(int nSocket);
	int Del_Socket(int nSocket);
	
private:
	vec_Client_Socket m_vec_Client_Socket;
	pthread_mutex_t   m_mutex;
};
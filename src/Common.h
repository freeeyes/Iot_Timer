#include <stdio.h>
#include <vector>

using namespace std;

//基础结构
//add by freeeyes

struct _Client_Socket
{
	int  m_nSocket;        //连接信息ID
	int  m_nVersion;       //版本号
	char m_szBoxID[12];    //当前盒子ID
	char m_szIp[30];       //当前盒子IP
	int  m_nPort;          //当前盒子端口  
	
	_Client_Socket()
	{
		m_nSocket    = 0;
		m_nVersion   = 0;
		m_szBoxID[0] = '\0';
		m_szIp[0]    = '\0';
		m_nPort      = 0;
	}
};
typedef vector<_Client_Socket> vec_Client_Socket;


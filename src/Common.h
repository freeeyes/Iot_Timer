#include <stdio.h>
#include <vector>

using namespace std;

//�����ṹ
//add by freeeyes

struct _Client_Socket
{
	int  m_nSocket;        //������ϢID
	int  m_nVersion;       //�汾��
	char m_szBoxID[12];    //��ǰ����ID
	char m_szIp[30];       //��ǰ����IP
	int  m_nPort;          //��ǰ���Ӷ˿�  
	
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


#ifndef __CONFIG_BASE__
#define __CONFIG_BASE__

#include <string.h>
#include "EZThread.h"
#include "EZTimer.h"
#include "EZSignals.h"
#include <json.h>

typedef struct tagENUM_MAP
{
	const char* name;
	int value;
}
ENUM_MAP;

class CConfigExchange;

class CConfigKey
{
	friend class CConfigExchange;
public:
	CConfigKey(int index)
	{
		m_kind = kindIndex;
		m_value.index = index;
	};

	CConfigKey(const char* name)
	{
		m_kind = kindName;
		m_value.name = name;
	};

private:
	enum Kind
	{
		kindNone = 0,
		kindIndex,
		kindName
	}m_kind;

	union
	{
		int index;
		const char *name;
	}m_value;
};

typedef Json::Value CConfigTable;
#ifdef WIN32
typedef Json::StyledWriter CConfigWriter;
#else
typedef Json::FastWriter CConfigWriter;
#endif
typedef Json::Reader CConfigReader;

typedef TSignal2<CConfigExchange &, int &> TCONFIG_SIGNAL;
typedef TCONFIG_SIGNAL::SigProc TCONFIG_PROC;

//����Ӧ�ý������λ��ʾ�������õĹ۲������ã����õ��ύ�߼��
enum CONFIG_APPLY_RET
{
	CONFIG_APPLY_OK = 0,					// �ɹ�
	CONFIG_APPLY_RESTART = 0x00000001,		// ��Ҫ����Ӧ�ó���
	CONFIG_APPLY_REBOOT = 0x00000002,		// ��Ҫ����ϵͳ
	CONFIG_APPLY_FILE_ERROR = 0x00000004,	// д�ļ�����
	CONFIG_APPLY_CAPS_ERROR = 0x00000008,	// ���Բ�֧��
	CONFIG_APPLY_GATEWAY_ERROR = 0x00000010,//��������ʧ��,��Ȼ����Ϊ�ɹ�
	CONFIG_APPLY_VALIT_ERROR = 0x00000020,	/*!< ��֤ʧ�� */
	CONFIG_APPLY_NOT_EXSIST = 0x00000040,	/*!< ���ò����� */
	CONFIG_APPLY_NOT_SAVE = 0x00000080,		// ��Ҫ���浽�ļ�
	CONFIG_APPLY_DELAY_SAVE = 0x00000100,	//��ʱ����..��λռ��ȡֵΪ0x180��ԭ���������ñ����ʱ����Ҫ����

	CONFIG_APPLY_USER_DEFINE = 0x0FF00000,	//�û��Զ��塪�� �ڴ������з���ret���������ֵ����ִ���Զ��崦��
};

class CConfigExchange
{
public:
	typedef enum tagCONFIG_STATE
	{
		CS_SAVING = 0,
		CS_LOADING,
		CS_VALIDATING,
		CS_DEFAUTING,
	} CONFIG_STATE;

public:
	CConfigExchange(int n, int& used, int id, TCONFIG_SIGNAL& sig);

	//״̬����Ч��
	void setValidity(int bValid);
	int getValidity();
	CONFIG_STATE getState() const;
	void setState(CONFIG_STATE state);

	//�������ݽ���
	void exchange(CConfigTable& table, CConfigKey key, int& value, int scope, int dflt, ENUM_MAP* map);
	void exchange(CConfigTable& table, CConfigKey key, int& value, int min, int max, int dflt, ENUM_MAP* map);
	void exchange(CConfigTable& table, CConfigKey key, int& value, int min, int max, int dflt);

	//�ַ������ݽ���
	void exchange(CConfigTable& table, CConfigKey key, std::string& value, const std::string& scope, const std::string& dflt);
	void exchange(CConfigTable& table, CConfigKey key, std::string& value, const std::string& dflt);
	void exchange(CConfigTable& table, CConfigKey key, char* value, const char* scope, const char* dflt);
	void exchange(CConfigTable& table, CConfigKey key, char* value, const char* dflt);

	//���������ݽ���
	void exchange(CConfigTable& table, CConfigKey key, int& value, int dflt);

	//16�������ݽ���
	void exchange(CConfigTable& table, CConfigKey key, unsigned int& value, unsigned int dflt);

public:
	//ȡĬ��ֵ
	void recall(int index = -1);

	//���Ϸ���
	int validate(int index = -1);

	//���µ���������
	void update(int index = -1);

	//�ύ���ã����Ϸ��Լ�顣����ֵΪ����Ӧ�õĽ����ΪCONFIG_APPLY_RET��λ��ϡ�
	int commit(const char* user = NULL, int index = -1, int retInitial = CONFIG_APPLY_DELAY_SAVE);

	// �������õ��ַ�����
	void setTable(CConfigTable& table);

	// ��ȡ���õ��ַ�����
	void getTable(CConfigTable& table);

	// ע��۲캯��
	int attach(CEZObject * pObj, TCONFIG_PROC pProc);

	// ���ٹ۲캯��
	int detach(CEZObject * pObj, TCONFIG_PROC pProc);

	// �õ����ڵ���commit���û���
	const char* getUser();

	// �õ������Ʊ�ʶ��
	int getID();

	// �������ýṹ�������Ѿ�ʹ�õ�����
	void setUsed(int used);

	// �õ����ýṹ�������Ѿ�ʹ�õ�����
	int getUsed();

	// �õ����ýṹ���鳤��
	int getNumber();

private:// ����ĺ�������ģ��ʵ��
	virtual void exchangeTable(CConfigTable& table, int index) = 0;
	virtual void copy(int index, int save) = 0;
	virtual int equal(int index) = 0;

private:
	const char* getValue(CConfigTable& table, CConfigKey key, const char* dflt);
	int getValue(CConfigTable& table, CConfigKey key, int dflt);
	CConfigTable& getValue(CConfigTable& table, CConfigKey key);

private:
	static	CEZMutex m_mutex;
	static const char* m_strUser;
	CONFIG_STATE m_iState;
	int m_bValid;
	int m_eleNumber;
	int m_eleUsed;			/*! �Ѿ�ʹ�õ�Ԫ�ظ�������ʼ��ʱ��������ĳ��ȣ����ö�ȡʱ���������ַ����ļ��е�ʵ��
							�ṹ�������������ֵ����ȡĬ��ʱ������ʾ�ĵ���setUsed������Ĭ����ĸ����������ط�ʹ
							������Ҳ���Ե���getUsed��setUsed�ӿڡ�����Ҫ�õ��������û��ĸ�����ֱ��ʹ��getUsed
							�ӿڼ��ɣ������û�����ʱ�����ȵ���setUsed������ʵ�ʵ��û���������ȥ����commit��*/
	int& m_eleUsedLatest;	//���������Ѿ�ʹ�õ�Ԫ�ظ���
	int m_id;
	TCONFIG_SIGNAL& m_sigRef;
};

//T ���ýṹ����
template <class T> void exchangeTable(CConfigExchange& xchg, CConfigTable& table, T& config, int index, int app);

//T ���ýṹ����
//number �ṹ��Ŀ
//observers �۲��������Ŀ
//appInstance Ӧ��ʵ�������ǰ��3������һ����Ҫͨ��������������ֲ�ͬ��Ӧ�ã�����ʹ����ͬ�ṹ������໥����
//As ���ñȽ�ʱ�����ͣ������void, ��ʾʹ���ڴ�ȽϺ������Ƚ�
template <class T, int number, int observers, int appInstance = 0, class As = void> class TConfig : public CConfigExchange
{
public:
	TConfig():CConfigExchange(number, m_used, (int)m_latest, m_sigConfig)
	{
	}
	virtual ~TConfig()
	{
	}

	// �õ��������õ�����
	inline static const T& getLatest(int index = 0)
	{
		return m_latest[index];
	}

	// �õ���ǰ���õ�����
	inline T& getConfig(int index = 0)
	{
		return m_config[index];
	}

	// �õ���ǰ���õ����ã�ͨ���±������
	inline T& operator[](int index)
	{
		return m_config[index];
	}

	// ��̬ע��۲캯��
	static inline int attach(CEZObject * pObj, TCONFIG_PROC pProc)
	{
		return m_sigConfig.Attach(pObj, pProc);
	}

	// ��̬ע���۲캯��
	static inline int detach(CEZObject * pObj, TCONFIG_PROC pProc)
	{
		return m_sigConfig.Detach(pObj, pProc);
	}

	// ���ø��½ӿ�, ������ʵ��
	void exchangeTable(CConfigTable& table, int index)
	{
		::exchangeTable(*(CConfigExchange*)this, table, getConfig(index), index, appInstance);
	}

	void copy(int index, int save)
	{
		if(save)
		{
			m_latest[index] = m_config[index];
		}
		else
		{
			m_config[index] = m_latest[index];
		}
	}

	inline int equal(const T* x, const T* y)
	{
		return (*x == *y);
	}

	inline int equal(const void * x, const void * y)
	{
		return (memcmp(x, y, sizeof(T)) == 0);
	}

	int equal(int index)
	{
		return equal((As*)&m_latest[index], (As*)&m_config[index]);
	}

private:
	static TCONFIG_SIGNAL m_sigConfig;
	static T m_latest[number];
	T m_config[number];
	static int m_used;
};

template<class T, int number, int observers, int appInstance, class As>
T TConfig<T, number, observers, appInstance, As>::m_latest[];

template<class T, int number, int observers, int appInstance, class As>
TCONFIG_SIGNAL TConfig<T, number, observers, appInstance, As>::m_sigConfig(observers);

template<class T, int number, int observers, int appInstance, class As>
int TConfig<T, number, observers, appInstance, As>::m_used = number;

#endif //__CONFIG_BASE__


#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#ifdef WIN32
#include <io.h>
#endif

#ifdef __TCS__
#include <time.h>
#endif

#include "Types.h"

struct FSOperations;

/// \class CFile
/// \brief �ļ������װ
class CFile
{
public:

	/// �ļ��򿪱�־
	enum OpenFlags {
		modeRead =         (int) 0x00000, ///< Ϊ����һ���ļ�������ļ������ڻ��޷��򿪣�Open����ʧ�ܡ�
		modeWrite =        (int) 0x00001, ///< Ϊд����һ�����ļ�������ļ����ڣ������ݽ������١�
		modeReadWrite =    (int) 0x00002, ///< Ϊ��д��һ���ļ�������ļ������ڻ��޷��򿪣�Open����ʧ�ܡ�
		shareCompat =      (int) 0x00000,
		shareExclusive =   (int) 0x00010,
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		shareDenyNone =    (int) 0x00040,
		modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000, ///< ����ļ������ڣ��Զ������ļ�����modeReadWrite���ʹ�á�
		modeNoTruncate =   (int) 0x02000, ///< ��modeCreateһ��ʹ�ã����Ҫ�������ļ��Ѿ����ڣ�ԭ�������ݱ�����
		typeText =         (int) 0x04000, // typeText and typeBinary are
		typeBinary =       (int) 0x08000, // used in derived classes only
		osNoBuffer =       (int) 0x10000, ///< �ϲ㴦���壬��ʱ��д��ƫ�ƺͳ��ȶ���Ҫ��ҳ���С���롣
		osWriteThrough =   (int) 0x20000,
		osRandomAccess =   (int) 0x40000,
		osSequentialScan = (int) 0x80000,
	};

	/// �ļ�����
	enum Attribute {
		normal =    0x00,
		readOnly =  0x01,
		hidden =    0x02,
		system =    0x04,
		volume =    0x08,
		directory = 0x10,
		archive =   0x20
	};

	/// �ļ���λ�ο�λ��
	enum SeekPosition
	{
		begin = 0x0,
		current = 0x1,
		end = 0x2 
	};

	/// ���캯����
	CFile();

	/// ����������������ݻ��廹��Ч���ᱻ�ͷš�
	virtual ~CFile();

	/// ���ļ����򿪺��ļ�ָ��ƫ����0��������modeAppend��־�򿪵��ļ��ļ�ָ�����ļ�ĩβ��
	/// \param [in] pFileName �ļ�����
	/// \param [in] dwFlags �򿪱�־��Ĭ��ΪmodeReadWrite��
	/// \retval true  �򿪳ɹ�
	/// \retval false  ��ʧ�ܣ��ļ������ڻ��޷��򿪡�
	virtual bool Open(const char* pFileName, unsigned int dwFlags = modeReadWrite);

	/// �ر��ļ���
	virtual void Close();

	/// װ�����ݣ�������ļ�����һ����С�Ļ��壬���ļ����ݶ����û��壬���ػ���ָ�롣
	/// ��UnLoad����һ���ṩ����
	/// \param [in] pFileName �ļ�����
	/// \retval NULL  װ��ʧ��
	/// \retval !NULL  ���ݻ���ָ�롣
	unsigned char * Load(const char* pFileName);

	/// �ͷ����ݻ���
	void UnLoad();

	/// ���ļ����ݡ����������ļ�ָ���Զ��ۼӡ�
	/// \param [out] pBuffer ���ݻ����ָ�롣
	/// \param [in] dwCount Ҫ�������ֽ���
	/// \retval >0  �������ֽ���
	/// \retval <=0 ��ʧ��
	virtual unsigned int Read(void *pBuffer, unsigned int dwCount);

	/// д�ļ����ݡ����������ļ�ָ���Զ��ۼӡ�
	/// \param [out] pBuffer ���ݻ����ָ�롣
	/// \param [in] dwCount Ҫд����ֽ���
	/// \retval >0  д����ֽ���
	/// \retval <=0 дʧ��
	virtual unsigned int Write(void *pBuffer, unsigned int dwCount);

	/// ͬ���ļ��ײ㻺�壬��д��������ã�ȷ��д��������Ѿ���������ϵͳ��
	virtual void Flush();

	/// �ļ���λ��
	/// \param [in] lOff ƫ�������ֽ�Ϊ��λ��
	/// \param [in] nFrom ƫ�����λ�ã����õ���ƫ��ΪlOff+nFrom��
	/// \return ƫ�ƺ��ļ���ָ��λ�á�
	virtual unsigned int Seek(long lOff, unsigned int nFrom);

	/// ���ص�ǰ���ļ�ָ��λ��
	virtual unsigned int GetPosition();

	/// �����ļ�����
	virtual unsigned int GetLength();

	/// ���ı��ļ���ǰƫ�ƴ���ȡһ���ַ��������������ļ�ָ���Զ��ۼӡ�
	/// \param [out] s ���ݻ��塣
	/// \param [in] size ��Ҫ��ȡ���ַ������ȡ�
	/// \retval NULL  ��ȡʧ��
	/// \retval !NULL  �ַ���ָ�롣
	virtual char * Gets(char *s, int size);

	/// ���ı��ļ���ǰƫ�ƴ�д��һ���ַ�����д�������ļ�ָ���Զ��ۼӡ�
	/// \param [in] s ���ݻ��塣
	/// \return  ʵ��д���ַ������ȡ�
	virtual int Puts(char *s);

	/// �ж��ļ��Ƿ��
	virtual bool IsOpened();

	/// �������ļ�
	/// \param oldName �ɵ��ļ���
	/// \param oldName �µ��ļ���
	static bool Rename(const char* oldName, const char* newName);

	/// ɾ���ļ�
	/// \param fileName �ļ���
	static bool Remove(const char* fileName);

	/// ����Ŀ¼
	/// \param Ŀ¼��
	static bool MakeDirectory(const char* dirName);

	/// ɾ��Ŀ¼
	/// \param Ŀ¼��
	static bool RemoveDirectory(const char* dirName);

	/// �ļ�ϵͳͳ��
	/// \param path ����·������һ���Ƿ�����Ŀ¼��
	/// \param userFreeBytes Ŀ¼�����ļ�ϵͳ��������û���ʣ��ռ��ֽ���
	/// \param totalBytes Ŀ¼�����ļ�ϵͳ�ܵ��ֽ���
	/// \param totalFreeBytes Ŀ¼�����ļ�ϵͳ�ܵ�ʣ��ռ��ֽ��������ʹ����Ӳ��
	///        �ռ���userFreeBytes���ܻ��totalFreeBytesС
	static bool StatFS(const char* path,
		uint64* userFreeBytes,
		uint64* totalBytes,
		uint64* totalFreeBytes);

protected:
	FILE *m_pFile;				///< ��׼����ļ��ṹָ�롣
	unsigned char *m_pBuffer;	///< �ļ����ݻ��壬Load��UnLoad�ӿ�ʹ�á�
	unsigned int m_dwLength;	///< �ļ����ȡ�
	FSOperations* m_opt;		///< �ļ�ϵͳ������
};


/// \brief �ļ������֧࣬��'*','?'ͨ�������
class CFileFind
{
public:
	/// �ļ�ϵͳ���ҽ��
	struct Info
	{
		unsigned    attrib;
		time_t      time_create;    /* -1 for FAT file systems */
		time_t      time_access;    /* -1 for FAT file systems */
		time_t      time_write;
		size_t      size;
		char        name[260];
	};

public:
	CFileFind();
	virtual ~CFileFind();
	virtual bool findFile(const char* fileName);
	virtual bool findNextFile();
	virtual void close();
	unsigned int getLength();
	std::string getFileName();
	std::string getFilePath();
	bool isReadOnly();
	bool isDirectory();
	bool isHidden();
	bool isNormal();

protected:
	long m_handle;
	Info m_fileInfo;
	FSOperations* m_opt;		///< �ļ�ϵͳ������
	std::string m_path;			///< ����·����
};

/// �ļ�ϵͳ����������
struct FSOperations
{
	FILE* (*fopen)(const char *, const char *);
	int (*fclose)(FILE *);
	size_t (*fread)(void *, size_t, size_t, FILE *);
	size_t (*fwrite)(const void *, size_t, size_t, FILE *);
	int (*fflush)(FILE *);
	int (*fseek)(FILE *, long, int);
	long (*ftell)(FILE *);
	char * (*fgets)(char *, int, FILE *);
	int (*fputs)(const char *, FILE *);
	int (*rename)(const char *oldname, const char *newname);
	int (*remove)(const char *path);
	long (*findfirst)(const char *, void *);
	int (*findnext)(long, void * );
	int (*findclose)(long);
	int (*mkdir)( const char *dirname);
	int (*rmdir)( const char *dirname);
	int (*statfs)( const char *path, uint64* userFreeBytes, uint64* totalBytes, uint64* totalFreeBytes);
};

/// Ϊ�˼����������ں��ļ�ϵͳ��֧�����ù��ӽӿڣ�ͨ����װ��ģ��ʵ����Щ�ӿڡ�
/// \param path ƥ���·��
/// \param opts �ļ�ϵͳ�ļ�������������0��ʾȡ������
void hookFS(const char* path, const FSOperations* opts);

#endif //__FILE_H__

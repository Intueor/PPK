//== ВКЛЮЧЕНИЯ.
#include "logger.h"
#ifndef WIN32
#include <unistd.h>
#endif

//== МАКРОСЫ.
#define LOG_FILE_EXT		".log"
#ifndef WIN32
#define MSleep(val)         usleep(val * 1000)
#else
#define MSleep(val)         Sleep(val)
#endif
#define _LOGTimeFormat      "%Y-%m-%d %X"
#ifdef WIN32
#define _LOGMsFormat        "%03ld"
#else
#define _LOGMsFormat        "%06ld"
#endif

//== ФУНКЦИИ КЛАССОВ.
//== Класс логгера.
// Конструктор.
#ifdef WIN32
// Замена аналогичной функции на Linux`e.
int Logger::gettimeofday(timeval* p_tp, struct p_timezone*)
{
	SYSTEMTIME system_time;
	FILETIME file_time;
	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	p_tp->tv_usec = (long)(system_time.wMilliseconds);
	return 0;
}
#endif

// Конструктор.
Logger::Logger(const std::string& r_strLogPath, const std::string& r_strLogName, int iLogLevel) : strLogName(r_strLogName), _iLogLevel(iLogLevel)
{
	oLogFileStream.open(r_strLogPath + r_strLogName + LOG_FILE_EXT, std::ios_base::in|std::ios_base::out|std::ios_base::trunc);
}

// Деструктор.
Logger::~Logger() { oLogFileStream.close(); }

// Отправка сообщения в лог.
void Logger::SendMsg(LogCat eLogCat, const std::string& r_strMsg, int uchLevel)
{
	if(uchLevel <= _iLogLevel)
	{
		std::string strCategory;
		switch(eLogCat)
		{
			case LogCat::I:
				strCategory = ": <I> ";
				break;
			case LogCat::W:
				strCategory = ": <W> ";
				break;
			case LogCat::E:
				strCategory = ": <E> ";
				break;
		}
		oLogTimeNow = time(0);
		gettimeofday(&oLogTimeval, NULL);
		strftime(m_chLogBuf, sizeof(m_chLogBuf), _LOGTimeFormat, localtime(&oLogTimeNow));
		sprintf(m_chLogMSBuf, _LOGMsFormat, oLogTimeval.tv_usec);
		oLogFileStream << m_chLogBuf << " " << m_chLogMSBuf << " " << strCategory << r_strMsg << "\n";
		oLogFileStream.flush();
		std::cout << m_chLogBuf << " " << m_chLogMSBuf << " " << strCategory << r_strMsg << std::endl;
	}
}

// Изменение текущего уровня логирования.
void Logger::ChangeLogLevel(uint8_t iLogLevel) { _iLogLevel = iLogLevel; }

// Получение текущего уровня логирования.
int Logger::LogLevel() const { return _iLogLevel; }

//== Класс потокобезопасного логгера.
// Конструктор.
TLogger::TLogger(const std::string& r_strLogPath, const std::string& r_strLogName, int iLogLevel) : Logger(r_strLogPath, r_strLogName, iLogLevel), _p_ptLogMutex(&ptLogMutex) {}

// Потокобезопасная отправка сообщения в лог.
void TLogger::SendMsg(LogCat eLogCat, const std::string& r_strMsg, int uchLevel)
{
	MSleep(0);
	pthread_mutex_lock(_p_ptLogMutex);
	Logger::SendMsg(eLogCat, r_strMsg, uchLevel);
	pthread_mutex_unlock(_p_ptLogMutex);
}

// Установка стороннего мьютекса или сброс на внутренний при nullptr.
void TLogger::SetMutex(pthread_mutex_t* p_ptLogMutex)
{
	if(p_ptLogMutex == nullptr) _p_ptLogMutex = &ptLogMutex;
	else _p_ptLogMutex = p_ptLogMutex;
}

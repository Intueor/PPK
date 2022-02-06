#ifndef LOGGER_H
#define LOGGER_H

//== ВКЛЮЧЕНИЯ.
#include <fstream>
#include <iostream>
#ifndef WIN32
#include <sys/time.h>
#include <pthread.h>
#else
#include <Windows.h>
#include <stdint.h>
#include <time.h>
#define	HAVE_STRUCT_TIMESPEC
#include "./pthread/include/pthread.h"
#endif
#include <sstream>

//== МАКРОСЫ.
//! \file logger.h
#define Log(p_lgr, cat, msg, ...)		p_lgr->SendMsg(cat, msg, ##__VA_ARGS__) ///< Логирование по указателю, ... - уровень логирования.
#define LogS(p_lgr, cat, msg, ...)		{ std::stringstream ss; ss << msg; p_lgr->SendMsg(cat, ss.str(), ##__VA_ARGS__); } ///< Логирование по указателю через строковый поток, ... - уровень логирования.

/// Перечисление типов сообщений (Инфо, Предупреждение, Ошибка).
enum LogCat {I, W, E};

//== КЛАССЫ.
/// Класс логгера.
class Logger
{
private:
#ifdef WIN32
	/// Замена аналогичной функции на Linux`e.
	int gettimeofday(timeval* p_tp, struct p_timezone*);
													///< \param[in,out] p_tp Указатель на структуру для заполнения.
													///< \return Успех или нет (0/-1).
#endif

public:
	/// Конструктор.
	Logger(const std::string& r_strLogPath, const std::string& r_strLogName, int iLogLevel = 0);
													///< \param[in] r_strLogPath Ссылка на строку пути.
													///< \param[in] r_strLogName Ссылка на строку имени.
													///< \param[in] iLogLevel Изначальный уровень логирования.
	/// Деструктор.
	~Logger();
	/// Отправка сообщения в лог.
	void SendMsg(LogCat eLogCat, const std::string& r_strMsg, int uchLevel = 0);
													///< \param[in] eLogCat Тип сообщения.
													///< \param[in] r_strMsg Ссылка на строку сообщения.
													///< \param[in] uchLevel Уровень сообщения.
	/// Изменение текущего уровня логирования.
	void ChangeLogLevel(uint8_t iLogLevel);
													///< \param[in] iLogLevel Уровень логирования.
	/// Получение текущего уровня логирования.
	int LogLevel() const;
													///< \return Уровень логирования.

private:
	std::fstream oLogFileStream; ///< Файловый поток.
	char m_chLogBuf[80]; ///< Буфер текста времени.
	char m_chLogMSBuf[8]; ///< Буфер текста миллисекунд.
	time_t oLogTimeNow; ///< Текущее время.
	timeval oLogTimeval; ///< Миллисекунды работы лога.
	std::string strLogName; ///< Имя лога.
	int _iLogLevel; ///< Уровень логирования.
};

#endif // LOGGER_H

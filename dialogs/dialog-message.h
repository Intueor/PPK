#ifndef DIALOGMESSAGE_H
#define DIALOGMESSAGE_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogMessage; }

//== ПЕРЕЧИСЛЕНИЯ.
/// Типы сообщений (Инфо, Предупреждение, Ошибка).
enum class MsgCat {I, W, E};

//== КЛАССЫ.
/// Класс диалога "Сообщение".
class DialogMessage : public QDialog
{
	Q_OBJECT
	using QDialog::exec; ///< Правка для совместимости с Clang.

private:
	Ui::DialogMessage* p_UI; ///< Указатель на объект пользовательского интерфейса.

public:
	/// Конструктор.
	explicit DialogMessage(QWidget* p_WidgetParent = nullptr);
									///< \param[in] p_WidgetParent Указатель на родительский виджет.
	/// Деструктор.
	~DialogMessage();

public Q_SLOTS:
	/// Выполнение диалога с категорией и заданным сообщением в строке.
	void exec(MsgCat eMsgCat, const QString& r_strMsg);
									///< \param[in] eMsgCat Категория сообщения.
									///< \param[in] r_strMsg Ссылка на строку сообщения.
	/// Выполнение диалога с категорией и заданным сообщением во временной строке.
	void exec(MsgCat eMsgCat, const QString&& r_strMsg);
									///< \param[in] eMsgCat Категория сообщения.
									///< \param[in] r_strMsg Ссылка на временную строку сообщения.
};

#endif // DIALOGMESSAGE_H

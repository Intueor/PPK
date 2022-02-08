#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogSettings; }

//== КЛАССЫ.
/// Класс диалога "Настройки".
class DialogSettings : public QDialog
{
	Q_OBJECT

private:
	Ui::DialogSettings* p_UI; ///< Указатель на объект пользовательского интерфейса.

public:
	/// Конструктор.
	explicit DialogSettings(QWidget* p_WidgetParent = nullptr);
									///< \param[in] p_WidgetParent Указатель на родительский виджет.
	/// Деструктор.
	~DialogSettings();
};

#endif // DIALOGSETTINGS_H

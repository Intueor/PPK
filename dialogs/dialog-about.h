#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

//== ВКЛЮЧЕНИЯ.
#include <QDialog>

//== ПРОСТРАНСТВА ИМЁН.
namespace Ui { class DialogAbout; }

//== КЛАССЫ.
/// Класс диалога "О программе".
class DialogAbout : public QDialog
{
	Q_OBJECT

private:
	Ui::DialogAbout* p_UI = nullptr; ///< Указатель на объект пользовательского интерфейса.

public:
	/// Конструктор.
	explicit DialogAbout(QWidget* p_WidgetParent = nullptr);
									///< \param[in] p_WidgetParent Указатель на родительский виджет.
	/// Деструктор.
	~DialogAbout();
};

#endif // DIALOGABOUT_H

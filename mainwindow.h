#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//== ПРОСТРАНСТВА ИМЁН.
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//== КЛАССЫ.
/// Класс главного окна.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/// Конструктор.
	MainWindow(QWidget* p_Parent = nullptr);
										///< \param[in] p_Parent Указатель на родительский виджет.
	/// Деструктор.
	~MainWindow();

private:
	Ui::MainWindow* p_UI; ///< Указатель на объект пользовательского интерфейса.
};
#endif // MAINWINDOW_H

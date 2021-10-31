#include "mainwindow.h"
#include "ui_mainwindow.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс главного окна.
// Конструктор.
MainWindow::MainWindow(QWidget* p_Parent) : QMainWindow(p_Parent), p_UI(new Ui::MainWindow)
{
	p_UI->setupUi(this);
}

// Деструктор.
MainWindow::~MainWindow()
{
	delete p_UI;
}


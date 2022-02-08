//== ВКЛЮЧЕНИЯ.
#include "dialog-about.h"
#include "ui_dialog-about.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс диалога "О программе".
// Конструктор.
DialogAbout::DialogAbout(QWidget *p_WidgetParent) : QDialog(p_WidgetParent), p_UI(new Ui::DialogAbout) { p_UI->setupUi(this); }

// Деструктор.
DialogAbout::~DialogAbout() { delete p_UI; }

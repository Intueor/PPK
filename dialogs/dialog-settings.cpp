//== ВКЛЮЧЕНИЯ.
#include "dialog-settings.h"
#include "ui_dialog-settings.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс диалога "О программе".
// Конструктор.
DialogSettings::DialogSettings(QWidget* p_WidgetParent) : QDialog(p_WidgetParent), p_UI(new Ui::DialogSettings) { p_UI->setupUi(this); }

// Деструктор.
DialogSettings::~DialogSettings() { delete p_UI; }



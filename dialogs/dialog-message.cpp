//== ВКЛЮЧЕНИЯ.
#include "dialog-message.h"
#include "ui_dialog-message.h"

//== ФУНКЦИИ КЛАССОВ.
//== Класс диалога "Сообщение".
// Конструктор.
DialogMessage::DialogMessage(QWidget* p_WidgetParent) : QDialog(p_WidgetParent), p_UI(new Ui::DialogMessage) { p_UI->setupUi(this); }

// Деструктор.
DialogMessage::~DialogMessage() { delete p_UI; }

// Выполнение диалога с категорией и заданным сообщением в строке.
void DialogMessage::exec(MsgCat eMsgCat, const QString& r_strMsg)
{
	switch(eMsgCat)
	{
		case MsgCat::I:
			p_UI->labelType->setText("ИНФОРМАЦИЯ");
			break;
		case MsgCat::W:
			p_UI->labelType->setText("ПРЕДУПРЕЖДЕНИЕ");
			break;
		case MsgCat::E:
			p_UI->labelType->setText("ОШИБКА");
			break;
	}
	p_UI->textBrowserMsg->setHtml(r_strMsg);
	p_UI->textBrowserMsg->setAlignment(Qt::AlignCenter);
	QDialog::exec();
}

// Выполнение диалога с категорией и заданным сообщением во временной строке.
void DialogMessage::exec(MsgCat eMsgCat, const QString&& r_strMsg) { exec(eMsgCat, r_strMsg); }

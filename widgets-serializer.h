#ifndef WIDGETSERIALIZER_H
#define WIDGETSERIALIZER_H

//== ВКЛЮЧЕНИЯ.
#include "mtableview.h"
#include <QSettings>
#include <QList>
#include <QSplitter>
#include <QRadioButton>
#include <QLineEdit>
#include <QWidget>
#include <QSpinBox>
#include <QMap>
#include <QSet>
#include <QTabWidget>
#include <QHeaderView>

//== МАКРОСЫ.
#define MIN_AUTO_SIZE_OF_TABLE_H_LAST_SECTION	5
#define MkWidgetName(widget,suffix)				widget+"-"+suffix
#define MkChildName(parent,widget,suffix)		parent+"-"+MkWidgetName(widget,suffix)

//== КЛАССЫ.
/// Класс сериализатора виджетов.
class WidgetsSerializer
{
private:
	QSettings& _r_Settings; ///< Ссылка на объект установок для чтения и записи состояний.
	QVector<QWidget*> _vpWidgetsExcluded; ///< Вектор с указателями на исключаемые из сериализации виджеты.
	// Обрабатываемые типы.
	QMap<QString, QSet<QSplitter*>> mpWNameToSplitters; ///< Мап имён виджетов к сету указателей на разделители.
	QMap<QString, QSet<QRadioButton*>> mpWNameToRadioButtons; ///< Мап имён виджетов к сету указателей на переключатели.
	QMap<QString, QSet<QLineEdit*>> mpWNameToLineEdits; ///< Мап имён виджетов к сету указателей на редактируемые строки.
	QMap<QString, QSet<QSpinBox*>> mpWNameToSpinBoxes; ///< Мап имён виджетов к сету указателей на спинбоксы.
	QMap<QString, QSet<QTabWidget*>> mpWNameToTabWidgets; ///< Мап имён виджетов к сету указателей на картотеки.
	QMap<QString, QSet<MTableView*>> mpWNameToMTableViews; ///< Мап имён виджетов к сету указателей на модифицированные таблицы.
	// Другие требуемые типы...

private:
	/// Удаление виджетов из листа по типам родителей.
	template <typename T>
	void EraseByParentTypes(QList<T>& r_lpWidgets, const QList<QString>& r_lTypes)
										//! \param[in,out] r_lpWidgets Ссылка на лист зарегистрированных виджетов данного типа.
										//! \param[in] r_lTypes Ссылка на лист с именами типов для вычистки.
	{
		auto itW = r_lpWidgets.begin();
		while(itW != r_lpWidgets.end())
		{
			bool bEr  = false;
			for(QString strType : r_lTypes)
			{
				if(strType == (*itW)->parentWidget()->metaObject()->className())
				{
					itW = r_lpWidgets.erase(itW);
					bEr = true;
					break;
				}
			}
			if(!bEr) itW++;
		}
	}

	/// Создание сета детей виджета по типу.
	template <typename T>
	QSet<T> MakeChildrenSetOfType(const QWidget& r_Widget) const
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \return Сет детей по типу.
	{
		QList<T> lpResult = r_Widget.template findChildren<T>();
		return {lpResult.begin(), lpResult.end()};
	}

	/// Создание сета детей виджета по типу с исключением листа неприемлемых родителей.
	template <typename T>
	QSet<T> MakeChildrenSetOfType(const QWidget& r_Widget, const QList<QString>&& r_lTypes)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_lTypes Ссылка на временный лист с именами типов родителей, детьми которых не могут являться члены итогового списка.
										//! \return Сет детей по типу с исключением листа неприемлемых родителей.
	{
		QList<T> lpResult = r_Widget.template findChildren<T>();
		EraseByParentTypes<T>(lpResult, r_lTypes);
		return {lpResult.begin(), lpResult.end()};
	}

public:
	/// Конструктор.
	WidgetsSerializer(QSettings& r_Settings)
										//! \param[in] r_Settings Ссылка на настройки.
		: _r_Settings(r_Settings) {}

	/// Регистрация дочерних элементов виджета.
	template <typename T>
	void RegisterChildren(const T& r_Widget);
										///< \param[in] r_Widget Ссылка на виджет для работы.
	/// Регистрация дочерних элементов виджета с исключениями.
	template <typename T>
	void RegisterChildren(const T& r_Widget, const QVector<QWidget*>& r_vpWidgetsExcluded);
										///< \param[in] r_Widget Ссылка на виджет для работы.
										///< \param[in] r_vpWidgetsExcluded Ссылка на вектор с указателями на исключаемые из регистрации виджеты.
	/// Регистрация дочерних элементов виджета с исключениями из временного объекта.
	template <typename T>
	void RegisterChildren(const T& r_Widget, const QVector<QWidget*>&& r_vpWidgetsExcluded)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_vpWidgetsExcluded Ссылка на временный вектор с указателями на исключаемые из регистрации виджеты.
	{ RegisterChildren(r_Widget, r_vpWidgetsExcluded); }

	/// Регистрация вектора элементов.
	template <typename T>
	void Register(const T& r_Widget, const QVector<QWidget*>& r_vpWidgets)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_vpWidgets Ссылка на вектор с указателями на виджеты для регистрации.
	{
		QString strWName = r_Widget.objectName();
		for(QWidget* p_WidgetToReg : r_vpWidgets)
		{
			QString strType = p_WidgetToReg->metaObject()->className();
			// Обрабатываемые типы.
			if(strType == "QSplitter") mpWNameToSplitters[strWName].insert(static_cast<QSplitter*>(r_Widget)); // Разделители.
			else if(strType == "QRadioButton") mpWNameToRadioButtons[strWName].insert(static_cast<QRadioButton*>(r_Widget)); // Переключатели.
			else if(strType == "QLineEdit") mpWNameToLineEdits[strWName].insert(static_cast<QLineEdit*>(r_Widget)); // Строчные редакторы.
			else if(strType == "QSpinBox") mpWNameToSpinBoxes[strWName].insert(static_cast<QSpinBox*>(r_Widget)); // Спинбоксы.
			else if(strType == "QTabWidget") mpWNameToTabWidgets[strWName].insert(static_cast<QTabWidget*>(r_Widget)); // Картотеки.
			else if(strType == "MTableView") mpWNameToMTableViews[strWName].insert(static_cast<MTableView*>(r_Widget)); // Таблицы.
			// Другие требуемые типы...
		}
	}

	/// Регистрация вектора элементов из временного объекта.
	template <typename T>
	void Register(const T& r_Widget, const QVector<QWidget*>&& r_vpWidgets)
										//! \param[in] r_Widget Ссылка на виджет для работы.
										//! \param[in] r_vpWidgets Ссылка на временный вектор с указателями на виджеты для регистрации.
	{ Register(r_Widget, r_vpWidgets); }

	/// Загрузка и установка состояний из настроек.
	template <typename T>
	void LoadStates(T& r_Widget, bool bIncludeMainGeometry = true);
										///< \param[in,out] r_Widget Ссылка на виджет для работы.
										///< \param[in] bIncludeMainGeometry true - включить в загрузку геометрию основного виджета.
	/// Запись состояний в настройки.
	template <typename T>
	void SaveStates(const T& r_Widget, bool bIncludeMainGeometry = true);
										///< \param[in] r_Widget Ссылка на виджет для работы.
										///< \param[in] bIncludeMainGeometry true - включить в сохранение геометрию основного виджета.
};

//== ИМПЛЕМЕНТАЦИЯ ШАБЛОННЫХ ФУНКЦИЙ КЛАССОВ.
//== Класс сериализатора виджетов.
// Регистрация дочерних элементов виджета.
template <typename T>
void WidgetsSerializer::RegisterChildren(const T& r_Widget)
{
	QString strWName = r_Widget.objectName();
	// Обрабатываемые типы.
	mpWNameToSplitters[strWName] += MakeChildrenSetOfType<QSplitter*>(r_Widget); // Разделители.
	mpWNameToRadioButtons[strWName] += MakeChildrenSetOfType<QRadioButton*>(r_Widget); // Переключатели.
	mpWNameToLineEdits[strWName] += MakeChildrenSetOfType<QLineEdit*>(r_Widget, {"QSpinBox"}); // Строчные редакторы, кроме детей QSpinBox.
	mpWNameToSpinBoxes[strWName] += MakeChildrenSetOfType<QSpinBox*>(r_Widget); // Спинбоксы.
	mpWNameToTabWidgets[strWName] += MakeChildrenSetOfType<QTabWidget*>(r_Widget); // Картотеки.
	mpWNameToMTableViews[strWName] += MakeChildrenSetOfType<MTableView*>(r_Widget); // Таблицы.
	// Другие требуемые типы...
}

// Регистрация дочерних элементов виджета с исключениями.
template <typename T>
void WidgetsSerializer::RegisterChildren(const T& r_Widget, const QVector<QWidget*>& r_vpWidgetsExcluded)
{
	QString strWName = r_Widget.objectName();
	RegisterChildren(r_Widget);
	for(QWidget* p_QWidget : r_vpWidgetsExcluded)
	{
		// Обрабатываемые типы.
		mpWNameToSplitters[strWName].remove(static_cast<QSplitter*>(p_QWidget)); // Разделители.
		mpWNameToRadioButtons[strWName].remove(static_cast<QRadioButton*>(p_QWidget)); // Переключатели.
		mpWNameToLineEdits[strWName].remove(static_cast<QLineEdit*>(p_QWidget)); // Строчные редакторы.
		mpWNameToSpinBoxes[strWName].remove(static_cast<QSpinBox*>(p_QWidget)); // Спинбоксы.
		mpWNameToTabWidgets[strWName].remove(static_cast<QTabWidget*>(p_QWidget)); // Картотеки.
		mpWNameToMTableViews[strWName].remove(static_cast<MTableView*>(p_QWidget)); // Таблицы.
		// Другие требуемые типы...
	}
}

// Загрузка и установка состояний.
template <typename T>
void WidgetsSerializer::LoadStates(T& r_Widget, bool bIncludeMainGeometry)
{
	QString strWName = r_Widget.objectName();
	if(bIncludeMainGeometry) r_Widget.restoreGeometry(_r_Settings.value(MkWidgetName(strWName, "G")).toByteArray());
	QVariant oVariant;
	// Обрабатываемые типы.
	// Дочерние разделители.
	for(QSplitter* p_Splitter : mpWNameToSplitters[strWName])
	{
		p_Splitter->restoreGeometry(_r_Settings.value(MkChildName(strWName, p_Splitter->objectName(), "G")).toByteArray());
		p_Splitter->restoreState(_r_Settings.value(MkChildName(strWName, p_Splitter->objectName(), "S")).toByteArray());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_RadioButton : mpWNameToRadioButtons[strWName])
	{
		oVariant = _r_Settings.value(MkChildName(strWName, p_RadioButton->objectName(), "V"));
		if(!oVariant.isNull()) p_RadioButton->setChecked(oVariant.toBool());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_LineEdit : mpWNameToLineEdits[strWName])
	{
		oVariant = _r_Settings.value(MkChildName(strWName, p_LineEdit->objectName(), "V"));
		if(!oVariant.isNull()) p_LineEdit->setText(oVariant.toString());
	}
	// Дочерние cпинбоксы.
	for(QSpinBox* p_SpinBox : mpWNameToSpinBoxes[strWName])
	{
		oVariant = _r_Settings.value(MkChildName(strWName, p_SpinBox->objectName(), "V"));
		if(!oVariant.isNull()) p_SpinBox->setValue(oVariant.toInt());
	}
	// Дочерние картотеки.
	for(QTabWidget* p_TabWidget : mpWNameToTabWidgets[strWName])
	{
		oVariant = _r_Settings.value(MkChildName(strWName, p_TabWidget->objectName(), "V"));
		if(!oVariant.isNull()) p_TabWidget->setCurrentIndex(oVariant.toInt());
	}
	// Дочерние таблицы.
	for(MTableView* p_MTableView : mpWNameToMTableViews[strWName])
	{
		oVariant = _r_Settings.value(MkChildName(strWName, p_MTableView->objectName(), "C"));
		if(!oVariant.isNull())
		{
			QString strColls = oVariant.toString();
			QStringList strlColls = strColls.split(',', Qt::SkipEmptyParts);
			int iC = 0;
			for(const auto& r_Coll : strlColls)
			{
				p_MTableView->setColumnWidth(iC, r_Coll.toInt());
				iC++;
			}
		}
	}
	// Другие требуемые типы...
}

// Запись состояний.
template <typename T>
void WidgetsSerializer::SaveStates(const T& r_Widget, bool bIncludeMainGeometry)
{
	QString strWName = r_Widget.objectName();
	if(bIncludeMainGeometry) _r_Settings.setValue(MkWidgetName(strWName, "G"), r_Widget.saveGeometry());
	// Обрабатываемые типы.
	// Дочерние разделители.
	for(QSplitter* p_Splitter : mpWNameToSplitters[strWName])
	{
		_r_Settings.setValue(MkChildName(strWName, p_Splitter->objectName(), "G"), p_Splitter->saveGeometry());
		_r_Settings.setValue(MkChildName(strWName, p_Splitter->objectName(), "S"), p_Splitter->saveState());
	}
	// Дочерние переключатели.
	for(QRadioButton* p_RadioButton : mpWNameToRadioButtons[strWName])
	{
		_r_Settings.setValue(MkChildName(strWName, p_RadioButton->objectName(), "V"), p_RadioButton->isChecked());
	}
	// Дочерние строчные редакторы.
	for(QLineEdit* p_LineEdit : mpWNameToLineEdits[strWName])
	{
		_r_Settings.setValue(MkChildName(strWName, p_LineEdit->objectName(), "V"), p_LineEdit->text());
	}
	// Дочерние cпинбоксы.
	for(QSpinBox* p_SpinBox : mpWNameToSpinBoxes[strWName])
	{
		_r_Settings.setValue(MkChildName(strWName, p_SpinBox->objectName(), "V"), p_SpinBox->value());
	}
	// Дочерние cпинбоксы.
	for(QTabWidget* p_TabWidget : mpWNameToTabWidgets[strWName])
	{
		_r_Settings.setValue(MkChildName(strWName, p_TabWidget->objectName(), "V"), p_TabWidget->currentIndex());
	}
	// Дочерние таблицы.
	for(MTableView* p_MTableView : mpWNameToMTableViews[strWName])
	{
		int iColls = p_MTableView->model()->columnCount();
		QString strColls;
		for(int iC = 0; iC < iColls; iC++)
		{
			int iColWidth = p_MTableView->columnWidth(iC);
			if((iC == iColls - 1) && p_MTableView->horizontalHeader()->stretchLastSection()) iColWidth = MIN_AUTO_SIZE_OF_TABLE_H_LAST_SECTION;
			strColls += QString::number(iColWidth);
			strColls += ",";
		}
		if(!strColls.isEmpty()) strColls.erase(strColls.end() - 1, strColls.end());
		_r_Settings.setValue(MkChildName(strWName, p_MTableView->objectName(), "C"), strColls);
	}
	// Другие требуемые типы...
}

#endif // WIDGETSERIALIZER_H

//== ВКЛЮЧЕНИЯ.
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dialog-settings.h"
#include <QSqlError>

//== МАКРОСЫ.
#define STATUS_DELAY	3000
#define ST(m)			oLabelStatus.setText(m)
#define SM(m)			p_UI->statusbar->showMessage(m, STATUS_DELAY)

//== ПРОСТРАНСТВА ИМЁН.
using namespace rapidxml;

//== ФУНКЦИИ КЛАССОВ.
//== Класс главного окна.
// Инициализация вида и модели таблицы.
QSqlRelationalTableModel* MainWindow::InitTable(QObject* p_Parent, const QString& r_strTableName, MTableView* p_MTableView, const QString& r_strFilter, bool bCanScroll, int iColumnForSort,
												const std::map<uchar, MSqlRelationalDelegate::CustomDelegateType>* const p_mpColumnsDataTypes,
												std::vector<MTableView*>* const p_v_p_InfluencingTableViews, std::vector<ColumnRelation>* const p_vColumnsRelations,
												std::vector<MHeaderView*>* const p_v_p_MHorizontalHeaderViewsRelated)
{
	QSqlRelationalTableModel* p_QSqlRelationalTableModel = new QSqlRelationalTableModel(p_Parent);
	p_QSqlRelationalTableModel->setTable(r_strTableName);
	p_QSqlRelationalTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
	if(!r_strFilter.isEmpty()) p_QSqlRelationalTableModel->setFilter(r_strFilter);
	p_MTableView->SetCanScroll(bCanScroll);
	p_MTableView->setModel(p_QSqlRelationalTableModel);
	p_MTableView->setColumnHidden(0, true);
	if(p_vColumnsRelations)
		for(auto oColumnRelation : *p_vColumnsRelations) p_QSqlRelationalTableModel->setRelation(oColumnRelation.iColumn, oColumnRelation.r_QSqlRelation);
	p_MTableView->setItemDelegate(new MSqlRelationalDelegate(p_QSqlRelationalTableModel, p_mpColumnsDataTypes));
	p_QSqlRelationalTableModel->select();
	if(p_v_p_InfluencingTableViews)
		for(auto p_MTableViewTarget : *p_v_p_InfluencingTableViews) p_MTableViewTarget->AddRelatedTableView(p_MTableView);
	p_MTableView->SetColumnForSort(iColumnForSort);
	p_MTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	if(p_v_p_MHorizontalHeaderViewsRelated)
		for(MHeaderView* p_MHeaderView : *p_v_p_MHorizontalHeaderViewsRelated) p_MTableView->AddRelatedHorizontalHeaderView(p_MHeaderView);
	return p_QSqlRelationalTableModel;
}

// Конструктор.
MainWindow::MainWindow(QWidget* p_WidgetParent) : QMainWindow(p_WidgetParent), p_UI(new Ui::MainWindow)
{
	p_UI->setupUi(this);
	p_Settings = new QSettings(SETTINGS_NAME, QSettings::IniFormat);
	up_Logger = std::make_unique<Logger>("./", "main");
	p_UI->statusbar->addPermanentWidget(&oLabelStatus);
	// Подключение к базе данных.
	strDBName = p_Settings->value("DBName").toString();
	if(strDBName.isEmpty()) strDBName = "default.db";
	strDBPath = "./" + strDBName;
	strDBTempPath = strDBPath + strDBTempExt;
	oDbFile.setFileName(strDBPath);
	const QString strMsg = "Файл базы данных не обнаружен, создание нового.";
	if(!oDbFile.exists())
	{
		Log(up_Logger, LogCat::I, strMsg.toStdString());
		SM(strMsg);
		if(oDbFile.open(QIODevice::WriteOnly | QIODevice::Text)) oDbFile.close();
		else
		{
gFE:		Log(up_Logger, LogCat::E, "Невозможно создать файл базы данных.");
			oDialogMessage.exec(MsgCat::E, "Невозможно создать файл базы данных.<br>Приложение будет закрыто.");
			exit(EXIT_FAILURE);
		}
	}
	oDbFile.setFileName(strDBTempPath);
	oDbFile.remove();
	if(!QFile::copy(strDBPath, strDBTempPath)) goto gFE;
	setWindowTitle(strDBName + " - " + windowTitle());
	oDB = QSqlDatabase::addDatabase("QSQLITE");
	oDB.setHostName("localhost");
	oDB.setDatabaseName("./" + strDBName + strDBTempExt);
	if(!oDB.open())
	{
		LogS(up_Logger, LogCat::E, "Ошибка инициализации интерфейса базы данных: " << oDB.lastError().databaseText().toStdString());
		oDialogMessage.exec(MsgCat::E, "Сбой подключения к базе данных.<br>Приложение будет закрыто.");
		exit(EXIT_FAILURE);
	}
	//
	const QString strDayPref = "День='";
	const QString strTypePref = "Тип=";
	// Инициализация вида и модели расписания.
	std::array<MTableView*, 6> ar_p_DayTables = {p_UI->tableViewTimetablePon, p_UI->tableViewTimetableVtor, p_UI->tableViewTimetableSred,
												 p_UI->tableViewTimetableChetv, p_UI->tableViewTimetablePyatn, p_UI->tableViewTimetableSubb};
	const QStringList strlDayNames = {"понедельник", "вторник", "среда", "четверг", "пятница", "суббота"};
	auto itDayNames = strlDayNames.begin();
	const QSqlRelation oDayTablesRelation2("СеткаЗанятий", "Ключ", "Время");
	const QSqlRelation oDayTablesRelation3("Контингент", "Ключ", "Имя");
	const QSqlRelation oDayTablesRelation4("Предметы", "Ключ", "Предмет");
	const QSqlRelation oDayTablesRelation6("ДлиныЗанятий", "Ключ", "Длина");
	for(MTableView* p_DayTable : ar_p_DayTables)
	{
		QString strFilter = strDayPref + *itDayNames + "'";
		itDayNames++;
		std::vector<ColumnRelation> vColumnsRelations = {{2, oDayTablesRelation2}, {3, oDayTablesRelation3}, {4, oDayTablesRelation4}, {6, oDayTablesRelation6}};
		std::vector<MTableView*> v_p_InfluencingTableViews = {p_UI->tableViewSchedule, p_UI->tableViewPConc, p_UI->tableViewPPrep, p_UI->tableViewDisciplines, p_UI->tableViewLengths};
		std::vector<MHeaderView*> v_p_CurrentDayRelatedHeaders;
		for(auto* p_RHDayTable : ar_p_DayTables)
		{
			if(p_DayTable != p_RHDayTable) v_p_CurrentDayRelatedHeaders.push_back(static_cast<MHeaderView*>(p_RHDayTable->horizontalHeader()));
		}
		InitTable(this, "Расписание", p_DayTable, strFilter, false, 2, nullptr, &v_p_InfluencingTableViews, &vColumnsRelations, &v_p_CurrentDayRelatedHeaders);
		p_DayTable->setColumnHidden(1, true);
	}
	// Инициализация вида и модели сетки занятий.
	InitTable(this, "СеткаЗанятий", p_UI->tableViewSchedule, "", false, 1, &mpSchedColsDataTypes);
	// Инициализация вида и модели прод. занятий.
	InitTable(this, "ДлиныЗанятий", p_UI->tableViewLengths, "", false, 1, &mpLenColsDataTypes);
	// Инициализация вида и модели предметов.
	InitTable(this, "Предметы", p_UI->tableViewDisciplines, "", false);
	// Инициализация вида и модели контингента спец.
	std::vector<MHeaderView*> v_p_PPrepRelatedHeaders = {{static_cast<MHeaderView*>(p_UI->tableViewPConc->horizontalHeader())}};
	InitTable(this, "Контингент", p_UI->tableViewPPrep, strTypePref + "0", false, 0, nullptr, nullptr, nullptr, &v_p_PPrepRelatedHeaders);
	p_UI->tableViewPPrep->setColumnHidden(4, true);
	// Инициализация вида и модели контингента конц.
	std::vector<MHeaderView*> v_p_PConcRelatedHeaders = {{static_cast<MHeaderView*>(p_UI->tableViewPConc->horizontalHeader())}};
	InitTable(this, "Контингент", p_UI->tableViewPConc, strTypePref + "1", false, 0, nullptr, nullptr, nullptr, &v_p_PConcRelatedHeaders);
	p_UI->tableViewPConc->setColumnHidden(4, true);
	// Загрузка настроек.
	up_WidgetsSerializer = std::make_unique<WidgetsSerializer>(*p_Settings);
	up_WidgetsSerializer->RegisterChildren(*this);
	up_WidgetsSerializer->RegisterChildren(oDialogSettings);
	if(p_Settings->allKeys().count() == 0)
	{
		Log(up_Logger, LogCat::W, "Файл настроек не обнаружен, загрузка значений по умолчанию");
		SM("Настройки не загружены, будут созданы новые.");
	}
	else
	{
		up_WidgetsSerializer->LoadStates(*this);
		up_WidgetsSerializer->LoadStates(oDialogSettings, false);
		ApplySettingsDialogValues();
		Log(up_Logger, LogCat::I, "Файл настроек применён.", 2);
	}
	LogS(up_Logger, LogCat::I, "База данных [" << strDBName.toStdString() << "] подключена.", 2);
	// Завершение.
	ST("Готов.");
}

// Деструктор.
MainWindow::~MainWindow()
{
	ST("Отключение от базы данных...");
	oDB.close();
	oDbFile.setFileName(strDBPath);
	oDbFile.remove();
	QString strMsg;
	if(!QFile::copy(strDBTempPath, strDBPath))
	{
		strMsg = "Невозможно сохранить базу данных в файл.";
		Log(up_Logger, LogCat::E, strMsg.toStdString());
		oDialogMessage.exec(MsgCat::E, strMsg);
	}
	else LogS(up_Logger, LogCat::I, "База данных [" << strDBName.toStdString() << "] сохранена.", 2);
	oDbFile.setFileName(strDBTempPath);
	oDbFile.remove();
	p_Settings->setValue("DBName", strDBName);
	up_WidgetsSerializer->SaveStates(*this);
	up_WidgetsSerializer->SaveStates(oDialogSettings, false);
	ST("Сохранение настроек...");
	p_Settings->sync();
	switch(p_Settings->status())
	{
		case QSettings::NoError:
			Log(up_Logger, LogCat::I, "Настройки сохранены в файл.", 2);
			break;
		case QSettings::AccessError:
			strMsg = "Ошибка доступа к файлу настроек.";
			Log(up_Logger, LogCat::E, strMsg.toStdString());
			oDialogMessage.exec(MsgCat::I, strMsg);
			break;
		case QSettings::FormatError:
			strMsg = "Ошибка формата файла настроек.";
			Log(up_Logger, LogCat::E, strMsg.toStdString());
			oDialogMessage.exec(MsgCat::I, strMsg);
			break;
	}
	Log(up_Logger, LogCat::I, "Завершение работы.", 2);
	ST("Выход...");
	delete p_Settings;
	delete p_UI;
}

// Приминение значений из диалога настроек.
void MainWindow::ApplySettingsDialogValues()
{
	up_Logger->ChangeLogLevel(oDialogSettings.p_UI->spinBoxLogLevel->value());
	LogS(up_Logger, LogCat::I, "Уровень логирования: " << up_Logger->LogLevel(), 1);
}

// Обработка пункта "О программе".
void MainWindow::on_actionAbout_triggered() { oDialogAbout.exec(); }

// Обработка пункта "Настройки".
void MainWindow::on_actionSettings_triggered()
{
	if(CancelableDialogExec(oDialogSettings)) ApplySettingsDialogValues();
}


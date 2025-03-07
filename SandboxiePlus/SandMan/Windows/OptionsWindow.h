#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OptionsWindow.h"
#include "SbiePlusAPI.h"
#include "../../MiscHelpers/Common/SettingsWidgets.h"

//////////////////////////////////////////////////////////////////////////
// COptionsWindow

class COptionsWindow : public CConfigDialog
{
	Q_OBJECT

public:
	COptionsWindow(const QSharedPointer<CSbieIni>& pBox, const QString& Name, QWidget *parent = Q_NULLPTR);
	~COptionsWindow();

	virtual void accept() {}
	virtual void reject();

	QStringList		GetCurrentGroups();
	QSet<QString>	GetPrograms() { return  m_Programs; }

	static void		TriggerPathReload();

	enum EntryTypes {
		eUndefined = 0,
		eProcess,
		ePath,
		eText,
		eTemplate
	};

signals:
	//void OptionsChanged();
	void Closed();

public slots:
	void ok();
	void apply();

private slots:

	//void OnWithTemplates();

	void OnOptChanged();

	void OnPickColor();
	void OnColorSlider(int value);

	void OnBoxTypChanged();
	void UpdateBoxType();

	void OnBrowsePath();
	void OnAddCommand();
	void OnDelCommand();
	void OnRunChanged() { m_GeneralChanged = true;  OnOptChanged(); }

	void OnVmRead();

	void OnAddGroup();
	void OnAddProg();
	void OnDelProg();
	void OnShowGroupTmpl()			{ LoadGroupsTmpl(true); }
	void OnGroupsChanged(QTreeWidgetItem* pItem, int Index) { m_GroupsChanged = true;  OnOptChanged(); }

	void OnForceProg();
	void OnForceBrowse();
	void OnForceDir();
	void OnDelForce();
	void OnShowForceTmpl()			{ LoadForcedTmpl(true); }
	void OnForcedChanged(QTreeWidgetItem* pItem, int Index);

	void OnBreakoutProg();
	void OnBreakoutBrowse();
	void OnBreakoutDir();
	void OnDelBreakout();
	void OnShowBreakoutTmpl()		{ LoadBreakoutTmpl(true); }

	void OnAddLingering();
	void OnDelStopProg();
	void OnShowStopTmpl()			{ LoadStopTmpl(true); }	
	void OnStopChanged(QTreeWidgetItem* pItem, int Index) { m_StopChanged = true;  OnOptChanged(); }
	void OnAddLeader();
	void OnDelLeader();
	void OnShowLeaderTmpl()			{ LoadLeaderTmpl(true); }	

	void OnRestrictStart();
	void OnAddStartProg();
	void OnDelStartProg();
	//void OnShowStartTmpl() 			{ LoadStartTmpl(true); }
	void OnStartChanged(QTreeWidgetItem* pItem, int Index);

	// net
	void OnINetItemDoubleClicked(QTreeWidgetItem* pItem, int Column);
	void OnINetSelectionChanged()	{ CloseINetEdit(); OnOptChanged();}
	void OnINetChanged(QTreeWidgetItem* pItem, int Column);
	void OnBlockINet();
	void OnAddINetProg();
	void OnDelINetProg();

	void OnNetFwItemDoubleClicked(QTreeWidgetItem* pItem, int Column);
	void OnNetFwSelectionChanged()	{ CloseNetFwEdit(); OnOptChanged();}
	void OnNetFwChanged(QTreeWidgetItem* pItem, int Column);
	void OnAddNetFwRule();
	void OnDelNetFwRule();
	void OnShowNetFwTmpl()			{ LoadNetFwRulesTmpl(true); }

	void OnTestNetFwRule();
	void OnClearNetFwTest();
	//
	
	// access
	//void OnAccessItemClicked(QTreeWidgetItem* pItem, int Column);
	void OnAccessItemDoubleClicked(QTreeWidgetItem* pItem, int Column);
	void OnAccessSelectionChanged() { CloseAccessEdit(); OnOptChanged();}
	void OnAccessChanged(QTreeWidgetItem* pItem, int Column);

	void OnAddFile()				{ AddAccessEntry(eFile, eOpen, "", ""); m_AccessChanged = true; OnOptChanged(); }
	void OnBrowseFile();
	void OnBrowseFolder();
	void OnDelFile()				{ DeleteAccessEntry(ui.treeFiles->currentItem()); m_AccessChanged = true; OnOptChanged(); }
	void OnShowFilesTmpl()			{ LoadAccessListTmpl(eFile, ui.chkShowFilesTmpl->isChecked(), true); }
	void OnAddKey()					{ AddAccessEntry(eKey, eOpen, "", ""); m_AccessChanged = true; OnOptChanged(); }
	void OnDelKey()					{ DeleteAccessEntry(ui.treeKeys->currentItem()); m_AccessChanged = true; OnOptChanged(); }
	void OnShowKeysTmpl()			{ LoadAccessListTmpl(eKey, ui.chkShowKeysTmpl->isChecked(), true); }
	void OnAddIPC()					{ AddAccessEntry(eIPC, eOpen, "", ""); m_AccessChanged = true; OnOptChanged(); }
	void OnDelIPC()					{ DeleteAccessEntry(ui.treeIPC->currentItem()); m_AccessChanged = true; OnOptChanged(); }
	void OnShowIPCTmpl()			{ LoadAccessListTmpl(eIPC, ui.chkShowIPCTmpl->isChecked(), true); }
	void OnAddWnd()					{ AddAccessEntry(eWnd, eOpen, "", ""); m_AccessChanged = true; OnOptChanged(); }
	void OnDelWnd()					{ DeleteAccessEntry(ui.treeWnd->currentItem()); m_AccessChanged = true; OnOptChanged(); }
	void OnShowWndTmpl()			{ LoadAccessListTmpl(eWnd, ui.chkShowWndTmpl->isChecked(), true); }
	void OnAddCOM()					{ AddAccessEntry(eCOM, eOpen, "", ""); m_AccessChanged = true; OnOptChanged(); }
	void OnDelCOM()					{ DeleteAccessEntry(ui.treeCOM->currentItem()); m_AccessChanged = true; OnOptChanged(); }
	void OnShowCOMTmpl()			{ LoadAccessListTmpl(eCOM, ui.chkShowCOMTmpl->isChecked(), true); }
	//void OnDelAccess()			{ DeleteAccessEntry(ui.treeAccess->currentItem()); m_AccessChanged = true; OnOptChanged(); }
	//void OnShowAccessTmpl()			{ LoadAccessListTmpl(true); }
	//

	void OnAddRecFolder();
	void OnAddRecIgnore();
	void OnAddRecIgnoreExt();
	void OnDelRecEntry();
	void OnDelRecIgnoreEntry();
	void OnShowRecoveryTmpl()		{ LoadRecoveryListTmpl(true); }
	void OnShowRecIgnoreTmpl()		{ LoadRecIgnoreListTmpl(true); }

	// advanced
	void OnNoWindowRename();

	void OnAddOption();
	void OnShowOptionTmpl()			{ LoadOptionListTmpl(true); }
	void OnDelOption();

	//void OnOptionItemClicked(QTreeWidgetItem* pItem, int Column);
	void OnOptionItemDoubleClicked(QTreeWidgetItem* pItem, int Column);
	void OnOptionSelectionChanged() { CloseOptionEdit(); OnOptChanged();}
	void OnOptionChanged(QTreeWidgetItem* pItem, int Column);


	void OnTriggerChanged()			{ m_AdvancedChanged = true; OnOptChanged(); }
	void OnShowTriggersTmpl()		{ ShowTriggersTmpl(true); }
	void OnAddAutoRun();
	void OnAddAutoSvc();
	void OnAddAutoExec();
	void OnAddRecoveryCheck();
	void OnAddDeleteCmd();
	void OnDelAuto();

	void OnAddProcess();
	void OnDelProcess();
	void OnShowHiddenProcTmpl()		{ ShowHiddenProcTmpl(true); }

	void OnAddHostProcess();
	void OnDelHostProcess();
	void OnShowHostProcTmpl()		{ ShowHostProcTmpl(true); }

	void OnAddUser();
	void OnDelUser();
	//

	void OnFilterTemplates()		{ ShowTemplates(); }
	void OnTemplateClicked(QTreeWidgetItem* pItem, int Column);
	void OnTemplateDoubleClicked(QTreeWidgetItem* pItem, int Column);
	void OnAddTemplates();
	void OnDelTemplates();
	void OnFolderChanged();
	void OnScreenReaders();

	void OnTab() { OnTab(ui.tabs->currentWidget()); }
	void OnAccessTab()  { OnTab(ui.tabsAccess->currentWidget()); }

	void OnGeneralChanged();
	void OnPSTChanged();
	void OnSecurityMode();
	void OnStartChanged()			{ m_StartChanged = true; OnOptChanged(); }
	//void OnRestrictionChanged()		{ m_RestrictionChanged = true; OnOptChanged(); }
	void OnINetBlockChanged()		{ m_INetBlockChanged = true; OnOptChanged(); }
	void OnRecoveryChanged()		{ m_RecoveryChanged = true; OnOptChanged(); }
	void OnAccessChanged();
	void OnSysSvcChanged();
	void OnAdvancedChanged();
	void OnOpenCOM();
	void OnIsolationChanged();
	void OnDebugChanged();

	void SetIniEdit(bool bEnable);
	void OnEditIni();
	void OnSaveIni();
	void OnCancelEdit();

	void OnSetTree();

protected:
	friend struct SFirewallRule;

	void closeEvent(QCloseEvent *e);

	bool eventFilter(QObject *watched, QEvent *e);

	void OnTab(QWidget* pTab);

	enum ENetWfAction
	{
		eAllow,
		eBlock
	};

	enum ENetWfProt
	{
		eAny,
		eTcp,
		eUdp,
		eIcmp,
	};

	enum EAccessEntry
	{
		eNormalFilePath,
		eOpenFilePath,
		eOpenPipePath,
		eClosedFilePath,
		eReadFilePath,
		eWriteFilePath,

		eNormalKeyPath,
		eOpenKeyPath,
		eOpenConfPath,
		eClosedKeyPath,
		eReadKeyPath,
		eWriteKeyPath,

		eNormalIpcPath,
		eOpenIpcPath,
		eClosedIpcPath,
		eReadIpcPath,

		eOpenWinClass,
		eNoRenameWinClass,

		eOpenCOM,
		eClosedCOM,
		eClosedCOM_RT,

		eMaxAccessEntry
	};

	enum EAccessType
	{
		eFile,
		eKey,
		eIPC,
		eWnd,
		eCOM,

		eMaxAccessType
	};

	enum EAccessMode
	{
		eNormal,
		eOpen,
		eOpen4All,
		eNoRename,
		eClosed,
		eClosedRT,
		eReadOnly,
		eBoxOnly,
		eIgnoreUIPI
	};

	enum ETriggerAction {
		eOnStartCmd,
		eOnStartSvc,
		eAutoExec,
		eRecoveryCheck,
		eDeleteCmd
	};

	void SetBoxColor(const QColor& color);
	void UpdateBoxColor();

	void SetProgramItem(QString Program, QTreeWidgetItem* pItem, int Column, const QString& Sufix = QString());

	QString SelectProgram(bool bOrGroup = true);
	void AddProgramToGroup(const QString& Program, const QString& Group);
	bool DelProgramFromGroup(const QString& Program, const QString& Group);
	QTreeWidgetItem* FindGroupByName(const QString& Group, bool bAdd = false);

	void CopyGroupToList(const QString& Groupe, QTreeWidget* pTree, bool disabled = false);
	QTreeWidgetItem* GetAccessEntry(EAccessType Type, const QString& Program, EAccessMode Mode, const QString& Path);
	void SetAccessEntry(EAccessType Type, const QString& Program, EAccessMode Mode, const QString& Path);
	void DelAccessEntry(EAccessType Type, const QString& Program, EAccessMode Mode, const QString& Path);

	void LoadConfig();
	void SaveConfig();
	void UpdateCurrentTab();

	void AddRunItem(const QString& Name, const QString& Command);

	void CreateGeneral();
	void LoadGeneral();
	void SaveGeneral();

	void UpdateBoxSecurity();

	void LoadGroups();
	void LoadGroupsTmpl(bool bUpdate = false);
	void SaveGroups();

	void LoadForced();
	void LoadForcedTmpl(bool bUpdate = false);
	void AddForcedEntry(const QString& Name, int type, bool disabled = false, const QString& Template = QString());
	void LoadBreakoutTmpl(bool bUpdate = false);
	void AddBreakoutEntry(const QString& Name, int type, bool disabled = false, const QString& Template = QString());
	void SaveForced();


	void LoadStop();
	void LoadStopTmpl(bool bUpdate = false);
	void LoadLeaderTmpl(bool bUpdate = false);
	void AddStopEntry(const QString& Name, bool disabled = false, const QString& Template = QString());
	void AddLeaderEntry(const QString& Name, bool disabled = false, const QString& Template = QString());
	void SaveStop();

	void LoadStart();
	//void LoadStartTmpl(bool bUpdate = false);
	void SaveStart();

	// Network
	void CreateNetwork();

	int GroupToINetMode(const QString& Mode);
	QString INetModeToGroup(int Mode);
	void LoadBlockINet();
	QString	GetINetModeStr(int Mode);
	void CloseINetEdit(bool bSave = true);
	void CloseINetEdit(QTreeWidgetItem* pItem, bool bSave = true);
	void CheckINetBlock();
	bool FindEntryInSettingList(const QString& Name, const QString& Value);
	void LoadINetAccess();
	void SaveINetAccess();

	void ParseAndAddFwRule(const QString& Value, bool disabled = false, const QString& Template = QString());
	void CloseNetFwEdit(bool bSave = true);
	void CloseNetFwEdit(QTreeWidgetItem* pItem, bool bSave = true);
	ENetWfProt GetFwRuleProt(const QString& Value);
	ENetWfAction GetFwRuleAction(const QString& Value);
	QString GetFwRuleActionStr(ENetWfAction Action);
	void LoadNetFwRules();
	void SaveNetFwRules();
	void LoadNetFwRulesTmpl(bool bUpdate = false);
	//
	
	// access
	void CreateAccess();

	QString	AccessTypeToName(EAccessEntry Type);
	void LoadAccessList();
	void LoadAccessListTmpl(bool bUpdate = false);
	void LoadAccessListTmpl(EAccessType Type, bool bChecked, bool bUpdate = false);
	QString	GetAccessTypeStr(EAccessType Type);
	QString	GetAccessModeStr(EAccessMode Mode);
	QString	GetAccessModeTip(EAccessMode Mode);
	void ParseAndAddAccessEntry(EAccessEntry EntryType, const QString& Value, bool disabled = false, const QString& Template = QString());
	void ParseAndAddAccessEntry(EAccessType Type, EAccessMode Mode, const QString& Value, bool disabled = false, const QString& Template = QString());
	void AddAccessEntry(EAccessType Type, EAccessMode Mode, QString Program, const QString& Path, bool disabled = false, const QString& Template = QString());
	QString MakeAccessStr(EAccessType Type, EAccessMode Mode);
	void SaveAccessList();
	QList<EAccessMode> GetAccessModes(EAccessType Type);
	void DeleteAccessEntry(QTreeWidgetItem* pItem, int Column = 0);

	void CloseAccessEdit(bool bSave = true);
	void CloseAccessEdit(QTreeWidgetItem* pItem, bool bSave = true);

	void UpdateAccessPolicy();

	QTreeWidget* GetAccessTree(EAccessType Type);
	//

	void LoadRecoveryList();
	void LoadRecoveryListTmpl(bool bUpdate = false);
	void LoadRecIgnoreListTmpl(bool bUpdate = false);
	void AddRecoveryEntry(const QString& Name, const QString& Template = QString());
	void AddRecIgnoreEntry(const QString& Name, const QString& Template = QString());
	void SaveRecoveryList();

	// advanced 
	void CreateAdvanced();
	void LoadAdvanced();
	void SaveAdvanced();
	void UpdateBoxIsolation();
	void ShowTriggersTmpl(bool bUpdate = false);
	void AddTriggerItem(const QString& Value, ETriggerAction Type, const QString& Template = QString());
	void ShowHiddenProcTmpl(bool bUpdate = false);
	void ShowHostProcTmpl(bool bUpdate = false);
	void AddHiddenProcEntry(const QString& Name, const QString& Template = QString());
	void AddHostProcEntry(const QString& Name, bool Value = true, const QString& Template = QString());
	void CheckOpenCOM();
	//

	void LoadOptionList();
	void LoadOptionListTmpl(bool bUpdate = false);
	void SaveOptionList();

	void AddOptionEntry(const QString& Name, QString Program, const QString& Value, const QString& Template = QString());

	void CloseOptionEdit(bool bSave = true);
	void CloseOptionEdit(QTreeWidgetItem* pItem, bool bSave = true);

	void CreateDebug();
	void LoadDebug();
	void SaveDebug();

	void LoadTemplates();
	void ShowTemplates();
	void SaveTemplates();
	void SetTemplate(const QString& Template, bool bEnabled);

	void LoadFolders();
	void ShowFolders();
	void SaveFolders();

	void LoadIniSection();
	void SaveIniSection();

	QString GetCategoryName(const QString& Category);

	bool m_HoldChange;

	bool m_ConfigDirty;
	QColor m_BorderColor;

	bool m_HoldBoxType;

	bool m_GeneralChanged;
	bool m_GroupsChanged;
	bool m_ForcedChanged;
	bool m_StopChanged;
	bool m_StartChanged;
	//bool m_RestrictionChanged;
	bool m_INetBlockChanged;
	bool m_NetFwRulesChanged;
	bool m_AccessChanged;
	bool m_TemplatesChanged;
	bool m_FoldersChanged;
	bool m_RecoveryChanged;
	bool m_AdvancedChanged;

	bool m_IsEnabledWFP;
	bool m_WFPisBlocking;

	bool m_Template;

	QMultiMap<QString, QPair<QString, QString>> m_AllTemplates;
	QStringList m_GlobalTemplates;
	QStringList m_BoxTemplates;
	QStringList m_BoxFolders;

	QList<QPair<QString, QString>> m_Settings;

	QSharedPointer<CSbieIni> m_pBox;

	QSet<QString> m_Programs;

	enum EProcCpec {
		eNoSpec,
		eSpec,
		eOnlySpec,
	};

	struct SAdvOption
	{
		EProcCpec ProcSpec;
		QStringList Values;
		QString Description;
	};

	QMap<QString, SAdvOption> m_AdvOptions;

private:

	void ReadAdvancedCheck(const QString& Name, QCheckBox* pCheck, const QString& Value = "y");
	void ReadGlobalCheck(QCheckBox* pCheck, const QString& Setting, bool bDefault);
	void WriteGlobalCheck(QCheckBox* pCheck, const QString& Setting, bool bDefault);
	void WriteAdvancedCheck(QCheckBox* pCheck, const QString& Name, const QString& Value = "y");
	void WriteAdvancedCheck(QCheckBox* pCheck, const QString& Name, const QString& OnValue, const QString& OffValue);
	void WriteText(const QString& Name, const QString& Value);
	void WriteTextList(const QString& Setting, const QStringList& List);

	Ui::OptionsWindow ui;
	QSlider* m_pColorSlider;

	struct SDbgOpt {
		QString Name;
		QString Value;
		bool Changed;
	};
	QMap<QCheckBox*, SDbgOpt> m_DebugOptions;
};


#pragma once
#include "qlistview.h"
#include "qmap.h"
#include "qstring.h"

class CQFilesListModel : public QAbstractListModel
{
	Q_OBJECT
public:
	CQFilesListModel(QWidget* parent = nullptr);
	~CQFilesListModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const;

	void AddEntry(QString entry_name, QVariant data);
	void RemoveEntry(QString entry_name);

protected:
	QVector<QString> m_Entries;
};


class CQFilesList : public QListView
{
	Q_OBJECT
public:
	CQFilesList(QWidget* parent = nullptr);

private:
	CQFilesListModel* m_Model;
};
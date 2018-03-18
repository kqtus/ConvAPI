#include "FIlesList.h"


CQFilesListModel::CQFilesListModel(QWidget* parent)
	: QAbstractListModel(parent)
{

}

CQFilesListModel::~CQFilesListModel()
{

}

int CQFilesListModel::rowCount(const QModelIndex& parent) const
{
	return m_Entries.size();
}

QVariant CQFilesListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= m_Entries.size())
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole)
	{
		return QVariant(m_Entries[index.row()]);
	}

	return QVariant();
}

void CQFilesListModel::AddEntry(QString entry_name, QVariant data)
{
	m_Entries.push_back(entry_name);
}

void CQFilesListModel::RemoveEntry(QString entry_name)
{
	//m_Entries.remove(entry_name);
}

CQFilesList::CQFilesList(QWidget* parent)
	: QListView(parent)
{

}
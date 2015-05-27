#include "treemodel.h"
#include <QIcon>
#include "Models/modelfunctions.h"

TreeModel::TreeModel(DirNode *root, QObject *parent) : QAbstractItemModel(parent){
  m_root = root;
  iconsPath="lib/icons/";
//  prepareIcons();
}

DirNode* TreeModel::getRoot(){
  return m_root;
}

void TreeModel::prepareIcons(){
//  icons.append(QIcon(iconsPath+"directory.png"));
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const{
  if( !index.isValid() )
    return QVariant();

  if(TreeNode* item=static_cast<TreeNode*>( index.internalPointer() )){
    if( role == Qt::DisplayRole ){
      return item->text;
    }

    if (role == Qt::DecorationRole){
      QString itemIconPath=item->icon;
      QString iconName=ModelFunctions::extractIconName(itemIconPath);

      QIcon icon(iconsPath+iconName);
      return icon;
    }
  }
  
  return QVariant();
}

int TreeModel::rowCount(const QModelIndex &parent) const{
  DirNode *parentObject;

  if( !parent.isValid() )
    parentObject = m_root;
  else{
    TreeNode* temp = static_cast<TreeNode*>( parent.internalPointer() );
    parentObject = dynamic_cast<DirNode*>(temp);
    if(!parentObject)
      return 0;
  }

  return parentObject->treeChildren->size();
}

int TreeModel::columnCount(const QModelIndex &parent) const{
  return 1;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const{
  DirNode *parentObject;

  if( !parent.isValid() )
    parentObject = m_root;
  else{
    TreeNode* temp = static_cast<TreeNode*>( parent.internalPointer() );
    parentObject = dynamic_cast<DirNode*>(temp);
    if(!parentObject)
      parentObject = m_root;
  }

  if( row < parentObject->treeChildren->size() ){
    TreeNode* child=parentObject->treeChildren->at(row);
    return createIndex( row, column, child );
  }
  else
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const{
  if( !index.isValid() )
    return QModelIndex();
 
  TreeNode *indexObject = static_cast<TreeNode*>( index.internalPointer() );
  TreeNode *parentObject = indexObject->m_parent;

  if( parentObject == m_root )
    return QModelIndex();
  
  DirNode *grandParentObject = dynamic_cast<DirNode*>( parentObject->m_parent );
  if(!grandParentObject)
    return QModelIndex();
    
  return createIndex( grandParentObject->treeChildren->indexOf( parentObject ), 0, parentObject );
}

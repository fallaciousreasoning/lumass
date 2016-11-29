 /****************************************************************************** 
 * Created by Alexander Herzig 
 * Copyright 2010,2011,2012 Landcare Research New Zealand Ltd 
 *
 * This file is part of 'LUMASS', which is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License, 
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/*
 * NMModelScene.cpp
 *
 *  Created on: 20/06/2012
 *      Author: alex
 */

#include <QMimeData>
#include <QString>
#include <QObject>
#include <QStatusBar>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QFileInfo>
#include <QGraphicsProxyWidget>

#include "NMGlobalHelper.h"
#include "NMModelScene.h"
#include "NMModelViewWidget.h"
#include "NMProcessComponentItem.h"
#include "NMAggregateComponentItem.h"
#include "NMParameterTable.h"
#include "NMSqlTableModel.h"
#include "lumassmainwin.h"
#include "nmlog.h"



class NMModelViewWidget;

const std::string NMModelScene::ctx = "NMModelScene";


NMModelScene::NMModelScene(QObject* parent)
	: QGraphicsScene(parent)
{
	//ctx = "NMModelScene";
    mMode = NMS_IDLE;
    mbSceneMove = false;
    mLinkHitTolerance = 15;
	mLinkZLevel = 10000;
	mLinkLine = 0;
    mHiddenModelItems.clear();
}

NMModelScene::~NMModelScene()
{
}

void
NMModelScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    NMDebugCtx(ctx, << "...");
    if (event->mimeData()->hasFormat("text/plain"))
    {
        QString leaveText = event->mimeData()->text();
        if (leaveText.startsWith(QString::fromLatin1("_NMModelScene_:")))
        {
            if (mDragItemList.count() == 1)
            {
                NMProcessComponentItem* pi = qgraphicsitem_cast<NMProcessComponentItem*>(
                            mDragItemList.at(0));
                if (pi != 0)
                {
                    if (pi->getIsDataBufferItem())
                    {
                        leaveText = QString::fromLatin1("_NMModelScene_:%1").arg(pi->getTitle());
                        QMimeData* md = const_cast<QMimeData*>(event->mimeData());
                        md->setText(leaveText);
                        event->setMimeData(md);
                        event->accept();
                    }
                }
            }
        }
    }
    mDragItemList.clear();
    NMDebugCtx(ctx, << "done!");

    //QGraphicsScene::dragLeaveEvent(event);
}

void
NMModelScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    NMDebugCtx(ctx, << "...");

    if (    event->mimeData()->hasFormat("text/plain")
        ||  event->mimeData()->hasUrls())
    {
        QString fileName;
        foreach(const QUrl& url, event->mimeData()->urls())
        {
            if (url.isLocalFile())
            {
                fileName = url.toLocalFile();
                NMDebugAI(<< fileName.toStdString() << std::endl);
                //break;
            }
        }

        NMDebugAI(<< event->mimeData()->text().toStdString());
        QString mimeText = event->mimeData()->text();
        if (    mimeText.startsWith(QString::fromLatin1("_NMProcCompList_:"))
            ||  mimeText.startsWith(QString::fromLatin1("_NMModelScene_:"))
            ||  mimeText.startsWith(QString::fromLatin1("_ModelComponentList_:"))
            ||  !fileName.isEmpty()
           )
        {
            NMDebug(<< mimeText.toStdString() << " - supported!" << std::endl);
            event->acceptProposedAction();
        }
        else
        {
            NMDebug(<< " - unfortunately not supported!" << std::endl);
        }
    }

    NMDebugCtx(ctx, << "done!");
}

void
NMModelScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    if (    event->mimeData()->hasFormat("text/plain")
        ||  event->mimeData()->hasUrls()
       )
    {
        event->acceptProposedAction();
    }
}

void
NMModelScene::toggleLinkToolButton(bool linkMode)
{
	if (linkMode)
	{
		this->views().at(0)->setDragMode(QGraphicsView::NoDrag);
		this->views().at(0)->setCursor(Qt::CrossCursor);
        this->setProcCompMoveability(false);
		this->mMode = NMS_LINK;
        NMGlobalHelper::getMainWindow()->updateExclusiveActions("linkAction");
	}
	else
    {
        if (!mbSceneMove)
        {
            this->setProcCompMoveability(true);
        }
        this->views().at(0)->setCursor(Qt::OpenHandCursor);
        this->mMode = NMS_MOVE;
    }
}

void
NMModelScene::updateComponentItemFlags(QGraphicsItem *item)
{
    switch(mMode)
    {
    case NMS_LINK:
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        break;
    case NMS_SELECT:
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        break;
    case NMS_IDLE:
    default:
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        break;
    }

    if (mbSceneMove)
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    else
    {
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}



void NMModelScene::setProcCompSelectability(bool selectable)
{
	QList<QGraphicsItem*> allItems = this->items();
	QListIterator<QGraphicsItem*> it(allItems);
    //NMProcessComponentItem* item;
	while(it.hasNext())
	{
		QGraphicsItem* item = it.next();
		if (item != 0)
		{
			item->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
		}
	}
}

void NMModelScene::setLinkCompSelectability(bool selectable)
{
	QList<QGraphicsItem*> allItems = this->items();
	QListIterator<QGraphicsItem*> it(allItems);
	NMComponentLinkItem* item;
	while(it.hasNext())
	{
		item = qgraphicsitem_cast<NMComponentLinkItem*>(it.next());
		if (item != 0)
			item->setFlag(QGraphicsItem::ItemIsSelectable, selectable);
	}
}


void NMModelScene::setProcCompMoveability(bool moveable)
{
    QList<QGraphicsItem*> allItems = this->items();
	QListIterator<QGraphicsItem*> it(allItems);
    //NMProcessComponentItem* item;
	while(it.hasNext())
	{
        QGraphicsItem* ni = it.next();
        QGraphicsProxyWidget* pwi = qgraphicsitem_cast<QGraphicsProxyWidget*>(ni);
        if (pwi)
        {
            //ni->setFlag(QGraphicsItem::ItemIsSelectable, moveable);
            pwi->setFlag(QGraphicsItem::ItemIsSelectable, moveable);
            pwi->setFlag(QGraphicsItem::ItemIsMovable, moveable);
        }
        else
        {
            ni->setFlag(QGraphicsItem::ItemIsMovable, moveable);
        }
	}
}


void NMModelScene::toggleSelToolButton(bool selMode)
{
	if (selMode)
	{
		this->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
		this->views().at(0)->setCursor(Qt::PointingHandCursor);
		this->mMode = NMS_SELECT;
		this->setProcCompSelectability(true);
		this->setLinkCompSelectability(false);
	}
	else
	{
        this->views().at(0)->setDragMode(QGraphicsView::NoDrag);
		this->mMode = NMS_IDLE;
		this->setProcCompSelectability(false);
		//this->setLinkCompSelectability(false);
	}
}

void NMModelScene::toggleMoveToolButton(bool moveMode)
{
    if (moveMode)
	{
        mbSceneMove = true;
        NMGlobalHelper::getMainWindow()->updateExclusiveActions("panAction");
        this->setProcCompMoveability(false);
	}
	else
	{
        mbSceneMove = false;
        if (mMode != NMS_LINK)
        {
            this->setProcCompMoveability(true);
        }
	}
    this->invalidate();
}

QGraphicsItem*
NMModelScene::getComponentItem(const QString& name)
{
	QGraphicsItem* retItem = 0;
	QList<QGraphicsItem*> allItems = this->items();
	QListIterator<QGraphicsItem*> it(allItems);
	while(it.hasNext())
	{
		QGraphicsItem* item = it.next();
		NMProcessComponentItem* procItem = qgraphicsitem_cast<NMProcessComponentItem*>(item);
		NMAggregateComponentItem* aggrItem = qgraphicsitem_cast<NMAggregateComponentItem*>(item);
        QGraphicsProxyWidget* widgetItem = qgraphicsitem_cast<QGraphicsProxyWidget*>(item);

		if (procItem != 0)
		{
			if (procItem->getTitle().compare(name) == 0)
				return procItem;
		}
		else if (aggrItem != 0)
		{
			if (aggrItem->getTitle().compare(name) == 0)
				return aggrItem;
		}
        else if (widgetItem != 0)
        {
            if (widgetItem->objectName().compare(name) == 0)
                return widgetItem;
        }
	}

	return retItem;
}

void
NMModelScene::addParameterTable(NMSqlTableView* tv,
                                NMAggregateComponentItem* ai,
                                NMModelComponent* host)
{
    if (tv == 0)
    {
        return;
    }

    NMSqlTableModel* tabModel = qobject_cast<NMSqlTableModel*>(tv->getModel());
    QString dbFN = tabModel->getDatabaseName();

    NMParameterTable* pt = new NMParameterTable(host);
    pt->setTableName(tv->getModel()->tableName());
    pt->setFileName(dbFN);
    pt->setUserID(tv->getModel()->tableName());
    pt->setDescription(tv->getModel()->tableName());
    if (host == 0)
    {
        host = NMModelController::getInstance()->getComponent(QString::fromLatin1("root"));
    }

    pt->setTimeLevel(host->getTimeLevel());
    QString ptName = NMModelController::getInstance()->addComponent(pt, host);


    QGraphicsProxyWidget* proxyWidget = this->addWidget(tv,
                   Qt::CustomizeWindowHint | Qt::Window | Qt::WindowTitleHint);
    proxyWidget->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxyWidget->setObjectName(ptName);
    this->updateComponentItemFlags(proxyWidget);

//    connect(this, SIGNAL(widgetViewPortRightClicked(QGraphicsSceneMouseEvent*,QGraphicsItem*)),
//            tv, SLOT(processParaTableRightClick(QGraphicsSceneMouseEvent*,QGraphicsItem*)));
    connect(this, SIGNAL(itemDblClicked(QGraphicsSceneMouseEvent*)),
            tv, SLOT(processParaTableDblClick(QGraphicsSceneMouseEvent*)));

    if (ai)
    {
        ai->addToGroup(proxyWidget);
    }
    else
    {
        this->addItem(proxyWidget);
    }
    proxyWidget->setPos(proxyWidget->mapToParent(mMousePos));
}


void NMModelScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	NMDebugCtx(ctx, << "...");
    mMousePos = event->scenePos();
    if (    event->mimeData()->hasFormat("text/plain")
        ||  event->mimeData()->hasUrls())
	{
		QString dropText = event->mimeData()->text();
        QStringList dropsplit = dropText.split(':');
        QString dropSource = dropsplit.at(0);
        QString dropItem;
        if (dropsplit.count() > 1)
        {
            dropItem = dropsplit.at(1);
        }

		if (NMModelController::getInstance()->isModelRunning())
        {
			QMessageBox::information(0, "Invalid user request!",
					"You cannot create new model components\nwhile a "
					"model is being executed! Please try again later!");

			NMErr(ctx, << "You cannot create new model components while there is "
					     "a model running. Please try again later!");
			NMDebugCtx(ctx, << "done!");
			return;
		}

        // do something depending on the source of the object

        // =========================================================
        //  MOVE / COPY COMPONENTS ON SCENE
        // =========================================================
        if (dropSource.startsWith(QString::fromLatin1("_NMModelScene_")))
        {
            //event->acceptProposedAction();
            QPointF dropPos = event->scenePos();
            switch(event->dropAction())
            {
            case Qt::MoveAction:
                NMDebugAI( << "moving: " << mDragItemList.count() << " items" << std::endl);
                emit signalItemMove(mDragItemList, mDragStartPos, dropPos);
                break;
            case Qt::CopyAction:
                NMDebugAI( << "copying: " << mDragItemList.count() << " items" << std::endl);
                emit signalItemCopy(mDragItemList, mDragStartPos, dropPos);
                break;
            }
            this->mDragItemList.clear();
        }
        // =========================================================
        //  ADD NEW PROC COMP ITEMS
        // =========================================================
        else if (dropSource.startsWith(QString::fromLatin1("_NMProcCompList_")))
        {
            QGraphicsItem* pi = this->itemAt(event->scenePos(), this->views()[0]->transform());
            NMAggregateComponentItem* ai = qgraphicsitem_cast<NMAggregateComponentItem*>(pi);

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //  TEXT LABEL
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            if (dropItem.compare(QString::fromLatin1("TextLabel")) == 0)
            {
                QGraphicsTextItem* labelItem = new QGraphicsTextItem(ai);
                labelItem->setHtml(QString::fromLatin1("<b>Text Label</b>"));
                labelItem->setTextInteractionFlags(Qt::NoTextInteraction);
                labelItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                labelItem->setOpenExternalLinks(true);
                if (ai != 0)
                {
                    ai->addToGroup(labelItem);
                    labelItem->setPos(ai->mapFromScene(event->scenePos()));
                }
                else
                {
                    this->addItem(labelItem);
                    labelItem->setPos(event->scenePos());
                }
                //event->acceptProposedAction();
            }
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //  PARAMETER TABLE
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            else if (dropItem.compare(QString::fromLatin1("ParameterTable")) == 0)
            {
                NMModelComponent* host = 0;

                if (ai)
                {
                    host = NMModelController::getInstance()->getComponent(ai->getTitle());
                }
                else
                {
                    NMProcessComponentItem* procItem = qgraphicsitem_cast<NMProcessComponentItem*>(pi);
                    if (procItem)
                    {
                        host = NMModelController::getInstance()->getComponent(procItem->getTitle())->getHostComponent();
                    }
                }

                NMSqlTableView* tv = NMGlobalHelper::getMainWindow()->openCreateTable(
                            LUMASSMainWin::NM_TABVIEW_SCENE, false);

                this->addParameterTable(tv, ai, host);
            }
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            //  PROCESS COMPONENT
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            else
            {
                NMProcessComponentItem* procItem = new NMProcessComponentItem(0, this);
                procItem->setTitle(dropItem);
                procItem->setDescription(dropItem);
                procItem->setPos(event->scenePos());
                if (dropItem.compare("DataBuffer") == 0)
                {
                    procItem->setIsDataBufferItem(true);
                }
                procItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                //event->acceptProposedAction();

                NMDebugAI(<< "asking for creating '" << dropItem.toStdString() << "' ..." << endl);
                emit processItemCreated(procItem, dropItem, event->scenePos());
            }
        }
        // =========================================================
        //  IMPORT FROM FILE
        // =========================================================
        else if (event->mimeData()->hasUrls())
        {
            QGraphicsItem* item = this->itemAt(mMousePos, this->views()[0]->transform());
            NMProcessComponentItem* procItem = qgraphicsitem_cast<NMProcessComponentItem*>(item);
            NMAggregateComponentItem* aggrItem = qgraphicsitem_cast<NMAggregateComponentItem*>(item);

            // supported formats for parameter tables
            QStringList tabFormats;
            tabFormats << "dbf" << "db" << "sqlite" << "ldb" << "csv" << "txt" << "xls";

            // we grab the first we can get hold of and check the ending ...
            QString fileName;
            if (event->mimeData()->urls().at(0).isLocalFile())
            {
                fileName = event->mimeData()->urls().at(0).toLocalFile();
            }

            QFileInfo fifo(fileName);
            QString suffix = fifo.suffix();

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // LUMASS MODEL FILE
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            if (    fileName.endsWith(QString::fromLatin1("lmv"))
                ||  fileName.endsWith(QString::fromLatin1("lmx"))
               )
            {
                if (!fileName.isEmpty())
                {
                    QFileInfo finfo(fileName);
                    if (finfo.isFile())
                    {
                        NMDebugAI(<< "gonna import model file: " << fileName.toStdString() << std::endl);
                        //event->acceptProposedAction();

                        emit signalModelFileDropped(fileName, mMousePos);
                    }
                }
            }
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // PARAMETER TABLE
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            else if (   tabFormats.contains(suffix, Qt::CaseInsensitive)
                     //&& aggrItem
                    )
            {
                NMModelComponent* host = 0;
                if (aggrItem)
                {
                    host = NMModelController::getInstance()->getComponent(aggrItem->getTitle());
                }

                NMGlobalHelper h;
                LUMASSMainWin* mwin = h.getMainWindow();
                NMSqlTableView* tv = 0;
                QStringList sqliteformats;
                sqliteformats << "db" << "sqlite" << "ldb";

                QString tableName;
                if (sqliteformats.contains(suffix, Qt::CaseInsensitive))
                {
                    tableName = mwin->selectSqliteTable(fileName);
                    if (!tableName.isEmpty())
                    {
                        tv = mwin->importTable(fileName,
                                          LUMASSMainWin::NM_TABVIEW_SCENE,
                                          true,
                                          tableName);
                    }
                }
                else
                {
                    tv = mwin->importTable(fileName, LUMASSMainWin::NM_TABVIEW_SCENE, true);
                }

                this->addParameterTable(tv, aggrItem, host);
            }
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // LIST OF IMAGE/TABLE FILENAMES ON PROCESS COMP ITEM
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            else if (procItem != 0)
            {
                // check whether the procItem has got a fileName property
                NMModelComponent* comp = NMModelController::getInstance()->getComponent(procItem->getTitle());
                NMIterableComponent* itComp = qobject_cast<NMIterableComponent*>(comp);
                NMProcess* proc = 0;
                if (itComp != 0)
                {
                    proc = itComp->getProcess();
                }

                if (proc != 0)
                {
                    QStringList propList = NMModelController::getPropertyList(proc);
                    QStringList fnProps;
                    foreach(const QString& p, propList)
                    {
                        if (p.contains(QString::fromLatin1("FileName"), Qt::CaseInsensitive))
                        {
                            fnProps << p;
                        }
                    }


                    if (fnProps.size() > 0)
                    {
                        QStringList fileNames;
                        foreach(const QUrl& url, event->mimeData()->urls())
                        {
                                fileNames << url.toLocalFile();
                        }

                        QString theProperty = fnProps.at(0);
                        bool bOk = true;
                        if (fnProps.size() > 1)
                        {

                            theProperty = QInputDialog::getItem(0,
                                                                QString::fromLatin1("Set Filenames"),
                                                                QString::fromLatin1("Select target property:"),
                                                                fnProps, 0, false, &bOk);
                        }

                        if (bOk)
                        {
                            QVariant propVal = proc->property(theProperty.toStdString().c_str());
                            if (propVal.type() == QVariant::StringList)
                            {
                                QStringList fnList = propVal.toStringList();
                                if (event->modifiers() & Qt::ControlModifier)
                                {
                                    fnList.append(fileNames);
                                }
                                else
                                {
                                    fnList = fileNames;
                                }

                                QVariant newVal = QVariant::fromValue(fnList);
                                proc->setProperty(theProperty.toStdString().c_str(), newVal);
                            }
                            else if (QString::fromLatin1("QList<QStringList>").compare(propVal.typeName(), Qt::CaseInsensitive) == 0)
                            {
                                QList<QStringList> fnListList = propVal.value<QList<QStringList> >();
                                fnListList.append(fileNames);

                                QVariant newVal = QVariant::fromValue(fnListList);
                                proc->setProperty(theProperty.toStdString().c_str(), newVal);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            NMDebugAI(<< "No valid drag source detected!" << std::endl);
        }
	}
    //event->accept();
    if (!mbSceneMove)
    {
        this->setProcCompMoveability(true);
    }
    mDragItemList.clear();
	NMDebugCtx(ctx, << "done!");
}

void
NMModelScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    mMousePos = event->scenePos();
    //qDebug() << "wheelEvent(): scene pos: " << mMousePos;
    emit zoom(event->delta());
}

void
NMModelScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
        QGraphicsItem* item = this->itemAt(event->scenePos(), this->views()[0]->transform());
        QGraphicsProxyWidget* widgetItem = qgraphicsitem_cast<QGraphicsProxyWidget*>(item);
        QGraphicsTextItem* textItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
		NMProcessComponentItem* procItem = qgraphicsitem_cast<NMProcessComponentItem*>(item);
		NMAggregateComponentItem* aggrItem = qgraphicsitem_cast<NMAggregateComponentItem*>(item);
		if (item == 0)
		{
			emit rootComponentDblClicked();
		}
        else if (widgetItem != 0)
        {
            //emit procAggregateCompDblClicked(widgetItem->objectName());
            emit itemDblClicked(event);
        }
		else if (procItem != 0)
		{
			emit procAggregateCompDblClicked(procItem->getTitle());
		}
		else if (aggrItem != 0)
		{
			emit procAggregateCompDblClicked(aggrItem->getTitle());
		}
        else if (textItem != 0)
        {
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction |
                                              Qt::TextBrowserInteraction);
        }
	}
	else
	{
		QGraphicsScene::mouseDoubleClickEvent(event);
	}
}

void
NMModelScene::serialiseItems(QList<QGraphicsItem*> items, QDataStream& data)
{

}

QGraphicsProxyWidget*
NMModelScene::getWidgetAt(const QPointF& pos)
{
    QGraphicsProxyWidget* ret = 0;
    QList<QGraphicsItem*> allItems = this->items();
    foreach (QGraphicsItem* gi, allItems)
    {
        ret = qgraphicsitem_cast<QGraphicsProxyWidget*>(gi);
        if (ret)
        {
            QRectF wf = ret->mapRectToScene(ret->windowFrameRect());
            if (wf.contains(pos))
            {
                break;
            }
            ret = 0;
        }
    }
    return ret;
}

void
NMModelScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    mMousePos = event->scenePos();
    mDragItemList.clear();
    QGraphicsItem* item = this->itemAt(event->scenePos(), this->views()[0]->transform());

    QGraphicsProxyWidget* pwi = 0;
    QGraphicsTextItem* textItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
    NMProcessComponentItem* procItem = qgraphicsitem_cast<NMProcessComponentItem*>(item);
    NMAggregateComponentItem* aggrItem = qgraphicsitem_cast<NMAggregateComponentItem*>(item);

	if (event->button() == Qt::LeftButton)
	{
		switch(mMode)
		{
		case NMS_LINK:
			mLinkLine = new QGraphicsLineItem(
					QLineF(event->scenePos(),
							event->scenePos()));
			mLinkLine->setPen(QPen(Qt::darkGray, 2));
			this->addItem(mLinkLine);
			break;

		default:
            {
                // SELECTION 'MODE'
                if (event->modifiers() & Qt::ControlModifier)
                {
                    this->views().at(0)->setCursor(Qt::PointingHandCursor);
                    this->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
                    //this->setProcCompMoveability(false);
                    this->setProcCompSelectability(true);
                    this->setLinkCompSelectability(false);
                    pwi = this->getWidgetAt(mMousePos);
                    if (pwi)
                    {
                        pwi->setSelected(true);
                        NMDebugAI(<< pwi->objectName().toStdString() << " selected!" << std::endl);
                    }
                }
                // INFO/DRAG 'MODE'
                else
                {
                    this->views().at(0)->setDragMode(QGraphicsView::ScrollHandDrag);

                    pwi = this->getWidgetAt(event->scenePos());
                    if (pwi)
                    {
                        this->views().at(0)->setCursor(Qt::ClosedHandCursor);
                        emit itemLeftClicked(pwi->objectName());
                    }
                    else if (item != 0)
                    {
                        this->views().at(0)->setCursor(Qt::ClosedHandCursor);
                        if (procItem)
                        {
                            emit itemLeftClicked(procItem->getTitle());
                        }
                        else if (aggrItem)
                        {
                            emit itemLeftClicked(aggrItem->getTitle());
                        }
                        else if (textItem)
                        {
                            textItem->setTextInteractionFlags(Qt::NoTextInteraction
                                                              | Qt::TextBrowserInteraction);
                        }
                    }
                    else
                    {
                        this->views().at(0)->setCursor(Qt::OpenHandCursor);
                        this->setProcCompSelectability(false);
                        emit itemLeftClicked(QString::fromUtf8("root"));
                    }
                }
            }
			break;
		}
        QGraphicsScene::mousePressEvent(event);
	}
	else if (event->button() == Qt::RightButton)
	{
        QPointF pt = event->scenePos();
        QGraphicsItem* sendItem = 0;

        pwi = this->getWidgetAt(pt);
        if (pwi)
        {
            QRectF fr = pwi->mapRectToScene(pwi->windowFrameRect());
            QRectF cr = pwi->mapRectToScene(pwi->contentsRect());
            QRectF tb = fr.intersected(cr);
            if (!tb.contains(pt))
            {
                emit widgetTitleBarRightClicked(event, pwi);
            }
            else
            {
                //emit widgetViewPortRightClicked(event, pwi);
                NMSqlTableView* tv = qobject_cast<NMSqlTableView*>(pwi->widget());
                tv->processParaTableRightClick(event, pwi);
            }
        }
        else if (item)
        {
            // first, we check, whether we've got a link on the hook
            sendItem = this->getLinkItem(event->scenePos());
            if (sendItem == 0 && item != 0)
            {
                sendItem = item;
            }
            emit itemRightBtnClicked(event, sendItem);
        }
	}
	else
	{
        QGraphicsScene::mousePressEvent(event);
	}
}

NMComponentLinkItem* NMModelScene::getLinkItem(QPointF pos)
{
    QGraphicsItem* item = this->itemAt(pos, this->views()[0]->transform());
	NMComponentLinkItem* link = qgraphicsitem_cast<NMComponentLinkItem*>(item);

	if (link == 0)
	{
		QPointF p = pos;
		qreal dxy = this->mLinkHitTolerance / 2.0;
		qreal wh = this->mLinkHitTolerance;
		QList<QGraphicsItem*> listItems = this->items(p.x()-dxy, p.y()-dxy, wh, wh,
				Qt::IntersectsItemShape, Qt::DescendingOrder);
		foreach(QGraphicsItem* i, listItems)
		{
			link = qgraphicsitem_cast<NMComponentLinkItem*>(i);
			if (link != 0)
			{
				NMProcessComponentItem* src = const_cast<NMProcessComponentItem*>(link->sourceItem());
				NMProcessComponentItem* tar = const_cast<NMProcessComponentItem*>(link->targetItem());
				NMDebugAI(<< "link from " << src->getTitle().toStdString()
						<< " to " << tar->getTitle().toStdString() << endl);
				break;
			}
		}
	}

	return link;
}

void
NMModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // update coordinate label in status bar
    QPointF sp = event->scenePos();
    QGraphicsItem* item = this->itemAt(sp, this->views()[0]->transform());
    NMAggregateComponentItem* ai = 0;
    NMProcessComponentItem* pi = 0;
    qreal x = sp.x();
    qreal y = sp.y();
    QString title = "Scene";
    if (item != 0)
    {
        pi = qgraphicsitem_cast<NMProcessComponentItem*>(item);
        if (pi != 0)
        {
            if (pi->parentItem() != 0)
            {
                ai = qgraphicsitem_cast<NMAggregateComponentItem*>(pi->parentItem());
            }
        }
        else
        {
             ai = qgraphicsitem_cast<NMAggregateComponentItem*>(item);
        }

        if (ai != 0)
        {
            title = ai->getTitle();
            QPointF aip = ai->mapFromScene(sp);
            x = aip.x();
            y = aip.y();
        }
    }

    QString pos = QString("Scene Position - X: %1 Y: %2 || %3 Position - X: %4 Y: %5")
            .arg(event->scenePos().x())
            .arg(event->scenePos().y())
            .arg(title)
            .arg(x)
            .arg(y);
    //NMModelViewWidget* vw = qobject_cast<NMModelViewWidget*>(this->views().at(0)->parent());
    LUMASSMainWin* mainWin = 0;
    //if (vw != 0)
    {
        mainWin = NMGlobalHelper::getMainWindow();//vw->getMainWindow();
        if (mainWin != 0)
        {
            mainWin->updateCoordLabel(pos);
        }
    }

    //mMousePos = event->scenePos();

	switch(mMode)
	{
	case NMS_LINK:
		if (mLinkLine == 0)
			break;
		mLinkLine->setLine(QLineF(mLinkLine->line().p1(), event->scenePos()));
		break;

	case NMS_MOVE:
    case NMS_SELECT:
    default:

        {
            QGraphicsItem* dragItem = qgraphicsitem_cast<QGraphicsItem*>(this->getWidgetAt(mMousePos));
            if (dragItem == 0)
            {
                dragItem = this->itemAt(mMousePos, this->views()[0]->transform());
            }
            if (    (event->buttons() & Qt::LeftButton)
                &&  dragItem != 0
                &&  (   QApplication::keyboardModifiers() & Qt::AltModifier
                     || QApplication::keyboardModifiers() & Qt::ShiftModifier
                    )
               )
            {
                mDragStartPos = event->scenePos();
                mDragItemList.clear();
                mDragItemList = this->selectedItems();
                if (mDragItemList.count() == 0)
                {
                    mDragItemList.push_back(dragItem);
                }
                QRectF selRect;
                foreach(const QGraphicsItem* gi, mDragItemList)
                {
                    selRect = selRect.united(gi->mapRectToScene(gi->boundingRect()));
                }
                QPixmap dragPix = QPixmap::grabWidget(this->views()[0],
                        this->views()[0]->mapFromScene(selRect).boundingRect());
                dragPix = dragPix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QDrag* drag = new QDrag(this);
                QMimeData* mimeData = new QMimeData;
                QString mimeText = QString("_NMModelScene_:%1").arg(mDragItemList.count());
                mimeData->setText(mimeText);

                drag->setMimeData(mimeData);
                drag->setPixmap(dragPix);
                if (event->modifiers() & Qt::ShiftModifier)
                {
                    NMDebugAI(<< " >> moving ..." << std::endl);
                    //drag->setDragCursor(dragPix, Qt::MoveAction);
                    drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::MoveAction);
                }
                else if (event->modifiers() & Qt::AltModifier)
                {
                   if (dragItem->type() == QGraphicsProxyWidget::Type)
                   {
                       QMessageBox::information(qobject_cast<QWidget*>(this->parent()),
                                                "Copy Item",
                                                "Sorry, LUMASS doesn't support "
                                                "(deep) copying parameter tables!");

                       //NMBoxInfo("Copy Item", "Sorry, LUMASS doesn't support (deep) copying parameter tables!");
                       return;
                   }
                   else
                   {
                       NMDebugAI(<< " >> copying ..." << std::endl);
                       //drag->setDragCursor(dragPix, Qt::CopyAction);
                       drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
                   }
                }
                NMDebugAI(<< "drag start - " << mimeText.toStdString() << std::endl);
                this->setProcCompMoveability(false);
            }
            else if (dragItem && mbSceneMove)
            {
                return;
            }
        }
        this->invalidate();
        break;
	}
    QGraphicsScene::mouseMoveEvent(event);
}

void
NMModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	NMProcessComponentItem* srcComp = 0;
	NMProcessComponentItem* tarComp = 0;
	NMComponentLinkItem* link = 0;
	QList<QGraphicsItem*> srcList;
	QList<QGraphicsItem*> tarList;
	switch(mMode)
	{
	case NMS_LINK:
		if (mLinkLine == 0)
			break;

		srcList = items(mLinkLine->line().p1());
		if (srcList.count() && srcList.first() == mLinkLine)
			srcList.removeFirst();
		tarList = items(mLinkLine->line().p2());
		if (srcList.count() && tarList.first() == mLinkLine)
			tarList.removeFirst();

		removeItem(mLinkLine);
		delete mLinkLine;
		mLinkLine = 0;

		if (srcList.count() > 0 && tarList.count() > 0 &&
			srcList.first() != tarList.first())
		{
			srcComp =
					qgraphicsitem_cast<NMProcessComponentItem*>(srcList.first());
			tarComp =
					qgraphicsitem_cast<NMProcessComponentItem*>(tarList.first());

			if (srcComp == 0 || tarComp == 0)
				break;
			int st = srcComp->type();
			int tt = tarComp->type();

			NMDebugAI(<< "types are: " << st << " "
					<< tt << std::endl);

			link = new NMComponentLinkItem(
					srcComp, tarComp, 0);

			srcComp->addOutputLink(-1, link);
			tarComp->addInputLink(-1, link);
            link->setZValue(this->mLinkZLevel);
            addItem(link);
            this->invalidate();
            emit linkItemCreated(link);
		}

		break;

	default:
        {
            if (event->modifiers() & Qt::ControlModifier)
            {
                this->views().at(0)->setCursor(Qt::PointingHandCursor);
                this->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
                this->setProcCompSelectability(true);
                this->setLinkCompSelectability(false);
                mMode = NMS_SELECT;
            }
            else
            {
                this->views().at(0)->setDragMode(QGraphicsView::ScrollHandDrag);
                this->views().at(0)->setCursor(Qt::OpenHandCursor);
                this->setProcCompSelectability(false);
                mMode = NMS_IDLE;
            }
            this->mDragItemList.clear();
        }
		break;
	}
    if (!mbSceneMove && mMode != NMS_LINK)
    {
        this->setProcCompMoveability(true);
    }
	QGraphicsScene::mouseReleaseEvent(event);
}




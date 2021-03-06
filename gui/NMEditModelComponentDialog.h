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
#ifndef NMEDITMODELCOMPONENTDIALOG_H
#define NMEDITMODELCOMPONENTDIALOG_H

#include <string>
#include <iostream>
#include <QWidget>
#include <QVariant>
#include "ui_NMEditModelComponentDialog.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include <QtTreePropertyBrowser>

#include "nmlog.h"

#include "NMComponentEditor.h"
#include "NMProcess.h"
//#include "NMModelComponent.h"

#ifdef BUILD_RASSUPPORT
  #include "NMRasdamanConnectorWrapper.h"
#endif
  
class NMEditModelComponentDialog : public QWidget
{
    Q_OBJECT

public:
    NMEditModelComponentDialog(QWidget *parent = 0);
    ~NMEditModelComponentDialog();

    void setObject(QObject* obj);

#ifdef BUILD_RASSUPPORT    
    void setRasdamanConnectorWrapper(NMRasdamanConnectorWrapper* wrap)
        {mCompEditor->setRasdamanConnectorWrapper(wrap);}
        //{this->mRasConn = wrap;}
#endif		

signals:
	void finishedEditing(QObject* obj);

public slots:
    void update(void) {mCompEditor->update();}


private:
    void closeEvent(QCloseEvent* event);

    static const std::string ctx;

    NMComponentEditor* mCompEditor;
    QScrollArea* mScrollArea;
};

#endif // NMEDITMODELCOMPONENTDIALOG_H

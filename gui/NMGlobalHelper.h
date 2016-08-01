#ifndef NMGLOBALHELPER_H
#define NMGLOBALHELPER_H

#include "vtkRenderWindow.h"
#include "lumassmainwin.h"
#include "ui_lumassmainwin.h"

#include <QString>
#include <QAbstractItemModel>
#include <QList>
#include <QItemSelection>

class NMGlobalHelper
{
public:

    // static functions
    static void startBusy(void);
    static void endBusy(void);

    static LUMASSMainWin* getMainWindow(void);
    static vtkRenderWindow* getRenderWindow(void);
    static QVTKWidget* getVTKWidget(void);

    static QString getMultiLineInput(const QString& title,
                              const QString& suggestion, QWidget* parent=0);

    static QItemSelection selectRows(const QAbstractItemModel *model,
                              QList<int>& ids);

    static qreal getLUMASSVersion(void);

private:
    static const std::string ctx;

};

#endif // NMGLOBALHELPER_H

/******************************************************************************
 * Created by Alexander Herzig
 * Copyright 2010-2016 Landcare Research New Zealand Ltd
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
// system and standardSt
#include <limits>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <list>
#include <stack>
#include <map>
#include "math.h"

// GDAL support
#include "gdal.h"
#include "gdal_priv.h"


// NM stuff
//#include "nmlog.h"
#ifndef NM_ENABLE_LOGGER
#   define NM_ENABLE_LOGGER
#   include "nmlog.h"
#   undef NM_ENABLE_LOGGER
#else
#   include "nmlog.h"
#endif


#include "NMMacros.h"
//#include "NMLayer.h"
#include "NMImageLayer.h"
#include "NMVectorLayer.h"
#include "NMMosra.h"
#include "NMTableView.h"
#include "NMSqlTableView.h"
//#include "NMChartWidget.h"
#include "NMImageReader.h"
#include "NMItk2VtkConnector.h"
#include "NMImageLayer.h"
#include "NMItkDataObjectWrapper.h"
#include "NMOtbAttributeTableWrapper.h"
#include "NMQtOtbAttributeTableModel.h"
#include "NMModelComponent.h"
#include "NMProcess.h"
#include "NMMfwException.h"

#include "modelcomponentlist.h"
#include "NMProcCompList.h"
#include "NMFastTrackSelectionModel.h"
#include "NMRATBandMathImageFilterWrapper.h"
#include "NMStreamingImageFileWriterWrapper.h"
#include "NMModelComponentFactory.h"
#include "NMProcessFactory.h"
#include "NMModelSerialiser.h"
#include "NMEditModelComponentDialog.h"
#include "NMModelScene.h"
#include "NMWidgetListView.h"
#include "NMGlobalHelper.h"
#include "NMParameterTable.h"
#include "NMModelViewWidget.h"
#include "NMModelScene.h"
#include "NMProcessComponentItem.h"
#include "NMAggregateComponentItem.h"
#include "NMComponentLinkItem.h"
#include "NMListWidget.h"
#include "NMToolBar.h"
#include "NMAbstractAction.h"
#include "NMModelAction.h"

#include "LpHelper.h"

#include "nmqsql_sqlite_p.h"
#include "nmqsqlcachedresult_p.h"
#include "otbSQLiteTable.h"
#include "avtPolygonToTrianglesTesselator.h"


// QT stuff
#include "lumassmainwin.h"
#include "ui_lumassmainwin.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QPointF>
#include <QPointer>
#include <QtDebug>
#include <QInputDialog>
#include <QHash>
#include <QVariant>
#include <QTime>
#include <QDateTime>
#include <QTableView>
#include <QFuture>
#include <QtConcurrentRun>
#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>
#include <QDialog>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMetaType>
#include <QMetaObject>
#include <QMetaProperty>
#include <QFile>
#include <QtXml>
#include <QTextStream>
#include "qttreepropertybrowser.h"
#include "qteditorfactory.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include <QToolBar>
#include <QToolButton>
#include <QActionGroup>
#include <QScopedPointer>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSplitter>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMouseEvent>

// orfeo
//#include "ImageReader.h"
//#include "ImageDataWrapper.h"
//#include "itkTimeProbe.h"
//#include "itkIndent.h"
//#include "otbGDALRATImageFileReader.h"
//#include "otbImage.h"
//#include "itkShiftScaleImageFilter.h"
//#include "itkCastImageFilter.h"
//#include "itkPasteImageFilter.h"
//#include "otbRATBandMathImageFilter.h"
//#include "itkVTKImageExport.h"
//#include "otbAttributeTable.h"
//#include "otbStreamingRATImageFileWriter.h"
//#include "otbImageFileWriter.h"
//#include "otbImageFileReader.h"
//#include "itkExtractImageFilter.h"
//#include "itkObjectFactoryBase.h"
//#include "itkNMCostDistanceBufferImageFilter.h"
//#include "otbProcessLUPotentials.h"
//#include "itkRescaleIntensityImageFilter.h"
//#include "otbImageIOBase.h"
//#include "otbImage2DToCubeSliceFilter.h"
//#include "otbMetaDataKey.h"
//#include "itkMetaDataObject.h"
//#include "otbFocalDistanceWeightingFilter.h"
//#include "otbPotentialBasedAllocation.h"
//#include "itkArray2D.h"
//#include "itkRandomImageSource.h"
//#include "otbSortFilter.h"
//#include "otbImageRegionAdaptativeSplitter.h"
//#include "itkVectorContainer.h"
//#include "itkStreamingImageFilter.h"
//#include "otbMultiParser.h"
//#include "otbParserX.h"
//#include "mpParser.h"

//#include "otbFlowAccumulationFilter.h"
//#include "otbParallelFillSinks.h"
//#include "otbStreamingRATImageFileWriter.h"

//#include "otbFlowAccumulationFilter.h"
//#include "otbParallelFillSinks.h"
//#include "otbStreamingRATImageFileWriter.h"

// FOR ::test function
//#include "itkFloodFilledImageFunctionConditionalIterator.h"
//#include "itkBinaryThresholdImageFunction.h"
//#include "otbDEMSlopeAspectFilter.h"
//#include "otbFlowAccumulationFilter.h"
//#include "otbSortFilter.h"
//#include "otbExternalSortFilter.h"
//#include "itkImageRegionSplitterMultidimensional.h"
//#include "itkNMImageRegionSplitterMaxSize.h"

// VTK
#include "vtkNew.h"
#include "vtkAbstractArray.h"
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"
#include "vtkDataArray.h"
#include "vtkDataSet.h"
#include "vtkDataSetAttributes.h"
#include "vtkDelimitedTextWriter.h"
#include "vtkDoubleArray.h"
#include "vtkCommand.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkImageData.h"
#include "vtkIntArray.h"
#include "vtkInteractorObserver.h"
#include "vtkInteractorStyle.h"
#include "vtkInteractorStyleImage.h"

#include "vtkInteractorStyleRubberBandZoom.h"
#include "vtkInteractorStyleRubberBand2D.h"
#include "vtkLongArray.h"
#include "vtkMath.h"
#include "vtkMergePoints.h"
#include "vtkObject.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkPoints.h"
#include "vtkPolygon.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkQtTableModelAdapter.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkSQLDatabase.h"
#include "vtkStringArray.h"
#include "vtkTable.h"
#include "vtkUnsignedCharArray.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
//#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkExtractPolyDataGeometry.h"
#include "vtkCylinder.h"
#include "vtkTransform.h"
#include "vtkSphere.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkImageToPolyDataFilter.h"
#include "vtkGeometryFilter.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkImageWrapPad.h"
#include "vtkGreedyTerrainDecimation.h"
#include "vtkPolyDataNormals.h"
#include "vtkCoordinate.h"
//#include "QVTKWidget.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "vtkJPEGWriter.h"
//#include "vtkLegendScaleActor.h"
#include "vtkImageResample.h"
#include "vtkAxisActor2D.h"
#include "vtkTextProperty.h"
#include "vtkTrivialProducer.h"
#include "vtkExtractCells.h"
#include "vtkInformation.h"
#include "vtkGeometryFilter.h"
#include "vtkGDALVectorReader.h"
#include "vtkUnsignedLongArray.h"

#include "QVTKOpenGLWidget.h"

#include "NMSqlTableModel.h"
#include "vtkOpenGLRenderWindow.h"
#include "NMVtkInteractorStyleImage.h"
#include "NMVtkMapScale.h"

//#include <sqlite3.h>
//#include "sqlite3extfunc.h"

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QProcess>


//// TOKYO CABINET
//#include "tcutil.h"
//#include "tchdb.h"
//#include <stdlib.h>
//#include <stdbool.h>
//#include <stdint.h>

//#include "valgrind/callgrind.h"

LUMASSMainWin::LUMASSMainWin(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LUMASSMainWin)
{
    // **********************************************************************
    // *                    META TYPES and other initisalisations           *
    // **********************************************************************

	// some meta type registration for supporting the given types for
	// properties and QVariant
	qRegisterMetaType< QList< QStringList> >();
	qRegisterMetaType< QList< QList< QStringList > > >();
	qRegisterMetaType< NMItkDataObjectWrapper::NMComponentType >();
	qRegisterMetaType< NMProcess::AdvanceParameter >();
	qRegisterMetaType< NMLayer::NMLegendType >();
	qRegisterMetaType< NMLayer::NMLayerType >();
	qRegisterMetaType< NMLayer::NMLegendClassType >();
	qRegisterMetaType< NMLayer::NMColourRamp >();
#ifdef BUILD_RASSUPPORT
	qRegisterMetaType< NMRasdamanConnectorWrapper*>("NMRasdamanConnectorWrapper*");
#endif
	qRegisterMetaType<NMItkDataObjectWrapper>("NMItkDataObjectWrapper");
	qRegisterMetaType<NMOtbAttributeTableWrapper>("NMOtbAttributeTableWrapper");
    qRegisterMetaType<NMModelController*>("NMModelController*");
    qRegisterMetaType<NMAbstractAction::NMOutputMap>("NMAbstractAction::NMOutputMap");
//    qRegisterMetaType<NMAbstractAction::NMActionOutputType>("NMAbstractAction::NMActionOutputType");
//    qRegisterMetaType<NMAbstractAction::NMActionTriggerType>("NMAbstractAction::NMActionTriggerType");

    // **********************************************************************
    // *                    INIT SETTINGS FRAMEWORK
    // **********************************************************************

//#ifdef _WIN32
    QString homepath = QDir::homePath();
//#else
//    QString homepath = "$HOME";
//#endif

    // initially set a directory which we're certain exists ...
    mSettings["Workspace"] = QVariant::fromValue(QString("%1").arg(homepath));
    mSettings["UserModels"] = QVariant::fromValue(QString("%1").arg(homepath));
    mSettings["LUMASSPath"] = qApp->applicationDirPath();

//    sqlite3_temp_directory = const_cast<char*>(
//                mSettings["Workspace"].toString().toStdString().c_str());

    // **********************************************************************
    // *                    INIT SOME ON-DEMAND GUI ELEMENTS
    // **********************************************************************

    mSettingsBrowser = 0;
    this->mbFirstTimeLoaded = true;

#ifdef BUILD_RASSUPPORT
    this->mpRasconn = 0;
    this->mbNoRasdaman = true;
#endif
    this->mbNoRasdaman = false;
    this->mpPetaView = 0;

    this->mbUpdatingExclusiveActions = false;
    this->mLastInfoLayer = 0;
    this->mActiveWidget = 0;


    // **********************************************************************
    // *                    GDAL                                            *
    // **********************************************************************
    GDALAllRegister();
    GetGDALDriverManager()->AutoLoadDrivers();

    // **********************************************************************
    // *                    SQLITE3                                         *
    // **********************************************************************

    //sqlite3_auto_extension((void(*)(void))sqlite3_extmathstrfunc_init);

    // **********************************************************************
    // *                    MAIN WINDOW SETUP & LOGGING & MODELLING         *
    // **********************************************************************

    // seed the std random number generator
    std::srand(std::time(0));

    // set up the logger
    mLogger = new NMLogger(this);
    mLogger->setHtmlMode(true);

    // set up the qt designer based controls
    ui->setupUi(this);
    mLUMASSIcon = QIcon(":lumass-icon.png");
    this->setWindowIcon(mLUMASSIcon);

    // connect logger with log widget
    connect(mLogger, SIGNAL(sendLogMsg(const QString &)), this, SLOT(appendHtmlMsg(const QString &)));

	// **********************************************************************
    // *                    MENU BAR AND DOCKS                              *
	// **********************************************************************

    // ================================================
    // INFO COMPONENT DOCK
    // ================================================

    QTableWidget* tabWidget = new QTableWidget(ui->infoWidgetList);
    tabWidget->setObjectName(QString::fromUtf8("layerInfoTable"));
    tabWidget->setAlternatingRowColors(true);
    ui->infoWidgetList->addWidgetItem(tabWidget, QString::fromUtf8("Layer Attributes"));

    mTreeCompEditor = new NMComponentEditor(ui->infoWidgetList);
    mTreeCompEditor->setObjectName(QString::fromUtf8("treeCompEditor"));
    ui->infoWidgetList->addWidgetItem(mTreeCompEditor, QString::fromUtf8("Component Properties"));

    ui->componentInfoDock->setMinimumWidth(200);
    ui->componentInfoDock->setVisible(true);

    // ================================================
    // LAYER / TABLE / MODEL COMPONENT DOCK (Source)
    // ================================================

    // set up the layer list
    mLayerList = new ModelComponentList(ui->compWidgetList);
    mLayerList->setObjectName(QString::fromUtf8("modelCompList"));
    ui->compWidgetList->addWidgetItem(mLayerList, QString::fromUtf8("Map Layers"));

    // set up the table list
#ifdef QT_HIGHDPI_SUPPORT
    qreal pratio = this->devicePixelRatioF();
#else
    qreal pratio = 1;
#endif
    mTableListWidget = new NMListWidget(ui->compWidgetList);
    mTableListWidget->setLogger(mLogger);
    mTableListWidget->setDragSourceName("StandaloneTableList");
    //mTableListWidget = liwi;
    mTableListWidget->setObjectName(QString::fromLatin1("tableListWidget"));
    mTableListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mTableListWidget->setIconSize(QSize(int((qreal)16*pratio),int((qreal)16*pratio)));

    mTableListWidget->setDragEnabled(false);
    mTableListWidget->setAcceptDrops(true);
    mTableListWidget->setDragDropMode(QAbstractItemView::DropOnly);
    mTableListWidget->viewport()->setObjectName(QString::fromLatin1("tableListView"));
    mTableListWidget->viewport()->setAcceptDrops(true);
    mTableListWidget->viewport()->installEventFilter(this);
    ui->compWidgetList->addWidgetItem(mTableListWidget, QString::fromUtf8("Table Objects"));

    // set up the process component list
    NMProcCompList* procList = new NMProcCompList(ui->compWidgetList);
    procList->setObjectName(QString::fromUtf8("processComponents"));
    ui->compWidgetList->addWidgetItem(procList, QString::fromUtf8("Model Components"));
    ui->componentsWidget->setMinimumWidth(170);

    // set up the user models list
    mUserModelListWidget = new NMListWidget(ui->compWidgetList);
    mUserModelListWidget->setLogger(mLogger);
    mUserModelListWidget->setDragSourceName(QString::fromLatin1("UserModelList"));
    //mUserModelListWidget = umlw;
    mUserModelListWidget->setObjectName(QString::fromLatin1("userModelListWidget"));
    mUserModelListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mUserModelListWidget->setIconSize(QSize(int((qreal)16*pratio),int((qreal)16*pratio)));

    mUserModelListWidget->setDragEnabled(false);
    mUserModelListWidget->setAcceptDrops(true);
    mUserModelListWidget->setDragDropMode(QAbstractItemView::DropOnly);
    mUserModelListWidget->viewport()->setObjectName("userModelListView");
    mUserModelListWidget->viewport()->setAcceptDrops(true);
    mUserModelListWidget->viewport()->installEventFilter(this);
    ui->compWidgetList->addWidgetItem(mUserModelListWidget, QString::fromUtf8("User Models"));


    // ================================================
    // LOG DOCK
    // ================================================
    ui->logDock->setVisible(false);


    // ================================================
    // BAR(s) SETUP - MENU - PROGRESS - STATUS
    // ================================================

    // we remove the rasdaman import option, when we haven't
    // rasdaman suppor
#ifndef BUILD_RASSUPPORT
    ui->menuObject->removeAction(ui->actionImportRasdamanLayer);
#endif
#ifndef DEBUG
    ui->menuGIS_ish->menuAction()->setVisible(false);
    ui->menuMOSO->removeAction(ui->actionTest);
#endif

    // since we havent' go an implementation for odbc import
    // we just remove the action for now
    //ui->menuObject->removeAction(ui->actionopenCreateTable);

    // add a label to the status bar for displaying
    // the coordinates of the map window
    this->m_coordLabel = new QLabel("", this, 0);
    this->mPixelValLabel = new QLabel("", this, 0);
    this->ui->statusBar->addWidget(m_coordLabel);
    this->ui->statusBar->addWidget(mPixelValLabel);

    // add a label to show random messages in the status bar
    this->mBusyProcCounter = 0;
    this->m_StateMsg = new QLabel("",  this, 0);
    this->ui->statusBar->addWidget(this->m_StateMsg);
    // progress bar
    this->mProgressBar = new QProgressBar(this);
    this->mProgressBar->setMaximumHeight(15);
    this->mProgressBar->setVisible(false);
    this->ui->statusBar->addPermanentWidget(mProgressBar, 1);

    // connect menu actions to member functions
#ifdef BUILD_RASSUPPORT
	connect(ui->actionImportRasdamanLayer, SIGNAL(triggered()), this, SLOT(loadRasdamanLayer()));
#endif
    connect(ui->actionLoad_Layer, SIGNAL(triggered()), this, SLOT(loadImageLayer()));
    connect(ui->actionImport_3D_Point_Set, SIGNAL(triggered()), this, SLOT(import3DPointSet()));
    connect(ui->actionToggle3DStereoMode, SIGNAL(triggered()), this, SLOT(toggle3DStereoMode()));
    connect(ui->actionToggle3DSimpleMode, SIGNAL(triggered()), this, SLOT(toggle3DSimpleMode()));
    connect(ui->actionLoad_VTK_PolyData, SIGNAL(triggered()), this, SLOT(loadVTKPolyData()));
    connect(ui->actionLoad_Vector_Layer, SIGNAL(triggered()), this, SLOT(loadVectorLayer()));
    connect(ui->actionMOSO, SIGNAL(triggered()), this, SLOT(doMOSO()));
    connect(ui->actionTreeFindLoops, SIGNAL(triggered()), this, SLOT(treeFindLoops()));
    connect(ui->actionTreeTopDown, SIGNAL(triggered()), this, SLOT(treeSelTopDown()));
    connect(ui->actionTreeBottomUp, SIGNAL(triggered()), this, SLOT(treeSelBottomUp()));

    //connect(ui->actionMOSO_batch, SIGNAL(triggered()), this, SLOT(doMOSObatch()));
    connect(ui->actionComponents_View, SIGNAL(toggled(bool)), this, SLOT(showComponentsView(bool)));
    connect(ui->actionShow_Components_Info, SIGNAL(toggled(bool)), this, SLOT(showComponentsInfoView(bool)));
    connect(ui->actionShow_Notifications, SIGNAL(toggled(bool)), this, SLOT(showNotificationsView(bool)));

    //connect(ui->actionModel_View, SIGNAL(triggered()), this, SLOT(showModelView()));
    connect(ui->actionShowScaleBar, SIGNAL(toggled(bool)), this, SLOT(showScaleBar(bool)));
    connect(ui->actionShowCoordinateAxes, SIGNAL(toggled(bool)), this, SLOT(showCoordinateAxes(bool)));
    connect(ui->actionRemoveAllObjects, SIGNAL(triggered()), this, SLOT(removeAllObjects()));
    connect(ui->actionFullExtent, SIGNAL(triggered()), this, SLOT(zoomFullExtent()));
    connect(ui->actionSaveAsVTKPolyData, SIGNAL(triggered()), this, SLOT(saveAsVtkPolyData()));
    connect(ui->actionSave_As_Image_File, SIGNAL(triggered()), this ,SLOT(saveImageFile()));
    connect(ui->actionSaveAsImage, SIGNAL(triggered()), this, SLOT(saveMapAsImage()));
    connect(ui->actionSave_Visible_Extent_Overview_As, SIGNAL(triggered()), this ,SLOT(saveImageFile()));
    connect(ui->actionTest, SIGNAL(triggered()), this, SLOT(test()));
    connect(ui->actionSaveAsVectorLayerOGR, SIGNAL(triggered()), this, SLOT(saveAsVectorLayerOGR()));
    connect(ui->actionImportTableObject, SIGNAL(triggered()), this, SLOT(importTableObject()));
    connect(ui->actionLUMASS, SIGNAL(triggered()), this, SLOT(aboutLUMASS()));
    connect(ui->actionBackground_Colour, SIGNAL(triggered()), this,
            SLOT(setMapBackgroundColour()));

    connect(ui->actionShow_Map_View, SIGNAL(toggled(bool)), this, SLOT(showMapView(bool)));
    connect(ui->actionShow_Model_View, SIGNAL(toggled(bool)), this, SLOT(showModelView(bool)));
    connect(ui->actionMap_View_Mode, SIGNAL(triggered()), this, SLOT(mapViewMode()));
    connect(ui->actionModel_View_Mode, SIGNAL(triggered()), this, SLOT(modelViewMode()));
    connect(ui->actionShowTable, SIGNAL(triggered()), this, SLOT(showTable()));

    connect(ui->componentsWidget, SIGNAL(visibilityChanged(bool)),
            ui->actionComponents_View, SLOT(setChecked(bool)));
    connect(ui->componentInfoDock, SIGNAL(visibilityChanged(bool)),
            ui->actionShow_Components_Info, SLOT(setChecked(bool)));
    connect(ui->logDock, SIGNAL(visibilityChanged(bool)),
            ui->actionShow_Notifications, SLOT(setChecked(bool)));
    connect(ui->actionConfigure_Settings, SIGNAL(triggered()), this, SLOT(configureSettings()));

    // USER TOOL SUPPORT
    connect(ui->actionAdd_Toolbar, SIGNAL(triggered()), this, SLOT(addUserToolBar()));


    // SYSTEM
    connect(this, SIGNAL(windowLoaded()), this, SLOT(readSettings()));

    // TEST TEST TEST
    connect(ui->actionImage_Polydata, SIGNAL(triggered()), this, SLOT(convertImageToPolyData()));


    // =============================================================
    //                      MAIN TOOL BAR
    // =============================================================
    this->ui->mainToolBar->setWindowTitle("Main LUMASS Tools");
    this->ui->mainToolBar->setObjectName("MainTools");

    // .....................
    // zoom actions
    QIcon zoomInIcon(":zoom-in-icon.png");
    QAction* zoomInAction = new QAction(zoomInIcon, "Zoom In", this->ui->mainToolBar);
    zoomInAction->setObjectName("zoomInAction");
    zoomInAction->setCheckable(true);
    mExclusiveActions.append(zoomInAction);
    //zoomInAction->setAutoRepeat(true);

    QIcon zoomOutIcon(":zoom-out-icon.png");
    QAction* zoomOutAction = new QAction(zoomOutIcon, "Zoom Out", this->ui->mainToolBar);
    zoomOutAction->setObjectName("zoomOutAction");
    zoomOutAction->setCheckable(true);
    mExclusiveActions.append(zoomOutAction);

    QIcon zoomContentIcon(":zoom-fit-best-icon.png");
    QAction* zoomToContentAct = new QAction(zoomContentIcon, "Zoom To Content", this->ui->mainToolBar);

    // ..........................
    // component management actions
    QIcon moveIcon(":move-icon.png");
    QAction* moveAction = new QAction(moveIcon, "Pan Map/Model",
            this->ui->mainToolBar);
    moveAction->setObjectName("panAction");
    moveAction->setCheckable(true);
    mExclusiveActions.append(moveAction);

    QIcon linkIcon(":link-icon.png");
    QAction* linkAction = new QAction(linkIcon, "Link Components", this->ui->mainToolBar);
    linkAction->setObjectName("linkAction");
    linkAction->setCheckable(true);
    mExclusiveActions.append(linkAction);

    QIcon selIcon(":select.png");
    QAction* selAction = new QAction(selIcon, "Select Features/Components", this->ui->mainToolBar);
    selAction->setObjectName("selAction");
    selAction->setCheckable(true);
    mExclusiveActions.append(selAction);

    QIcon clearIcon(":clear.png");
    QAction* clearAction = new QAction(clearIcon, "Clear Selection", this->ui->mainToolBar);
    clearAction->setObjectName("clearAction");

    // .....................................
    // main windows orientation action

    QIcon mapIcon(":view-map.png");
    QAction* actMapBtn = new QAction(mapIcon, "Map View", ui->mainToolBar);
    mMiscActions["actMapBtn"] = actMapBtn;
    actMapBtn->setObjectName("actMapBtn");
    actMapBtn->setCheckable(true);
    actMapBtn->setChecked(true);

    QIcon modelIcon(":model-icon.png");
    QAction* actModelBtn = new QAction(modelIcon, "Model View", ui->mainToolBar);
    mMiscActions["actModelBtn"] = actModelBtn;
    actModelBtn->setObjectName("actModelBtn");
    actModelBtn->setCheckable(true);
    actModelBtn->setChecked(true);


    QActionGroup* windowLayoutGroup = new QActionGroup(this->ui->mainToolBar);

    QIcon horzIcon(":view-split-left-right.png");
    QAction* actHorzLayout = new QAction(horzIcon, "Views: Left | Right", ui->mainToolBar);
    mMiscActions["actHorzLayout"] = actHorzLayout;
    actHorzLayout->setObjectName("actHorzLayout");
    actHorzLayout->setCheckable(true);
    actHorzLayout->setChecked(true);

    QIcon vertIcon(":view-split-top-bottom.png");
    QAction* actVertLayout = new QAction(vertIcon, "Views: Top | Bottom", ui->mainToolBar);
    mMiscActions["actVertLayout"] = actVertLayout;
    actVertLayout->setObjectName("actVertLayout");
    actVertLayout->setCheckable(true);
    actVertLayout->setChecked(false);

    windowLayoutGroup->addAction(actHorzLayout);
    windowLayoutGroup->addAction(actVertLayout);

    // ..........................
    // model execution actions
    QIcon execIcon(":model-execute-icon.png");
    QAction* execAction = new QAction(execIcon, "Execute Model", this->ui->mainToolBar);

    QIcon stopIcon(":model-stop-icon.png");
    QAction* stopAction = new QAction(stopIcon, "Stop Model Execution", this->ui->mainToolBar);

    QIcon resetIcon(":model-reset-icon.png");
    QAction* resetAction = new QAction(resetIcon, "Reset Model",  this->ui->mainToolBar);

    this->ui->mainToolBar->addAction(zoomInAction);
    this->ui->mainToolBar->addAction(zoomOutAction);
    this->ui->mainToolBar->addAction(zoomToContentAct);
    this->ui->mainToolBar->addAction(moveAction);

    this->ui->mainToolBar->addSeparator();
    this->ui->mainToolBar->addAction(selAction);
    this->ui->mainToolBar->addAction(clearAction);

    this->ui->mainToolBar->addSeparator();
    this->ui->mainToolBar->addAction(linkAction);
    this->ui->mainToolBar->addAction(resetAction);
    this->ui->mainToolBar->addAction(stopAction);
    this->ui->mainToolBar->addAction(execAction);

    this->ui->mainToolBar->addSeparator();
    this->ui->mainToolBar->addAction(actMapBtn);
    this->ui->mainToolBar->addAction(actModelBtn);
    this->ui->mainToolBar->addActions(windowLayoutGroup->actions());

    //this->ui->mainToolBar->addSeparator();
//    QLineEdit* searchedit = new QLineEdit(ui->mainToolBar);
//    searchedit->setPlaceholderText("Enter ComponentName or UserID");
//    QAction *actCompSearch = ui->mainToolBar->addWidget(searchedit);
//    actCompSearch->setText("Search Model");
//    connect(searchedit, SIGNAL(returnPressed()), this,
//            SLOT(searchModelComponent()));

    this->ui->mainToolBar->installEventFilter(this);


    // connect model view widget signals / slots
    connect(linkAction, SIGNAL(toggled(bool)),
    		this->ui->modelViewWidget, SIGNAL(linkToolToggled(bool)));
    connect(linkAction, SIGNAL(toggled(bool)),
            this, SLOT(toggleLinkTool(bool)));
    connect(selAction, SIGNAL(toggled(bool)),
            this->ui->modelViewWidget, SIGNAL(selToolToggled(bool)));
    connect(selAction, SIGNAL(toggled(bool)),
            this, SLOT(toggleSelectTool(bool)));

    connect(moveAction, SIGNAL(toggled(bool)),
            this->ui->modelViewWidget, SIGNAL(moveToolToggled(bool)));
    connect(moveAction, SIGNAL(toggled(bool)),
            this, SLOT(pan(bool)));

    connect(this, SIGNAL(noExclusiveToolSelected()),
            this->ui->modelViewWidget, SIGNAL(idleMode()));
    connect(clearAction, SIGNAL(triggered()),
            this->ui->modelViewWidget, SIGNAL(unselectItems()));
    connect(clearAction, SIGNAL(triggered()),
            this, SLOT(clearSelection()));

    connect(zoomToContentAct, SIGNAL(triggered()), this, SLOT(zoomToContent()));
    connect(zoomInAction, SIGNAL(toggled(bool)), this, SLOT(zoomIn(bool)));
    connect(zoomInAction, SIGNAL(toggled(bool)),
            this->ui->modelViewWidget, SIGNAL(zoomInToolToggled(bool)));
    connect(zoomOutAction, SIGNAL(toggled(bool)), this, SLOT(zoomOut(bool)));
    connect(zoomOutAction, SIGNAL(toggled(bool)),
            this->ui->modelViewWidget, SIGNAL(zoomOutToolToggled(bool)));

    connect(actMapBtn, SIGNAL(toggled(bool)), this, SLOT(showMapView(bool)));
    connect(actModelBtn, SIGNAL(toggled(bool)), this, SLOT(showModelView(bool)));

    connect(execAction, SIGNAL(triggered()), this->ui->modelViewWidget, SLOT(executeModel()));
    connect(stopAction, SIGNAL(triggered()), this->ui->modelViewWidget, SIGNAL(requestModelAbortion()));
    connect(resetAction, SIGNAL(triggered()), this->ui->modelViewWidget, SLOT(resetModel()));

    connect(windowLayoutGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(swapWindowLayout(QAction*)));

    // **********************************************************************
	// *                    VTK DISPLAY WIDGET                              *
	// **********************************************************************

	// suppress the vtkOutputWindow to pop up
#ifdef _WIN32
	vtkObject::GlobalWarningDisplayOff();
#endif

    // create the render window
    //vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renwin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renwin->SetStereoCapableWindow(1);
    renwin->SetStereoTypeToCrystalEyes();
    //renwin->SetStereoRender(1);
    //NMDebugAI(<< renwin->ReportCapabilities() << std::endl);

    // set the number of allowed layers in the window
    renwin->SetAlphaBitPlanes(1);
    renwin->SetMultiSamples(0);
    renwin->SetNumberOfLayers(2);

	// set-up the background renderer
	this->mBkgRenderer = vtkSmartPointer<vtkRenderer>::New();
	this->mBkgRenderer->SetLayer(0);
    //this->mBkgRenderer->SetBackground(1,1,1);
    this->mBkgRenderer->SetBackground(0.7,0.7,0.7);
	//this->mBkgRenderer->SetUseDepthPeeling(1);
	//this->mBkgRenderer->SetMaximumNumberOfPeels(100);
	//this->mBkgRenderer->SetOcclusionRatio(0.1);

    this->mScaleRenderer = vtkSmartPointer<vtkRenderer>::New();
    this->mScaleRenderer->SetActiveCamera(mBkgRenderer->GetActiveCamera());
    this->mScaleRenderer->SetLayer(1);

    vtkSmartPointer<NMVtkMapScale> legendActor =
            vtkSmartPointer<NMVtkMapScale>::New();

    legendActor->AllAxesOff();
    legendActor->SetLabelModeToXYCoordinates();
    legendActor->SetLegendVisibility(0);
    legendActor->GetLegendTitleProperty()->SetFontSize(12);
    legendActor->GetLegendTitleProperty()->SetItalic(0);
    legendActor->GetLegendLabelProperty()->SetFontSize(10);
    legendActor->GetLegendLabelProperty()->SetItalic(0);
    legendActor->SetVisibility(1);
    legendActor->SetDPI(renwin->GetDPI());

    //legendActor->SetLabelModeToDistance();

    this->mScaleRenderer->AddViewProp(legendActor);


    //renwin->SetStereoTypeToCrystalEyes();
    //this->mBkgRenderer->SetBackground(0,0,0);
	renwin->AddRenderer(this->mBkgRenderer);
    renwin->AddRenderer(this->mScaleRenderer);

    // set the render window
		// for supporting 3D mice (3DConnexion Devices)
		//this->ui->qvtkWidget->SetUseTDx(true);
    this->ui->qvtkWidget->SetRenderWindow(renwin);

    // listen in on events received by the QtVTKWidget
    this->ui->qvtkWidget->setAcceptDrops(true);
    this->ui->qvtkWidget->installEventFilter(this);


    this->m_b3D = false;
    this->ui->actionToggle3DStereoMode->setChecked(false);
    this->ui->actionToggle3DStereoMode->setEnabled(false);

	// display orientation marker axes
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetShaftTypeToLine();
	axes->SetXAxisLabelText("X");
	axes->SetYAxisLabelText("Y");
	axes->SetZAxisLabelText("Z");
	axes->SetTotalLength(10,10,10);
	axes->SetOrigin(0,0,0);

    NMVtkInteractorStyleImage* iasm = NMVtkInteractorStyleImage::New();
#ifdef QT_HIGHDPI_SUPPORT
    iasm->setDevicePixelRatio(this->devicePixelRatioF());
#endif
    this->ui->qvtkWidget->GetInteractor()->SetInteractorStyle(iasm);

	m_orientwidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	m_orientwidget->SetOrientationMarker(axes);
	m_orientwidget->SetInteractor(static_cast<vtkRenderWindowInteractor*>(this->ui->qvtkWidget->GetInteractor()));
	m_orientwidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	m_orientwidget->InteractiveOff();
	m_orientwidget->SetEnabled(0);

    // QVTKWidget Events---------------------------------------------------------
    this->m_vtkConns = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    this->m_vtkConns->Connect(ui->qvtkWidget->GetRenderWindow()->GetInteractor(),
    		vtkCommand::MouseMoveEvent,
    		this, SLOT(updateCoords(vtkObject*)));
    this->m_vtkConns->Connect(ui->qvtkWidget->GetRenderWindow()->GetInteractor(),
    		vtkCommand::LeftButtonPressEvent,
    		this, SLOT(pickObject(vtkObject*)));
    this->m_vtkConns->Connect(ui->qvtkWidget->GetRenderWindow()->GetInteractor(),
            vtkCommand::MiddleButtonPressEvent,
            this, SLOT(pickObject(vtkObject*)));

//    this->m_vtkConns->Connect(ui->qvtkWidget->GetRenderWindow()->GetInteractor(),
//            vtkCommand::LeftButtonPressEvent,
//            this, SLOT(userPick(vtkObject*)));

    // **********************************************************************
    // *                    CENTRAL WIDGET                                  *
    // **********************************************************************

    QVBoxLayout* boxL = new QVBoxLayout();
    if (ui->centralWidget->layout())
        delete ui->centralWidget->layout();

    ui->centralWidget->setLayout(boxL);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, ui->centralWidget);
    splitter->setObjectName("MainSplitter");
    splitter->setChildrenCollapsible(true);
    splitter->addWidget(ui->qvtkWidget);
    splitter->addWidget(ui->modelViewWidget);
    boxL->addWidget(splitter);
    QSize totalSize = splitter->size();
    QList<int> sizes;
    sizes << totalSize.width() / 2 << totalSize.width() / 2;
    splitter->setSizes(sizes);


    connect(ui->modelViewWidget, SIGNAL(modelViewActivated(QObject *)),
            this, SLOT(modelViewActivated(QObject *)));

    connect(ui->logEdit, SIGNAL(anchorClicked(QUrl)),
            ui->modelViewWidget, SLOT(zoomToComponent(QUrl)));

    ui->modelViewWidget->setLogger(mLogger);

    // ================================================
    // INITIAL WIDGET's VISIBILITY
    // ================================================

    this->ui->componentInfoDock->setVisible(true);
    this->ui->actionShow_Components_Info->setChecked(true);

    this->ui->infoWidgetList->setWidgetItemVisible(1, false);

    this->ui->componentsWidget->setVisible(true);
    this->ui->actionComponents_View->setChecked(true);

    // set menu's check buttons to right state
    this->ui->actionShow_Map_View->setChecked(true);
    this->ui->actionShow_Model_View->setChecked(true);

    this->ui->modelViewWidget->updateToolContextBox();

}

LUMASSMainWin::~LUMASSMainWin()
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

	this->removeAllObjects();

    GDALDestroyDriverManager();

#ifdef BUILD_RASSUPPORT
	// close the table view and delete;
	if (this->mpPetaView != 0)
	{
		this->mpPetaView->close();
		delete this->mpPetaView;
	}


	if (this->mpRasconn)
		delete this->mpRasconn;
#endif

    // wait for the logging thread to quit
    emit isAboutToClose();
    //mLoggingThread->wait();

	NMDebugAI(<< "delete ui ..." << endl);
	delete ui;

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void LUMASSMainWin::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        mLastToolBar.clear();
    }
}

bool
LUMASSMainWin::notify(QObject* receiver, QEvent* event)
{
	try
	{
		return qApp->notify(receiver, event);
	}
	catch (std::exception& e)
	{
        NMLogError(<< "Exception thrown: " << e.what() << endl);
	}

	return true;
}

bool
LUMASSMainWin::eventFilter(QObject *obj, QEvent *event)
{
//    NMDebugAI(<< obj->objectName().toStdString() << " sends "
//              << event->type() << std::endl);

    // ===========================================================
    //                      QVTKWIDGET EVENTS
    // ===========================================================
    if (obj->objectName().compare(QString("qvtkWidget")) == 0)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (!ke)
                return false;

            vtkRenderWindow* renwin = this->ui->qvtkWidget->GetRenderWindow();
            if (!renwin)
                return false;

            if (    ke->modifiers().testFlag(Qt::ControlModifier)
                    &&  ke->modifiers().testFlag(Qt::AltModifier)
                    &&  this->m_b3D && renwin->GetStereoRender())
            {

                if (ke->key() == Qt::Key_C)
                {
                    renwin->SetStereoTypeToCrystalEyes();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Crystal Eye");
                }
                else if (ke->key() == Qt::Key_Y)
                {
                    renwin->SetStereoTypeToAnaglyph();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Anaglyph");
                }
                else if (ke->key() == Qt::Key_I)
                {
                    renwin->SetStereoTypeToInterlaced();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Interlaced");
                }
                else if (ke->key() == Qt::Key_B)
                {
                    renwin->SetStereoTypeToRedBlue();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Red-Blue");
                }
                else if (ke->key() == Qt::Key_G)
                {
                    renwin->SetStereoTypeToSplitViewportHorizontal();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Split Viewport Horizontal");
                }
                else if (ke->key() == Qt::Key_D)
                {
                    renwin->SetStereoTypeToDresden();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Dresden");
                }
                else if (ke->key() == Qt::Key_U)
                {
                    renwin->SetStereoTypeToCheckerboard();
                    renwin->StereoUpdate();
                    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                                          NMLogger::NM_LOG_INFO,
                                          "Stereo mode switched to Checkerboard");
                }
            }
        }
        else if (event->type() == QEvent::DragEnter)
        {
            QDragEnterEvent* dee = static_cast<QDragEnterEvent*>(event);
            if (dee)
            {
                this->mLayerList->dragEnterEvent(dee);
            }
        }
        else if (event->type() == QEvent::DragMove)
        {
            QDragMoveEvent* dme = static_cast<QDragMoveEvent*>(event);
            if (dme)
            {
                this->mLayerList->dragMoveEvent(dme);
            }
        }
        else if (event->type() == QEvent::Drop)
        {
            QDropEvent* de = static_cast<QDropEvent*>(event);
            if (de)
            {
                this->mLayerList->dropEvent(de);
            }
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            mActiveWidget = this->ui->qvtkWidget;
            QAction* selAction = ui->mainToolBar->findChild<QAction*>("selAction");
            if (selAction)
            {
                selAction->setEnabled(false);
            }
        }
    }
    // ===========================================================
    //                      TableListWidget
    // ===========================================================
    else if (obj->objectName().compare(QString("tableListView")) == 0)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            Qt::MouseButton mbtn = me->button();
            QPoint gpos = me->globalPos();
            QPoint lpos = me->pos();
            if (me)
            {
                QListWidgetItem* item = mTableListWidget->itemAt(me->pos());
                if (item)
                {
                    if (mbtn == Qt::RightButton)
                    {
                        this->removeTableObject(item, gpos);
                    }
                    else if (mbtn == Qt::LeftButton)
                    {
                        NMDebugAI(<< "got the left button!\n");
                        QRect rect = mTableListWidget->visualItemRect(item);
                        QSize isize = mTableListWidget->iconSize();
                        if (lpos.x() <= rect.x() + isize.width())
                        {
                            this->tableObjectVisibility(item);
                        }
                    }
                }
            }
        }
        else if (event->type() == QEvent::DragEnter)
        {
            QDragEnterEvent* dee = static_cast<QDragEnterEvent*>(event);
            if (dee)
            {
                this->mLayerList->dragEnterEvent(dee);
            }
        }
        else if (event->type() == QEvent::DragMove)
        {
            QDragMoveEvent* dme = static_cast<QDragMoveEvent*>(event);
            if (dme)
            {
                this->mLayerList->dragMoveEvent(dme);
            }
        }
        else if (event->type() == QEvent::Drop)
        {
            QDropEvent* de = static_cast<QDropEvent*>(event);
            if (de)
            {
                this->mLayerList->dropEvent(de);
            }
        }

    }
    // ===========================================================
    //                      UserModelListWidget (VIEW)
    // ===========================================================
    else if (obj->objectName().compare(QString("userModelListView")) == 0)
    {
        if (event->type() == QEvent::DragEnter)
        {
            QDragEnterEvent* dee = static_cast<QDragEnterEvent*>(event);
            if (dee && !checkMimeDataForModelComponent(dee->mimeData()).isEmpty())
            {
                dee->acceptProposedAction();
            }
        }
        else if (event->type() == QEvent::DragMove)
        {
            QDragMoveEvent* dme = static_cast<QDragMoveEvent*>(event);
            if (dme)
            {
                dme->acceptProposedAction();
            }
        }
        else if (event->type() == QEvent::Drop)
        {
            QDropEvent* de = static_cast<QDropEvent*>(event);
            QString compName = checkMimeDataForModelComponent(de->mimeData());
            if (de && !compName.isEmpty())
            {
                NMModelComponent* mc = NMGlobalHelper::getModelController()->getComponent(compName);
                if (mc)
                {
                    addModelToUserModelList(compName);
                    de->accept();
                    return true;
                }
            }
        }
    }
    // ===============
    // MAIN TOOL BAR
    // ===============
    else if (obj->objectName().compare(QString::fromLatin1("MainTools")) == 0)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            if (me->buttons() & Qt::RightButton)
            {
                mLastToolBar.clear();
            }
        }
    }

    return false;
}

QString
LUMASSMainWin::checkMimeDataForModelComponent(const QMimeData *mimedata)
{
    QString compName;

    QString dropSource;
    QString dropComponent;
    if (mimedata->hasFormat("text/plain"))
    {
        QString ts = mimedata->text();
        QStringList tl = ts.split(':', QString::SkipEmptyParts);
        if (tl.count() == 2)
        {
            dropSource = tl.at(0);
            dropComponent = tl.at(1);
        }
    }

    if (   dropSource.startsWith(QString::fromLatin1("_NMModelScene_"))
        && NMGlobalHelper::getModelController()->contains(dropComponent)
       )
    {
        NMLogDebug(<< "on the hook: " << dropSource.toStdString()
               << ":" << dropComponent.toStdString());

        compName = dropComponent;
    }

    return compName;
}


void
LUMASSMainWin::modelViewActivated(QObject* obj)
{
    mActiveWidget = obj;
    QAction* selAction = ui->mainToolBar->findChild<QAction*>("selAction");
    if (selAction)
    {
        selAction->setEnabled(true);
    }
}

void
LUMASSMainWin::updateExclusiveActions(const QString &checkedAction,
                                      bool toggled)
{
    if (mbUpdatingExclusiveActions)
    {
        return;
    }

    mbUpdatingExclusiveActions = true;
    for (int a=0; a < mExclusiveActions.size(); ++a)
    {
        QAction* act = mExclusiveActions.at(a);
        if (act->objectName().compare(checkedAction) != 0)
        {
            // we explicitly toggle here since the
            // model view tools set internal properties
            // based on the toggle message; so just
            // updating the 'tool button' is not enough
            if (act->isChecked())
            {
                act->toggle();
            }
        }
    }
    if (!toggled)
    {
        // let the NMModelViewWidget know
        emit noExclusiveToolSelected();

        NMLogDebug(<< "no exclusive tool selected!");

        // set map interactor multi mode
        NMVtkInteractorStyleImage* iact = NMVtkInteractorStyleImage::SafeDownCast(
                    ui->qvtkWidget->GetInteractor()->GetInteractorStyle());
        if (iact)
        {
            iact->setNMInteractorMode(NMVtkInteractorStyleImage::NM_INTERACT_MULTI);
        }
    }
    mbUpdatingExclusiveActions = false;
}

void
LUMASSMainWin::zoomToContent()
{
    // work out which view we're working on
    if (mActiveWidget)
    {
        if (mActiveWidget->objectName().compare(QString("qvtkWidget")) == 0)
        {
            this->zoomFullExtent();
        }
        else if (mActiveWidget->objectName().compare(QString("modelViewWidget")) == 0)
        {
            this->ui->modelViewWidget->zoomToContent();
        }
    }
}

void
LUMASSMainWin::pan(bool toggled)
{
    QAction* in = ui->mainToolBar->findChild<QAction*>("panAction");
    if (in)
    {
        in->setChecked(toggled);
        if (toggled)
        {
            NMVtkInteractorStyleImage* iact = NMVtkInteractorStyleImage::SafeDownCast(
                        ui->qvtkWidget->GetInteractor()->GetInteractorStyle());
            if (iact)
            {
                iact->setNMInteractorMode(NMVtkInteractorStyleImage::NM_INTERACT_PAN);
                iact->setUserTool("");
            }
        }
        this->updateExclusiveActions(in->objectName(), toggled);
    }
}

void
LUMASSMainWin::zoomIn(bool toggled)
{
    QAction* in = ui->mainToolBar->findChild<QAction*>("zoomInAction");
    if (in)
    {
        in->setChecked(toggled);
        if (toggled)
        {
            NMVtkInteractorStyleImage* iact = NMVtkInteractorStyleImage::SafeDownCast(
                        ui->qvtkWidget->GetInteractor()->GetInteractorStyle());
            if (iact)
            {
                iact->setNMInteractorMode(NMVtkInteractorStyleImage::NM_INTERACT_ZOOM_IN);
                iact->setUserTool("");
            }
        }
        this->updateExclusiveActions(in->objectName(), toggled);
    }
}

void
LUMASSMainWin::zoomOut(bool toggled)
{
    QAction* out = ui->mainToolBar->findChild<QAction*>("zoomOutAction");
    if (out)
    {
        out->setChecked(toggled);
        if (toggled)
        {
            NMVtkInteractorStyleImage* iact = NMVtkInteractorStyleImage::SafeDownCast(
                        ui->qvtkWidget->GetInteractor()->GetInteractorStyle());
            if (iact)
            {
                iact->setNMInteractorMode(NMVtkInteractorStyleImage::NM_INTERACT_ZOOM_OUT);
                iact->setUserTool("");
            }
        }
        this->updateExclusiveActions(out->objectName(), toggled);
    }
}

void
LUMASSMainWin::clearSelection()
{
    NMLayer* l = this->mLayerList->getSelectedLayer();
    if (l)
    {
        l->selectCell(-1, NMLayer::NM_SEL_CLEAR);
    }
}

void
LUMASSMainWin::toggleSelectTool(bool toggled)
{
    QAction* sel = ui->mainToolBar->findChild<QAction*>("selAction");
    if (sel)
    {
        sel->setChecked(toggled);
        this->updateExclusiveActions(sel->objectName(), toggled);
    }
}

void
LUMASSMainWin::toggleLinkTool(bool toggled)
{
    QAction* link = ui->mainToolBar->findChild<QAction*>("linkAction");
    if (link)
    {
        link->setChecked(toggled);
        this->updateExclusiveActions(link->objectName(), toggled);
    }
}

void
LUMASSMainWin::appendHtmlMsg(const QString& msg)
{
    ui->logEdit->insertHtml(msg);
    ui->logEdit->ensureCursorVisible();
}

void
LUMASSMainWin::appendLogMsg(const QString& msg)
{
    ui->logEdit->textCursor().insertText(msg);
    ui->logEdit->ensureCursorVisible();
}

NMLogWidget*
LUMASSMainWin::getLogWidget()
{
    return ui->logEdit;
}

//NMModelController*
//LUMASSMainWin::getModelController(void)
//{
//    return mModelController;
//}

void
LUMASSMainWin::swapWindowLayout(QAction* act)
{
    QSplitter* splitter = this->ui->centralWidget->findChild<QSplitter*>("MainSplitter");
    if (splitter)
    {
        if (act->text().compare("Views: Left | Right") == 0 && act->isChecked())
        {
            splitter->setOrientation(Qt::Horizontal);
        }
        else if (act->text().compare("Views: Top | Bottom") == 0 && act->isChecked())
        {
            splitter->setOrientation(Qt::Vertical);
        }
    }
}

void
LUMASSMainWin::showScaleBar(bool bshow)
{
    vtkProp* prop = this->mScaleRenderer->GetViewProps()->GetLastProp();
    if (prop)
    {
        NMVtkMapScale* ms = NMVtkMapScale::SafeDownCast(prop);
        ms->SetLegendVisibility(bshow ? 1 : 0);
    }
}

void
LUMASSMainWin::showCoordinateAxes(bool bshow)
{
    vtkProp* prop = this->mScaleRenderer->GetViewProps()->GetLastProp();
    if (prop)
    {
        NMVtkMapScale* ms = NMVtkMapScale::SafeDownCast(prop);
        if (bshow)
        {
            ms->AllAxesOn();
        }
        else
        {
            ms->AllAxesOff();
        }
    }
}

void
LUMASSMainWin::showTable()
{
    if (mTableList.size() == 0)
    {
        return;
    }

    QStringList tables = mTableList.keys();


    bool bOk = false;
    QInputDialog ipd(this);
    ipd.setOption(QInputDialog::UseListViewForComboBoxItems);
    ipd.setComboBoxItems(tables);
    ipd.setComboBoxEditable(false);
    ipd.setWindowTitle("Show Table");
    ipd.setLabelText("Select table");
    int ret = ipd.exec();

    QString tableName = ipd.textValue();

    if (!ret || tableName.isEmpty())
    {
        return;
    }

    NMSqlTableView* tview = mTableList.find(tableName).value().second.data();
    tview->show();
    tview->raise();

}

void
LUMASSMainWin::setMapBackgroundColour()
{
    double* bc = this->mBkgRenderer->GetBackground();
    QColor bclr;
    bclr.setRgbF(bc[0], bc[1], bc[2]);
    bclr = QColorDialog::getColor(bclr, this, "Background Colour");
    this->mBkgRenderer->SetBackground(bclr.redF(),
                                      bclr.greenF(),
                                      bclr.blueF());
}

vtkRenderWindow*
LUMASSMainWin::getRenderWindow(void)
{
    return this->ui->qvtkWidget->GetRenderWindow();
}

void
LUMASSMainWin::saveMapAsImage()
{

    QString fileName = QFileDialog::getSaveFileName(this,
            QString("Save Map As Image File"),
            QString("map.png"), tr("Portable Network Graphic (*.png);;JPEG Image (*.jpg *.jpeg)"));
    if (fileName.isEmpty())
    {
        return;
    }

    vtkRenderWindow* renwin = this->ui->qvtkWidget->GetRenderWindow();

    NMDebugAI(<< "renwin size: " << renwin->GetSize()[0] << " x "
              << renwin->GetSize()[1] << std::endl);

    QString label = "Output size with magnification factor 1 is ";
    label = QString("%1 %2x%3. Current factor: ")
            .arg(label)
            .arg(renwin->GetSize()[0])
            .arg(renwin->GetSize()[1]);

    bool bok;
    int factor = QInputDialog::getInt(this, "Output Magnification Factor",
                             label, 1, 1, 10, 1, &bok);
    if (!bok)
    {
        return;
    }


    vtkNew<vtkWindowToImageFilter> win2ImgFilter;

    win2ImgFilter->SetInput(renwin);
    win2ImgFilter->SetScale(factor);
    win2ImgFilter->SetFixBoundary(true);

    QString tmpName = fileName.toLower();
    if (tmpName.endsWith(".jpg") || tmpName.endsWith("jpeg"))
    {
        vtkNew<vtkJPEGWriter> writer;
        writer->SetInputConnection(win2ImgFilter->GetOutputPort());
        writer->SetFileName(fileName.toStdString().c_str());
        writer->Write();
    }
    else if (tmpName.endsWith(".png"))
    {
        vtkNew<vtkPNGWriter> writer;
        writer->SetInputConnection(win2ImgFilter->GetOutputPort());
        writer->SetFileName(fileName.toStdString().c_str());
        writer->Write();
    }
    else
    {
        NMLogError(<< "Save Map As Image File: Unsupported image format!");
    }
}

void
LUMASSMainWin::convertImageToPolyData()
{
    // check, whether we've got a selected image
    NMLayer* l = this->mLayerList->getSelectedLayer();
    if (l == 0)
    {
        NMDebugAI(<< "No layer selected!" << std::endl);
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    NMImageLayer* il = qobject_cast<NMImageLayer*>(l);
    if (il == 0)
    {
        NMDebugAI(<< "No image layer selected!" << std::endl);
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    vtkDataSet* ids = const_cast<vtkDataSet*>(il->getDataSet());
    vtkImageData* id = vtkImageData::SafeDownCast(ids);

    QList<int> iSel;
    image2PolyData(id, iSel);
}

void
LUMASSMainWin::image2PolyData(vtkImageData *img, QList<int> &unitIds)
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

    vtkImageData* id = img;

    vtkNew<vtkIdList> pixIds;
    vtkDataSetAttributes* dsa = 0;
    vtkDataArray* idxScalars = id->GetPointData()->GetArray(0);
    if (unitIds.count() > 0)
    {
        for (int pix=0; pix < idxScalars->GetNumberOfTuples(); ++pix)
        {
            const long val = idxScalars->GetVariantValue(pix).ToLong();
            if (unitIds.contains(val))
            {
                pixIds->InsertNextId(pix);
            }
        }
    }


    double spacing[3];
    id->GetSpacing(spacing);
    int* extent = id->GetExtent();

    // pad the image by 1
    // to be able to create cells afterwards
    // note: image data stores points at cell
    // centres
    vtkNew<vtkImageWrapPad> pad;
    pad->SetInputData(id);
    pad->SetOutputWholeExtent(
                extent[0], extent[1] + 1,
                extent[2], extent[3] + 1,
                extent[4], extent[5]);

    // convert to polydata
    vtkNew<vtkImageDataGeometryFilter> imgConv;
    imgConv->SetInputConnection(pad->GetOutputPort());

    // translate by -dx/2, -dy/2
    vtkNew<vtkTransform> transform;
    transform->Translate(-spacing[0]/2.0, -spacing[1]/2.0, 0);
    vtkNew<vtkTransformPolyDataFilter> transformFilter;
    transformFilter->SetTransform(transform.GetPointer());
    transformFilter->SetInputConnection(imgConv->GetOutputPort());
    transformFilter->Update();

    // get polygons
    vtkPolyData* pdraw = transformFilter->GetOutput();
    int ncells = 0;
    if (pixIds->GetNumberOfIds() > 0)
    {
        ncells = pixIds->GetNumberOfIds();
    }
    else
    {
        ncells = pdraw->GetNumberOfCells();
    }


    //  ----------------------------------------------------
    //  CREATE NEW POLYDATA OBJECTS
    //  ----------------------------------------------------
    vtkNew<vtkPolyData> pd;
    pd->Allocate(ncells);

    vtkNew<vtkCellArray> polys;
    polys->Allocate(ncells*5 + ncells);

    // create attr table
    vtkNew<vtkLongArray> nmid;
    nmid->SetName("nm_id");
    nmid->Allocate(ncells);

    vtkNew<vtkUnsignedCharArray> nmhole;
    nmhole->SetName("nm_hole");
    nmhole->Allocate(ncells);

    vtkNew<vtkUnsignedCharArray> nmsel;
    nmsel->SetName("nm_sel");
    nmsel->Allocate(ncells);

    vtkNew<vtkLongArray> polyid;
    polyid->SetName("PolyID");
    polyid->Allocate(ncells);


    //  ----------------------------------------------------
    //  COPY PTS AND CLOSE POLYS
    //  ----------------------------------------------------

    vtkCell* cell = 0;
    for (int p=0; p < ncells; ++p)
    {
        long id;
        if (pixIds->GetNumberOfIds() > 0)
        {
            id = pixIds->GetId(p);
            cell = pdraw->GetCell(pixIds->GetId(p));
        }
        else
        {
            id = p;
            cell = pdraw->GetCell(p);
        }
        const int npts = cell->GetNumberOfPoints();
        vtkIdList* ids = cell->GetPointIds();

        polys->InsertNextCell(npts+1);
        polys->InsertCellPoint(ids->GetId(0));
        for (int i=npts-1; i >= 0; --i)
        {
            polys->InsertCellPoint(ids->GetId(i));
        }

        // the attributes
        nmid->InsertNextValue(p+1);
        nmhole->InsertNextValue(0);
        nmsel->InsertNextValue(0);
        const long val = idxScalars->GetVariantValue(id).ToLong();
        polyid->InsertNextValue(val);
    }

    pd->SetPoints(pdraw->GetPoints());
    pd->SetPolys(polys.GetPointer());
    pd->GetCellData()->SetScalars(nmid.GetPointer());
    pd->GetCellData()->AddArray(nmhole.GetPointer());
    pd->GetCellData()->AddArray(nmsel.GetPointer());
    pd->GetCellData()->AddArray(polyid.GetPointer());

    pd->BuildCells();
    pd->BuildLinks();

    //  ----------------------------------------------------
    //  ADD LAYER TO LUMASS
    //  ----------------------------------------------------

    NMVectorLayer* newPolys = new NMVectorLayer(this->getRenderWindow());

    NMLayer* l = this->mLayerList->getSelectedLayer();
    QString name = "Image_polys";
    if (l)
    {
        name = l->objectName() + QString("_polys");
    }
    newPolys->setLegendType(NMLayer::NM_LEGEND_SINGLESYMBOL);
    newPolys->setObjectName(name);
    newPolys->setDataSet(pd.GetPointer());
    newPolys->setVisible(true);
    this->mLayerList->addLayer(newPolys);

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void
LUMASSMainWin::mapViewMode()
{
    //ui->qvtkWidget->setVisible(true);
    ui->actionShow_Map_View->setChecked(true);
    this->showMapView(true);

    ui->actionShow_Model_View->setChecked(false);
    this->showModelView(false);
    //ui->modelViewWidget->parentWidget()->setVisible(false);

    ui->infoDock->setVisible(true);
    ui->actionShow_Components_Info->setChecked(true);

    ui->componentsWidget->setVisible(true);
    ui->actionComponents_View->setChecked(true);

    ui->compWidgetList->setWidgetItemVisible(0, true);
    ui->compWidgetList->setWidgetItemVisible(1, false);

    ui->infoWidgetList->setWidgetItemVisible(0, true);
    ui->infoWidgetList->setWidgetItemVisible(1, false);
}

void
LUMASSMainWin::modelViewMode()
{
    //ui->qvtkWidget->setVisible(false);
    ui->actionShow_Map_View->setChecked(false);
    this->showMapView(false);

    //ui->modelViewWidget->parentWidget()->setVisible(true);
    ui->actionShow_Model_View->setChecked(true);
    this->showModelView(true);

    ui->componentsWidget->setVisible(true);
    ui->actionComponents_View->setChecked(true);

    ui->infoDock->setVisible(true);
    ui->actionShow_Components_Info->setChecked(true);

    ui->compWidgetList->setWidgetItemVisible(0, false);
    ui->compWidgetList->setWidgetItemVisible(1, true);

    ui->infoWidgetList->setWidgetItemVisible(0, false);
    ui->infoWidgetList->setWidgetItemVisible(1, true);
}

void
LUMASSMainWin::showMapView(bool vis)
{
    ui->qvtkWidget->setVisible(vis);
    if (!vis)
    {
        this->mActiveWidget = ui->modelViewWidget;
    }
    QAction* actMapBtn = ui->mainToolBar->findChild<QAction*>("actMapBtn");
    if (actMapBtn) actMapBtn->setChecked(vis);

    ui->actionShow_Map_View->setChecked(vis);

}

void
LUMASSMainWin::showModelView(bool vis)
{
    ui->modelViewWidget->setVisible(vis);
    if (!vis)
    {
        this->mActiveWidget = ui->qvtkWidget;
    }
    QAction* actModelBtn = ui->mainToolBar->findChild<QAction*>("actModelBtn");
    if (actModelBtn) actModelBtn->setChecked(vis);

    ui->actionShow_Model_View->setChecked(vis);
}

const vtkRenderer*
LUMASSMainWin::getBkgRenderer(void)
{
	return this->mBkgRenderer;
}

const vtkRenderer*
LUMASSMainWin::getScaleRenderer(void)
{
    return this->mScaleRenderer;
}

const NMComponentEditor*
LUMASSMainWin::getCompEditor(void)
{
    if (mTreeCompEditor)
        return mTreeCompEditor;
    else
        return 0;
    //return this->ui->compEditor;
}

#ifdef BUILD_RASSUPPORT
RasdamanConnector*
LUMASSMainWin::getRasdamanConnector(void)
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

	// for now, this means that once rasdaman has been checked,
	// and was found to be unavailable, it won't be available
	// for the rest of the session, and the user has to close
	// and restart LUMASS - this is not really clever design
	// and needs revision --> TODO:
	if (mbNoRasdaman)
    {
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return 0;
    }

	if (this->mpRasconn == 0)
	{
		std::string connfile = std::string(getenv("HOME")) + "/.rasdaman/rasconnect";
		this->mpRasconn = new RasdamanConnector(connfile);
	}

	// get the sql for creating required views and functions to
	// facilitate metadata browsing
	std::string geospatial_fn = std::string(_lumass_binary_dir) +
			"/shared/ps_view_geometadata.sql";
	std::string extrametadata_fn = std::string(_lumass_binary_dir) +
			"/shared/ps_view_extrametadata.sql";
	std::string metadatatable_fn = std::string(_lumass_binary_dir) +
			"/shared/ps_func_metadatatable.sql";

	QFile geofile(QString(geospatial_fn.c_str()));
	bool fileopen = true;
	if (!geofile.open(QIODevice::ReadOnly | QIODevice::Text))
		fileopen = false;

	QTextStream in(&geofile);
	QString geosql(in.readAll());

	QFile extrafile(QString(extrametadata_fn.c_str()));
	if (!extrafile.open(QIODevice::ReadOnly | QIODevice::Text))
		fileopen = false;

	QTextStream in2(&extrafile);
	QString extrasql(in2.readAll());

	QFile tabfile(QString(metadatatable_fn.c_str()));
	if (!tabfile.open(QIODevice::ReadOnly | QIODevice::Text))
		fileopen = false;

	if (!fileopen)
	{
                NMLogError(<< ctxLUMASSMainWin << ": func_metadatatable: Failed installing petascope browsing support!");
		if (this->mpRasconn)
		{
			this->mpRasconn->disconnect();
			delete this->mpRasconn;
			this->mpRasconn = 0;
		}
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return 0;
	}

	QTextStream in3(&tabfile);
	QString tabsql(in3.readAll());

	// now execute the files
	std::string query = extrasql.toStdString() + geosql.toStdString() +
			   tabsql.toStdString();

	try
	{
		this->mpRasconn->connect();
	}
	catch (r_Error& raserr)
	{
		//NMBoxErr("Rasdaman Connection Error", raserr.what()
		//		 << ": Check whether the rasdaman database is up and running!");
                NMLogError(<< ctxLUMASSMainWin << ": " << raserr.what());
		if (this->mpRasconn)
		{
			this->mpRasconn->disconnect();
			delete this->mpRasconn;
			this->mpRasconn = 0;
		}
		this->mbNoRasdaman = true;
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return 0;
	}

	const PGconn* conn = this->mpRasconn->getPetaConnection();
	PGresult* res = PQexec(const_cast<PGconn*>(conn), query.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		NMBoxErr("Failed initialising rasdaman metadata browser!",
				 "Check whether Postgres is up and running and access is "
				"is configured properly!");
                NMLogError(<< ctxLUMASSMainWin << ": PQexec: Failed installing petascope browsing support!"
				                 << PQresultErrorMessage(res));
                NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return 0;
	}
	PQclear(res);

	return this->mpRasconn;
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}
#endif

void LUMASSMainWin::aboutLUMASS(void)
{
    QString vinfo = QString("Version %2.%3.%4 (%1)\nCommit %5\nLast updated %6")
                      .arg(_lumass_build_type)
                      .arg(_lumass_version_major)
                      .arg(_lumass_version_minor)
                      .arg(_lumass_version_revision)
                      .arg(_lumass_commit_hash)
                      .arg(_lumass_commit_date);

	QString year = QString(_lumass_commit_date).split(" ").at(4);
	QString title = tr("About LUMASS");
	stringstream textstr;
	textstr << "LUMASS - The Land-Use Management Support System" << endl
			<< vinfo.toStdString() << endl
			<< "Developed by Alexander Herzig" << endl
			<< "Copyright 2010-" << year.toStdString() << " Landcare Research New Zealand Ltd" << endl
			<< "www.landcareresearch.co.nz" << endl << endl
			<< "LUMASS is free software and licenced under the GPL v3." << endl
			<< "Contact: herziga@landcareresearch.co.nz" << endl
            << "Code: https://bitbucket.org/landcareresearch/lumass" << endl
            << "User group: https://groups.google.com/forum/#!forum/lumass-users" << endl
            << endl
            << "LUMASS builds on the following open source libraries " << endl
            << "Qt " << _lumass_qt_version << " - http://www.qt.io/" << endl
            << "OTB " << _lumass_otb_version << " - https://www.orfeo-toolbox.org/" << endl
            << "ITK " << _lumass_itk_version << " - http://www.itk.org/" << endl
            << "VTK " << _lumass_vtk_version << " - http://www.vtk.org/ " << endl
            << "lp_solve 5.5 - http://sourceforge.net/projects/lpsolve/ " << endl
            << "GDAL " << _lumass_gdal_version << " - http://www.gdal.org/ " << endl
            << "SQLite " << _lumass_sqlite_version << " - http://www.sqlite.org" << endl
            << "Spatialite 4.3 - https://www.gaia-gis.it/fossil/libspatialite/index" << endl
            << "MuParser 2.2.5 - http://beltoforion.de/article.php?a=muparser" << endl
            << "MuParserX 4.0.7 - http://beltoforion.de/article.php?a=muparserx" << endl
#ifdef BUILD_RASSUPPORT
  #ifdef PACKAGE_STRING
            << PACKAGE_STRING
  #else
            << "Rasdaman ?.?.?"
  #endif
            << " - http://www.rasdaman.org/ " << endl
#endif
            << "";



    QString text(textstr.str().c_str());
	QMessageBox::about(this, title, text);

}

void
LUMASSMainWin::importTableObject()
{
    this->openCreateTable(NM_TABVIEW_STANDALONE);
}

NMSqlTableView*
LUMASSMainWin::openCreateTable(TableViewType tvType,
                                bool bOpen)
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

    NMSqlTableView* tv = 0;

    QString fileName;
    if (bOpen)
    {
        fileName = QFileDialog::getOpenFileName(this,
             tr("Import Table Data"), "~",
             tr("dBASE (*.dbf);;Delimited Text (*.csv *.txt);;SQLite Table (*.db *.sqlite *.ldb);;Excel File (*.xls)"));
    }
    else
    {
        fileName = QFileDialog::getSaveFileName(this, tr("Create Parameter Table"),
                    "~", tr("SQLiteTable (*.ldb *.db *.sqlite)"));
    }

    if (fileName.isEmpty())
    {
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return tv;
    }

    QStringList sqliteformats;
    sqliteformats << "db" << "sqlite" << "ldb";

    QString tableName;

    QFileInfo fifo(fileName);
    if (bOpen && sqliteformats.contains(fifo.suffix().toLower()))
    {
        tableName = this->selectSqliteTable(fileName);
        if (!tableName.isEmpty())
        {
            tv = this->importTable(fileName, tvType, bOpen, tableName);
        }
    }
    else
    {
        tv = this->importTable(fileName, tvType, bOpen);
    }

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
    return tv;
}


QString
LUMASSMainWin::selectSqliteTable(const QString &dbFileName)
{
    QString tableName;

    otb::SQLiteTable::Pointer sqlTable = otb::SQLiteTable::New();
    sqlTable->SetUseSharedCache(false);
    sqlTable->SetDbFileName(dbFileName.toStdString());
    if (sqlTable->openConnection())
    {
        std::vector<std::string> tables = sqlTable->GetTableList();
        if (tables.size() == 1)
        {
            return QString(tables[0].c_str());
        }

        QStringList qtables;
        for (int i=0; i < tables.size(); ++i)
        {
            qtables << tables[i].c_str();
        }

        QInputDialog ipd(this);
        ipd.setOption(QInputDialog::UseListViewForComboBoxItems);
        ipd.setComboBoxItems(qtables);
        ipd.setComboBoxEditable(false);
        ipd.setWindowTitle("Database Tables");
        ipd.setLabelText("Select table to open:");
        int ret = ipd.exec();

        if (ret)
        {
            tableName = ipd.textValue();
        }

        sqlTable->disconnectDB();
    }
    else
    {
        NMBoxErr("Open sqlite database",
                 "Couldn't open database '"
                 << dbFileName.toStdString() << "'!");
    }

    return tableName;
}

NMSqlTableView*
LUMASSMainWin::importTable(const QString& fileName,
                            TableViewType tvType,
                            bool bOpen,
                            const QString &tableName)
{
    otb::SQLiteTable::Pointer sqlTable = otb::SQLiteTable::New();
    std::vector<std::string> vinfo = sqlTable->GetFilenameInfo(fileName.toStdString());
    if (vinfo.size() < 3)
    {
        return 0;
    }

    // initiate unique table identifier to the
    // basename of the given filename (i.e. without
    // path and extension
    NMSqlTableView* resview = 0;
    QString viewName = QString(vinfo.at(1).c_str());
    // if we've got a table name given already (as picked
    // by the user) we use that as table identifier
    if (!tableName.isEmpty())
    {
        viewName = tableName;
    }

    // it could be that different DBs have a table with the
    // same name, in that case we have to check the
    // actual DB filename to see whether the table is actually
    // the same
    int lfd = 1;
    if (mTableList.contains(QString(vinfo.at(1).c_str())))
    {
        bool dbpresent = false;

        // in case we haven't got a tableName (and hence a filename)
        // we create a filename from the filename info, otherwise ...
        QString ldbName = QString("%1/%2%3.ldb")
                            .arg(vinfo[0].c_str())
                            .arg(vinfo[1].c_str())
                            .arg(vinfo[2].c_str());
        // ... we use the name we've got already
        if (!tableName.isEmpty())
        {
            ldbName = fileName;
        }

        // now we go through our table repository to see whether it's not only the table name
        // which we've got already but whether it is actually from the the database we'v already
        // got in our list
        QMap<QString, QPair<otb::SQLiteTable::Pointer, QSharedPointer<NMSqlTableView> > >::const_iterator it =
                mTableList.constBegin();
        while (it != mTableList.constEnd())
        {
            if (ldbName.compare(it.value().first->GetDbFileName().c_str(), Qt::CaseInsensitive) == 0)
            {
                if (!tableName.isEmpty())
                {
                    if (tableName.compare(it.value().first->GetTableName().c_str(), Qt::CaseInsensitive) == 0)
                    {
                        dbpresent = true;
                        break;
                    }
                }
            }

            ++it;
        }

        lfd++;
        // depending on the type of table, we do treat things differently ...
        // ... if we've got a standalone table ...
        if (tvType == NM_TABVIEW_STANDALONE)
        {
            // ... and it has already been imported, we just raise it, ...
            if (dbpresent)
            {

                    NMLogInfo(<< "Import Table Data: Table has already been imported!");

                    resview = mTableList.find(viewName).value().second.data();
                    resview->show();
                    resview->raise();
                    return resview;
            }
            // ... otherwise we create a unique table name by numbering
            // subsequent tables sharing the name of an already present db table
            else
            {
                viewName = QString("%1 <%2>").arg(vinfo[1].c_str()).arg(lfd);
                while(mTableList.contains(viewName))
                {
                    ++lfd;
                    viewName = QString("%1 <%2>").arg(vinfo[1].c_str()).arg(lfd);
                }
            }
        }
        // ... if we've got a parameter table, just strictly allow
        // tables to share names, even if they're from different source
        // DBs (might change in the future)
        else if (tvType == NM_TABVIEW_SCENE)
        {
            std::stringstream msg;
            msg << "The model already contains a ParameterTable '"
                << viewName.toStdString() << "'!";

            NMLogInfo(<< "Add ParameterTable - " << msg.str());
            return 0;
        }
    }


    sqlTable->SetUseSharedCache(false);

    // now we either open the table or create a new table
    // using the names and filenames just negotiated
    QString tablename = tableName;
    QString dbfilename = fileName;
    if (tableName.isEmpty())
    {
        if (bOpen)
        {
            if (!sqlTable->CreateFromVirtual(fileName.toStdString()))
            {
                NMDebugCtx(ctxLUMASSMainWin, << "done!");
                return 0;
            }
        }
        else
        {
            if (sqlTable->CreateTable(fileName.toStdString()) != otb::SQLiteTable::ATCREATE_CREATED)
            {
                NMBoxErr("Create Table", "Failed creating LUMASS data base table '"
                          << fileName.toStdString() << "'!");
                NMDebugCtx(ctxLUMASSMainWin, << "done!");
                return 0;
            }
        }
        tablename = sqlTable->GetTableName().c_str();
        dbfilename = sqlTable->GetDbFileName().c_str();
        sqlTable->CloseTable();
    }

    // we also create a unique connection name for our
    // Qt-based connection
    QString conname = QString("%1_%2").arg(tablename).arg(lfd);

    // create a proper low-level sqlite3 connection so, we can ...
	sqlite3* tabconn = 0;
    void* splCache = spatialite_alloc_connection();
	int rc = ::sqlite3_open_v2(dbfilename.toStdString().c_str(), 
			&tabconn, 
        			SQLITE_OPEN_URI |
        			SQLITE_OPEN_READWRITE, 0);
    if (rc != SQLITE_OK)
    {
        NMLogError(<< ctxLUMASSMainWin
            << ": Failed opening SqlTableView connection!");
        ::sqlite3_close(tabconn);
        spatialite_cleanup_ex(splCache);
        splCache = 0;
        tabconn = 0;
                NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return 0;
    }

    // ... load the spatialite extension for extra functionality
    spatialite_init_ex(tabconn, splCache, 1);

    // use the 'hacked' (NM)QSQLiteDriver, so we
    // can create a driver based on an existing sqilte3
    // connection (with spatialite loaded) ...
    NMQSQLiteDriver* drv = new NMQSQLiteDriver(tabconn, 0);
    // ... and establish the Qt-Db connection
    QSqlDatabase db = QSqlDatabase::addDatabase(drv, conname);
    db.setDatabaseName(dbfilename);

    // now we create our NMSqlTableView and do some book keeping
    NMSqlTableModel* srcModel = new NMSqlTableModel(this, db);
    // note: setting the db name here is just for reference purposes
    srcModel->setDatabaseName(dbfilename);
    srcModel->setTable(tablename);
    srcModel->select();

    if (viewName.isEmpty())
    {
        viewName = QString(tablename);
    }

    QSharedPointer<NMSqlTableView> tabview;

    if (tvType == NM_TABVIEW_STANDALONE)
    {
        tabview = QSharedPointer<NMSqlTableView>(
                    new NMSqlTableView(srcModel,
                        NMSqlTableView::NMTABVIEW_STANDALONE, 0)
                    );
    }
    else if (tvType == NM_TABVIEW_SCENE)
    {
        tabview = QSharedPointer<NMSqlTableView>(
                    new NMSqlTableView(srcModel,
                        NMSqlTableView::NMTABVIEW_PARATABLE, 0)
                    );
    }
    tabview->setLogger(mLogger);

    tabview->setTitle(viewName);
    connect(tabview.data(), SIGNAL(tableViewClosed()), this, SLOT(tableObjectViewClosed()));
    connect(tabview.data(), SIGNAL(zoomToTableCoords(double*)), this, SLOT(zoomToCoords(double*)));


    QPair<otb::SQLiteTable::Pointer, QSharedPointer<NMSqlTableView> > tabPair;
    tabPair.first = sqlTable;
    tabPair.second = tabview;

    mTableList.insert(viewName, tabPair);

	QPair<void*, sqlite3*> adminPair;
	adminPair.first = splCache;
	adminPair.second = tabconn;

	mTableAdminObjects.insert(viewName, adminPair);

    if (tvType == NM_TABVIEW_STANDALONE)
    {
        QPixmap pm;
        pm.load(":table_object.png");

        QListWidgetItem* wi = new QListWidgetItem(QIcon(pm), viewName, mTableListWidget);
        mTableListWidget->addItem(wi);

        tabview->show();
        tabview->raise();
    }

    return tabview.data();
}

NMSqlTableView*
LUMASSMainWin::createTableView(otb::SQLiteTable::Pointer sqlTab)
{
    QString tmpname = QString::fromLatin1("mem_%1").arg(sqlTab->GetTableName().c_str());
    QString conname = tmpname;

    int nr = 2;
    while(QSqlDatabase::connectionNames().contains(conname))
    {
        conname = QString("%1_%2").arg(tmpname).arg(nr);
        ++nr;
    }

    // use the 'hacked' (NM)QSQLiteDriver, so we
    // can create a driver based on an existing sqilte3
    // connection (with spatialite loaded) ...
    NMQSQLiteDriver* drv = new NMQSQLiteDriver(sqlTab->GetDbConnection(), 0);
    // ... and establish the Qt-Db connection
    QSqlDatabase db = QSqlDatabase::addDatabase(drv, conname);
    db.setDatabaseName(sqlTab->GetDbFileName().c_str());

    // now we create our NMSqlTableView and do some book keeping
    NMSqlTableModel* srcModel = new NMSqlTableModel(this, db);
    // note: setting the db name here is just for reference purposes
    srcModel->setDatabaseName(sqlTab->GetDbFileName().c_str());
    srcModel->setTable(sqlTab->GetTableName().c_str());
    srcModel->select();

    QString viewName = QString(conname);


    QSharedPointer<NMSqlTableView> tabview = QSharedPointer<NMSqlTableView>(
                    new NMSqlTableView(srcModel,
                        NMSqlTableView::NMTABVIEW_STANDALONE, 0)
                    );

    tabview->setTitle(viewName);
    connect(tabview.data(), SIGNAL(tableViewClosed()), this, SLOT(tableObjectViewClosed()));


    QPair<otb::SQLiteTable::Pointer, QSharedPointer<NMSqlTableView> > tabPair;
    tabPair.first = sqlTab;
    tabPair.second = tabview;

    mTableList.insert(viewName, tabPair);

    QPair<void*, sqlite3*> adminPair;
    adminPair.first = 0;
    adminPair.second = sqlTab->GetDbConnection();

    mTableAdminObjects.insert(viewName, adminPair);

    QPixmap pm;
    pm.load(":table_object.png");

    QListWidgetItem* wi = new QListWidgetItem(QIcon(pm), viewName, mTableListWidget);
    mTableListWidget->addItem(wi);

    tabview->show();
    tabview->raise();

    return tabview.data();
}

void
LUMASSMainWin::tableObjectVisibility(QListWidgetItem* item)
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

    QString name = item->text();
    QMap<QString, QPair<otb::SQLiteTable::Pointer, QSharedPointer<NMSqlTableView> > >::iterator it =
            mTableList.find(name);
    if (it == mTableList.end())
    {
        return;
    }

    QPixmap pm;
    NMSqlTableView* tview = it.value().second.data();
    if (tview->isVisible())
    {
        tview->close();
        pm.load(":table_object_invisible.png");
    }
    else
    {
        tview->show();
        tview->raise();
        pm.load(":table_object.png");
    }
    item->setIcon(QIcon(pm));

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void
LUMASSMainWin::tableObjectViewClosed()
{
    QObject* obj = this->sender();
    NMSqlTableView* tv = qobject_cast<NMSqlTableView*>(obj);
    if (tv == 0)
    {
        return;
    }

    QList<QListWidgetItem*> items = mTableListWidget->findItems(tv->windowTitle(), Qt::MatchFixedString);
    if (items.size() == 1)
    {
        this->tableObjectVisibility(items.at(0));
    }
}

void
LUMASSMainWin::deleteTableObject(const QString& name)
{

    QMap<QString, QPair<otb::SQLiteTable::Pointer, QSharedPointer<NMSqlTableView> > >::iterator itList =
               mTableList.find(name);

    if (itList != mTableList.end())
    {
        NMSqlTableModel* model = qobject_cast<NMSqlTableModel*>(itList.value().second->getModel());
        QString cname = itList.value().second->windowTitle();
        model->clear();
        model->database().close();
        delete model;

        QSqlDatabase::removeDatabase(cname);
        mTableList.erase(itList);
    }

    QMap<QString, QPair<void*, sqlite3*> >::iterator itAdmin =
            mTableAdminObjects.find(name);
    if (itAdmin != mTableAdminObjects.end())
    {
        void* cache = itAdmin.value().first;
        sqlite3* conn = itAdmin.value().second;

        sqlite3_close(conn);
        spatialite_cleanup_ex(cache);
        cache = 0;
        conn = 0;
        mTableAdminObjects.erase(itAdmin);
    }
}

void
LUMASSMainWin::removeTableObject(QListWidgetItem* item, QPoint globalPos)
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

    //    QObject* obj = this->sender();
    //    QContextMenuEvent* cme = static_cast<QContextMenuEvent*>(mLastEvent);
    //    if (    obj->objectName().compare(mTableListWidget->objectName()) != 0
    //         || cme == 0
    //       )
    //    {
    //        NMDebugCtx(ctxLUMASSMainWin, << "done!");
    //        return;
    //    }

    QString name = item->text();
    if (!mTableList.keys().contains(name))
    {
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    QScopedPointer<QMenu> menu(new QMenu(this));
    QAction* act = new QAction(menu.data());
    QString actText = QString("Remove '%1'").arg(item->text());
    act->setText(actText);
    menu->addAction(act);

    QAction* ret = menu->exec(globalPos);

    //    QString msg = QString("Do you want to remove table '%1'?").arg(name);
    //    QMessageBox::StandardButton btn =
    //            QMessageBox::question(this, "Remove Table Object",
    //                                  msg);
    if (ret == 0)
    {
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    this->deleteTableObject(name);

    mTableListWidget->removeItemWidget(item);
    delete item;

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}



void LUMASSMainWin::saveAsVectorLayerOGR(void)
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

	// get the selected layer
    NMLayer* l = this->mLayerList->getSelectedLayer();
	if (l == 0)
		return;

	// make sure, we've got a vector layer
	if (l->getLayerType() != NMLayer::NM_VECTOR_LAYER)
		return;

	// get the vector layer
	NMVectorLayer* vL = qobject_cast<NMVectorLayer*>(l);

	// what kind of feature type are we dealing with?
	NMVectorLayer::NMFeatureType intype = vL->getFeatureType();
	NMDebugAI( << "the feature type is: " << intype << endl);

	// get a list of available drivers


#ifndef GDAL_200
    OGRRegisterAll();
    OGRSFDriverRegistrar* reg = OGRSFDriverRegistrar::GetRegistrar();
#else
    GDALAllRegister();
    GDALDriverManager* reg = GetGDALDriverManager();
#endif

	QStringList sDriverList;
	for (int i=0; i < reg->GetDriverCount(); i++)
	{
#ifndef GDAL_200
        OGRSFDriver* driver = reg->GetDriver(i);
		sDriverList.append(driver->GetName());
#else
        GDALDriver* driver = reg->GetDriver(i);
        QString vecdrv = driver->GetMetadataItem(GDAL_DCAP_VECTOR);
        if (vecdrv.compare(QString("yes"), Qt::CaseInsensitive) == 0)
        {
            const char* drname = GDALGetDriverLongName(driver);
            sDriverList.append(drname);
        }
#endif
		
	}

	// show a list of all available drivers and ask user which driver to use
	bool bOk = false;
	QString theDriverName = QInputDialog::getItem(this, tr("Save As Vector Layer"),
			tr("Select Data Format (Driver)"),
			sDriverList, 0, false, &bOk, 0);

	// if the user pressed cancel
	if (!bOk)
		return;

	NMDebugAI( << "chosen driver: " << theDriverName.toStdString() << endl);

	// ask the user for a filename
	QString sFileDlgTitle = tr("Save As ");
	sFileDlgTitle += theDriverName;

	QString fileName = QFileDialog::getSaveFileName(this,
			sFileDlgTitle, tr("~/") + l->objectName(), theDriverName);
	if (fileName.isNull())
		return;

	NMDebugAI( << "new file name is: " << fileName.toStdString() << endl);

    // create the data set
	// ToDo: adjust code for DB-based formats
	// (i.e. do we need port information, host, user, passwd, etc.)
#ifndef GDAL_200
    OGRSFDriver* theDriver = reg->GetDriverByName(theDriverName.toStdString().c_str());
    // create the new data source
    OGRDataSource* ds = theDriver->CreateDataSource(fileName.toStdString().c_str(), 0);
#else
    GDALDriver* theDriver = reg->GetDriverByName(theDriverName.toStdString().c_str());
    GDALDataset* ds = theDriver->Create(fileName.toStdString().c_str(), 0, 0, 0, GDT_Unknown, 0);
#endif

    switch(vL->getFeatureType())
    {
    case NMVectorLayer::NM_POLYGON_FEAT:
        this->vtkPolygonPolydataToOGR(ds, vL);
        break;
    default:
        NMDebugAI( << "Export of this feature type is not supported!" << std::endl);
        break;

    }


    // clean up OGR stuff
	// destroy the data source object
#ifndef GDAL_200
	OGRDataSource::DestroyDataSource(ds);
#else
    GDALClose(ds);
#endif

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void
LUMASSMainWin::saveImageFile()
{
    QObject* obj = this->sender();
    if (obj && obj->objectName().compare(QString("actionSave_Visible_Extent_Overview_As")) == 0)
    {
        this->saveAsImageFile(true);
    }
    else
    {
        this->saveAsImageFile(false);
    }
}

void LUMASSMainWin::saveAsImageFile(bool onlyVisImg)
{
    // get the selected layer
    NMLayer* l = this->mLayerList->getSelectedLayer();
    if (l == 0)
        return;

    // make sure, we've got a vector layer
    if (l->getLayerType() != NMLayer::NM_IMAGE_LAYER)
        return;

    // get the vector layer
    NMImageLayer* il = qobject_cast<NMImageLayer*>(l);

    QString fileName = QFileDialog::getSaveFileName(this,
            QString("Save As Image File"),
            QString("~/%1.kea").arg(l->objectName()));
    if (fileName.isNull())
        return;

    //NMModelController* ctrl = NMModelController::getInstance();
    QScopedPointer<NMModelController> ctrl(new NMModelController());
    ctrl->setLogger(mLogger);

    // ---------------- SET UP INPUT ----------------------
    QSharedPointer<NMItkDataObjectWrapper> dw = il->getImage();
    dw->getDataObject()->SetReleaseDataFlag(false);
    NMDataComponent* dc = new NMDataComponent();
    dc->setObjectName("DataBuffer");
    QString bufCompName = ctrl->addComponent(dc);
    dc->setInput(dw);

    // ----------------- SET UP READER IF APPLICABLE--------
    QString srcFileName = il->getFileName();
    QFileInfo fifo(srcFileName);
    QString readerCompName;
    bool bReader = false;
    if (!onlyVisImg && fifo.isFile() && fifo.isReadable())
    {
        bReader = true;

        // SET UP THE READER Process
        NMImageReader* readerProc = new NMImageReader();
        readerProc->setFileNames(QStringList(srcFileName));
        readerProc->setImgTypeSpec(dw);

        NMSequentialIterComponent* readerComp =
                new NMSequentialIterComponent();
        readerComp->setObjectName("ImageReader");
        readerComp->setProcess(readerProc);
        readerCompName = ctrl->addComponent(readerComp);
    }

    // ---------------- SET UP WRITER ---------------------
    // create the process
    NMStreamingImageFileWriterWrapper* writerProc =
                new NMStreamingImageFileWriterWrapper();
    writerProc->setFileNames(QStringList(fileName));
    writerProc->setImgTypeSpec(dw);
    writerProc->setWriteTable(true);
    //writerProc->setInputTables(QStringList(bufCompName));
    writerProc->setPyramidResamplingType(QString("NEAREST"));
    writerProc->setRGBMode(dw->getIsRGBImage());


    // create host component
    NMSequentialIterComponent* writerComp =
                new NMSequentialIterComponent();
    writerComp->setObjectName("ImageWriter");
    writerComp->setProcess(writerProc);

    QString writerCompName = ctrl->addComponent(writerComp);
    QList<QStringList> llst;
    QStringList lst;

    if (bReader)
    {
        lst << readerCompName;
    }
    else
    {
        lst << bufCompName;
    }

    llst << lst;
    writerComp->setInputs(llst);

    // ---- CONTROLLER DOES THE REST ------
    ctrl->executeModel(writerComp->objectName());
//    QStringList del;
//    del << bufCompName << writerCompName;
//    if (!onlyVisImg)
//    {
//        del << readerCompName;
//    }
//    ctrl->deleteLater(del);
}

void LUMASSMainWin::checkInteractiveLayer(void)
{
    int firstVisID = -1;
    bool bAllInactive = true;
    vtkRenderer* ren = 0;

    for (int id = this->mLayerList->getLayerCount()-1; id >= 0; --id)
    {
        NMLayer* l = this->mLayerList->getLayer(id);
        ren = const_cast<vtkRenderer*>(l->getRenderer());
//        if (l->isVisible() && ren->GetInteractive())
//        {
//            bAllInactive = false;
//        }

//        if (firstVisID < 0 && l->isVisible())
//        {
//            firstVisID = id;
//        }

        ren->SetInteractive(0);
        if (    (l->isVisible() || l->getSelection().count() > 0)
            &&  !ren->GetInteractive()
           )
        {
            firstVisID = id;
        }

    }

    if (firstVisID >= 0)
    {
        ren = const_cast<vtkRenderer*>(this->mLayerList->getLayer(firstVisID)->getRenderer());
        ren->SetInteractive(1);
    }
}

void LUMASSMainWin::checkRemoveLayerInfo(NMLayer* l)
{
    if (l == mLastInfoLayer)
    {
        updateLayerInfo(0, -1);
    }
}

void LUMASSMainWin::updateLayerInfo(NMLayer* l, long long cellId)
{
        //NMDebugCtx(ctxLUMASSMainWin, << "...");

    QTableWidget* ti = this->ui->infoWidgetList->findChild<QTableWidget*>(
                QString::fromUtf8("layerInfoTable"));
    if (ti == 0)
    {
        NMWarn(ctxLUMASSMainWin, << "Couldn't find 'layerInfoTable'!");
        return;
    }
	ti->clear();

    if (l == 0 || cellId < 0)
	{
		ti->setColumnCount(0);
		ti->setRowCount(0);
        this->mLastInfoLayer = 0;
        //NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return;
	}
    this->mLastInfoLayer = l;

	ti->setColumnCount(2);
	ti->horizontalHeader()->setStretchLastSection(true);

	QStringList colHeaderLabels;
    colHeaderLabels << QString(tr("Attributes (%1)").arg(cellId)) << "Value";

	ti->setHorizontalHeaderLabels(colHeaderLabels);

	// =====================================================================
	// 					READ VECTOR ATTRIBUTES
	// =====================================================================
	if (l->getLayerType() == NMLayer::NM_VECTOR_LAYER)
	{
		vtkDataSet* ds = const_cast<vtkDataSet*>(l->getDataSet());
		vtkDataSetAttributes* dsAttr = ds->GetAttributes(vtkDataSet::CELL);
		int nfields = dsAttr->GetNumberOfArrays();

		// we substract 2 fields from that list (i.e. nm_sel, nm_hole)
		ti->setRowCount(nfields-2);

		long rowcnt = 0;
		for (int r=0; r < nfields; ++r)
		{
			vtkAbstractArray* aa = dsAttr->GetAbstractArray(r);
			if (	aa == 0
				||  strcmp(aa->GetName(),"nm_sel") == 0
				||  strcmp(aa->GetName(),"nm_hole") == 0
			   )
			{
				continue;
			}

			QTableWidgetItem* item1 = new QTableWidgetItem(aa->GetName());
			ti->setItem(rowcnt,0, item1);

			QTableWidgetItem* item2;
            if (cellId < aa->GetNumberOfTuples())
			{
				item2 = new QTableWidgetItem(
                    aa->GetVariantValue(cellId).ToString().c_str());
			}
			else
			{
				item2 = new QTableWidgetItem("CellID invalid!");
			}
			ti->setItem(rowcnt,1, item2);
			++rowcnt;
		}
	}
	// =====================================================================
	// 					READ IMAGE ATTRIBUTES
	// =====================================================================
	else if (l->getLayerType() == NMLayer::NM_IMAGE_LAYER)
	{
		NMImageLayer* il = qobject_cast<NMImageLayer*>(l);

		// TODO: we have to reasonably extend this to any table
		// we've got
        //otb::AttributeTable::Pointer tab = il->getRasterAttributeTable(1);
		//if (tab.IsNull())
		//{
		//	NMDebugAI(<< __FUNCTION__ << ": Couldn't fetch the image attribute table!" << std::endl);
		//	return;
		//}
		//QAbstractItemModel* tab = il->getTable();

        NMSqlTableModel* sqlModel = qobject_cast<NMSqlTableModel*>(
                    const_cast<QAbstractItemModel*>(il->getTable()));

        QString queryStr = QString("SELECT * from %1 where %2 = %3")
                .arg(sqlModel->tableName())
                .arg(sqlModel->getNMPrimaryKey())
                .arg(cellId);

        QSqlDatabase db = sqlModel->database();
        db.transaction();
        QSqlQuery q(db);
        QSqlRecord rec;

        bool bempty = true;
        if (q.exec(queryStr))
        {
            if (q.next())
            {
                rec = q.record();
                bempty = false;
            }
        }

        int ncols = sqlModel == 0 || rec.isEmpty() ? 1 : rec.count();
        ti->setRowCount(ncols);

        for (int r=0; r < ncols; ++r)
		{
			QTableWidgetItem *item1;
            QString colname = sqlModel == 0 ? "Value" : rec.fieldName(r);

            if (!colname.isEmpty())
                item1 = new QTableWidgetItem(colname);
			else
				item1 = new QTableWidgetItem("");
            ti->setItem(r, 0, item1);

            QTableWidgetItem *item2;
            if (rec.count())
			{
                QVariant varVal = rec.value(r);
                if (varVal.isValid())
                {
                    item2 = new QTableWidgetItem(varVal.toString());
                }
                else
                {
                    item2 = new QTableWidgetItem("CellID invalid!");
                }
            }
			else
			{
                //item2 = new QTableWidgetItem(QString("%1").arg(cellId, 0, 'g'));
                item2 = new QTableWidgetItem(QString("%1").arg(cellId));
			}
			ti->setItem(r, 1, item2);
		}
        q.finish();
        q.clear();
        db.commit();
	}

    connect(l, SIGNAL(destroyed()), ti, SLOT(clear()));
    ui->infoWidgetList->setWidgetItemVisible(0, true);
    ui->compDock->setVisible(true);

        //NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void
LUMASSMainWin::treeFindLoops(void)
{
    this->treeAnalysis(0);
}

void
LUMASSMainWin::treeSelTopDown(void)
{
    this->treeAnalysis(1);
}

void
LUMASSMainWin::treeSelBottomUp(void)
{
    this->treeAnalysis(2);
}

void
LUMASSMainWin::treeAdmin(QAbstractItemModel *&model,
                          int& parIdx, int& childIdx,
                          void *&obj, int& type)
{
    // =======================================================================
    // get selected layer
    // =======================================================================

    model = 0;
    type = -1;
    NMLayer* l = this->mLayerList->getSelectedLayer();
    if (l != 0)
    {
        model = const_cast<QAbstractItemModel*>(l->getTable());
        obj = l;
        type = 0;
    }

    if (model == 0)
    {
        QList<QListWidgetItem*> itms = this->mTableListWidget->selectedItems();
        if (itms.size() > 0)
        {
            QString viewname = itms.at(0)->text();
            QMap<QString, QPair<otb::SQLiteTable::Pointer, QSharedPointer< NMSqlTableView > > >::const_iterator it =
                    mTableList.find(viewname);
            if (it != mTableList.cend())
            {
                model = it.value().second->getModel();
                obj = it.value().second.data();
                type = 1;
            }
        }
    }

    if (model == 0)
    {
        NMBoxInfo("Tree Analysis",
                  "Please select either a Map Layer or a Table Object!");
        parIdx = -1;
        childIdx = -1;
        type = -1;
        obj = 0;
        return;
    }

    QSqlTableModel* sqlModel = qobject_cast<QSqlTableModel*>(model);
    if (sqlModel)
    {
        while(sqlModel->canFetchMore())
        {
            sqlModel->fetchMore();
        }
    }


    int ncols = model->columnCount();
    parIdx = -1;
    childIdx = -1;

    QStringList colnames;
    QMap<QString, int> nameIdxMap;

    for (int i=0; i < ncols; i++)
    {
        QString colname = model->headerData(i, Qt::Horizontal,
                                                 Qt::DisplayRole).toString();
        colnames.append(colname);
        nameIdxMap.insert(colname, i);
    }

    // =======================================================================
    // ask users for id columns
    // =======================================================================
    bool ok;
    QString idColName = QInputDialog::getItem(this, tr("Tree Analysis"),
                                             tr("Please select Parent column:"), colnames,
                                             0, false, &ok, 0);
    if (ok && !idColName.isEmpty())
    {
        parIdx = nameIdxMap.find(idColName).value();
    }
    else
    {
        model = 0;
        type = -1;
        obj = 0;
        return;
    }
    colnames.removeOne(idColName);

    QString dnColName = QInputDialog::getItem(this, tr("Tree Analysis"),
                                             tr("Please select Child column:"), colnames,
                                             0, false, &ok, 0);
    if (ok && !dnColName.isEmpty())
    {
        childIdx = nameIdxMap.find(dnColName).value();
    }
    else
    {
        parIdx = -1;
        model = 0;
        type = -1;
        obj = 0;
    }
}

/**
 * @brief LUMASSMainWin::treeAnalysis analyses trees.
 * @param mode - 0: find loops
 *               1: top down
 *               2: bottom up
 */
void
LUMASSMainWin::treeAnalysis(const int& mode)
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

    // ===============================================
    // prepare some variables
    // ===============================================
    int stopId = 0;

    QAbstractItemModel* model;
    int idIdx;
    int dnIdx;
    int type = -1;
    void* obj = 0;


    // ===============================================
    // ask user for stree structure
    // ===============================================
    this->treeAdmin(model, idIdx, dnIdx, obj, type);
    if (model == 0 || idIdx < 0 || dnIdx < 0 || obj == 0 || type < 0)
    {
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    // ===============================================
    // determine start id of analysis (if applicable)
    // ===============================================

    // if we're in tree selection mode, we insist on a startId, otherwise
    // we bail out!
    NMLayer* l = 0;
    NMSqlTableView* view = 0;
    QItemSelection isel;
//    if (type == 0)
//    {
        l = static_cast<NMLayer*>(obj);
        if (l)
        {
            isel = l->getSelection();
        }
//    }
//    else if (type == 1)
//    {
//        view = static_cast<NMSqlTableView*>(obj);
//        if (view)
//        {
//            isel = view->getSelection();
//        }
//    }

    int startId = -9999;
    int startRow = -1;
    if (mode > 0 && isel.size() == 0)
    {
        NMBoxInfo("Tree Selection",
                  "Please select an item indicating the starting "
                  "position of the tree selection!");
        return;
    }
    else if (isel.size() > 0)//(mode > 0)
    {
        startRow = isel.at(0).topLeft().row();
        QModelIndex sidx = model->index(startRow, idIdx);
        startId = model->data(sidx).toInt();
    }


    // ===============================================
    // do the actual analysis
    // ===============================================

    NMGlobalHelper h;
    h.startBusy();

    QList<int> btms = this->processTree(model, idIdx, dnIdx,
                                        startId, stopId, mode);
    if (startRow >= 0)
    {
        btms.append(startRow);
    }

    // ===============================================
    // select items in the table / layer
    // ===============================================

    if (btms.size() > 1)
    {
        quicksort(btms, 0, btms.size()-1, true);
    }

    QItemSelection newsel = h.selectRows(model, btms);
    l->setSelection(newsel);

    NMDebugAI(<< "TREE ANALYSIS SELECTION" << std::endl);
    NMDebugAI(<< "-----------------------" << std::endl);
    foreach(QItemSelectionRange r, newsel)
    {
        NMDebugAI(<< r.top() << " -- " << r.bottom() << std::endl);
    }
    NMDebugAI(<< std::endl);

    /// ToDo:
    /// this shouldn't require the layer type dependend
    /// treatment. however due to some issues with
    /// the 'outside tableview' selection update
    /// for image layers, we have it for now ..


    //    if (l->getLayerType() == NMLayer::NM_VECTOR_LAYER)
    //    {
    //        QItemSelection newsel = h.selectRows(model, btms);
    //        if (l)
    //        {
    //            l->setSelection(newsel);
    //        }
    //        else if (view)
    //        {
    //            view->setSelection(newsel);
    //        }
    //    }
    //    else
    //    {
    //        btms.removeOne(0);

    //        if (btms.size() > 0)
    //        {
    //            vtkDataSet* ds = const_cast<vtkDataSet*>(l->getDataSet());
    //            vtkImageData* id = vtkImageData::SafeDownCast(ds);

    //            this->image2PolyData(id, btms);
    //        }
    //    }

    h.endBusy();

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

QList<int>
LUMASSMainWin::processTree(QAbstractItemModel*& model, int& parIdx,
                       int& childIdx, int startId, int stopId, int mode)
{
    /// mode:
    /// 0 - find loops
    /// 1 - sel top down
    /// 2 - sel bottom up

    QMultiMap<int, int> treeMap;
    QMultiMap<int, int> rowMap;
    int nrows = model->rowCount();

    int idIdx = parIdx;
    int dnIdx = childIdx;
    if (mode == 2)
    {
        idIdx = childIdx;
        dnIdx = parIdx;
    }

    for (int r=0; r < nrows; ++r)
    {
        const QModelIndex mid = model->index(r, idIdx);
        const QModelIndex mdn = model->index(r, dnIdx);
        const int id = model->data(mid).toInt();
        const int dn = model->data(mdn).toInt();
        treeMap.insert(id, dn);
        rowMap.insert(id, r);
    }

    // -------------------------------
    // let's get rolling ...
    QSet<int> recordIdList;

    QMultiMap<int,int>::const_iterator idIter;
    if (mode == 0)
    {
        idIter = treeMap.find(startId);
        if (idIter == treeMap.cend())
        {
            idIter = treeMap.cbegin();
            startId = -9999;
        }
    }
    else if (mode == 1 || mode == 2)
    {
        idIter = treeMap.find(startId);
    }

    QMultiMap<int,int>::const_iterator resIt;
    while (idIter != treeMap.cend())
    {
        int id = idIter.key();
        QList<int> idHistory;

        if (mode == 0 && !checkTree(id, stopId, idHistory, treeMap))
        {
            // get the table record id of
            // last (looping) item
            resIt = rowMap.find(idHistory.last());
            if (resIt != rowMap.cend())
            {
                recordIdList.insert(resIt.value());
            }

            // DEBUG
            NMDebugAI(<< id << "- trail: " << std::endl);
            foreach(const int& num, idHistory)
            {
                NMDebug(<< num << " ")
            }
            NMDebug(<< std::endl << std::endl);
        }
        else if (mode == 1 || mode == 2)
        {
            idHistory.append(id);
            checkTree(id, stopId, idHistory, treeMap);
            {
                // get record ids of all items in the
                // tree
                foreach (const int& hid, idHistory)
                {
                    //resIt = rowMap.find(hid);
                    QList<int> rows = rowMap.values(hid);
                    foreach (const int& r, rows)
                    {
                        recordIdList.insert(r);
                    }
                }
                idIter = treeMap.cend();
            }
        }

        if (mode == 0 && startId == -9999)
        {
            ++idIter;
        }
        else
        {
            idIter = treeMap.cend();
        }
    }

    return recordIdList.toList();
}

QStringList
LUMASSMainWin::getNextParamExpr(const QString& expr)
{
    QStringList innerExpr;

    QList<int> startPos;

    for (int i=0; i < expr.size(); ++i)
    {
        if (expr.at(i) == '[')
        {
            startPos << i;
        }
        else if (expr.at(i) == ']')
        {
            if (startPos.size() > 0)
            {
                int start = startPos.last();
                int len = i - start + 1;
                QStringRef sub = expr.midRef(start, len);
                innerExpr << sub.toString();
                startPos.clear();
            }
        }
    }

    return innerExpr;
}


void LUMASSMainWin::test()
{

}


bool
LUMASSMainWin::checkTree(const int& rootId, const int &stopId,
                          QList<int>& idHistory,
                          const QMultiMap<int, int> &treeMap)
{
    /// note:
    /// this method checks tree's going down (upstream)
    /// or up (downstream); bifurcations, e.g. when going
    /// upstream, are supported by using the QMultiMap treeMap,
    /// i.e. multiple keys are allowed (a catchment can occur
    /// multiple times as downstream catchment); once a 'dead end'
    /// is found it is marked by 'ret = false', preventing further
    /// investigation following this id; however other possible
    /// branches are still investigated by taking the next id from
    /// the dnList ...

    bool ret = true;

    QList<int> dnList = treeMap.values(rootId);
    foreach (const int& dn, dnList)
    {
        // reached the top (of the tree) ...
        // ... or the bottom (of the catchment)
        if (dn == stopId)
        {
            idHistory.append(dn);
            return true;
        }
        else
        {
            // ... inspected this already ...
            if (idHistory.contains(dn))
            {
                ret = false;
            }
            else
            {
                // ... let's go up/down the tree ...
                idHistory.append(dn);
                if (!(ret = checkTree(dn, stopId, idHistory, treeMap)))
                {
                    ret = false;
                }
            }
        }
    }

    return ret;
}


/// only for debug and testing purposes
int
LUMASSMainWin::sqlite_resCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
    //NMDebugAI(<< "SQLite query results:" << std::endl);

    int i;
    for(i=0; i<argc; i++)
    {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}

void LUMASSMainWin::zoomFullExtent()
{
    int nlayers = this->mLayerList->getLayerCount();
	if (nlayers <= 0)
		return;

	this->mBkgRenderer->ResetCamera(const_cast<double*>(
            this->mLayerList->getMapBBox()));

    NMGlobalHelper::getRenderWindow()->Render();
}

void LUMASSMainWin::zoomToCoords(double *box)
{
    int nlayers = this->mLayerList->getLayerCount();
    if (nlayers <= 0)
        return;

    this->mBkgRenderer->ResetCamera(box);

    NMGlobalHelper::getRenderWindow()->Render();
}

void LUMASSMainWin::removeAllObjects()
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

    int nlayers = this->mLayerList->getLayerCount();
	int seccounter = 0;
	while(nlayers > 0)
	{
        NMLayer* l = this->mLayerList->getLayer(nlayers-1);
        this->mLayerList->removeLayer(l->objectName());
        nlayers = this->mLayerList->getLayerCount();

		if (seccounter > 100) break;
		seccounter++;
	}
    mTableList.clear();

	QMap<QString, QPair<void*, sqlite3*> >::iterator it = mTableAdminObjects.begin();
	while (it != mTableAdminObjects.end())
	{
		void* cache = it.value().first;
		sqlite3* conn = it.value().second;

		sqlite3_close(conn);
        spatialite_cleanup_ex(cache);
		cache = 0;
		conn = 0;
		++it;
	}
    mTableAdminObjects.clear();

    mTableListWidget->clear();

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void LUMASSMainWin::pickObject(vtkObject* obj)
{
    // picking implementation only works properly in 2d mode
    if (m_b3D)
    {
        return;
    }

    // get interactor
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(
			obj);

    int rwWidth = iren->GetRenderWindow()->GetSize()[0];
    int rwHeight = iren->GetRenderWindow()->GetSize()[1];
    //NMLogDebug(<< "renwin size: " << rwWidth << ", " << rwHeight);

	if (iren->GetShiftKey())
		return;

	// get event position
	int event_pos[2];
	iren->GetEventPosition(event_pos);

    int rawpos[2] = {event_pos[0], event_pos[1]};

#ifdef QT_HIGHDPI_SUPPORT
#   ifndef VTK_OPENGL2
        event_pos[0] = (qreal)event_pos[0]*this->devicePixelRatioF();
        event_pos[1] = (qreal)event_pos[1]*this->devicePixelRatioF();
#   endif
#endif

	// get the selected layer or quit
    NMLayer* l = this->mLayerList->getSelectedLayer();
	if (l == 0)
		return;

	double wPt[4];
    double raw_wPt[4];

	//	vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
	//	picker->Pick(event_pos[0], event_pos[1], 0, const_cast<vtkRenderer*>(l->getRenderer()));
	//	vtkIdType pickedId = picker->GetCellId();

	vtkInteractorObserver::ComputeDisplayToWorld(this->mBkgRenderer,
			event_pos[0], event_pos[1], 0, wPt);
	wPt[2] = 0;

    vtkInteractorObserver::ComputeDisplayToWorld(this->mBkgRenderer,
            rawpos[0], rawpos[1], 0, raw_wPt);
    raw_wPt[2] = 0;


    //NMLogDebug(<< setprecision(2) << fixed << "raw: " << raw_wPt[0] << ", " << raw_wPt[1]);
    //NMLogDebug(<< setprecision(2) << fixed << "dpi: " << wPt[0] << ", " << wPt[1]);

    vtkIdType cellId = -1;
	// ==========================================================================
	// 									VECTOR PICKING
	// ==========================================================================
	if (l->getLayerType() == NMLayer::NM_VECTOR_LAYER)
	{
		NMVectorLayer* vl = qobject_cast<NMVectorLayer*>(l);
        if (    vl->getFeatureType() != NMVectorLayer::NM_POLYGON_FEAT
             || vl->getTable() == nullptr
           )
        {
            return;
        }

		vtkDataSet* ds = const_cast<vtkDataSet*>(l->getDataSet());
		vtkDataSetAttributes* dsAttr = ds->GetAttributes(vtkDataSet::CELL);
		vtkDataArray* nmids = dsAttr->GetArray("nm_id");
		vtkDataArray* hole = dsAttr->GetArray("nm_hole");
        //vtkDataArray* sa = dsAttr->GetArray("nm_sel");

		vtkPolyData* pd = vtkPolyData::SafeDownCast(ds);
        //vtkPoints* cellPoints = pd->GetPoints();
        long ncells = pd->GetNumberOfCells();

		vtkCell* cell;
        //long long subid, inout;
        //double pcoords[3], clPt[3], dist2;
        //double* weights = new double[pd->GetMaxCellSize()];
		bool in = false;
		QList<vtkIdType> vIds;
		QList<vtkIdType> holeIds;
		QList<vtkIdType> vnmIds;

        //NMDebugAI(<< "analysing cells ..." << endl);
		for (long c = 0; c < ncells; c++)
		{
			if (hole->GetTuple1(c) == 0)
			{
				//		NMDebugAI( << "cell (nmid) " << nmids->GetTuple1(c) << ":" << endl);
				cell = pd->GetCell(c);
                in = this->ptInPoly2D(wPt, cell);
                //                vtkPolygon* poly = vtkPolygon::SafeDownCast(cell);
                //                double n[3];
                //                double* pts = static_cast<vtkDoubleArray*>(cell->GetPoints()->GetData())->GetPointer(0);
                //                poly->ComputeNormal(poly->GetNumberOfPoints(), pts, n);
                //                in = vtkPolygon::PointInPolygon(
                //                            wPt, cell->GetNumberOfPoints(),
                //                            pts,
                //                            cell->GetBounds(),
                //                            n);
                if (in)
				{
					vIds.push_back(c);
					vnmIds.push_back(nmids->GetTuple1(c));
					// don't really what this is for anymore
					if (hole->GetTuple1(c) == 1)
						  holeIds.push_back(c);
				}
			}
		}

		//	// remove holes from result vector
		//	for (int k=0; k < vIds.size(); ++k)
		//	{
		//		if (holeIds.contains(vIds.at(k)))
		//		{
		//			vIds.removeOne(vIds.at(k));
		//		}
		//	}

		if (vIds.size() > 1)
		{
			NMDebug(<< endl);
			NMDebugAI(<< "WARNING - multiple hits - cellIds: ");

			foreach(const int &id, vIds)
				NMDebug(<< id << " ");
			NMDebug(<< endl);

			NMDebugAI(<< "                          - nmids: ");
			foreach(const int &id, vnmIds)
				NMDebug(<< id << " ");
			NMDebug(<< endl);
		}

		// the doc widget hosting the layer info table

        QList<vtkIdType> lstCellId;
        QList<vtkIdType> lstNMId;
		if (vIds.size() == 0)
		{
            // this is mean if you're in a tedious selection process
            // and only because the bloody picking algorithm doesn't
            // work, you've got to start again - so we don't do it;
            // note, the user can deselect via the table view
            //l->selectCell(0, NMLayer::NM_SEL_CLEAR);
			this->updateLayerInfo(l, -1);
			return;
		}

		cellId = vIds.last();
        vtkIdType nmid = nmids->GetTuple1(cellId);
		lstCellId.push_back(cellId);
		lstNMId.push_back(nmid);

    }
	// ==========================================================================
	// 									PIXEL PICKING
	// ==========================================================================
	else
	{
		NMImageLayer *il = qobject_cast<NMImageLayer*>(l);
        if (il->getTable() == 0)
        {
					return;
        }

		vtkImageData *img = vtkImageData::SafeDownCast(
				const_cast<vtkDataSet*>(l->getDataSet()));

        //int* ext = img->GetExtent();

		if (img == 0)
			return;

		int did[3] = {-1,-1,-1};
        il->world2pixel(wPt, did, false, true);

        // normally, we're dealing with 2D images anyway,
        //did[2] = did[2] < 0 ? 0 : did[2];

        for (unsigned int d=0; d < 2; ++d)
		{
			if (did[d] < 0)
			{
				this->updateLayerInfo(l, -1);
				l->selectCell(cellId, NMLayer::NM_SEL_CLEAR);
				return;
			}
		}

        vtkDataArray* idxScalars = img->GetPointData()->GetArray(0);
        void* idxPtr = img->GetArrayPointer(idxScalars, did);

        if (idxScalars != 0 && idxPtr != 0)
        {
            switch(idxScalars->GetDataType())
            {
            vtkTemplateMacro(getVtkIdTypeFromVtkTypedPtr(
                                 static_cast<VTK_TT*>(idxPtr), &cellId)
                        );
            default:
                NMWarn(ctxLUMASSMainWin, << "Scalar pointer type not supported!");
            }
        }
    }

    // select cell if the user pressed the Ctrl key,
    // otherwise just show the layer info,
    // this allows the user to be able to inspect
    // selected cells - much more useful this way round,
    // I guess ... ?
    if (iren->GetControlKey() && l->isSelectable())
    {
        l->selectCell(cellId, NMLayer::NM_SEL_ADD);
    }
    else
    {
        this->updateLayerInfo(l, cellId);
    }

    // =========================================
    //       EXECUTE USER ACTIONS
    // =========================================
    processUserPickAction(cellId);

}


bool
LUMASSMainWin::ptInPoly2D(double pt[3], vtkCell* cell)
{
//	NMDebugCtx(ctxLUMASSMainWin, << "...");

	// although getting a 3-pt, we're only dealing with x/y coordinates;
	// ray function used: y = ax + b
	// here we're testing whether the ray 'y = 0x + pt[1]' intersects with
	// the line segments of the polygon and count the intersections
	bool ret = false;

	pt[2] = 0;
	double bnd[6];
    double delta[] = {0,0,0};
	cell->GetBounds(bnd);
	bnd[4] = -1;
	bnd[5] = 1;

    if (!vtkMath::PointIsWithinBounds(pt, bnd, delta))
        return false;

	// alloc memory
	double* x = new double[2]; // the load vector taking the 'b' coefficient of the ray function
	double** a = new double*[2];
	for (int i=0; i < 2; ++i)
		a[i] = new double[2];

    //        NMDebugAI(<< "test point: " << pt[0] << " " << pt[1] << " "
    //                  << pt[3] <<  endl);

	// we assume here, that the points are ordered either anti-clockwise or clockwise
	// iterate over polygon points, create line segments
	double s1[2];
	double s2[2];
	double bs;
	double as;
	double segbnd[4];
    double dy, dx;
	int onseg = 0;
	int retcnt = 0;
	for (int i=0; i < cell->GetNumberOfPoints()-1; ++i)
	{
		// get points
		s1[0] = cell->GetPoints()->GetPoint(i)[0];
		s1[1] = cell->GetPoints()->GetPoint(i)[1];
		s2[0] = cell->GetPoints()->GetPoint(i+1)[0];
		s2[1] = cell->GetPoints()->GetPoint(i+1)[1];

		segbnd[0] = (s1[0] < s2[0]) ? s1[0] : s2[0];
		segbnd[1] = (s1[0] > s2[0]) ? s1[0] : s2[0];
		segbnd[2] = (s1[1] < s2[1]) ? s1[1] : s2[1];
		segbnd[3] = (s1[1] > s2[1]) ? s1[1] : s2[1];


		// calc as and bs
        // avoid 0 slope of ray
        dy = s2[1] - s1[1];
        dx = s2[0] - s1[0];


        a[1][1] = 1;                                // coefficient for y of polygon segment
        if (dx == 0)
        {
            bs = s1[0];
            as = -1;
            a[1][1] = 0;
        }
        else
        {
            as = dy / dx;
            bs = s1[1] - (as * s1[0]);
        }

		// fill load vector and parameter matrix
		x[0] = pt[1];								// b of test ray
		x[1] = bs;      							// b of polygon segment
        a[0][0] = 0; 		 a[0][1] = 1;			// coefficients for x and y of test ray
        a[1][0] = as * (-1);            	    	// coefficient for x

        //		// DEBUG
        //        NMDebugAI(<< "S-" << i+1 << ": " << "S1(" << s1[0] << ", " << s1[1] << ") "
        //                  << "S2(" << s2[0] << ", " << s2[1] << "): y = " << as << "x + "
        //                  << bs << std::endl);

        //        NMDebugAI(<< "      Linear System: ..." << endl);
        //        NMDebugAI(<< "      " << x[0] << " = " << a[0][0] << " " << a[0][1] << endl);
        //        NMDebugAI(<< "      " << x[1] << " = " << a[1][0] << " " << a[1][1] << endl);

		onseg = 0;
		if (vtkMath::SolveLinearSystem(a, x, 2))
		{
			if ( x[0] >= pt[0] &&
				 ((x[0] >= segbnd[0] && x[0] <= segbnd[1]) &&	// test segment boundary
				  (x[1] >= segbnd[2] && x[1] <= segbnd[3])   )    )
			{
				onseg = 1;
				++retcnt;
			}
		}
        //        NMDebug( << "\t\thit: " << onseg << endl);

	}
    //    NMDebugAI(<< "total hits: " << retcnt << endl << endl);

	// check whether retcnt is odd (=inside) or even (=outside)
	if (retcnt > 0 && retcnt % 2 != 0)
		ret = true;

	// release memory
	delete[] x;
	for (int i=0; i < 2; ++i)
		delete[] a[i];
	delete[] a;

        //NMDebugCtx(ctxLUMASSMainWin, << "done!");
	return ret;
}

//void
//LUMASSMainWin::searchModelComponent()
//{
//    QLineEdit* le = qobject_cast<QLineEdit*>(sender());
//    if (le)
//    {
//        QString compName = le->text();

//        // COMPONENT MODEL NAME ENTERED
//        if (NMGlobalHelper::getModelController()->contains(compName))
//        {
//            emit componentOfInterest(compName);
//        }
//        // LOOK FOR EITHER USER-ID OR PARAMETERS
//        else
//        {
//            // to indicate whether we've found a parameter
//            QStringList compProp;


//            // LOOK FOR COMPONENTS
//            QList<NMModelComponent*> comps = NMGlobalHelper::getModelController()->getComponents(compName);
//            if (compName.isEmpty() ? comps.size()-1 : comps.size() > 0)
//            {
//                QStringList nameList;
//                foreach (const NMModelComponent* mc, comps)
//                {
//                    nameList << mc->objectName();
//                }
//                if (nameList.size() > 1)
//                {
//                    NMLogWarn(<< "Model Controller: components matching userID='"
//                              << compName.toStdString() << "': "
//                              << nameList.join(' ').toStdString());
//                }

//                emit componentOfInterest(comps.at(0)->objectName());
//            }
//            // LOOK for PARAMETERS
//            else if (!compName.isEmpty())
//            {
//                NMModelController* mc = NMGlobalHelper::getModelController();
//                QMap<QString, NMModelComponent*> mmap = mc->getRepository();
//                QMap<QString, NMModelComponent*>::const_iterator mit = mmap.cbegin();
//                while (mit != mmap.cend())
//                {
//                    NMModelComponent* comp = const_cast<NMModelComponent*>(mit.value());
//                    QStringList props = NMGlobalHelper::searchPropertyValues(comp, compName);
//                    foreach(const QString& p, props)
//                    {
//                        compProp << QString::fromLatin1("%1:%2")
//                                    .arg(mit.key())
//                                    .arg(p);
//                    }

//                    NMSequentialIterComponent* pc = qobject_cast<NMSequentialIterComponent*>(comp);
//                    if (pc && pc->getProcess() != 0)
//                    {
//                        QStringList procProps = NMGlobalHelper::searchPropertyValues(pc->getProcess(),
//                                                                                     compName);
//                        foreach(const QString& p1, procProps)
//                        {
//                            compProp << QString::fromLatin1("%1:%2")
//                                        .arg(mit.key())
//                                        .arg(p1);
//                        }
//                    }

//                    ++mit;
//                }
//            }

//            if (compProp.size() > 0)
//            {
//                NMLogInfo(<< "Found '" << compName.toStdString() << "' in these components and properties: "
//                          << compProp.join(' ').toStdString());
//            }
//            else if (!compName.isEmpty() && compProp.size() == 0)
//            {
//                NMLogInfo(<< "'" << compName.toStdString()
//                      << "' neither references a model component nor a model parameter!");
//            }
//            else
//            {
//                NMLogInfo(<< "No model component found!");
//            }
//        }
//    }
//}

void
LUMASSMainWin::updateCoordLabel(const QString& newCoords)
{
	this->m_coordLabel->setText(newCoords);
}

//void
//LUMASSMainWin::setCurrentInteractorLayer(const NMLayer* layer)
//{
////	for (int i=0; i < mLayerList->getLayerCount(); ++i)
////	{
////		NMLayer* l = const_cast<NMLayer*>(mLayerList->getLayer(i));
////		vtkRenderer* ren = const_cast<vtkRenderer*>(l->getRenderer());
////		ren->SetInteractive(0);
////	}
////
////	NMLayer* la = const_cast<NMLayer*>(layer);
////	vtkRenderer* renA = const_cast<vtkRenderer*>(la->getRenderer());
////	renA->SetInteractive(1);
//
//}

void LUMASSMainWin::updateCoords(vtkObject* obj)
{
	// get interactor
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(
			obj);


	// get event position
	int event_pos[2];
	iren->GetEventPosition(event_pos);

#ifdef QT_HIGHDPI_SUPPORT
#   ifndef VTK_OPENGL2
        event_pos[0] = (qreal)event_pos[0]*this->devicePixelRatioF();
        event_pos[1] = (qreal)event_pos[1]*this->devicePixelRatioF();
#   endif
#endif

	double wPt[4];
	vtkInteractorObserver::ComputeDisplayToWorld(this->mBkgRenderer,
			event_pos[0], event_pos[1], 0, wPt);
	wPt[2] = 0;

	// update label
    QString s = QString("XY Location: %1, %2 "). // Z: %3").
    arg(wPt[0], 0, 'f', 3).arg(wPt[1], 0, 'g', 10); //.arg(wPt[3],0,'f',2);

	this->m_coordLabel->setText(s);

    // generated pixel information is not meaningful in 3d, so
    // lets skip that
    if (m_b3D)
    {
        this->mPixelValLabel->setText("");
        return;
    }

    // =======================================================================================
    // get pixel value of top 10 visible image layers in the order from top to bottom
    int vl=0;
    for (int l=this->mLayerList->getLayerCount()-1; l >= 0 && vl < 10; --l)
    {
<<<<<<< HEAD
<<<<<<< HEAD
        if (    mLayerList->getLayer(l)->isVisible()
             && mLayerList->getLayer(l)->getLayerType() == NMLayer::NM_IMAGE_LAYER
            )
=======
        if (mLayerList->getLayer(l)->isVisible())
>>>>>>> 977af29... enh: lumassmainwin
=======
        if (    mLayerList->getLayer(l)->isVisible()
             && mLayerList->getLayer(l)->getLayerType() == NMLayer::NM_IMAGE_LAYER
            )
>>>>>>> 10b37d1... fix: replace deprecated QVTKWidget with QVTKOpenGLWidget
        {
            ++vl;
        }
    }

    QString pixval = "";
    stringstream visvs, pixStr, lprPixStr;
    for (int i=this->mLayerList->getLayerCount()-1, cnt=1; i >= 0 && cnt <= vl; --i)
    {
        NMImageLayer* il = qobject_cast<NMImageLayer*>(this->mLayerList->getLayer(i));
        if (il == nullptr || !il->isVisible())
        {
            continue;
        }

        // get the current pixel and LPR pixel cooridnates for the top most
        // visible layer
        vtkImageData* img = vtkImageData::SafeDownCast(
                const_cast<vtkDataSet*>(il->getDataSet()));

<<<<<<< HEAD
<<<<<<< HEAD
        // bail out on error
        if (img == 0)
        {
            continue;
        }
=======
        if (img == 0)
            return;
>>>>>>> 977af29... enh: lumassmainwin
=======
        // bail out on error
        if (img == 0)
        {
            continue;
        }
>>>>>>> 10b37d1... fix: replace deprecated QVTKWidget with QVTKOpenGLWidget

        int ext[6];
        img->GetExtent(ext);

        int did[3] = {-1,-1,-1};
        int lprpix[3] = {-1,-1,-1};

        il->world2pixel(wPt, did, false, false);
        il->world2pixel(wPt, lprpix, true, false);

        // keep the pixel and LPR pixel coordinates just for the
        // topmost visible layer!
        if (cnt == 1)
        {
            pixStr << did[0] << ", " << did[1] << ", " << did[2];
            lprPixStr << lprpix[0] << ", " << lprpix[1] << ", " << lprpix[2];
        }

        stringstream cvs;
        cvs << setprecision(10);
        if (    (did[0] >= ext[0] && did[0] <= ext[1])
             && (did[1] >= ext[2] && did[1] <= ext[3])
             && (did[2] >= ext[4] && did[2] <= ext[5])
           )
        {
            for (unsigned int d=0; d < img->GetNumberOfScalarComponents(); ++d)
            {
                cvs << img->GetScalarComponentAsDouble(did[0], did[1], did[2], d)
                        << " ";
            }
        }
        else
        {
            cvs << "nodata ";
        }

        // need to fetch the proper attribute value, when scalars represent
        // the row index of the RAT
        if (    il->getTable() != nullptr
            &&  il->getLegendType() == NMLayer::NM_LEGEND_INDEXED
            &&  !il->useIdxMap()
           )
        {
            int lrow = ::atoi(cvs.str().c_str());

            QAbstractItemModel* itemModel = const_cast<QAbstractItemModel*>(il->getTable());

            NMQtOtbAttributeTableModel* ramModel = qobject_cast<NMQtOtbAttributeTableModel*>(itemModel);
            NMSqlTableModel* sqlModel = qobject_cast<NMSqlTableModel*>(itemModel);

           if (sqlModel != nullptr)
           {
               QString queryStr = QString("SELECT %1 from %2 where %3 = %4")
                       .arg(il->getLegendValueField())
                       .arg(sqlModel->tableName())
                       .arg(sqlModel->getNMPrimaryKey())
                       .arg(lrow);

               QSqlQuery q(sqlModel->database());
               if (q.exec(queryStr))
               {
                   if (q.next())
                   {
                       cvs.str("");
                       cvs << q.value(0).toString().toStdString();
                   }
               }
           }
           // here we assume a 0-based indexed attribute table
           else if (ramModel != nullptr)
           {
               int colidx = 0;
               for (int i=0; i < ramModel->columnCount(); ++i)
               {
                   QVariant colName = ramModel->headerData(i, Qt::Horizontal);
                   if (il->getLegendValueField().compare(colName.toString(), Qt::CaseInsensitive) == 0)
                   {
                       colidx = i;
                       break;
                   }
               }

               QModelIndex mi = ramModel->index(lrow, colidx);
               cvs.str("");
               cvs << mi.data().toString().toStdString().c_str();
           }
        }
        visvs << cvs.str();
        if (cnt < vl)
        {
             visvs << ", ";
        }
        ++cnt;
    }

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 10b37d1... fix: replace deprecated QVTKWidget with QVTKOpenGLWidget
    // no message, no display
    if (    visvs.str().size() == 0
         || lprPixStr.str().size() == 0
         || pixStr.str().size() == 0
       )
    {
        this->mPixelValLabel->setText("");
    }
    else
    {
        pixval = QString("| Pixel: %1 | LPRPixel: %2 | Values: %3 ").
                    arg(pixStr.str().c_str()).
                    arg(lprPixStr.str().c_str()).
                    arg(visvs.str().c_str());
        this->mPixelValLabel->setText(pixval);
    }
<<<<<<< HEAD
=======
    pixval = QString("| Pixel: %1 | LPRPixel: %2 | Values: %3 ").
                arg(pixStr.str().c_str()).
                arg(lprPixStr.str().c_str()).
                arg(visvs.str().c_str());
	this->mPixelValLabel->setText(pixval);
>>>>>>> 977af29... enh: lumassmainwin
=======
>>>>>>> 10b37d1... fix: replace deprecated QVTKWidget with QVTKOpenGLWidget
}

void
LUMASSMainWin::showBusyStart()
{
    if (this->sender())
    {
        NMDebugAI(<< this->sender()->objectName().toStdString() << " - turned busy!" << std::endl);
    }
    QString msg = QString("processing ..."); //.arg(this->sender()->objectName());
	this->m_StateMsg->setText(msg);
	this->mProgressBar->reset();
    this->mProgressBar->setMinimum(0);
    this->mProgressBar->setMaximum(0);
    this->mProgressBar->setVisible(true);
    this->mBusyProcCounter++;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void
LUMASSMainWin::showBusyEnd()
{
    this->mBusyProcCounter--;
    if (mBusyProcCounter == 0)
    {
        if (this->sender())
        {
            NMDebugAI(<< this->sender()->objectName().toStdString() << " - stopped processing!" << std::endl);
        }
        this->m_StateMsg->setText("");
        this->mProgressBar->setVisible(false);
    }
    QApplication::restoreOverrideCursor();
}

void
LUMASSMainWin::showBusyValue(int value)
{
    if (this->mBusyProcCounter)
    {
        this->mProgressBar->setValue(value);
    }
}

void LUMASSMainWin::showComponentsView(bool vis)
{
    this->ui->componentsWidget->setVisible(vis);
}

void
LUMASSMainWin::showNotificationsView(bool vis)
{
    ui->logDock->setVisible(vis);
}

void LUMASSMainWin::showComponentsInfoView(bool vis)
{
    ui->infoDock->setVisible(vis);
    ui->componentInfoDock->setVisible(vis);

    // a bit of a dirty hack to avoid ill-aligned display
    // of the layer info table widget; should be really resolved in
    // NMWidgetListView
    QWidget* w = ui->infoWidgetList->getWidgetItem(0);
    if (w == 0)
    {
        return;
    }
    bool livis = w->isVisible();
    ui->infoWidgetList->setWidgetItemVisible(0, true);
    ui->infoWidgetList->setWidgetItemVisible(0, false);
    ui->infoWidgetList->setWidgetItemVisible(0, livis);
}


void LUMASSMainWin::doMOSO()
{
    //NMDebugCtx(ctxLUMASSMainWin, << "...");

	QString fileName = QFileDialog::getOpenFileName(this,
	     tr("Open Optimisation Settings"), "~", tr("LUMASS Optimisation Settings (*.los)"));

	if (fileName.isNull())
	{
        //NMDebugAI( << "Please provide a filename!" << endl);
        mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                              NMLogger::NM_LOG_INFO,
                              "Please specifiy file to run an optimisation scenario!");
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return;
	}

	QFileInfo fileinfo(fileName);

    //QDir parentDir = fileinfo.absoluteDir();
    //parentDir.cdUp();
    QString path = fileinfo.absoluteDir().path();
	QString baseName = fileinfo.baseName();
	if (!fileinfo.isReadable())
	{
        NMLogError(<< ctxLUMASSMainWin << ": Could not read file '" << fileName.toStdString() << "'!");
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return;
	}

    QString tresPath = ::getenv("MOSO_RESULT_PATH");
    if (tresPath.isEmpty())
    {
        tresPath = path;
    }
    QFileInfo pathInfo(tresPath);
    QDir parentDir(pathInfo.path());

    QStringList dirList;
    dirList << "opt_report" << "opt_lucmatrix" << "opt_result"
            << "opt_relative" << "opt_total" << "opt_lp" << "opt_tab";

    for (int d=0; d < dirList.size(); ++d)
    {
        QString dir = dirList.at(d);
        NMDebugAI(<< dirList.at(d).toStdString() << " -> ");
        if (!parentDir.exists(dir))
        {
            if (!parentDir.mkdir(dir))
            {
                dir = fileinfo.dir().dirName();
                dirList.replace(d, dir);
            }
        }
        NMDebug(<< dirList.at(d).toStdString() << endl );
    }

	// create a new optimisation object
	NMMosra* mosra = new NMMosra(this);
    mosra->setLogger(mLogger);

	// load the file with optimisation settings
	mosra->loadSettings(fileName);

	// look for the layer mentioned in the settings file
    NMLayer* layer = this->mLayerList->getLayer(mosra->getLayerName());
	if (layer == 0)
	{
        //NMDebugAI( << "couldn't find layer '" << mosra->getLayerName().toStdString() << "'" << endl);
        mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                              NMLogger::NM_LOG_ERROR,
                              QString("Could not find layer '%1'!")
                                 .arg(mosra->getLayerName())
                              );
		delete mosra;
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return;
	}

	// now set the layer, do moso and clean up
	//mosra->setLayer(layer);
    mosra->setDataSet(layer->getDataSet());
    mosra->setTimeOut(0); // clears any time out setting ...
    mosra->setBreakAtFirst(true);
    //NMDebugAI(<< "split off solving to seperate thread ... " << endl);

    QDateTime started = QDateTime::currentDateTime();

    //QFuture<int> future = QtConcurrent::run(mosra, &NMMosra::solveLp);

    //    NMDebugAI(<< "waiting 10 secs ..." << endl);
    //::sleep(10);

    //    QMessageBox::StandardButton btn = QMessageBox::question(this, "Abort Optimisation?",
    //                                                            "Press 'Yes' to abort the current optimisation run!");
    //    if (btn == QMessageBox::Yes)
    //    {
    //        mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
    //                              NMLogger::NM_LOG_ERROR,
    //                              QString("Could not find layer '%1'!")
    //                                 .arg(mosra->getLayerName())
    //                              );
    //        //NMDebugAI(<< "cancel solving ..." << endl);
    //        mosra->cancelSolving();
    //    }

    //    // asking the user for the lp timeout
    //    bool timeok;
    //    int timeout = QInputDialog::getInt(this, tr("lp_solve timeout"), tr("timeout in secs:"), 60, 5, 86400, 30, &timeok);
    //    if (!timeok)
    //        return;
    //    mosra->setTimeOut(timeout);

    QString sRepName = QString(tr("%1/%2/report_%3.txt")).arg(pathInfo.path())
            .arg(dirList.at(0))
            .arg(baseName);

    //if (!future.result())
    if (!mosra->solveLp())
	{
        mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                              NMLogger::NM_LOG_ERROR,
                              QString("We encountered trouble setting/solving the problem "
                              "or the optimisation was aborted - s. report '%1'")
                              .arg(sRepName));

        NMDebugAI(<< "We encountered trouble setting/solving the problem or the optimisation was aborted!" << std::endl);
        NMDebugAI( << "write report to '" << sRepName.toStdString() << "'" << endl);
        mosra->writeReport(sRepName);

        delete mosra;
		return;
	}

    QDateTime stopped = QDateTime::currentDateTime();
    int msec = started.msecsTo(stopped);
    int min = msec / 60000;
    double sec = (msec % 60000) / 1000.0;

    QString elapsedTime = QString("%1:%2").arg((int)min).arg(sec,0,'g',3);
    NMDebugAI(<< "Optimisation took (min:sec): " << elapsedTime.toStdString() << endl);
    NMMsg(<< "Optimisation took (min:sec): " << elapsedTime.toStdString() << endl);

    mLogger->processLogMsg(QDateTime::currentDateTime().time().toString(),
                          NMLogger::NM_LOG_INFO,
                          QString("Optimisation took (min:sec): %1")
                             .arg(elapsedTime)
                          );

	// ============================================================================
    NMDebugAI( << "write report to '" << sRepName.toStdString() << "'" << endl);
    mosra->writeReport(sRepName);


	int solved = mosra->mapLp();
	//layer->emitDataSetChanged();
	layer->tableDataChanged(QModelIndex(), QModelIndex());


	if (solved)
	{
        // ==========================================================================
        // SUMMARISE THE DATA BEFORE WE MESS AROUND WITH IT
        // once we've got a feasible result, we write the change matrix and
        // the optimisation result table out
        vtkSmartPointer<vtkTable> changeTab;
        vtkSmartPointer<vtkTable> resTab = mosra->sumResults(changeTab);

        // =========================================================================
        // export attribute table

        vtkSmartPointer<vtkTable> origtab = mosra->getDataSetAsTable();
        vtkSmartPointer<vtkTable> tab = vtkSmartPointer<vtkTable>::New();
        tab->DeepCopy(origtab);

        vtkUnsignedCharArray* hole = vtkUnsignedCharArray::SafeDownCast(
                    tab->GetColumnByName("nm_hole"));

        // filter 'hole' polygons
        int nrows = tab->GetNumberOfRows();
        int r = 0;
        while(r < nrows)
        {
            if (hole->GetValue(r))
            {
                tab->RemoveRow(r);
                --nrows;
            }
            else
            {
                ++r;
            }
        }

        // get rid of admin fields
        tab->RemoveColumnByName("nm_id");
        tab->RemoveColumnByName("nm_hole");
        tab->RemoveColumnByName("nm_sel");

        // setup the writer
        vtkSmartPointer<vtkDelimitedTextWriter> writer =
                vtkSmartPointer<vtkDelimitedTextWriter>::New();
        writer->SetFieldDelimiter(",");

        QString tabName = QString("%1/%2/tab_%3.csv").arg(pathInfo.path())
                .arg(dirList.at(6))
                .arg(baseName);

        writer->SetInputData(tab);
        writer->SetFileName(tabName.toStdString().c_str());
        writer->Update();

        // =========================================================================
		NMDebugAI( << "visualising optimisation results ..." << endl);


        QString resName = QString("%1/%2/res_%3.csv").arg(pathInfo.path())
                .arg(dirList.at(2))
                .arg(baseName);

		// show table if we got one
		if (resTab.GetPointer() != 0)
		{
            NMDebugAI(<< "writing optimisation results -> '"
                      << resName.toStdString() << "'" << std::endl);
            writer->SetInputData(resTab);
            writer->SetFileName(resName.toStdString().c_str());
            writer->Update();

            int ncols = resTab->GetNumberOfColumns();
            int numCri = (ncols-1)/4;

            // create table only containing the absolute performance
            // after this optimisation run
            vtkSmartPointer<vtkTable> optTab = vtkSmartPointer<vtkTable>::New();
            optTab->AddColumn(resTab->GetColumn(0));
            for (int i=0, off=2; i < numCri; ++i, off += 4)
            {
                optTab->AddColumn(resTab->GetColumn(off));
            }

            QString totalName = QString("%1/%2/tot_%3.csv").arg(pathInfo.path())
                    .arg(dirList.at(4))
                    .arg(baseName);
            writer->SetInputData(optTab);
            writer->SetFileName(totalName.toStdString().c_str());
            writer->Update();


            // create table only containing relative changes (%)
            // from original result table
            for (int i=0; i < numCri; ++i)
            {
                for (int re=0; re < 3; ++re)
                {
                    resTab->RemoveColumn(i+1);
                }
            }

            QString relName = QString("%1/%2/rel_%3.csv").arg(pathInfo.path())
                    .arg(dirList.at(3))
                    .arg(baseName);
            writer->SetInputData(resTab);
            writer->SetFileName(relName.toStdString().c_str());
            writer->Update();

            vtkQtTableModelAdapter* tabModel = new vtkQtTableModelAdapter(this);
            NMFastTrackSelectionModel* ftsm = new NMFastTrackSelectionModel(tabModel, this);
            tabModel->setTable(resTab);
            NMTableView* tv = new NMTableView(tabModel, 0);
            tv->setSelectionModel(ftsm);
            tv->setTitle(tr("Performance Change!"));
            tv->show();
		}

        if (changeTab.GetPointer() != 0)
        {
            vtkQtTableModelAdapter* tabModel = new vtkQtTableModelAdapter(this);
            NMFastTrackSelectionModel* ftsm = new NMFastTrackSelectionModel(tabModel, this);
            tabModel->setTable(changeTab);
            NMTableView* tv = new NMTableView(tabModel, 0);
            tv->setSelectionModel(ftsm);
            tv->setTitle(tr("Change Matrix!"));
            tv->show();

            QString chgName = QString("%1/%2/chg_%3.csv").arg(pathInfo.path())
                    .arg(dirList.at(1))
                    .arg(baseName);

            NMDebugAI(<< "writing change matrix -> '"
                      << chgName.toStdString() << "'" << std::endl);
            writer->SetInputData(changeTab);
            writer->SetFileName(chgName.toStdString().c_str());
            writer->Update();
        }

        // obviously, we have to prepare the table a bit better
        // TODO: need to switch the NMChartWidget to VTK's new chart framework!
        //		QStandardItemModel* model = this->prepareResChartModel(resTab);
        //		if (model != 0)
        //		{
        //			NMChartWidget* cw = new NMChartWidget(this);
        //			cw->setChartModel(model);
        //			cw->setWinTitle(tr("Optimisation Change Chart"));
        //			cw->show();
        //		}
	}

    QString lpName = QString(tr("%1/%2/lp_%3.lp")).arg(pathInfo.path())
            .arg(dirList.at(5))
            .arg(baseName);
	mosra->getLp()->WriteLp(lpName.toStdString());
	delete mosra;

	if (solved)
	{
		NMDebugAI( << "mapping unique values ..." << endl);
        //        NMVectorLayer* vl = qobject_cast<NMVectorLayer*>(layer);
        //        vl->mapUniqueValues(tr("OPT_STR"));
		layer->setLegendType(NMLayer::NM_LEGEND_INDEXED);
		layer->setLegendClassType(NMLayer::NM_CLASS_UNIQUE);
		layer->setLegendValueField("OPT_STR");
		layer->setLegendDescrField("OPT_STR");
        //layer->updateLegend();
        layer->updateMapping();
	}

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

QStandardItemModel*
LUMASSMainWin::prepareResChartModel(vtkTable* restab)
{
	if (restab == 0)
		return 0;

        NMDebugCtx(ctxLUMASSMainWin, << "...");


	int nDestCols = restab->GetNumberOfRows();
	int nSrcCols = restab->GetNumberOfColumns();
	int nDestRows = (nSrcCols-1) / 4;

	QStandardItemModel* model = new QStandardItemModel(nDestRows, nDestCols, this->parent());
	model->setItemPrototype(new QStandardItem());

	NMDebugAI( << "populating table ..." << endl);

	QStringList slVHeaderLabels;
	int srccol = 4;
	for (int row=0; row < nDestRows; ++row, srccol+=4)
	{
		QString sVHeader = restab->GetColumnName(srccol);
		slVHeaderLabels.append(sVHeader);
		model->setVerticalHeaderItem(row, new QStandardItem());
		model->verticalHeaderItem(row)->setData(QVariant((int)row*40), Qt::DisplayRole);

		for (int col=0; col < nDestCols; ++col)
		{
			if (row == 0)
			{
				QString sHHeader = restab->GetValue(col, 0).ToString().c_str();
				model->setHorizontalHeaderItem(col, new QStandardItem());
				model->horizontalHeaderItem(col)->setData(QVariant(sHHeader), Qt::DisplayRole);
			}

			model->setItem(row, col, new QStandardItem());
			model->item(row, col)->setData(QVariant(restab->GetValue(col, srccol).ToDouble()),
					Qt::DisplayRole);
		}
	}
	model->setVerticalHeaderLabels(slVHeaderLabels);

        NMDebugCtx(ctxLUMASSMainWin, << "done!");

	return model;

}

void LUMASSMainWin::displayChart(vtkTable* srcTab)
{

// NMChartWidget needs switching to VTK 6's new chart framework
//	NMChartWidget* cw = new NMChartWidget(srcTab, this);
//	cw->show();
}

void LUMASSMainWin::loadVTKPolyData()
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

	QString fileName = QFileDialog::getOpenFileName(this,
	     tr("Open XML/Binary VTK PolyData File"), "~",
	     tr("PolyData (*.vtp *.vtk)"));

	if (fileName.isNull())
		return;

	vtkSmartPointer<vtkPolyData> pd;

	//check, what kind of format we've got
	if (fileName.endsWith(".vtp", Qt::CaseInsensitive))
	{
		//QSysInfo::ByteOrder == QSysInfo::BigEndian

		vtkSmartPointer<vtkXMLPolyDataReader> xr = vtkSmartPointer<vtkXMLPolyDataReader>::New();
		xr->SetFileName(fileName.toStdString().c_str());
		xr->Update();
		pd = xr->GetOutput();
	}
	else if (fileName.endsWith(".vtk", Qt::CaseInsensitive))
	{
		vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		pd = reader->GetOutput();
	}

	QFileInfo finfo(fileName);
	QString layerName = finfo.baseName();

	vtkRenderWindow* renWin = this->ui->qvtkWidget->GetRenderWindow();
	NMDebugAI( << "creating the vector layer and assigning data set..." << endl);
	NMVectorLayer* layer = new NMVectorLayer(renWin);
	layer->setFileName(fileName);
	layer->setObjectName(layerName);
	layer->setDataSet(pd);
	layer->setVisible(true);
    this->mLayerList->addLayer(layer);

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void LUMASSMainWin::saveSelectionAsVtkPolyData()
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

//    // get the selected layer
//    NMLayer* l = this->mLayerList->getSelectedLayer();
//    if (l == 0)
//        return;

//    // make sure, we've got a vector layer
//    if (l->getLayerType() != NMLayer::NM_VECTOR_LAYER)
//        return;

//    QString layerName = l->objectName();

//    // take the first layer and save as vtkpolydata
//    QFileDialog dlg(this);
//    dlg.setAcceptMode(QFileDialog::AcceptSave);
//    dlg.setFileMode(QFileDialog::AnyFile);
//    dlg.setWindowTitle(tr("Save As VTK PolyData File"));
//    dlg.setDirectory("~/");
//    dlg.setNameFilter("VTK PolyData File (*.vtk)");

//    QString selectedFilter;
//    QString fileName;
//    if (dlg.exec())
//    {
//        fileName = dlg.selectedFiles().at(0);
//        if (fileName.isNull() || fileName.isEmpty())
//            return;
//        selectedFilter = dlg.selectedNameFilter();
//    }
//    else
//        return;

//    const QItemSelection sel = l->getSelection();




//    NMDebugAI(<< "writing ASCII *.vtk file ..." << endl);
//    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
//    writer->SetFileName(fileName.toStdString().c_str());
//    writer->SetInputData(const_cast<vtkDataSet*>(l->getDataSet()));
//    writer->SetFileTypeToASCII();
//    writer->Update();

    NMDebugCtx(ctxLUMASSMainWin, << "done!");

}

void LUMASSMainWin::saveAsVtkPolyData()
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

	// get the selected layer
    NMLayer* l = this->mLayerList->getSelectedLayer();
	if (l == 0)
		return;

	// make sure, we've got a vector layer
	if (l->getLayerType() != NMLayer::NM_VECTOR_LAYER)
		return;

	QString layerName = l->objectName();

	// take the first layer and save as vtkpolydata
	QFileDialog dlg(this);
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setWindowTitle(tr("Save As VTK PolyData File"));
	dlg.setDirectory("~/");
    //dlg.setNameFilter("XML PolyData (*.vtp);;Binary PolyData (*.vtk)");
    dlg.setNameFilter("VTK PolyData File (*.vtk)");

	QString selectedFilter;
	QString fileName;
//	QString fileName = QFileDialog::getSaveFileName(this,
//			tr("Save As VTK PolyData File (binary/ASCII)"), tr("~/") + layerName + tr(".vtk"),
//			tr("Binary PolyData (*.vtk);;ASCII PolyData (*.vtk)"), &selectedFilter);
	if (dlg.exec())
	{
		fileName = dlg.selectedFiles().at(0);
		if (fileName.isNull() || fileName.isEmpty())
			return;
		selectedFilter = dlg.selectedNameFilter();
	}
	else
		return;

//	if (selectedFilter == "XML PolyData (*.vtp)")
//	{
//		NMDebugAI(<< "writing XML-file " << fileName.toStdString() << " ..." << endl);
//		vtkSmartPointer<vtkXMLPolyDataWriter> xw = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
//		//if (xw.GetPointer() == 0)
//		//{
//		//	NMDebugAI(<< "XML PolyDataWriter is NULL!");
//		//	return;
//		//}
//        vtkDataSet* ds = const_cast<vtkDataSet*>(l->getDataSet());
//        vtkPolyData* pd = vtkPolyData::SafeDownCast(ds);
//        xw->SetInputData(pd);//const_cast<vtkDataSet*>();
//		xw->SetFileName(fileName.toStdString().c_str());
//		xw->Write();
//		//xw->Update();
//	}
//	else

    // until we've resolved the issue with the vtkXMLPolyDataWriter we
    // use the *.vtk ASCII format for its portability across platforms
	{
        NMDebugAI(<< "writing ASCII *.vtk file ..." << endl);
		vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
		writer->SetFileName(fileName.toStdString().c_str());
        writer->SetInputData(const_cast<vtkDataSet*>(l->getDataSet()));
        writer->SetFileTypeToASCII();
		writer->Update();
	}

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void LUMASSMainWin::import3DPointSet()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	     tr("Open PointDataSet (x,y,z)"), "~", tr("All Text Files (*.*)"));

	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    	return;

	// some vars we need
	vtkSmartPointer<vtkPolyData> pointCloud = vtkPolyData::New();
	vtkSmartPointer<vtkPoints> pts = vtkPoints::New();
	vtkSmartPointer<vtkCellArray> verts = vtkCellArray::New();

    QTextStream in(&file);

    // determine splitter (either ' ', ',', ';', '\t')
    QStringList splitChars;
    splitChars << " " << "," << ";" << "\t";
    QString splitter;
	QString line;
	QStringList entr;

	line = in.readLine();
    bool found = false;
	QStringList::iterator spcit = splitChars.begin();
	for (; spcit != splitChars.end(); spcit++)
	{
		entr = line.split(*spcit);
		if (entr.count() == 3)
		{
			splitter = *spcit;
			found = true;
			break;
		}
	}

	if (!found)
	{
		qDebug() << "Can't read content from: '" << fileName << "'! Check file format!";
		return;
	}


    double x, y, z;
    double lowPt[3] = {0.0, 0.0, 10000};
    double highPt[3] = {0.0, 0.0, -100000};
    bool bx, by, bz;
    int num=0;
    vtkIdType id;
    in.reset();
    while (!in.atEnd())
    {
    	// split the line into values
    	line = in.readLine();
    	entr = line.split(splitter);

        // skip incomplete data sets
        if (entr.size() < 3)
         continue;

    	x = entr.at(0).toDouble(&bx);
        y = entr.at(1).toDouble(&by);
        z = entr.at(2).toDouble(&bz);

        if (!bx || !by || !bz)
         continue;

        if (z < lowPt[2])
        {
        	lowPt[0] = x;
        	lowPt[1] = y;
        	lowPt[2] = z;
        }

        if (z > highPt[2])
        {
        	highPt[0] = x;
        	highPt[1] = y;
        	highPt[2] = z;
        }

        id = pts->InsertNextPoint(x, y, z);
        verts->InsertNextCell(1, &id);

        num++;
    }
    file.close();

	// setting up the PolyData
    pointCloud->SetPoints(pts);
    pointCloud->SetVerts(verts);

//    this->displayPolyData(pointCloud, lowPt, highPt);

    QFileInfo fi(file);

	vtkRenderWindow* renWin = this->ui->qvtkWidget->GetRenderWindow();
	NMVectorLayer* layer = new NMVectorLayer(renWin);
	layer->setObjectName(fi.baseName());
	layer->setDataSet(pointCloud);
	layer->setVisible(true);
    this->mLayerList->addLayer(layer);

}

/*
void LUMASSMainWin::displayPolyData(vtkSmartPointer<vtkPolyData> polydata, double* lowPt, double* highPt)
{
	NMDebugCtx(ctx, << "...");

	// give some feedback about what you're going to display
	vtkIndent ind(0);
    polydata->PrintSelf(std::cout, ind);

    // get the boundary of the polydata
    double vp[6];
    polydata->GetBounds(vp);

    vtkSmartPointer<vtkOGRLayerMapper> mapper = vtkSmartPointer<vtkOGRLayerMapper>::New();
	mapper->SetInput(polydata);

	vtkSmartPointer<vtkLookupTable> clrtab = vtkSmartPointer<vtkLookupTable>::New();
	clrtab->SetNumberOfTableValues(polydata->GetPolys()->GetNumberOfCells());
	clrtab->SetHueRange(0, 1);
	clrtab->SetSaturationRange(0.5, 1);
	clrtab->SetValueRange(0.5, 1);
	clrtab->Build();

	mapper->SetScalarRange(0, clrtab->GetNumberOfTableValues()-1);
	mapper->SetLookupTable(clrtab);

	vtkSmartPointer<vtkActor> polyActor = vtkSmartPointer<vtkActor>::New();
	polyActor->SetMapper(mapper);
	polyActor->SetPickable(1);

	// create a polydata layer for the outline of polys
	vtkSmartPointer<vtkPolyData> outline = vtkSmartPointer<vtkPolyData>::New();
	outline->SetPoints(polydata->GetPoints());
	outline->SetLines(polydata->GetPolys());

	vtkSmartPointer<vtkIntArray> outlineclr = vtkSmartPointer<vtkIntArray>::New();
	outlineclr->SetNumberOfValues(outline->GetNumberOfCells());
	for (int i=0; i < outline->GetNumberOfCells(); i++)
		outlineclr->SetValue(i, 0);
	outline->GetCellData()->SetScalars(outlineclr);

	vtkSmartPointer<vtkLookupTable> olclrtab = vtkSmartPointer<vtkLookupTable>::New();
	olclrtab->SetNumberOfTableValues(1);
	olclrtab->SetTableValue(0, 0, 0, 0, 1);


	vtkSmartPointer<vtkOGRLayerMapper> outlinemapper = vtkSmartPointer<vtkOGRLayerMapper>::New();
	outlinemapper->SetInput(outline);
	outlinemapper->SetLookupTable(olclrtab);

	vtkSmartPointer<vtkActor> outlineactor = vtkSmartPointer<vtkActor>::New();
	outlineactor->SetMapper(outlinemapper);
	outlineactor->SetPickable(1);

	this->m_renderer->AddActor(polyActor);
	this->m_renderer->AddActor(outlineactor);

    this->m_renderer->ResetCamera(vp);
    this->m_renderer->Render();
    this->ui->qvtkWidget->update();

    NMDebugAI( << "bounds: " << vp[0] << ", " << vp[1] << ", " << vp[2] << ", " << vp[3] << ", " <<
    		vp[4] << ", " << vp[5] << endl);

    NMDebugCtx(ctx, << "done!");

// ----------------------------------------------------------
// * 	some snipptes that might be useful later
// * ----------------------------------------------------------


//    vtkSmartPointer<vtkDataSetMapper> surfmapper = vtkDataSetMapper::New();

//	vtkSmartPointer<vtkElevationFilter> filter = vtkElevationFilter::New();
//    // apply the elevation filter
//    if (lowPt != 0 && highPt != 0)
//    {
//    	filter->SetInput(polydata);
//    	filter->SetLowPoint(lowPt);
//    	filter->SetHighPoint(highPt);
//    	mapper->SetInput(filter->GetPolyDataOutput());
//    }
//    else
//    	mapper->SetInput(polydata);

//    vtkSmartPointer<vtkDelaunay2D> delaunay = vtkDelaunay2D::New();
//    delaunay->SetInput(polydata);

//    surfmapper->SetInput(delaunay->GetOutput());


//	display also the points
//    vtkSmartPointer<vtkPolyDataMapper> ptsmapper = vtkPolyDataMapper::New();
//    ptsmapper->SetInput(polydata);
//
//    vtkSmartPointer<vtkActor> ptsactor = vtkActor::New();
//    ptsactor->SetMapper(ptsmapper);
//
//    vtkSmartPointer<vtkProperty> prop = vtkProperty::New();
//    prop->SetColor(1,0,0);
//    prop->SetPointSize(2);
//    ptsactor->SetProperty(prop);
//    this->m_renderer->AddViewProp(ptsactor);
}
*/


vtkSmartPointer<vtkPolyData> LUMASSMainWin::wkbPointToPolyData(OGRLayer& l)
{
	 /* OGC SimpleFeature Spec
	  *
	  * Point {double x; double y}
	  *
      *	WKBPoint {char byteOrder; static unsigned int wkbType = 1;
	  *		Point				point
	  * }
	  *
	  * WKBMultiPoint {char byteOrder; static unsigned int wkbType = 4;
	  * 	unsigned int 		numPoints;
	  * 	Point				points[numPoints]
	  * }
	  */

	return 0;
}


vtkSmartPointer<vtkPolyData> LUMASSMainWin::wkbLineStringToPolyData(OGRLayer& l)
{
	/*  OGC SimpleFeature Spec
	 *
	 * 	Point {double x; double y}
	 *
	 *  WKBLineString {char byteOrder; static unsigned int wkbType = 2;
	 *  	unsigned int		numPoints;
	 *  	Point 				points[numPoints]
	 *  }
	 *
	 *  WKBMultiLineString {char byteOrder; static unsigned int wkbType = 5;
	 *  	unsigned int 		numLineStrings;
	 *  	WKBLineString		lineStrings[numLineStrings]
	 *  }
	 *
	 */

	return 0;
}


vtkSmartPointer<vtkPolyData> LUMASSMainWin::wkbPolygonToPolyData(OGRLayer& l)
{
	/*	OGC SimpleFeature Spec
	 *
	 * 	Point {double x; double y}
	 * 	LinearRing {unsigned int numPoints; Point points[numPoints]}
	 *
	 *  WKBPolygon {char byteOrder; static unsigned int wkbType = 3;
	 *  	unsigned int		numRings;
	 *		LinearRing			rings[numRings]
	 *	}
	 *
	 *	WKBMultiPolygon {char byteOrder; static unsigned int wkbType = 6;
	 *		unsigned int		numPolygons;
	 *		WKBPolygon			polygons[numPolygons]
	 *	}
	 *
     * DEPRECATED
	 *	To allow for proper polygon display (i.e. holes / donuts, non-convex polygons),
	 *	the herewith created vtkPolyData should be used in conjunction with the
	 *	vtkOGRLayerMapper class. vtkOGRLayerMapper uses GLU tesselation to display
	 *	the above mentioned features. For this to work, the following vertex order
	 *	is required to identify features
	 *		- exterior ring: counter-clockwise
	 *		- interior rings: clockwise
	 *
	 *	Unfortunately, the OGR exportToWKB function uses the opposite ordering of rings. So here
	 *	all exterior rings are traversed from the last point to the first one to adhere
	 *	with vtk requirements
	 */

        NMDebugCtx(ctxLUMASSMainWin, << "...");

	// create vtk data objects
	vtkSmartPointer<vtkPolyData> vtkVect = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkMergePoints> mpts = vtkSmartPointer<vtkMergePoints>::New();

	// set divisions --> no of division in x-y-z-direction (i.e. range)
	// set bounds
	OGREnvelope ogrext;
    OGRErr oge = l.GetExtent(&ogrext);
	double fbnds[6] = {ogrext.MinX-10, ogrext.MaxX+10,
					   ogrext.MinY-10, ogrext.MaxY+10, 0, 0};
	mpts->SetDivisions(10, 10, 1);
	mpts->SetTolerance(1e-2);
	mpts->InitPointInsertion(points, fbnds);

	vtkIdType pointId, cellId;
	double dMax = std::numeric_limits<double>::max();
	double dMin = std::numeric_limits<double>::max() * -1;
	double bnd[6] = {dMax, dMin, dMax, dMin, 0, 0};
	int featcount = l.GetFeatureCount(1);
	NMDebugAI(<< "number of features: " << featcount << endl);

	vtkVect->Allocate(featcount, 100);

	// OGC SimpleFeature WellKnownBinary (WKB) byte order enum:
	// wkbXDR = 0 (big endian) | wkbNDR = 1 (little endian)
	OGRwkbByteOrder bo = QSysInfo::ByteOrder == QSysInfo::BigEndian ? wkbXDR : wkbNDR;
	OGRFeature *pFeat;

	// ----------------------------------------------------------------------------------
	// create the attributes arrays for this layer
	l.ResetReading();
	pFeat = l.GetNextFeature();
	int nfields = pFeat->GetFieldCount();

	std::vector<vtkSmartPointer<vtkAbstractArray> > attr;
    // keeps track of the features to copy
    // we filter all "nm_*" by default
    std::vector<int> fieldIdx;

	vtkSmartPointer<vtkLongArray> nm_id = vtkSmartPointer<vtkLongArray>::New();
	nm_id->SetName("nm_id");
	nm_id->Allocate(l.GetFeatureCount(1), 100);

	vtkSmartPointer<vtkUnsignedCharArray> nm_hole = vtkSmartPointer<vtkUnsignedCharArray>::New();
	nm_hole->SetName("nm_hole");
	nm_hole->Allocate(l.GetFeatureCount(1), 100);

	vtkSmartPointer<vtkUnsignedCharArray> nm_sel = vtkSmartPointer<vtkUnsignedCharArray>::New();
	nm_sel->SetName("nm_sel");
	nm_sel->Allocate(l.GetFeatureCount(1), 100);

	vtkSmartPointer<vtkIntArray> iarr;
	vtkSmartPointer<vtkDoubleArray> darr;
	vtkSmartPointer<vtkStringArray> sarr;

	for (int f=0; f < nfields; ++f)
	{
		OGRFieldDefn* fdef = pFeat->GetFieldDefnRef(f);
        //		NMDebugAI( << fdef->GetNameRef() << ": " << fdef->GetFieldTypeName(fdef->GetType()) << endl);
        if (::strcmp(fdef->GetNameRef(), "nm_id") == 0  ||
            ::strcmp(fdef->GetNameRef(), "nm_hole") == 0 ||
            ::strcmp(fdef->GetNameRef(), "nm_sel") == 0)
        {
            continue;
        }
        fieldIdx.push_back(f);

		switch(fdef->GetType())
		{
			case OFTInteger:
				iarr = vtkSmartPointer<vtkIntArray>::New();
				iarr->SetName(fdef->GetNameRef());
				iarr->Allocate(featcount, 100);
				attr.push_back(iarr);
				break;
			case OFTReal:
				darr = vtkSmartPointer<vtkDoubleArray>::New();
				darr->SetName(fdef->GetNameRef());
				darr->Allocate(featcount, 100);
				attr.push_back(darr);
				break;
			default: // case OFTString:
				sarr = vtkSmartPointer<vtkStringArray>::New();
				sarr->SetName(fdef->GetNameRef());
				sarr->Allocate(featcount, 100);
				attr.push_back(sarr);
				break;
		}
	}

	// ------------------------------------------------------------------------------------------------
	// process feature by feature
	l.ResetReading();
	//NMDebugAI(<< "importing features ... " << std::endl);
	unsigned int featCounter = 1;
	while ((pFeat = l.GetNextFeature()) != NULL)
	{
		OGRGeometry *geom = pFeat->GetGeometryRef();
		if (geom == 0)
		{
                        NMWarn(ctxLUMASSMainWin, << "Oops - got NULL geometry for this feature! - Abort.");
			continue;
		}
		int fid = pFeat->GetFieldAsInteger(0);

		int wkbSize = geom->WkbSize();
//		NMDebug(<< endl << endl << "feature #" << featCounter << ":" << fid << " (" << geom->getGeometryName() <<
//				", " << wkbSize << " bytes)" << endl);
		unsigned char* wkb = new unsigned char[wkbSize];
		if (wkb == 0)
		{
                        NMLogError(<< ctxLUMASSMainWin << ": not enough memory to allocate feature wkb buffer!");
			return 0;
		}
		geom->exportToWkb(bo, wkb);

		// multi or not ?
		unsigned int pos = sizeof(char); // we start with the type, 'cause we know the byte order already
		unsigned int type;
		memcpy(&type, (wkb+pos), sizeof(unsigned int));

		// jump over type (= 4 byte)
		pos += sizeof(unsigned int);

		//NMDebugAI( << "feature #" << featCounter << " is of geometry type " << type << endl);

		unsigned int nPolys;
		unsigned int nRings;

		if (type == 6)
		{
			memcpy(&nPolys, (wkb+pos), sizeof(unsigned int));
			// jump over the nPolys (= 4 byte),
			// skip byte order for 1st polygon (= 1 byte),
			// skip geometry type for 1st polygon (= 4 byte);
			pos += sizeof(unsigned int) + sizeof(char) + sizeof(unsigned int);
		}
		else
			nPolys = 1;

//		NMDebug(<< endl << "number of polygons: " << nPolys << endl);

		unsigned int nPoints;
		double x, y;

		// pos should point to the number of rings of the 1st polygon
		for (unsigned int p=0; p < nPolys; ++p) // -----------------------------------------
		{
			// get the number of rings for this polygon
			memcpy(&nRings, (wkb+pos), sizeof(unsigned int));

//			NMDebug(<< "polygon #" << p+1 << " - " << nRings << " rings ... " << endl);

			// jump over nRings (= 4 byte)
			pos += sizeof(unsigned int);

			// process rings
			for (unsigned int r=0; r < nRings; ++r) // ......................................
			{
				// get the number of points for this ring
				memcpy(&nPoints, (wkb+pos), sizeof(unsigned int));

//				NMDebug(<< "ring #" << r+1 << " - " << nPoints << " points ..." << endl);

				// jump over nPoints (= 4 byte)
				pos += sizeof(unsigned int);

				// insert next cell and assign cellId as preliminary attribute
				cellId = polys->InsertNextCell(nPoints);

				// assign feature id and cell values
				nm_sel->InsertNextValue(0);
				if (r == 0)
				{
					nm_id->InsertNextValue(featCounter);
					nm_hole->InsertNextValue(0);
				}
				else
				{
					nm_id->InsertNextValue(-1);
					nm_hole->InsertNextValue(1);
				}

				vtkIntArray* iar;
				vtkDoubleArray* dar;
				vtkStringArray* sar;
                for (int fidx=0; fidx < fieldIdx.size(); ++fidx)
				{
                    switch(pFeat->GetFieldDefnRef(fieldIdx[fidx])->GetType())
					{
					case OFTInteger:
                        iar = vtkIntArray::SafeDownCast(attr[fidx]);
                        iar->InsertNextValue(pFeat->GetFieldAsInteger(fieldIdx[fidx]));
						break;
					case OFTReal:
                        dar = vtkDoubleArray::SafeDownCast(attr[fidx]);
                        dar->InsertNextValue(pFeat->GetFieldAsDouble(fieldIdx[fidx]));
						break;
					default:
                        sar = vtkStringArray::SafeDownCast(attr[fidx]);
                        sar->InsertNextValue(pFeat->GetFieldAsString(fieldIdx[fidx]));
						break;
					}
				}

				/* We traverse all points backwards, since the vertex order given by
				 * exportToWKB is opposite to the VTK required one (and the
				 * OGC simple feature spec).
				 *
				  0   1   2   3   4 ... nPoints-1
				 >x y x y x y x y x y   x y
				 --> advancing the buffer pointer (pos) nPoints-1 times by
					 16 bytes moves it right in front of the x coordinate
					 of the last point, adding another 8 bytes, in front
					 of the y coordinate of the last point; then we are
					 going backwards through the buffer and read
					 the coordinates;
				*/

				unsigned int rpos = pos + ((nPoints-1) * 2 * sizeof(double)) + sizeof(double);
				pos = rpos + sizeof(double);

				for (unsigned int pnt=0; pnt < nPoints; ++pnt) // ............................
				{
					// get the coordinates and jump over it
					// i.e. forward 8 byte each time (double!)
					memcpy(&y, (wkb+rpos), sizeof(double)); rpos -= sizeof(double);
					memcpy(&x, (wkb+rpos), sizeof(double)); rpos -= sizeof(double);

					double pt[3];
					vtkIdType tmpId;

//					NMDebug(<< pnt << "(" << x << "," << y << ")" << endl);
					pt[0] = x;
					pt[1] = y;
					pt[2] = 0.0;

                    //mpts->InsertUniquePoint(pt, tmpId);
                    tmpId = points->InsertNextPoint(pt);
					polys->InsertCellPoint(tmpId);
				}
//				NMDebug(<< endl);
			}
			//skip byte order for next polygon (= 1 byte)
			//skip geom type for next polygon (= 4 byte)
			pos += sizeof(char) + sizeof(unsigned int);
		}

		// increase feature counter
		//NMDebug(<< featCounter << " ");
		++featCounter;

		// release memory for the wkb buffer
		delete[] wkb;
	}
	//NMDebug(<< std::endl);

	// add geometry (i.e. points and cells)
	vtkVect->SetPoints(mpts->GetPoints());
	vtkVect->SetPolys(polys);

	// add attributes
	vtkVect->GetCellData()->SetScalars(nm_id);
	vtkVect->GetCellData()->AddArray(nm_hole);
	vtkVect->GetCellData()->AddArray(nm_sel);
    for (int f=0; f < fieldIdx.size(); ++f)
		vtkVect->GetCellData()->AddArray(attr[f]);

	vtkVect->BuildCells();
    vtkVect->BuildLinks();

	NMDebugAI(<< featCounter << " features imported" << endl);

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
	return vtkVect;
}

void
LUMASSMainWin::importShapeFile(const QString &filename)
{

}

// experimental
vtkSmartPointer<vtkPolyData> LUMASSMainWin::wkbPolygonToTesselatedPolyData(OGRLayer& l)
{
    /*	OGC SimpleFeature Spec
     *
     * 	Point {double x; double y}
     * 	LinearRing {unsigned int numPoints; Point points[numPoints]}
     *
     *  WKBPolygon {char byteOrder; static unsigned int wkbType = 3;
     *  	unsigned int		numRings;
     *		LinearRing			rings[numRings]
     *	}
     *
     *	WKBMultiPolygon {char byteOrder; static unsigned int wkbType = 6;
     *		unsigned int		numPolygons;
     *		WKBPolygon			polygons[numPolygons]
     *	}
     *
     * DEPRECATED
     *	To allow for proper polygon display (i.e. holes / donuts, non-convex polygons),
     *	the herewith created vtkPolyData should be used in conjunction with the
     *	vtkOGRLayerMapper class. vtkOGRLayerMapper uses GLU tesselation to display
     *	the above mentioned features. For this to work, the following vertex order
     *	is required to identify features
     *		- exterior ring: counter-clockwise
     *		- interior rings: clockwise
     *
     *	Unfortunately, the OGR exportToWKB function uses the opposite ordering of rings. So here
     *	all exterior rings are traversed from the last point to the first one to adhere
     *	with vtk requirements
     */

        NMDebugCtx(ctxLUMASSMainWin, << "...");

    // create vtk data objects
    vtkSmartPointer<vtkPolyData> vtkVect = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkMergePoints> mpts = vtkSmartPointer<vtkMergePoints>::New();

    // set divisions --> no of division in x-y-z-direction (i.e. range)
    // set bounds
    OGREnvelope ogrext;
    OGRErr oge = l.GetExtent(&ogrext);
    double fbnds[6] = {ogrext.MinX-10, ogrext.MaxX+10,
                       ogrext.MinY-10, ogrext.MaxY+10, 0, 0};
//    mpts->SetDivisions(10, 10, 1);
//    mpts->SetTolerance(1e-2);
//    mpts->InitPointInsertion(points, fbnds);

    vtkIdType pointId, cellId;
    double dMax = std::numeric_limits<double>::max();
    double dMin = std::numeric_limits<double>::max() * -1;
    double bnd[6] = {dMax, dMin, dMax, dMin, 0, 0};
    int featcount = l.GetFeatureCount(1);
    NMDebugAI(<< "number of features: " << featcount << endl);

    vtkVect->Allocate(featcount, 100);
    // this is just a guess; may need some fine tuning in the end
    points->Allocate(featcount*3, 100);
    vtkVect->SetPoints(points);

    // OGC SimpleFeature WellKnownBinary (WKB) byte order enum:
    // wkbXDR = 0 (big endian) | wkbNDR = 1 (little endian)
    OGRwkbByteOrder bo = QSysInfo::ByteOrder == QSysInfo::BigEndian ? wkbXDR : wkbNDR;
    OGRFeature *pFeat;

    // ----------------------------------------------------------------------------------
    // create the attributes arrays for this layer
    l.ResetReading();
    pFeat = l.GetNextFeature();
    int nfields = pFeat->GetFieldCount();

    std::vector<vtkSmartPointer<vtkAbstractArray> > attr;
    // keeps track of the features to copy
    // we filter all "nm_*" by default
    std::vector<int> fieldIdx;

    unsigned int ent = l.GetFeatureCount(0);

    vtkSmartPointer<vtkLongArray> nm_id = vtkSmartPointer<vtkLongArray>::New();
    nm_id->SetName("nm_id");
    nm_id->Allocate(ent, 100);

    vtkSmartPointer<vtkUnsignedCharArray> nm_hole = vtkSmartPointer<vtkUnsignedCharArray>::New();
    nm_hole->SetName("nm_hole");
    nm_hole->Allocate(ent, 100);

    vtkSmartPointer<vtkUnsignedLongArray> nm_triid = vtkSmartPointer<vtkUnsignedLongArray>::New();
    nm_triid->SetName("nm_triid");
    nm_triid->Allocate(ent, 100);

    vtkSmartPointer<vtkUnsignedCharArray> nm_sel = vtkSmartPointer<vtkUnsignedCharArray>::New();
    nm_sel->SetName("nm_sel");
    nm_sel->Allocate(ent, 100);

    vtkSmartPointer<vtkIntArray> iarr;
    vtkSmartPointer<vtkDoubleArray> darr;
    vtkSmartPointer<vtkStringArray> sarr;

//    for (int f=0; f < nfields; ++f)
//    {
//        OGRFieldDefn* fdef = pFeat->GetFieldDefnRef(f);
//        //		NMDebugAI( << fdef->GetNameRef() << ": " << fdef->GetFieldTypeName(fdef->GetType()) << endl);
//        if (::strcmp(fdef->GetNameRef(), "nm_id") == 0  ||
//            ::strcmp(fdef->GetNameRef(), "nm_hole") == 0 ||
//            ::strcmp(fdef->GetNameRef(), "nm_sel") == 0)
//        {
//            continue;
//        }
//        fieldIdx.push_back(f);

//        switch(fdef->GetType())
//        {
//            case OFTInteger:
//                iarr = vtkSmartPointer<vtkIntArray>::New();
//                iarr->SetName(fdef->GetNameRef());
//                iarr->SetName("TriID");
//                iarr->Allocate(featcount, 100);
//                attr.push_back(iarr);
//                break;
//            case OFTReal:
//                darr = vtkSmartPointer<vtkDoubleArray>::New();
//                darr->SetName(fdef->GetNameRef());
//                darr->Allocate(featcount, 100);
//                attr.push_back(darr);
//                break;
//            default: // case OFTString:
//                sarr = vtkSmartPointer<vtkStringArray>::New();
//                sarr->SetName(fdef->GetNameRef());
//                sarr->Allocate(featcount, 100);
//                attr.push_back(sarr);
//                break;
//        }
//    }

    // ------------------------------------------------------------------------------------------------
    // process feature by feature
    l.ResetReading();
    //NMDebugAI(<< "importing features ... " << std::endl);
    unsigned int featCounter = 1;
    unsigned int trisCounter = 0;
    while ((pFeat = l.GetNextFeature()) != NULL)
    {
        OGRGeometry *geom = pFeat->GetGeometryRef();
        if (geom == 0)
        {
                        NMWarn(ctxLUMASSMainWin, << "Oops - got NULL geometry for this feature! - Abort.");
            continue;
        }

        avtPolygonToTrianglesTesselator tess(points);
        tess.SetNormal(0.0, 0.0, 1.0);

//        int fid = pFeat->GetFieldAsInteger(0);

        int wkbSize = geom->WkbSize();
//		NMDebug(<< endl << endl << "feature #" << featCounter << ":" << fid << " (" << geom->getGeometryName() <<
//				", " << wkbSize << " bytes)" << endl);
        unsigned char* wkb = new unsigned char[wkbSize];
        if (wkb == 0)
        {
                        NMLogError(<< ctxLUMASSMainWin << ": not enough memory to allocate feature wkb buffer!");
            return 0;
        }
        geom->exportToWkb(bo, wkb);

        // multi or not ?
        unsigned int pos = sizeof(char); // we start with the type, 'cause we know the byte order already
        unsigned int type;
        memcpy(&type, (wkb+pos), sizeof(unsigned int));

        // jump over type (= 4 byte)
        pos += sizeof(unsigned int);

        //NMDebugAI( << "feature #" << featCounter << " is of geometry type " << type << endl);

        unsigned int nPolys;
        unsigned int nRings;

        if (type == 6)
        {
            memcpy(&nPolys, (wkb+pos), sizeof(unsigned int));
            // jump over the nPolys (= 4 byte),
            // skip byte order for 1st polygon (= 1 byte),
            // skip geometry type for 1st polygon (= 4 byte);
            pos += sizeof(unsigned int) + sizeof(char) + sizeof(unsigned int);
        }
        else
            nPolys = 1;

//		NMDebug(<< endl << "number of polygons: " << nPolys << endl);

        unsigned int nPoints;
        double x, y;

        // pos should point to the number of rings of the 1st polygon
        for (unsigned int p=0; p < nPolys; ++p) // -----------------------------------------
        {
            // get the number of rings for this polygon
            memcpy(&nRings, (wkb+pos), sizeof(unsigned int));

//			NMDebug(<< "polygon #" << p+1 << " - " << nRings << " rings ... " << endl);

            // jump over nRings (= 4 byte)
            pos += sizeof(unsigned int);

            // process rings
            for (unsigned int r=0; r < nRings; ++r) // ......................................
            {
                // get the number of points for this ring
                memcpy(&nPoints, (wkb+pos), sizeof(unsigned int));

//				NMDebug(<< "ring #" << r+1 << " - " << nPoints << " points ..." << endl);

                // jump over nPoints (= 4 byte)
                pos += sizeof(unsigned int);

                // insert next cell and assign cellId as preliminary attribute
                //cellId = polys->InsertNextCell(nPoints);

                // assign feature id and cell values
                //                nm_sel->InsertNextValue(0);
                //                if (r == 0)
                //                {
                //                    nm_id->InsertNextValue(featCounter);
                //                    nm_hole->InsertNextValue(0);
                //                }
                //                else
                //                {
                //                    nm_id->InsertNextValue(-1);
                //                    nm_hole->InsertNextValue(1);
                //                }

//                vtkIntArray* iar;
//                vtkDoubleArray* dar;
//                vtkStringArray* sar;
//                for (int fidx=0; fidx < fieldIdx.size(); ++fidx)
//                {
//                    switch(pFeat->GetFieldDefnRef(fieldIdx[fidx])->GetType())
//                    {
//                    case OFTInteger:
//                        iar = vtkIntArray::SafeDownCast(attr[fidx]);
//                        iar->InsertNextValue(pFeat->GetFieldAsInteger(fieldIdx[fidx]));
//                        break;
//                    case OFTReal:
//                        dar = vtkDoubleArray::SafeDownCast(attr[fidx]);
//                        dar->InsertNextValue(pFeat->GetFieldAsDouble(fieldIdx[fidx]));
//                        break;
//                    default:
//                        sar = vtkStringArray::SafeDownCast(attr[fidx]);
//                        sar->InsertNextValue(pFeat->GetFieldAsString(fieldIdx[fidx]));
//                        break;
//                    }
//                }

                /* We traverse all points backwards, since the vertex order given by
                 * exportToWKB is opposite to the VTK required one (and the
                 * OGC simple feature spec).
                 *
                  0   1   2   3   4 ... nPoints-1
                 >x y x y x y x y x y   x y
                 --> advancing the buffer pointer (pos) nPoints-1 times by
                     16 bytes moves it right in front of the x coordinate
                     of the last point, adding another 8 bytes, in front
                     of the y coordinate of the last point; then we are
                     going backwards through the buffer and read
                     the coordinates;
                */

                tess.BeginContour();

                unsigned int rpos = pos + ((nPoints-1) * 2 * sizeof(double)) + sizeof(double);
                pos = rpos + sizeof(double);

                for (unsigned int pnt=0; pnt < nPoints; ++pnt) // ............................
                {
                    // get the coordinates and jump over it
                    // i.e. forward 8 byte each time (double!)
                    memcpy(&y, (wkb+rpos), sizeof(double)); rpos -= sizeof(double);
                    memcpy(&x, (wkb+rpos), sizeof(double)); rpos -= sizeof(double);

                    double pt[3];
                    vtkIdType tmpId;

//					NMDebug(<< pnt << "(" << x << "," << y << ")" << endl);
                    pt[0] = x;
                    pt[1] = y;
                    pt[2] = 0.0;

                    //mpts->InsertUniquePoint(pt, tmpId);
                    //tmpId = points->InsertNextPoint(pt);
                    // polys->InsertCellPoint(tmpId);

                    tess.AddContourVertex(x, y, 0.0);
                }
//				NMDebug(<< endl);

                tess.EndContour();
            }
            //skip byte order for next polygon (= 1 byte)
            //skip geom type for next polygon (= 4 byte)
            pos += sizeof(char) + sizeof(unsigned int);

            int ntris = tess.Tessellate(vtkVect);
            for (int t=0; t < ntris; ++t)
            {
                nm_id->InsertNextValue(featCounter);
                nm_hole->InsertNextValue(0);
                nm_sel->InsertNextValue(0);
                nm_triid->InsertNextValue(trisCounter+t+1);
            }
            trisCounter += ntris;
        }

        // increase feature counter
        //NMDebug(<< featCounter << " ");
        ++featCounter;

        // release memory for the wkb buffer
        delete[] wkb;
    }
    //NMDebug(<< std::endl);

    // add geometry (i.e. points and cells)
//    vtkVect->SetPoints(mpts->GetPoints());
//    vtkVect->SetPolys(polys);

    // add attributes
    nm_id->Squeeze();
    nm_hole->Squeeze();
    nm_sel->Squeeze();
    nm_triid->Squeeze();
    vtkVect->Squeeze();
    vtkVect->GetCellData()->SetScalars(nm_id);
    vtkVect->GetCellData()->AddArray(nm_hole);
    vtkVect->GetCellData()->AddArray(nm_sel);
    vtkVect->GetCellData()->AddArray(nm_triid);

//    for (int f=0; f < fieldIdx.size(); ++f)
//        vtkVect->GetCellData()->AddArray(attr[f]);

//    vtkVect->BuildCells();
//    vtkVect->BuildLinks();

    NMDebugAI(<< featCounter << " features imported" << endl);

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
    return vtkVect;
}

void
LUMASSMainWin::vtkPolygonPolydataToOGR(
#ifndef GDAL_200
		OGRDataSource* ds,
#else
		GDALDataset *ds, 
#endif		
		NMVectorLayer* vectorLayer)
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

    // create the output layer
    NMDebugAI(<< "Create output layer ..." << std::endl);
    OGRLayer* ogrLayer = ds->CreateLayer(vectorLayer->objectName().toStdString().c_str(),
                                         0, wkbPolygon, 0);
    if (ogrLayer == 0)
    {
        NMDebugAI(<< "Failed creating the OGR polygon layer!" << std::endl);
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    // make a list of available attributes
    vtkDataSet* vtkDS = const_cast<vtkDataSet*>(vectorLayer->getDataSet());
    vtkDataSetAttributes* dsAttr = vtkDS->GetAttributes(vtkDataSet::CELL);

    // check, whether we've got some info about donut polygons ...
    vtkDataArray* hole = dsAttr->GetArray("nm_hole");
    if (hole == 0)
    {
        NMDebugAI(<< "Failed creating the OGR polygon layer!" << std::endl);
        NMLogError(<< ctxLUMASSMainWin << ": Lacking info on donut polygons - bailing out!");
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

    //=================================================================================
    // copying attribute table structure
    //=================================================================================

    QStringList origFieldNames;
    QStringList writeFieldNames;

    NMDebugAI(<< "Add attributes to output layer ..." << std::endl);
    int numFields = dsAttr->GetNumberOfArrays();
    for (int f=0; f < numFields; ++f)
    {
        vtkAbstractArray* aa = dsAttr->GetAbstractArray(f);
        QString fieldName = aa->GetName();
        origFieldNames << fieldName;
        if (fieldName.length() > 10)
        {
            bool bok;
            QString newName = QInputDialog::getText(this, "Confirm Field Name",
                                                    "Please enter field name with max. 10 characters.",
                                                    QLineEdit::Normal, fieldName, &bok);
            if (!bok)
            {
                NMDebugAI(<< "No valid new field name entered! Field name gets mangled!" << std::endl);
                writeFieldNames << fieldName.left(10);
            }
            else
            {
                if (newName.length() <= 10)
                {
                    writeFieldNames << newName;
                }
                else
                {
                    NMDebugAI(<< "No valid new field name entered! Field name gets mangled!" << std::endl);
                    writeFieldNames << fieldName.left(10);
                }
            }
        }
        else
        {
            writeFieldNames << fieldName;
        }


        if (aa->IsNumeric())
        {
            if (    aa->GetDataType() == VTK_FLOAT
                 || aa->GetDataType() == VTK_DOUBLE
               )
            {
                OGRFieldDefn field(writeFieldNames.at(f).toStdString().c_str(), OFTReal);
                if (ogrLayer->CreateField(&field) != OGRERR_NONE)
                {
                    NMDebugAI(<< "Failed crating double field '" << aa->GetName() << "'!"
                              << std::endl);
                }

            }
            else
            {
                OGRFieldDefn field(writeFieldNames.at(f).toStdString().c_str(), OFTInteger);
                if (ogrLayer->CreateField(&field) != OGRERR_NONE)
                {
                    NMDebugAI(<< "Failed crating integer field '" << aa->GetName() << "'!"
                              << std::endl);
                }
            }
        }
        else
        {
            OGRFieldDefn field(writeFieldNames.at(f).toStdString().c_str(), OFTString);
            if (ogrLayer->CreateField(&field) != OGRERR_NONE)
            {
                NMDebugAI(<< "Failed crating string field '" << aa->GetName() << "'!"
                          << std::endl);
                continue;
            }
        }
    }

    //=================================================================================
    // copying the geometries and fill the attribute table
    //=================================================================================

    NMDebugAI(<< "Copy data feature by feature ... " << std::endl);
    // now we're looping over the polygons, define OGRFeatures, and
    // write them out
    vtkPolyData* pd = vtkPolyData::SafeDownCast(vtkDS);
    vtkIdType ncells = pd->GetNumberOfCells();
    vtkPoints* pts = pd->GetPoints();
    vtkCellArray* ca = pd->GetPolys();
    ca->InitTraversal();

    vtkIdType npts;
    vtkIdType* cpts;
    double* xyz;
    int holecell = -1;
    for (int cell=0; cell < ncells; ++cell)
    {
        OGRFeature* feat = OGRFeature::CreateFeature(ogrLayer->GetLayerDefn());

        // ===============================================================
        // create the geometry
        if (ca->GetNextCell(npts, cpts) == 0)
        {
            break;
        }

        OGRPolygon poly;
        OGRLinearRing outerRing;
        //for (int n=0; n < npts; ++n)
        for (int n=npts-1; n >= 0; --n)
        {
            xyz = pts->GetPoint(cpts[n]);
            outerRing.addPoint(xyz[0], xyz[1], xyz[2]);
        }

        poly.addRing(&outerRing);

        holecell = cell;
        while (holecell < ncells-1)
        {
            if (hole->GetTuple1(holecell+1))
            {
                if (ca->GetNextCell(npts, cpts) == 0)
                {
                    break;
                }
                OGRLinearRing innerRing;
                //for (int n=0; n < npts; ++n)
                for (int n=npts-1; n >= 0; --n)
                {
                    xyz = pts->GetPoint(cpts[n]);
                    innerRing.addPoint(xyz[0], xyz[1], xyz[2]);
                }
                poly.addRing(&innerRing);

                ++holecell;
            }
            else
            {
                break;
            }
        }

        if (feat->SetGeometry(&poly) != OGRERR_NONE)
        {
            NMDebugAI(<< "Failed adding polygon #" << cell << "!" << std::endl);
            break;
        }

        // ================================================================
        // add the attributes to the layer
        for (int f=0; f < numFields; ++f)
        {
            vtkAbstractArray* aa = dsAttr->GetAbstractArray(f);
            QString fieldName = writeFieldNames.at(f);
            if (aa->IsNumeric())
            {
                vtkDataArray* da = vtkDataArray::SafeDownCast(aa);
                if (    aa->GetDataType() == VTK_FLOAT
                     || aa->GetDataType() == VTK_DOUBLE
                   )
                {
                    feat->SetField(fieldName.toStdString().c_str(), static_cast<double>(da->GetTuple1(cell)));
                }
                else
                {
                    feat->SetField(fieldName.toStdString().c_str(), static_cast<int>(da->GetTuple1(cell)));
                }
            }
            else
            {
                vtkStringArray* sa = vtkStringArray::SafeDownCast(aa);
                feat->SetField(fieldName.toStdString().c_str(), sa->GetValue(cell).c_str());
            }
        }
        // re-align cell with holecell
        cell = holecell;

        // ================================================================
        // finally, we add the local feature to the data set feature
        if (ogrLayer->CreateFeature(feat) != OGRERR_NONE)
        {
            NMDebugAI(<< "Failed crating feature #" << cell << "!" << std::endl);
        }

        OGRFeature::DestroyFeature(feat);

        if (cell % 500 == 0)
        {
            NMDebug( << ".");
        }
    }
    NMDebug(<< std::endl);

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

vtkSmartPointer<vtkPolyData> 
LUMASSMainWin::OgrToVtkPolyData(
#ifndef GDAL_200
	OGRDataSource* pDS
#else
	GDALDataset* pDS
#endif
)
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

	// working just on the first layer right now
    OGRLayer *pLayer = pDS->GetLayer(0);
		NMDebugAI( << "the layer contains " << pLayer->GetFeatureCount(1)
			<< " features" << endl);
		NMDebugAI( << "geometry type: "
			<< pLayer->GetLayerDefn()->GetGeomType() << endl);

	// get an idea of the type of geometry we are dealing with
	// and call the appropriate internal conversion function
	OGRwkbGeometryType geomType = pLayer->GetLayerDefn()->GetGeomType();

	vtkSmartPointer<vtkPolyData> vtkVect;
	switch (geomType)
	{
	case wkbPoint:
	case wkbMultiPoint:
		vtkVect = this->wkbPointToPolyData(*pLayer);
		break;
	case wkbLineString:
	case wkbMultiLineString:
		vtkVect = this->wkbLineStringToPolyData(*pLayer);
		break;
	case wkbPolygon:
	case wkbMultiPolygon:
        vtkVect = this->wkbPolygonToPolyData(*pLayer);
        //vtkVect = this->wkbPolygonToTesselatedPolyData(*pLayer);
		break;
	default:
                NMLogError(<< ctxLUMASSMainWin << ": Geometry type '" << geomType << "' is currently not supported!");
		vtkVect = NULL;
	}

        NMDebugCtx(ctxLUMASSMainWin, << "done!");

	return vtkVect;
}

void LUMASSMainWin::loadVectorLayer()
{
        NMDebugCtx(ctxLUMASSMainWin, << "...");

	QString fileName = QFileDialog::getOpenFileName(this,
	     tr("Import OGR Vector File"), "~", tr("OGR supported files (*.*)"));
	if (fileName.isNull()) return;

	NMDebugAI( << "opening '" << fileName.toStdString() << "' ..." << endl);

#ifndef GDAL_200
    OGRRegisterAll();
    OGRDataSource *pDS = OGRSFDriverRegistrar::Open(fileName.toStdString().c_str(),
            FALSE, NULL);
    if (pDS == NULL)
    {
                NMLogError(<< ctxLUMASSMainWin << ": failed to open '" << fileName.toStdString() << "'!");
        return;
    }

    vtkSmartPointer<vtkPolyData> vtkVec = this->OgrToVtkPolyData(pDS);
    OGRDataSource::DestroyDataSource(pDS);
#else

    GDALAllRegister();
    GDALDataset *pDS = (GDALDataset*)GDALOpenEx(fileName.toStdString().c_str(),
            GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (pDS == NULL)
    {
        NMLogError(<< ctxLUMASSMainWin << ": failed to open '" << fileName.toStdString() << "'!");
        return;
    }

    vtkSmartPointer<vtkPolyData> vtkVec = this->OgrToVtkPolyData(pDS);
    GDALClose(pDS);
#endif

	QFileInfo finfo(fileName);
	QString layerName = finfo.baseName();

    vtkRenderWindow* renWin = this->ui->qvtkWidget->GetRenderWindow();
    //vtkSmartPointer<NMVtkOpenGLRenderWindow> renWin = vtkSmartPointer<NMVtkOpenGLRenderWindow>::New();
    //this->ui->qvtkWidget->SetRenderWindow(renWin);
	NMVectorLayer* layer = new NMVectorLayer(renWin);
	layer->setObjectName(layerName);
	layer->setDataSet(vtkVec);
	layer->setVisible(true);
    this->mLayerList->addLayer(layer);

        NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void
LUMASSMainWin::connectImageLayerProcSignals(NMLayer* layer)
{
	connect(layer, SIGNAL(layerProcessingStart()), this, SLOT(showBusyStart()));
	connect(layer, SIGNAL(layerProcessingEnd()), this, SLOT(showBusyEnd()));
	connect(layer, SIGNAL(layerLoaded()), this, SLOT(addLayerToCompList()));
}

#ifdef BUILD_RASSUPPORT
void
LUMASSMainWin::loadRasdamanLayer()
{
	// if we haven't got a connection, we shouldn't
	// try browsing the metadata
	if (this->getRasdamanConnector() == 0)
		return;

	this->updateRasMetaView();
    if (this->mpPetaView != 0)
		this->mpPetaView->show();
	else
	{
		NMBoxInfo("Failed initialising rasdaman metadata browser!",
				 "Check whether Postgres is up and running and access is "
				"is configured properly!");
		return;
	}
}

void
LUMASSMainWin::fetchRasLayer(const QString& imagespec,
		const QString& covname)
{
//	try
//	{
		NMDebugAI( << "opening " << imagespec.toStdString() << " ..." << std::endl);

		vtkRenderWindow* renWin = this->ui->qvtkWidget->GetRenderWindow();
		NMImageLayer* layer = new NMImageLayer(renWin);

		RasdamanConnector* rasconn = this->getRasdamanConnector();
		if (rasconn == 0)
		{
                        NMLogError(<< ctxLUMASSMainWin << ": Connection with rasdaman failed!");
			return;
		}

		layer->setRasdamanConnector(rasconn);
		layer->setObjectName(covname);
		this->connectImageLayerProcSignals(layer);

		QtConcurrent::run(layer, &NMImageLayer::setFileName, imagespec);

		//if (layer->setFileName(imagespec))
		//{
		//	layer->setVisible(true);
        //	this->mLayerList->addLayer(layer);
		//}
		//else
		//	delete layer;
//	}
//	catch(r_Error& re)
//	{
//		//this->mpRasconn->disconnect();
//		//this->mpRasconn->connect();
//		NMErr(ctxLUMASSMainWin, << re.what());
//		NMDebugCtx(ctxLUMASSMainWin, << "done!");
//	}
}

void
LUMASSMainWin::eraseRasLayer(const QString& imagespec)
{
	QString msg = QString(tr("You're about to delete image '%1'!\nDo you really want to do this?")).
			arg(imagespec);

	int ret = QMessageBox::warning(this, tr("Delete Rasdaman Image"),
			msg, QMessageBox::Yes | QMessageBox::No);

	if (ret == QMessageBox::No)
	{
		return;
	}

	NMDebugAI(<< "Deleting image ..." << imagespec.toStdString() << std::endl);

	int pos = imagespec.indexOf(':');
	QString coll = imagespec.left(pos);

	bool bok;
	double oid = imagespec.right(imagespec.size()-1-pos).toDouble(&bok);
	if (!bok)
	{
                NMLogError(<< ctxLUMASSMainWin << ": Couldn't extract OID from rasdaman image spec!");
		return;
	}

	RasdamanConnector* rasconn = this->getRasdamanConnector();
	if (rasconn == 0)
	{
		return;
	}

	RasdamanHelper2 helper(rasconn);
	helper.deletePSMetadata(coll.toStdString(), oid);
	helper.dropImage(coll.toStdString(), oid);

	std::vector<double> oids = helper.getImageOIDs(coll.toStdString());
	if (oids.size() == 0)
		helper.dropCollection(coll.toStdString());

    if (this->mpPetaView != 0)
		this->updateRasMetaView();
}

void
LUMASSMainWin::updateRasMetaView()
{
	// query the flat metadata table showing all coverage/image metadata
	RasdamanConnector* rasconn = this->getRasdamanConnector();
	if (rasconn == 0)
	{
		return;
	}
	const PGconn* conn = rasconn->getPetaConnection();

	std::stringstream query;
	query << "select create_metatable(); "
		  << "select * from tmp_flatmetadata as t1 "
		  << "right join geometadata as t2 "
		  << "on t1.oid = t2.oid;";

	// copy the table into a vtkTable to be fed into a TableView
	PGresult* res = const_cast<PGresult*>(
			PQexec(const_cast<PGconn*>(conn), query.str().c_str()));
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		NMBoxErr("Rasdaman Image Metadata",
				PQresultErrorMessage(res));
		return;
	}

	int nrows = PQntuples(res);
	int ncols = PQnfields(res);
	if (nrows < 1)
	{
		NMBoxInfo("Rasdaman Image Metadata",
				"No registered rasdaman images found in database!");
                NMDebugCtx(ctxLUMASSMainWin, << "done!");
		return;
	}

	// because of the left join above, we've got two oid fields,
	// but we only want one!
	std::vector<std::string> colnames;
	std::vector<int> colindices;
	int oidcnt = 0;
	for (int c=0; c < ncols; ++c)
	{
		if (   QString(PQfname(res,c)).compare("oid", Qt::CaseInsensitive) == 0
		    && oidcnt == 0)
		{
			colnames.push_back(PQfname(res,c));
			colindices.push_back(c);
			++oidcnt;
		}
		else
		{
			colnames.push_back(PQfname(res,c));
			colindices.push_back(c);
		}
	}

	// copy the table structure
	// all columns, except oid, text columns, so we mostly just use vtk string arrays
	vtkSmartPointer<vtkTable> metatab = vtkSmartPointer<vtkTable>::New();
	QMap<QString, int> idxmap;
	for (int i=0; i < colindices.size(); ++i)
	{
		QString colname(PQfname(res, colindices[i]));
		//NMDebug(<< colname.toStdString() << " ");
		if (colname.compare("oid", Qt::CaseInsensitive) == 0)
		{
			vtkSmartPointer<vtkLongArray> lar = vtkSmartPointer<vtkLongArray>::New();
			lar->SetNumberOfComponents(1);
			lar->SetNumberOfTuples(nrows);
			lar->FillComponent(0,0);
			lar->SetName(PQfname(res, colindices[i]));
			metatab->AddColumn(lar);
			idxmap.insert(colname, colindices[i]);
		}
		else
		{
			vtkSmartPointer<vtkStringArray> sar = vtkSmartPointer<vtkStringArray>::New();
			sar->SetNumberOfComponents(1);
			sar->SetNumberOfTuples(nrows);
			sar->SetName(PQfname(res, colindices[i]));
			metatab->AddColumn(sar);
			idxmap.insert(colname, colindices[i]);
		}
	}
	//NMDebug(<< std::endl);

	//vtkSmartPointer<vtkUnsignedCharArray> car = vtkSmartPointer<vtkUnsignedCharArray>::New();
	//car->SetNumberOfComponents(1);
	//car->SetNumberOfTuples(nrows);
	//car->SetName("nm_sel");

	//vtkSmartPointer<vtkLongArray> idar = vtkSmartPointer<vtkLongArray>::New();
	//idar->SetNumberOfComponents(1);
	//idar->SetNumberOfTuples(nrows);
	//idar->FillComponent(0,0);
	//idar->SetName("nm_id");


	// copy the table body
	int ntabcols = metatab->GetNumberOfColumns();
	for (int r=0; r < nrows; ++r)
	{
		for (int i=0; i < ntabcols; ++i)
		{
			vtkAbstractArray* aar = metatab->GetColumn(i);
			QString colname(aar->GetName());
			if (colname.compare("oid", Qt::CaseInsensitive) == 0)
			{
				vtkLongArray* lar = vtkLongArray::SafeDownCast(aar);
				lar->SetValue(r, ::atol(PQgetvalue(res, r, idxmap.value(colname))));
			}
			else
			{
				vtkStringArray* sar = vtkStringArray::SafeDownCast(aar);
				sar->SetValue(r, PQgetvalue(res, r, idxmap.value(colname)));
			}
		}
		//car->SetTuple1(r, 0);
		//idar->SetTuple1(r, r);
	}
	//NMDebug( << std::endl);
	//metatab->AddColumn(car);
	//metatab->AddColumn(idar);

	PQclear(res);

	vtkQtTableModelAdapter* model;
	if (this->mpPetaView == 0)
	{
		model = new vtkQtTableModelAdapter(metatab, this);
	}
	else
	{
		model = qobject_cast<vtkQtTableModelAdapter*>(this->mPetaMetaModel);
		model->setTable(metatab);

		delete this->mpPetaView;
		this->mpPetaView = 0;
	}
	model->SetKeyColumn(0);
	this->mPetaMetaModel = model;

	this->mpPetaView = new NMTableView(this->mPetaMetaModel, 0);
	this->mpPetaView->setViewMode(NMTableView::NMTABVIEW_RASMETADATA);
	this->mpPetaView->setSelectionModel(
			new NMFastTrackSelectionModel(this->mPetaMetaModel, mpPetaView));


	connect(this->mpPetaView, SIGNAL(notifyLoadRasLayer(const QString&, const QString&)),
			this, SLOT(fetchRasLayer(const QString&, const QString&)));
	connect(this->mpPetaView, SIGNAL(notifyDeleteRasLayer(const QString&)),
			this, SLOT(eraseRasLayer(const QString&)));


	this->mpPetaView->setTitle("Rasdaman Image Metadata");
	//this->mpPetaView->setRowKeyColumn("nm_id");
	//this->mpPetaView->hideAttribute("nm_sel");
	//this->mpPetaView->hideAttribute("nm_id");

	return;
}
#endif

void LUMASSMainWin::loadImageLayer()
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

	QString fileName = QFileDialog::getOpenFileName(this,
	     tr("Open Image"), "~", tr("All Image Files (*.*)"));
	if (fileName.isNull())
		return;

    loadImageLayer(fileName);

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}


void LUMASSMainWin::loadImageLayer(const QString& fileName)
{
    NMDebugCtx(ctxLUMASSMainWin, << "...");

	NMDebugAI( << "opening " << fileName.toStdString() << " ..." << std::endl);

	QFileInfo finfo(fileName);
    if (!finfo.exists() || !finfo.isReadable())
    {
        NMLogError(<< "Load Image: Failed to load image file '"
                   << fileName.toStdString() << "'!")
        NMDebugCtx(ctxLUMASSMainWin, << "done!");
        return;
    }

	vtkRenderWindow* renWin = this->ui->qvtkWidget->GetRenderWindow();
	NMImageLayer* layer = new NMImageLayer(renWin, 0, this);

    this->connectImageLayerProcSignals(layer);
    layer->setFileName(fileName);

    NMDebugCtx(ctxLUMASSMainWin, << "done!");
}

void
LUMASSMainWin::addLayerToCompList()
{
	NMLayer* layer = qobject_cast<NMLayer*>(this->sender());
	if (layer == 0)
		return;

	layer->setVisible(true);
    this->mLayerList->addLayer(layer);
}

void
LUMASSMainWin::addLayerToCompList(NMLayer* layer)
{
    if (layer == 0)
        return;

    this->mLayerList->addLayer(layer);
}

void LUMASSMainWin::toggle3DSimpleMode()
{
	if (m_b3D)
	{
		NMDebugAI( << "switching 3D off ..." << endl);

		// check for stero mode
		if (this->ui->actionToggle3DStereoMode->isChecked())
		{
			this->toggle3DStereoMode();
			this->ui->actionToggle3DStereoMode->setChecked(false);
		}

		// set image interaction
        NMVtkInteractorStyleImage* iasim = NMVtkInteractorStyleImage::New();
#ifdef QT_HIGHDPI_SUPPORT
        iasim->setDevicePixelRatio(this->devicePixelRatioF());
#endif
        this->ui->qvtkWidget->GetInteractor()->SetInteractorStyle(iasim);

		// reset the camera for the background renderer
		vtkRenderer* ren0 = this->mBkgRenderer;
		vtkCamera* cam0 = ren0->GetActiveCamera();

		double *fp = new double[3];
		double *p = new double[3];
		double dist;

		ren0->ResetCamera();
		cam0->GetFocalPoint(fp);
		cam0->GetPosition(p);
		dist = ::sqrt( ::pow((p[0]-fp[0]),2) + ::pow((p[1]-fp[1]),2) + ::pow((p[2]-fp[2]),2) );
		cam0->SetPosition(fp[0], fp[1], fp[2]+dist);
		cam0->SetViewUp(0.0, 1.0, 0.0);

		delete[] fp;
		delete[] p;

		// set new camera for all other renderers as well
		vtkRendererCollection* recoll = this->ui->qvtkWidget->GetRenderWindow()->GetRenderers();
		vtkRenderer *ren = recoll->GetNextItem();
		while (ren != NULL)
		{
			ren->SetActiveCamera(cam0);
			ren = recoll->GetNextItem();
		}

		this->m_orientwidget->SetEnabled(0);
		this->ui->actionToggle3DStereoMode->setEnabled(false);
		m_b3D = false;

        // re-enable the 2D-related tool actions
        // disable 2D-related pan an zoom tools
        QAction* pact = this->ui->mainToolBar->findChild<QAction*>("panAction");
        if (pact) pact->setEnabled(true);

        QAction* zin = this->ui->mainToolBar->findChild<QAction*>("zoomInAction");
        if (zin) zin->setEnabled(true);

        QAction* zout = this->ui->mainToolBar->findChild<QAction*>("zoomOutAction");
        if (zout) zout->setEnabled(true);

        // (re-)activate pan action as default 2D interactor mode
        // coming back from 3D mode
        this->pan(true);
	}
	else
	{
		NMDebugAI( << "switching 3D on!! ..." << endl);

        // we're unselecting all exclusive tool bar actions
        // before going into 3D mode
        this->updateExclusiveActions("", false);

        // disable 2D-related pan an zoom tools
        QAction* pact = this->ui->mainToolBar->findChild<QAction*>("panAction");
        if (pact) pact->setEnabled(false);

        QAction* zin = this->ui->mainToolBar->findChild<QAction*>("zoomInAction");
        if (zin) zin->setEnabled(false);

        QAction* zout = this->ui->mainToolBar->findChild<QAction*>("zoomOutAction");
        if (zout) zout->setEnabled(false);

        this->ui->qvtkWidget->GetInteractor()->SetInteractorStyle(
            vtkInteractorStyleTrackballCamera::New());

		this->m_orientwidget->SetEnabled(1);
		this->ui->actionToggle3DStereoMode->setEnabled(true);
		m_b3D = true;
	}

    emit signalIsIn3DMode(m_b3D);
    NMGlobalHelper::getRenderWindow()->Render();
}

void LUMASSMainWin::toggle3DStereoMode()
{
        this->ui->qvtkWidget->GetRenderWindow()->SetStereoRender(
	    		!this->ui->qvtkWidget->GetRenderWindow()->GetStereoRender());
}

void
LUMASSMainWin::configureSettings()
{
    QDialog* dlg = new QDialog(this);
    dlg->setWindowModality(Qt::WindowModal);
    QVBoxLayout* vlayout = new QVBoxLayout(dlg);

    QtTreePropertyBrowser* bro = new QtTreePropertyBrowser(dlg);
    mSettingsBrowser = bro;
    bro->setResizeMode(QtTreePropertyBrowser::Interactive);
    vlayout->addWidget(bro);

    QPushButton* btnClose = new QPushButton("Close", dlg);
    dlg->connect(btnClose, SIGNAL(pressed()), dlg, SLOT(accept()));
    vlayout->addWidget(btnClose);

    populateSettingsBrowser();

    dlg->exec();
    dlg->deleteLater();
    delete bro;
    mSettingsBrowser = 0;
}

void
LUMASSMainWin::populateSettingsBrowser(void)
{
    if (mSettingsBrowser == 0)
    {
        return;
    }

    mSettingsBrowser->clear();

    QMap<QString, QVariant>::iterator it = mSettings.begin();
    while (it != mSettings.end())
    {
        QtVariantEditorFactory* ed = new QtVariantEditorFactory(mSettingsBrowser);
        QtVariantPropertyManager* man = new QtVariantPropertyManager(mSettingsBrowser);
        QtVariantProperty* vprop = man->addProperty(it.value().type(), it.key());
        vprop->setValue(it.value());
        mSettingsBrowser->setFactoryForManager(man, ed);
        mSettingsBrowser->addProperty(vprop);

        connect(man, SIGNAL(signalCallAuxEditor(QtProperty*,const QStringList &)),
                this, SLOT(settingsFeeder(QtProperty*,const QStringList &)));
        connect(man, SIGNAL(valueChanged(QtProperty*,QVariant)),
                this, SLOT(updateSettings(QtProperty*,QVariant)));
        ++it;
    }
}

void
LUMASSMainWin::updateSettings(QtProperty *prop, const QVariant &val)
{
    if (prop == 0)
    {
        return;
    }

    updateSettings(prop->propertyName(), val);
}

void
LUMASSMainWin::updateSettings(const QString &setting, const QVariant &val)
{
    switch (val.type())
    {
    case QVariant::String:
        mSettings[setting] = val.toString();
        break;
    default:
        break;
    }

    populateSettingsBrowser();

    //    if (setting.compare(QString::fromLatin1("Workspace")) == 0)
    //    {
    //        sqlite3_temp_directory = const_cast<char*>(
    //                    mSettings["Workspace"].toString().toStdString().c_str());
    //    }
    //    else
    if (setting.compare(QString::fromLatin1("UserModels")) == 0)
    {
        scanUserModels();
    }

    emit settingsUpdated(setting, val);
}

void
LUMASSMainWin::settingsFeeder(QtProperty *prop, const QStringList &strVal)
{
    if (prop == 0)
    {
        return;
    }

    if (    prop->propertyName().compare(QString::fromLatin1("UserModels")) == 0
        ||  prop->propertyName().compare(QString::fromLatin1("Workspace")) == 0
       )
    {
        QFileInfo finfo(strVal.at(0));
        QString newfn = QFileDialog::getExistingDirectory(this,
                                     QString("Set '%1' Path").arg(prop->propertyName()),
                                     finfo.absoluteFilePath());

        if (!newfn.isEmpty())
        {
            updateSettings(prop, QVariant::fromValue(newfn));
        }
    }
}

QString
LUMASSMainWin::getUserModelPath(const QString &model)
{
    QString ret;

    if (mUserModelPath.find(model) != mUserModelPath.end())
    {
        ret = mUserModelPath[model];
    }

    return ret;
}

QStringList
LUMASSMainWin::getUserToolsList(void)
{
    return mUserActions.keys();
}

const NMAbstractAction*
LUMASSMainWin::getUserTool(const QString &toolName)
{
    return mUserActions.value(toolName);
}


void
LUMASSMainWin::removeUserTool(NMAbstractAction *act)
{
    if (act == 0)
    {
        return;
    }

    NMModelAction* mact = qobject_cast<NMModelAction*>(act);
    if (mact != nullptr)
    {
        if (mact->getModelController()->isModelRunning())
        {
            NMLogError(<< "Can't remove User Tool while model is still running!");
            return;
        }
        else
        {
            mact->getModelController()->resetComponent("root");
        }
    }

    NMLogDebug(<< "Removing user tool '" << act->text().toStdString() << "'");
    mExclusiveActions.removeOne(act);
    mUserActions.remove(act->objectName());
    NMToolBar* toolbar = qobject_cast<NMToolBar*>(const_cast<QObject*>(act->parent()));
    toolbar->removeAction(act);


    // remove tool entry from settings file to
    // prevent reloading after restart
    QSettings settings("LUMASS", "GUI");

#ifdef __linux__
    settings.setIniCodec("UTF-8");
#endif

    settings.beginGroup("UserToolBars");
    settings.beginGroup(toolbar->objectName());
    settings.remove(act->objectName());
    settings.endGroup();
    settings.endGroup();

    ui->modelViewWidget->updateToolContextBox();

    act->deleteLater();
}

void
LUMASSMainWin::loadUserModelTool(const QString& modelPath,
                                 const QString& userModel,
                                 const QString& toolBarName)
{
    // ============================================
    // look for configuration table
    // ============================================
    QString usermodelpath = modelPath;
    QString baseName = QString("%1Tool").arg(userModel);
    QString toolTableName = QString("%1/%2.ldb")
            .arg(modelPath)
            .arg(baseName);

    QFileInfo fnInfo(toolTableName);
    if (!fnInfo.exists() || !fnInfo.isReadable())
    {
        QString deeppath = QString("%1/%2").arg(usermodelpath).arg(userModel);
        QString deeptool = QString("%1/%2Tool.ldb").arg(deeppath).arg(userModel);
        QFileInfo deepFifo(deeptool);
        if (deepFifo.exists() && deepFifo.isReadable())
        {
            usermodelpath = deeppath;
            toolTableName = deeptool;
        }
        else
        {
            NMLogError(<< "Load User Tool: Couldn't read tool table for '"
                     << userModel.toStdString() << "'!")
            return;
        }
    }

    QString modelFile = QString("%1/%2.lmx")
            .arg(usermodelpath)
            .arg(userModel);

    QFileInfo mfInfo(modelFile);
    if (!mfInfo.exists() || !mfInfo.isReadable())
    {
        NMLogError(<< "Load User Tool: Couldn't read tool's user model '"
                   << usermodelpath.toStdString() << "/"
                   << userModel.toStdString() << ".lmx'!")
        return;
    }

    otb::SQLiteTable::Pointer toolTable = otb::SQLiteTable::New();
    toolTable->SetUseSharedCache(false);
    toolTable->SetDbFileName(toolTableName.toStdString());
    if (!toolTable->openConnection())
    {
        NMLogError(<< "Load User Tool: Couldn't read tool table for '"
                 << userModel.toStdString() << "'!")
        return;
    }
    toolTable->SetTableName(baseName.toStdString());
    toolTable->PopulateTableAdmin();

    // ============================================
    // find tool bar
    // ============================================

    NMToolBar* toolbar = this->findChild<NMToolBar*>(toolBarName);
    if (toolbar == 0)
    {
        NMLogError(<< "Load User Tool: Couldn't find specified tool bar!");
        return;
    }
    QString toolName = toolTable->GetStrValue("AdminValue", "Admin = 'Name'").c_str();

    if (toolbar->findChild<NMAbstractAction*>(toolName))
    {
        NMLogInfo(<< "User Tool '" << toolName.toStdString()
                  << "' has already been loaded!");
        return;
    }

    // ===================================
    // create the user action && set up the model controller
    // ===================================

    NMModelAction* uact = new NMModelAction(toolName, toolbar);
    uact->setObjectName(toolName);
    uact->setModelName(userModel);
    uact->setModelPath(usermodelpath);
    uact->setLogger(mLogger);
    mExclusiveActions << uact;

    // if modelPath is pointing 'into' the
    // tool directory, go one level up ...
    QFileInfo mdir(usermodelpath);
    QString mdirbaseName = mdir.baseName();
    QString usermodelname = userModel;
    if (mdirbaseName.compare(usermodelname) == 0)
    {
        int pos = modelPath.lastIndexOf('/', -1);
        usermodelpath = modelPath.left(pos);
    }

    // initially, we set the tool up as a simple button
    uact->setCheckable(false);

    // create model context
    NMModelController* ctrl = new NMModelController(uact);
    ctrl->setObjectName(toolName);
    ctrl->getLogger()->setHtmlMode(true);
    //ctrl->updateSettings("UserModels", mSettings["UserModels"]);
    ctrl->updateSettings("UserModels", usermodelpath);
    ctrl->updateSettings("Workspace", mSettings["Workspace"]);
    ctrl->updateSettings("LUMASSPath", mSettings["LUMASSPath"]);

    connect(this, SIGNAL(settingsUpdated(const QString &,QVariant)),
            uact, SLOT(updateSettings(const QString &,QVariant)));
    connect(ctrl->getLogger(), SIGNAL(sendLogMsg(QString)),
            this, SLOT(appendHtmlMsg(QString)));
    connect(ui->modelViewWidget, SIGNAL(requestModelAbortion()),
            uact, SLOT(requestAbort()), Qt::DirectConnection);
    connect(ctrl, SIGNAL(signalModelStarted()), this, SLOT(showBusyStart()));
    connect(ctrl, SIGNAL(signalModelStopped()), this, SLOT(showBusyEnd()));
    connect(ctrl, SIGNAL(signalModelStopped()), this, SLOT(displayUserModelOutput()));
    connect(uact, SIGNAL(signalRemoveUserTool(NMAbstractAction*)), this,
            SLOT(removeUserTool(NMAbstractAction*)));
    uact->setModelController(ctrl);


    NMModelSerialiser xmlS;
    xmlS.setModelController(ctrl);
    xmlS.setLogger(mLogger);

    NMSequentialIterComponent* root = new NMSequentialIterComponent();
    root->setObjectName("root");
    root->setDescription("Top level model component managed by the ModelController");
    ctrl->addComponent(root);
    xmlS.parseComponent(modelFile, 0, ctrl);


    // ====================================
    // parse Tool Table
    // ====================================
    uact->reloadUserConfig();


     // just let the user know ...
    if (uact->getTriggerCount() == 0)
    {
        uact->setTrigger("", NMAbstractAction::NM_ACTION_TRIGGER_NIL);
        NMLogInfo(<< "Load User Tool: No trigger parameters specified!");
    }


    if (uact->isCheckable())
    {
        connect(uact, SIGNAL(triggered(bool)), this, SLOT(selectUserTool(bool)));
    }
    else
    {
        connect(uact, SIGNAL(triggered()), this, SLOT(executeUserModel()));
    }

    toolbar->addAction(uact);
    mUserActions.insert(uact->objectName(), uact);
}

void
LUMASSMainWin::executeUserModel(void)
{
    NMModelAction* uact = qobject_cast<NMModelAction*>(sender());
    if (uact)
    {
        NMModelController* ctrl = uact->getModelController();
        if (ctrl)
        {
            const QString compName = "root";
            QtConcurrent::run(ctrl, &NMModelController::executeModel, compName);
        }
    }
}

void
LUMASSMainWin::updateUserModelTriggerParameters(NMAbstractAction *uact)
{
    if (uact == 0)
    {
        return;
    }

    QString filenameParam = uact->getTriggerKey(NMAbstractAction::NM_ACTION_TRIGGER_FILENAME);
    QString colnameParam  = uact->getTriggerKey(NMAbstractAction::NM_ACTION_TRIGGER_COLUMN);
    QString tablenameParam = uact->getTriggerKey(NMAbstractAction::NM_ACTION_TRIGGER_TABLENAME);

    // selected layer filename
    if (!filenameParam.isEmpty() && (colnameParam.isEmpty() && tablenameParam.isEmpty()))
    {
        NMLayer* selLayer = mLayerList->getSelectedLayer();
        if (selLayer)
        {
            QString fn = selLayer->getFileName();
            uact->setProperty(filenameParam.toStdString().c_str(), QVariant::fromValue(fn));
        }
    }


}

QMenu*
LUMASSMainWin::createPopupMenu(void)
{
    QMenu* menu = QMainWindow::createPopupMenu();
    if (!mLastToolBar.isEmpty())
    {
        menu->addSeparator();

        QString removeText = QString("%1 '%2'").arg(tr("Remove")).arg(mLastToolBar);
        QAction* removeAct = new QAction(removeText, menu);
        menu->addAction(removeAct);

        connect(removeAct, SIGNAL(triggered()), this, SLOT(removeUserToolBar()));

        menu->addSeparator();
        QAction* addAct = new QAction(tr("Add User Tool ..."), menu);
        menu->addAction(addAct);

        connect(addAct, SIGNAL(triggered()), this, SLOT(addUserModelToolToToolBar()));
    }
    return menu;
}

void
LUMASSMainWin::addUserModelToolToToolBar()
{
    NMToolBar* bar = 0;
    if (!mLastToolBar.isEmpty())
    {
        bar = this->findChild<NMToolBar*>(mLastToolBar);
    }

    if (bar && mUserModels.count())
    {
        bool bOK;
        QString userTool = QInputDialog::getItem(bar,
                                                 tr("Add User-Model Tool"),
                                                 tr("Select User Model"),
                                                 this->mUserModels,
                                                 0, false, &bOK);
        if (bOK && !userTool.isEmpty())
        {
            this->loadUserModelTool(mSettings["UserModels"].toString(), userTool, bar->objectName());
            ui->modelViewWidget->updateToolContextBox();
        }
    }
}

void
LUMASSMainWin::removeUserToolBar(void)
{
    if (!mLastToolBar.isEmpty())
    {
        NMToolBar* tb = this->findChild<NMToolBar*>(mLastToolBar);
        if (tb)
        {
            QList<NMAbstractAction*> userActions = tb->findChildren<NMAbstractAction*>();
            for (int ua=0; ua < userActions.size(); ++ua)
            {
                this->removeUserTool(userActions.at(ua));
            }

            this->removeToolBar(tb);
            mLastToolBar.clear();

            // remove tool entry from settings file to
            // prevent reloading after restart
            QSettings settings("LUMASS", "GUI");

#ifdef __linux__
            settings.setIniCodec("UTF-8");
#endif

            settings.beginGroup("UserToolBars");
            settings.remove(tb->objectName());
            settings.endGroup();

            delete tb;
        }
    }
    mLastToolBar.clear();
}

void
LUMASSMainWin::updateLastToolBar(void)
{
    if (sender())
    {
        mLastToolBar = sender()->objectName();
    }
    else
    {
        mLastToolBar.clear();
    }
}

void
LUMASSMainWin::addUserToolBar()
{
    bool bOK = false;
    QString tbname = QInputDialog::getText(this, tr("Add Tool Bar"),
                                           tr("Tool Bar Name:"),
                                           QLineEdit::Normal,
                                           QString(), &bOK);
    if (!bOK || tbname.isEmpty())
    {
        return;
    }

    createUserToolBar(tbname);
}

void
LUMASSMainWin::createUserToolBar(const QString& tbname,
                                 const QByteArray& ba)
{

    NMToolBar* toolbar = this->findChild<NMToolBar*>(tbname);
    if (toolbar == 0)
    {
        toolbar = new NMToolBar(tbname, this);
    }
    else
    {
        NMLogInfo(<< "There is already a Tool Bar '"
                  << tbname.toStdString() << "'!");
        return;
    }

    if (ba.size())
    {
        toolbar->restoreGeometry(ba);
    }

    toolbar->setAllowedAreas(Qt::AllToolBarAreas);
    toolbar->setObjectName(tbname);
    toolbar->setOrientation(Qt::Horizontal);
    toolbar->setLogger(mLogger);

    QMainWindow* mw = static_cast<QMainWindow*>(this);
    mw->addToolBar(toolbar);

    connect(toolbar, SIGNAL(signalPopupMenu()), this, SLOT(updateLastToolBar()),
            Qt::DirectConnection);

}

void
LUMASSMainWin::selectUserTool(bool toggled)
{
    QAction* userAction = static_cast<QAction*>(sender());

    if (userAction)
    {
        userAction->setChecked(toggled);

        NMVtkInteractorStyleImage* iact = NMVtkInteractorStyleImage::SafeDownCast(
                    ui->qvtkWidget->GetInteractor()->GetInteractorStyle());

        if (iact)
        {
            if (toggled)
            {
                iact->setNMInteractorMode(NMVtkInteractorStyleImage::NM_INTERACT_MULTI);
                iact->setUserTool(userAction->objectName().toStdString());
            }
            else
            {
                iact->setUserTool("");
            }
        }
        this->updateExclusiveActions(userAction->objectName(), toggled);
    }
}

void
LUMASSMainWin::processUserPickAction(long long cellId)
{
    NMVtkInteractorStyleImage* iact = NMVtkInteractorStyleImage::SafeDownCast(
                ui->qvtkWidget->GetInteractor()->GetInteractorStyle());
    if (iact)
    {
        QString userTool = iact->getUserTool().c_str();
        QMap<QString, NMAbstractAction*>::iterator actIt = mUserActions.find(userTool);
        if (actIt != mUserActions.end())
        {
            NMModelAction* act = qobject_cast<NMModelAction*>(actIt.value());
            QString key = act->getTriggerKey(NMAbstractAction::NM_ACTION_TRIGGER_ID);
            if (act != nullptr && !key.isEmpty())
            {
                NMModelController* ctrl = const_cast<NMModelController*>(
                            act->getModelController());
                if (!ctrl->isModelRunning())
                {
                    ctrl->updateSettings(key, QVariant::fromValue(cellId));
                    const QString compName = "root";
                    QtConcurrent::run(ctrl, &NMModelController::executeModel, compName);
                }
                else
                {
                    NMLogInfo(<< userTool.toStdString()
                              << " is already running - try again once its finished!");
                }
            }
        }
    }
}

void
LUMASSMainWin::displayUserModelOutput(void)
{
    NMModelController* ctrl = static_cast<NMModelController*>(sender());
    if (ctrl == 0)
    {
        return;
    }

    QMap<QString, NMAbstractAction*>::iterator actionIt = mUserActions.find(ctrl->objectName());
    if (actionIt == mUserActions.end())
    {
        NMLogDebug(<< "displayUserModelOutput(): Couldn't find user model '"
                   << ctrl->objectName().toStdString() << "'!");
        return;
    }
    NMAbstractAction* userAction = actionIt.value();

    const NMAbstractAction::NMOutputMap& outMap = userAction->getOutputs();
    NMAbstractAction::NMOutputMap::const_iterator outIt = outMap.cbegin();
    while (outIt != outMap.cend())
    {
        NMAbstractAction::NMActionOutputType outtype = outIt.value();
        const QString& output = outIt.key();
        NMModelComponent* comp = ctrl->getComponent(output);
        if (comp == 0)
        {
            QList<NMModelComponent*> setofcomps = ctrl->getComponents(output);
            if (setofcomps.count() > 0)
            {
                comp = setofcomps.at(0);
            }
        }

        if (comp == 0)
        {
            NMLogError(<< userAction->objectName().toStdString() << ": "
                       << "Invalid output component '" << output.toStdString() << "'!");
            continue;
        }

        // get the process holding the FileName(s) (and TableName) properties
        NMIterableComponent* icomp = qobject_cast<NMIterableComponent*>(comp);
        if (icomp == 0)
        {
            NMLogError(<< userAction->objectName().toStdString() << ": "
                       << "Invalid output component '" << output.toStdString() << "'!");
            continue;
        }

        NMProcess* proc = icomp->getProcess();
        if (proc == 0)
        {
            NMLogError(<< userAction->objectName().toStdString() << ": "
                       << "Invalid output component '" << output.toStdString() << "'!");
            continue;
        }

        // we can just display an output buffer (default), or
        // specify a reader or writer component, whose filename
        // specifies the output data set to be displayed
        switch (outtype)
        {
        case NMAbstractAction::NM_ACTION_DISPLAY_FILENAME:
            {
                // always just grab the first filename from the list
                QVariant fnVar = proc->getParameter("FileNames");
                if (fnVar.isValid() && fnVar.type() == QVariant::StringList)
                {
                    QString fn = fnVar.toStringList().at(0);
                    this->loadImageLayer(fn);
                }
                // and here comes the impl. for tables
                else
                {
                    bool bfailed = true;
                    fnVar = proc->getParameter("FileName");
                    if (fnVar.isValid())
                    {
                        QVariant tn = proc->getParameter("TableName");
                        if (tn.isValid())
                        {
                            this->importTable(fnVar.toString(),
                                              LUMASSMainWin::NM_TABVIEW_STANDALONE,
                                              false,
                                              tn.toString());
                            bfailed = false;
                        }
                    }

                    if (bfailed)
                    {
                        if (proc->parent() != nullptr)
                        {
                            NMLogError(<< userAction->objectName().toStdString() << ": "
                                       << "Failed to display output table!");
                        }
                    }
                }
            }
            break;

        default:
        case NMAbstractAction::NM_ACTION_DISPLAY_BUFFER:
            {
                // ============================================
                //              OUTPUT LAYER
                // ============================================
                if (!comp->getOutput(0).isNull() && comp->getOutput(0)->getDataObject() != 0)
                {
                    QString layerName = output;
                    //            if (!comp->getUserID().isEmpty())
                    //            {
                    //                layerName = comp->getUserID();
                    //            }

                    vtkRenderWindow* renWin = ui->qvtkWidget->GetRenderWindow();
                    NMImageLayer* iLayer = new NMImageLayer(renWin, 0, this);
                    iLayer->setObjectName(layerName);
                    iLayer->setLogger(mLogger);
                    connectImageLayerProcSignals(iLayer);

                    iLayer->setImage(comp->getOutput(0));

                    connect(comp, SIGNAL(NMDataComponentChanged()), iLayer, SLOT(updateSourceBuffer()));
                }
                // =========================================================
                //              STAND ALONE TABLE
                // =========================================================
                else if (!comp->getOutput(0).isNull() && comp->getOutput(0)->getOTBTab().IsNotNull())
                {
                    otb::SQLiteTable::Pointer sqltab = static_cast<otb::SQLiteTable*>(comp->getOutput(0)->getOTBTab().GetPointer());

                    if (sqltab && !sqltab->GetDbFileName().empty())
                    {
                        createTableView(sqltab);
                    }
                }
            }
            break;
        }

        // get the next tool output
        ++outIt;
    }

    // experimental
    ctrl->resetComponent("root");
}

void
LUMASSMainWin::scanUserModels()
{
    mUserModelListWidget->clear();
    mUserModelPath.clear();
    mUserModels.clear();

    QString folder = mSettings["UserModels"].toString();

    QFileInfo finfo(folder);
    QDir dir = finfo.absoluteFilePath();

    QFileInfoList lst = dir.entryInfoList();
    for (int i=0; i < lst.size(); ++i)
    {
        const QFileInfo& fifo = lst.at(i);
        QString path = dir.absolutePath();
        if (fifo.isDir())
        {
            path = fifo.absoluteFilePath();
        }

        QString lmvFile = QString("%1/%2.lmv")
                .arg(path)
                .arg(fifo.baseName());
        QString lmxFile = QString("%1/%2.lmx")
                .arg(path)
                .arg(fifo.baseName());
        QString toolTable = QString("%1/%2Tool.ldb")
                .arg(path)
                .arg(fifo.baseName());

        if (   QFile::exists(lmvFile)
            && QFile::exists(lmxFile)
            && !lmxFile.endsWith("autosave.lmx", Qt::CaseInsensitive)
           )
        {
            if (mUserModelPath.find(fifo.baseName()) == mUserModelPath.end())
            {
                QPixmap pm;
                pm.load(":model-icon.png");
                QListWidgetItem* wi = new QListWidgetItem(QIcon(pm),
                                                          fifo.baseName(),
                                                          mUserModelListWidget);
                QFileInfo modelFileInfo(lmxFile);
                mUserModelListWidget->addItem(wi);
                mUserModelPath[fifo.baseName()] = modelFileInfo.absolutePath();
            }

            if (QFile::exists(toolTable))
            {
                mUserModels << fifo.baseName();
            }
        }
        else
        {
            // log a warning that not all model files are valid
            std::stringstream str;
            str << "Scanning User Models: "
                << fifo.absolutePath().toStdString() << "/"
                << fifo.baseName().toStdString()
                << (fifo.suffix().isEmpty() ? "" : ".") << fifo.suffix().toStdString();

            if (!lmxFile.endsWith("autosave.lmx", Qt::CaseInsensitive))
            {
                str << " is an automatic backup copy we don't consider here!";
            }
            else
            {
                str << " is not a LUMASS model!";
            }

            NMLogDebug(<< str.str());
        }
    }
}

void
LUMASSMainWin::addModelToUserModelList(const QString& modelName)
{
    bool bOnList = false;
    for (int i=0; i < mUserModelListWidget->count(); ++i)
    {
        if (mUserModelListWidget->item(i)->text().compare(modelName, Qt::CaseInsensitive) == 0)
        {
            bOnList = true;
            break;
        }
    }


    // =========================================
    //   pick a nice name for the model
    // =========================================
    NMModelComponent* mc = NMGlobalHelper::getModelController()->getComponent(modelName);
    QString basename = modelName;

    if (mc)
    {
        basename = mc->getUserID();
        if (basename.isEmpty())
        {
            basename = mc->getDescription();
            if (basename.isEmpty())
            {
                basename = modelName;
            }
        }
    }

    QString filename = QString("%1/%2.lmx")
                        .arg(mSettings["UserModels"].toString())
                        .arg(basename);
    QString foldername = QString("%1/%2")
                        .arg(mSettings["UserModels"].toString())
                        .arg(basename);

    QFileInfo fileInfo(filename);
    QFileInfo folderInfo(foldername);

    // =========================================
    //   check, whether we've got a model with this name already
    // =========================================
    bool bWriteModel = true;
    bool bAddToList = true;
    if (fileInfo.exists() || folderInfo.exists())
    {
        QMessageBox::StandardButton userBtn =
                QMessageBox::question(
                    this, tr("Add User Model"),
                    QString("The model '%1' already exists! Do you "
                            "want to override the model?").arg(basename));

        if (userBtn == QMessageBox::No)
        {
            bWriteModel = false;
        }
        else
        {
            if (folderInfo.exists())
            {
                filename = QString("%1/%2.lmx")
                        .arg(foldername).arg(basename);
            }
        }
        bAddToList = false;
    }

    // =========================================
    //   if all lights are green, add the model
    // =========================================
    if (bWriteModel)
    {
        QList<QGraphicsItem*> items;
        QGraphicsItem* theItem = ui->modelViewWidget->getScene()->getComponentItem(modelName);
        if (theItem != 0)
        {
            items << theItem;
            ui->modelViewWidget->exportItems(items, filename, false);

        }

        if (bAddToList)
        {
            QPixmap pm;
            pm.load(":model-icon.png");
            QListWidgetItem* wi = new QListWidgetItem(QIcon(pm), basename, mUserModelListWidget);
            mUserModelListWidget->addItem(wi);
            QFileInfo storageFileInfo(filename);
            mUserModelPath[basename] = storageFileInfo.absolutePath();
        }
    }
}

void
LUMASSMainWin::show()
{
    QMainWindow::show();
    if (mbFirstTimeLoaded)
    {
        mbFirstTimeLoaded = false;
        emit windowLoaded();
    }
}

void LUMASSMainWin::readSettings()
{
    QSettings settings("LUMASS", "GUI");

#ifdef __linux__
    settings.setIniCodec("UTF-8");
#endif

    // ================================================================
    //              LUMASSMainWin
    // ================================================================
    settings.beginGroup("LUMASSMainWin");

    // size & toolbars & docks
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    // custom widgets' state
    for (int i=0; i < ui->compWidgetList->getWidgetItemCount(); ++i)
    {
        QString key = QString("%1/visible").arg(ui->compWidgetList->getWidgetItemName(i));
        QVariant val = settings.value(key);
        if (val.isValid())
        {
            ui->compWidgetList->setWidgetItemVisible(i, val.toBool());
        }
    }

    for (int i=0; i < ui->infoWidgetList->getWidgetItemCount(); ++i)
    {
        QString key = QString("%1/visible").arg(ui->infoWidgetList->getWidgetItemName(i));
        QVariant val = settings.value(key);
        if (val.isValid())
        {
            ui->infoWidgetList->setWidgetItemVisible(i, val.toBool());
        }
    }

    QVariant val = settings.value("MapView/visible");
    if (val.isValid())
    {
        mMiscActions["actMapBtn"]->setChecked(val.toBool());
        ui->qvtkWidget->setVisible(val.toBool());
    }

    val = settings.value("ModelView/visible");
    if (val.isValid())
    {
        mMiscActions["actModelBtn"]->setChecked(val.toBool());
        ui->modelViewWidget->setVisible(val.toBool());
    }

    val = settings.value("HorzLayout");
    if (val.isValid())
    {
        if (val.toBool()) // horz layout
        {
            mMiscActions["actHorzLayout"]->setChecked(true);
            this->swapWindowLayout(mMiscActions["actHorzLayout"]);
        }
        else // vert layout
        {
            mMiscActions["actVertLayout"]->setChecked(true);
            this->swapWindowLayout(mMiscActions["actVertLayout"]);
        }
    }

    val = settings.value("CentralSplitter");
    if (val.isValid())
    {
        QSplitter* splitter = this->ui->centralWidget->findChild<QSplitter*>("MainSplitter");
        splitter->restoreState(val.toByteArray());
    }
    settings.endGroup();

    // ================================================================
    //              Directories
    // ================================================================
    settings.beginGroup("Directories");
    val = settings.value("Workspace");
    if (val.isValid())
    {
        mSettings["Workspace"] = val.toString();
        //        sqlite3_temp_directory = const_cast<char*>(
        //                    mSettings["Workspace"].toString().toStdString().c_str());
        emit settingsUpdated("Workspace", val);
    }

    val = settings.value("UserModels");
    if (val.isValid())
    {
        mSettings["UserModels"] = val.toString();
        emit settingsUpdated("UserModels", val);
    }

    settings.endGroup();

    // ================================================================
    //              re scan user models
    // ================================================================
    scanUserModels();

    // ================================================================
    //              UserToolBars & UserTools
    // ================================================================
    settings.beginGroup("UserToolBars");

    // ---------------------------------------------------------
    // for each toolbar

    QStringList toolBars = settings.childGroups();
    foreach (const QString& tbKey, toolBars)
    {
        settings.beginGroup(tbKey);
        QVariant baVar = settings.value("geometry");
        QByteArray ba;
        if (baVar.isValid())
        {
            ba = baVar.toByteArray();
        }
        this->createUserToolBar(tbKey, ba);

        // --------------------------------------------------------
        // scan 'new' and 'old' settings - identify tools

        QStringList userTools = settings.childGroups();
        QMap<QString, QString> mapModelName;
        QMap<QString, QString> mapModelPath;
        QStringList lstJustNames;
        foreach (const QString& utKey, userTools)
        {
            settings.beginGroup(utKey);
            QVariant nameVar = settings.value("ModelName");
            if (nameVar.isValid())
            {
                mapModelName.insert(utKey, nameVar.toString());
            }

            QVariant pathVar = settings.value("ModelPath");
            if (pathVar.isValid())
            {
                mapModelPath.insert(utKey, pathVar.toString());
            }
            settings.endGroup(); // userTools group
        }

        QStringList oldSpecKeys = settings.childKeys();
        foreach (const QString& oldKey, oldSpecKeys)
        {
            // name hasn't been found yet
            if (mapModelName.find(oldKey) == mapModelName.end())
            {
                // insert
                QVariant oldVal = settings.value(oldKey);
                if (oldVal.isValid() && oldVal.type() == QVariant::String)
                {
                    mapModelName.insert(oldKey, oldVal.toString());
                }
            }
            else // remove key
            {
                settings.remove(oldKey);
                NMLogDebug(<< "removed old UserTool name key: " << oldKey.toStdString());
            }
        }

        // ------------------------------------------------------
        // load identified tools

        QMap<QString, QString>::const_iterator pathIter;
        QMap<QString, QString>::const_iterator nameIter = mapModelName.cbegin();
        while (nameIter != mapModelName.cend())
        {
            // if we've got an explict model path, we use that, ...
            pathIter = mapModelPath.find(nameIter.key());
            if (pathIter != mapModelPath.cend())
            {
                this->loadUserModelTool(pathIter.value(), nameIter.value(), tbKey);
            }
            // ... otherwise, we see whether there's a user model around with that name
            else if (mUserModels.contains(nameIter.value()))
            {
                this->loadUserModelTool(mUserModelPath[nameIter.value()], nameIter.value(), tbKey);
            }
            else
            {
                NMLogError(<< "The referenced user tool '"
                           << nameIter.value().toStdString() << "' "
                           << " could not be found at the previously specified location!"
                           << " Please double check your settings and add the tool manually agin.");
            }
            ++nameIter;
        }
    }
    settings.endGroup(); // UserToolBars
    this->ui->modelViewWidget->updateToolContextBox();
}

void LUMASSMainWin::writeSettings(void)
{
    QSettings settings("LUMASS", "GUI");

#ifdef __linux__
    settings.setIniCodec("UTF-8");
#endif

    // ================================================================
    //              LUMASSMainWin
    // ================================================================

    settings.beginGroup("LUMASSMainWin");

    // size & toolbars & docks
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    // custom widgets states
    for (int i=0; i < ui->compWidgetList->getWidgetItemCount(); ++i)
    {
        QString key = QString("%1/visible").arg(ui->compWidgetList->getWidgetItemName(i));
        settings.setValue(key, ui->compWidgetList->isWidgetItemVisible(i));
    }

    for (int i=0; i < ui->infoWidgetList->getWidgetItemCount(); ++i)
    {
        QString key = QString("%1/visible").arg(ui->infoWidgetList->getWidgetItemName(i));
        settings.setValue(key, ui->infoWidgetList->isWidgetItemVisible(i));
    }

    // map and model view
    settings.setValue("MapView/visible", ui->qvtkWidget->isVisible());
    settings.setValue("ModelView/visible", ui->modelViewWidget->isVisible());
    settings.setValue("HorzLayout", mMiscActions["actHorzLayout"]->isChecked());

    // central splitter position
    QSplitter* splitter = this->ui->centralWidget->findChild<QSplitter*>("MainSplitter");
    settings.setValue("CentralSplitter", splitter->saveState());

    settings.endGroup();

    // ================================================================
    //              Directories
    // ================================================================
    settings.beginGroup("Directories");

    settings.setValue("Workspace", mSettings["Workspace"]);
    settings.setValue("UserModels", mSettings["UserModels"]);

    settings.endGroup();

    // ================================================================
    //              User TOOLS
    // ================================================================
    settings.beginGroup("UserToolBars");

    QList<NMToolBar*> toolBars = this->findChildren<NMToolBar*>();
    foreach(const NMToolBar* tb, toolBars)
    {
        settings.beginGroup(tb->objectName());
        settings.setValue("geometry", tb->saveGeometry());

        QList<NMAbstractAction*> userTools = tb->findChildren<NMAbstractAction*>();
        foreach(const NMAbstractAction* act, userTools)
        {
            QString nameKey = QString("%1/ModelName").arg(act->objectName());
            QString pathKey = QString("%1/ModelPath").arg(act->objectName());
            settings.setValue(nameKey, act->getModelName());
            settings.setValue(pathKey, act->getModelPath());
        }
        settings.endGroup();
    }
    settings.endGroup();
}

void LUMASSMainWin::closeEvent(QCloseEvent* event)
{
    writeSettings();
    QMainWindow::closeEvent(event);
}


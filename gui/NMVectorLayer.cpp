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
 * NMVectorLayer.cpp
 *
 *  Created on: 11/03/2011
 *      Author: alex
 */

//#include "NMTableView.h"

#ifndef NM_ENABLE_LOGGER
#   define NM_ENABLE_LOGGER
#   include "nmlog.h"
#   undef NM_ENABLE_LOGGER
#else
#   include "nmlog.h"
#endif
#include "NMGlobalHelper.h"

#include "NMVectorLayer.h"

#include <QHash>
#include <QTime>
#include <QVector>
#include <QFileInfo>
#include <QColor>

#include "vtkIntArray.h"
#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkLookupTable.h"
#include "vtkTriangleFilter.h"
#include "vtkOGRLayerMapper.h"
#include "vtkPolyDataMapper.h"
//#include "vtkOpenGLPolyDataMapper.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"
#include "vtkPolygon.h"
#include "vtkMath.h"
#include "vtkTable.h"
#include "vtkDataArray.h"
#include "vtkLongArray.h"
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"
#include "vtkSmartPointer.h"
//#include "vtkLODActor.h"

#include "vtkQtEditableTableModelAdapter.h"
#include "vtkQtTableModelAdapter.h"

#include "vtkTableToSQLiteWriter.h"
#include "vtkSQLDatabase.h"
#include "vtkSQLQuery.h"
#include "vtkSQLiteQuery.h"
#include "vtkRowQueryToTable.h"
#include "vtkSQLiteDatabase.h"
#include "vtkProperty.h"
//#include "vtkDepthSortPolyData.h"
#include "vtkExtractCells.h"
#include "vtkGeometryFilter.h"
#include "vtkDataSetMapper.h"

#include "vtkOpenGLPolyDataMapper.h"
#include "NMVtkOpenGLPolyDataMapper.h"
#include "NMPolygonToTriangles.h"


NMVectorLayer::NMVectorLayer(vtkRenderWindow* renWin,
		vtkRenderer* renderer,
		QObject* parent)
	: NMLayer(renWin, renderer, parent)
{
	this->mLayerType = NMLayer::NM_VECTOR_LAYER;
	this->mFeatureType = NMVectorLayer::NM_UNKNOWN_FEATURE;
	this->mContour = 0;
	this->mContourMapper = 0;
	this->mContourActor = 0;
	this->mContourColour = QColor(0, 0, 0, 255);

	this->mLayerIcon = QIcon(":vector_layer.png");
    this->mContourOnly = false;
}

NMVectorLayer::~NMVectorLayer()
{
	NMDebugCtx(ctxNMVectorLayer, << "...");
	NMDebugAI(<< "removing layer: " << this->objectName().toStdString() << std::endl);

	if (this->mRenderer != 0)
	{
		this->removeFromMap();
	}

	NMDebugCtx(ctxNMVectorLayer, << "done!");
}

NMVectorLayer::NMFeatureType NMVectorLayer::getFeatureType(void)
{
	return this->mFeatureType;
}

void NMVectorLayer::removeFromMap(void)
{
	NMDebugCtx(ctxNMVectorLayer, << "...");

	// the actor is going to be removed by the superclass NMLayer
//	if (this->mActor != 0)
//		if (this->mRenderer != 0)
//			this->mRenderer->RemoveActor(this->mActor);

	if (this->mContourActor != 0)
		if (this->mRenderer != 0)
			this->mRenderer->RemoveActor(this->mContourActor);

//	this->setVisible(false);

	NMDebugCtx(ctxNMVectorLayer, << "done!");
}

void NMVectorLayer::setContour(vtkPolyData* contour)
{
	if (!contour)
	{
		NMDebugCtx(ctxNMLayer, << "contour is NULL!");
		return;
	}

	this->mContour = contour;

	// create a default black colour for the outline
	vtkSmartPointer<vtkLongArray> contClr = vtkSmartPointer<vtkLongArray>::New();
	contClr->Allocate(this->mContour->GetNumberOfCells());
	contClr->SetNumberOfValues(this->mContour->GetNumberOfCells());
	for (int i=0; i < this->mContour->GetNumberOfCells(); ++i)
			contClr->SetValue(i, i);
	this->mContour->GetCellData()->SetScalars(contClr);

	// create a contour mapper
#ifdef VTK_OPENGL2
    vtkSmartPointer<vtkPolyDataMapper> m = vtkSmartPointer<vtkPolyDataMapper>::New();
#else
    vtkSmartPointer<vtkOGRLayerMapper> m = vtkSmartPointer<vtkOGRLayerMapper>::New();
#endif



//    vtkSmartPointer<vtkTriangleFilter> tf = vtkSmartPointer<vtkTriangleFilter>::New();
//    tf->SetInputData(this->mContour);
//    m->SetInputConnection(tf->GetOutputPort());
    m->SetInputData(this->mContour);

	vtkSmartPointer<vtkLookupTable> olclrtab = vtkSmartPointer<vtkLookupTable>::New();
	long ncells = this->mContour->GetNumberOfCells();
	olclrtab->Allocate(ncells);
	olclrtab->SetNumberOfTableValues(ncells);

	// since we can save selections with the data set, we better check, whether
	// we've got some
	vtkDataSetAttributes* dsAttr = this->mDataSet->GetAttributes(vtkDataSet::CELL);
	vtkDataArray* sel = dsAttr->GetArray("nm_sel");

	for (int c=0; c < ncells; ++c)
	{
		olclrtab->SetTableValue(c,
					mContourColour.redF(),
					mContourColour.greenF(),
					mContourColour.blueF(),
					mContourColour.alphaF());
	}
	//olclrtab->SetTableValue(0, 0, 0, 0, 0);
	//olclrtab->SetTableValue(ncells, 0, 0, 0, 0);
	olclrtab->SetTableRange(0, ncells-1);

	m->SetLookupTable(olclrtab);
	m->SetUseLookupTableScalarRange(1);
	this->mContourMapper = m;

	// create a contour actor
	vtkSmartPointer<vtkActor> a = vtkSmartPointer<vtkActor>::New();
	a->SetMapper(m);
	//a->GetProperty()->SetOpacity(0.2);
	this->mContourActor = a;
	this->mContourActor->SetVisibility(1);
	this->mContourActor->GetProperty()->SetLineWidth(1);
	this->mRenderer->AddActor(a);
}

void
NMVectorLayer::setContourColour(QColor clr)
{
    if (this->getFeatureType() != NMVectorLayer::NM_POLYGON_FEAT)
        return;

    if (this->mContourActor.GetPointer() == 0)
        return;

    if (clr.isValid())
    {
        this->mContourColour = clr;
    }

    vtkLookupTable* cclrs = vtkLookupTable::SafeDownCast(mContourMapper->GetLookupTable());
    for(int a=0; a < cclrs->GetNumberOfTableValues(); ++a)
    {
        cclrs->SetTableValue(a,
                             mContourColour.redF(),
                             mContourColour.greenF(),
                             mContourColour.blueF(),
                             mContourColour.alphaF());
    }
    this->mContourMapper->Update();
}

void NMVectorLayer::setDataSet(vtkDataSet* dataset)
{
	// set the polydata
	if (!dataset)
	{
		NMDebugCtx(ctxNMVectorLayer, << "data set is NULL!");
		return;
	}

	emit layerProcessingStart();

	vtkPolyData* pd = vtkPolyData::SafeDownCast(dataset);
	this->mDataSet = pd;

	// set the feature type
	if (pd->GetVerts()->GetNumberOfCells())
		this->mFeatureType = NMVectorLayer::NM_POINT_FEAT;
	else if (pd->GetLines()->GetNumberOfCells())
		this->mFeatureType = NMVectorLayer::NM_POLYLINE_FEAT;
	else if (pd->GetPolys()->GetNumberOfCells())
		this->mFeatureType = NMVectorLayer::NM_POLYGON_FEAT;

	// set the bounding box
	pd->GetBounds(this->mBBox);

	//mDepthSort = vtkSmartPointer<vtkDepthSortPolyData>::New();
	//mDepthSort->SetInput(pd);
	////sortof->SetDirectionToBackToFront();
	////sortof->SetVector(1,1,1);
	//mDepthSort->SetCamera(this->mRenderer->GetActiveCamera());
	//mDepthSort->SortScalarsOn();

	// create and set the mapper
#ifdef VTK_OPENGL2
    vtkSmartPointer<NMVtkOpenGLPolyDataMapper> m = vtkSmartPointer<NMVtkOpenGLPolyDataMapper>::New();

//    QString vscodeFN = "/home/alex/garage/cpp/lumass/utils/NMVtk/vtkPolyDataVS.glsl";
//    QFile vscodeFile(vscodeFN);
//    vscodeFile.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream vsstr(&vscodeFile);
//    QString vscode = vsstr.readAll();
//    m->SetVertexShaderCode(vscode.toStdString().c_str());
//    vscodeFile.close();

//    QString cscodeFN = "/home/alex/garage/cpp/lumass/utils/NMVtk/vtkPolyDataCS.glsl";
//    QFile cscodeFile(cscodeFN);
//    cscodeFile.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream csstr(&cscodeFile);
//    QString cscode = csstr.readAll();
//    m->SetTessControlShaderCode(cscode.toStdString().c_str());
//    cscodeFile.close();

//    QString escodeFN = "/home/alex/garage/cpp/lumass/utils/NMVtk/vtkPolyDataES.glsl";
//    QFile escodeFile(escodeFN);
//    escodeFile.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream esstr(&escodeFile);
//    QString escode = esstr.readAll();
//    m->SetTessEvaluationShaderCode(escode.toStdString().c_str());
//    escodeFile.close();

//    QString fscodeFN = "/home/users/herziga/garage/cpp/lumass/utils/NMVtk/vtkPolyDataFS.glsl";
//    QFile fscodeFile(fscodeFN);
//    fscodeFile.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream fsstr(&fscodeFile);
//    QString fscode = fsstr.readAll();
//    m->SetFragmentShaderCode(fscode.toStdString().c_str());
//    fscodeFile.close();

//    vtkSmartPointer<vtkTriangleFilter> tf = vtkSmartPointer<vtkTriangleFilter>::New();
//    tf->SetInputData(pd);
//    m->SetInputConnection(tf->GetOutputPort());

//    vtkSmartPointer<NMPolygonToTriangles> tess = vtkSmartPointer<NMPolygonToTriangles>::New();
//    tess->SetInputData(pd);
//    tess->Update();
//    pd = tess->GetOutput();
//    this->mDataSet = pd;
//    //m->SetInputData(tess->GetOutput());


//    std::vector<vtkIdType> ids = tess->GetPolyIdMap();
//    std::cout << "tris id -> poly id ...\n";
//    for (int i=0; i < ids.size(); ++i)
//    {
//        std::cout << "   " << i << "    -> " << ids.at(i) << "\n";
//    }


    m->SetInputData(pd);
#else
    vtkSmartPointer<vtkOGRLayerMapper> m = vtkSmartPointer<vtkOGRLayerMapper>::New();
    m->SetInputData(pd);
#endif

//    m->SetInputData(pd);
	//m->SetInputConnection(mDepthSort->GetOutputPort());
	//m->SetScalarRange(0, mDepthSort->GetOutput()->GetNumberOfCells());
	this->mMapper = m;

	// create and set the actor
    vtkSmartPointer<vtkActor> a = vtkSmartPointer<vtkActor>::New();
	a->SetMapper(m);
	//a->GetProperty()->SetOpacity(0.3);
	//mDepthSort->SetProp3D(a);
    //a->GetProperty()->SetRepresentation(VTK_WIREFRAME);
	this->mActor = a;
	this->mActor->SetVisibility(0);


	//vtkActor* actor = vtkActor::SafeDownCast(this->mActor);
	//actor->GetProperty()->SetOpacity(0.2);
	this->mRenderer->AddActor(a);

	// create contours, if we've got polygons
    if (this->mFeatureType == NMVectorLayer::NM_POLYGON_FEAT)
    {
        //NMDebugAI( << "NMVectorLayer '" << this->objectName().toStdString() <<
        //		"' contains polygons!" << endl);
        vtkSmartPointer<vtkPolyData> cont = vtkSmartPointer<vtkPolyData>::New();
        cont->SetPoints(pd->GetPoints());
        cont->SetLines(pd->GetPolys());
        this->setContour(cont);
    }
//    this->mContour = nullptr;

	this->initiateLegend();

	emit layerProcessingEnd();
}

const vtkPolyData* NMVectorLayer::getContour(void)
{
	return this->mContour;
}

#ifdef VTK_OPENGL2
const vtkPolyDataMapper *NMVectorLayer::getContourMapper(void)
#else
const vtkOGRLayerMapper* NMVectorLayer::getContourMapper(void)
#endif
{
	return this->mContourMapper;
}

const vtkActor* NMVectorLayer::getContourActor(void)
{
	return  this->mContourActor;
}

void NMVectorLayer::setVisible(bool visible)
{
    //NMDebugCtx(ctxNMVectorLayer, << "...");

	if (this->mDataSet == 0)
		return;

	if (this->mIsVisible != visible)
	{
		int vis = visible ? 1 : 0;

		if (this->mActor != 0)
		{
			// now handle all additional parts, that have to
			// change visibility
            if (this->mContourActor != 0)
            {
				this->mContourActor->SetVisibility(vis);
            }

            if (!mContourOnly)
            {
                this->mActor->SetVisibility(vis);
            }

            this->mIsVisible = visible;
			emit visibilityChanged(this);
		}
	}

    //NMDebugCtx(ctxNMVectorLayer, << "done!");
}

/*double NMVectorLayer::getArea()
{
	NMDebugCtx(ctxNMVectorLayer, << "...");

	NMDebugAI( << "call base class ..." << endl);
	this->mTotalArea = NMLayer::getArea();
	NMDebugAI( << "base class' area is " << this->mTotalArea / 10000
			<< " ha" << endl);

	// check for polyline or polygon
	vtkPolyData* pd = vtkPolyData::SafeDownCast(this->mDataSet);
	vtkIdType numPolys = pd->GetNumberOfPolys();
	if (numPolys < 1)
	{
		NMDebugAI( << "oh oh, no polys!" << endl);
		return this->mTotalArea;
	}

	// calculate area in x-y plane for bounding box ;
	NMDebugAI( << "calc the plane in x-y" << endl);
	if (numPolys > 0)
	{
		// calculate area for polygons
		double area = 0;
		for (long c=0; c < numPolys; c++)
		{
			vtkIdType cid = c;
			vtkCell* cell = this->mDataSet->GetCell(cid);
			if (cell == 0)
			{
				NMDebugInd(2, << "error getting cell " << c << endl);
				return -1;
			}
			vtkPolygon* poly = vtkPolygon::SafeDownCast(cell);

			area += poly->ComputeArea();
		}
		this->mTotalArea = area;
	}

	NMDebugAI( << "layer's area: " << this->mTotalArea << endl);

	NMDebugCtx(ctxNMVectorLayer, << "done!");

	return this->mTotalArea;
}*/

long NMVectorLayer::getNumberOfFeatures(void)
{
	return this->mDataSet->GetNumberOfCells();
}


//void
//NMVectorLayer::mapUniqueValues()
//{
//	if (this->mFeatureType != NMVectorLayer::NM_POLYGON_FEAT)
//	{
//		NMLayer::mapUniqueValues();
//		return;
//	}
//
//	// make a list of available attributes
//	vtkDataSet* ds = const_cast<vtkDataSet*>(this->getDataSet());
//	vtkDataSetAttributes* dsAttr = ds->GetAttributes(vtkDataSet::CELL);
//	vtkUnsignedCharArray* hole = vtkUnsignedCharArray::SafeDownCast(
//			dsAttr->GetArray("nm_hole"));
//	vtkLongArray* nmid = vtkLongArray::SafeDownCast(
//			dsAttr->GetArray("nm_id"));
//
//	// let's find out about the attribute
//	// if we've got doubles, we refuse to map unique values ->
//	// doesn't make sense, does it?
//	vtkAbstractArray* anAr = dsAttr->GetAbstractArray(mLegendValueField.toStdString().c_str());
//	int type = anAr->GetDataType();
//	if (type == VTK_DOUBLE)
//	{
//		NMDebugAI( << "oh no, not with doubles!" << endl);
//		return;
//	}
//
//	bool bNum = true;
//	if (type == VTK_STRING)
//		bNum = false;
//
//	const long ncells = anAr->GetNumberOfTuples();
//
//	// we create a new look-up table and set the number of entries we need
//	mLookupTable = vtkSmartPointer<vtkLookupTable>::New();
//	mLookupTable->SetNumberOfTableValues(ncells);
//
//	// let's create a new legend info table
//	this->resetLegendInfo();
//
//	// we iterate over the number of tuples in the user specified attribute array
//	// and assign each unique categorical value its own (hopefully unique)
//	// random colour, which is then inserted into the layer's lookup table; we further
//	// specify a default name for each colour and put it together with the
//	// chosen colour into a LengendInfo-Table, which basically holds the legend
//	// category to display; for linking attribute values to table-info and lookup-table
//	// indices, we fill the HashMap mHashValueIndices (s. Header file for further descr.)
//	bool bConvOk;
//	int clrCount = 0, val;
//	QString sVal;
//	vtkMath::RandomSeed(QTime::currentTime().msec());
//	double rgba[4];
//	for (int t=0; t < ncells; ++t)
//	{
//		if (hole->GetValue(t))
//		{
//			mLookupTable->SetTableValue(t, rgba[0], rgba[1], rgba[2]);
//			continue;
//		}
//
//		if (bNum)
//		{
//			int val = anAr->GetVariantValue(t).ToInt(&bConvOk);
//			sVal = QString(tr("%1")).arg(val);
//		}
//		else
//		{
//			sVal = anAr->GetVariantValue(t).ToString().c_str();
//		}
//
//		QHash<QString, QVector<int> >::iterator it = this->mHashValueIndices.find(sVal);
//		if (it == this->mHashValueIndices.end())
//		{
//			// add the key value pair to the hash map
//			QVector<int> idxVec;
//			idxVec.append(clrCount);
//			this->mHashValueIndices.insert(sVal, idxVec);
//
//			// add a new row to the legend_info table
//			vtkIdType newidx = this->mLegendInfo->InsertNextBlankRow(-9);
//
//			// add the value to the index map
//			double lowup[2];
//			lowup[0] = val;
//			lowup[1] = val;
//
//			vtkDoubleArray* lowupAbstrAr = vtkDoubleArray::SafeDownCast(
//					this->mLegendInfo->GetColumnByName("range"));
//			lowupAbstrAr->SetTuple(newidx, lowup);
//
//			// generate a random triple of uchar values
//			for (int i=0; i < 3; i++)
//				rgba[i] = vtkMath::Random();
//			rgba[3] = 1;
//
//			// add the color spec to the colour map
//			vtkDoubleArray* rgbaAr = vtkDoubleArray::SafeDownCast(
//					this->mLegendInfo->GetColumnByName("rgba"));
//			rgbaAr->SetTuple(newidx, rgba);
//
//			// add the name (sVal) to the name column of the legendinfo table
//			vtkStringArray* nameAr = vtkStringArray::SafeDownCast(
//					this->mLegendInfo->GetColumnByName("name"));
//			nameAr->SetValue(newidx, sVal.toStdString().c_str());
//
//			// add the color spec to the mapper's color table
//			mLookupTable->SetTableValue(t, rgba[0], rgba[1], rgba[2], rgba[3]);
//			//			NMDebugAI( << clrCount << ": " << sVal.toStdString() << " = " << rgba[0]
//			//					<< " " << rgba[1] << " " << rgba[2] << endl);
//
//			clrCount++;
//		}
//		else
//		{
//			// add the index to the index map
//			int tabInfoIdx = this->mHashValueIndices.find(sVal).value()[0];
//			this->mHashValueIndices.find(sVal).value().append(t);
//
//			// add the colour to the real color table
//			vtkDoubleArray* dblAr = vtkDoubleArray::SafeDownCast(this->mLegendInfo->GetColumnByName("rgba"));
//			//double tmprgba[4];
//			dblAr->GetTuple(tabInfoIdx, rgba);
//
//			mLookupTable->SetTableValue(t, rgba[0], rgba[1], rgba[2]);
//		}
//	}
//
//	mNumLegendRows = mLegendInfo->GetNumberOfRows() + 2;
//	mLegendDescrField = mLegendValueField;
//
//	//// get the mapper and look whats possible
//	//vtkMapper* mapper = vtkMapper::SafeDownCast(this->mMapper);
//	//mapper->SetLookupTable(mLookupTable);
//	//mapper->Modified();
//    //
//	//emit visibilityChanged(this);
//	//emit legendChanged(this);
//
//	return;
//}


//void NMVectorLayer::mapSingleSymbol()
//{
//	NMDebugCtx(ctxNMVectorLayer, << "...");
//	if (this->mFeatureType != NMVectorLayer::NM_POLYGON_FEAT)
//	{
//		NMLayer::mapSingleSymbol();
//		NMDebugCtx(ctxNMVectorLayer, << "done!");
//		return;
//	}
//
//	vtkDataSetAttributes* dsAttr = this->mDataSet->GetAttributes(vtkDataSet::CELL);
//	vtkLongArray* nmids = vtkLongArray::SafeDownCast(dsAttr->GetArray("nm_id"));
//	long ncells = nmids->GetNumberOfTuples();
//
//	vtkUnsignedCharArray* hole = vtkUnsignedCharArray::SafeDownCast(dsAttr->GetArray("nm_hole"));
//
//	// we create a new look-up table and set the number of entries we need
//	mLookupTable = vtkSmartPointer<vtkLookupTable>::New();
//	mLookupTable->SetNumberOfTableValues(ncells);
//
//	mNumLegendRows = 2;
//	mLegendDescrField = tr("All Features");
//
//	// let's create a new legend info table
//	//this->resetLegendInfo();
//
//	// chose a random colour
//	QString sVal;
//	vtkMath::RandomSeed(QTime::currentTime().msec());
//
//	double rgba[4];
//	for (int i=0; i < 3; i++)
//		rgba[i] = vtkMath::Random();
//	rgba[3] = 1;
//
//	// cell index vector for has map
//	//QVector<int> idxVec;
//
//	// fill colour look-up table for mapper
//	for (long l=0; l < ncells; ++l)
//	{
//		// add the new cell index to the hash map
//		//idxVec.append(l);
//
//		//if (hole->GetValue(l))
//		//{
//		//	//mLookupTable->SetTableValue(l, rgba[0], rgba[1], rgba[2], 0);
//		//}
//		//else
//		{
//			mLookupTable->SetTableValue(l, rgba[0], rgba[1], rgba[2], rgba[3]);
//		}
//	}
//
//	//// fill hash map
//	//vtkIdType newidx = this->mLegendInfo->InsertNextBlankRow(-9);
//	//this->mHashValueIndices.insert(tr(""), idxVec);
//    //
//    //
//	//// fill mLegendInfoTable with corresponding infor
//	//double lowup[2];
//	//lowup[0] = -9;
//	//lowup[1] = -9;
//    //
//	//vtkDoubleArray* lowupAbstrAr = vtkDoubleArray::SafeDownCast(
//	//		this->mLegendInfo->GetColumnByName("range"));
//	//lowupAbstrAr->SetTuple(newidx, lowup);
//    //
//	//vtkStringArray* nameAr = vtkStringArray::SafeDownCast(
//	//		this->mLegendInfo->GetColumnByName("name"));
//	//nameAr->SetValue(newidx, "");
//    //
//	//vtkDoubleArray* clrs = vtkDoubleArray::SafeDownCast(
//	//		this->mLegendInfo->GetColumnByName("rgba"));
//	//clrs->SetTuple(newidx, rgba);
//
//	NMDebugCtx(ctxNMVectorLayer, << "done!");
//
//}

void NMVectorLayer::createTableView(void)
{
	if (this->mTableView != 0)
	{
		return;
		//delete this->mTableView;
		//this->mTableView = 0;
	}

	if (!this->updateAttributeTable())
		return;

	if (this->mTableModel == 0)
	{
        NMLogError(<< ctxNMVectorLayer << ": table model missing!");
		return;
	}

	this->mTableView = new NMTableView(this->mTableModel, 0);
	this->mTableView->setSelectionModel(this->mSelectionModel);

	// hide the 'hole' rows
	if (this->mFeatureType == NMVectorLayer::NM_POLYGON_FEAT)
	{
		vtkUnsignedCharArray* holes = vtkUnsignedCharArray::SafeDownCast(
				this->mAttributeTable->GetColumnByName("nm_hole"));
		QList<int> hiddenrows;
		//NMDebugAI(<< __FUNCTION__ << ": hidden rows ..." << std::endl);
		for (int row=0; row < this->mAttributeTable->GetNumberOfRows(); ++row)
		{
			if (holes->GetValue(row))
			{
				//NMDebug(<< row << " ");
				hiddenrows << row;
			}
		}
		//NMDebug(<< " --> sum=" << hiddenrows.size() << std::endl);
		this->mTableView->hideSource(hiddenrows);
		this->mTableView->hideAttribute("nm_hole");
	}
	this->mTableView->hideAttribute("nm_sel");
	this->mTableView->setTitle(tr("Attributes of ") + this->objectName());

	connect(this, SIGNAL(selectabilityChanged(bool)), mTableView, SLOT(setSelectable(bool)));
    connect(mTableView, SIGNAL(notifyLastClickedRow(long long)), this, SLOT(forwardLastClickedRowSignal(long long)));

	// connect tableview signals to layer slots
	//this->connect(this->mTableView,
	//		SIGNAL(tableDataChanged(QStringList&, QStringList&)), this,
	//		SLOT(emitAttributeTableChanged(QStringList&, QStringList&)));
	//this->connect(this->mTableView, SIGNAL(selectionChanged()), this,
	//		SLOT(updateSelectionData()));

	// connect layer signals to tableview slots
	//this->connect(this, SIGNAL(attributeTableChanged(vtkTable*)),
	//		this->mTableView, SLOT(setTable(vtkTable*)));
//	this->connect(this, SIGNAL(layerSelectionChanged(NMLayer*)),
//			this->mTableView, SLOT(updateSelection()));

}

int NMVectorLayer::updateAttributeTable(void)
{
	NMDebugCtx(ctxNMVectorLayer, << "...");

	if (mTableModel != 0)
	{
		NMDebugAI(<< "we've got a table already!" << std::endl);
		NMDebugCtx(ctxNMVectorLayer, << "done!");
		return 1;
	}

	vtkDataSetAttributes* dsa = this->mDataSet->GetAttributes(vtkDataSet::CELL);
	if (dsa == 0 || dsa->GetNumberOfArrays() == 0)
		return 0;

	disconnectTableSel();

	vtkSmartPointer<vtkTable> rawtab = vtkSmartPointer<vtkTable>::New();
	rawtab->SetRowData(dsa);

	this->mAttributeTable = rawtab;
	vtkQtEditableTableModelAdapter* tabModel;
	if (this->mTableModel == 0)
	{
		tabModel = new vtkQtEditableTableModelAdapter(mAttributeTable);
	}
	else
	{
		tabModel = qobject_cast<vtkQtEditableTableModelAdapter*>(this->mTableModel);
		tabModel->setTable(this->mAttributeTable);
	}
	tabModel->SetKeyColumnName("nm_id");

	// in any case, we create a new item selection model
	if (this->mSelectionModel == 0)
	{
		this->mSelectionModel = new NMFastTrackSelectionModel(tabModel, this);
	}
	this->mTableModel = tabModel;

	connectTableSel();
	emit legendChanged(this);

	NMDebugCtx(ctxNMVectorLayer, << "done!");
	return 1;
}

//const vtkTable* NMVectorLayer::getTable(void)
//{
//	if (this->mAttributeTable.GetPointer() == 0)
//		this->updateAttributeTable();
//
//	return this->mAttributeTable;
//}

//void NMVectorLayer::updateDataSet(QStringList& slAlteredColumns,
//		QStringList& slDeletedColumns)
//{
//	NMDebugCtx(ctxNMVectorLayer, << "...");
//
//	if (slDeletedColumns.size() == 0 && slAlteredColumns.size() == 0)
//	{
//		NMDebugAI(<< "nothing to save!" << endl);
//		NMDebugCtx(ctxNMVectorLayer, << "done!");
//		return;
//	}
//
//	vtkDataSetAttributes* dsAttr = this->mDataSet->GetAttributes(vtkDataSet::CELL);
//
//	vtkUnsignedCharArray* holeAr;
//	if (this->mFeatureType == NMVectorLayer::NM_POLYGON_FEAT)
//		holeAr = vtkUnsignedCharArray::SafeDownCast(dsAttr->GetArray("nm_hole"));
//
//
//	// delete all deleted columns
//	for (int d=0; d < slDeletedColumns.size(); ++d)
//		dsAttr->RemoveArray(slDeletedColumns.at(d).toStdString().c_str());
//
//	// add / update columns
//	for (int a=0; a < slAlteredColumns.size(); ++a)
//	{
//		vtkAbstractArray* aa = this->mAttributeTable->GetColumnByName(
//				slAlteredColumns.at(a).toStdString().c_str());
//
//		vtkSmartPointer<vtkAbstractArray> na = vtkAbstractArray::CreateArray(aa->GetDataType());
//		na->SetName(aa->GetName());
//		na->SetNumberOfComponents(1);
//		na->Allocate(holeAr->GetNumberOfTuples());
//
//		int aaCnt = 0;
//		for (int r=0; r < holeAr->GetNumberOfTuples(); ++r)
//		{
//			if (holeAr->GetValue(r))
//			{
//				na->InsertVariantValue(r, vtkVariant(0));
//			}
//			else
//			{
//				na->InsertVariantValue(r, vtkVariant(aa->GetVariantValue(aaCnt)));
//				++aaCnt;
//			}
//		}
//
//		dsAttr->AddArray(na);
//	}
//
//	slDeletedColumns.clear();
//	slAlteredColumns.clear();
//
//	//emit dataSetChanged(this);
//	//emit legendChanged(this);
//
//	NMDebugCtx(ctxNMVectorLayer, << "done!");
//}

void NMVectorLayer::writeDataSet(void)
{
	NMDebugCtx(ctxNMVectorLayer, << "...");

	QFileInfo finfo(this->mFileName);
	if (!finfo.isWritable())
	{
        NMLogError(<< ctxNMVectorLayer << ": can't write to '" << this->mFileName.toStdString() <<
				"' - check permissions!" << endl);
		return;
	}

	// save the layer's data set under the current name
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<
			vtkPolyDataWriter>::New();
	writer->SetFileName(this->mFileName.toStdString().c_str());
    writer->SetInputData(this->mDataSet);
    //writer->SetFileTypeToBinary();
    writer->SetFileTypeToASCII();
	writer->Update();

	this->mHasChanged = false;
	//emit legendChanged(this);

	NMDebugCtx(ctxNMVectorLayer, << "done!");
}

void
NMVectorLayer::setContoursVisible(bool vis)
{
    if (this->mContourActor.GetPointer() != 0)
    {
        if (vis && this->isVisible())
        {
            this->mContourActor->SetVisibility(true);
        }
        else
        {
            this->mContourActor->SetVisibility(false);
        }
    }
}

void
NMVectorLayer::setFeaturesVisible(bool vis)
{
    if (this->mActor.GetPointer() != 0)
    {
        if (vis && this->isVisible())
        {
            this->mActor->SetVisibility(true);
        }
        else
        {
            this->mActor->SetVisibility(false);
        }
    }
    mContourOnly = !vis;
}

void
NMVectorLayer::selectionChanged(const QItemSelection& newSel,
		const QItemSelection& oldSel)
{
	//const int numranges = newSel.size();
	//const int nrows = this->mAttributeTable->GetNumberOfRows();

	// create new selections
#ifdef VTK_OPENGL2
    mSelectionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#else
    mSelectionMapper = vtkSmartPointer<vtkOGRLayerMapper>::New();
#endif
	vtkSmartPointer<vtkLookupTable> clrtab = vtkSmartPointer<vtkLookupTable>::New();
	vtkSmartPointer<vtkIdList> selCellIds = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkLongArray> scalars = vtkSmartPointer<vtkLongArray>::New();

	int selcnt = 0;
	foreach(const QItemSelectionRange& range, newSel)
	{
		const int top = range.top();
		const int bottom = range.bottom();
		for (int row=top; row<=bottom; ++row)
		{
			++selcnt;
		}
	}
	selCellIds->SetNumberOfIds(selcnt);
	scalars->SetNumberOfTuples(selcnt);
	clrtab->SetNumberOfTableValues(selcnt);

    //this->printSelRanges(newSel, "incoming update selection");

	int clrcnt = 0;
	foreach(const QItemSelectionRange& range, newSel)
	{
		const int top = range.top();
		const int bottom = range.bottom();
		for (int row=top; row<=bottom; ++row)
		{
			scalars->SetValue(clrcnt, clrcnt);
			selCellIds->SetId(clrcnt, row);
            //clrtab->SetTableValue(clrcnt, 1, 0, 0);
            clrtab->SetTableValue(clrcnt, mClrSelection.redF(),
                                          mClrSelection.greenF(),
                                          mClrSelection.blueF(),
                                          mClrSelection.alphaF());
			++clrcnt;
		}
	}

	//NMDebugAI(<< "we should have " << selCellIds->GetNumberOfIds() << " extracted cells" << std::endl);

	if (this->mCellSelection.GetPointer() != 0 && mSelectionActor.GetPointer() != 0)
	{
        //NMDebugAI(<< "removed old selection" << std::endl);
		this->mRenderer->RemoveActor(mSelectionActor);
	}

	vtkSmartPointer<vtkExtractCells> extractor = vtkSmartPointer<vtkExtractCells>::New();
    extractor->SetInputData(mDataSet);
	extractor->SetCellList(selCellIds);

	vtkSmartPointer<vtkGeometryFilter> geoFilter = vtkSmartPointer<vtkGeometryFilter>::New();
	geoFilter->SetInputConnection(extractor->GetOutputPort());
	geoFilter->Update();

	mCellSelection = vtkSmartPointer<vtkPolyData>::New();

	mCellSelection->SetPoints(geoFilter->GetOutput()->GetPoints());
	mCellSelection->SetLines(geoFilter->GetOutput()->GetPolys());
	mCellSelection->GetCellData()->SetScalars(scalars);

    //	NMDebugAI(<< "we've got " << mCellSelection->GetNumberOfCells()
    //			<< " cells in selection" << std::endl);

    mSelectionMapper->SetInputData(mCellSelection);
	mSelectionMapper->SetLookupTable(clrtab);

	vtkSmartPointer<vtkActor> a = vtkSmartPointer<vtkActor>::New();
	a->SetMapper(mSelectionMapper);

	mSelectionActor = a;
	mRenderer->AddActor(a);

	// call the base class implementation to do datatype agnostic stuff
	NMLayer::selectionChanged(newSel, oldSel);

	emit visibilityChanged(this);
	emit legendChanged(this);
}

void
NMVectorLayer::updateSelectionColor()
{
    const QItemSelection& curSel = this->mSelectionModel->selection();
    vtkSmartPointer<vtkLookupTable> clrtab = vtkLookupTable::SafeDownCast(
                                                this->mSelectionMapper->GetLookupTable());

    int clrcnt = 0;
    foreach(const QItemSelectionRange& range, curSel)
    {
        const int top = range.top();
        const int bottom = range.bottom();
        for (int row=top; row<=bottom; ++row)
        {
            clrtab->SetTableValue(clrcnt, mClrSelection.redF(),
                                          mClrSelection.greenF(),
                                          mClrSelection.blueF(),
                                          mClrSelection.alphaF());
            ++clrcnt;
        }
    }

    this->mSelectionMapper->Update();
}

//void NMVectorLayer::updateSelectionData(void)
//{
//	NMDebugCtx(ctxNMVectorLayer, << "...");
//
//	vtkDataSetAttributes* dsAttr = this->mDataSet->GetAttributes(vtkDataSet::CELL);
//
//	vtkUnsignedCharArray* holeAr;
//	if (this->mFeatureType == NMVectorLayer::NM_POLYGON_FEAT)
//		holeAr = vtkUnsignedCharArray::SafeDownCast(dsAttr->GetArray("nm_hole"));
//
//	vtkDataArray* dsSel = dsAttr->GetArray("nm_sel");
//
//	vtkTable* tab = const_cast<vtkTable*>(this->mTableView->getTable());
//
//	vtkDataArray* tabSel = vtkDataArray::SafeDownCast(
//					tab->GetColumnByName("nm_sel"));
//
//	vtkLookupTable* clrTab = vtkLookupTable::SafeDownCast(
//			this->mContourMapper->GetLookupTable());
//
//	int valCellCnt = 0;
//	for (int r = 0; r < holeAr->GetNumberOfTuples(); ++r)
//	{
//		if (holeAr->GetValue(r) == 1)
//		{
//			clrTab->SetTableValue(r, 0,0,0,1);
//			continue;
//		}
//
//		dsSel->SetTuple1(r, tabSel->GetTuple1(valCellCnt));
//
//		if (tabSel->GetTuple1(valCellCnt) != 0)
//			clrTab->SetTableValue(r, 1,0,0,1);
//		else
//			clrTab->SetTableValue(r, 0,0,0,1);
//
//		++valCellCnt;
//	}
//
//	emit visibilityChanged(this);
//	emit legendChanged(this);
//
//	NMDebugCtx(ctxNMVectorLayer, << "done!");
//}

//void NMVectorLayer::updateLayerSelection(QList<long> lstCellId,
//		QList<long> lstNMId, NMLayerSelectionType seltype)
//{
//	// update the table, if present
//	if (this->mTableView != 0)
//	{
//		vtkDataArray* tabSel = vtkDataArray::SafeDownCast(
//				this->mAttributeTable->GetColumnByName("nm_sel"));
//
//		switch (seltype)
//		{
//		case NM_SEL_NEW:
//			this->mTableView->clearSelection();
//			foreach(const int &nmid, lstNMId)
//				tabSel->SetTuple1(nmid-1, 1);
//			break;
//		case NM_SEL_ADD:
//			foreach(const int &nmid, lstNMId)
//				tabSel->SetTuple1(nmid-1, 1);
//			break;
//		case NM_SEL_REMOVE:
//			foreach(const int &nmid, lstNMId)
//				tabSel->SetTuple1(nmid-1, 0);
//			break;
//		case NM_SEL_CLEAR:
//			this->mTableView->clearSelection();
//			break;
//		}
//
//		emit attributeTableChanged(this->mAttributeTable);
//	}
//
//	// update the data set and selection
//	vtkDataSetAttributes* dsAttr = this->mDataSet->GetAttributes(vtkDataSet::CELL);
//
//	vtkUnsignedCharArray* holeAr;
//	if (this->mFeatureType == NMVectorLayer::NM_POLYGON_FEAT)
//		holeAr = vtkUnsignedCharArray::SafeDownCast(dsAttr->GetArray("nm_hole"));
//	vtkDataArray* dsSel = dsAttr->GetArray("nm_sel");
//
//	vtkLookupTable* clrTab = vtkLookupTable::SafeDownCast(
//			this->mContourMapper->GetLookupTable());
//
//	switch(seltype)
//	{
//	case NM_SEL_NEW:
//		for (int r = 0; r < holeAr->GetNumberOfTuples(); ++r)
//		{
//			clrTab->SetTableValue(r, 0,0,0,1);
//			dsSel->SetTuple1(r,0);
//		}
//
//		foreach(const int &cid, lstCellId)
//		{
//			dsSel->SetTuple1(cid,1);
//			clrTab->SetTableValue(cid, 1,0,0,1);
//		}
//		break;
//	case NM_SEL_ADD:
//		foreach(const int &cid, lstCellId)
//		{
//			dsSel->SetTuple1(cid,1);
//			clrTab->SetTableValue(cid, 1,0,0,1);
//		}
//		break;
//	case NM_SEL_REMOVE:
//		foreach(const int &cid, lstCellId)
//		{
//			dsSel->SetTuple1(cid,0);
//			clrTab->SetTableValue(cid, 0,0,0,1);
//		}
//		break;
//	case NM_SEL_CLEAR:
//		for (int r = 0; r < holeAr->GetNumberOfTuples(); ++r)
//		{
//			clrTab->SetTableValue(r, 0,0,0,1);
//			dsSel->SetTuple1(r,0);
//		}
//		break;
//	}
//
//	emit dataSetChanged(this);
//	emit legendChanged(this);
//	emit visibilityChanged(this);
//}




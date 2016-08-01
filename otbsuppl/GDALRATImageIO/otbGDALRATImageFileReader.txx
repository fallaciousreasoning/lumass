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
/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#ifndef __otbGDALRATImageFileReader_txx
#define __otbGDALRATImageFileReader_txx
#include "otbGDALRATImageFileReader.h"
#include "otbGDALRATImageIO.h"

//#include "itkMetaDataObject.h"
//
//#include "otbMacro.h"
//#include "otbSystem.h"
//#include "itkImageIOBase.h"
//#include "otbImageIOFactory.h"
//#include "otbImageKeywordlist.h"
//#include "otbMetaDataKey.h"
//
//#include "imaging/ossimImageHandlerRegistry.h"
//#include "imaging/ossimImageHandler.h"
//#include "init/ossimInit.h"
//#include "base/ossimKeywordlist.h"
//#include "projection/ossimProjection.h"
//#include "projection/ossimProjectionFactoryRegistry.h"
//#include "ossim/ossimPluginProjectionFactory.h"
//
//#include "projection/ossimTileMapModel.h"
//
//#include <itksys/SystemTools.hxx>
//#include <fstream>
//
#include "otbTileMapImageIO.h" //FIXME find a better way
#include "otbCurlHelper.h"


namespace otb
{

template<class TOutputImage>
GDALRATImageFileReader<TOutputImage>::GDALRATImageFileReader() :
		otb::ImageFileReader<TOutputImage>(), m_DatasetNumber(0)
{
	m_Curl = CurlHelper::New();
    m_OverviewIdx = -1;
    m_UseUserLargestPossibleRegion = false;
    m_RAT = 0;
}

template<class TOutputImage>
GDALRATImageFileReader<TOutputImage>::~GDALRATImageFileReader()
{
}

template<class TOutputImage>
void GDALRATImageFileReader<TOutputImage>::PrintSelf(std::ostream& os,
		itk::Indent indent) const
{
	Superclass::PrintSelf(os, indent);
	otb::GDALRATImageFileReader<TOutputImage>* pr =
			const_cast<otb::GDALRATImageFileReader<TOutputImage>* >(this);

	if (pr->GetImageIO())
	{
		os << indent << "ImageIO: \n";
		pr->GetImageIO()->Print(os, indent.GetNextIndent());
	}
	else
	{
		os << indent << "ImageIO: (null)" << "\n";
	}

	//os << indent << "UserSpecifiedImageIO flag: " << this->m_UserSpecifiedImageIO << "\n";
	os << indent << "GetFileName(): " << pr->GetFileName() << "\n";

	if (this->m_RAT.IsNotNull())
		this->m_RAT->PrintStructure(os, indent);
}

template<class TOutputImage>
void GDALRATImageFileReader<TOutputImage>::GenerateOutputInformation(void)
{

	typename TOutputImage::Pointer output = this->GetOutput();

	itkDebugMacro(
			<< "Reading file for GenerateOutputInformation()" << this->GetFileName());

	// Check to see if we can read the file given the name or prefix
	//
	if (this->GetFileName() == "")
	{
		throw ImageFileReaderException(__FILE__, __LINE__,
				"FileName must be specified");
	}

	// Find real image file name
	// !!!!  Update FileName
	std::string lFileName;
	bool found = GetGdalReadImageFileName(this->GetFileName(), lFileName);
	if (found == false)
	{
		otbMsgDebugMacro(
				<< "Filename was NOT unknown. May be recognized by a Image factory ! ");
	}
	// Update FileName
	this->SetFileName(lFileName);

	std::string lFileNameOssimKeywordlist = this->GetFileName();

	// Test if the file exists and if it can be opened.
	// An exception will be thrown otherwise.
	// We catch the exception because some ImageIO's may not actually
	// open a file. Still reports file error if no ImageIO is loaded.

	try
	{
		m_ExceptionMessage = "";
		this->TestFileExistanceAndReadability();
	}
	catch (itk::ExceptionObject & err)
	{
		m_ExceptionMessage = err.GetDescription();
	}

	GDALRATImageIO::Pointer gio = dynamic_cast<GDALRATImageIO*>(this->GetImageIO());
	if (gio.IsNull()) //try creating via factory
	{
		// NEW in the GDALRATImageFileRader compared to otb::ImageFileReader<...>
		// since the ImageIOFactory mechanism relies on the file extension, we
		// have to manually check, whether the current image can be read by GDAL and if so,
        //		otb::ImageIOBase::Pointer aio = ImageIOFactory::CreateImageIO(
        //				this->GetFileName(), otb::ImageIOFactory::ReadMode);
        otb::GDALRATImageIO::Pointer gio = otb::GDALRATImageIO::New();
        //gio = dynamic_cast<GDALRATImageIO*>(aio.GetPointer());
		if (gio.IsNotNull())
		{
			this->SetImageIO(gio);
            gio->SetOverviewIdx(m_OverviewIdx);
			gio->SetRATSupport(true);
			this->m_RAT = gio->ReadRAT(1);
		}
	}

    // The whole point of this class is to use the GDALRATImageIO, so let's disable this
    //	if (this->GetImageIO() == 0)
    //	{
    //		this->Print(std::cerr);
    //		ImageFileReaderException e(__FILE__, __LINE__);
    //		std::ostringstream msg;
    //		msg << " Could not create IO object for file "
    //				<< this->GetFileName() << std::endl;
    //		msg << "  Tried to create one of the following:" << std::endl;
    //		std::list<itk::LightObject::Pointer> allobjects =
    //				itk::ObjectFactoryBase::CreateAllInstance("otbImageIOBase");
    //		for (std::list<itk::LightObject::Pointer>::iterator i =
    //				allobjects.begin(); i != allobjects.end(); ++i)
    //		{
    //			otb::ImageIOBase* io =
    //					dynamic_cast<otb::ImageIOBase*>(i->GetPointer());
    //			msg << "    " << io->GetNameOfClass() << std::endl;
    //		}
    //		msg << "  You probably failed to set a file suffix, or" << std::endl;
    //		msg << "    set the suffix to an unsupported type." << std::endl;
    //		e.SetDescription(msg.str().c_str());
    //		throw e;
    //		return;
    //	}

	// Special actions for the gdal image IO
	if (strcmp(this->GetImageIO()->GetNameOfClass(), "GDALRATImageIO") == 0)
	{
		typename GDALRATImageIO::Pointer imageIO =
				dynamic_cast<GDALRATImageIO*>(this->GetImageIO());

		// Hint the IO whether the OTB image type takes complex pixels
		// this will determine the strategy to fill up a vector image
		OutputImagePixelType dummy;
		imageIO->SetIsComplex(PixelIsComplex(dummy));

		// VectorImage ??
		if (strcmp(output->GetNameOfClass(), "VectorImage") == 0)
			imageIO->SetIsVectorImage(true);
		else
			imageIO->SetIsVectorImage(false);

		// Pass the dataset number (used for hdf files for example)
		imageIO->SetDatasetNumber(m_DatasetNumber);
	}

	// Got to allocate space for the image. Determine the characteristics of
	// the image.
	//
	this->GetImageIO()->SetFileName(this->GetFileName());
	this->GetImageIO()->ReadImageInformation();
	// Initialization du nombre de Composante par pixel
// THOMAS ceci n'est pas dans ITK !!
//  output->SetNumberOfComponentsPerPixel(this->GetImageIO()->GetNumberOfComponents());

	SizeType dimSize;
	double spacing[TOutputImage::ImageDimension];
	double origin[TOutputImage::ImageDimension];
	typename TOutputImage::DirectionType direction;
	std::vector<double> axis;

	for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
	{
		if (i < this->GetImageIO()->GetNumberOfDimensions())
		{
			dimSize[i] = this->GetImageIO()->GetDimensions(i);
			spacing[i] = this->GetImageIO()->GetSpacing(i);
			origin[i] = this->GetImageIO()->GetOrigin(i);
// Please note: direction cosines are stored as columns of the
// direction matrix
			axis = this->GetImageIO()->GetDirection(i);
			for (unsigned j = 0; j < TOutputImage::ImageDimension; ++j)
			{
				if (j < this->GetImageIO()->GetNumberOfDimensions())
				{
					direction[j][i] = axis[j];
				}
				else
				{
					direction[j][i] = 0.0;
				}
			}
		}
		else
		{
			// Number of dimensions in the output is more than number of dimensions
			// in the ImageIO object (the file).  Use default values for the size,
			// spacing, origin and direction for the final (degenerate) dimensions.
			dimSize[i] = 1;
			spacing[i] = 1.0;
			origin[i] = 0.0;
			for (unsigned j = 0; j < TOutputImage::ImageDimension; ++j)
			{
				if (i == j)
				{
					direction[j][i] = 1.0;
				}
				else
				{
					direction[j][i] = 0.0;
				}
			}
		}
	}

	output->SetSpacing(spacing); // Set the image spacing
	output->SetOrigin(origin); // Set the image origin
	output->SetDirection(direction); // Set the image direction cosines

	// Update otb Keywordlist
	ImageKeywordlist otb_kwl = ReadGeometryFromImage(lFileNameOssimKeywordlist);

	// Pass the depth parameter from the tilemap around
	if (strcmp(this->GetImageIO()->GetNameOfClass(), "TileMapImageIO") == 0)
	{
		typename TileMapImageIO::Pointer imageIO =
				dynamic_cast<TileMapImageIO*>(this->GetImageIO());
		std::ostringstream depth;
		depth << imageIO->GetDepth();
		otb_kwl.AddKey("depth", depth.str());
	}

	otbMsgDevMacro(<< otb_kwl);

	// Update itk MetaData Dictionary
	itk::MetaDataDictionary& dict = this->GetImageIO()->GetMetaDataDictionary();

	// Don't add an empty ossim keyword list
	if (otb_kwl.GetSize() != 0)
	{
		itk::EncapsulateMetaData<ImageKeywordlist>(dict,
				MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
	}
	else
	{
		//
		// For image with world file, we have a non-null GeoTransform, an empty kwl, but no projection ref.
		// Decision made here : if the keywordlist is empty, and the geotransform is not the identity,
		// then assume the file is in WGS84
		//
		std::string projRef;
		itk::ExposeMetaData(dict, MetaDataKey::ProjectionRefKey, projRef);

		const double Epsilon = 1.0E-12;
		if (projRef.empty() && vcl_abs(origin[0]) > Epsilon
		&& vcl_abs(origin[1]) > Epsilon
		&& vcl_abs(spacing[0] - 1) > Epsilon
		&& vcl_abs(spacing[1] - 1) > Epsilon)
		{
			std::string wgs84ProjRef =
"GEOGCS[\"GCS_WGS_1984\", DATUM[\"D_WGS_1984\", SPHEROID[\"WGS_1984\", 6378137, 298.257223563]],"
              "PRIMEM[\"Greenwich\", 0], UNIT[\"Degree\", 0.017453292519943295]]";

      itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, wgs84ProjRef);
      }	}

	//Copy MetaDataDictionary from instantiated reader to output image.
	output->SetMetaDataDictionary(this->GetImageIO()->GetMetaDataDictionary());
	this->SetMetaDataDictionary(this->GetImageIO()->GetMetaDataDictionary());

	typedef typename TOutputImage::IndexType IndexType;

	IndexType start;
	start.Fill(0);

	ImageRegionType region;
    if (m_UseUserLargestPossibleRegion)
    {
        // NOTE: the user is responsible for specifying a
        // region which is a sub region of the actual LPR!!
        region.SetSize(m_UserLargestPossibleRegion.GetSize());
        region.SetIndex(m_UserLargestPossibleRegion.GetIndex());
    }
    else
    {
        region.SetSize(dimSize);
        region.SetIndex(start);
    }

// THOMAS : ajout
// If a VectorImage, this requires us to set the
// VectorLength before allocate
	if (strcmp(output->GetNameOfClass(), "VectorImage") == 0)
	{
		typedef typename TOutputImage::AccessorFunctorType AccessorFunctorType;
		AccessorFunctorType::SetVectorLength(output,
				this->GetImageIO()->GetNumberOfComponents());
	}

    output->SetLargestPossibleRegion(region);
}

template<class TOutputImage>
void GDALRATImageFileReader<TOutputImage>::SetOverviewIdx(int ovvidx)
{
    if (ovvidx != this->m_OverviewIdx)
    {
        this->m_OverviewIdx = ovvidx;
        otb::GDALRATImageIO* gio = static_cast<otb::GDALRATImageIO*>(
                    this->GetImageIO());
        if (gio)
        {
            gio->SetOverviewIdx(ovvidx);
            this->Modified();
        }
    }
}

template<class TOutputImage>
bool GDALRATImageFileReader<TOutputImage>::GetGdalReadImageFileName(
		const std::string& filename, std::string& GdalFileName)
{
	std::vector<std::string> listFileSearch;
	listFileSearch.push_back("DAT_01.001");
	listFileSearch.push_back("dat_01.001"); // RADARSAT or SAR_ERS2
	listFileSearch.push_back("IMAGERY.TIF");
	listFileSearch.push_back("imagery.tif"); //For format SPOT5TIF
// Not recognized as a supported file format by GDAL.
//        listFileSearch.push_back("IMAGERY.BIL"); listFileSearch.push_back("imagery.bil"); //For format SPOT5BIL
	listFileSearch.push_back("IMAG_01.DAT");
	listFileSearch.push_back("imag_01.dat"); //For format SPOT4

	std::string str_FileName;
	bool fic_trouve(false);

	// Si c'est un repertoire, on regarde le contenu pour voir si c'est pas du RADARSAT, ERS
	std::vector<std::string> listFileFind;
	listFileFind = System::Readdir(filename);
	if (listFileFind.empty() == false)
	{
		unsigned int cpt(0);
		while ((cpt < listFileFind.size()) && (fic_trouve == false))
		{
			str_FileName = std::string(listFileFind[cpt]);
			for (unsigned int i = 0; i < listFileSearch.size(); ++i)
			{
				if (str_FileName.compare(listFileSearch[i]) == 0)
				{
					GdalFileName = std::string(filename) + str_FileName; //listFileSearch[i];
					fic_trouve = true;
				}
			}
			cpt++;
		}
	}
	else
	{
		std::string strFileName(filename);

        std::string extension = itksys::SystemTools::GetFilenameLastExtension(strFileName);
        extension = extension.substr(1, extension.size()-1);

		if ((extension == "HDR") || (extension == "hdr"))
		{
			//Supprime l'extension
			GdalFileName = System::GetRootName(strFileName);
		}

		else
		{
			// Sinon le filename est le nom du fichier a ouvrir
			GdalFileName = std::string(filename);
		}
		fic_trouve = true;
	} otbMsgDevMacro(<< "lFileNameGdal : " << GdalFileName.c_str()); otbMsgDevMacro(<< "fic_trouve : " << fic_trouve);
	return (fic_trouve);
}

//template <class TOutputImage>
//ImageKeywordlist
//GDALRATImageFileReader<TOutputImage>
//::GenerateKeywordList(const std::string& filename)
//{
//  // Trying to read ossim MetaData
//  bool             hasMetaData = false;
//  ossimKeywordlist geom_kwl; // = new ossimKeywordlist();
//
//  // Test the plugins factory
//  /** Before, the pluginfactory was tested if the ossim one returned false.
//      But in the case TSX, the images tif were considered as ossimQuickbirdTiffTileSource
//      thus a TSX tif image wasn't read with TSX Model. We don't use the ossimRegisteryFactory
//      because the default include factory contains ossimQuickbirdTiffTileSource. */
//  ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()
//                                 ->createProjection(ossimFilename(filename.c_str()), 0);
//
//  if (!projection)
//    {
//    otbMsgDevMacro(<< "OSSIM Instantiate projection FAILED ! ");
//    }
//  else
//    {
//    otbMsgDevMacro(<< "OSSIM Instantiate projection SUCCESS ! ");
//    hasMetaData = projection->saveState(geom_kwl);
//
//    // Free memory
//    delete projection;
//    }
//
//  if (!hasMetaData)
//    {
//    // Add the radar factory
//    // ossimImageHandlerRegistry::instance()->addFactory(ossimImageHandlerSarFactory::instance());
//
//    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()
//                                 ->open(ossimFilename(filename.c_str()));
//    if (!handler)
//      {
//      otbMsgDevMacro(<< "OSSIM Open Image FAILED ! ");
//      }
//
//    else
//      {
//      otbMsgDevMacro(<< "OSSIM Open Image SUCCESS ! ");
//      //     hasMetaData = handler->getImageGeometry(geom_kwl);
//      ossimProjection* projection = handler->getImageGeometry()->getProjection();
//
//      if (projection)
//        {
//        if (projection->getClassName() == "ossimTileMapModel")
//          {
//          //FIXME find a better way to do that
//          //we need to pass the depth information which in on the IO to the projection
//          //to be handle throught the kwl
//          typename TileMapImageIO::Pointer imageIO = dynamic_cast<TileMapImageIO*>(this->GetImageIO());
//          if (imageIO.IsNotNull())
//            {
//            dynamic_cast<ossimplugins::ossimTileMapModel*>(projection)->setDepth(imageIO->GetDepth());
//            }
//          }
//        hasMetaData = projection->saveState(geom_kwl);
////             delete projection; //FIXME find out where this should occur
//        }
//      }
//    // Free memory
//    delete handler;
//    }
//
//  if (!hasMetaData)
//    {
//    otbMsgDevMacro(<< "OSSIM MetaData not present ! ");
//    }
//  else
//    {
//    otbMsgDevMacro(<< "OSSIM MetaData present ! ");
//    otbMsgDevMacro(<< geom_kwl);
//    }
//
//  // TODO: check if the empty case is handled properly
//  ImageKeywordlist otb_kwl;
//  otb_kwl.SetKeywordlist(geom_kwl);
//  return otb_kwl;
//
//}

template<class TOutputImage>
void GDALRATImageFileReader<TOutputImage>::TestFileExistanceAndReadability()
{
	// Test if the file a server name
	if (this->GetFileName()[0] == 'h' && this->GetFileName()[1] == 't'
			&& this->GetFileName()[2] == 't' && this->GetFileName()[3] == 'p')
	{
		int res = m_Curl->TestUrlAvailability(this->GetFileName());
		if (res != 0 && res != 63) // 63 stands for filesize exceed
		{
			ImageFileReaderException e(__FILE__, __LINE__);
            std::ostringstream msg;
			msg
					<< "File name is an http address, but curl fails to connect to it "
					<< std::endl << "Filename = " << this->GetFileName()
					<< std::endl << "Curl error code = " << res << std::endl;
			e.SetDescription(msg.str().c_str());
			throw e;
		}
		return;
	}

	// Test if we have an hdf file with dataset spec
	std::string realfile(this->GetFileName());
	unsigned int datasetNum;
	if (System::ParseHdfFileName(this->GetFileName(), realfile, datasetNum))
	{
		otbMsgDevMacro(<< "HDF name with dataset specification detected"); otbMsgDevMacro(<< " - " << realfile); otbMsgDevMacro(<< " - " << datasetNum);
		this->SetFileName(realfile);
		m_DatasetNumber = datasetNum;
	}

	// Test if the file exists.
	if (!itksys::SystemTools::FileExists(this->GetFileName()))
	{
		ImageFileReaderException e(__FILE__, __LINE__);
        std::ostringstream msg;
		msg << "The file doesn't exist. " << std::endl << "Filename = "
				<< this->GetFileName() << std::endl;
		e.SetDescription(msg.str().c_str());
		throw e;
		return;
	}

	// Test if the file can be open for reading access.
	//Only if GetFileName() specify a filename (not a dirname)
    if (itksys::SystemTools::FileExists(this->GetFileName(), true) == true)
	{
		std::ifstream readTester;
		readTester.open(this->GetFileName());
		if (readTester.fail())
		{
			readTester.close();
            std::ostringstream msg;
			msg << "The file couldn't be opened for reading. " << std::endl
					<< "Filename: " << this->GetFileName() << std::endl;
			ImageFileReaderException e(__FILE__, __LINE__, msg.str().c_str(),
					ITK_LOCATION);
			throw e;
			return;

		}
		readTester.close();
	}
}

template<class TOutputImage>
AttributeTable::Pointer GDALRATImageFileReader<TOutputImage>::GetAttributeTable(
		int iBand)
{
	if (this->GetImageIO() == 0)
		this->GenerateOutputInformation();

	// let's see if we've got already the right table
	if (this->m_RAT.IsNotNull())
	{
		if (this->m_RAT->GetBandNumber() == iBand)
			return this->m_RAT;
	}
	else
	{
		GDALRATImageIO* gio =
				dynamic_cast<GDALRATImageIO*>(this->GetImageIO());
		if (gio != 0)
		{
			this->m_RAT = gio->ReadRAT(iBand);
			return this->m_RAT;
		}
	}

	return 0;
}

} //namespace otb

#endif

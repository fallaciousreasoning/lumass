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

#ifndef __otbStreamingRATImageFileWriter_txx
#define __otbStreamingRATImageFileWriter_txx

#include "nmlog.h"
#define ctxSIFR "StreamingRATImageFileWriter"

#include "otbStreamingRATImageFileWriter.h"
#include "itkImageFileWriter.h"

#include "itkObjectFactoryBase.h"

#include "itkImageRegionMultidimensionalSplitter.h"
#include "otbImageIOFactory.h"
#include "otbGDALRATImageIO.h"

#ifdef BUILD_RASSUPPORT
	#include "otbRasdamanImageIO.h"
#endif

#include "itkMetaDataObject.h"
#include "otbImageKeywordlist.h"
#include "otbMetaDataKey.h"

#include "otbConfigure.h"

#include "otbNumberOfDivisionsStrippedStreamingManager.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbTileDimensionTiledStreamingManager.h"
#include "otbRAMDrivenTiledStreamingManager.h"


namespace otb
{

/**
 *
 */
template <class TInputImage>
StreamingRATImageFileWriter<TInputImage>
::StreamingRATImageFileWriter()
  : m_WriteGeomFile(false)
{
  m_UserSpecifiedIORegion = true;
  m_FactorySpecifiedImageIO = false;

  // By default, we use tiled streaming, with automatic tile size
  // We don't set any parameter, so the memory size is retrieved from
  // the OTB configuration options
  // this->SetAutomaticTiledStreaming();
  this->SetAutomaticStrippedStreaming(512);

  m_ResamplingType = "NEAREST";
  m_StreamingMethod = "STRIPPED";
  m_StreamingSize = 512;

  m_UseCompression = true;
  m_RATHaveBeenWritten = false;
  m_UseForcedLPR = false;
  m_UseUpdateRegion = false;
  m_UpdateMode = false;
  m_WriteImage = true;
#ifdef BUILD_RASSUPPORT
  this->mRasconn = 0;
#endif
}

/**
 *
 */
template <class TInputImage>
StreamingRATImageFileWriter<TInputImage>
::~StreamingRATImageFileWriter()
{
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsStrippedStreamingManager<TInputImage> NumberOfDivisionsStrippedStreamingManagerType;
  typename NumberOfDivisionsStrippedStreamingManagerType::Pointer streamingManager = NumberOfDivisionsStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsTiledStreamingManager<TInputImage> NumberOfDivisionsTiledStreamingManagerType;
  typename NumberOfDivisionsTiledStreamingManagerType::Pointer streamingManager = NumberOfDivisionsTiledStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
{
  typedef NumberOfLinesStrippedStreamingManager<TInputImage> NumberOfLinesStrippedStreamingManagerType;
  typename NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager = NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetAutomaticStrippedStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenStrippedStreamingManager<TInputImage> RAMDrivenStrippedStreamingManagerType;
  typename RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetTileDimensionTiledStreaming(unsigned int tileDimension)
{
  typedef TileDimensionTiledStreamingManager<TInputImage> TileDimensionTiledStreamingManagerType;
  typename TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();
  streamingManager->SetTileDimension(tileDimension);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetAutomaticTiledStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenTiledStreamingManager<TInputImage> RAMDrivenTiledStreamingManagerType;
  typename RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

/**
 *
 */
template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetBufferMemorySize(unsigned long memory_size_divisions)
{
  itkWarningMacro("SetNumberOfDivisionsStrippedStreaming is DEPRECATED. "
                  "Use one of SetNumberOfLinesStrippedStreaming, "
                  "SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming, "
                  "or SetAutomaticTiledStreaming." );

  unsigned int ram = static_cast<unsigned int>(memory_size_divisions / 1024 / 1024);
  this->SetAutomaticStrippedStreaming(ram);
}

/**
 *
 */
template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetBufferNumberOfLinesDivisions(unsigned long nb_lines_divisions)
{
  itkWarningMacro("SetBufferNumberOfLinesDivisions is DEPRECATED. "
                  "Use one of SetNumberOfLinesStrippedStreaming, "
                  "SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming, "
                  "or SetAutomaticTiledStreaming." );

  unsigned int nb_lines = static_cast<unsigned int>(nb_lines_divisions);
  this->SetNumberOfLinesStrippedStreaming(nb_lines);
}

/**
 *
 */
template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetNumberOfStreamDivisions(unsigned long nb_divisions)
{
  itkWarningMacro("SetNumberOfStreamDivisions is DEPRECATED. "
                  "Use one of SetNumberOfLinesStrippedStreaming, "
                  "SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming, "
                  "or SetAutomaticTiledStreaming." );
  unsigned int nb_div = static_cast<unsigned int>(nb_divisions);
  this->SetNumberOfDivisionsStrippedStreaming(nb_div);
}

/**
 *
 */
template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetAutomaticNumberOfStreamDivisions(void)
{
  itkWarningMacro("SetAutomaticNumberOfStreamDivisions is DEPRECATED. "
                  "Use one of SetNumberOfLinesStrippedStreaming, "
                  "SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming, "
                  "or SetAutomaticTiledStreaming." );
  this->SetAutomaticStrippedStreaming(0);
}

/**
 *
 */
template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetTilingStreamDivisions(void)
{
  itkWarningMacro("SetTilingStreamDivisions is DEPRECATED. "
                  "Use one of SetNumberOfLinesStrippedStreaming, "
                  "SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming, "
                  "or SetAutomaticTiledStreaming." );
  this->SetAutomaticTiledStreaming(0);
}

template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetTilingStreamDivisions(unsigned long nb_divisions)
{
  itkWarningMacro("SetTilingStreamDivisions is DEPRECATED. "
                  "Use one of SetNumberOfLinesStrippedStreaming, "
                  "SetAutomaticStrippedStreaming, SetTileDimensionTiledStreaming, "
                  "or SetAutomaticTiledStreaming." );
  unsigned int tileDim = static_cast<unsigned int>(nb_divisions);
  this->SetNumberOfDivisionsTiledStreaming(nb_divisions);
}

/**
 *
 */
template <class TInputImage>
unsigned long
StreamingRATImageFileWriter<TInputImage>
::GetNumberOfStreamDivisions(void)
{
  return m_StreamingManager->GetNumberOfSplits();
}

/**
 *
 */
template <class TInputImage>
std::string
StreamingRATImageFileWriter<TInputImage>
::GetMethodUseToCalculateNumberOfStreamDivisions(void)
{
  itkWarningMacro("GetMethodUseToCalculateNumberOfStreamDivisions is DEPRECATED");
  return "NOT-IMPLEMENTED";
}

/**
 *
 */
template <class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "File Name: "
     << (m_FileName.data() ? m_FileName.data() : "(none)") << std::endl;

  os << indent << "Image IO: ";
  if (m_ImageIO.IsNull())
    {
    os << "(none)\n";
    }
  else
    {
    os << m_ImageIO << "\n";
    }

  os << indent << "IO Region: " << m_IORegion << "\n";

  if (m_UseCompression)
    {
    os << indent << "Compression: On\n";
    }
  else
    {
    os << indent << "Compression: Off\n";
    }

  if (m_UseInputMetaDataDictionary)
    {
    os << indent << "UseInputMetaDataDictionary: On\n";
    }
  else
    {
    os << indent << "UseInputMetaDataDictionary: Off\n";
    }

  if (m_FactorySpecifiedImageIO)
    {
    os << indent << "FactorySpecifiedmageIO: On\n";
    }
  else
    {
    os << indent << "FactorySpecifiedmageIO: Off\n";
    }
}

//---------------------------------------------------------
template<class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetIORegion(const itk::ImageIORegion& region)
{
  itkDebugMacro("setting IORegion to " << region);
  if (m_IORegion != region)
    {
    m_IORegion = region;
    this->Modified();
    m_UserSpecifiedIORegion = true;
    }
}

template<class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::EnlargeOutputRequestedRegion(itk::DataObject* output)
{
	Superclass::EnlargeOutputRequestedRegion(output);
	OutputImageType* out = static_cast<OutputImageType*>(output);

	if (m_UpdateMode && m_UseForcedLPR)
	{
		itk::ImageRegion<TInputImage::ImageDimension> outr;
		for (unsigned int d=0; d < TInputImage::ImageDimension; ++d)
		{
			outr.SetIndex(d, m_ForcedLargestPossibleRegion.GetIndex()[d]);
			outr.SetSize(d, m_ForcedLargestPossibleRegion.GetSize()[d]);
		}
		out->SetLargestPossibleRegion(outr);
	}
}

template<class TInputImage>
void StreamingRATImageFileWriter<TInputImage>
::GenerateInputRequestedRegion()
 {
    //NMDebugCtx(ctxSIFR, << "...");
    Superclass::GenerateInputRequestedRegion();

    InputImageType * inputPtr = const_cast<InputImageType*>(this->GetInput());

    if(!inputPtr)
    {
        return;
    }
    typename InputImageType::RegionType lregion = inputPtr->GetLargestPossibleRegion();

    typename InputImageType::SizeType rsize;
    rsize.Fill(0);
    lregion.SetSize(rsize);

    inputPtr->SetRequestedRegion(lregion);
}

template<class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetForcedLargestPossibleRegion(const itk::ImageIORegion& forcedReg)
{
	if (m_ForcedLargestPossibleRegion != forcedReg)
	{
		m_ForcedLargestPossibleRegion = forcedReg;
		this->m_UseForcedLPR = true;
		this->Modified();
	}
}

template<class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::SetUpdateRegion(const itk::ImageIORegion& updateRegion)
{
	if ((updateRegion.GetIndex()[0] + updateRegion.GetSize()[0]) <= m_ForcedLargestPossibleRegion.GetSize()[0] &&
		(updateRegion.GetIndex()[1] + updateRegion.GetSize()[1]) <= m_ForcedLargestPossibleRegion.GetSize()[1]     )
	{
		this->m_UpdateRegion = updateRegion;
		this->m_UseUpdateRegion = true;
		this->Modified();
	}
	else
	{
        NMProcWarn(<< "The provided update region does not fit into the configured 'forced largest possible region'!");
		this->m_UseUpdateRegion = false;
	}
}

#ifdef BUILD_RASSUPPORT
template<class TInputImage>
void StreamingRATImageFileWriter<TInputImage>
::SetRasdamanConnector(RasdamanConnector* rascon)
 {
	if (rascon)
	{
		this->mRasconn = rascon;
		otb::RasdamanImageIO::Pointer rio = otb::RasdamanImageIO::New();
		rio->setRasdamanConnector(rascon);
		this->m_ImageIO = rio;
	}
	else
	{
		this->mRasconn = 0;
		this->m_ImageIO = NULL;
	}
 }
#endif

/**
 *
 */
template<class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::UpdateOutputData(itk::DataObject *itkNotUsed(output))
{
    //    NMDebugCtx(ctxSIFR, << "...");

  unsigned int idx;
  /**
   * prevent chasing our tail
   */
  if (this->m_Updating)
    {
    return;
    }

  /**
   * Prepare all the outputs. This may deallocate previous bulk data.
   */
  this->PrepareOutputs();

  /**
   * Make sure we have the necessary inputs
   */
  unsigned int ninputs = this->GetNumberOfValidRequiredInputs();
  if (ninputs < this->GetNumberOfRequiredInputs())
    {
    itkExceptionMacro(<< "At least " << static_cast<unsigned int>(
                        this->GetNumberOfRequiredInputs()) << " inputs are required but only " << ninputs <<
                      " are specified.");
    return;
    }
  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);
  this->m_Updating = true;
  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent(itk::StartEvent());

  /**
   * Allocate the output buffer.
   */
  OutputImagePointer    outputPtr = this->GetOutput(0);
  OutputImageRegionType outputRegion = this->GetOutput(0)->GetLargestPossibleRegion();


  /** Prepare ImageIO  : create ImageFactory */

  if (m_FileName == "")
    {
    // Make sure that we can write the file given the name
    itkExceptionMacro(<< "No filename was specified");
    }

  if (m_ImageIO.IsNotNull())
  {
	  m_ImageIO->SetFileName(m_FileName.c_str());
  }

  if (m_ImageIO.IsNull())
    {
	  // if the image io hasn't been set, we're using the GDALRATImageIO by default;
	  GDALRATImageIO::Pointer gio = GDALRATImageIO::New();
      if (gio.IsNull())
      {
          itkExceptionMacro(<< "Failed to create instance of GDALRATImageIO");
      }

	  gio->SetFileName(this->m_FileName);
	  this->m_ImageIO = gio;

    }
  /** End of Prepare ImageIO  : create ImageFactory */


  /** set writer and imageIO output information */
  GDALRATImageIO* gio = dynamic_cast<otb::GDALRATImageIO*>(m_ImageIO.GetPointer());
#ifdef BUILD_RASSUPPORT
  RasdamanImageIO* rio = dynamic_cast<otb::RasdamanImageIO*>(m_ImageIO.GetPointer());
#endif

  if (m_InputRAT.IsNotNull())
  {
	  if (gio != 0)
	  {
		  gio->setRasterAttributeTable(m_InputRAT, 1);
	  }
#ifdef BUILD_RASSUPPORT
	  else if (rio != 0)
	  {
		  rio->setRasterAttributeTable(m_InputRAT, 1);
	  }
#endif
  }


  if (m_UpdateMode)
  {
	  if (gio != 0)
		  gio->SetImageUpdateMode(true);
#ifdef BUILD_RASSUPPORT
	  else if (rio != 0)
		  rio->SetImageUpdateMode(true);
#endif
  }

  /* in case we want to make the image bigger than the we've currently data for
   * (e.g. for externally driven sequential writing with intertwined reading),
   */
  if (m_UseForcedLPR)
  {
	  if (gio != 0)
		  gio->SetForcedLPR(m_ForcedLargestPossibleRegion);
#ifdef BUILD_RASSUPPORT
	  else if (rio != 0)
		  rio->SetForcedLPR(m_ForcedLargestPossibleRegion);
#endif
  }

  // in case the user specified an explicit update region for externally controlled
  // streaming, we set this as the outputRegion to allow for streaming over this region
  if (m_UpdateMode && m_UseUpdateRegion)
  {
	  for (unsigned int d=0; d < TInputImage::ImageDimension; ++d)
	  {
		  outputRegion.SetIndex(d, m_UpdateRegion.GetIndex()[d]);
		  outputRegion.SetSize(d, m_UpdateRegion.GetSize()[d]);
	  }
  }

  /**
   * Grab the input
   */
  InputImagePointer inputPtr =
    const_cast<InputImageType *>(this->GetInput(0));

  /**
   * Set the user's streaming preferences
   */
  if (m_StreamingMethod.compare("STRIPPED") == 0)
  {
      this->SetAutomaticStrippedStreaming(m_StreamingSize);
  }
  else
  {
      this->SetAutomaticTiledStreaming(m_StreamingSize);
  }


  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfDivisionsStrippedStreaming()
   * and what the Splitter thinks is a reasonable value.
   */

  /** Control if the ImageIO is CanStreamWrite*/
  if (m_ImageIO->CanStreamWrite() == false || InputImageDimension == 1)
  {
      otbWarningMacro(
                  << "ImageIO doesn't support streaming, or we've got a 1D image!");
      this->SetNumberOfDivisionsStrippedStreaming(1);
  }
  else if (inputPtr->GetBufferedRegion() == inputPtr->GetLargestPossibleRegion())
  {
      otbMsgDevMacro(<< "Requested region is the largest possible region, "
                     << "let's honour that, he preceding filter might not have a choice!");
      this->SetNumberOfDivisionsStrippedStreaming(1);
  }
  else if (m_StreamingMethod.compare("NO_STREAMING") == 0)
  {
      this->SetNumberOfDivisionsStrippedStreaming(1);
  }
  m_StreamingManager->PrepareStreaming(inputPtr, outputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();

  // no point in chopping up the image, if we're not
  // intrested in it (and only want to write the table)
  if (!m_WriteImage)
  {
      m_NumberOfDivisions = 1;
  }
  otbMsgDebugMacro(<< "Number Of Stream Divisions : " << m_NumberOfDivisions);

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;

  //
  // Setup the ImageIO with information from outputPtr
  //
  m_ImageIO->SetNumberOfDimensions(TInputImage::ImageDimension);
  const typename TInputImage::SpacingType&   spacing = outputPtr->GetSpacing();
  const typename TInputImage::PointType&     origin = outputPtr->GetOrigin();
  const typename TInputImage::DirectionType& direction = outputPtr->GetDirection();
  int direction_sign(0);
  for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
  {
      if (direction[i][i] < 0)
      {
         direction_sign = -1;
      }
      else
      {
          direction_sign = 1;
      }
      // Final image size
      m_ImageIO->SetDimensions(i, outputRegion.GetSize(i));
      m_ImageIO->SetSpacing(i, spacing[i] * direction_sign);
      m_ImageIO->SetOrigin(i, origin[i]);

      vnl_vector<double> axisDirection(TInputImage::ImageDimension);
      // Please note: direction cosines are stored as columns of the
      // direction matrix
      for (unsigned int j = 0; j < TInputImage::ImageDimension; ++j)
      {
          axisDirection[j] = direction[j][i] * direction_sign;
      }
      m_ImageIO->SetDirection(i, axisDirection);
  }

  m_ImageIO->SetUseCompression(m_UseCompression);
  m_ImageIO->SetMetaDataDictionary(inputPtr->GetMetaDataDictionary());

  /** Create Image file */
  // Setup the image IO for writing.
  //
  //NMDebugAI(<< "Image information for ImageIO ..." << endl);
  //NMDebugAI(<< "  origin: " << origin[0] << ", " << origin[1] << endl);
  //NMDebugAI(<< "  spacing: " << spacing[0] << ", " << spacing[1] << endl);
  //NMDebugAI(<< "  region ... " << endl);
  //outputRegion.Print(std::cout, itk::Indent(nmlog::nmindent + 4));

  m_ImageIO->WriteImageInformation();

  // Notify START event observers
  this->InvokeEvent(itk::StartEvent());

  this->UpdateProgress(0);

  for (m_CurrentDivision = 0;
       m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
       m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
  {
      streamRegion = m_StreamingManager->GetSplit(m_CurrentDivision);

      inputPtr->SetRequestedRegion(streamRegion);
      inputPtr->PropagateRequestedRegion();
      inputPtr->UpdateOutputData();

      // Write the whole image
      itk::ImageIORegion ioRegion(TInputImage::ImageDimension);
      for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
      {
          ioRegion.SetSize(i, streamRegion.GetSize(i));
          ioRegion.SetIndex(i, streamRegion.GetIndex(i));
      }
      this->SetIORegion(ioRegion);
      m_ImageIO->SetIORegion(m_IORegion);

      // Start writing stream region in the image file
      if (m_WriteImage)
      {
          this->GenerateData();
      }
  }

  /** build overviews */
  if (m_ResamplingType.compare("NONE") != 0 && m_WriteImage)
  {
      if (gio != 0)
      {
          gio->BuildOverviews(m_ResamplingType);
      }
  }

  /**
   * If we ended due to aborting, push the progress up to 1.0 (since
   * it probably didn't end there)
   */
  if (!this->GetAbortGenerateData())
  {
      this->UpdateProgress(1.0);
  }

  // Notify end event observers
  this->InvokeEvent(itk::EndEvent());


  // ONLY WHEN RAT AVAILABLE AND HAS NOT BEEN WRITTEN WITH IMAGE (above)
  if (!m_WriteImage && m_InputRAT.IsNotNull())
  {
      if (gio)
      {
          NMProcDebug(<< "writing ONLY RAT!");
          gio->WriteRAT(m_InputRAT);
      }
      if (m_InputRAT->GetTableType() == otb::AttributeTable::ATTABLE_TYPE_SQLITE)
      {
          otb::SQLiteTable* sqltab = static_cast<otb::SQLiteTable*>(m_InputRAT.GetPointer());
          if (sqltab)
          {
              sqltab->CloseTable();
          }
      }
  }


  /**
   * Now we have to mark the data as up to data.
   */
  for (idx = 0; idx < this->GetNumberOfOutputs(); ++idx)
    {
    if (this->GetOutput(idx))
      {
      this->GetOutput(idx)->DataHasBeenGenerated();
      }
    }

  // Write the image keyword list if any
  // ossimKeywordlist geom_kwl;
  // ImageKeywordlist otb_kwl;

  // itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
  // itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
  // otb_kwl.convertToOSSIMKeywordlist(geom_kwl);
  //FIXME: why nothing is done with otb_kwl in that case???
  // If required, put a call to WriteGeometry() here

  /**
   * Release any inputs if marked for release
   */

  this->ReleaseInputs();

  // Mark that we are no longer updating the data in this filter
  this->m_Updating = false;

  // close the GDALDataset
  if (gio != 0)
  {
      gio->CloseDataset();
  }

  // provenance information
  std::stringstream sstr;

  // create table entity
  std::vector<std::string> args;
  std::vector<std::string> attrs;

  std::string fn = this->GetFileName();
  std::size_t pos1 = fn.find_last_of("/\\");
  std::string basename = fn.substr(pos1+1);
  std::size_t pos2 = basename.find_last_of('.');
  std::string ext = basename.substr(pos2+1);

  // provn used by
  sstr.str("");
  sstr << "img:" << basename;
  args.push_back(sstr.str());

//  sstr.str("");
//  sstr << "nm:FileName=\"" << this->GetFileName();
//  attrs.push_back(sstr.str());

//  sstr.str("");
//  sstr << "nm:ImageFormat=\"" << ext;
//  attrs.push_back(sstr.str());

  NMProcProvN(itk::NMLogEvent::NM_PROV_GENERATION, args, attrs);

  args.clear();
  attrs.clear();
  sstr.str("");
  sstr << "img:" << basename;
  args.push_back(sstr.str());

  sstr.str("");
  sstr << "nm:FileName=\"" << this->GetFileName() << "\"";
  attrs.push_back(sstr.str());

  sstr.str("");
  sstr << "nm:Format=\"" << ext << "\"";
  attrs.push_back(sstr.str());

  NMProcProvN(itk::NMLogEvent::NM_PROV_ENTITY, args, attrs);

}

//---------------------------------------------------------

/**
 *
 */
template<class TInputImage>
void
StreamingRATImageFileWriter<TInputImage>
::GenerateData(void)
{
  const InputImageType * input = this->GetInput();

  // Make sure that the image is the right type and no more than
  // four components.
  typedef typename InputImageType::PixelType ImagePixelType;

  if (strcmp(input->GetNameOfClass(), "VectorImage") == 0)
    {
    typedef typename InputImageType::InternalPixelType VectorImagePixelType;
    m_ImageIO->SetPixelTypeInfo(typeid(VectorImagePixelType));

    typedef typename InputImageType::AccessorFunctorType AccessorFunctorType;
    m_ImageIO->SetNumberOfComponents(AccessorFunctorType::GetVectorLength(input));
    }
  else
    {
    // Set the pixel and component type; the number of components.
    m_ImageIO->SetPixelTypeInfo(typeid(ImagePixelType));
    }

  // Setup the image IO for writing.
  //
  //okay, now extract the data as a raw buffer pointer
  const void* dataPtr = (const void*) input->GetBufferPointer();
  m_ImageIO->Write(dataPtr);

  if (m_WriteGeomFile)
    {
    ImageKeywordlist otb_kwl;
    itk::MetaDataDictionary dict = this->GetInput()->GetMetaDataDictionary();
    itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, otb_kwl);
    WriteGeometry(otb_kwl, this->GetFileName());
    }

  this->ReleaseInputs();
}

} // end namespace otb

#endif

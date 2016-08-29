 /****************************************************************************** 
 * Created by Alexander Herzig 
 * Copyright 2016 Landcare Research New Zealand Ltd
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

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2008-10-16 18:05:25 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*
 * This class is inspired by the itk::MeanImageFilter. It has been copied and
 * extensively adapted by Alexander Herzig, Landcare Research New Zealand Ltd.
 *
 */

#ifndef __otbNMScriptableKernelFilter2_h
#define __otbNMScriptableKernelFilter2_h


// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.

//#include "nmlog.h"

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNMConstShapedNeighborhoodIterator.h"

#include "otbMultiParser.h"

#include "otbsupplfilters_export.h"

namespace otb
{
/*! \class NMScriptableKernelFilter2
 *  \brief A scriptable (optionally shaped neighborhood) filter
 *
 *  This filter allows the user to execute a custom script
 *  to calculate the output pixel value. If required, the user
 *  may access individual pixel values of a user-defined
 *  CIRCULAR or RECTANGULAR neighbourhood around the centre
 *  pixel to be calculated.
 *  The user-defined script represents a sequence of muParser-based
 *  expressions, completed by a semi-colon ';'. Additionally,
 *  users may use C-style for loops, e.g.
 *
 *  \code
 *  size=10;
 *  out=0;
 *  b=5.7;
 *  for (i=1; i < size; i = i+1)
 *  {
 *      b = b * i;
 *  }
 *  out=b;
 *  \endcode
 *
 *  A for loop may not be nested in a muParser expression,
 *  as for example
 *
 *  \code
 *  (a<0) ? for ... : 0;
 *  \endcode
 *
 *  However, for loops itself may be nested, e.g.
 *
 *  \code
 *  for (i=0; i < number; i=i+1)
 *  {
 *      for (g=4; g >=0; g = g-1)
 *      {
 *          out = i*g;
 *      }
 *  }
 *  \endcode
 *
 *  and muParser expressions may be used in the loop header
 *  header or body, e.g.
 *
 *  TBD
 *
 *  RESERVED names (must not be used for user variables!):
 *
 *  numPix       : number of active pixel in the neighbourhood
 *  centrePixIdx : 1D neighbourhood index of the centre pixel
 *  addr         : object address, used in kwinVal, tabVal, neigDist
 *  thid         : thread id, used in kwinVal and tabVal
 *  kwinVal      : function to access neighbourhood values by 1D-index
 *  tabVal       : function to access table values by column and row index
 *  neigDist     : neighbour distance from centre pixel (in pixel)
 *
 */
template <class TInputImage, class TOutputImage>
class OTBSUPPLFILTERS_EXPORT NMScriptableKernelFilter2 :
    public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef NMScriptableKernelFilter2                          Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self)

  /** Run-time type information (and related methods). */
  itkTypeMacro(NMScriptableKernelFilter2, itk::ImageToImageFilter)
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType               InputPixelType;
  typedef typename OutputImageType::PixelType              OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::SizeType    InputSizeType;
  typedef typename InputImageType::IndexType   IndexType;

  typedef typename InputImageType::SpacingType  SpacingType;
  typedef typename InputImageType::PointType    OriginType;

  typedef typename otb::MultiParser            ParserType;
  typedef typename otb::MultiParser::Pointer   ParserPointerType;
  typedef typename otb::MultiParser::ValueType ParserValue;
  typedef typename otb::MultiParser::CharType CharType;

  typedef typename itk::NMConstShapedNeighborhoodIterator<InputImageType> InputShapedIterator;
  typedef typename InputShapedIterator::OffsetType  OffsetType;
  typedef typename InputShapedIterator::NeighborIndexType NeighborIndexType;

  typedef typename itk::ImageRegionConstIterator<InputImageType> InputRegionIterator;
  typedef typename itk::ConstNeighborhoodIterator<InputImageType> InputNeighborhoodIterator;
  typedef typename itk::ImageRegionIterator<OutputImageType> OutputRegionIterator;

  /*! Set the radius of the neighborhood.
   *  RECTANGULAR neighbourhoods may have
   *  (of course) a different size for each
   *  dimension. However, if the KernelShape
   *  is CIRCULAR, we create a square neighbourhood
   *  based on the biggest size across dimensions
   *
   *  NOTE: the circular neighbourhood assumes
   *        square pixels
   */
  itkSetMacro(Radius, InputSizeType)
  void SetRadius(const int* radius);

  /*! Get the radius of the neighbourhood*/
  itkGetConstReferenceMacro(Radius, InputSizeType)
  
  /*! Set the kernel script */
  itkSetStringMacro(KernelScript)

  /*! Set the kernel shape <Square, Circle> */
  itkSetStringMacro(KernelShape)

  /*! Set the script variable (name) associated with
   *  the output image pixel value */
  itkSetStringMacro(OutputVarName)

  /*! Set the nodata value of the computation */
  //itkSetMacro(Nodata, OutputPixelType)
  void SetNodata(const double& nodata);

  /*! Forward component UserIDs to the filter*/
  void SetInputNames(const std::vector<std::string>& inputNames);

  void SetFilterInput(const unsigned int& idx, itk::DataObject* dataObj);

  void setRAT(unsigned int idx, AttributeTable::Pointer table);


  /*! The filter needs a larger input requested region than
   * the output requested region.
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

  void Reset();

  static const double kwinVal(const char* imgName, double kwinIdx, double threadId, double thisAddr)
  {
      return m_mapNameImgValue[static_cast<double>(thisAddr)][threadId][imgName][kwinIdx];
  }

  static const double tabVal(const char* tabName, double colIdx, double rowIdx, double thisAddr)
  {
      return m_mapNameTable[static_cast<double>(thisAddr)][tabName][colIdx][rowIdx];
  }

  static const double neigDist(double kwinIdx, double thisAddr)
  {
      return m_mapNeighbourDistance[static_cast<double>(thisAddr)][kwinIdx];
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (itk::Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  NMScriptableKernelFilter2();
  virtual ~NMScriptableKernelFilter2();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** This filter is implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *d
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId );

  void BeforeThreadedGenerateData();
  void AfterThreadedGenerateData();
  void CacheInputData();
  void ParseScript();
  void ParseCommand(const std::string& expr);

  /*!
   * \brief Loop     mechanics behind C-style for loop based on muParser expressions
   * \param i
   * \param threadId
   */
  inline void Loop(int i, const int& threadId)
  {
      const int numForExp = m_vecBlockLen[i]-3;
      const ParserPointerType& testParser = m_vecParsers[threadId][++i];
      ParserValue& testValue = m_mapNameAuxValue[threadId][m_mapParserName[testParser.GetPointer()]];
      testValue = testParser->Eval();

      ParserPointerType& counterParser = m_vecParsers[threadId][++i];
      ParserValue& counterValue = m_mapNameAuxValue[threadId][m_mapParserName[counterParser.GetPointer()]];

      while (testValue != 0)
      {
          for (int exp=1; exp <= numForExp; ++exp)
          {
              const ParserPointerType& forParser = m_vecParsers[threadId][i+exp];
              ParserValue& forValue = m_mapNameAuxValue[threadId][m_mapParserName[forParser.GetPointer()]];
              forValue = forParser->Eval();

              if (m_vecBlockLen[i+exp] > 1)
              {
                  Loop(i+exp, threadId);
                  exp += m_vecBlockLen[i+exp]-1;
              }
          }

          counterValue = counterParser->Eval();
          testValue = testParser->Eval();
      }
  }

private:
  NMScriptableKernelFilter2(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  long long m_NumPixels;
  long long m_PixelCounter;
  std::vector<long long> m_vthPixelCounter;
  int m_NumNeighbourPixel;

  InputSizeType m_Radius;
  std::vector<NeighborIndexType> m_ActiveKernelIndices;
  ParserValue m_CentrePixelIndex;
  ParserValue m_ActiveNeighborhoodSize;

  std::string m_KernelScript;
  std::string m_KernelShape;
  std::string m_OutputVarName;

  std::vector<std::string> m_DataNames;
  OutputPixelType m_Nodata;

  SpacingType   m_Spacing;
  OriginType    m_Origin;

  double m_This;
  std::vector<double> m_thid;

  // admin objects for the scriptable kernel filter

  // ===================================================
  //        thread dependant object/value stores
  // ===================================================

  // have to define these separately for each thread ...
  // the parsers themselves
  std::vector<std::vector<ParserPointerType> > m_vecParsers;

  // the x, y, and z coordinate of centre pixel
  std::vector<ParserValue> m_mapXCoord;
  std::vector<ParserValue> m_mapYCoord;
  std::vector<ParserValue> m_mapZCoord;

  // the output value per thread
  std::vector<ParserValue> m_vOutputValue;


  // ===================================================
  //        thread independant object/value stores
  // ===================================================

  // the link between each parser and the name of the variable representing its output value
  std::map<MultiParser*, std::string> m_mapParserName;
  // the link between input images and their user defined names
  std::map<std::string, InputImageType*> m_mapNameImg;
  // the length of each script block; note a block is either a single statement/expression,
  // or a for loop including the test and counter variables
  std::vector<int> m_vecBlockLen;

  // the over- and underflows per thread
  std::vector<long long> m_NumOverflows;
  std::vector<long long> m_NumUnderflows;

  // these are aux scalar values which are defined in the KernelScript itself, e.g.
  // loop counter and test variables
  std::vector<std::map<std::string, ParserValue> > m_mapNameAuxValue;


  // ===================================================
  //        static value stores
  // ===================================================
  // these stores make class instance dependant values
  // available via the static muParser callback functions
  // ... bit a of dirty hack ... I guess

  // can share those across threads ...
  // read-only neighbour pixel distances to centre pixel
  static std::map<double, std::vector<ParserValue> > m_mapNeighbourDistance;

  // read-only input tables
  static std::map<double, std::map<std::string, std::vector<std::vector<ParserValue> > > > m_mapNameTable;

  // the map kernel values are thread dependant
  static std::map<double, std::vector<std::map<std::string, std::vector<ParserValue> > > > m_mapNameImgValue;


};
  
} // end namespace itk

// definition of static member vars of above class
template<class TInputImage, class TOutput>
std::map<double, std::map<std::string, std::vector<std::vector<typename otb::NMScriptableKernelFilter2<TInputImage, TOutput>::ParserValue> > > >
otb::NMScriptableKernelFilter2<TInputImage, TOutput>::m_mapNameTable;

template<class TInputImage, class TOutput>
std::map<double, std::vector<std::map<std::string, std::vector<typename otb::NMScriptableKernelFilter2<TInputImage, TOutput>::ParserValue> > > >
otb::NMScriptableKernelFilter2<TInputImage, TOutput>::m_mapNameImgValue;

template<class TInputImage, class TOutput>
std::map<double, std::vector<typename otb::NMScriptableKernelFilter2<TInputImage, TOutput>::ParserValue> >
otb::NMScriptableKernelFilter2<TInputImage, TOutput>::m_mapNeighbourDistance;


//#include "otbNMScriptableKernelFilter2_ExplicitInst.h"

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbNMScriptableKernelFilter2.txx"
#endif


#endif

 /*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVTKImageExport.h,v $
  Language:  C++
  Date:      $Date: 2008-10-09 15:31:37 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __NMitkVTKImageExport_h
#define __NMitkVTKImageExport_h

#include "itkVTKImageExportBase.h"

namespace itk
{

/** \class NMVTKImageExport
 * \brief Connect the end of an ITK image pipeline to a VTK pipeline.
 *
 * NMVTKImageExport can be used at the end of an ITK image pipeline to
 * connect with a VTK pipeline that begins with vtkImageImport.
 * Callbacks provided by NMVTKImageExport are registered with
 * vtkImageImport to connect the pipeline execution together.  Once
 * connected, update requests coming through the VTK pipeline are
 * automatically propagated to the ITK pipeline.
 *
 * While VTKImageExportBase provides the pipeline functionality
 * independent of image type, instances must be created through
 * NMVTKImageExport.  This class provides the implementations for
 * callbacks that depend on the image type.
 *
 * Note that not all image types will work correctly.  VTK will only
 * support images of 1, 2, or 3 dimensions.  Scalar value types can be
 * one of: float, double, char, unsigned char, short, unsigned short,
 * int, unsigned int, long, unsigned long.
 *
 * Currently NMVTKImageExport does not support pixel types with multiple
 * components (like RGBPixel).
 *
 * \ingroup IOFilters
 * \sa VTKImageExportBase
 */
template <class TInputImage>
class ITK_EXPORT NMVTKImageExport: public VTKImageExportBase
{
public:
  /** Standard class typedefs. */
  typedef NMVTKImageExport            Self;
  typedef VTKImageExportBase        Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(NMVTKImageExport,VTKImageExportBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** The type of the input image. */
  typedef TInputImage InputImageType;

  /** Set the input image of this image exporter. */
  void SetInput(const InputImageType*);

protected:
  NMVTKImageExport();
  ~NMVTKImageExport() {}
  void PrintSelf(std::ostream& os, Indent indent) const;  

  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType InputRegionType;
  typedef typename InputRegionType::SizeType  InputSizeType;
  typedef typename InputRegionType::IndexType InputIndexType;
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension );
  
  InputImageType * GetInput(void);
  
  int* WholeExtentCallback();
  double* SpacingCallback();
  double* OriginCallback();
  float* FloatSpacingCallback();
  float* FloatOriginCallback();
  const char* ScalarTypeCallback();
  int NumberOfComponentsCallback();
  void PropagateUpdateExtentCallback(int*);
  int* DataExtentCallback();
  void* BufferPointerCallback();
  
private:
  NMVTKImageExport(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string m_ScalarTypeName;
  int m_WholeExtent[6];
  int m_DataExtent[6];
  double m_DataSpacing[3];
  double m_DataOrigin[3];
  float m_FloatDataSpacing[3];
  float m_FloatDataOrigin[3];
};

} // end namespace itk
  
//#ifndef ITK_MANUAL_INSTANTIATION
#include "NMitkVTKImageExport.txx"
//#endif

#endif

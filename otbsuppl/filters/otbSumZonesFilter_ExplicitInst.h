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

#ifndef __SumZonesFilter_INST_H
#define __SumZonesFilter_INST_H

#include "otbImage.h"
#include "otbSumZonesFilter.h"

template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<unsigned char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<char, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<unsigned short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<short, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<unsigned int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<int, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<unsigned long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<long, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<float, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 1>, otb::Image<double, 1> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<unsigned char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<char, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<unsigned short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<short, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<unsigned int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<int, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<unsigned long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<long, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<float, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 2>, otb::Image<double, 2> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned char, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<char, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned short, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<short, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned int, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<int, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<unsigned long, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<long, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<float, 3>, otb::Image<double, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<unsigned char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<char, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<unsigned short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<short, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<unsigned int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<int, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<unsigned long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<long, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<float, 3> >;
template class OTBSUPPLFILTERS_EXPORT otb::SumZonesFilter< otb::Image<double, 3>, otb::Image<double, 3> >;


#endif  // include guard

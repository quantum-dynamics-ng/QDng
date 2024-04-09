// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef TNT_IO_H
#define TNT_IO_H

#include "tnt.h"

namespace TNT
{

/**
 * Simple class template for IO.
 * 
 * TNT classes assume a simple convention in ascii files:
 * The first line of file contains coloumns and rows of of the following lines.
 * 
 */
template <class T>
class TNT_IO
{
  public:
    
      /**
       * Read matrix from ASCII file.
       * 
       * \param infile File name
       * \param matrix
       * \return false if open fails
       */
      bool static ReadAsciiMatrix(const string &infile, Matrix<T> &matrix)
      {
	ifstream File;
	
	File.open (infile.c_str());
	if (!File.is_open()) return false;
	File >> matrix;
	File.close();
	return true;
      }
	
      /**
       * Write matrix to ASCII file.
       * 
       * \param outfile File name
       * \param vector
       * \return false if open fails
       */
      bool static WriteAsciiMatrix(const string &outfile, Matrix<T> &matrix)
      {
	ofstream File;

	File.open (outfile.c_str(), ios_base::trunc);
	if (!File.is_open()) return false;
	File << matrix;
	File.close();
	return true;
      }
      
      /**
       * Read vector from ASCII file.
       * 
       * \param infile File name
       * \param vector
       * \return false if open fails
       */      
      bool static ReadAsciiVector(const string &infile, Vector<T> &vector)
      {
	ifstream File;
	
	File.open (infile.c_str());
	if (!File) return false;
	File >> vector;
	File.close();
	return true;
      }

      /**
       * Write vector to ASCII file.
       * 
       * \param outfile File name
       * \param vector
       * \return false if open fails
       */
      bool static WriteAsciiVector(const string &outfile, Vector<T> &vector)
      {
	ofstream File;
	
	File.open (outfile.c_str(), ios_base::trunc);
	if (!File) return false;
	File << vector;
	File.close();
	return true;
      }

};
    
    





} /* namespace TNT */

#endif


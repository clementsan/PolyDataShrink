#include <iostream>

#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataNormals.h"

int main(int argc, const char **argv)
{
  if ( argc <= 3 ) 
    {
      std::cout << argv[0] << " original_polydata output_polydata shrink_percentage [translate_x translate_y translate_z]" << std::endl ;
    exit(0);
  }

  float cog[3] ;

  // read in the vtk polydata file
  vtkPolyDataReader *reader = vtkPolyDataReader::New() ;
  reader->SetFileName ( argv[1] ) ;
  reader->Update () ;
  vtkPolyData *polydata = reader->GetOutput () ;

  unsigned long int nPoints = polydata->GetNumberOfPoints();
  vtkPoints * vtkpoints =  polydata->GetPoints();
  vtkFloatingPointType *point ;
 
  double *bb ;
  polydata->ComputeBounds () ;
  bb = polydata->GetBounds () ;
  cog[0] = ( bb[1] - bb[0] ) * 0.5 + bb[0] ;
  cog[1] = ( bb[3] - bb[2] ) * 0.5 + bb[2] ;
  cog[2] = ( bb[5] - bb[4] ) * 0.5 + bb[4] ;
  std::cout << "Old center of gravity: " << cog[0] << " " << cog[1] << " " << cog[2] << std::endl ;
  

  float scale = atof ( argv[3] ) / 100.0 ;
  std::cout << "Scale: " << scale << std::endl ;

  float translate[3] ;

  if ( argc == 7 ) 
    {
      translate[0] = atof ( argv[4] ) ;
      translate[1] = atof ( argv[5] ) ;
      translate[2] = atof ( argv[6] ) ;
    }
  else 
    {
      translate[0] = translate[1] = translate[2] = 0.0f ;
    }
  //std::cout << "New center of gravity: " << cogNew[0] << " " << cogNew[1] << " " << cogNew[2] << std::endl ;

  float cogNew[3] ;
  cogNew[0] = cog[0] + translate[0] ;
  cogNew[1] = cog[1] + translate[1] ;
  cogNew[2] = cog[2] + translate[2] ;

  for ( unsigned long int p = 0 ; p < nPoints ; p++ )
    {
      point = vtkpoints->GetPoint( p ) ;    
      point[0] = ( point[0] - cog[0] ) * scale + cogNew[0] ;
      point[1] = ( point[1] - cog[1] ) * scale + cogNew[1] ;
      point[2] = ( point[2] - cog[2] ) * scale + cogNew[2] ;
      
      vtkpoints->SetPoint  ( p, point ) ;
    }

  // compute normals, just to be sure they will be in the vtk file
  vtkPolyDataNormals *normalFilter = vtkPolyDataNormals::New() ;
  normalFilter->SetInput ( polydata ) ;
  normalFilter->SplittingOff () ;
  normalFilter->Modified() ;
  normalFilter->Update () ;

  // write out the vtk polydata file
  vtkPolyDataWriter *writer = vtkPolyDataWriter::New() ;
  writer->SetFileName ( argv[2] ) ;
  writer->SetInput ( normalFilter->GetOutput() ) ;
  writer->Update () ;
 
  reader->Delete () ;
  writer->Delete () ;
  normalFilter->Delete () ;

  return 0 ;
}

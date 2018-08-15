#include <iostream>
#include "RTree.h"

using namespace std;

typedef double ValueType;

struct Rect rects[] =
{
  Rect(0.1, 0.1, 2.1, 2.1), // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
  Rect(6.1, 5., 11., 6.),
  Rect(1.1, 2., 3., 4.),
  Rect(7.1, 3., 8., 4.),
  Rect(7.1, 1., 9., 2.),
  Rect(2.1, 4., 4., 6.),
  Rect(10.1, 3., 11., 4.),
  Rect(5.1, 1., 6., 2),
  Rect(4.1, 2., 4., 2.),
  Rect(10.1, 1., 10., 1.),
};

int nrects = sizeof(rects) / sizeof(rects[0]);
ValueType a_point[2];
int a_k;

struct Rect rects2[] =
{
  Rect(2.1, 4., 4., 6.),
  Rect(10.1, 3., 11., 4.),
  Rect(5.1, 1., 6., 2),
};
int nrects2 = sizeof(rects2) / sizeof(rects2[0]);

Rect search_rect(4., 0., 11., 6.); // search will find above rects that this one overlaps


int main()
{
  typedef RTree<ValueType, ValueType, 2, float, 8> MyTree;
  MyTree tree;

  int i, nhits;
  cout << "nrects = " << nrects << "\n";
/*
  for(i=0; i<nrects-1; i++)
  {
    tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
  }
  tree.Updatetree(rects[nrects-1].min, rects[nrects-1].max, nrects-1); // Note, all values including zero are fine in this version
  */
  for(i=0; i<nrects; i++)
  {
    tree.Updatetree(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
  }

  cout<< "Iniciando búsqueda"<<endl;
  cout<< "------------------"<<endl;

  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback);

  cout << "Se encontraron " << nhits << " elementos.\n";

  cout<< "Iniciando búsqueda KNN"<<endl;
  cout<< "-----------------------"<<endl;
  a_point[0]=1.25;
  a_point[1]=6.5;
  a_k=3;
  tree.Search_knn(a_point, a_k);
  for (int knn_aux = 0; knn_aux < search_knn_export.size(); knn_aux++) {
    cout<<"Element A_k_"<<knn_aux<<" = "<<search_knn_export[knn_aux]<<endl;
  }
  cout << "Se encontraron " << search_knn_export.size() << " elementos.\n";

/*
  tree.RemoveAll();
  cout<<"RESETEANDO ARBOL"<<endl;
  cout<<"------------------"<<endl;
  cout<<"------------------"<<endl;
  //tree.Updatetree(rects[9].min, rects[9].max, 9); // Note, all values including zero are fine in this version
  for(i=0; i<nrects2; i++)
  {
    tree.Updatetree(rects2[i].min, rects2[i].max, i); // Note, all values including zero are fine in this version
  }
*/
/*
  cout<< "Iniciando búsqueda"<<endl;
  cout<< "------------------"<<endl;
  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback);
  cout << "Se encontraron " << nhits << " elementos.\n";*/
  return 0;

}

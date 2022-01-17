#ifndef PELICULAS_H
#define PELICULAS_H

using namespace std;

class Pelicula{
  protected:
  int idPelicula;
  int anio;
  string titulo;
  string productora;
  string director;
  
  public:
  
  Pelicula (int idP, string t, int a, string d, string p);
  
  void MostrarCatalogo (SAConnection* con);
  void SuministrarPelicula (string t, int a, string d, string p, SAConnection* con);
  void InhabilitarPelicula (int idP, SAConnection* con);
  void BuscarTituloCatalogo (string t, SAConnection *con);d
  void MostrarRecomendaciones (int telefono,SAConnection *con);
  void VerPelicula (int telefono, int idPelicula, SAConnection *con);
  
};


#endif

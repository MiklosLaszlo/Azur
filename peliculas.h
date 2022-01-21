#ifndef PELICULAS_H
#define PELICULAS_H

#include <SQLAPI.h>
#include <utility>
#include <cstring>
#include <string>

using namespace std;

  
void MostrarCatalogo (SAConnection* con);
void SuministrarPelicula (string t, int a, string d, string p, int CIF, SAConnection* con);
void InhabilitarPelicula (int idP, SAConnection* con);
void BuscarTituloCatalogo (string t, SAConnection *con);
void MostrarRecomendaciones (int telefono,SAConnection *con);
void VerPelicula (int idSesion, int idPelicula, SAConnection *con);
  


#endif

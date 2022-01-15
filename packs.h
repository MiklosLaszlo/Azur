#ifndef PACKS_H
#define PACKS_H

#include <SQLAPI.h>
#include <vector>
#include <utility>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

void InformarNovedades(string listaPacks, SAConnection* con);

void ModificarPack(vector<unsigned> idPeliculas, double precio ,string idPack ,SAConnection* con);

void CrearPack(vector<unsigned> idPeliculas, double precio ,string idPack ,SAConnection* con);

void DesactivarPack(string idPack ,SAConnection* con);

void RecibirRecomendaciones(vector< pair <unsigned, unsigned >> clienteRPeliculas, SAConnection* con);


#endif

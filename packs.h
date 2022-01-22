#ifndef PACKS_H
#define PACKS_H

#include <SQLAPI.h>
#include <vector>
#include <utility>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

void InformarNovedades(vector<string> listaPacks, SAConnection* con);

void ModificarPack(vector<int> idPeliculas, double precio ,string idPack ,SAConnection* con);

void CrearPack(vector<int> idPeliculas, double precio ,string idPack ,SAConnection* con);

void DesactivarPack(string idPack ,SAConnection* con);

void RecibirRecomendaciones(vector< pair <unsigned, unsigned >> clienteRPeliculas, SAConnection* con);


#endif

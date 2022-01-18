#ifndef FINANZAS_H
#define FINANZAS_H

#include <SQLAPI.h>
#include <vector>
#include <utility>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

struct ContratoCliente{
	int idContrato;
	string nombreCliente;
	int tlfCliente;
	string correo;
	string sexo;
	int[3] fechaNacimiento;
	double tarjeta;
	vector<string> packsContratados;
	int[3] fechaInicio;
	int[3] fechaFin;
	double precio;
};

struct ContratoProveedor{
	int idContrato;
	string nombreProveedor;
	int tlfProveedor;
	string correo;
	int cif;
	vector<string> peliculasASuministrar;
	int[3] fechaInicio;
	int[3] fechaFin;
	double precio;
};

struct FacturaCliente{
	double precio;
	int tlfCliente;
	int[3] fechaPago;
};

struct FacturaProveedor{
	double precio;
	int cif;
	int[3] fechaPago;
};

struct BalanceGastos{
	vector<FacturaCliente> ingresos;
	vector<FacturaProveedor> gastos;
	double balance;
};

ContratoCliente GenerarContratoCliente(int tlf, vector<string> listaPacks, int[3] fechaFin, double precio, SAConnection* con);

void DarAltaEmpresa(string nombre, int tlf, string correo, int cif, SAConnection* con);

ContratoProveedor GenerarContratoProveedor(int cif, vector<string> peliculas, double precio, SAConnection* con);

FacturaCliente RecibirPago(int tlf, double precio, int[3] fechaPago, SAConnection* con);

FacturaProveedor RealizarPago(int cif, double precio, int[3] fechaPago, SAConnection* con);

BalanceGastos BalanceDeGastos(SAConnection* con);

#endif

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
	SAString nombreCliente;
	int tlfCliente;
	SAString correo;
	SAString sexo;
	SADateTime fechaNacimiento;
	double tarjeta;
	vector<SAString> packsContratados;
	SADateTime fechaInicio;
	SADateTime fechaFin;
	double precio;
};

struct ContratoProveedor{
	int idContrato;
	SAString nombreProveedor;
	int tlfProveedor;
	SAString correo;
	int cif;
	vector<SAString> peliculasAActivar;
	SADateTime fechaInicio;
	SADateTime fechaFin;
	double precio;
};

struct FacturaCliente{
	int idfacturac;
	double precio;
	int tlfCliente;
	SADateTime fechaPago;
};

struct FacturaProveedor{
	int idfacturap;
	double precio;
	int cif;
	SADateTime fechaPago;
};

struct BalanceGastos{
	vector<FacturaCliente> ingresos;
	vector<FacturaProveedor> gastos;
	double balance;
	bool balanceCorrecto ;
};

ContratoCliente GenerarContratoCliente(int tlf, vector<SAString> listaPacks, SADateTime fechaFin, double precio, SAConnection* con);

void DarAltaEmpresa(SAString nombre, int tlf, SAString correo, int cif, SAConnection* con);

ContratoProveedor GenerarContratoProveedor(int cif, vector<SAString> peliculas, SADateTime fechaFin, double precio, SAConnection* con);

FacturaCliente RecibirPago(int tlf, double precio, SADateTime fechaPago, SAConnection* con);

FacturaProveedor RealizarPago(int cif, double precio, SADateTime fechaPago, SAConnection* con);

BalanceGastos BalanceDeGastos(SAConnection* con);

//Funciones auxiliares para mostrar las estructuras 
void mostrarContratoCliente(ContratoCliente contrato);
void mostrarContratoProveedor(ContratoProveedor contrato);
void mostrarBalance(BalanceGastos balanceTotal);

#endif

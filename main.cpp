//Subgrupo Azul: Irene, Gabriel, Nicolás y Gerardo
#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include <iostream>
#include <ctime>

#include "packs.h"
#include "clientes.h"
#include "peliculas.h"
#include "finanzas.h"

void MenuPrincipal(){
  // 1 Registrar Cliente
  // 2 Inciar Sesion cliente -> MEnu cliente (ver pelicula, modficar datos, buscar catalago, mostrar catalog/recomendaciones, dar de baja y finalizar sesion)
  // 3 COSAS EMPLEADO (iniciar sesion "empleado") -> Menu empleado (submenu de packs, submenu finanzas)
  // 4 Dar alta Empresa
  // 5 COsas de la empresa -> MEnu empresa (solo suministrar peliculas)
};

void Submenufinanzas(SAConnection* con){
  //1 generar contrato de un cliente
  //2 generar contrato empleado
  //3 balance de gastos
  int opcion = 0;
  Balance balance;
  ContratoCliente contratoC;
  ContratoProveedor contratoP;
  
  while(opcion!=4){
    cout<<"Está usted en el submenú del de departamento de finanzas, ¿Qué desea hacer?"<<endl;
    cout<<"\t1- Generar contrato cliente\n\t2- Generar contrato proveedor\n\t3- Balance de gastos\n\t4- Salir del submenú"<<endl;
    cin>>opcion;
    switch(opcion){
      case 1: //Contrato cliente
	int tlfCliente, numPacks;
	int[3] fechaFin;
	vector<string> packsContratados;
	double precio;
		    
	cout<<"Introduzca el telefono del cliente: ";
	cin>>tlfCliente;
	cout<<"\n¿Cuantos packs desea contratar el cliente?: ";
	cin>>numPacks;
	SAString aux;
	    for(int i=0; i<numPacks; i++){
		cout<<"\n\tIntroduzca el nombre del pack "<<i<<":";
		cin>>pack;
		packsContratados.push_back(aux);
	    }
		    
	contratoC = GenerarContratoCliente(, con);
	mostrarContratoProveedor(contratoP);	    
        break;
		    
      case 2: //Contrato proveedor
        contratoP = GenerarContratoProveedor(, con);
	mostrarContratoProveedor(contratoP);
        break;
		    
      case 3: //Balance gastos
        balance = BalanceDeGastos(con);
	mostrarBalance(balance);
        break;
    }
  }
};

void Submenupakcs(SAConnection* con){

};

/*SUBMENU PARA SUMINISTRAR PELICULAS*/
void SubMenuSuministrar (SAConnection *con){
	int anio, CIF, n;
	string titulo, director, productora; 
	
	do{
		cout << "Introduzca el CIF de su empresa: ";
		cin >> CIF;
		
		SACommand busqueda;
		busqueda.setConnection(con);
		busqueda.setCommandText(_TSA("SELECT COUNT(*) FROM PROVEEDOR WHERE CIF = :1"));
		busquda.Param(1).setAsInt64() = CIF;
		busqueda.Execute();
		
		busqueda.Fetch();
		int n = busqueda[1].asInt64();
	}while(n!=1);
	
	
	cout << "Va a suministrar una película. Introduzca los siguientes datos: " << endl;
	cout << "Título: ";
	cin >> titulo;
	cout << "Año: ";
	cin >> anio;
	cout << "Director: ";
	cin >> director;
	cout << "Productora: ";
	cin >> productora;
	
	SuministrarPelicula(titulo,anio,director,productora,CIF,con);
};

main(int argc, char* argv[]){
  SAConnection con;
  try {
        con.Connect(_TSA("Toca"), _TSA("x7485923"), _TSA("x7485923"), SA_Oracle_Client); //usar alias
        cout<< "¡Bienvenido, estamos conectados!" <<endl;
    }
    catch(SAException &x) {
        con.Rollback();
        cout << x.ErrNativeCode() << endl; // DEVUELVE EL CODIGO DE ERROR
        cout<<x.ErrText().GetMultiByteChars()<<endl;
        exit(-1);
    }

    int i =0;

}

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
	string pack;
	vector<string> packsContratados;
	double precio;
		    
	cout<<"Introduzca el telefono del cliente: ";
	cin>>tlfCliente;
	cout<<"\n¿Cuantos packs desea contratar el cliente?: ";
	cin>>numPacks;
	SAString aux;
	    for(int i=0; i<numPacks; i++){
		cout<<"\n\tIntroduzca el nombre del pack "<<i<<":";
		getline(cin,pack);
		aux=pack.c_str();
		packsContratados.push_back(aux);
	    }
	cout<<"\nIntroduzca la fecha de finalización del contrato (3 ints: dia mes anio): ";
	cin>>fechaFin[0]>>fechaFin[1]>>fechaFin[2];
	SADateTime f_fin(fechaFin[0], fechaFin[1], fechaFin[2]);
	cout<<"\nIntroduzca el precio del contrato: ";
	cin>>precio;	
		    
	contratoC = GenerarContratoCliente(tlfCliente, packsContratados, f_fin, precio, con);
	mostrarContratoProveedor(contratoP);	    
        break;
		    
      case 2: //Contrato proveedor
	int cif, numPeliculas;
	int[3] fechaFin;
	string pelicula;
	vector<string> peliculasParaActivar;
	double precio;
		    
	cout<<"Introduzca el CIF del proveedor: ";
	cin>>cif;
	cout<<"\n¿Cuantas peliculas desea contratar de las suministradas?: ";
	cin>>numPeliculas;
	SAString aux;
	    for(int i=0; i<numPeliculas; i++){
		cout<<"\n\tIntroduzca el nombre de la pelicula "<<i<<":";
		getline(cin,pelicula);
		aux=pelicula.c_str();
		peliculasParaActivar.push_back(aux);
	    }
	cout<<"\nIntroduzca la fecha de finalización del contrato (3 ints: dia mes anio): ";
	cin>>fechaFin[0]>>fechaFin[1]>>fechaFin[2];
	SADateTime f_fin(fechaFin[0], fechaFin[1], fechaFin[2]);
	cout<<"\nIntroduzca el precio del contrato: ";
	cin>>precio;	
		    
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
	char YN;
	
	do{
		cout << "Introduzca el CIF de su empresa: ";
		cin >> CIF;
		
		SACommand busqueda;
		busqueda.setConnection(con);
		busqueda.setCommandText(_TSA("SELECT COUNT(*) FROM PROVEEDOR WHERE CIF = :1"));
		busquda.Param(1).setAsInt64() = CIF;
		busqueda.Execute();
		
		if (busqueda.Fetch());
			n = busqueda[1].asInt64();
		else
			n = -1;
	}while(n!=1);
	
	do{
	cout << "Va a suministrar una película. Introduzca los siguientes datos: " << endl;
	cout << "Título: ";
	getline(cin,titulo);
	cout << "Año: ";
	cin >> anio;
	cout << "Director: ";
	getline(cin,director);
	cout << "Productora: ";
	getline(cin,productora);
	
	SuministrarPelicula(titulo,anio,director,productora,CIF,con);
		
	cout << "¿Quiere seguir suministrando películas? (Y/N)" << endl;
	cin >> YN;
	}while(YN=='Y');
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

//Subgrupo Azul: Irene, Gabriel, Nicolás y Gerardo
// REVISAR CUANDO commit
#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include <iostream>
#include <ctime>

#include "packs.h"
#include "clientes.h"
#include "peliculas.h"
#include "finanzas.h"

void MenuPrincipal(){
  cout << "Que desea:\n 1) Registrar Cliente\n 2) Iniciar Sesion cliente\n 3) Iniciar Sesion Empleado\n 4) Dar alta empresa\n 5) Menu empresa\n 6) Salir"
  // 1 Registrar Cliente
  // 2 Inciar Sesion cliente -> Menu cliente (ver pelicula, modficar datos, buscar catalago, mostrar catalog/recomendaciones, dar de baja y finalizar sesion)
  // 3 COSAS EMPLEADO (iniciar sesion "empleado") -> Menu empleado (submenu de packs, submenu finanzas)
  // 4 Dar alta Empresa
  // 5 COsas de la empresa -> MEnu empresa (solo suministrar peliculas)
  // 6 Salir
};

void MenuEmpleado(SAConnection* con){
  cout << "Que desea hacer:\n 1)Ingresar en el submenu de packs\n 2)Ingresar en el submenu finanzas\n 3) Salir" << endl;
  int i=0;
  while(i=!3){
    cout << "Que desea hacer:\n 1)Ingresar en el submenu de packs\n 2)Ingresar en el submenu finanzas\n 3) Salir" << endl;
    cin >> i;
    switch (i) {
      case 1:
        Submenupakcs(con);
        break;
      case 2:
        Submenufinanzas(con);
       break;
      case 3:
        cout << "Hasta otra" << endl;
       break;
      default:
        cout << "Opción Inválida" << endl;
        break;
    }
  }
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

	contratoC = GenerarContratoCliente(, con);
	mostrarContratoProveedor(contratoP);
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

void MenuCliente(int idSesion,SAConnection* con){

}


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
	getline(cin,titulo);
	cout << "Año: ";
	cin >> anio;
	cout << "Director: ";
	getline(cin,director);
	cout << "Productora: ";
	getline(cin,productora);

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
    con.setAutoCommit(SA_AutoCommitOff);
    int i = 0;

    while(i != 6){
      MenuPrincipal();
      cin >> i;
      switch (i) {
        case 1:
          string n; string c; unsigned int telf; string cor; char s; unsigned int t;
          int dd, aa, mm;
          cout << "Inserte su nombre" << endl;
          getline(cin, n);
          cout << "Inserte la contraseña" << endl;
          getline(cin, c);
          cout << "Inserte el correo" << endl;
          getline(cin, cor);
          cout << "Diga su sexo H (hombre), M (mujer) u O (otros) insertando un cáracter"<<endl;
          cin >> s;
          cout << "Escriba su dia mes y año de nacimiento, separado por espacios" << endl;
          cin >> dd; cin >> mm; cin >> aa;
          SADateTime f(dd,mm,aa);
          cout << "Escriba su numero de telefono  (9 digitos)" << endl;
          cin >> telf;
          cout << "Escriba su numero de tarjeta (16 digitos)" << endl;
          cin >> t;
          cout << "Se va a intentar crear el cliente" << endl;
          DarAltaCliente(n,c,telf,cor,s,f,t, &con);
          break;
        case 2:
          cout << "Ingrese su telefono: " << endl;
          string c; unsigned int telf; int idses;
          cin >> telf;
          cout << "Ingrese su contraseña: " << endl;
          getline(cin, c);
          cout << "Se intentara iniciar sesion" << endl;
          idses = IniciarSesion(telf, c, &con);
          if (idses < 0){
            cout << "Error al iniciar la sesion" << endl;
          }
          else{
            cout << "Sesion iniciada" << endl;
            MenuCliente(idses, &con);
          }

          break;
        case 3:

            break;
        case 4:

          break;
        case 5:

          break;
        case 6:
          break;
        default:
          cout << "Opción inválida" << endl;
          break;
      }
    };

}

//Subgrupo Azul: Irene, Gabriel, Nicolás y Gerardo
// REVISAR CUANDO commit
#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include <ctime>
#include <vector>
#include <utility>
#include <cstring>
#include <string>
#include <iostream>

#include "packs.h"
#include "clientes.h"
#include "peliculas.h"
#include "finanzas.h"

using namespace std;

void MenuPrincipal(){
  cout << "Que desea:\n 1) Registrar Cliente\n 2) Iniciar Sesion cliente\n 3) Iniciar Sesion Empleado\n 4) Dar alta empresa\n 5) Menu empresa\n 6) Salir"<< endl;
  // 1 Registrar Cliente
  // 2 Inciar Sesion cliente -> Menu cliente (ver pelicula, modficar datos, buscar catalago, mostrar catalog/recomendaciones, dar de baja y finalizar sesion)
  // 3 COSAS EMPLEADO (iniciar sesion "empleado") -> Menu empleado (submenu de packs, submenu finanzas)
  // 4 Dar alta Empresa
  // 5 Cosas de la empresa -> Menu empresa (solo suministrar peliculas)
  // 6 Salir
};

void Submenufinanzas(SAConnection* con){
  //1 generar contrato de un cliente
  //2 generar contrato empleado
  //3 balance de gastos
  //4 recibir pago de un cliente
  //5 pagar a un proveedor
  //6 Salir del submenu
  int opcion = 0;
  BalanceGastos balance;
  ContratoCliente contratoC;
  ContratoProveedor contratoP;

  while(opcion!=6){
    cout<<"Está usted en el submenú del de departamento de finanzas"<<endl;
    cout<<"\n\t1- Generar contrato cliente\n\t2- Generar contrato proveedor\n\t3- Balance de gastos\n\t4- Factura de un cliente\n\t5- Factura de un proveedor\n\t6- Salir del submenú"<<endl;
    cout<<"¿Qué desea hacer?: ";
    cin>>opcion;
    switch(opcion){
      case 1:{ //Contrato cliente
	int tlfCliente, numPacks;
	int fechaFin[3];
	string pack;
	vector<string> packsContratados;
	double precio;

	cout<<"\nIntroduzca el telefono del cliente: ";
	cin>>tlfCliente;
	cout<<"\n¿Cuantos packs desea contratar el cliente?: ";
	cin>>numPacks;

	    for(int i=0; i<numPacks; i++){
		cout<<"\n\tIntroduzca el nombre del pack "<<i<<":";
		getline(cin,pack);
		SAString aux(pack.c_str());
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
}
        break;

      case 2:{ //Contrato proveedor
	int cif, numPeliculas;
	int[3] fechaFin;
	string pelicula;
	vector<string> peliculasParaActivar;
	double precio;

	cout<<"\nIntroduzca el CIF del proveedor: ";
	cin>>cif;
	cout<<"\n¿Cuantas peliculas desea contratar de las suministradas?: ";
	cin>>numPeliculas;
	    for(int i=0; i<numPeliculas; i++){
		cout<<"\n\tIntroduzca el nombre de la pelicula "<<i<<":";
		getline(cin,pelicula);
		SAString aux(pack.c_str());
		peliculasParaActivar.push_back(aux);
	    }
	cout<<"\nIntroduzca la fecha de finalización del contrato (3 ints: dia mes anio): ";
	cin>>fechaFin[0]>>fechaFin[1]>>fechaFin[2];
	SADateTime f_fin(fechaFin[0], fechaFin[1], fechaFin[2]);
	cout<<"\nIntroduzca el precio del contrato: ";
	cin>>precio;

        contratoP = GenerarContratoProveedor(, con);
	mostrarContratoProveedor(contratoP);
}
        break;

      case 3: //Balance gastos
        balance = BalanceDeGastos(con);
	mostrarBalance(balance);
        break;
		    
      case 4: //Factura cliente
	FacturaCliente facturac;
	int tlfCliente;
	double precio;
	int[3] fechaPago;
		    
	cout<<"\nIntroduzca el telefono del cliente: ";
	cin>>tlfCliente;
	cout<<"\nIntroduzca el precio de la factura: ";
	cin>>precio;
	cout<<"\nIntroduzca la fecha de finalización de pago (3 ints: dia mes anio): ";
	cin>>fechaPago[0]>>fechaPago[1]>>fechaPago[2];
	SADateTime f_pago(fechaPago[0], fechaPago[1], fechaPago[2]);
		    
        facturac = RecibirPago(tlfCliente, precio, f_pago, con);
	mostrarFacturaCliente(facturac);
		    
        break;
	
      case 5: //Factura proveedor
	FacturaProveedor facturap;
	int cif;
	double precio;
	int[3] fechaPago;
		    
	cout<<"\nIntroduzca el CIF del proveedor: ";
	cin>>cif;
	cout<<"\nIntroduzca el precio de la factura: ";
	cin>>precio;
	cout<<"\nIntroduzca la fecha de finalización de pago (3 ints: dia mes anio): ";
	cin>>fechaPago[0]>>fechaPago[1]>>fechaPago[2];
	SADateTime f_pago(fechaPago[0], fechaPago[1], fechaPago[2]);
		    
        facturac = RecibirPago(precio, cif, f_pago, con);
	mostrarFacturaProveedor(facturap);
		    
        break;		    
    }
  }
};

void Submenupakcs(SAConnection* con){
  int opcion = 0;
  while(opcion != 7){
    cout<<"Está usted en el submenú de  packs, ¿Qué desea hacer?"<<endl;
    cout<<"1)Crear pack\n2)Modificar pack\n3)Inhabilitar película\n4)Desactivar pack\n5)Recibir recomendaciones\n6)Informar novedades\n7)Salir del menu"<<endl;
    cin >> opcion;
    switch (opcion) {
      case 1:{
        string nombre;
        cout << "Introduzca el nombre del pack a crear" << endl;
        getline(cin,nombre);
        cout << "Introduzca los id de las peliculas, con id=-1 se detendra la caputra, recuerdo que las peliculas tienen id no negativa" << endl;
        int idpeli; vector<int> lpelicula;
        do{
          cin >> idpeli;
          if(idpeli != -1)
            lpelicula.push_back(idpeli);
        }while(idpeli != -1);
        cout << "Introduzca el precio del pack, tendria que ser no negativo" << endl;
        double precio;
        cin >> precio;
        CrearPack(lpelicula, precio ,nombre ,con);
      }
      break;
      case 2:{
        string nombre;
        cout << "Introduzca el nombre del pack a modificar" << endl;
        getline(cin,nombre);
        cout << "Introduzca los id de las peliculas, con id=-1 se detendra la caputra, recuerdo que las peliculas tienen id no negativa" << endl;
        int idpeli; vector<int> lpelicula;
        do{
          cin >> idpeli;
          if(idpeli != -1)
            lpelicula.push_back(idpeli);
        }while(idpeli != -1);
        cout << "Introduzca el precio del pack, tendria que ser no negativo" << endl;
        double precio;
        cin >> precio;
        ModificarPack(lpelicula, precio ,nombre ,con);
      }
      break;
      case 3:{
        int idpeli;
        cout << "Introduzca el id de la pelicula a desactivar" << endl;
        cin >> idpeli;
        InhabilitarPelicula(idpeli, con);
      }
      break;
      case 4:{
        cout << "Introduzca el nombre del pack a desactivar" << endl;
        string nombre;
        getline(cin,nombre);
        DesactivarPack(nombre,con);
      }
      break;
      case 5:{
        cout << "Inserte el id de la pelicula y el telefono de la persona a la que se recomienda la pelicula, inserta la id y despues el telfono, si inserta id=-1 se detendra la captura" << endl;
        vector< pair <unsigned, unsigned > > clienteRPeliculas;
        int id_peli;
        int tel;
        do{
          cin >> id_peli;
          cin >> tel;
          if(id_peli!=-1){
            pair <unsigned, unsigned > init(id_peli,tel);
            clienteRPeliculas.push_back(init);
          }
        }while(id_peli != -1);
        RecibirRecomendaciones(clienteRPeliculas, con);
      }
      break;
      case 6:{
        int tam;
        cout << "Cuantos packs va a ingresar" << endl;
        cin >> tam;
        cout << "Escriba el nombre de los packs a informar"<< endl;
        vector<string> listaPacks;
        for(int i=0; i < tam; i++){
          string nombre;
          getline(cin,nombre);
          listaPacks.push_back(nombre);
        }
        InformarNovedades(listaPacks, con);
      }
      break;
      case 7:
        cout << "Cerrando menu" << endl;
      break;
      default:
        cout << "Opción incorrecta" << endl;
      break;
    }
  }
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

/*SUBMENU CLIENTES*/
void SubMenuCliente (int idSesion, int telefono, SAConnection *con){

cout<<"Está usted en el submenú del de departamento de finanzas, ¿Qué desea hacer?"<<endl;
  cout<<"\t1- Ver pelicula\n\t2- Modificar Datos\n\t3- Buscar Película\n\t4- Mostrar Catálogo\n\t5- Mostrar Recomendaciones\n\t6- Dar de baja\n\t7- Finalizar Sesión"<<endl;
  cin>>opcion;
  switch(opcion){
    case 1:{
      int idP;

      cout << "Introduzca el ID de la película que desea ver: ";
      cin >> idP;

      VerPelicula(idSesion, idP, con);
    }
    break;

    case 2: {//Modificar Datos
      string name, password, correo;
      int dd,mm,aa;
      unsigned int tarjeta;
      char sexo;
	    cout << "Introduzca los nuevos datos:\n\tNombre: ";
      cin >> name;
      cout << "\n\tContraseña: ";
      cin >> password;
      cout << "\n\tCorreo electrónico: ";
      cin >> correo;
      cout << "\n\tSexo(H/M/O): ";
      cin >> sexo;
      cout << "\n\tEscriba su dia mes y año de nacimiento, separado por espacios: ";
      cin >> dd; cin >> mm; cin >> aa;
      SADateTime f(dd,mm,aa);
      cout << "\n\Tarjeta: ";
      cin >> tarjeta;
      ModificarCliente(name, password, idSesion, correo, sexo, f, tarjeta, con)
    }
    break;

    case 3:{
      string tit_busc;

      cout << "Inserte el título de la película que quiera ver: ";
      getline(cin,tit_busc);

      BuscarTituloCatalogo (tit_busc, con);
    }
    break;

    case 4:
      MostrarCatalogo(con);
    break;

    case 5:
      MostrarRecomendaciones(telefono,con);
    break;

    case 6: //Dar de baja
      DarBajaCliente(idSesion, con);
    break;

    case 7: //Finalizar sesión
      FinalizarSesion(idSesion, con);
    break;
}

}

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
		busqueda.Param(1).setAsInt64() = CIF;
		busqueda.Execute();
		if (busqueda.FetchNext())
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
    con.setAutoCommit(SA_AutoCommitOff);
    int i = 0;

    string n; string c; string cor; char s; unsigned int t;
    nsigned int telf; int idses;
    string nombre; int telf; string correo; int cif;
    int dd, aa, mm;
    while(i != 6){
      MenuPrincipal();
      cin >> i;
      switch (i) {
        case 1:{

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
        }
          break;
        case 2:{
          cout << "Ingrese su telefono: " << endl;

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
            SubMenuCliente(idses,telf, &con);
          }
        }
          break;
        case 3:
          cout << "Esperemos que seas un empleado ;)" << endl;
          MenuEmpleado(&con);
          break;
        case 4:{

          cout << "Introduzca el CIF de su empresa: " << endl;
          cin >> cif;
          cout << "Introduzca el nombre de su empresa: "<<endl;
          getline(cin, nombre);
          cout << "Introduzca el telfono de su empresa"<< endl;
          cin >> telf;
          cout << "Introduzca el correo de su empresa: "<<endl;
          getline(cin, correo);
          SAString aux(nombre.c_str());
          SAString aux2(correo.c_str());
          cout << "Se intentara dar de alta a la empresa"<<endl;
          DarAltaEmpresa(aux, telf, aux2, cif, &con);
        }
          break;
        case 5:
          SubMenuSuministrar(&con);
          break;
        case 6:
          break;
        default:
          cout << "Opción inválida" << endl;
          break;
      }
    };
    con.Disconnect();
}

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


};

void Submenupakcs(SAConnection* con){

};

void MenuCliente(SAConnection* con){

}

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
          cout << "Ingrese su telefono:" << endl;

          cout << "Ingrese su contraseña:" << endl;

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

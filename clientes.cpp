/*
  Cliente(nombre, contrasena, telefono, correo, sexo, tarjeta)
*/

#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include "clientes.h"
#include <iostream>

using namespace std;

void DarAltaCliente(string n, string c, unsigned int telf, string cor, char s, string f, 
                 unsigned int t, SAConnection* con){
/*  if(n.length()<50 && c.length()<50 && to_string(telf).length()==9 && cor.length()<50 
     && (s=='F' || s=='M' || s=='O') && f.length()<50 && to_string(t).length()==16){*/
  SACommand guardado, crear;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVE POINT registrarcliente"));
    
  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }
    
  crear.setConnection(con);
  crear.setCommandText(_TSA("INSERT INTO cliente VALUES(:1, :2, :3, :4, :5, :6, :7)"));
  crear.Param(1).setAsString() = n;
  crear.Param(2).setAsString() = c;
  crear.Param(3).setAsInt64() = telf;
  crear.Param(4).setAsString() = cor;
  crear.Param(5).setAsChar() = s;
  crear.Param(6).setAsString() = f;
  crear.Param(7).setAsInt64() = t;
  
  try{
    crear.Execute();
  }
  catch(SAException &x){
    int i = x.ErrNativeCode();
    switch(i){
      case 1:
        cout << "Ya existía un cliente con ese teléfono, use ModificarCliente si quiere cambiar el nombre\n";
        break;
      case ___:
        cout << "ERROR NOT NULLS\n";
        break;
      /*case 20001:
        cout << "\n";
        break;*/
      default:
        cout << "Excepción no controlada\n" << endl;
        cout << x-ErrText().GetMultiByteChars()<< endl;
        break;
    }
    
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT registrarcliente"));
    guardado.Execute();
    return ;
  }
  //}
  con->commit();
};

void IniciarSesion(){
  
};

void FinalizarSesion(){
  
};

void DarBajaCliente(unsigned int telf, SAConnection* con){
  SACommand guardado, desactiva;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT darbajacliente"));
  
  try{
    guardado.execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }
  
  desactiva.setConnection(con);
  desactiva.setCommandText(_TSA("DELETE FROM CLIENTEACTIVO WHERE telefono=:1"));
  desactiva.Param(1).setAsInt64() = telf;
  
  try{
    desactiva.Execute();
  }
  catch(SAException &x){
    cerr << "Error a la hora de dar de baja al cliente\n";
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT darbajacliente"));
    guardado.Execute();
  }
  
  con->commit();
};

void ModificarCliente(){
  
};

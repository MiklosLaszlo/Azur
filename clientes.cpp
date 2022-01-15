/*
  Cliente(nombre, contrasena, telefono, correo, sexo, tarjeta)
*/

#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include "clientes.h"
#include <iostream>

using namespace std;

Cliente::Cliente(string n, string c, unsigned int telf, string cor, char s, string f, 
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
  crear.setCommandText(_TSA("INSERT INTO cliente VALUES(:1, :2, :3, :4, :5, :6, :7, :8)"));
  crear.Param(1).setAsString() = n;
  crear.Param(2).setAsString() = c;
  crear.Param(3).setAsUInt64() = telf;
  crear.Param(2).setAsString() = cor;
  crear.Param(2).setAsChar() = s;
  crear.Param(2).setAsString() = f;
  //}
}

void Cliente::IniciarSesion(){
  
}

void Cliente::FinalizarSesion(){
  
}

void Cliente::DarBajaCliente(){
  
}

void Cliente::ModificarCliente(){
  
}

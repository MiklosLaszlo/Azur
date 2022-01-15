#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include "clientes.h"
#include <iostream>

using namespace std;

Cliente::Cliente(string n, string c, int telf, string cor, char s, string f, int t){
  if(n.length()<50 && c.length()<50 && to_string(telf).length()==9 && cor.length()<50 
     && (s=='F' || s=='M' || s=='O') && f.length()<50 && to_string(t).length()==16){
    
  }
}

void Cliente::IniciarSesion(){
  
}

void Cliente::FinalizarSesion(){
  
}

void Cliente::DarBajaCliente(){
  
}

void Cliente::ModificarCliente(){
  
}

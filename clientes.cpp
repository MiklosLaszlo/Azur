/*
  Cliente(nombre, contrasena, telefono, correo, sexo, tarjeta)
*/

#include <stdio.h>  // for printf
#include <SQLAPI.h> // main SQLAPI++ header
#include "clientes.h"
#include <iostream>

using namespace std;

void DarAltaCliente(string n, string c, unsigned int telf, string cor, char s, SADateTime f, 
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
  
  SAString auxn(n.c_str());
  SAString auxc(c.c_str());
  SAString auxcor(cor.c_str());
  crear.setConnection(con);
  crear.setCommandText(_TSA("INSERT INTO cliente VALUES(:1, :2, :3, :4, :5, :6, :7)"));
  crear.Param(1).setAsString() = auxn;
  crear.Param(2).setAsString() = auxc;
  crear.Param(3).setAsInt64() = telf;
  crear.Param(4).setAsString() = auxcor;
  crear.Param(5).setAsChar() = s;
  crear.Param(6).setAsDateTime() = f;
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
      /*case ___:
        cout << "ERROR NOT NULLS\n";
        break;*/
      case 20010:
        cout << "El telefono debe tener 9 digitos\n";
        break;
      case 20011:
        cout << "La tarjeta debe tener 16 digitos\n";
        break;
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

int IniciarSesion(unsigned int telf, string pw, SAConnection* con){
  SACommand guardado, inicia, selectID;
  guardado.SetConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT iniciarSesion"));
  
  try{
    guardado.execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }
  
  inicia.setConnection(con);
  SAString auxc(pw.c_string());
  inicia.setCommandText(_TSA("SELECT * FROM CLIENTE WHERE (telefono = :1 AND contrasena = :2)"));
  inicia.Param(1).setAsInt64() = telf;
  inicia.Param(2).setAsString() = auxc;
  
  if(!(inicia.FetchNext())){
    cerr << "Telefono o contraseña incorrecta\n";
    return -1;
  }
  else{
    inicia.setCommandText(_TSA("INSERT INTO SESIONCLIENTESESION (telefono,horaInicio) VALUES(:1,:2)"));
    inicia.Param(1).setAsInt() = telf;
    inicia.Param(2).setAsDateTime() = SADateTime::currentDataTime();
    
    try{
      inicia.Execute();
    }
    catch(SAException &x){
      cerr << "Error al iniciar sesion\n";
      cerr << x.ErrText().GetMulriByteChars() << endl;
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT iniciarSesion"));
      guardado.Execute();
    }
    
    selectID.setCommand(_TSA("SELECT secuencia_suministrarIdSesion.currval FROM dual"));
    try{
      selectID.Execute();
      selectID.FetchNext();
    }
    catch(SAS &x){
      cerr << x.ErrText().GetMultiByetChars()<<endl;
    }
    
    int id = selectID.Param(1).asInt64();
  }
  con -> commit();
  return id;
};

void FinalizarSesion(int idSes, SAConnection* con){
  SACommand guardado, finalizaSesion, borraSesionActiva;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT finalizarsesion"));
  
  try{
    guardado.execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }
  
  finalizaSesion.setConnection(con);
  finalizaSesion.setCommandText(_TSA("UPDATE SESIONCLIENTESESION SET horaFin=:1"));
  finalizaSesion.param(1).setAsDateTime() = SADateTime::currentDataTime();
  
  try{
    finalizaSesion.Execute();
  }
  catch(SAException &x){
    cerr << "Error al finalizar sesion\n";
    cerr << x.ErrText().GetMulriByteChars() << endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT iniciarSesion"));
    guardado.Execute();
  }
  
  borraSesionActiva.setConnection(con);
  borraSesionActiva.setCommandText(_TSA("DELETE FROM SESIONACTIVA WHERE idSesion=:1"));
  borraSesionActiva.Param(1).setAsInt64() = idSes;
  
  try{
    borraSesionActiva.Execute();
  }
  catch(SASException &x){
    cerr << "Error a la hora de eliminar SesionActiva\n";
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT finalizarSesion"));
    guardado.Execute();
  }
  
  con->commit();
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

void ModificarCliente(string n, string c, unsigned int telf, string cor, char s, SADateTime f, unsigned int t, SAConnection* con){
  SACommand guardado, modificar;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT modifcliente"));
  
  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }
  
  SAString auxn(n.c_str());
  SAString auxc(c.c_str());
  SAString auxcor(cor.c_str());
  modificar.setConnection(con);
  modificar.setCommandText(_TSA("SELECT * FROM CLIENTE WHERE telefono = :1"));
  modificar.Param(1).setAsInt64() = telf;
  
  if(!(modificar.FetchNext())){
    cerr << "El cliente que se quiere modificar no pertenece a la base de datos\n";
  }
  else{
    modificar.setCommandText(_TSA("UPDATE CLIENTE SET nombre=:1, contraseña=:2, correo=:3, sexo=:4, fecha=:5, tarjeta=:6 WHERE telefono=:7"));
    modificar.Param(1).setAsString() = auxn;
    modificar.Param(2).setAsString() = auxc;
    modificar.Param(3).setAsString() = auxcor;
    modificar.Param(4).setAsChar() = s;
    modificar.Param(5).setAsDateTime() = auxf;
    modificar.Param(6).setAsInt64() = t;
    modificar.Param(7).setAsInt64() = telf;
    
    try{
      modificar.Execute();
    }
    catch(SAException &x){
      cerr << "Error al modificar alguno de los datos del cliente, se cancelaran todos los cambios\n";
      int i = x.ErrNativeCode();
      switch(i){
        case 20011:
          cout << "La tarjeta debe tener 16 cifras\n";
          break;
        default:
          cout << "Excepcion no controlada\n";
          cout<<x.ErrText().GetMultiByteChars()<<endl;
          break;
      }
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modificarcliente"));
      guardado.Execute();
      return ;
    }
  }
  
  con->commit();
    
};

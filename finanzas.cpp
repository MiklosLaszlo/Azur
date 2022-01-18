#include "finanzas.h"

using namespace std;

ContratoCliente GenerarContratoCliente(int tlf, vector<SAString> listaPacks, SADateTime fechaFin, double precio, SAConnection* con){
  ContratoCliente contrato;
  SACommand insertContrato, selectCliente, insertPack, selcetID;
  insertContrato.setConnection(con);
  selectCliente.setConnection(con);
  insertPack.setConnection(con);
  selectID.setConnection(con);

  //Consulto los datos del cliente, hay un trigger que verifica que el cliente sea activo
  selectCliente.setCommandText(_TSA("SELECT nombre,correoelectronico,sexo,fechanacimiento,tarjeta FROM Cliente where telefono=(:1)"));
  selectCliente.Param(1).setAsInt64()=tlf;
  try{
    selectCliente.Execute();
    selectCliente.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener los datos del cliente para generar el contrato" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  //Inserto el contrato, un trigger crea su id correspondiente
  insertContrato.setCommandText(_TSA("INSERT INTO firmaClienteContratocliente (fechainicio,fechafin,precio,telefono) VALUES (:1,:2,:3,:4)"));
  insertContrato.Param(1).setAsDateTime() = SADateTime::currentDateTime();
  insertContrato.Param(2).setAsDateTime() = fechaFin;
  insertContrato.Param(3).setAsDouble() = precio;
  insertContrato.Param(4).setAsInt64() = tlf;
  try{
    insertContrato.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al crear el contrato" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  selectID.setCommandText(_TSA("SELECT secuencia_contratoCliente.currval FROM dual"));
  try{
    selectID.Execute();
    selectID.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener el id del contrato" << endl;
    contrato.idContrato = -1;
    return contrato;
  }
  int id = selectID.Param(1).asInt64();

  //Un trigger comprueba que los packs contratados son activos
  insertPack.setCommandText(_TSA("INSERT INTO continen VALUES (:1,:2)"));
  insertPack.Param(1).setAsInt64() = id;
  try{
    for(int i=0; i<listaPacks.size(); i++){
      insertPack.Param(2).setAsString() = listaPacks[i];
      insertPack.Execute();
    }
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al insertar un supuesto pack activo" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  contrato={ id, selectCliente[1].asString(), tlf, selectCliente[2].asString(), selectCliente[3].asString(),
    selectCliente[4].asDateTime(), selectCliente.asDouble(), listaPacks, SADateTime::currentDateTime(), fechaFin, precio};
  return contrato;
}


void DarAltaEmpresa(SAString nombre, int tlf, SAString correo, int cif, SAConnection* con){
  SACommand insertProveedor;
  insertProveedor.setConnection(con);
  
  insertProveedor.setCommandText(_TSA("INSERT INTO proveedor (cif,nombreempresa,telefonoempresa,correoempresa) VALUES (:1,:2,:3,:4)"));
  insertProveedor.Param(1).setAsInt64() = cif;
  insertProveedor.Param(2).setAsString() = nombre;
  insertProveedor.Param(3).setAsInt64() = tlf;
  insertProveedor.Param(2).setAsString() = correo;
  try{
    inserProveedor.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al dar de alta el proveedor" << endl;
    return;
  }
  cout<<"\nProveedor añadido correctamente"<<endl;
}


ContratoProveedor GenerarContratoProveedor(int cif, vector<SAString> peliculas, double precio, SAConnection* con){
  ContratoProveedor contrato;
  return contrato;
}

FacturaCliente RecibirPago(int tlf, double precio, SADateTime fechaPago, SAConnection* con){
  FacturaCliente factura;
  return factura;
}

FacturaProveedor RealizarPago(double precio, int cif, SADateTime fechaPago, SAConnection* con){
  FacturaProveedor factura;
  return factura;
}

BalanceGastos BalanceDeGastos(SAConnection* con){
  BalanceGastos balance;
  retun balance;
}

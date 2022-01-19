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
    selectCliente[4].asDateTime(), selectCliente[5].asDouble(), listaPacks, SADateTime::currentDateTime(), fechaFin, precio};
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


ContratoProveedor GenerarContratoProveedor(int cif, vector<SAString> peliculas, SADateTime fechaFin, double precio, SAConnection* con){
  ContratoProveedor contrato;
  SACommand insertContrato, selectProveedor, selcetID;
  insertContrato.setConnection(con);
  selectProveedor.setConnection(con);
  selectID.setConnection(con);

  //Consulto los datos del proveedor
  selectProveedor.setCommandText(_TSA("SELECT nombreempresa,telefonoempresa,correoempresa FROM Proveedor where cif=(:1)"));
  selectProveedor.Param(1).setAsInt64()=cif;
  try{
    selectProveedor.Execute();
    selectProveedor.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener los datos del proveedor para generar el contrato" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  //Inserto el contrato, un trigger crea su id correspondiente
  insertContrato.setCommandText(_TSA("INSERT INTO firmaProveedorContratoProveedor (fechainicio,fechafin,precio,cif) VALUES (:1,:2,:3,:4)"));
  insertContrato.Param(1).setAsDateTime() = SADateTime::currentDateTime();
  insertContrato.Param(2).setAsDateTime() = fechaFin;
  insertContrato.Param(3).setAsDouble() = precio;
  insertContrato.Param(4).setAsInt64() = cif;
  try{
    insertContrato.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al crear el contrato" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  selectID.setCommandText(_TSA("SELECT secuencia_contratoProveedor.currval FROM dual"));
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

  contrato={ id, selectProveedor[1].asString(), selectProveedor[2].asInt64(), selectProveedor[3].asString(), cif, peliculas,
            SADateTime::currentDateTime(), fechaFin, precio};
  return contrato;
}

FacturaCliente RecibirPago(int tlf, double precio, SADateTime fechaPago, SAConnection* con){
  FacturaCliente factura;
  SACommand insertFactura, selcetID;
  insertFactura.setConnection(con);
  
  insertFactura.setCommandText(_TSA("INSERT INTO facturaClientePaga (telefono,precio,fecha) VALUES (:1,:2,:3)"));
  insertFactura.Param(1).setAsInt64() = tlf;
  insertFactura.Param(2).setAsDouble() = precio;
  insertFactura.Param(3).setAsDateTime() = fechaPago;
  try{
    insertFactura.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al insertar la factura del cliente" << endl;
    factura.idfacturac=-1;
    return factura;
  }
 
  selectID.setCommandText(_TSA("SELECT secuencia_facturaCliente.currval FROM dual"));
  try{
    selectID.Execute();
    selectID.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener el id de la factura del cliente" << endl;
    factura.idfacturac=-1;
    return factura;
  }
  int id = selectID.Param(1).asInt64(); 
  factura={id, precio, tlf, fechaPago};
  return factura;
}

FacturaProveedor RealizarPago(double precio, int cif, SADateTime fechaPago, SAConnection* con){
  FacturaProveedor factura;
  SACommand insertFactura, selcetID;
  insertFactura.setConnection(con);
  
  insertFactura.setCommandText(_TSA("INSERT INTO facturaProveedorRecibeDinero (cif,precio,fecha) VALUES (:1,:2,:3)"));
  insertFactura.Param(1).setAsInt64() = cif;
  insertFactura.Param(2).setAsDouble() = precio;
  insertFactura.Param(3).setAsDateTime() = fechaPago;
  try{
    insertFactura.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al insertar la factura del proveedor" << endl;
    factura.idfacturac=-1;
    return factura;
  }
 
  selectID.setCommandText(_TSA("SELECT secuencia_facturaProveedor.currval FROM dual"));
  try{
    selectID.Execute();
    selectID.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener el id de la factura del proveedor" << endl;
    factura.idfacturac=-1;
    return factura;
  }
  int id = selectID.Param(1).asInt64(); 
  factura={id, precio, cif, fechaPago};
  return factura;
}

BalanceGastos BalanceDeGastos(SAConnection* con){
  BalanceGastos balanceTotal;
  balanceTotal.balance=0;
  FacturaCliente fcliente;
  FacturaProveedor fproveedor;
  vector<FacturaCliente> fclientes;
  vector<FacturaProveedor> fproveedores;
  SACommand selectFClientes, selectFProveedores;
  selectFClientes.setConnection(con);
  selectFProveedores.setConnection(con);
  
  selectFClientes.setCommandText(_TSA("SELECT * FROM facturaClientePaga"));
  try{
    selectFClientes.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener las facturas de los clientes" << endl;
    balanceTotal.balance = -1;
    return balanceTotal;
  }
  
  while(selectFClientes.FetchNext()){
    fcliente = {selectFClientes[1].asInt64(), selectFClientes[3].asReal(), selectFClientes[4].asInt64(), selectFClientes[2].asDateTime()};
    fclientes.push_back(fcliente);
    balanceTotal.balance = balanceTotal.balance + selectFClientes[3].asReal();
  }
  balanceTotal.ingresos=fclientes;
  
  selectFProveedores.setCommandText(_TSA("SELECT * FROM facturaProveedorRecibeDinero"));
  try{
    selectFProveedores.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener las facturas de los proveedores" << endl;
    balanceTotal.balance = -1;
    return balanceTotal;
  }
  
  while(selectFProveedores.FetchNext()){
    fproveedor = {selectFProveedores[1].asInt64(), selectFProveedores[3].asReal(), selectFProveedores[4].asInt64(), selectFProveedores[2].asDateTime()};
    fproveedores.push_back(fproveedor);
    balanceTotal.balance = balanceTotal.balance - selectFProveedores[3].asReal();
  }
  balanceTotal.gastos=fproveedores;
  
  return balanceTotal;
}

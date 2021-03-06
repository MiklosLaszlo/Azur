#include "finanzas.h"

using namespace std;

ContratoCliente GenerarContratoCliente(int tlf, vector<SAString> listaPacks, SADateTime fechaFin, double precio, SAConnection* con){
  ContratoCliente contrato;
  SACommand insertContrato, selectCliente, insertPack, selectID;
  insertContrato.setConnection(con);
  selectCliente.setConnection(con);
  insertPack.setConnection(con);
  selectID.setConnection(con);
  if(listaPacks.size() < 1){
    cout << "Se quiere insertar un contrato de un cliente sin ningun pack, rechazando la inserccion" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  SACommand guardado;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT contratoCliente"));
  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

  //Consulto los datos del cliente, hay un trigger que verifica que el cliente sea activo
  selectCliente.setCommandText(_TSA("SELECT nombrecliente,correo,sexo,fechanacimiento,tarjeta FROM Cliente where telefono=:1"));
  selectCliente.Param(1).setAsInt64()=tlf;
  try{
    selectCliente.Execute();
    selectCliente.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener los datos del cliente para generar el contrato" << endl;
    contrato.idContrato = -1;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoCliente"));
    guardado.Execute();
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoCliente"));
    guardado.Execute();
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoCliente"));
    guardado.Execute();
    return contrato;
  }
  int id = selectID[1].asInt64();

  //Un trigger comprueba que los packs contratados son activos
  insertPack.setCommandText(_TSA("INSERT INTO contienen VALUES (:1,:2)"));
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoCliente"));
    guardado.Execute();
    return contrato;
  }

  contrato={ id, selectCliente[1].asString(), tlf, selectCliente[2].asString(), selectCliente[3].asString(),
    selectCliente[4].asDateTime(), selectCliente[5].asInt64(), listaPacks, SADateTime::currentDateTime(), fechaFin, precio};

  con->Commit(); //Hacemos el cambio permanente
  return contrato;
}


void DarAltaEmpresa(SAString nombre, int tlf, SAString correo, int cif, SAConnection* con){
  SACommand insertProveedor;
  insertProveedor.setConnection(con);

  cout << nombre.GetMultiByteChars() << " " << tlf << " " << correo.GetMultiByteChars() << " " << cif << endl;
  insertProveedor.setCommandText(_TSA("INSERT INTO proveedor (cif,nombreempresa,telefonoempresa,correoempresa) VALUES (:1,:2,:3,:4)"));
  insertProveedor.Param(1).setAsInt64() = cif;
  insertProveedor.Param(2).setAsString() = nombre;
  insertProveedor.Param(3).setAsInt64() = tlf;
  insertProveedor.Param(4).setAsString() = correo;
  try{
    insertProveedor.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al dar de alta el proveedor" << endl;
    return;
  }
  cout<<"\nProveedor añadido correctamente"<<endl;
  con->Commit(); //Hacemos el cambio permanente
}


ContratoProveedor GenerarContratoProveedor(int cif, vector<SAString> peliculas, SADateTime fechaFin, double precio, SAConnection* con){
  ContratoProveedor contrato;
  SACommand insertContrato, selectProveedor, selectID, insertActiva;
  insertContrato.setConnection(con);
  selectProveedor.setConnection(con);
  selectID.setConnection(con);
  insertActiva.setConnection(con);
  if(listaPacks.size() < 1){
    cout << "Se quiere insertar un contrato de un proveedor sin ninguna pelicula, rechazando la inserccion" << endl;
    contrato.idContrato = -1;
    return contrato;
  }
  SACommand guardado;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT contratoProveedor"));
  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    contrato.idContrato = -1;
    return contrato;
  }

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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoProveedor"));
    guardado.Execute();
    return contrato;
  }

  //Inserto el contrato, un trigger crea su id correspondiente
  insertContrato.setCommandText(_TSA("INSERT INTO FIRMACP (fechainicio,fechafin,precio,cif) VALUES (:1,:2,:3,:4)"));
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoProveedor"));
    guardado.Execute();
    return contrato;
  }
  cout << "h" << endl;
  selectID.setCommandText(_TSA("SELECT secuencia_contratoProveedor.currval FROM dual"));
  try{
    selectID.Execute();
    selectID.FetchNext(); //Dispongo la información del select
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener el id del contrato" << endl;
    contrato.idContrato = -1;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoProveedor"));
    guardado.Execute();
    return contrato;
  }
  int idContrato = selectID[1].asInt64();

  int idPelicula;
  for(int i=0; i<peliculas.size(); i++){//Activo las peliculas contratadas
    selectID.setCommandText(_TSA("SELECT idpelicula FROM suministrapelicula WHERE titulo=:1 AND cif=:2"));
    selectID.Param(1).setAsString() = peliculas[i];
    selectID.Param(2).setAsInt64() = cif;
    try{
      selectID.Execute();
      selectID.FetchNext(); //Dispongo la información del select
      idPelicula = selectID[1].asInt64();
    }
    catch(SAException &x){
      cerr<<x.ErrText().GetMultiByteChars()<<endl;
      cerr<<"Error al obtener el id de la pelicula para activarla" << endl;
      contrato.idContrato = -1;
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoProveedor"));
      guardado.Execute();
      return contrato;
    }
    insertActiva.setCommandText(_TSA("INSERT INTO Activa (idpelicula,idContratoProveedor) VALUES (:1,:2)"));
    insertActiva.Param(1).setAsInt64() = idPelicula;
    insertActiva.Param(2).setAsInt64() = idContrato;
    try{
      insertActiva.Execute();
    }
    catch(SAException &x){
      cerr<<x.ErrText().GetMultiByteChars()<<endl;
      cerr<<"Error al crear activar la pelicula" << endl;
      contrato.idContrato = -1;
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT contratoProveedor"));
      guardado.Execute();
      return contrato;
    }
  }

  contrato={ idContrato, selectProveedor[1].asString(), selectProveedor[2].asInt64(), selectProveedor[3].asString(), cif, peliculas,
            SADateTime::currentDateTime(), fechaFin, precio};

  con->Commit(); //Hacemos el cambio permanente
  return contrato;
}

FacturaCliente RecibirPago(int tlf, double precio, SADateTime fechaPago, SAConnection* con){
  FacturaCliente factura;
  SACommand insertFactura, selectID,guardado;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT modifcliente"));
  selectID.setConnection(con);
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modificarcliente"));
    guardado.Execute();
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modificarcliente"));
    guardado.Execute();
    factura.idfacturac=-1;
    return factura;
  }
  int id = selectID[1].asInt64();
  factura={id, precio, tlf, fechaPago};

  con->Commit(); //Hacemos el cambio permanente
  return factura;
}

FacturaProveedor RealizarPago( int cif, double precio, SADateTime fechaPago, SAConnection* con){
  FacturaProveedor factura;
  SACommand insertFactura, selectID, guardado;
  insertFactura.setConnection(con);
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT modifcliente"));
  selectID.setConnection(con);
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modificarcliente"));
    guardado.Execute();
    factura.idfacturap=-1;
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
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modificarcliente"));
    guardado.Execute();
    factura.idfacturap=-1;
    return factura;
  }
  int id = selectID[1].asInt64();
  factura={id, precio, cif, fechaPago};

  con->Commit(); //Hacemos el cambio permanente
  return factura;
}

BalanceGastos BalanceDeGastos(SAConnection* con){
  BalanceGastos balanceTotal;
  balanceTotal.balance=0;
  balanceTotal.balanceCorrecto=true;
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
    balanceTotal.balanceCorrecto = false;
    return balanceTotal;
  }

  while(selectFClientes.FetchNext()){
    fcliente = {selectFClientes[1].asInt64(), selectFClientes[3].asDouble(), selectFClientes[4].asInt64(), selectFClientes[2].asDateTime()};
    fclientes.push_back(fcliente);
    balanceTotal.balance = balanceTotal.balance + selectFClientes[3].asDouble();
  }
  balanceTotal.ingresos=fclientes;

  selectFProveedores.setCommandText(_TSA("SELECT * FROM facturaProveedorRecibeDinero"));
  try{
    selectFProveedores.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error al obtener las facturas de los proveedores" << endl;
    balanceTotal.balanceCorrecto = false;
    return balanceTotal;
  }

  while(selectFProveedores.FetchNext()){
    fproveedor = {selectFProveedores[1].asInt64(), selectFProveedores[3].asDouble(), selectFProveedores[4].asInt64(), selectFProveedores[2].asDateTime()};
    fproveedores.push_back(fproveedor);
    balanceTotal.balance = balanceTotal.balance - selectFProveedores[3].asDouble();
  }
  balanceTotal.gastos=fproveedores;

  return balanceTotal;
}

void mostrarContratoCliente(ContratoCliente contrato){
  cout<<"Datos del contrato generado para el cliente"<<endl;
  cout<<"\tidContrato:"<<contrato.idContrato<<"\n\tNombre del cliente:"<<contrato.nombreCliente.GetMultiByteChars()<<endl;
  cout<<"\tTelefono:"<<contrato.tlfCliente<<"\n\tCorreo:"<<contrato.correo.GetMultiByteChars()<<endl;
  cout<<"\tSexo:"<<contrato.sexo.GetMultiByteChars()<<"\n\tFecha nacimiento:"<<contrato.fechaNacimiento.GetDay()<<"/"<<contrato.fechaNacimiento.GetMonth()<<"/"<<contrato.fechaNacimiento.GetYear()<<endl;
  cout<<"\tNum tarjeta:"<<contrato.tarjeta<<"\n\tPacks contratados:";
    for(int i=0; i<contrato.packsContratados.size(); i++){
      cout<<"\n\t\t"<<contrato.packsContratados[i].GetMultiByteChars();
    }
  cout<<"\n\tFecha inicio:"<<contrato.fechaInicio.GetDay()<<"/"<<contrato.fechaInicio.GetMonth()<<"/"<<contrato.fechaInicio.GetYear();
  cout<<"\n\tFecha fin:"<<contrato.fechaFin.GetDay()<<"/"<<contrato.fechaFin.GetMonth()<<"/"<<contrato.fechaFin.GetYear();
  cout<<"\n\tPrecio:"<<contrato.precio<<endl;
}

void mostrarContratoProveedor(ContratoProveedor contrato){
  cout<<"Datos del contrato generado para el proveedor"<<endl;
  cout<<"\tidContrato:"<<contrato.idContrato<<"\n\tNombre del proveedor:"<<contrato.nombreProveedor.GetMultiByteChars()<<endl;
  cout<<"\tTelefono:"<<contrato.tlfProveedor<<"\n\tCorreo:"<<contrato.correo.GetMultiByteChars()<<endl;
  cout<<"\tCIF:"<<contrato.cif<<"\n\tPeliculas a activar:";
    for(int i=0; i<contrato.peliculasAActivar.size(); i++){
      cout<<"\n\t\t"<<contrato.peliculasAActivar[i].GetMultiByteChars();
    }
  cout<<"\n\tFecha inicio:"<<contrato.fechaInicio.GetDay()<<"/"<<contrato.fechaInicio.GetMonth()<<"/"<<contrato.fechaInicio.GetYear();
  cout<<"\n\tFecha fin:"<<contrato.fechaFin.GetDay()<<"/"<<contrato.fechaFin.GetMonth()<<"/"<<contrato.fechaFin.GetYear()<<endl;
  cout<<"\n\tPrecio:"<<contrato.precio<<endl;
}

void mostrarFacturaCliente(FacturaCliente factura){
  cout<<"Factura de cliente"<<endl;
  cout<<"\n\tidFactura:"<<factura.idfacturac<<"\n\tPrecio:"<<factura.precio<<"\n\tTelefono cliente:"<<factura.tlfCliente;
  cout<<"\n\tFecha pago:"<<factura.fechaPago.GetDay()<<"/"<<factura.fechaPago.GetMonth()<<"/"<<factura.fechaPago.GetYear()<<endl;
}

void mostrarFacturaProveedor(FacturaProveedor factura){
  cout<<"Factura de proveedor"<<endl;
  cout<<"\n\tidFactura:"<<factura.idfacturap<<"\n\tPrecio:"<<factura.precio<<"\n\tCIF:"<<factura.cif;
  cout<<"\n\tFecha pago:"<<factura.fechaPago.GetDay()<<"/"<<factura.fechaPago.GetMonth()<<"/"<<factura.fechaPago.GetYear()<<endl;
}

void mostrarBalance(BalanceGastos balanceTotal){
  cout<<"Datos del balance de gastos"<<endl;
  cout<<"Ingresos:"<<endl;
    for(int i=0; i<balanceTotal.ingresos.size(); i++)
      mostrarFacturaCliente(balanceTotal.ingresos[i]);
  cout<<"Gastos:"<<endl;
    for(int i=0; i<balanceTotal.gastos.size(); i++)
      mostrarFacturaProveedor(balanceTotal.gastos[i]);
  cout<<"Balance: "<<balanceTotal.balance<<endl;
}

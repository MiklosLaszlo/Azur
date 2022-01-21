--CREAR TABLAS y sequences
CREATE TABLE FIRMACLIENTECONTRATOCLIENTE(
    idContratoCliente INT PRIMARY KEY,
    fechainicio DATE NOT NULL,
    fechafin DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    telefono INT,
    FOREIGN KEY telefono REFERENCES CLIENTE(telefono)
);

CREATE SEQUENCE secuencia_contratoCliente; --Genera el id de la tabla de contrato-cliente

CREATE TABLE CONTIENEN(
    idContratoCliente INT,
    nombrepack varchar(20),
    FOREIGN KEY idContratoCliente REFERENCES FIRMACLIENTECONTRATOCLIENTE(idContratoCliente),
    FOREIGN KEY nombrepack REFERENCES PACK(nombrepack),
);

CREATE TABLE PROVEEDOR(
    cif INT PRIMARY KEY,
    nombreempresa varchar(20) NOT NULL,
    telefonoempresa INT NOT NULL,
    correoempresa varchar(20) NOT NULL
);

CREATE TABLE FIRMACP(
    idContratoProveedor INT PRIMARY KEY,
    fechainicio DATE NOT NULL,
    fechafin DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    cif INT,
    FOREIGN KEY cif REFERENCES PROVEEDOR(cif)
);

CREATE SEQUENCE secuencia_contratoProveedor; --Genera el id de la tabla de contrato-proveedor

CREATE TABLE FACTURACLIENTEPAGA(
    idfacturac INT PRIMARY KEY,
    fecha DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    telefono INT,
    FOREIGN KEY telefono REFERENCES CLIENTE(telefono)
);

CREATE SEQUENCE secuencia_facturaCliente; --Genera el id de la tabla de factura-cliente

CREATE TABLE FACTURAPROVEEDORRECIBEDINERO(
    idfacturap INT PRIMARY KEY,
    fecha DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    cif INT,
    FOREIGN KEY cif REFERENCES PROVEEDOR(cif)
);

CREATE SEQUENCE secuencia_facturaProveedor; --Genera el id de la tabla de factura-proveedor

--TRIGGERS
CREATE OR REPLACE TRIGGER insertar_ContratoCliente --Crea id contrato cliente y comprueba que el cliente es activo
  before insert on firmaClienteContratoCliente
  for each row
    DECLARE
      cli INTEGER
    BEGIN
      SELECT COUNT(*) INTO cli FROM ClienteActivo WHERE telefono=new.telefono; --el tlf que quiero insertar
      IF (cli<1) THEN
        RAISE_APPLICATION_ERROR(-20100, 'INTENTO DE CREAR CONTRATO A UN CLIENTE NO ACTIVO')
      END IF
      SELECT secuencia_contratoCliente.nextval INTO :new.idContratoCliente FROM dual;
    END insertar_ContratoCliente;
/

CREATE OR REPLACE TRIGGER insertar_ContratoProveedor --Crea id contrato proveedor
  before insert on FIRMACP
  for each row
    BEGIN
      SELECT secuencia_contratoProveedor.nextval INTO :new.idContratoProveedor FROM dual;
    END insertar_ContratoProveedor;
/

CREATE OR REPLACE TRIGGER insertar_FacturaCliente --Crea id factura cliente
  before insert on facturaClientePaga
  for each row
    BEGIN
      SELECT secuencia_facturaCliente.nextval INTO :new.idfacturac FROM dual;
    END insertar_FacturaCliente;
/

CREATE OR REPLACE TRIGGER insertar_FacturaProveedor --Crea id factura proveedor
  before insert on facturaProveedorRecibeDinero
  for each row
    BEGIN
      SELECT secuencia_facturaProveedor.nextval INTO :new.idfacturap FROM dual;
    END insertar_FacturaProveedor;
/

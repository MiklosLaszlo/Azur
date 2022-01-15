SELECT table_name FROM user_tables;

DROP TRIGGER precio_correcto;

DROP TABLE PACK;

-- CREAR tablas --

CREATE TABLE CLIENTE(
    nombreCliente varchar2(50) NOT NULL,
    contrasena varchar2(20) NOT NULL,
    telefono INT PRIMARY KEY,
    correo varchar2(50) NOT NULL,
    sexo CHAR,
    fechanacimiento DATE,
    tarjeta INT NOT NULL
);

CREATE TABLE CLIENTEACTIVO(
    telefono INT,
    FOREIGN KEY (telefono) REFERENCES cliente(telefono),
    PRIMARY KEY(telefono)
);

CREATE TABLE PACK(
    nombrePack varchar2(20) PRIMARY KEY,
    precio FLOAT NOT NULL
);

CREATE TABLE PACKACTIVO (
    nombrePack varchar2(20),
    FOREIGN KEY (nombrePack) REFERENCES pack(nombrePack),
    PRIMARY KEY(nombrePack)
);

-- Disparadores --

-- Comprueba que el precio sea correcto en los pack --
CREATE OR REPLACE TRIGGER precio_correcto
BEFORE INSERT OR UPDATE ON PACK
FOR EACH ROW WHEN(new.precio < 0)
BEGIN
    RAISE_APPLICATION_ERROR(-20001, 'PRECIO INVALIDO');
END precio_correcto;
/

-- Cuando se modifique o se cree un pack este se activa, en caso de que no este activado ya --
CREATE OR REPLACE TRIGGER crearPackActivo
AFTER INSERT OR UPDATE ON PACK
FOR EACH ROW
DECLARE
    cuantos INTEGER;
BEGIN
    SELECT COUNT(*) INTO cuantos FROM PACKACTIVO WHERE nombrepack= :new.nombrepack;
    IF(cuantos < 1) THEN
        INSERT INTO PACKACTIVO VALUES(:new.nombrepack);
    END IF;
END crearPackActivo;
/

-- Comprueba que cuando se inserta una recomendacion esta esta inicialmente recomendada a un cliente activo y a una pelicula activa --
CREATE OR REPLACE TRIGGER precio_correcto
BEFORE INSERT ON PACK
FOR EACH ROW WHEN(new.precio < 0)
BEGIN
    RAISE_APPLICATION_ERROR(-20001, 'PRECIO INVALIDO');
END precio_correcto;
/

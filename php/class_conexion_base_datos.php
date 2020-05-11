<?php
/*********************************************
	Fichero class_conexion_base_datos.php
**********************************************/
class MiBasedeDatos{

	private $servername;
	private $username;
	private $password;
	private $database="";
	private $conexion;
	private $conectada;

	function  __construct($param_bd)
	{
		/*
		Al constructor hay que pasarle un array de parámetros con los índices correctos
		/ Parámetros de la base de datos de clima en mi estudio
		$param_bd = array(
		"servername" => "host",
		"username"   => "usuario de la base de datos",
		"password"   => "password",
		"database"   => "nombre de la base de datos");
		*/
		$this->servername = $param_bd["servername"];
		$this->username = $param_bd["username"];
		$this->password = $param_bd["password"];
		$this->database = $param_bd["database"];
		$this->conectada=false;
	}
	function conectada(){return $this->conectada;}//devuelve un booleano true si está conectada y false si no lo está.
	function conectarBD($param_bd)//Intenta conectar con la base de datos, devuelve el resulado del intento.
	{ 
		//variable que guarda la conexión de la base de datos
		$this->conexion = mysqli_connect($this->servername, $this->username, $this->password, $this->database); 
		//Comprobamos si la conexión ha tenido exito
		if(!$this->conexion)
		{
			echo 'Ha sucedido un error inexperado en la conexion de la base de datos<br>';
			$this->conectada=false;
		} 
		else {$this->conectada=true;}
	//devolvemos el objeto de conexión para usarlo en las consultas  
		return $this->conexion; 
	}  
	//...................................................................................................
	function desconectarBD()    //Desconectar la conexion a la base de datos
	{
		//Cierra la conexión y guarda el estado de la operación en una variable
		if($this->conectada)
		{
			$close = mysqli_close($this->conexion); //true si cierra, false si no
			$this->conectada=false;
		}
		//Comprobamos si se ha cerrado la conexión correctamente
		if(!$close)
		{  
			echo 'Ha sucedido un error inexperado en la desconexion de la base de datos<br>'; 
		}    
		//devuelve el estado del cierre de conexión
		return $close;         
	}
	//...................................................................................................
	//Devuelve un array multidimensional con el resultado de una consulta
	function consulta($sql)
	{
		$array_datos = array();
		if(!$resultado = mysqli_query($this->conexion, $sql)) die();
		//guardamos en un array multidimensional todos los datos de la consulta
		$i=0;
		while($row = mysqli_fetch_array($resultado))
		{   
			//guardamos en array_datos todos los vectores/filas que nos devuelve la consulta
			$array_datos[$i] = $row;
			$i++;
		}
	return $array_datos;
	}
};
//****************************************************************************************************
?>
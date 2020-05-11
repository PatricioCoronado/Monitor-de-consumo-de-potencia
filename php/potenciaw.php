<!-- versión 17/04/2020 V1.0 -->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

<?php //Título
if(!empty($_GET["origen"])) {printf("<title> potencia en %s </title>",$_GET["origen"]);}
else printf("<title> potencia en %s </title>","potencia");
?>
<!-- <link rel="icon" type="image/jpg" href="images/TyH.jpg" /> -->
</head>
<body>
<?php
/***********************************************************************************************
		Fichero potenciaw.php
		Pone en una base de datos los valore recibidos o los muestra en una gráfica
************************************************************************************************/
//Si no se recibe ni potencia no origen salimos sin hacer nada
if (empty($_GET["potencia"]) && empty($_GET["origen"])) {die("#SIN_DATOS<br>");}// Si falta "ubicacion" y/o datos "valor1" y "valor2" da un error
//
require_once("parametros_bd_PACO_Datos.php");// para tener disponible el array de parámetros de $parametros_bd
require_once('class_conexion_base_datos.php');//Conexión a base de datos
date_default_timezone_set('Europe/Madrid');
//Busca el dato de potencia
if(!empty($_GET["potencia"]))//Dato fundamental, si no está no hay nada que guardar
{
	$svalor="";
	$svalor=$_GET["potencia"];//Lee la medida recibida en una cadena
	sscanf($svalor,"%f",$potencia);//Extrae el float $potencia
	//Mira a ver si hay irigen de la potencia
	if(!empty($_GET["origen"]))
	{
		$origen=$_GET["origen"];//Lee el origen del dato
	}
	else $origen="desconocido";
	//Mira a ver si hay un voltage de pila
	if(!empty($_GET["pila"]))	//Si se envía el voltaje de la pila...
	{	
		$valor=$_GET["pila"];
		sscanf($valor,"%f",$pila);//Se lee
	}
	else $pila=0.0;
//....INTRODUCE DATOS EN LA TABLA "consumo-electrico" ........	
	$fecha= date('Y-m-d H:i:s');//Leemos la fecha actual
	//Creamos la conexión con la base de datos
	$MiBdD = new MiBasedeDatos($parametros_bd);
	//Conecta con la base de datos
	$conn=$MiBdD->conectarBD($parametros_bd);
	// Testea la conexión, si falla sale del script
	if (!$MiBdD->conectada()){die("#FALLO_CONEXION_BD");}
	//Si no falla la conexión introducimos los datos en la b. de d.
	$sql ="INSERT INTO `consumo-electrico`(`potencia`,`fechayhora`,`pila`,`origen`)	VALUES('$potencia','$fecha','$pila','$origen')";
	if (mysqli_query($conn, $sql)) //Ejecuta el query
	{echo "#OK";}//Si ejecuta el query escribe #OK en la página
	 else{echo "#ERROR";}//si falla escribe error
	$MiBdD->desconectarBD();//Cierra la base de datos
	die("");
}
//Si no se envía la potencia pero si el ogigen, se plotea una gráfica
if(empty($_GET["potencia"]) && !empty($_GET["origen"])) //Si no hay potencia pero si origen...
{
	// Conectamos, extraemos datos y cerramos.
	$MiBdD = new MiBasedeDatos($parametros_bd);
	$conn=$MiBdD->conectarBD($parametros_bd);//Conecta con la base de datos
	if (!$MiBdD->conectada())
	{
		die("fallo de conexion para leer");
	}// Testea la conexión, si falla sale del script
	else 
	{
		//printf("conexión exitosa");
	}
	//Selecciona la en la tabla "origen de datos" en función de la ubicación
	$ubicacion=$_GET["origen"];
	$sqlQuery = "SELECT potencia,fechayhora FROM `consumo-electrico` WHERE origen = ". "\"".$ubicacion."\"";	
	$datos=$MiBdD->consulta($sqlQuery); //Leemos los datos solicitados en "datos"
	$MiBdD->desconectarBD();//Desconectamos la base de datos
	//Determina las muestras a plotear
	//Plotea todas las muestras
	$num_filas = count($datos);//número de filas leidas

	/*------------------Esto es para trimar el número de muestras ------------------
	//El número de días a mostrar será el mínimo ($muestras, $num_filas)
	$muestras = 1000; //Selecciona el número de dias a mostrar

	if($num_filas > ($muestras+1) ) $num_filas=$muestras;
	{
		$muestras=$num_filas;
	}
	--------------------------------------------------------------------------------*/
	//Variables para plotear mostrar
	$potencia;//Array de datos del eje y (potencia)
	$tiempos; //Array de datos del eje z (tiempo)
	//en un bucle for obtenemos en cada iteración el valor númerico y 
	//el TIMESTAMP del tiempo y lo almacenamos en los arrays 
	for($i = 0 ;$i<$num_filas;$i++)
	{
		$potencia[$i]= $datos[$i][0];//Leemos la temperatura
		$time= $datos[$i][1];//leemos el tiempo en formato Y-M-D h:m:s ej: 2020-05-07 16:02:15
		$date = new DateTime($time);//cambiamos el formato del tiempo
		$tiempos[$i] = $date->getTimestamp()*1000;//Tiempo en formato epoch*1000 (segundos a milisegundos)
	}
}
//die();
?>
<!--------------------------- Gráfica de la evolución de la potencia  ------------------->
<!------ librerias en java  ----------->
<script src="https://code.jquery.com/jquery.js"></script>
<!-- Importo el archivo Javascript de Highcharts directamente desde su servidor -->
<script src="https://code.highcharts.com/stock/highstock.js"></script>
<script src="https://code.highcharts.com/modules/exporting.js"></script>
<!------------------------------------->
<div id="contenedor_temperatura"></div>
<script>
Highcharts.setOptions({
    time: {
        timezone: 'Europe/Madrid'
    }
});
//  
chartCPU = new Highcharts.StockChart
({
    chart: {renderTo: 'contenedor_temperatura'},
    rangeSelector : {enabled: false},
    title: {text: 'potencia'},
    xAxis: {type: 'datetime'},
    yAxis: {minPadding: 0.2,maxPadding: 0.2,title: {text: 'Vatios', margin: 10}},
    series: [{name: 'valor',data: (function() {var data = [];
                <?php
                    for($i = 0 ;$i<count($datos);$i++){
                ?>
                data.push([<?php echo $tiempos[$i];?>,<?php echo $potencia[$i];?>]);
                <?php } ?>
                return data;
            						})()
    	   }],
	 credits: {enabled: false}
});
</script>   
<!------------------------Fin gráfica de la evolución de la potencia  ------------------->
</body>
</html>
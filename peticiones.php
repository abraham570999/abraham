<?php 

require 'bd/conexion_bd.php';

$obj = new BD_PDO();

if(isset($_GET['ilum'])&&isset($_GET['temp'])&&isset($_GET['hum'])){
	$obj->Ejecutar_Instruccion("INSERT INTO datos_iot (iluminacion, temperatura, humedad, proximidad, fecha, hora) values ('".$_GET['ilum']."','".$_GET['temp']."', '".$_GET['hum']."','".$_GET['proximidad']."' Now(), DATE_FORMAT(NOW( ), '%H:%i:%S'))");

     	
} 

 ?>
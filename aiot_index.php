<!DOCTYPE html>
<html lang="en">
<head>
 <meta charset="UTF-8">
 <meta name="viewport" content="width=device-width, initial-scale=1.0">
 <title>Datos Aiot</title>
<!-- Bootstrap core CSS -->
 <link href="vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script>
function cargarDatos1()
{

//$.getJSON("http://localhost/iot/oficina/Datos_Arduino/info_arduino.php").done(function(datos

 $.getJSON("http://192.168.0.10/").done(function(datos)
 {
 $("#tempVal").text(datos.temperatura);
 $("#humedadVal").text(datos.humedad);
 $("#iluminacionVal").text(datos.iluminacion);
 $("#proximidadVal").text(datos.proximidad);
 var estatus_iluminacion = datos.iluminacion;
 var btniluminacion = document.getElementById("btniluminacion");
 if(estatus_iluminacion == "ON")
 {
 btniluminacion.setAttribute("Value","ON");
 btniluminacion.setAttribute("Class","btn btn-success");
 }
 else
 {
 btniluminacion.setAttribute("Value","OFF");
 btniluminacion.setAttribute("Class","btn btn-danger");
 }
 });
}
function iluminacion(valor)
{
 //En la línea siguiente cambiamos el status del relay que enciende o apaga la ventilación
 if (valor == "ON")
 {
 v = "L";
 }
 else
 {
 v = "H";
 }
$.getJSON("http://192.168.0.10/" + v).done(function(datos)
{
});
//Herificar el nuevo estatus
$.getJSON("http://192.168.0.10/").done(function(datos)
 {
 $("#tempVal").text(datos.temperatura);
 $("#humedadVal").text(datos.humedad);
 $("#iluminacionVal").text(datos.iluminacion);
 $("#proximidadVal").text(datos.proximidad);
var valor = document.getElementById("btniluminacion");
var ilum = datos.iluminacion;
});
location.reload(true);
}
</script>
</head>
<style>
	body {
  font-family: "Brush Script MT", cursive;
}
</style>
</head>
<body onLoad="cargarDatos();" style="background-color: #9370DB; text-align: center;">
<section id="inicio" style="background-image: url('img/2.jpg')">
 <br>
 <div class="container" >
 <div class="row">
 <div class="col-lg-12 text-center" style="color: white; text-align: center;">
 <h1 class="mt-5" style="color:pink ">CONTROL DE TEMPERATURA, HUMEDAD, PROXIMIDAD E ILUMINACION
</h1>

 </div>
 </div>
 </div>
 <br>
</section>
<section id="informacion" >
 <div class="container" >
 <div class="row">
 <div class="col-lg-6">
<h3>Iluminacion: <span id="iluminacionVal"></span></span></h3>
 <h3></h3>
 <h3>Temperatura: <span id="tempVal"></span></h3>
 <h3></h3>
 <h3>Humedad: <span id="humedadVal"></span> </h3>
 <h3></h3>
 <h5>Proximidad: <span id="proximidadVal"></span> </h5>
 <h3></h3>
 </div>
 </div>
 <div class="row">
 <div class="col-lg-3">
 </div>
 <div class="-col-lg-6 text-center">
 	<input type="button" class="btn-danger" id="btniluminacion" onclick="JavaScript: Iluminacion(this.value)
 	;" value="" >
 	</div>
 <div class="col-lg-3">
 </div>
 </div>
 </div>
 <br><br>
</section>

</body>
</html>
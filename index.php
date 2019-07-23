<?php
header("Refresh:6");

	function connect(){
		$server="localhost";
		$user="root";
		$pass="";
		$db="ioT";
	   	
		$connection = mysqli_connect($server, $user, $pass);

		if (!$connection) {
	    	die('MySQL ERROR: ' . mysqli_error());
		}
		
		mysqli_select_db($connection, $db) or die( 'MySQL ERROR: '. mysqli_error() );

		return $connection;
	}	
	
	   	
   	$link=connect();
	if(isset($_POST["occupied"]) && isset($_POST["sensorid"]) && isset($_POST["timestamp"]) ){
		$occupied = $_POST["occupied"];
		$sensorid = $_POST["sensorid"];
		date_default_timezone_set('GMT+2');
		$timestamp = date('m/d/Y h:i:s a', time()-$_POST["timestamp"]/1000);
		//$date = date('m/d/Y h:i:s a', time());
		$query = "INSERT INTO test (timestamp, occupied, sensorid) 
		VALUES ('".$timestamp."','".$occupied."','".$sensorid."')"; 	
		mysqli_query($link,$query);
	}
	if(isset($_POST["DE"])){
		mysqli_query($link,"DELETE FROM test;");
	}

	$result=mysqli_query($link, "SELECT * FROM test ORDER BY id DESC");

?>

<html>
   <head>
      <title>Sensor Data</title>
	  
	  
	  
	  <style>
table.blueTable {
  border: 1px solid #1C6EA4;
  background-color: #EEEEEE;
  width: 100%;
  text-align: left;
  border-collapse: collapse;
}
table.blueTable td, table.blueTable th {
  border: 1px solid #AAAAAA;
  padding: 3px 2px;
}
table.blueTable tbody td {
  font-size: 13px;
}
table.blueTable tr:nth-child(even) {
  background: #D0E4F5;
}
table.blueTable thead {
  background: #1C6EA4;
  background: -moz-linear-gradient(top, #5592bb 0%, #327cad 66%, #1C6EA4 100%);
  background: -webkit-linear-gradient(top, #5592bb 0%, #327cad 66%, #1C6EA4 100%);
  background: linear-gradient(to bottom, #5592bb 0%, #327cad 66%, #1C6EA4 100%);
  border-bottom: 2px solid #444444;
}
table.blueTable thead th {
  font-size: 15px;
  font-weight: bold;
  color: #FFFFFF;
  border-left: 2px solid #D0E4F5;
}
table.blueTable thead th:first-child {
  border-left: none;
}

table.blueTable tfoot {
  font-size: 14px;
  font-weight: bold;
  color: #FFFFFF;
  background: #D0E4F5;
  background: -moz-linear-gradient(top, #dcebf7 0%, #d4e6f6 66%, #D0E4F5 100%);
  background: -webkit-linear-gradient(top, #dcebf7 0%, #d4e6f6 66%, #D0E4F5 100%);
  background: linear-gradient(to bottom, #dcebf7 0%, #d4e6f6 66%, #D0E4F5 100%);
  border-top: 2px solid #444444;
}
table.blueTable tfoot td {
  font-size: 14px;
}
table.blueTable tfoot .links {
  text-align: right;
}
table.blueTable tfoot .links a{
  display: inline-block;
  background: #1C6EA4;
  color: #FFFFFF;
  padding: 2px 8px;
  border-radius: 5px;
}	  


#demoFont {
font-family: Georgia, serif;
font-size: 25px;
letter-spacing: 2px;
word-spacing: 2px;
color: #000000;
font-weight: normal;
text-decoration: none;
font-style: normal;
font-variant: normal;
text-transform: none;
}


.button {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}

	  
	  </style>
	  
   </head>
<body>
   <h1 id="demoFont">Button State Detector</h1>

	<form method="post" action="">
	<input type="submit" name="DE" value="Delete Everything!" class="button"/>
	</form>

   <table border="1" cellspacing="1" cellpadding="1" class="blueTable">
		<tr>
			<td>&nbsp;Timestamp&nbsp;</td>
			<td>&nbsp;Occupied?&nbsp;</td>
			<td>&nbsp;Sensor ID&nbsp;</td>
		</tr>

      <?php 
		  if($result!==FALSE){
		     while($row = mysqli_fetch_array($result)) {
		        printf("<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td></tr>", 
		           $row["timestamp"], $row["occupied"], $row["sensorid"]);
		     }
		     mysqli_free_result($result);
		     mysqli_close($link);
		  }
      ?>

   </table>
</body>
</html>

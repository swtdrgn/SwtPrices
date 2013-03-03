<?
require("include/template.php");
$json_output = array('success' => false);
if( is_auth() ){
	
	$note = sql_filter($_POST["note"],10000);
	
	$query = "";
	
	connect_db();
	
	$tdate = date("Y-m-d\TH:i:s");
	$exdate = date("Y-m-d",strtotime('+3 days'));
	
	for( $i=0;$i<$_POST["tradesize"];$i++ ){
		$query = "";
		if( strcmp($_POST["tradetype" . $i],"Gift") == 0 ){
			$item_type=1;
		}
		else{
			$item_type=0;
		}
		
		$query .= "INSERT INTO  `swtprices`.`steam_trade` (`creator` ,`hashkey` , `created` ,`expiration` ,`type` ,`item_name` ,`item_url` ,`item_type`) VALUES (";
		$query .= "'" . mysql_real_escape_string($_SESSION["steam64"]) . "','" . mysql_real_escape_string(sha1($_SESSION["steam64"])) . "','" . $tdate . "','" .$exdate . "','0',  '" . mysql_real_escape_string(urlencode($_POST["trade".$i])) . "',  '" . mysql_real_escape_string(urlencode($_POST["tradeimg".$i])) . "',  '" . $item_type . "'";
		$query .= ");";
		//echo $query . "<br/><br/>";
		mysql_query($query);

	}
	
	for( $i=0;$i<$_POST["wantsize"];$i++ ){
		$query = "";
		if( strcmp($_POST["wanttype" . $i],"Gift") == 0 ){
			$item_type=1;
		}
		else{
			$item_type=0;
		}
		
		$query .= "INSERT INTO  `swtprices`.`steam_trade` (`creator` ,`hashkey` ,`created` ,`expiration` ,`type` ,`item_name` ,`item_url` ,`item_type`) VALUES (";
		$query .= "'" . mysql_real_escape_string($_SESSION["steam64"]) . "','" . mysql_real_escape_string(sha1($_SESSION["steam64"])) . "','" . $tdate . "','" . $exdate . "','1',  '" . mysql_real_escape_string(urlencode($_POST["want".$i])) . "',  '" . mysql_real_escape_string(urlencode($_POST["wantimg".$i])) . "',  '" . $item_type . "'";
		$query .= ");";
		//echo $query . "<br/><br/>";
		mysql_query($query);

	}
	
	$query = "INSERT INTO  `swtprices`.`steam_trade` (`creator` ,`hashkey` ,`created` ,`expiration` ,`type` ,`note`) VALUES (";
	$query .= "'" . mysql_real_escape_string($_SESSION["steam64"]) . "','" . mysql_real_escape_string(sha1($_SESSION["steam64"])) . "','" . $tdate . "','" . $exdate . "','2',  '" . $note . "'";
	$query .= ");";
	//echo $query . "<br/><br/>";
	mysql_query($query);
	
	//mysql_query($query);
	mysql_close();
	$json_output["success"] = true;
}
echo json_encode($json_output);

?>
<?
	//header("Location: " . urldecode($_GET["addr"]));
	//header("Location: " . urldecode($_GET["addr"]));
	require("include/template.php");
	$_SESSION["steam64"] = "76561198040477363";
	$_SESSION["steamkey"] = swt_steam_hash($_SESSION["steam64"]);
	//$_SESSION["steamkey"] = "2089638a0dbbec02f63d494dbace3cc8837ddac0";
	$_SESSION["AuthSteam"] = true;
	$_SESSION["steam64"] = "76561198040477363";
	
	//header("Location: " . urldecode($_GET["addr"]));
	//print_r($_SESSION);
	
	//echo $_GET['openid_assoc_handle'] . "<br/>";
	//echo $_GET['openid_signed'] . "<br/>";
	//echo $_GET['openid_sig'];
	
	
	header("Location: " . urldecode($_GET["addr"]));
?>
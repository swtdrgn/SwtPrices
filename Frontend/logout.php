<?
	session_start();
	unset($_SESSION["steam64"]);
	unset($_SESSION["steamkey"]);
	unset($_SESSION["AuthSteam"]);
	
	header("Location: /index.html");
?>
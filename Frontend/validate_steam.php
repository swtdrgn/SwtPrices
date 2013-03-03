<?
	//header("Location: " . urldecode($_GET["addr"]));
	//header("Location: " . urldecode($_GET["addr"]));
	require("include/template.php");
	$_SESSION["steam64"] = SteamSignIn::validate();
	if( $_SESSION["steam64"] != "" ){
		//$_SESSION["steamkey1"] = rand_string(10);
		$_SESSION["steamkey"] = swt_steam_hash($_SESSION["steam64"],"");
		$_SESSION["AuthSteam"] = true;
		$xmlpage = "http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=A8D997CC1A0F7A48DFAF4C7EA5281DB5&steamids=" . $_SESSION["steam64"];
		//echo "session: " . session_start();
		//echo urldecode($_GET["addr"]);
	}
	header("Location: " . urldecode($_GET["addr"]));
	//print_r($_SESSION);
	
	//echo $_GET['openid_assoc_handle'] . "<br/>";
	//echo $_GET['openid_signed'] . "<br/>";
	//echo $_GET['openid_sig'];
	
?>
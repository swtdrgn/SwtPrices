<?php
session_start();

function include_header(){
	?>
	
	<?
}

function include_meta_content(){
	?>
	<head>
	<meta http-equiv="content-type" content="text/html; charset=UTF-8">

    <!--                                                               -->
    <!-- Consider inlining CSS to reduce the number of requested files -->
    <!--                                                               -->
    <link type="text/css" rel="stylesheet" href="SwtPrices.css">
	<script src="/include/common.js"></script>

    <!--                                           -->
    <!-- Any title is fine                         -->
    <!--                                           -->
    <title>swtPrices</title>
    
    <!--                                           -->
    <!-- This script loads your compiled module.   -->
    <!-- If you add any GWT meta tags, they must   -->
    <!-- be added before this line.                -->
    <!--                                           -->
    <!-- <script type="text/javascript" language="javascript" src="swtprices/swtprices.nocache.js"></script> -->
	</head>

	<!--                                           -->
	<!-- The body can have arbitrary html, or      -->
	<!-- you can leave the body empty if you want  -->
	<!-- to create a completely dynamic UI.        -->
	<!--                                           -->
	<?
}

function include_body_header(){
	?>

	<!-- OPTIONAL: include this if you want history support -->
    <!-- <iframe src="javascript:''" id="__gwt_historyFrame" tabIndex='-1' style="position:absolute;width:0;height:0;border:0"></iframe> -->
    
    <!-- RECOMMENDED if your web app will not function without JavaScript enabled -->
    <noscript>
      <div style="width: 22em; position: absolute; left: 50%; margin-left: -11em; color: red; background-color: white; border: 1px solid red; padding: 4px; font-family: sans-serif">
        Your web browser must have JavaScript enabled
        in order for this application to display correctly.
      </div>
    </noscript>
	
	<div class="header">
	
    <h1>swtPrices
		<? if( !isset($_SESSION["AuthSteam"]) ){
			/*
			<a href="<?= SteamSignIn::genUrl("http://prices.comli.com/validate_steam.php?addr=".urlencode($_SERVER["REQUEST_URI"]),false); ?>"><img style="float:right; margin-bottom: 10px;" src="http://cdn.steamcommunity.com/public/images/signinthroughsteam/sits_large_noborder.png"/></a>
			*/
		?>
			
			<a href="<?= SteamSignIn::genUrl("http://gameprices.swthome.net/validate_steam.php?addr=".urlencode($_SERVER["REQUEST_URI"]),false); ?>"><img style="float:right; margin-bottom: 10px;" src="http://cdn.steamcommunity.com/public/images/signinthroughsteam/sits_large_noborder.png"/></a>
			<a href="fakevalidate.php?addr=<?= urlencode($_SERVER["REQUEST_URI"]) ?>" style="float:right;"><span>[Log in as Simon]</span></a>
		<? }else{
			//echo '<div style="float:right;">'.$_SESSION["steam64"].'</div>';
			//echo "<div style=\"font-size: 13px;\">" . $_SESSION["steam64"] . "</div><br/>";
			//echo "<div style=\"font-size: 13px;\">" . $_SESSION["steamkey"] . "</div>";
			echo '<div style="float:right; font-size: 13px;"><a style="color: white;" href="logout.php">Logout</a></div>';
		}
		?>
	</h1>

		<form name="search_games" action="game_search.html" method="get" style="width:400px;">
		<input style="width: 300px; margin-right: 10px;" id="sendButtonContainer" type="text" name="query" autocomplete="off" ></input>
		<input type="submit" value="Search"></input>
		</form>
	<div>
		<div class="dropdown"><a class="nav" href="/index.html" >Home</a></div>
		<div class="dropdown" id="trade" onmouseover="drop_tab(this);" onmouseout="collapse_tab(this);">
			<a class="nav" href="/trade.html" >Trade</a>
			<div>
				<a class="firstdrop" href="/trade.html">Search Trade Offers</a>
				<a href="/mytrade.html">My Posted Trades</a>
				<a href="/myoffer.html">My Offers</a>
				<a class="lastdrop" href="/trade_proposal.html">Post Trade</a>
			</div>
		</div>
		<div class="dropdown"><a class="nav" href="/forum.html" >Forum</a></div>
		<div class="dropdown2" onmouseover="drop_tab(this);" onmouseout="collapse_tab(this);">
			<a class="nav" style="margin-right: 0px;" >Account</a>
			<div>
				<a class="firstdrop2 lastdrop" href="/inbox.html">Inbox</a>
			</div>
		</div>
	</div>
	
	</div>
	
	<!-- <div style="background-color: #000060; padding: 5px;"> -->
    
	<!-- </div> -->
    <?
}

function connect_db(){
	@mysql_connect();
	@mysql_select_db("swtprices");
}


/**
*
* @package Steam Community API
* @copyright (c) 2010 ichimonai.com
* @license http://opensource.org/licenses/mit-license.php The MIT License
*
*/

class SteamSignIn
{
	const STEAM_LOGIN = 'https://steamcommunity.com/openid/login';

	/**
	* Get the URL to sign into steam
	*
	* @param mixed returnTo URI to tell steam where to return, MUST BE THE FULL URI WITH THE PROTOCOL
	* @param bool useAmp Use &amp; in the URL, true; or just &, false. 
	* @return string The string to go in the URL
	*/
	public static function genUrl($returnTo = false, $useAmp = true)
	{
		$returnTo = (!$returnTo) ? (!empty($_SERVER['HTTPS']) ? 'https' : 'http') . '://' . $_SERVER['HTTP_HOST'] . $_SERVER['SCRIPT_NAME'] : $returnTo;
		
		$params = array(
			'openid.ns'			=> 'http://specs.openid.net/auth/2.0',
			'openid.mode'		=> 'checkid_setup',
			'openid.return_to'	=> $returnTo,
			'openid.realm'		=> (!empty($_SERVER['HTTPS']) ? 'https' : 'http') . '://' . $_SERVER['HTTP_HOST'],
			'openid.identity'	=> 'http://specs.openid.net/auth/2.0/identifier_select',
			'openid.claimed_id'	=> 'http://specs.openid.net/auth/2.0/identifier_select',
		);
		
		$sep = ($useAmp) ? '&amp;' : '&';
		return self::STEAM_LOGIN . '?' . http_build_query($params, '', $sep);
	}
	



	/**
	* Validate the incoming data
	*
	* @return string Returns the SteamID64 if successful or empty string on failure
	*/
	public static function validate()
	{
		// Star off with some basic params
		$params = array(
			'openid.assoc_handle'	=> $_GET['openid_assoc_handle'],
			'openid.signed'			=> $_GET['openid_signed'],
			'openid.sig'			=> $_GET['openid_sig'],
			'openid.ns'				=> 'http://specs.openid.net/auth/2.0',
		);
		
		// Get all the params that were sent back and resend them for validation
		$signed = explode(',', $_GET['openid_signed']);
		foreach($signed as $item)
		{
			$val = $_GET['openid_' . str_replace('.', '_', $item)];
			$params['openid.' . $item] = get_magic_quotes_gpc() ? stripslashes($val) : $val; 
		}

		// Finally, add the all important mode. 
		$params['openid.mode'] = 'check_authentication';
		
		// Stored to send a Content-Length header
		$data =  http_build_query($params);
		$context = stream_context_create(array(
			'http' => array(
				'method'  => 'POST',
				'header'  => 
					"Accept-language: en\r\n".
					"Content-type: application/x-www-form-urlencoded\r\n" .
					"Content-Length: " . strlen($data) . "\r\n",
				'content' => $data,
			),
		));

		$result = file_get_contents(self::STEAM_LOGIN, false, $context);
		
		// Validate wheather it's true and if we have a good ID
		preg_match("#^http://steamcommunity.com/openid/id/([0-9]{17,25})#", $_GET['openid_claimed_id'], $matches);
		$steamID64 = is_numeric($matches[1]) ? $matches[1] : 0;

		// Return our final value
		return preg_match("#is_valid\s*:\s*true#i", $result) == 1 ? $steamID64 : '';
	}
}

function rand_string( $length ) {
	$chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	$str = "";
	for( $i = 0; $i < $length; $i++ ) {
		$str .= $chars[ rand(0,63) ];
	}
	return $str;
}

function swt_steam_hash( $steam64, $random_key="" ){
	return hash("sha512","steam".$random_key.$steam64."swtprices");
}

function steam_auth( $steam64,$gen_key ){
	return strcmp($gen_key,swt_steam_hash($steam64)) == 0;
}

function is_auth( $redirect=false ){
	if( isset($_SESSION["steam64"]) && steam_auth($_SESSION["steam64"],$_SESSION["steamkey"]) ){
		return true;
	}
	else if($redirect){
		//echo SteamSignIn::genUrl("http://localhost/validate_steam.php?addr=".urlencode($_SERVER["REQUEST_URI"]),false);
		//header("Location: " . SteamSignIn::genUrl("http://games.localhost/validate_steam.php?addr=".urlencode($_SERVER["REQUEST_URI"]),false) );
		header("Location: " . SteamSignIn::genUrl("http://gameprices.swthome.net/validate_steam.php?addr=".urlencode($_SERVER["REQUEST_URI"]),false) );
	}
	return false;
}

function sql_filter( $text,$size ){
	if(strlen($text)>$size){
		return mysql_real_escape_string(urlencode(str_replace("\n","<br/>",str_replace("<","&lt;",substr($text,0,$size)))));
	}
	else{
		return mysql_real_escape_string(urlencode(str_replace("\n","<br/>",str_replace("<","&lt;",$text))));
	}
}
	
?>
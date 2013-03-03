function drop_tab( element ){
	var dropbox = element.getElementsByTagName("div");
	dropbox[0].style.display = "block";
}

function collapse_tab( element ){
	var dropbox = element.getElementsByTagName("div");
	dropbox[0].style.display = "none";
}
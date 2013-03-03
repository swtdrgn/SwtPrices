function toggle_bad_results() {
	//document.write(document.getElementsByClassName("bad_result")[0].style);

	//document.getElementsByClassName("bad_result").style.display = 'inline-block';
	
	var nodelist = document.getElementById("steam").getElementsByClassName("bad_result");
	var i;
	if( nodelist.length > 0 ){
		if( nodelist[0].style.display == 'inline-block' ){
			shown = true;
			for(i=0;i<nodelist.length;i++){
				nodelist[i].style.display = 'none';
			}
			return "Show All Changes";
		}
	}
	
	for(i=0;i<nodelist.length;i++){
		nodelist[i].style.display = 'inline-block';
	}
	
	return "Show Some Changes";
}

function hide_all_bad_results() {
	var nodelist = getElementsByClassName("bad_result");
	var i;
	for(i=0;i<nodelist.length;i++){
		nodelist[i].style.display = 'none';
	}
}
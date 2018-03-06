function toggleVisibility(eClass) {
    var x = document.getElementsByClassName(eClass);
	for(var i = 0; i<x.length; i++){
		if (x[i].classList.contains("hide")) {
			x[i].classList.remove("hide");
			x[i].classList.add("show");
		} else {
			x[i].classList.remove("show");
			x[i].classList.add("hide");
		}
	}
    
}